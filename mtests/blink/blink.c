#define F_CPU 1000000L      // quarc in [Hz]
#include <avr/io.h>
#include <util/delay.h>     // <avr/delay.h> once

int main(void)
{
  DDRB|=_BV(3);             // PB3 as output
  DDRB|=_BV(4);             // PB4 as output

  PORTB^=_BV(3);            // blink interchangably

  for(;;)
  {
    PORTB^=_BV(3);          // negate PB3
    PORTB^=_BV(4);          // negate PB4
    // sleep 0.5[s]
    for(uint8_t i=0; i<5; ++i)
      _delay_ms(100);
  }

  return 0;
}

