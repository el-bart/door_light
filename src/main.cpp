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


using AdcSampler = Sampler<Millivolts, Light::irSamples>;

//
// MAIN PROGRAM
//
int main(void)
{
  Pwm             pwm;
  LedCtrl         ir(pwm);
  LedLight        light(pwm);
  Adc             adc;
  AdcSampler      sampler(0);
  LowPowerHandler lph;
  // enable interrupts globaly
  sei();

  //
  // infinite system loop
  //
  uint16_t timerCycle = 0;  // counter for timer cycles
  uint16_t nextIrMeasure = 0;   // on which timer count do next IR measurement?
  uint8_t  secInCycle = 0;  // number of second within the cycle
  while(true)
  {
    if( timerCycle >= nextIrMeasure )
    {
      // TODO measure
      // set next measurement after 
      constexpr auto f = Pwm::frequency();
      constexpr auto d = f / Light::irMeasureSec;
      nextIrMeasure = ( nextIrMeasure + d ) % f;
    }
    //const auto voltIr = adc.irVoltage();
    // TODO
    const bool turnOn = true;

    if(turnOn)
    {
      // TODO: initial setup
      secInCycle = 1;
    }

    if( secInCycle > 0u )
    {
      if( secInCycle <= Light::dimIn )
      {
        // TODO
      }
      else
        if( secInCycle <= Light::lightOn )
        {
          // TODO
        }
        else
          if( secInCycle <= Light::dimOut )
          {
            // TODO
          }
          else
          {
            // TODO: turn off
            secInCycle = 0;
          }
    } // if(in light cycle)

    // mark new cycle
    ++timerCycle;
    // check if 1s has passed
    if( timerCycle >= Pwm::frequency() )
    {
      timerCycle = 0;                       // reset counter
      lph.handle( adc.vccVoltage() );       // check Vcc (note: may never return from here)
    }
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
}
