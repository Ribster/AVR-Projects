/*
 * TestPrograms.c
 *
 * Created: 1-12-2013 15:24:02
 *  Author: Administrator
 */ 
#include "TestPrograms.h"
#include "GlobalDefinitions.h"
#include "RGB Functions.h"
#define F_CPU 16000000UL
#include <util/delay.h>

void TEST1(){
	for (int b=8; b>0; b--)
	{
		for (int i=15; i>0; i--)
		{
			WS2801[0] += 0x010000;
			WS2801_Update();
			_delay_ms(2);
		}
			WS2801[0] += 0x010000;
			WS2801_Update();
			_delay_ms(5);
	}
	WS2801[0] = 0x000000;
	WS2801_Update();
	_delay_ms(2500);
}
void TEST2(){
	
}
void TEST3(){
		uint8_t kleur = 0x05;
		// Gedeelte
		kleur = 0xF0;
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
		WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			//Gedeelte
			kleur = 0xFF;
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
			WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		// Gedeelte
		kleur = 0xF0;
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
		WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			//Gedeelte
			kleur = 0xFF;
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
			WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		// Gedeelte
		kleur = 0xF0;
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
		WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			//Gedeelte
			kleur = 0xFF;
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
			WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		// Gedeelte
		kleur = 0xF0;
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
		WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
		WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
		WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
		WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
		WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			//Gedeelte
			kleur = 0xFF;
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB1, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB1, Neen);
			WS2801_Set_Delay_Update(kleur, 420, 350, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB3, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB3, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 200, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB7, Neen);
			WS2811_SetGroupColorUpdate(Group2, COLOR_RB5, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_RB5, Neen);
			WS2801_Set_Delay_Update(kleur, 840, 700, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB9, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
			WS2801_Set_Delay_Update(kleur, 240, 150, Neen);
			WS2811_SetGroupColorUpdate(Group4, COLOR_RB11, Neen);
			WS2811_SetGroupColorUpdate(Group1, COLOR_RB19, Neen);
		//Gedeelte
		WS2811_SetGroupColorUpdate(GroupAll, COLOR_RB1, Neen);
		WS2801_Set_Delay_Update(kleur, 420, 200, Neen);
		WS2811_SetGroupColorUpdate(GroupAll, COLOR_RB15, Neen);
		WS2801_Set_Delay_Update(kleur, 350, 200, Neen);
		WS2811_SetGroupColorUpdate(GroupAll, COLOR_RB2, Neen);
		WS2801_Set_Delay_Update(kleur, 350, 200, Neen);
		WS2811_SetGroupColorUpdate(GroupAll, COLOR_RB6, Neen);
		WS2801_Set_Delay_Update(kleur, 760, 400, Neen);
		WS2811_SetGroupColorUpdate(GroupAll, COLOR_RB9, Neen);
		WS2801_Set_Delay_Update(kleur, 280, 100, Neen);
		WS2811_SetGroupColorUpdate(GroupAll, COLOR_UIT, Neen);
}
void TEST4(){
		#define TijdvoorDelay 10
		for (uint8_t u=1; u<31; u++)
		{
			WS2811_SetGroupRainbowColor(Group1, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
			WS2811_SetGroupRainbowColor(Group2, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
			WS2811_SetGroupRainbowColor(Group3, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
			WS2811_SetGroupRainbowColor(Group4, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
		}
		
		for (uint8_t u=32; u>1; u--)
		{
			WS2811_SetGroupRainbowColor(Group1, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
			WS2811_SetGroupRainbowColor(Group2, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
			WS2811_SetGroupRainbowColor(Group3, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
			WS2811_SetGroupRainbowColor(Group4, u);
			WS2811_Update();
			_delay_ms(TijdvoorDelay);
		}
		
// 		WS2811_SetGroupColorUpdate(GroupAll, COLOR_UIT);
// 		_delay_ms(500);
}

void TEST5(void){
	WS2811_RotateGroupUpdate(Group4, 50, Rechts, Ja, 150);
	WS2811_RotateGroupUpdate(Group4, 50, Links, Ja, 150);
	WS2811_RotateGroupUpdate(Group2, 50, Rechts, Ja, 150);
	WS2811_RotateGroupUpdate(Group2, 50, Links, Ja, 150);
	WS2811_RotateGroupUpdate(Group3, 50, Rechts, Ja, 150);
	WS2811_RotateGroupUpdate(Group3, 50, Links, Ja, 150);
}

void TEST7(void){
	for (int bu=0; bu<150; bu++)
	{
		WS2811_RotateGroup(Group4, 2, Rechts, Neen, 0);
		WS2811_RotateGroup(Group2, 2, Rechts, Neen, 0);
		WS2811_RotateGroup(Group3, 2, Rechts, Neen, 0);
		WS2811_Update();
		_delay_ms(150);
	}
	
	for (int bu=0; bu<150; bu++)
	{
		WS2811_RotateGroup(Group4, 2, Links, Neen, 0);
		WS2811_RotateGroup(Group2, 2, Links, Neen, 0);
		WS2811_RotateGroup(Group3, 2, Links, Neen, 0);
		WS2811_Update();
		_delay_ms(150);
	}
}

void TEST6(void){
		WS2811_SetMid(Group4_Start,4, COLOR_ROOD);
		WS2811_SetMid(Group4_Start+4,5, COLOR_WIT);
		WS2811_SetMid(Group2_Start,2, COLOR_ROOD);
		WS2811_SetMid(Group2_Start+2,2, COLOR_WIT);
		WS2811_SetMid(Group2_Start+4,1, COLOR_WIT);
		WS2811_SetMid(Group3_Start,2, COLOR_WIT);
		WS2811_SetMid(Group3_Start+2,2, COLOR_ROOD);
		WS2811_SetMid(Group3_Start+4,1, COLOR_WIT);
		WS2811_Update();
}	

void TEST8(void){
			// Gedeelte
			
 			uint8_t kleur = 0;
 			//LED_Set_C_Delay(kleur, 200, 150);
			
// 			kleur = 0x01;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x05;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x10;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x15;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x20;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x25;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x30;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x35;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x40;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x45;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x50;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x55;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x60;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x65;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x70;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x75;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x80;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x85;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x90;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0x95;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xA0;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xA5;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xB0;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xB5;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xC0;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xC5;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xD0;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xD5;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xE0;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xE5;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xF0;
// 			LED_Set_C_Delay(kleur, 200, 150);
// 			kleur = 0xF5;
// 			LED_Set_C_Delay(kleur, 200, 150);

			for (;kleur<0x80;kleur++)
			{
				WS2801_Set_Delay_Update(kleur, 2, 1, Neen);
			}
			for (kleur=0x79;kleur>0;kleur--)
			{
				WS2801_Set_Delay_Update(kleur, 2, 1, Neen);
			}
			
			
/*			_delay_ms(1000);*/
}