#define F_CPU 1000000L      // quarc in [Hz]
#include <avr/io.h>
#include <util/delay.h>     // <avr/delay.h> once

int main(void)
{
  DDRB|=0xFF;               // while PB as output

  PORTB^=0xFF;              // blink interchangably

  for(;;)
  {
  PORTB^=0xFF;              // negate
    // sleep 0.5[s]
    for(uint8_t i=0; i<5; ++i)
      _delay_ms(100);
  }

  return 0;
}

