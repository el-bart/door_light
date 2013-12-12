#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

#include "Adc.hpp"
#include "Pwm.hpp"
#include "LedCtrl.hpp"
#include "LedLight.hpp"
#include "Watchdog.hpp"
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
  Watchdog        wdg;
  LowPowerHandler pwr;
  Pwm             pwm;
  LedCtrl         ir(pwm);
  LedLight        light(pwm);
  DimHandler      dim(light);
  Adc             adc;

  wdg.reset();
  sei();


  //
  // main system loop. can be interrupted only by low power.
  //
  uint16_t       pwmCycles    = 0;      // PWM generated cycles count: from 0 to Pwm::frequency()
  Millivolts     base         = 0;      // base voltage to compare IR with. 0 ensures first check will lit up main lights, as expected
  Millivolts     lastAvg      = 0;      // last computed average voltage value
  uint32_t       sum          = 0;      // current sum of the obtained values
  constexpr auto samplesCount = Pwm::frequency() / Light::irSamples;    // number of samples to average
  uint8_t        sampleNum    = 0;      // currently collected samples count
  while(true)
  {
    //
    // collect averaged data
    //
    sum += adc.irVoltage();             // grab ADC measurement ASAP, since PWM might have low fill
    ++sampleNum;                        // mark new sample
    if( sampleNum >= samplesCount )
    {
      // save the actuall result and reset temporary variables
      lastAvg   = sum / sampleNum;
      sum       = 0;
      sampleNum = 0;
      // check if light has changed enough to treat it as "on"
      if( distance(lastAvg, base) > Voltage::irThreshold )
      {
        base = lastAvg;                 // save to prevent re-reseting this value
        dim.start();                    // start light cycle
      }
    }

    //
    // process dimming, if required
    //
    dim.nextTick();

    //
    // periodic stuff
    //
    ++pwmCycles;                                // mark next cycle has passed
    if( pwmCycles >= Pwm::frequency() )         // second passed?
    {
      base      = lastAvg;                      // save last measuement for later every once a while
      pwmCycles = 0;                            // reset cycle count to count new second
      if( pwr.isLowPower( adc.vccVoltage() ) )  // monitor Vcc levels
        break;
    }

    //
    // housekeeping and syncing loop with timer
    //
    wdg.reset();                        // confirm we're alive :)
    PowerSave::idle();                  // wait until next cycle.
  } // main processing loop


  //
  // low power handling code - one way trip...
  //
  cli();
  wdg.disable();
  ir.enable(false);
  light.enable(false);
  // run infinite sleeping loop. note that power-down mode is NOT possible, since this
  // makes main lights on all the time!
  while(true)
    pwr.halt();
}
