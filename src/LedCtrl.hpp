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
  }

  void enable(const bool on) { set(on); }
};

#endif
