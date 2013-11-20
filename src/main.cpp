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
#include "DimHandler.hpp"
#include "LowPowerHandler.hpp"

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
  Pwm             pwm;
  LedCtrl         ir(pwm);
  LedLight        light(pwm);
  DimHandler      dim(light);
  Adc             adc;
  LowPowerHandler pwr;
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
    if( pwmCycles >= Pwm::frequency() )         // second passed?
    {
      prev      = now;                          // save last measurement for later
      pwmCycles = 0;                            // reesd cycle count to count new second
      if( pwr.isLowPower( adc.vccVoltage() ) )  // monitor Vcc levels
        break;
    }

    // wait until next cycle.
    PowerSave::idle();
  } // main processing loop


  //
  // low power handling code - one way trip...
  //
  cli();
  ir.enable(false);
  light.enable(false);
  // run infinite sleeping loop. note that power-down mode is NOT possible, since this
  // makes main lights on all the time!
  while(true)
    pwr.halt();
}
