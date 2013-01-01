/* -*- c++ -*- */
/*
 * Copyright 2012 Dimitri Stolnikov <horiz0n@gmx.net>
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <fstream>
#include <string>
#include <sstream>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

#include <gr_io_signature.h>
#include <gr_deinterleave.h>
#include <gr_float_to_complex.h>

#include "rtl_tcp_source_c.h"

#include <osmosdr_arg_helpers.h>

using namespace boost::assign;

static std::string get_tuner_name( enum rtlsdr_tuner tuner_type )
{
  if ( RTLSDR_TUNER_E4000 == tuner_type )
    return "E4000";
  else if ( RTLSDR_TUNER_FC0012 == tuner_type )
    return "FC0012";
  else if ( RTLSDR_TUNER_FC0013 == tuner_type )
    return "FC0013";
  else if ( RTLSDR_TUNER_FC2580 == tuner_type )
    return "FC2580";
  else if ( RTLSDR_TUNER_R820T == tuner_type )
    return "R820T";
  else
    return "Unknown";
}

rtl_tcp_source_c_sptr make_rtl_tcp_source_c(const std::string &args)
{
  return gnuradio::get_initial_sptr(new rtl_tcp_source_c(args));
}

rtl_tcp_source_c::rtl_tcp_source_c(const std::string &args) :
  gr_hier_block2("rtl_tcp_source_c",
                 gr_make_io_signature (0, 0, 0),
                 gr_make_io_signature (1, 1, sizeof (gr_complex)))
{
  std::string host = "127.0.0.1";
  unsigned short port = 1234;
  int payload_size = 16384;

  _freq = 0;
  _rate = 0;
  _gain = 0;
  _corr = 0;
  _auto_gain = false;

  dict_t dict = params_to_dict(args);

  if (dict.count("rtl_tcp")) {
    std::vector< std::string > tokens;
    boost::algorithm::split( tokens, dict["rtl_tcp"], boost::is_any_of(":") );

    if ( tokens[0].length() && (tokens.size() == 1 || tokens.size() == 2 ) )
      host = tokens[0];

    if ( tokens.size() == 2 ) // port given
      port = boost::lexical_cast< unsigned short >( tokens[1] );
  }

  if (dict.count("psize"))
    payload_size = boost::lexical_cast< int >( dict["psize"] );

  if (!host.length())
    host = "127.0.0.1";

  if (0 == port)
    port = 1234;

  if (payload_size <= 0)
    payload_size = 16384;

  _src = make_rtl_tcp_source_f(sizeof(float), host.c_str(), port, payload_size);

  if ( _src->get_tuner_type() != RTLSDR_TUNER_UNKNOWN )
  {
    std::cerr << "The RTL TCP server reports a "
              << get_tuner_name( _src->get_tuner_type() )
              << " tuner with " << _src->get_tuner_gain_count()
              << " selectable gains."
              << std::endl;
  }

  set_gain_mode(false); /* enable manual gain mode by default */

  /* rtl tcp source provides a stream of interleaved IQ floats */
  gr_deinterleave_sptr deinterleave = gr_make_deinterleave(sizeof(float));

  /* block to convert deinterleaved floats to a complex stream */
  gr_float_to_complex_sptr f2c = gr_make_float_to_complex(1);

  connect(_src, 0, deinterleave, 0);
  connect(deinterleave, 0, f2c, 0); /* I */
  connect(deinterleave, 1, f2c, 1); /* Q */
  connect(f2c, 0, self(), 0);
}

rtl_tcp_source_c::~rtl_tcp_source_c()
{
}

gr_basic_block_sptr rtl_tcp_source_c::self()
{
  return gr_hier_block2::self();
}

std::string rtl_tcp_source_c::name()
{
  return "RTL TCP Client Source";
}

size_t rtl_tcp_source_c::get_num_channels( void )
{
  return 1;
}

osmosdr::meta_range_t rtl_tcp_source_c::get_sample_rates( void )
{
  osmosdr::meta_range_t range;

  range += osmosdr::range_t( 250000 ); // known to work
  range += osmosdr::range_t( 1000000 ); // known to work
  range += osmosdr::range_t( 1024000 ); // known to work
  range += osmosdr::range_t( 1800000 ); // known to work
  range += osmosdr::range_t( 1920000 ); // known to work
  range += osmosdr::range_t( 2000000 ); // known to work
  range += osmosdr::range_t( 2048000 ); // known to work
  range += osmosdr::range_t( 2400000 ); // known to work
//  range += osmosdr::range_t( 2600000 ); // may work
//  range += osmosdr::range_t( 2800000 ); // may work
//  range += osmosdr::range_t( 3000000 ); // may work
//  range += osmosdr::range_t( 3200000 ); // max rate

  return range;
}

double rtl_tcp_source_c::set_sample_rate( double rate )
{
  _src->set_sample_rate( int(rate) );

  _rate = rate;

  return get_sample_rate();
}

double rtl_tcp_source_c::get_sample_rate( void )
{
  return _rate;
}

