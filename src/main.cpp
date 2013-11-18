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
  sei();

  for(int i=0; i<1*5; ++i)
  {
    ir.enable(false);
    wait(100);
    ir.enable(true);
    wait(100);
  }

  auto startVolt = adc.irVoltage();

  //
  // infinite system loop
  //
  uint32_t cnt = 0;
  while(true)
  {
    auto now = adc.irVoltage();
    if( distance( now, startVolt ) > 30u )
    {
      ir.enable(false);
      wait(1000);
      ir.enable(true);
      PowerSave::idle();
      startVolt = adc.irVoltage();
    }

    if( cnt % (Pwm::frequency()/2) == 0 )
      startVolt = now;
    ++cnt;

    PowerSave::idle();
  }
}
