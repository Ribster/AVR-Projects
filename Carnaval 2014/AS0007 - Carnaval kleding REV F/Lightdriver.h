/*
 * Lightdriver.h
 *
 * Created: 7/02/2014 23:08:17
 *  Author: Robbe Van Assche
 */ 


#ifndef LIGHTDRIVER_H_
#define LIGHTDRIVER_H_

#include "GlobalDefinitions.h"
#include "Colors.h"
#include <avr/io.h>
#include <stdlib.h>

//WS2801
#define WS2801_PIN_CKI PIND7
#define WS2801_PIN_SDI PINB0
#define WS2801_DDR DDRD
#define WS2801_PORT PORTD
#define WS2801_CKI_LOW WS2801_PORT &= ~(1<<WS2801_PIN_CKI)
#define WS2801_SDI_LOW PORTB &= ~(1<<WS2801_PIN_SDI)
#define WS2801_CKI_HIGH WS2801_PORT |= (1<<WS2801_PIN_CKI)
#define WS2801_SDI_HIGH PORTB |= (1<<WS2801_PIN_SDI)


void WS2801_Update(void);
void WS2801_SetColor(uint8_t Color);
void WS2801_Fade(uint16_t Delay, uint8_t MaxMinVal0to80, uint8_t In1Out0);
uint8_t WS2801_GetColor(void);

//WS2811
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

#define Group1_Start 20
#define Group1_End 25
#define Group2_Start 15
#define Group2_End 19
#define Group3_Start 10
#define Group3_End 14
#define Group4_Start 1
#define Group4_End 9
#define GroupALL_Start 0
#define GroupALL_End WS2811_ledcount-1

#define WS2811_PIN PIND5
#define WS2811_DDR DDRD
#define WS2811_PORT PORTD
#define WS2811_LOW WS2811_PORT &= ~(1<<WS2811_PIN);
#define WS2811_HIGH WS2811_PORT |= (1<<WS2811_PIN);
#define WS2811_ledcount 25
#define NOP_62_5 __asm__("nop\n\t")
#define NOP_250 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t") // Wait 62.5ns * 4
#define NOP_600 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // Wait 62.5ns * 9
#define NOP_650 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") // Wait 62.5ns * 11
#define NOP_1000 __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")


void WS2811_Update(void);
void WS2811_SetColor(uint32_t Color, uint8_t Pixel);
void WS2811_SetColorALL(uint32_t Color);
void WS2811_SetColorPixelIntensity(uint8_t ColorBlue0Green8Red16, uint8_t Pixel,uint8_t Intensity0to7, uint8_t ADD1No0);
void WS2811_SetGroupColor(uint32_t Color, uint8_t Groupnmbr, uint8_t Even0Odd1All2);
void WS2811_SetGroupPixel(uint32_t Color, uint8_t Groupnmbr, uint8_t GroupMember);
void WS2811_GroupIntensify(uint8_t ColorBlue0Green8Red16, uint8_t Group,uint8_t Intensity0to7, uint8_t ADD1No0);
void WS2811_Replace(uint32_t FromColor, uint32_t ToColor);
void WS2811_Rotate(uint8_t CW1CCW0, uint8_t GpNmbr1to5);
void WS2811_SWAP(uint8_t GroupFrom1or2or3or4is4inor5is4out, uint8_t GroupTo1or2or3or4is4inor5is4out);
void WS2811_Shift(uint8_t BackToFrontAdd0FrontToBackAdd1BackToFront2FrontToBack3, uint32_t ColorToAdd);
void WS2811_ShiftBackToFront(uint8_t Times, uint32_t ColorToAdd, uint8_t Add0Rotate1);
void WS2811_ShiftFrontToBack(uint8_t Times, uint32_t ColorToAdd, uint8_t Add0Rotate1);
uint32_t WS2811_GetColor(uint8_t Pixel);
void WS2811_SetLeft(uint32_t Color);
void WS2811_SetRight(uint32_t Color);
// Summary
void UpdateAll(void);

#endif /* LIGHTDRIVER_H_ */