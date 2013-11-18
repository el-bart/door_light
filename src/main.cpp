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

  for(int i=0; i<4*5; ++i)
  {
    ir.enable(false);
    _delay_ms(100);
    ir.enable(true);
    _delay_ms(100);
  }

  //
  // infinite system loop
  //
  uint16_t timerCycle = 0;      // counter for timer cycles
  uint16_t nextIrMeasure = 0;   // on which timer count do next IR measurement?
  uint8_t  secInCycle = 0;      // number of second within the cycle
  while(true)
  {
    // do IR light measurement on every predefined time slot
    if( timerCycle >= nextIrMeasure )
    {
      // measure and check if it has been signaled to turn on the lights
      sampler.add( adc.irVoltage() );
      if( distance( sampler.oldest(), sampler.newest() ) >= Voltage::irThreshold )
        secInCycle = 1;

      // set next measurement after predefined amount of time
      constexpr auto f = Pwm::frequency();
      constexpr auto d = f / Light::irMeasureSec;
      nextIrMeasure = ( nextIrMeasure + d ) % f;
    }

    // enable light with dim-in and dim-out cycles
    if( secInCycle > 0u )
    {
      if( secInCycle <= Light::dimIn )
      {
        // TODO
        light.enable(true); // TODO: temporary soultion
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
            light.enable(false);
          }
    } // if(in light cycle)

    // mark new cycle
    ++timerCycle;
    // check if 1s has passed
    if( timerCycle >= Pwm::frequency() )
    {
      timerCycle = 0;                       // reset counter
      // TODO: temporary removed, not to break stuff while testing
      //lph.handle( adc.vccVoltage() );       // check Vcc (note: may never return from here)
      // move to next second in the light loop, if on
      if(secInCycle)
        ++secInCycle;
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
