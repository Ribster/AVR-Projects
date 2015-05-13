/*
 * Initializations.h
 *
 * Created: 1-12-2013 15:11:00
 *  Author: Robbe Van Assche
 */ 


#ifndef INITIALIZATIONS_H_
#define INITIALIZATIONS_H_
#include "GlobalDefinitions.h"
#include "Lightdriver.h"

// Pushbuttons
// #define ButtonR1 (~(PINC) & (1<<PINC0))
// #define ButtonR2 (~(PINC) & (1<<PINC1))
// #define ButtonR3 (~(PINC) & (1<<PINC2))
// #define ButtonL1 (~(PINC) & (1<<PINC3))
// #define ButtonL2 (~(PINC) & (1<<PINC4))
// #define ButtonL3 (~(PINC) & (1<<PINC5))
// Samplingpins
// #define DOUT_STR1 ((PINB) & (1<<PINB1))
// #define DOUT_STR2 ((PIND) & (1<<PIND2))
// #define DOUT_STR3 ((PIND) & (1<<PIND1))
// #define DOUT_STR4 ((PIND) & (1<<PIND0))

//DEBUG
#define DebugLed_ON PORTD |= (1<<PIND1)
#define DebugLed_OFF PORTD &= ~(1<<PIND1)
#define DebugLed_TOGGLE PORTD ^= (1<<PIND1)


void init(void);

#endif /* INITIALIZATIONS_H_ */

