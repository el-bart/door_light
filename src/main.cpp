#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#include "Adc.hpp"
#include "Pwm.hpp"
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
  //AdcSampler sampler(0);   // this will force turn-lights-on on reset
  sei();

  //
  // infinite system loop
  //
  uint16_t   pwmCycles = 0;
  Millivolts prev      = 0;
  while(true)
  {
    // grab ADC measurement ASAP, since PWM might have low fill
    const auto now = adc.irVoltage();
    // check if light has changed enough to treat it as "on"
    if( distance( now, prev ) > Voltage::irThreshold )
    {
      prev = now;
      dim.start();
    }
    // process dimming, if required
    dim.nextTick();

    //
    // periodic stuff
    //
    ++pwmCycles;
    if( pwmCycles >= Pwm::frequency() ) // second passed?
    {
      prev = now;                       // save last measurement for later
      pwmCycles = 0;                    // reesd cycle count to count new second
    }

    // wait until next cycle.
    PowerSave::idle();
  }
}
