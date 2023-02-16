/*
 * Created: 29.07.2020 17:26:08
 * Author : meik
 */ 

#define taster_1 PB1

#include <avr/io.h>
//#include <avr/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

volatile uint32_t temp=0;               // global flags
volatile uint32_t con=0;

volatile uint32_t ii=0;
volatile uint32_t jj=0;

uint32_t i=0;
uint32_t j=7;
uint32_t k=0;

volatile uint32_t aa=0;
volatile uint32_t bb=0;
volatile uint32_t cc=0;
volatile uint32_t dd=0;
volatile uint32_t ee=0;
volatile uint32_t ff=0;
volatile uint32_t gg=0;
volatile uint32_t hh=0;

volatile uint32_t sec=0;                // bring-up time at first start
volatile uint32_t min=1;
volatile uint32_t hr=1;

int main(void)
{
  ASSR = (1<<AS2);										  // async mode
  ACSR = 1<<ACD;											  // save power
  PRR = 1<<PRTWI|1<<PRTIM0|1<<PRTIM1|11<<PRUSART0|1<<PRADC;
  TCCR2A = (1<<WGM21);								  // CTC to OCRA
  OCR2A = 0;													  // no overflow value (full stack)
  TCCR2B = 1<<CS22|1<<CS21|1<<CS20;		  // prescaler auf 1024

  while((ASSR & (1<< TCR2BUB)));		    // sync for first start-up
  TIMSK2 = (1<<OCIE2A);
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);  // enable sleep mode 
  sei();                                // enable interrupts

  DDRD = 0b00111111;                    // define I/Os at channels
  DDRC = 0b00001111;
  DDRB &= ~(1<<taster_1);

  /* main loop */

  while (1) 
  {
    while((ASSR & (1<< TCR2AUB)));
    sleep_mode();
    TCCR2A = TCCR2A;					          // if not, concurrent interrupts

    /* time calc */

    if(sec<60*(32)){}						        // 32 clk cycles represents 1 second...
    else{sec=0; min++;}                 // ...thus:  min = 59 sec * 32 tiks
    if(min<=59){}
    else {min=0; hr++;}
    if(hr<=12){}
    else{hr=1;}

    /* switch button on/off */ 

    if (con==1){sec=0;}				          // stop counting time 
    else{
      if (PINB & (1<<taster_1))
      {
        if (ii>=0){                     // short press (for time-mode)
          ii=0;
          if (PINB & (1<<taster_1)){
            temp=1;
          }
        }
        else {
          ii++;
        }
        if (jj>=150){                   // long press  (for set-time-mode)
          jj=0;
          ii=0;					
          if (PINB & (1<<taster_1)){
            temp=4;
            PORTD = 0x00;			          // turn LEDs off for blinking (case 4)
            PORTC = 0x00;
          }
        }
        else {
          jj++;
        }
      }
      else
      {
        ii=0;
        jj=0;
      }
    }

    /* switch/case for normal-mode & set-time-mode */

    switch(temp) {

      /* normal-mode: show time */ 

      case 1:                           // knightrider up 
        if (aa>=1){                     // (begin: short press, end: time-indep)
          aa=0;
          i++;
          PORTD = (1<<i);
          PORTC = (i<<i);
          if (i>=7){
            i=0;
            temp=2;
          }
        }
        else {
          aa++;
          temp=1;
        }
        break;

        
      case 2:                           // show time and wait a bit 
        if (bb>=150){                   // (begin: temp=2, end: time-indep)
          bb=0;
          temp=3;
          PORTD = 0;
          PORTC = 0;
        }
        else {
          PORTD = min;
          PORTC = hr;
          bb++;
          temp=2;
        }
        break;

        
      case 3:                           // knightrider down 
        if (cc>=1){                     // (begin: temp=1, end: time-indep)
          cc=0;
          j--;
          PORTD = (1<<j);
          PORTC = (1<<j);
          if (j<=0){
            j=7;
            temp=255;
          }
        }
        else {
          cc++;
          temp=3;
        }
        break;

      /* set-time-mode: increment minutes by pressing button */

      case 4:                           // all LEDs blink uniformly 
        if (dd>=4){                     // (begin: long press, end: time-indep)
          dd=0;
          i++;
          PORTD ^= (1<<PD0 | 1<<PD1 | 1<<PD2 | 1<<PD3 | 1<<PD4 | 1<<PD5 | 1<<PD6);
          PORTC ^= (1<<PC0 | 1<<PC1 | 1<<PC2 | 1<<PC3);
          if (i>=14){
            i=0;
            temp=5;
          }
        }
        else {
          dd++;
          temp=4;
          con=1;				                // deact. button instantly until temp=6 
        }
        break;


      case 5:                           // set-time-mode, and disable clk count
        if (ff>=10){                    // (begin: temp=4, end: time-depend)
          ff=0;
          k++;
          if (PINB & (1<<taster_1)){
            min=min+5;                  // adjust incremental jump here
            k=0;
          }
          if (k>=10){
            PORTD = 0x00;				        // turn LEDs off for blinking again
            PORTC = 0x00;
            k=0;
            temp=6;
          }
        }
        else {
          PORTD = min;
          PORTC = hr;
          ff++;
          temp=5;
        }
        break;


      case 6:                           // all LEDs blink uniformly
        if (gg>=4){                     // (begin: temp=5; end: time-depend)
          gg=0;
          i++;
          PORTD ^= (1<<PD0 | 1<<PD1 | 1<<PD2 | 1<<PD3 | 1<<PD4 | 1<<PD5 | 1<<PD6);
          PORTC ^= (1<<PC0 | 1<<PC1 | 1<<PC2 | 1<<PC3);
          if (i>=14){
            i=0;
            temp=7;
          }
        }
        else {
          gg++;
          temp=6;
        }
        break;

      case 7:                           // switch to normal-mode again
        if (PINB & (1<<taster_1)){      // (begin: temp=6, end: button press)
          hh=0;
          temp=255;
          con=0;					              // button is now hot...
        }                               // ... to trigger clk again
        else{
          PORTD = 0;				            // do not show time here
          PORTC = 0;
          temp=7;
        }
        break;

      /* turn LEDs off */

      default:                          // turn LEDs off
        PORTC = 0;                      // (begin: break/default, end: time-ind)
        PORTD = 0;
        break;
    }

  }

}

/* global interrupt */

ISR (TIMER2_COMPA_vect) {               // multitasking approach
  sec++;	                              // see line 18
}
