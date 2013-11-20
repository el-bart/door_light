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
#include "DimHandler.hpp"

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


using AdcSampler = Sampler<Millivolts, Light::irSamples>;

//
// MAIN PROGRAM
//
int main(void)
{
  Pwm        pwm;
  LedCtrl    ir(pwm);
  LedLight   light(pwm);
  DimHandler dim(light);
  Adc        adc;
  AdcSampler sampler(0);   // this will force turn-lights-on on reset
  sei();

  //
  // infinite system loop
  //
  uint16_t pwmCycles = 0;
  while(true)
  {
    // grab ADC measurement ASAP, since PWM might have low fill
    const auto now = adc.irVoltage();
    // check if light has changed enough to treat it as "on"
    if( distance( now, sampler.average() ) > Voltage::irThreshold )
      dim.start();

    // do next dimmer tick (true means this is the last one)
    if( dim.nextTick() )
    {
      // fill sampler with no-self-light data
      for(decltype(sampler.size()) i=0; i<sampler.size(); ++i)
      {
        ++pwmCycles;
        PowerSave::idle();
        sampler.add( adc.irVoltage() );
      }
    }

    //
    // periodic stuff
    //
    ++pwmCycles;
    if( pwmCycles >= Pwm::frequency() ) // second passed?
    {
      sampler.add(now);                 // save last measurement for later
      pwmCycles = 0;                    // reesd cycle count to count new second
    }

    // wait until next cycle.
    PowerSave::idle();
  }
}
