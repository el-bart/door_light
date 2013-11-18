#ifndef INCLUDE_LEDLIGHT_HPP_FILE
#define INCLUDE_LEDLIGHT_HPP_FILE

#include "config.hpp"
#include "LedBase.hpp"

/** @brief main light source controlling facility.
 */
class LedLight: public LedBase<Pin::light>
{
public:
  explicit LedLight(Pwm& pwm):
    pwm_(pwm)
  {
    fill(Light::lightFill);
    enable(false);
  }

  void fill(const uint8_t f)
  {
    static_assert(Pin::light == _BV(PB1), "PWM set to another channel");    // PWM setup - use OC0B
    pwm_.fillB(f);
  }

  void enable(const bool on)
  {
    pwm_.enableB(on);
    if(not on)
      enablePin(false);
  }

private:
  void enablePin(const bool on) { set(not on); }

  Pwm& pwm_;
};

#endif
