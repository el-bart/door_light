#ifndef INCLUDE_PWM_HPP_FILE
#define INCLUDE_PWM_HPP_FILE

#include "config.hpp"
#include <avr/io.h>

/** @brief basic PWM controller.
 */
class Pwm
{
public:
  Pwm(void)
  {
    // sanity checks
    static_assert(Pin::ctrlLed == _BV(PB0), "PWM0 set to unknown channel");
    static_assert(Pin::light   == _BV(PB1), "PWM1 set to unknown channel");
    // set waveform generation mode
    TCCR0A |=  WGM01|WGM00;     // set fast pwm mode
    TCCR0B &= ~(FOC0A|FOC0B);   // required by the spec, though match with default values
    // set prescaler to N=8: (9.6*10^6/8)/(N*256) == 585.9375 ~= 600[Hz]
    TCCR0B |= CS01|CS01;        // N=8
    // set OC0x work mode
    TCCR0A |= COM0A1;           // clear OC0A on match, set on TOP. // NOTE: no PWM for ctrl light
    TCCR0A |= COM0B1;           // clear OC0B on match, set on TOP.
    // enable interrupts from T0
    TIMSK0 |= TOIE0;            // enable T0 interrupts
  }
};

#endif
