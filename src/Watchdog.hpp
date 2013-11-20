#ifndef INCLUDE_WATCHDOG_HPP_FILE
#define INCLUDE_WATCHDOG_HPP_FILE

#include "config.hpp"
#include <avr/wdt.h>

/** \brief watchdog operating class.
 */
class Watchdog
{
public:
  /** \brief initialies watchdog timer.
   *  \note watchdog starts in disabled mode.
   */
  Watchdog(void)
  {
    disable();
  }

  void enable(void)
  {
    wdt_enable(WDTO_120MS);     // 120[ms] is enough to decide that we're out of business
  }

  void reset(void)
  {
    wdt_reset();
  }

  void disable(void)
  {
    wdt_disable();
  }

}; // class Watchdog

#endif
