#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#include "Adc.hpp"
#include "Pwm.hpp"
#include "Sampler.hpp"
#include "LedCtrl.hpp"
#include "LedLight.hpp"
#include "PowerSave.hpp"
#include "LowPowerHandler.hpp"

template<typename T>
T distance(const T a, const T b)
{
  if(a<b)
    return b-a;
  return a-b;
}


void wait(uint16_t ms)
{
  for(uint16_t i=0; i<ms; ++i)
    _delay_ms(1);
}


using AdcSampler = Sampler<Millivolts, irSamples>;

//
// MAIN PROGRAM
//
int main(void)
{
  Pwm        pwm;
  LedCtrl    ir(pwm);
  LedLight   light(pwm);
  Adc        adc;
  AdcSampler sampler(0);
  sei();                    // enable interrupts globaly

  //
  // infinite system loop
  //
  uint16_t cycle = 0;       // cycles counter
  while(true)
  {
    //const auto voltIr = adc.irVoltage();
    // TODO

    ++cycle;
    // save power until next interrupt from the timer
    PowerSave::idle();
  }

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
  for(;;) { }
}
