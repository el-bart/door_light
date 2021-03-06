#ifndef INCLUDE_LEDCTRL_HPP_FILE
#define INCLUDE_LEDCTRL_HPP_FILE

#include "config.hpp"
#include "LedBase.hpp"
#include "Pwm.hpp"

/** @brief control LEDs (IR and red) controll class.
 */
class LedCtrl
{
public:
  explicit LedCtrl(Pwm& pwm):
    pwm_(pwm)
  {
    fill(Light::irFill);
    enable(true);
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
      pinLed_.set(false);
  }

private:
  LedBase<Pin::ctrlLed> pinLed_;
  Pwm&                  pwm_;
};

#endif
