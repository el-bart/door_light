#include "config.hpp"
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

#include "Adc.hpp"
#include "Sampler.hpp"
#include "LedCtrl.hpp"
#include "LedLight.hpp"

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


using AdcSampler = Sampler<Adc::Millivolts, irSamples>;

//
// MAIN PROGRAM
//
int main(void)
{
  LedCtrl    ir;
  LedLight   light;
  Adc        adc;
  AdcSampler sampler(0);

#if 0
  {
    ir.enable(true);
    _delay_ms(1);
    const auto first = adc.irVoltage();
    for(uint8_t i=0; i<irSamples; ++i)
      sampler.add(first);
    ir.enable(false);
    _delay_ms(50);
  }
#endif

  // TODO                                               
  ir.enable(true);
  for(;;)
  {
    light.enable(true);
    const auto v = adc.irVoltage();
    sampler.add(v);
    const auto min = sampler.min();
    const auto max = sampler.max();
    const auto d   = max-min;
    const auto s   = d<<4;
    wait(s);

    light.enable(false);
    wait(1000-s);
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
