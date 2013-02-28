#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

volatile uint8_t XXX=0;             

ISR(TIM0_OVF_vect)
{
XXX++;              
  // nothing to be done here - just wake up the CPU
}

ISR(TIM0_COMPA_vect)
{
XXX++;              
  // nothing to be done here - just wake up the CPU
}

ISR(TIM0_COMPB_vect)
{
XXX++;              
  // nothing to be done here - just wake up the CPU
}


inline void setupInterrupts(void)
{
  // setup operation mode
  TCCR0A |= _BV(WGM01); // 010 - CTC mode
  // setup frequency fout=(fclk)/(2*preskaler*ocr0a)
  TCCR0A |= _BV(CS01);  // set preskaler to 010, that is N=8
  OCR0A   = 75;         // proportional value
  // enable interrupts
  TIFR0  |= _BV(OCF0A); // interrupt when counter reaches TOP
  TIMSK0 |= _BV(OCIE0A);// enable this interrupt
  //TIMSK0 |= _BV(OCIE0B);
  //TIMSK0 |= _BV(TOIE0);
  sei();                // globally enable interrupts
  // TODO
}


inline void setupAdc(void)
{
  ADMUX &=~_BV(REFS0);  // Vref=Vcc
  ADMUX |= _BV(ADLAR);  // left-adjust result, so that ADCH will contain full result
  ADMUX &=~_BV(MUX1);   // select ADC1
  ADMUX |= _BV(MUX0);   // ...
  ADCSRA&=~_BV(ADPS2);  // set prescaler to 011 (N=8), which is 150kHz
  ADCSRA|= _BV(ADPS1);  // ...
  ADCSRA|= _BV(ADPS0);  // ...
}


inline void setupPorts(void)
{
  DDRB&=~_BV(2);        // PB2 as input (phototransistor; ADC1)
  DDRB|= _BV(3);        // PB3 as output (IR and RED LEDs)
  DDRB|= _BV(4);        // PB4 as output (main light)
}


inline void bit(const uint8_t b, bool on)
{
  if(on)
    PORTB|= _BV(b);
  else
    PORTB&=~_BV(b);
}
inline void ctrlLed(bool on) { bit(3, on); }
inline void light(bool on)   { bit(4, on); }


uint8_t readAdc(void)
{
  ADCSRA|=_BV(ADSC);                // start convertion
  while( ADCSRA & _BV(ADSC) ) {}    // wait for conversion to finish
  return ADCH;                      // return the result
}

inline uint8_t irLight(void)
{
  ADCSRA|= _BV(ADEN);               // enable ADC
  readAdc();                        // discard first reading
  const uint8_t adc = readAdc();    // this one is reasonable
  ADCSRA|= _BV(ADEN);               // disable ADC
  // compute actual voltage, multiplied by 10, to avoid floating point emulation
  constexpr uint8_t vref = 5;                       // reference voltage
  const uint16_t    vin  = (adc*(vref*10l))/1024l;  // conversion equation
  return vin;
}


template<typename T>
T distance(const T a, const T b)
{
  if(a<b)
    return b-a;
  return a-b;
}


//
// MAIN PROGRAM
//
int main(void)
{
  // init program
  setupPorts();
  setupAdc();
  setupInterrupts();

  ctrlLed(true);
  light(false);
  uint8_t  index   = 0;
  uint8_t  read[2] = { irLight(), irLight() };
  uint16_t onLeft  = 0;
  // main loop
  for(;;)
  {
    _delay_ms(50);

    if(onLeft>0)
      --onLeft;
    else
      light(false);

    if( distance(read[0], read[1]) > 1 )
    {
      light(true);
      onLeft =  1*20;                   // 46[s]
    }

    index       = index==0 ? 1 : 0;     // change index to oposite
    read[index] = irLight();            // perform reading
  }

  // program never reaches here
  return 0;
}
