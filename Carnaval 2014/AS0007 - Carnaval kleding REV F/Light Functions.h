/*
 * Light_Functions.h
 *
 * Created: 5-12-2013 20:00:32
 *  Author: Robbe Van Assche
 */ 


#ifndef LIGHT_FUNCTIONS_H_
#define LIGHT_FUNCTIONS_H_
#include "GlobalDefinitions.h"
#include "Light Functions.h"
#include "Lightdriver.h"
#include "Initializations.h"

#define TIMER_ON TCCR1B |= (1<<CS10)
#define TIMER_OFF TCCR1B &= ~(1<<CS10)

#define TIM_Status_ON StatusTimer = 1
#define TIM_Status_OFF StatusTimer = 0
#define TIM_Value_CLEAR TimerValue16 = 0
#define TIM_Value_READ TimerValue16
#define TIM_Value_DECREMENT(x) TimerValue16 -= x
#define TIM_Value_SET16(x) TimerValue16 = x
#define TIM_Timer_Set(x) OCR1A=x

#define TIM_Delay_Sequence_Standard 15842



void SONGMIX(void);
#endif /* LIGHT FUNCTIONS_H_ */