#ifndef INCLUDE_DIMHANDLER_HPP_FILE
#define INCLUDE_DIMHANDLER_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include "Pwm.hpp"
#include "LedLight.hpp"

class DimHandler
{
public:
  explicit DimHandler(LedLight& light):
    light_(light),
    tick_(0),
    start_(false)
  { }

  void start(void)
  {
    light_.fill(0xFF);         // start with 0 fill
    light_.enable(true);    // turn lights on and start proper PWM
    tick_  = 1;             // first tick goes here
    start_ = false;         // do this once :)
  }

  bool isStarted(void) const
  {
    return tick_ > 0u;
  }

  /** @brief process next cycle.
   *  @return true if tisk was the last tick of the cycle, false otherwise.
   */
  bool nextTick(void)
  {
    if( not isStarted() )
      return false;
    tick();
    if( tick_ < toTicks(Light::dimIn) )
      return firstStage();

    return stop();
  }

private:
  bool firstStage(void)
  {
    // TODO
    return false;
  }

  void tick(void)
  {
    ++tick_;
  }

  bool stop(void)
  {
    light_.enable(false);
    tick_  = 0;
    return true;            // this is the last tick in this cycle
  }

  static constexpr uint16_t toTicks(const uint8_t s)
  {
    return s * Pwm::frequency();
  }

  LedLight& light_;
  uint16_t  tick_;
  bool      start_;
};

#endif
