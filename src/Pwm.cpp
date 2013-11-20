#include "config.hpp"
#include <avr/interrupt.h>

ISR(TIM0_OVF_vect)
{
  // main task of this interrupt is to wake up CPU from sleep mode. :-)
}