osmosdr::freq_range_t rtl_tcp_source_c::get_freq_range( size_t chan )
{
  osmosdr::freq_range_t range;

  enum rtlsdr_tuner tuner = _src->get_tuner_type();

  if ( tuner == RTLSDR_TUNER_E4000 ) {
    /* there is a (temperature dependent) gap between 1100 to 1250 MHz */
    range += osmosdr::range_t( 52e6, 2.2e9 );
  } else if ( tuner == RTLSDR_TUNER_FC0012 ) {
    range += osmosdr::range_t( 22e6, 948e6 );
  } else if ( tuner == RTLSDR_TUNER_FC0013 ) {
    range += osmosdr::range_t( 22e6, 1.1e9 );
  } else if ( tuner == RTLSDR_TUNER_FC2580 ) {
    range += osmosdr::range_t( 146e6, 308e6 );
    range += osmosdr::range_t( 438e6, 924e6 );
  } else if ( tuner == RTLSDR_TUNER_R820T ) {
    range += osmosdr::range_t( 24e6, 1766e6 );
  } else {
    range += osmosdr::range_t( 52e6, 2.2e9 ); // assume E4000 tuner
  }

  return range;
}

double rtl_tcp_source_c::set_center_freq( double freq, size_t chan )
{
  _src->set_freq( int(freq) );

  _freq = freq;

  return get_center_freq(chan);
}

double rtl_tcp_source_c::get_center_freq( size_t chan )
{
  return _freq;
}

double rtl_tcp_source_c::set_freq_corr( double ppm, size_t chan )
{
  _src->set_freq_corr( int(ppm) );

  _corr = ppm;

  return get_freq_corr( chan );
}

double rtl_tcp_source_c::get_freq_corr( size_t chan )
{
  return _corr;
}

std::vector<std::string> rtl_tcp_source_c::get_gain_names( size_t chan )
{
  std::vector< std::string > names;

  names += "LNA";

  return names;
}

osmosdr::gain_range_t rtl_tcp_source_c::get_gain_range( size_t chan )
{
  osmosdr::gain_range_t range;

  /* the following gain values have been copied from librtlsdr */

  /* all gain values are expressed in tenths of a dB */
  const int e4k_gains[] = { -10, 15, 40, 65, 90, 115, 140, 165, 190, 215,
                            240, 290, 340, 420 };
  const int fc0012_gains[] = { -99, -40, 71, 179, 192 };
  const int fc0013_gains[] = { -99, -73, -65, -63, -60, -58, -54, 58, 61,
                               63, 65, 67, 68, 70, 71, 179, 181, 182,
                               184, 186, 188, 191, 197 };
  const int fc2580_gains[] = { 0 /* no gain values */ };
  const int r820t_gains[] = { 0, 9, 14, 27, 37, 77, 87, 125, 144, 157,
                              166, 197, 207, 229, 254, 280, 297, 328,
                              338, 364, 372, 386, 402, 421, 434, 439,
                              445, 480, 496 };
  const int unknown_gains[] = { 0 /* no gain values */ };

  const int *ptr = NULL;
  int len = 0;

  switch (_src->get_tuner_type())
  {
  case RTLSDR_TUNER_E4000:
    ptr = e4k_gains; len = sizeof(e4k_gains);
    break;
  case RTLSDR_TUNER_FC0012:
    ptr = fc0012_gains; len = sizeof(fc0012_gains);
    break;
  case RTLSDR_TUNER_FC0013:
    ptr = fc0013_gains; len = sizeof(fc0013_gains);
    break;
  case RTLSDR_TUNER_FC2580:
    ptr = fc2580_gains; len = sizeof(fc2580_gains);
    break;
  case RTLSDR_TUNER_R820T:
    ptr = r820t_gains; len = sizeof(r820t_gains);
    break;
  default:
    ptr = unknown_gains; len = sizeof(unknown_gains);
    break;
  }

  if ( ptr != NULL && len > 0 )
  {
    for (int i = 0; i < int(len / sizeof(int)); i++)
      range += osmosdr::range_t( ptr[i] / 10.0f );
  }

  return range;
}

osmosdr::gain_range_t rtl_tcp_source_c::get_gain_range( const std::string & name, size_t chan )
{
  return get_gain_range( chan );
}

bool rtl_tcp_source_c::set_gain_mode( bool automatic, size_t chan )
{
  _src->set_gain_mode(int(!automatic));

  _auto_gain = automatic;

  return get_gain_mode(chan);
}

bool rtl_tcp_source_c::get_gain_mode( size_t chan )
{
  return _auto_gain;
}

double rtl_tcp_source_c::set_gain( double gain, size_t chan )
{
  osmosdr::gain_range_t gains = rtl_tcp_source_c::get_gain_range( chan );

  _src->set_gain( int(gains.clip(gain) * 10.0) );

  _gain = gain;

  return get_gain(chan);
}

double rtl_tcp_source_c::set_gain( double gain, const std::string & name, size_t chan )
{
  return set_gain(chan);
}

double rtl_tcp_source_c::get_gain( size_t chan )
{
  return 0;
}

double rtl_tcp_source_c::get_gain( const std::string & name, size_t chan )
{
  return get_gain(chan);
}

std::vector< std::string > rtl_tcp_source_c::get_antennas( size_t chan )
{
  std::vector< std::string > antennas;

  antennas += get_antenna(chan);

  return antennas;
}

std::string rtl_tcp_source_c::set_antenna( const std::string & antenna, size_t chan )
{
  return get_antenna(chan);
}

std::string rtl_tcp_source_c::get_antenna( size_t chan )
{
  return "ANT";
}
