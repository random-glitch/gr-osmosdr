#ifndef CONFIG_H_IN
#define CONFIG_H_IN

#define GR_OSMOSDR_VERSION "0.1.5"
#define GR_OSMOSDR_LIBVER "0.1.5"

/* #undef ENABLE_OSMOSDR */
#define ENABLE_FCD
#define ENABLE_FILE
/* #undef ENABLE_RTL */
#define ENABLE_RTL_TCP
/* #undef ENABLE_UHD */
#define ENABLE_IIO
/* #undef ENABLE_MIRI */
/* #undef ENABLE_SDRPLAY */
/* #undef ENABLE_HACKRF */
/* #undef ENABLE_BLADERF */
#define ENABLE_RFSPACE
/* #undef ENABLE_AIRSPY */
/* #undef ENABLE_SOAPY */
#define ENABLE_REDPITAYA
/* #undef ENABLE_FREESRP */

//provide NAN define for MSVC older than VC12
#if defined(_MSC_VER) && (_MSC_VER < 1800)
#include <limits>
#define NAN std::numeric_limits<double>::quiet_NaN()
#endif

#endif // CONFIG_H_IN
