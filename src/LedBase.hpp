#ifndef INCLUDE_LEDBASE_HPP_FILE
#define INCLUDE_LEDBASE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

/** @brief basic LED contorling facility tools.
 */
template<uint8_t P>
class LedBase
{
public:
  LedBase(void)
  {
    DDRB |= P;  // set this pin as output
  }

  void set(const bool on)
  {
    if(on) PORTB |=  P;
    else   PORTB &= ~P;
  }
};

#endif
