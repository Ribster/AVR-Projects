/*
 * LCD.h
 *
 * Created: 15-12-2013 20:35:17
 *  Author: Administrator
 */ 


#ifndef LCD_H_
#define LCD_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SCK 5 // Clock line
#define MISO 4 // Master In Slave Out line
#define MOSI 3 // Master Out Slave In line
#define CS 2 // Chip select pin of the AVR; referred to in the datasheet as "SS"
#define DC 4 // Data / Command
#define RESET 3 // Reset

#define SCK_LOW SPI_PORT &= ~(1<<SCK)
#define SCK_HIGH SPI_PORT |= 1<<SCK

#define MOSI_LOW SPI_PORT &= ~(1<<MOSI)
#define MOSI_HIGH SPI_PORT |= 1<<MOSI

#define MISO_LOW SPI_PORT &= ~(1<<MISO)
#define MISO_HIGH SPI_PORT |= 1<<MISO

#define CS_LOW SPI_PORT &= ~(1<<CS)
#define CS_HIGH SPI_PORT |= 1<<CS

#define DC_LOW PORTD &= ~(1<<DC)
#define DC_HIGH PORTD |= 1<<DC

#define RESET_LOW PORTD &= ~(1<<RESET)
#define RESET_HIGH PORTD |= 1<<RESET

typedef enum{
	Tile_1 = 1,
	Tile_2,
	Tile_3,
	Tile_4,
	Tile_5,
	Tile_6
} TileNumbers;

typedef enum{
	State_Idle=1,
	State_Good,
	State_Bad
} TileStates;

typedef enum{
	CharTileOne=1,
	CharTileTwo,
	CharTileThree,
	CharTileFour,
	CharTileFive,
	CharTileSix,
	CharStateIdle,
	CharStateGood,
	CharStateBad
} TileCharacter;

#define L_xMin 0
#define L_xMax 117

#define R_xMin 123
#define R_xMax 240

#define LR3_yMin 0
#define LR3_yMax 102

#define LR2_yMin 108
#define LR2_yMax 209

#define LR1_yMin 215
#define LR1_yMax 320

#define LCDMin 0
#define LCDxMax 240
#define LCDyMax 320

#define WidthCharacters 12
#define HeightCharacters 12
#define ColorCharactersBlack 0x00
#define ColorCharactersWhite 0xFF

uint8_t Tile_1_State = State_Idle;
uint16_t Tile_1_Dimensions[] = {L_xMin, LR1_yMin, L_xMax, LR1_yMax};
uint8_t Tile_2_State = State_Idle;
uint16_t Tile_2_Dimensions[] = {L_xMin, LR2_yMin, L_xMax, LR2_yMax};
uint8_t Tile_3_State = State_Idle;
uint16_t Tile_3_Dimensions[] = {L_xMin, LR3_yMin, L_xMax, LR3_yMax};
uint8_t Tile_4_State = State_Idle;
uint16_t Tile_4_Dimensions[] = {R_xMin, LR1_yMin, R_xMax, LR1_yMax};
uint8_t Tile_5_State = State_Idle;
uint16_t Tile_5_Dimensions[] = {R_xMin, LR2_yMin, R_xMax, LR2_yMax};
uint8_t Tile_6_State = State_Idle;
uint16_t Tile_6_Dimensions[] = {R_xMin, LR3_yMin, R_xMax, LR3_yMax};

#define xStateIdleWidth 0
#define yStateIdleHeight 0

#define xStateGoodWidth 2
#define yStateGoodHeight 1

#define xStateBadWidth 3
#define yStateBadHeight 1



uint8_t TileColor_Idle[]={0xFF,0xFF,0xFF};
uint8_t TileColor_Bad[]={0xff,0x00,0x00};
uint8_t TileColor_Good[]={0x00,0xff,0x00};
	
uint16_t Char_1[] = {0x0000, 0x0060, 0x00E0, 0x01E0, 0x0060, 0x0060, 0x0060, 0x0060, 0x0060, 0x01F8, 0x01F8, 0x0000};
uint16_t Char_2[] = {0x0000, 0x01F8, 0x03FC, 0x030C, 0x001C, 0x0038, 0x0070, 0x00E0, 0x01C0, 0x03FC, 0x03FC, 0x0000};
uint16_t Char_3[] = {0x0000, 0x01F8, 0x03FC, 0x030C, 0x000C, 0x0078, 0x0078, 0x000C, 0x030C, 0x03FC, 0x01F8, 0x0000};
uint16_t Char_4[] = {0x0000, 0x0038, 0x0078, 0x00F8, 0x01D8, 0x0398, 0x0318, 0x03FC, 0x03FC, 0x0018, 0x0018, 0x0000};
uint16_t Char_5[] = {0x0000, 0x03FC, 0x03FC, 0x0300, 0x0300, 0x03F8, 0x03FC, 0x000C, 0x000C, 0x03FC, 0x03F8, 0x0000};
uint16_t Char_6[] = {0x0000, 0x01FC, 0x03FC, 0x0300, 0x0300, 0x03F8, 0x03FC, 0x030C, 0x030C, 0x03FC, 0x01F8, 0x0000};

uint16_t Char_O[] = {0x0000, 0x01F8, 0x03FC, 0x030C, 0x030C, 0x030C, 0x030C, 0x030C, 0x030C, 0x03FC, 0x01F8, 0x0000};
uint16_t Char_K[] = {0x0000, 0x030C, 0x031C, 0x0338, 0x0370, 0x03E0, 0x03E0, 0x0370, 0x0338, 0x031C, 0x030C, 0x0000};
uint16_t Char_N[] = {0x0000, 0x030C, 0x038C, 0x03CC, 0x03EC, 0x037C, 0x033C, 0x031C, 0x030C, 0x030C, 0x030C, 0x0000};




#endif /* LCD_H_ */