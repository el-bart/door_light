#ifndef INCLUDE_LEDCTRL_HPP_FILE
#define INCLUDE_LEDCTRL_HPP_FILE

#include "config.hpp"
#include "LedBase.hpp"
#include "Pwm.hpp"

/** @brief control LEDs (IR and red) controll class.
 */
class LedCtrl: public LedBase<Pin::ctrlLed>
{
public:
  explicit LedCtrl(Pwm& pwm):
    pwm_(pwm)
  {
    enable(false);
    fill(irFill);
  }

  void fill(const uint8_t f)
  {
    static_assert(Pin::ctrlLed == _BV(PB0), "PWM set to another channel");  // PWM setup to use OC0A
    pwm_.fillA(f);
  }

  void enable(const bool on)
  {
    pwm_.enableA(on);
    if(not on)
      set(false);
  }

private:
  Pwm& pwm_;
};

#endif
