#include "config.hpp"
#include <avr/interrupt.h>

// interrupt is to be ignored - all it does is wake up the CPU from sleep mode
ISR(TIM0_OVF_vect)
{ }
