/*
 * RGB_Functions.h
 *
 * Created: 1-12-2013 23:09:03
 *  Author: Administrator
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#ifndef RGB FUNCTIONS_H_
#define RGB FUNCTIONS_H_

#include "Colors.h"



// Settings groups
typedef enum {
	Group1=1,
	Group2,
	Group3,
	Group4,
	Group5,
	GroupAll
} Groepen;

typedef enum{
	Links=1,
	Rechts
} Richtingen;

typedef enum{
	Ja=1,
	Neen,
	AfEnToe,
	Willekeur,
	Speciaal
}Keuzes;

#define Group1_Start 1
#define Group1_Length 6
#define Group2_Start Group1_Length+1
#define Group2_Length 5
#define Group3_Start Group2_Start+Group2_Length+1
#define Group3_Length 5
#define Group4_Start Group3_Start+Group3_Length+1
#define Group4_Length 9
#define Group5_Start Group4_Start+Group4_Length+1
#define Group5_Length 10
#define GroupALL_Start 1
#define GroupALL_Length Group4_Start+Group4_Length

//WS2811
#define WS2811_PIN PIND5
#define WS2811_DDR DDRD
#define WS2811_PORT PORTD
#define WS2811_LOW WS2811_PORT &= ~(1<<WS2811_PIN);
#define WS2811_HIGH WS2811_PORT |= (1<<WS2811_PIN);
#define WS2811_ledcount GroupALL_Length
#define NOP_62_5 __asm__("nop\n\t")
#define NOP_250 __asm__("nop\n\t""nop\n\t""nop\n\t") // Wait 62.5ns * 4
#define NOP_600 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // Wait 62.5ns * 9
#define NOP_650 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // Wait 62.5ns * 11
#define NOP_1000 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")
long WS2811[WS2811_ledcount];

void WS2811_Update(void);
void WS2811_SetMid(uint8_t From, uint8_t Aantal, long Kleur);
void WS2811_SetMidAlternerend(uint8_t From, uint8_t Aantal, long Kleur, uint8_t Alternerend);
void WS2811_SetMidUpdate(uint8_t From, uint8_t Aantal, long Kleur, uint8_t Alternerend);
void WS2811_SetGroupColor(int Groep, long Kleur, uint8_t Alternerend);
void WS2811_SetGroupColorUpdate(int Groep, long Kleur, uint8_t Alternerend);
void WS2811_SetGroupRainbowColor(uint8_t Groep, uint8_t Startnr_Regenboog);
void WS2811_RotateGroup(uint8_t Groep, uint8_t AantalKeer, uint8_t Richting, uint8_t UpdateTijdensRotate, uint16_t DelaytijdTijdensRotate);
void WS2811_RotateGroupUpdate(uint8_t Groep, uint8_t AantalKeer, uint8_t Richting, uint8_t UpdateTijdensRotate, uint16_t DelaytijdTijdensRotate);
void WS2811_OneToEndGroupUpdate(uint8_t Used_GroupStart, uint8_t Used_GroupLength, long Color);
#endif /* RGB FUNCTIONS_H_ */