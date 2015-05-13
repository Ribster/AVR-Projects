/*
 * Main.c
 *
 * Created: 1-12-2013 15:01:58
 *  Author: Robbe Van Assche
 */ 



#include "GlobalDefinitions.h"
#include "Initializations.h"
#include "Light Functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



int main(void)
{

	init();

	
	while(1)
    {
		SONGMIX();
    }
	return 0;
}

ISR(TIMER1_COMPA_vect){
	if (TimerValue16>0)
	{
		TIM_Value_DECREMENT(1);
	}
	else
	{
		TIM_Status_OFF;
		TIMER_OFF;		
	}
}	