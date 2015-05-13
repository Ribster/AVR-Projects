/*
 * GlobalDefinitions.h
 *
 * Created: 1-12-2013 23:10:00
 *  Author: Robbe Van Assche
 */ 


#ifndef GLOBALDEFINITIONS_H_
#define GLOBALDEFINITIONS_H_

#include <avr/io.h>

#define F_CPU 16000000UL

extern volatile uint8_t StatusTimer;
extern volatile uint16_t TimerValue16;

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "Colors.h"
#include "Lightdriver.h"

#endif /* GLOBALDEFINITIONS_H_ */