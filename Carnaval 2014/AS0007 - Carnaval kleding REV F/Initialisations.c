/*
 * Initialisations.c
 *
 * Created: 1-12-2013 16:31:40
 *  Author: Robbe Van Assche
 */ 
#include "GlobalDefinitions.h"
#include "Initializations.h"
#include "Light Functions.h"
#include <avr/interrupt.h>

void init(void){
	//Init for WS2801 pins
	WS2801_DDR |= (1<<WS2801_PIN_CKI);
	DDRB |= (1<<WS2801_PIN_SDI);
	WS2801_PORT &= ~(1<<WS2801_PIN_CKI);
	PORTB &= ~(1<<WS2801_PIN_SDI);
	//Init for WS2811 pins
	WS2811_DDR |= 1<<WS2811_PIN;
	WS2811_PORT &= ~(1<<WS2811_PIN);
	
// 	//Init for debugled
// 	DDRD |= (1<<PIND0) | (1<<PIND1);
// 	PORTD &= ~((1<<PIND0)|(1<<PIND1));
	
	//CTC mode
	TCCR1B |= (1<<WGM12);
	TIMSK1 |= (1<<OCIE1A);
	TIM_Timer_Set(TIM_Delay_Sequence_Standard);
	sei();
	
	WS2811_SetColorALL(0x000000);
	WS2801_SetColor(0x00);
	
 	WS2801_Update();
 	WS2811_Update();
}