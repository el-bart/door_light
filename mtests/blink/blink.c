#define F_CPU 1000000L      // quarc in [Hz]
#include <avr/io.h>
#include <util/delay.h>     // <avr/delay.h> once

int main(void)
{
  DDRB |=0xFF;              // while PB as output
  PORTB =0xFF;              // blink interchangably
  for(;;)
  {
     PORTB^=0xFF;           // negate
     _delay_ms(1000);       // wait 1[s]
  }

  return 0;
}

