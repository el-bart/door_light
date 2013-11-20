#ifndef INCLUDE_LOWPOWERHANDLER_HPP_FILE
#define INCLUDE_LOWPOWERHANDLER_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include "PowerSave.hpp"

class LowPowerHandler
{
public:
  LowPowerHandler(void):
    tooLowCount_{0}
  { }

  bool isLowPower(const Millivolts vcc)
  {
    // if voltage below threshold?
    if( vcc < Voltage::minVcc )
    {
      ++tooLowCount_;
      if( tooLowCount_ > Voltage::maxVccThresholdBreach )
        return true;
    }
    // everything's ok - no need to worry! :)
    tooLowCount_ = 0;
    return false;
  }

  void halt(void)
  {
    // we do not return from here
    while(true)
      PowerSave::idle();
  }

private:
  uint8_t tooLowCount_;
};

#endif
