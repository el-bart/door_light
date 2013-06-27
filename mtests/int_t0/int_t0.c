#define F_CPU 1000000L      // quarc in [Hz]

#include    <avr/io.h>
#include    <avr/interrupt.h>

volatile int abc = 0;

int main(void)
{
  //cli();
  DDRB |= _BV(4);
  PORTB|= _BV(4);
for(;;);            

/*
  DDRB |=0xFF;
PORTB|=0xFF;            

  //PORTB &=~((1<<PB0)|(1<<PB1)|(1<<PB2))|_B; //
  //PORTB|=_BV(4);
  //USTAWIENIE TIMERA DLA ATTINY13!!!!!!!!!!!!!!!!
  TCCR0A |=(1<<WGM01);//tryb CTC.
  // WGM - waveform generation mode bit
  //TCCR0B |= (1<<CS02)|(1<<CS00); //przeskaler ust na 1024 za pom Clock Select bit
  OCR0A = 237;//OCR0A - rejestr porownania.
  TIMSK0 |= (1<<OCIE0A);
  sei();//globalne zezwolenie na przerwania
  while(1){
//    if(abc)
      PORTB|=0xFF;
  }
  */
}

//==========Procedura obslugi przerwania======================//
ISR(BADISR_vect)
{
      // user code here
}
#if 0
ISR(TIM0_COMPA_vect){
    abc = 1;
}

ISR(BADISR_vect)
{
      // user code here
}

#endif
