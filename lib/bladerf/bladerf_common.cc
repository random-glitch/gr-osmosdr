/* -*- c++ -*- */
/*
 * Copyright 2013 Nuand LLC
 * Copyright 2013 Dimitri Stolnikov <horiz0n@gmx.net>
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

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

#include "bladerf_common.h"

#define BLADERF_FIFO_SIZE_ENV   "BLADERF_SAMPLE_FIFO_SIZE"

using namespace boost::assign;

bladerf_common::bladerf_common() : running(true)
{
  const char *env_fifo_size;
  size_t fifo_size;

  /* 1 Sample = i,q (2 int16_t's) */
  this->raw_sample_buf = new int16_t[2 * BLADERF_SAMPLE_BLOCK_SIZE];
  if (!raw_sample_buf) {
    throw std::runtime_error( std::string(__FUNCTION__) +
                              " has failed to allocate a raw sample buffer!" );
  }

  env_fifo_size = getenv(BLADERF_FIFO_SIZE_ENV);
  fifo_size = BLADERF_SAMPLE_FIFO_SIZE;

  if (env_fifo_size != NULL) {
    try {
      fifo_size = boost::lexical_cast<size_t>(env_fifo_size);
    } catch (const boost::bad_lexical_cast &e) {
      std::cerr << "Warning: \"" << BLADERF_FIFO_SIZE_ENV
                << "\" is invalid" << "... defaulting to "
                << fifo_size;
    }

    if (fifo_size < BLADERF_SAMPLE_FIFO_MIN_SIZE) {
      fifo_size = BLADERF_SAMPLE_FIFO_MIN_SIZE;
      std::cerr << "Warning: \"" << BLADERF_FIFO_SIZE_ENV
                << "\" is too small" << "... defaulting to "
                << BLADERF_SAMPLE_FIFO_MIN_SIZE;
    }
  }

  this->sample_fifo = new boost::circular_buffer<gr_complex>(fifo_size);
  if (!this->sample_fifo)
    throw std::runtime_error( std::string(__FUNCTION__) +
                              " has failed to allocate a sample FIFO!" );
}

bladerf_common::~bladerf_common()
{
  delete[] this->raw_sample_buf;
  delete this->sample_fifo;
}

osmosdr::freq_range_t bladerf_common::freq_range()
{
  /* assuming the same for RX & TX */
  return osmosdr::freq_range_t( 300e6, 3.8e9 );
}

osmosdr::meta_range_t bladerf_common::sample_rates()
{
  /* assuming the same for RX & TX */
  return osmosdr::meta_range_t( 160e3, 40e6, 1e6 );
}

osmosdr::freq_range_t bladerf_common::filter_bandwidths()
{
  /* the same for RX & TX according to the datasheet */
  osmosdr::freq_range_t bandwidths;

  std::vector<double> half_bandwidths;
  half_bandwidths += \
      0.75, 0.875, 1.25, 1.375, 1.5, 1.92, 2.5,
      2.75, 3, 3.5, 4.375, 5, 6, 7, 10, 14;

  BOOST_FOREACH( double half_bws, half_bandwidths )
    bandwidths += osmosdr::range_t( half_bws * 2.e6 );

  return bandwidths;
}

std::vector< std::string > bladerf_common::devices()
{
  struct ::bladerf_devinfo *devices;
  ssize_t n_devices;
  std::vector< std::string > ret;

  n_devices = bladerf_get_device_list(&devices);

  if (n_devices > 0) {
    for (ssize_t i = 0; i < n_devices; i++)  {

      std::stringstream s;
      std::string dev(devices[i].path);

      s << "bladerf=" << dev.substr(dev.find_first_of("01234567890")) << ","
        << "label='nuand bladeRF SN " << std::setfill('0') << std::setw(16)
        << devices[i].serial << "'";

      ret.push_back(s.str());
    }

    bladerf_free_device_list(devices, n_devices);
  }

  return ret;
}

bool bladerf_common::is_running()
{
  boost::shared_lock<boost::shared_mutex> lock(this->state_lock);
  return this->running;
}

void bladerf_common::set_running(bool is_running)
{
  boost::unique_lock<boost::shared_mutex> lock(this->state_lock);
  this->running = is_running;
}
