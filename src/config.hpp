/*
 * config.hpp
 *
 */
#ifndef INCLUDE_CONFIG_HPP_FILE
#define INCLUDE_CONFIG_HPP_FILE

#include <inttypes.h>
#include <avr/io.h>

// units commonly used
constexpr uint32_t kHz=1000L;
constexpr uint32_t MHz=1000L*kHz;

// clock frequency (9.6MHz with preskaler set to 8)
constexpr uint32_t fcpu = (9*MHz+600*kHz)/8;
// needs to be a define as well...
#define F_CPU fcpu

// bits on port definitions
namespace Pin
{
constexpr auto ctrlLed = _BV(PB0);  // control and IR LEDs
constexpr auto light   = _BV(PB1);  // main light
constexpr auto irIn    = _BV(PB3);  // IR phototransistor voltage monitoring input
constexpr auto vccIn   = _BV(PB4);  // supply voltage monitoring input
}

// some thresholds
namespace Thresold
{
constexpr uint16_t minVcc = 3400;   // minimum operational voltage (3.4[V]);
constexpr uint16_t irDiff = 100;    // voltage difference to threat as a trigger (in millivolts)
}

// misc constants
constexpr uint8_t irSamples = 6;    // number of samples to collect for thresholding
constexpr uint8_t irFill    = 50;   // fill level of the PWM for Ctrl setting; 0-255

#endif
