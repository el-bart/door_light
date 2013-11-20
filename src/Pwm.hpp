#ifndef INCLUDE_PWM_HPP_FILE
#define INCLUDE_PWM_HPP_FILE

#include "config.hpp"
#include <avr/io.h>
#include <inttypes.h>

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
    TCCR0A |=  _BV(WGM01)|_BV(WGM00);   // set fast pwm mode
    TCCR0B &= ~(_BV(FOC0A)|_BV(FOC0B)); // required by the spec, though match with default values
    // set prescaler to N; (9.6*10^6/8)/(N*256)
    TCCR0B |= _BV(CS01);                // N=8 => 585.9375[Hz] ~= 600[Hz]
    // set OC0x work modes
    enableA(false);
    enableB(false);
    // enable interrupts from T0
    TIMSK0 |= _BV(TOIE0);               // enable T0 interrupts
  }

  void enableA(const bool e)
  {
    // clear OC0A on match, set on TOP (n-mosfet).
    enableImpl(e, _BV(COM0A1)|_BV(COM0A0), _BV(COM0A1));
  }
  void enableB(const bool e)
  {
    // clear OC0A on match, set on TOP (n-mosfet).
    enableImpl(e, _BV(COM0B1)|_BV(COM0B0), _BV(COM0B1));
  }

  void fillA(const uint8_t fill) { OCR0A = fill; }
  void fillB(const uint8_t fill) { OCR0B = fill; }

  constexpr static uint16_t frequency(void) { return 586; }

private:
  void enableImpl(const bool enable, const uint8_t bits, const uint8_t set)
  {
    TCCR0A &= ~bits;
    if(enable)
      TCCR0A |= set;
  }
};

#endif
