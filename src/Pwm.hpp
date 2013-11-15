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
    // set prescaler to N; (9.6*10^6/8)/(N*256)
    TCCR0B |= CS01|CS01;        // N=8 => 585.9375[Hz] ~= 600[Hz]
    // set OC0x work mode
    enableA(false);
    enableB(false);
    // enable interrupts from T0
    TIMSK0 |= TOIE0;            // enable T0 interrupts
  }

  void enableA(const bool e)
  {
    // clear OC0A on match, set on TOP.
    enableImpl(e, COM0A1|COM0A0, COM0A1);
  }
  void enableB(const bool e)
  {
    // clear OC0B on match, set on TOP.
    enableImpl(e, COM0B1|COM0B0, COM0B1);
  }

  void fillA(const uint8_t fill) { OCR0A = fill; }
  void fillB(const uint8_t fill) { OCR0B = fill; }

private:
  void enableImpl(const bool enable, const uint8_t bits, const uint8_t set)
  {
    TCCR0A &= ~bits;
    if(enable)
      TCCR0A |= set;
  }
};

#endif
