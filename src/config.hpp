/*
 * config.hpp
 *
 */
#ifndef INCLUDE_CONFIG_HPP_FILE
#define INCLUDE_CONFIG_HPP_FILE

#include <inttypes.h>

// units commonly used
constexpr uint32_t KHz=1000L;
constexpr uint32_t MHz=1000L*KHz;

// clock frequency in [Hz] (needs to be a define...)
#define F_CPU (1*MHz+200*KHz)

#endif
