#ifndef INCLUDE_LEDLIGHT_HPP_FILE
#define INCLUDE_LEDLIGHT_HPP_FILE

#include "config.hpp"
#include "LedBase.hpp"

/** @brief main light source controlling facility.
 */
class LedLight: public LedBase<Pin::light>
{
public:
  LedLight(void)
  {
    enable(false);
    // PWM setup - use OC0B
    static_assert(Pin::light == _BV(PB1), "PWM set to another channel");
  }

  void pwm(uint8_t fill)
  {
  }

  void enable(const bool on) { set(not on); }
};

#endif
