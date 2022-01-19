/*
 * UHR_2.c
 *
 * Created: 29.07.2020 17:26:08
 * Author : meik_
 */ 

//#define F_CPU 1e6
#define taster_1 PB1

#include <avr/io.h>
//#include <avr/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

volatile uint32_t temp=0;
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

volatile uint32_t sec=0;					//hier kann zeit eingestellt werden
volatile uint32_t min=1;
volatile uint32_t hr=1;

int main(void)
{
	ASSR = (1<<AS2);												  // asynchron laufen lassen
	ACSR = 1<<ACD;													  // Strom sparen
	PRR = 1<<PRTWI|1<<PRTIM0|1<<PRTIM1|1<<PRSPI|1<<PRUSART0|1<<PRADC;
	TCCR2A = (1<<WGM21);											  // CTC auf OCRA
	OCR2A = 0;													   	//no prescaler here 
	TCCR2B = 1<<CS22|1<<CS21|1<<CS20;								  // Prescaler auf 1024

	while((ASSR & (1<< TCR2BUB)));		// synchronisieren
	TIMSK2 = (1<<OCIE2A);
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sei();

	DDRD = 0b00111111;
	DDRC = 0b00001111;
	DDRB &= ~(1<<taster_1);

    while (1) 
    {
		while((ASSR & (1<< TCR2AUB)));
		sleep_mode();
		TCCR2A = TCCR2A;					// sonst Mehrfachinterrupts:
			
		if(sec<60*(32)){}						//sec=59*32 wegen prescaler und damit taster schnell reagiert
		else{sec=0; min++;}
		if(min<=59){}
		else {min=0; hr++;}
		if(hr<=12){}
		else{hr=1;}
		
		// TASTER EIN/AUS SCHALTEN ------------------------------------------------------------

		if (con==1){sec=0;}				//aufhören zu Zeit zählen
		else{
			if (PINB & (1<<taster_1))
			{
				//kürzer verweilen
				if (ii>=0){
					ii=0;
					if (PINB & (1<<taster_1)){
						temp=1;
					}
				}
				else {
					ii++;
				}
				//länger verweilen
				if (jj>=150){
					jj=0;
					ii=0;					//Satck der Anzeige unbedingt 0 setzen !
					if (PINB & (1<<taster_1)){
						temp=4;
						PORTD = 0x00;			//LEDs ausschalten fürs blinken
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


		switch(temp) {

			//UHRZEIT ANZEIGEN---------------------------------------------------------------------


			//Knightrider hoch (Begin: Taster kurz, Ende: zeitabhängig)
			case 1:
			if (aa>=1){
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

			//Uhrzeit anzeigen und kurz warten (Begin: temp=2, Ende: zeitabhängig)
			case 2:
			if (bb>=150){
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

			//Knightrider runter (Begin: temp=1, Ende: zeitabhängig)
			case 3:
			if (cc>=1){
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


			//UHRZEIT STELLEN ---------------------------------------------------------------------

			//Blink (Begin: Taster lang, Ende: zeitabhängig)
			case 4:
			if (dd>=4){
				dd=0;
				i++;
				PORTD ^= (1<<PD0 | 1<<PD1 | 1<<PD2 | 1<<PD3 | 1<<PD4 | 1<<PD5 | 1<<PD6 );
				PORTC ^= (1<<PC0 | 1<<PC1 | 1<<PC2 | 1<<PC3);
				if (i>=14){
					i=0;
					temp=5;
				}
			}
			else {
				dd++;
				temp=4;
				con=1;				//deaktiviere Taster sofort bis temp=6
			}
			break;

			//Uhrzeit stellen Modi aktivieren und ticken ausstellen (Begin: temp=4, Ende: zeitabhängig)
			case 5:
			if (ff>=10){
				ff=0;
				k++;
				if (PINB & (1<<taster_1)){
					min=min+5;
					k=0;
				}
				if (k>=10){
					PORTD = 0x00;				//LEDs ausschalten fürs blinken
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

			// Blink (Begin: temp=5, Ende: zeitabhängig)
			case 6:
			if (gg>=4){
				gg=0;
				i++;
				PORTD ^= (1<<PD0 | 1<<PD1 | 1<<PD2 | 1<<PD3 | 1<<PD4 | 1<<PD5 | 1<<PD6 );
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

			// Start für neue Zeit mit einem Klick (Begin: temp=6; Ende= Taster)
			case 7:
			if (PINB & (1<<taster_1)){
				hh=0;
				temp=255;
				con=0;					//aktiviere Taster wieder
			}
			else{
				PORTD = 0;				//uhrzeit hier doch nich anzeigen, wegen batterieverbrauch
				PORTC = 0;
				temp=7;
			}
			break;


			//AUSSCHALTEN -------------------------------------------------------------------------

			//sonst alles aus
			default:
			PORTC = 0;
			PORTD = 0;
			break;
		}

	}
}

ISR (TIMER2_COMPA_vect) {
	sec++;	
}
