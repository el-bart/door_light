/*
 * config.hpp
 *
 */
#ifndef INCLUDE_CONFIG_HPP_FILE
#define INCLUDE_CONFIG_HPP_FILE

#include <inttypes.h>

// units commonly used
constexpr uint32_t kHz=1000L;
constexpr uint32_t MHz=1000L*kHz;

// clock frequency (9.6MHz with preskaler set to 8)
constexpr uint32_t g_fcpu = (9*MHz+600*kHz)/8;
// needs to be a define as well...
#define F_CPU g_fcpu

#endif
