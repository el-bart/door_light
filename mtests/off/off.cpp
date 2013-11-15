#include <avr/io.h>

int main(void)
{
  // set direction for both pins
  DDRB  |=  _BV(PB0) | _BV(PB1);
  // turn off leds
  PORTB &= ~_BV(PB0);
  PORTB |=  _BV(PB1);
  // wait forevert
  for(;;);
}
