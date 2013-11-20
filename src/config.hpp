/*
 * config.hpp
 *
 */
#ifndef INCLUDE_CONFIG_HPP_FILE
#define INCLUDE_CONFIG_HPP_FILE

#include <inttypes.h>
#include <avr/io.h>

//
// basic constants
//

// units commonly used
constexpr uint32_t kHz=1000L;
constexpr uint32_t MHz=1000L*kHz;

// clock frequency (9.6MHz with preskaler set to 8)
constexpr uint32_t fcpu = (9*MHz+600*kHz)/8;
// needs to be a define as well...
#define F_CPU fcpu

//
// common types
//
using Millivolts = uint16_t;

//
// misc settings
//

// bits on port definitions
namespace Pin
{
constexpr auto ctrlLed = _BV(PB0);  // control and IR LEDs
constexpr auto light   = _BV(PB1);  // main light
constexpr auto irIn    = _BV(PB3);  // IR phototransistor voltage monitoring input
constexpr auto vccIn   = _BV(PB4);  // supply voltage monitoring input
}

// light-related settings
namespace Light
{
constexpr uint8_t irSamples  = 4;       // number of samples to collect for thresholding
constexpr uint8_t irFill     = 50;      // fill level of the PWM for Ctrl setting; 0-255

constexpr uint8_t lightFill  = 0xFF;    // PWM fill for main light DC/DC: 0-255

// TODO: when main light brightness can be controlled, update following 3 constants to 1, 36, 5 respectively:
constexpr uint8_t dimIn      =  1;      // number of seconds dim-in should last
constexpr uint8_t lightOn    =  15;     // number of seconds light should remain fully on
constexpr uint8_t dimOut     =  1;      // number of seconds dim-out should last
constexpr uint8_t lightCycle = dimIn + lightOn + dimOut;    // total light cycle length
}

// basic thresholds
namespace Voltage
{
constexpr uint8_t    maxVccThresholdBreach =    3;  // maxumu number of times minimal Vcc threshold can be breached before going down.
constexpr Millivolts minVcc                = 3400;  // minimum operational voltage (3.4[V]);
constexpr Millivolts irThreshold           =  150;  // voltage difference to threat as a trigger (in millivolts)
}

#endif
