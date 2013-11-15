#include "config.hpp"
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

#include "Adc.hpp"


inline void setupPorts(void)
{
  DDRB |=  _BV(0);        // PB3 as output (IR and RED LEDs)
  DDRB |=  _BV(1);        // PB4 as output (main light)
}


inline void bit(const uint8_t b, const bool on)
{
  if(on) PORTB |=  _BV(b);
  else   PORTB &= ~_BV(b);
}
inline void ctrlLed(const bool on) { bit(0, on); }
inline void light(const bool on)   { bit(1, not on); }


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
  Adc adc;
  // init program
  setupPorts();
  //setupAdc();

  // initial state
  ctrlLed(false);
  light(false);

  // TODO                                               
  for(;;)
  {
    _delay_ms(1000);
    ctrlLed(true);
    _delay_ms(1000);
    ctrlLed(false);
    //...           
    adc.irVoltage();
    adc.vccVoltage();
  }
  // TODO                                               

  /*
  uint8_t  read[2] = { adc.irVoltage(), adc.irVoltage() };
  uint8_t  index   = 0;
  uint16_t onLeft  = 2*20;              // light up at start for 2[s]

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
      onLeft = 42*20;                   // 42[s]
    }

    index       = index==0 ? 1 : 0;     // change index to oposite
    read[index] = irLight();            // perform reading
  }
  */

  // program never reaches here
  return 0;
}
