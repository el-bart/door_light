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
    start_(false),
    alreadyOn_(false)
  { }

  void start(void)
  {
    light_.fill(0x00);      // start with 0 fill
    light_.enable(true);    // turn lights on and start proper PWM
    tick_  = 1;             // first tick goes here
    start_ = false;         // do this once :)
    alreadyOn_ = false;     // indicates that main light-on section has been already visited
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
    // pre-condition check
    if( not isStarted() )
      return false;
    // count ticks
    tick();
    // cycle: dim-in, lights on, dim-out
    if( tick_ < toTicks(Light::dimIn) )
      return dimInHandler();
    if( tick_ < toTicks(Light::dimIn + Light::lightOn) )
      return lightOnHandler();
    if( tick_ < toTicks(Light::dimIn + Light::lightOn + Light::dimOut) )
      return dimOutHandler();
    // end of cycle
    return stop();
  }

private:
  bool dimInHandler(void)
  {
    const auto from = toTicks(0);
    const auto len  = toTicks(Light::dimIn);
    light_.fill( curTickToFill(from, len) );
    return false;
  }

  bool lightOnHandler(void)
  {
    if(not alreadyOn_)
    {
      light_.fill(0xFF);
      alreadyOn_ = true;
    }
    return false;
  }

  bool dimOutHandler(void)
  {
    const auto from = toTicks(Light::dimIn + Light::lightOn);
    const auto len  = toTicks(Light::dimOut);
    light_.fill( uint8_t{255}-curTickToFill(from, len) );
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

  uint16_t toTicks(const uint8_t s) const
  {
    return s * Pwm::frequency();
  }

  uint8_t curTickToFill(const uint16_t from, const uint16_t len)
  {
    return static_cast<uint8_t>( ( uint16_t{0xFF} * scaleDownTick(tick_-from) ) / scaleDownTick(len) );
  }

  uint16_t scaleDownTick(const uint16_t in)
  {
    constexpr uint8_t c = Pwm::frequency() / 32u;
    return in / c;
  }

  LedLight& light_;
  uint16_t  tick_;
  bool      start_;
  bool      alreadyOn_;
};

#endif
