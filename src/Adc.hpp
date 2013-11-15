#ifndef INCLUDE_ADC_HPP_FILE
#define INCLUDE_ADC_HPP_FILE

#include "config.hpp"
#include <avr/io.h>
#include <inttypes.h>

/** @brief ADC wrapper, with proper computations.
 */
class Adc
{
public:
  typedef uint16_t Millivolts;

  /** @brief initialize ADC.
   */
  Adc(void)
  {
    // ADC setup
    ADMUX  |=  _BV(REFS0);  // Vref=internal voltage reference (1.1V)
    ADMUX  &= ~_BV(ADLAR);  // right-adjust result, so that ADCH<<8|ADCL will contain full result
    ADCSRA &= ~_BV(ADPS2);  // set prescaler to 011 (N=8), which is 150kHz
    ADCSRA |=  _BV(ADPS1);  // ...
    ADCSRA |=  _BV(ADPS0);  // ...
    // note: no channel selection - this will be done when actually asking for data

    // ports setup
    constexpr auto ins = Pin::irIn | Pin::vccIn;    // use both ADC ports
    DDRB  &= ~ins;                                  // inputs
    PORTB &= ~ins;                                  // w/o pull-up resistors
  }

  Millivolts irVoltage(void)
  {
    setInput(Pin::irIn);
    return voltageOnCurrentPin();
  }

  Millivolts vccVoltage(void)
  {
    setInput(Pin::vccIn);
    return voltageOnCurrentPin();
  }

private:
  void setInput(const uint8_t input)
  {
    switch(input)
    {
      // 00 -> ADC0
      case _BV(PB5):
           ADMUX &= ~_BV(MUX1);
           ADMUX &= ~_BV(MUX0);
           break;
      // 01 -> ADC1
      case _BV(PB2):
           ADMUX &= ~_BV(MUX1);
           ADMUX |=  _BV(MUX0);
           break;
      // 10 -> ADC2
      case _BV(PB4):
           ADMUX |=  _BV(MUX1);
           ADMUX &= ~_BV(MUX0);
           break;
      // 11 -> ADC3
      case _BV(PB3):
           ADMUX |=  _BV(MUX1);
           ADMUX |=  _BV(MUX0);
           break;
    }
  }

  Millivolts voltageOnCurrentPin(void)
  {
    ADCSRA |= _BV(ADEN);        // enable ADC
    readAdc();                  // discard first reading
    const auto adc = readAdc(); // this one is reasonable
    ADCSRA |= _BV(ADEN);        // disable ADC
    return scale(adc);          // compute actual voltage, based on the measurement
  }

  uint16_t readAdc(void)
  {
    ADCSRA |= _BV(ADSC);            // start convertion
    while( ADCSRA & _BV(ADSC) ) {}  // wait for conversion to finish
    const auto     low  = ADCL;     // read low byte first - this blocks overrides
    const uint16_t high = ADCH;     // read high byte - anble register writing again
    return {(high<<8u)|low};        // do explicit {} to ensure no implicit conversions
  }

  Millivolts scale(uint16_t in)
  {
    in &= uint16_t{0xFFFC};                             // drop 2 least signifficant bits as a noise
    // compute actual voltage in millivolts.
    // general equation is: ADC=Vin*1024/Vref
    // this we get:         Vin=ADC*Vref/1024
    constexpr uint32_t vref{1100};                      // reference voltage is 1.1[V]
    const auto         vin = (in*vref)/uint16_t{1024};  // lossless computation result
    return static_cast<Millivolts>(vin);                // allow truncation of some bits (note: shold never happen, due to ranges)
  }
};

#endif
