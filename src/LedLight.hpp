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
  }

  void enable(const bool on) { set(not on); }
};

#endif
