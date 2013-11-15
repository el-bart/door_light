#ifndef INCLUDE_LEDCTRL_HPP_FILE
#define INCLUDE_LEDCTRL_HPP_FILE

#include "config.hpp"
#include "LedBase.hpp"

/** @brief control LEDs (IR and red) controll class.
 */
class LedCtrl: public LedBase<Pin::ctrlLed>
{
public:
  LedCtrl(void)
  {
    enable(false);
    // PWM setup - use OC0A
    static_assert(Pin::ctrlLed == _BV(PB0), "PWM set to another channel");
  }

  void pwm(uint8_t fill)
  {
  }

  void enable(const bool on) { set(on); }
};

#endif
