/*
 * Light_Functions.c
 *
 * Created: 5-12-2013 20:00:22
 *  Author: ¨Robbe Van Assche
 */ 

#include "GlobalDefinitions.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Light Functions.h"
#include "Lightdriver.h"
#include "Initializations.h"

//#define DONE

volatile uint8_t StatusTimer;
volatile uint16_t TimerValue16;
static uint8_t General_Counter;
static uint8_t PartCount = 1;  
static uint8_t SectionFlag;
static uint8_t HelpFlag;
static uint8_t UpDown;
static int8_t KleurCount;

static uint32_t LastColor;


static uint32_t COLORPOOL[] = {0xFF0000,0xFF7F00,0xFFFF00,0x00FF00,0x0000FF,0xB20000,0xFEBFBF,0xFE8080,0xB25900,0xFEDFBF,0xFEBF80,0xB2B300,0xFEFFBF,0xFEFF80,0x00B200,0xBFFEBF,0x80FE80,0x0000B2,0xBFBFFE,0x8080FE,0xFFFF24,0x91FF24,0x24FF24,0x24FF91,0x24FFFF,0x2491FF,0x2424FF,0x9124FF,0xFF24FF,0xFF2491,0xFF2424,0xFF9124,0xFFFF61,0xFFFF9E,0x6161FF,0x9E9EFF};
static uint32_t COLORPOOLRAINBOW[] = {0xFF0000,0xFF0700,0xFF0F00,0xFF1700,0xFF1F00,0xFF2700,0xFF2F00,0xFF3700,0xFF3F00,0xFF4700,0xFF4F00,0xFF5700,0xFF5F00,0xFF6700,0xFF6F00,0xFF7700,0xFF7F00,0xFF8700,0xFF8F00,0xFF9700,0xFF9F00,0xFFA700,0xFFAF00,0xFFB700,0xFFBF00,0xFFC700,0xFFCF00,0xFFD700,0xFFDF00,0xFFE700,0xFFEF00,0xFFF700,0xFFFF00,0xEFFF00,0xDFFF00,0xCFFF00,0xBFFF00,0xAFFF00,0x9FFF00,0x8FFF00,0x7FFF00,0x6FFF00,0x5FFF00,0x4FFF00,0x3FFF00,0x2FFF00,0x1FFF00,0x0FFF00,0x00FF00,0x00EF0F,0x00DF1F,0x00CF2F,0x00BF3F,0x00AF4F,0x009F5F,0x008F6F,0x007F7F,0x006F8F,0x005F9F,0x004FAF,0x003FBF,0x002FCF,0x001FDF,0x000FEF,0x0000FF,0x0400F7,0x0900EF,0x0E00E7,0x1200DF,0x1700D7,0x1C00D0,0x2000C8,0x2500C0,0x2A00B8,0x2E00B0,0x3300A9,0x3800A1,0x3C0099,0x410091,0x460089,0x4B0082,0x4F0089,0x530091,0x570099,0x5C00A1,0x6000A9,0x6400B0,0x6800B8,0x6D00C0,0x7100C8,0x7500D0,0x7900D7,0x7E00DF,0x8200E7,0x8600EF,0x8A00F7,0x8F00FF};
void StartTimer_16(uint16_t x){
	TIM_Value_SET16(x); TIM_Status_ON; TIMER_ON;
}

void StopTimer(void){
	do {} while (StatusTimer);
	WS2801_Update(); 
	WS2811_Update();
}

void PartStart(uint8_t PartNumber){
	General_Counter++;
	if (PartCount<PartNumber){
		General_Counter = 0;
		PartCount = PartNumber;
	}
}
  
void All_Off(void){
	WS2801_SetColor(0x00);
	WS2811_SetColorALL(0x000000);
}
void All_On(void){
	WS2801_SetColor(0xff); 
	WS2811_SetColorALL(0x444444);
}

void FlashTimer(uint16_t xy){
	WS2801_SetColor(0x00);
	StopTimer();
	StartTimer_16(xy);
	WS2801_SetColor(0xff);
} 

void Sequence_Part2(uint16_t xy){
	StartTimer_16(75);
	PartStart(2);
	FlashTimer(xy-75);
	StopTimer();
}
void Sequence_Part3(uint16_t xy){
	StartTimer_16(xy);
	PartStart(3);
	if (General_Counter==0)
	{
		WS2811_SetColorALL(0x000000);
		uint32_t KleurTijdelijk = COLORPOOL[0];
		uint32_t KleurTijdelijk2 = COLORPOOL[3];
		WS2811_SetGroupPixel(KleurTijdelijk, 4, 2);
		WS2811_SetGroupPixel(KleurTijdelijk2, 4, 3);
		WS2811_SetGroupPixel(KleurTijdelijk2, 4, 5);
		WS2811_SetGroupPixel(KleurTijdelijk, 4, 6);

		WS2811_SetGroupPixel(KleurTijdelijk, 2, 3);
		WS2811_SetGroupPixel(KleurTijdelijk2, 2, 2);
		WS2811_SetGroupPixel(KleurTijdelijk, 3, 3);
		WS2811_SetGroupPixel(KleurTijdelijk2, 3, 2);

		WS2811_SetGroupPixel(KleurTijdelijk2, 1, 1);
		WS2811_SetGroupPixel(KleurTijdelijk, 1, 3);
		WS2811_SetGroupPixel(KleurTijdelijk, 1, 4);
		WS2811_SetGroupPixel(KleurTijdelijk2, 1, 6);
		General_Counter = 1;
	}
	WS2811_Rotate(1,1);
	WS2811_Rotate(1,2);
	WS2811_Rotate(1,3);
	WS2811_Rotate(1,4);
	WS2811_Rotate(1,5);
	
	if (General_Counter<2)
	{
		WS2811_Replace(COLORPOOL[0], COLORPOOL[1]);
	}
	else if(General_Counter<3){
		WS2811_Replace(COLORPOOL[3], COLORPOOL[4]);
	}
	else if(General_Counter<4){
		WS2811_Replace(COLORPOOL[1], COLORPOOL[0]);
	}
	else if(General_Counter<5){
		WS2811_Replace(COLORPOOL[4], COLORPOOL[3]);
		General_Counter=0;
	}
	StopTimer();
}
void Sequence_Part4(uint16_t xy){
	StartTimer_16(xy);
	PartStart(4);
	uint32_t KleurTijdelijk = COLORPOOL[0];
	uint32_t KleurTijdelijk2 = COLORPOOL[1];
	uint32_t KleurTijdelijk3 = COLORPOOL[3];
	uint32_t KleurTijdelijk4 = COLORPOOL[4];
	uint32_t KleurTijdelijk5 = COLORPOOL[0];
	uint32_t KleurTijdelijk6 = COLORPOOL[1];
	if (General_Counter==0)
	{
		WS2811_SetColorALL(0x000000);
		WS2801_SetColor(0xff);

		
		WS2811_SetGroupPixel(KleurTijdelijk, 4, 1);
		WS2811_SetGroupPixel(KleurTijdelijk2, 4, 2);
		WS2811_SetGroupPixel(KleurTijdelijk4, 4, 3);
		
		WS2811_SetGroupPixel(KleurTijdelijk5, 4, 5);
		WS2811_SetGroupPixel(KleurTijdelijk6, 4, 6);
		WS2811_SetGroupPixel(KleurTijdelijk3, 4, 7);
		

		WS2811_SetGroupPixel(KleurTijdelijk3, 2, 1);
		WS2811_SetGroupPixel(KleurTijdelijk5, 2, 3);
		WS2811_SetGroupPixel(KleurTijdelijk2, 2, 5);
		
		WS2811_SetGroupPixel(KleurTijdelijk, 3, 1);
		WS2811_SetGroupPixel(KleurTijdelijk4, 3, 3);
		WS2811_SetGroupPixel(KleurTijdelijk3, 3, 5);

		WS2811_SetGroupPixel(KleurTijdelijk, 1, 1);
		WS2811_SetGroupPixel(KleurTijdelijk3, 1, 3);
		WS2811_SetGroupPixel(KleurTijdelijk2, 1, 5);
		
		General_Counter = 1;
	}

	if (General_Counter<2)
	{
		WS2811_Replace(KleurTijdelijk, KleurTijdelijk3);
		if (General_Counter>1)
		{
			WS2801_SetColor(0x00);
		}
		WS2811_Rotate(0,1);
		WS2811_Rotate(1,2);
		WS2811_Rotate(0,3);
		WS2811_Rotate(1,4);
		WS2811_Rotate(0,5);
	}
	else if(General_Counter<3){
		WS2811_Replace(KleurTijdelijk2, KleurTijdelijk6);
		WS2801_SetColor(0xFF);
	}
	else if(General_Counter<4){
		WS2811_Replace(KleurTijdelijk3, KleurTijdelijk);
		WS2801_SetColor(0x00);
		WS2811_Rotate(0,1);
		WS2811_Rotate(0,3);
		WS2811_Rotate(1,4);
		WS2811_Rotate(0,5);
	}
	else if(General_Counter<5){
		WS2811_Replace(KleurTijdelijk4, KleurTijdelijk2);
	}
	else if(General_Counter<6){
		WS2811_Replace(KleurTijdelijk5, KleurTijdelijk4);
		WS2811_Rotate(0,1);
		WS2811_Rotate(1,2);
		WS2811_Rotate(1,4);
		WS2811_Rotate(0,5);
	}
	else if(General_Counter<7){
		WS2811_Replace(KleurTijdelijk6, KleurTijdelijk5);
		WS2801_SetColor(0xff);
		General_Counter=0;
	}
	StopTimer();
}
void Sequence_Part5(uint16_t xy){
	StartTimer_16(100);
	PartStart(5);
	WS2811_SetColorALL(0x000000);
	FlashTimer(xy-100);
	if (General_Counter==17)
	{
		All_Off();
	}
	StopTimer();
}
void Sequence_Part6(uint16_t xy){
	StartTimer_16(1);
	PartStart(6);
	if (SectionFlag==1|SectionFlag==3)
	{
		StopTimer();
		StartTimer_16(xy-8);
		WS2801_SetColor(0x00);
	}
	else if(SectionFlag==2)
	{
		StopTimer();
		StartTimer_16(75);
		FlashTimer(xy-88);
	}
	
	
	if (General_Counter==0){
		WS2811_ShiftBackToFront(1,0x000000,0);
		WS2811_ShiftBackToFront(1,COLORPOOLRAINBOW[18],0);
	}
	else if (General_Counter==1){
		WS2811_ShiftBackToFront(1,COLORPOOLRAINBOW[18],0);
	}
	else if (General_Counter==2){
		WS2811_ShiftBackToFront(1,0x000000,0);
	}
	else if (General_Counter==3){
		WS2811_ShiftBackToFront(1,COLORPOOLRAINBOW[47],0);
	}
	else if (General_Counter==4){
		WS2811_ShiftBackToFront(1,COLORPOOLRAINBOW[47],0);
	}
	else if (General_Counter==5){
		WS2811_ShiftBackToFront(1,COLORPOOLRAINBOW[85],0);
	}
	else if (General_Counter==6){
		WS2811_ShiftBackToFront(1,0x00,1);
	}
	else if (SectionFlag==1){
		WS2811_ShiftBackToFront(1,0x00,1);
	}
	else if (SectionFlag==2){
		WS2811_ShiftBackToFront(2,0x00,1);
	}
	else if (SectionFlag==3){
		WS2811_ShiftFrontToBack(1,0x00,1);
	}
	
}
void Sequence_Part7(uint16_t xy){
	StartTimer_16(1);
	PartStart(7);
	if (SectionFlag==3){
		StopTimer();
		StartTimer_16(75+1);
		WS2811_SetColorALL(0x000000);
		FlashTimer(xy-76);
	}
	else{
		StopTimer();
		StartTimer_16(xy);
	}		
	
	if (General_Counter==0|SectionFlag==4)
	{
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[9],1,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[25],1,6);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[44],2,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[25],2,4);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[44],3,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[25],3,5);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[9],4,5);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[25],4,9);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[9],4,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[25],4,6);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[44],4,1);
		
		WS2801_SetColor(0x00);
		SectionFlag = 1;
	}
	
	if (SectionFlag==1){
		WS2811_Rotate(1,1);
		WS2811_Rotate(1,2);
		WS2811_Rotate(1,3);
		WS2811_Rotate(0,4);
		WS2811_Rotate(1,5);
	}
	else if (SectionFlag==5)
	{
		WS2801_SetColor(0xff);
	}

	
	StopTimer();
}
void Sequence_Part8(uint16_t xy){
	StartTimer_16(xy);
	PartStart(8);
	WS2811_SetColorALL(0x444444);
	StopTimer();
}
void Sequence_Part9(uint16_t xy){
	StartTimer_16(1);
	PartStart(9);
	if (SectionFlag==2)
	{
		StopTimer();
		StartTimer_16(75+5);
		FlashTimer(xy+27-75);
	}
	else{
		
		StopTimer();
		StartTimer_16(xy+12);
		WS2801_SetColor(0x00);
	}
	
	
	if (General_Counter==0)
	{
		WS2811_SetColorALL(0x000000);
	}
	if (General_Counter<10)
	{
		WS2811_ShiftBackToFront(1,COLORPOOLRAINBOW[General_Counter*10],0);
		if (General_Counter==7)
		{
			General_Counter = 11;
		}
		//WS2811_SWAP(1,4);
	}
	else if (General_Counter>20 & General_Counter<90)
	{
		WS2811_ShiftFrontToBack(2,COLORPOOLRAINBOW[(General_Counter-41)*10],0);
		if (General_Counter==44)
		{
			General_Counter = 91;
		}
	}
	else if (General_Counter==12)
	{
		WS2811_SetGroupPixel(0x000000,4,2);
		WS2811_SetGroupPixel(0x000000,4,3);
		WS2811_SetGroupPixel(0x000000,4,6);
		WS2811_SetGroupPixel(0x000000,4,7);
		WS2811_SetGroupColor(0x000000,1,0);
		WS2811_SetGroupColor(0x000000,2,0);
		WS2811_SetGroupColor(0x000000,3,1);
	}
	else if (General_Counter==92)
	{
		WS2811_SetGroupPixel(0x000000,4,2);
		WS2811_SetGroupPixel(0x000000,4,3);
		WS2811_SetGroupPixel(0x000000,4,6);
		WS2811_SetGroupPixel(0x000000,4,7);
		WS2811_SetGroupColor(0x000000,1,0);
		WS2811_SetGroupColor(0x000000,2,0);
		WS2811_SetGroupColor(0x000000,3,1);
	}
	else if (General_Counter>92)
	{
		WS2811_Rotate(0,1);
		WS2811_Rotate(0,2);
		WS2811_Rotate(0,3);
		WS2811_Rotate(0,4);
		WS2811_Rotate(0,5);
		if (General_Counter>98)
		{
			General_Counter=0;
		}
	}
	else if (General_Counter<20)
	{
		WS2811_Rotate(1,1);
		WS2811_Rotate(1,2);
		WS2811_Rotate(1,3);
		WS2811_Rotate(1,4);
		WS2811_Rotate(0,5);
	}
	else if (General_Counter==20)
	{
		General_Counter = 40;
	}
	
	StopTimer();
}
void Sequence_Part10(uint16_t xy){
	StartTimer_16(1);
	PartStart(10);
	if (General_Counter == 0)
	{
		All_Off();
		StopTimer();
		WS2801_Fade(160,15,0);
		All_Off();
	}
	if (General_Counter >1)
	{
		StopTimer();
		StartTimer_16(20);
		All_Off();
		FlashTimer(xy-25);
		
	}
	else{
		StopTimer();
		StartTimer_16(xy-5);
	}
	StopTimer();
}
void Sequence_Part11(uint16_t xy){
	StartTimer_16(xy);
	PartStart(11);
	StopTimer();
}
void Sequence_Part12(uint16_t xy){
	StartTimer_16(xy);
	PartStart(12);
	if (General_Counter==0)
	{
		All_Off();
	}
	if (WS2811_GetColor(Group2_Start+1)==0x000000)
	{
		WS2811_SetGroupColor(0x444444,2,2);
		WS2811_SetGroupColor(0x000000,3,2);
		WS2811_SetGroupPixel(0x444444,1,1);
		WS2811_SetGroupPixel(0x444444,1,2);
		WS2811_SetGroupPixel(0x444444,1,3);
		WS2811_SetGroupPixel(0x000000,1,4);
		WS2811_SetGroupPixel(0x000000,1,5);
		WS2811_SetGroupPixel(0x000000,1,6);
		WS2811_SetGroupPixel(0x444444,4,7);
		WS2811_SetGroupPixel(0x000000,4,3);
	}
	else{
		WS2811_SetGroupColor(0x000000,2,2);
		WS2811_SetGroupColor(0x444444,3,2);
		WS2811_SetGroupPixel(0x000000,1,1);
		WS2811_SetGroupPixel(0x000000,1,2);
		WS2811_SetGroupPixel(0x000000,1,3);
		WS2811_SetGroupPixel(0x444444,1,4);
		WS2811_SetGroupPixel(0x444444,1,5);
		WS2811_SetGroupPixel(0x444444,1,6);
		WS2811_SetGroupPixel(0x000000,4,7);
		WS2811_SetGroupPixel(0x444444,4,3);
	}
	SectionFlag = 1;
	StopTimer();
}
void Sequence_Part13(uint16_t xy){
	StartTimer_16(xy);
	PartStart(13);
	if (WS2811_GetColor(Group2_Start+1)==0x000000)
	{
		WS2811_SetGroupColor(0x444444,2,2);
		WS2811_SetGroupColor(0x000000,3,2);
		WS2811_SetGroupPixel(0x444444,1,1);
		WS2811_SetGroupPixel(0x444444,1,2);
		WS2811_SetGroupPixel(0x444444,1,3);
		WS2811_SetGroupPixel(0x000000,1,4);
		WS2811_SetGroupPixel(0x000000,1,5);
		WS2811_SetGroupPixel(0x000000,1,6);
		WS2811_SetGroupPixel(0x444444,4,7);
		WS2811_SetGroupPixel(0x000000,4,3);
	}
	else{
		WS2811_SetGroupColor(0x000000,2,2);
		WS2811_SetGroupColor(0x444444,3,2);
		WS2811_SetGroupPixel(0x000000,1,1);
		WS2811_SetGroupPixel(0x000000,1,2);
		WS2811_SetGroupPixel(0x000000,1,3);
		WS2811_SetGroupPixel(0x444444,1,4);
		WS2811_SetGroupPixel(0x444444,1,5);
		WS2811_SetGroupPixel(0x444444,1,6);
		WS2811_SetGroupPixel(0x000000,4,7);
		WS2811_SetGroupPixel(0x444444,4,3);
	}
	if (SectionFlag == 6)
	{
		All_Off();
	}
	else if (SectionFlag == 5)
	{
		All_On();
	}
	
	StopTimer();
}
void Sequence_Part14(uint16_t xy){
	StartTimer_16(xy);
	PartStart(14);
	All_Off();
	StopTimer();
}
void Sequence_Part15(uint16_t xy){
	StartTimer_16(xy-5);
	PartStart(15);
	All_Off();
	
	if (General_Counter==0)
	{
		SectionFlag = 0;
	}

	uint32_t SetColor = COLORPOOLRAINBOW[SectionFlag];

	WS2811_SetGroupPixel(SetColor,1,General_Counter);
	WS2811_SetGroupPixel(SetColor,2,General_Counter);
	WS2811_SetGroupPixel(SetColor,3,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,1,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,2,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,3,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);


	if (General_Counter==2)
	{
		General_Counter=0;
	}
	
	if (SectionFlag>93)
	{
		SectionFlag = 0;
	}
	else{
		SectionFlag+=3;
	}
	
	
	StopTimer();
}
void Sequence_Part16(uint16_t xy){
	StartTimer_16(xy-5);
	PartStart(16);
	if (General_Counter==0)
	{
		SectionFlag = 0;
	}

	uint32_t SetColor = COLORPOOLRAINBOW[SectionFlag];

	WS2811_SetGroupPixel(SetColor,1,General_Counter);
	WS2811_SetGroupPixel(SetColor,2,General_Counter);
	WS2811_SetGroupPixel(SetColor,3,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,1,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,2,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,3,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);
	
	WS2811_Rotate(1,1);
	WS2811_Rotate(1,2);
	WS2811_Rotate(1,3);
	WS2811_Rotate(1,4);
	WS2811_Rotate(1,5);

	if (General_Counter==3)
	{
		General_Counter=0;
	}
	
	if (SectionFlag>93)
	{
		SectionFlag = 0;
	}
	else{
		SectionFlag+=3;
	}
	StopTimer();
}
void Sequence_Part17(uint16_t xy){
	StartTimer_16(xy-6);
	PartStart(17);
	if (General_Counter==0)
	{
		SectionFlag = 0;
	}

	uint32_t SetColor = COLORPOOLRAINBOW[SectionFlag];
	All_Off();
	WS2811_SetGroupPixel(SetColor,1,General_Counter);
	WS2811_SetGroupPixel(SetColor,2,General_Counter);
	WS2811_SetGroupPixel(SetColor,3,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,1,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,2,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,3,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);


	if (General_Counter==2)
	{
		General_Counter=0;
	}
	
	if (SectionFlag>93)
	{
		SectionFlag = 0;
	}
	else{
		SectionFlag+=3;
	}
	
	StopTimer();
}
void Sequence_Part18(uint16_t xy){
	StartTimer_16(xy);
	PartStart(18);
	if (General_Counter==0)
	{
		SectionFlag = 0;
	}

	uint32_t SetColor = COLORPOOLRAINBOW[SectionFlag];
	WS2811_SetGroupColor(0x000000,1,0);
	WS2811_SetGroupColor(0x000000,2,0);
	WS2811_SetGroupColor(0x000000,3,0);
	WS2811_SetGroupColor(0x000000,4,0);
	WS2811_SetGroupPixel(SetColor,1,General_Counter);
	WS2811_SetGroupPixel(SetColor,2,General_Counter);
	WS2811_SetGroupPixel(SetColor,3,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,1,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,2,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,3,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);
	
	WS2811_Rotate(0,1);
	WS2811_Rotate(0,2);
	WS2811_Rotate(1,3);
	WS2811_Rotate(0,4);
	WS2811_Rotate(0,5);

	if (General_Counter==3)
	{
		General_Counter=0;
	}
	
	if (SectionFlag>93)
	{
		SectionFlag = 0;
	}
	else{
		SectionFlag+=3;
	}
	StopTimer();
}
void Sequence_Part19(uint16_t xy){
	StartTimer_16(xy);
	PartStart(19);
	if (General_Counter==0)
	{
		SectionFlag = 0;
	}

	uint32_t SetColor = COLORPOOLRAINBOW[SectionFlag];
	All_Off();
	WS2811_SetGroupPixel(SetColor,1,General_Counter);
	WS2811_SetGroupPixel(SetColor,2,General_Counter);
	WS2811_SetGroupPixel(SetColor,3,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,1,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,2,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,3,5-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,9-General_Counter);
	WS2811_SetGroupPixel(SetColor,4,General_Counter);


	if (General_Counter==4)
	{
		General_Counter=0;
		WS2801_SetColor(0xff);
	}
	
	if (SectionFlag>93)
	{
		SectionFlag = 0;
	}
	else{
		SectionFlag+=3;
	}
	StopTimer();
}
void Sequence_Part20(uint16_t xy){
	StartTimer_16(xy);
	PartStart(20);
	All_Off();
	StopTimer();
}
void Sequence_Part21(uint16_t xy){
	StartTimer_16(xy);
	PartStart(21);
	if (General_Counter==0)
	{
		General_Counter = 93;
	}
	General_Counter -=7;
	
	uint32_t Kleurke = COLORPOOLRAINBOW[General_Counter];
	
	if (General_Counter<8)
	{
		General_Counter = 93;
	}

	if (SectionFlag == 1)
	{
		WS2811_SetColorALL(Kleurke);

		WS2801_SetColor(0x00);
	}
	
	if (HelpFlag==0&SectionFlag == 1){
		WS2811_SetGroupColor(0x000000,1,0);
		WS2811_SetGroupColor(0x000000,2,0);
		WS2811_SetGroupColor(0x000000,3,1);
		HelpFlag = 1;
	}
	else if (SectionFlag == 1)
	{
		WS2811_SetGroupColor(0x000000,1,1);
		WS2811_SetGroupColor(0x000000,2,1);
		WS2811_SetGroupColor(0x000000,3,0);
		HelpFlag = 0;
	}		
	
	StopTimer();
}
void Sequence_Part22(uint16_t xy){
	StartTimer_16(xy-2);
	PartStart(22);
	if (General_Counter==0)
	{
		General_Counter = 93;
	}
	General_Counter -=7;
	
	uint32_t Kleurke = COLORPOOLRAINBOW[General_Counter];
	
	if (General_Counter<8)
	{
		General_Counter = 93;
	}

	if (SectionFlag == 1)
	{
		WS2811_SetColorALL(Kleurke);

		WS2801_SetColor(0x00);
	}
	
	if (HelpFlag==0&SectionFlag == 1){
		WS2811_SetGroupColor(0x000000,1,0);
		WS2811_SetGroupColor(0x000000,2,0);
		WS2811_SetGroupColor(0x000000,3,1);
		WS2811_SetGroupColor(0x000000,4,1);
		HelpFlag = 1;
	}
	else if (SectionFlag == 1)
	{
		WS2811_SetGroupColor(0x000000,1,1);
		WS2811_SetGroupColor(0x000000,2,1);
		WS2811_SetGroupColor(0x000000,3,0);
		WS2811_SetGroupColor(0x000000,4,0);
		HelpFlag = 0;
	}
	
	StopTimer();
}
void Sequence_Part23(uint16_t xy){
	StartTimer_16(xy/2);
	PartStart(23);
	if (General_Counter==0)
	{
		General_Counter = 93;
	}
	General_Counter -=7;
	
	uint32_t Kleurke = COLORPOOLRAINBOW[General_Counter];
	
	if (General_Counter<8)
	{
		General_Counter = 93;
	}

	if (SectionFlag == 1)
	{
		WS2811_SetColorALL(Kleurke);

		WS2801_SetColor(0x00);
	}
	
	if (HelpFlag==0&SectionFlag == 1){
		WS2811_SetGroupColor(0x000000,1,0);
		WS2811_SetGroupColor(0x000000,2,0);
		HelpFlag = 1;
	}
	else if (SectionFlag == 1)
	{
		
		WS2811_SetGroupColor(0x000000,3,0);
		WS2811_SetGroupColor(0x000000,4,0);
		HelpFlag = 0;
	}
	if (SectionFlag==1){
		SectionFlag = 0;
	}
	else{
		SectionFlag = 1;
	}
	StopTimer();
	StartTimer_16((xy/2)-4);
	PartStart(23);
	if (General_Counter==0)
	{
		General_Counter = 93;
	}
	General_Counter -=7;
	
	Kleurke = COLORPOOLRAINBOW[General_Counter];
	
	if (General_Counter<8)
	{
		General_Counter = 93;
	}

	if (SectionFlag == 1)
	{
		WS2811_SetColorALL(Kleurke);
		WS2801_SetColor(0x00);
	}
	
	if (HelpFlag==0&SectionFlag == 1){
		HelpFlag = 1;
	}
	else if (SectionFlag == 1)
	{
		HelpFlag = 0;
	}
	if (SectionFlag==1){
		SectionFlag = 0;
	}
	else{
		SectionFlag = 1;
	}
	StopTimer();
}
void Sequence_Part24(uint16_t xy){
	StartTimer_16(xy-2);
	PartStart(24);
	
	uint32_t Kleur;
	
	if (General_Counter==0)
	{
		HelpFlag = 0;
		SectionFlag = 0;
	}
	
	if (SectionFlag==1){
		SectionFlag = 0;
	}
	else{
		SectionFlag = 1;
	}
	if (SectionFlag==1){
		
		for (uint8_t i = 0; i<WS2811_ledcount+1; i++)
		{
			Kleur = COLORPOOLRAINBOW[HelpFlag+i];
			WS2811_SetColor(Kleur,i);
		}
		WS2801_SetColor(0xff);
	}
	else{
		if (HelpFlag<10)
		{
			WS2811_SetGroupPixel(0x000000,4,HelpFlag);
			WS2811_SetGroupPixel(0x000000,1,HelpFlag);
			WS2811_SetGroupPixel(0x000000,1,HelpFlag+2);
			WS2811_SetGroupPixel(0x000000,2,HelpFlag);
			WS2811_SetGroupPixel(0x000000,2,HelpFlag+2);
			WS2811_SetGroupPixel(0x000000,3,HelpFlag);
			WS2811_SetGroupPixel(0x000000,3,HelpFlag+2);
		}
		else{
			WS2811_SetGroupPixel(0x000000,1,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,2,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,3,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,4,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,1,(HelpFlag/12)+2);
			WS2811_SetGroupPixel(0x000000,2,(HelpFlag/12)+2);
			WS2811_SetGroupPixel(0x000000,3,(HelpFlag/12)+2);
			WS2811_SetGroupPixel(0x000000,4,(HelpFlag/12)+2);
		}

		if (HelpFlag>45){
			WS2811_Replace(0x000000,COLORPOOLRAINBOW[HelpFlag-45]);
		}
		else{
			WS2811_Replace(0x000000,COLORPOOLRAINBOW[HelpFlag+45]);
		}
		
		
		WS2801_SetColor(0x00);
	}
	if (HelpFlag>65)
	{
		HelpFlag = 0;
	}
	else{
		HelpFlag += 2;
	}
		
		if (HelpFlag<10)
		{
			WS2811_SetGroupPixel(0x000000,4,HelpFlag);
			WS2811_SetGroupPixel(0x000000,1,HelpFlag);
			WS2811_SetGroupPixel(0x000000,1,HelpFlag+2);
			WS2811_SetGroupPixel(0x000000,2,HelpFlag);
			WS2811_SetGroupPixel(0x000000,2,HelpFlag+2);
			WS2811_SetGroupPixel(0x000000,3,HelpFlag);
			WS2811_SetGroupPixel(0x000000,3,HelpFlag+2);
		}
		else{
			WS2811_SetGroupPixel(0x000000,1,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,2,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,3,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,4,HelpFlag/12);
			WS2811_SetGroupPixel(0x000000,1,(HelpFlag/12)+2);
			WS2811_SetGroupPixel(0x000000,2,(HelpFlag/12)+2);
			WS2811_SetGroupPixel(0x000000,3,(HelpFlag/12)+2);
			WS2811_SetGroupPixel(0x000000,4,(HelpFlag/12)+2);
		}	
	
	StopTimer();
}
void Sequence_Part25(uint16_t xy){
	StartTimer_16(1);
	PartStart(25);
	
	if (General_Counter==0)
	{
		SectionFlag = 0;
		WS2811_SetGroupColor(COLORPOOLRAINBOW[General_Counter],4,2);
	}
	
	if (SectionFlag==1)
	{
		StopTimer();
		StartTimer_16(75);
		FlashTimer(xy-75-10);
	}
	else{
		StopTimer();
		StartTimer_16(xy-7);
		WS2801_SetColor(0x00);
	}
	

		WS2811_SWAP(4,2);
		WS2811_SWAP(4,3);
		WS2811_SetGroupColor(COLORPOOLRAINBOW[General_Counter*8],4,2);
		WS2811_SetGroupColor(COLORPOOLRAINBOW[(General_Counter*8)+10],1,2);
	
	if (General_Counter>9)
	{
		General_Counter = 0;
	} 
	
	switch (General_Counter)
	{
	case (1|5):
		WS2811_SetGroupColor(0x000000,1,0);
		WS2811_SetGroupColor(0x000000,2,0);
		WS2811_SetGroupColor(0x000000,3,1);
		WS2811_SetGroupColor(0x000000,4,0);
		break;
	case (2|7):
		WS2811_SetGroupColor(0x000000,1,1);
		WS2811_SetGroupColor(0x000000,2,1);
		WS2811_SetGroupColor(0x000000,3,0);
		WS2811_SetGroupColor(0x000000,4,1);
		break;
	case (3|8):
		WS2811_SetGroupColor(0x000000,1,2);
		break;
	case (4|9):
		WS2811_SetGroupColor(0x000000,1,2);
		WS2811_SetGroupColor(0x000000,2,2);
		WS2811_SetGroupColor(0x000000,3,2);
		WS2811_SetGroupColor(0x000000,4,2);
		break;
	}
	
	
	
	StopTimer();
}
void Sequence_Part26(uint16_t xy){
	StartTimer_16(xy-6);
	PartStart(26);
	
	uint32_t Kleurken = COLORPOOLRAINBOW[79];
	uint32_t TweedeKleurken = COLORPOOLRAINBOW[62];
	uint32_t DerdeKleurken = COLORPOOLRAINBOW[45];
	uint32_t VierdeKleurken = COLORPOOLRAINBOW[29];
	
	if (General_Counter==0)
	{
		SectionFlag = 0;
	}
	
	if (SectionFlag==0)
	{
		WS2811_SetColorALL(0x000000);
		SectionFlag = 1;
		WS2811_SetGroupColor(Kleurken,1,1);
		WS2811_SetGroupColor(Kleurken,2,1);
		WS2811_SetGroupColor(Kleurken,3,0);
		WS2811_SetGroupColor(Kleurken,4,1);
	}
	else if (SectionFlag==1)
	{
		WS2811_Replace(Kleurken,TweedeKleurken);
		SectionFlag = 2;
		WS2811_Rotate(1,1);
		WS2811_Rotate(1,2);
		WS2811_Rotate(1,3);
	}
	else if (SectionFlag==2)
	{
		WS2811_SetColorALL(0x000000);
		SectionFlag = 3;
		WS2811_SetGroupColor(DerdeKleurken,1,0);
		WS2811_SetGroupColor(DerdeKleurken,2,0);
		WS2811_SetGroupColor(DerdeKleurken,3,1);
		WS2811_SetGroupColor(DerdeKleurken,4,0);
	}
	else if (SectionFlag==3)
	{
		WS2811_Replace(DerdeKleurken,VierdeKleurken);
		SectionFlag = 0;
		WS2811_Rotate(0,1);
		WS2811_Rotate(0,2);
		WS2811_Rotate(0,3);
	}
	else if (SectionFlag==20)
	{
		All_On();
	}
	
	StopTimer();
}
void Sequence_Part27(uint16_t xy){
	StartTimer_16(xy-6);
	PartStart(27);
	if (General_Counter==0)
	{
		SectionFlag = 0;   
		General_Counter = 5;
		All_Off();
	}
	
	General_Counter -= 1;
	
	if (SectionFlag==0)
	{
		SectionFlag = 1;
		All_Off();
		if (General_Counter==4)
		{
			WS2811_SetGroupColor(0x444444,2,2);
		}
		else{
			WS2811_SetGroupColor(0x444444,3,2);
		}
		
	}
	else if (SectionFlag == 1)
	{
		SectionFlag = 0;
		All_On();
		if (General_Counter==4)
		{
			WS2811_SetGroupColor(0x444444,2,2);
			General_Counter +=1;
		}
		else{
			WS2811_SetGroupColor(0x444444,3,2);
			General_Counter -=1;
		}
	}
	else if (SectionFlag==2)
	{
		WS2801_SetColor(0x00);
		WS2811_SetColorALL(COLORPOOLRAINBOW[45]);
	}
	
	StopTimer();
}
void Sequence_Part28(uint16_t xy) {
	StartTimer_16(1);
	PartStart(28);
	if (General_Counter==0){
		SectionFlag = 1;
		WS2811_SetColorALL(COLORPOOLRAINBOW[0]);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],1,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],1,4);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],2,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],2,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],4,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],4,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,6);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,7);
	}
	
	if (SectionFlag==0){
		StopTimer(); 
		StartTimer_16(xy-1-4);
	}
	else if (SectionFlag==1){
		StopTimer();
		StartTimer_16(75);
		FlashTimer(xy-76-7);
		
		WS2811_Rotate(1,1);
		WS2811_Rotate(1,2);
		WS2811_Rotate(1,3);
		WS2811_Rotate(1,4);
		WS2811_Rotate(1,5);
	}
	
	
	
	
	
	StopTimer();
}
void Sequence_Part30(uint16_t xy){
	StartTimer_16(1);
	PartStart(30);
	if (General_Counter==0){
		SectionFlag = 1;
		WS2811_SetColorALL(COLORPOOLRAINBOW[0]);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],1,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],1,4);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],2,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],2,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],4,2);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],4,3);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,6);
		WS2811_SetGroupPixel(COLORPOOLRAINBOW[68],3,7);
	}
	
	StopTimer();
	if (SectionFlag==0){
		StartTimer_16(xy-1-4);
	}
	else if (SectionFlag==1){
		StartTimer_16(75);
		FlashTimer(xy-76-5);
		WS2811_Rotate(0,1);
		WS2811_Rotate(0,2);
		WS2811_Rotate(0,3);
		WS2811_Rotate(0,4);
		WS2811_Rotate(0,5);
	}
	else if (SectionFlag == 2)
	{
		StartTimer_16(xy-1);
		All_On();
	}
	else if (SectionFlag == 3)
	{
		StartTimer_16(xy-1);
		All_Off();
	}
	StopTimer();
}
void Sequence_Part31(uint16_t xy){
	StartTimer_16(xy-1);
	PartStart(31);
	if (General_Counter==0)
	{
		General_Counter = 1;
		KleurCount = 0;
		UpDown = 0;
	}
	

	
	SectionFlag++;
	
	switch (HelpFlag){
		case (0):
			
			switch (General_Counter){
				case (1):
					LastColor = COLORPOOLRAINBOW[80-(SectionFlag*2)];
					WS2811_SetColorALL(LastColor);
				break;
				case (2):
					WS2811_SetGroupPixel(0x000000,1,2);
					WS2811_SetGroupPixel(0x000000,1,5);
				
					WS2811_SetGroupPixel(0x000000,4,4);
					WS2811_SetGroupPixel(0x000000,4,5);
					WS2811_SetGroupPixel(0x000000,4,8);
					WS2811_SetGroupPixel(0x000000,4,9);
				
					WS2811_SetGroupColor(0x000000,2,0);
					WS2811_SetGroupColor(0x000000,3,1);
				break;
				case (3):
					WS2811_Rotate(1,2);
					WS2811_Rotate(0,3);
					uint32_t NewColor = COLORPOOLRAINBOW[80-(SectionFlag*2)];
					WS2811_Replace(LastColor, NewColor);
					LastColor = NewColor;
				break;
				case (4):
					WS2811_Rotate(1,1);
					WS2811_Rotate(0,4);
				break;
				case (5):
					WS2811_Rotate(1,2);
					WS2811_Rotate(0,3);
					WS2811_Rotate(1,5);
					General_Counter = 0;
				break;
			}
			WS2801_SetColor(0x00);
		break;
		
		case (1):
			switch (General_Counter){
				case (1):
				LastColor = COLORPOOLRAINBOW[80-(SectionFlag*2)];
				WS2811_SetColorALL(LastColor);
				break;
				case (2):
				WS2811_SetGroupPixel(0x000000,1,1);
				WS2811_SetGroupPixel(0x000000,1,4);
				
				WS2811_SetGroupPixel(0x000000,4,1);
				WS2811_SetGroupPixel(0x000000,4,2);
				WS2811_SetGroupPixel(0x000000,4,3);
				WS2811_SetGroupPixel(0x000000,4,4);
				
				WS2811_SetGroupColor(0x000000,2,1);
				WS2811_SetGroupColor(0x000000,3,0);
				break;
				case (3):
				WS2811_Rotate(0,2);
				WS2811_Rotate(1,3);
				uint32_t NewColor = COLORPOOLRAINBOW[80-(SectionFlag*2)];
				WS2811_Replace(LastColor, NewColor);
				LastColor = NewColor;
				break;
				case (4):
				WS2811_Rotate(0,1);
				WS2811_Rotate(1,4);
				break;
				case (5):
				WS2811_Rotate(0,2);
				WS2811_Rotate(1,3);
				WS2811_Rotate(0,5);
				General_Counter = 0;
				break;
			}
			WS2801_SetColor(0x00);
		break;
		case (2):
			WS2811_SetColorALL(0xff0080);
			WS2801_SetColor(0x00);
		break;
		case (3):
			if (WS2801_GetColor()==0x00){
				WS2801_SetColor(0xff);
			}
			else{
				WS2801_SetColor(0x00);
			}
			
			if (SectionFlag==0)
			{
				All_On();
			}
			else if (SectionFlag==1)
			{
				WS2811_SetColorALL(0x000000);
			}
			else if (SectionFlag==2)
			{
				WS2811_SetRight(0x444444);
				WS2811_SetLeft(COLORPOOLRAINBOW[KleurCount]);
			}
			else if (SectionFlag==3)
			{
				WS2811_SetLeft(COLORPOOLRAINBOW[KleurCount]);
				WS2811_SetRight(COLORPOOLRAINBOW[KleurCount]);
			}
			else if (SectionFlag==4)
			{
				WS2811_SetRight(COLORPOOLRAINBOW[KleurCount]);
				WS2811_SetLeft(0x444444);
			}
			else if (SectionFlag==5)
			{
				WS2811_SetColorALL(0x444444);
			}
			else if (SectionFlag==6)
			{
				WS2811_SetLeft(0x444444);
				WS2811_SetRight(COLORPOOLRAINBOW[KleurCount]);
			}
			else if (SectionFlag==7)
			{
				WS2811_SetLeft(COLORPOOLRAINBOW[KleurCount]);
				WS2811_SetRight(COLORPOOLRAINBOW[KleurCount]);
			}
			else if (SectionFlag==8)
			{
				WS2811_SetLeft(COLORPOOLRAINBOW[KleurCount]);
				WS2811_SetRight(0x444444);
				SectionFlag = 0;
			}
			
		break;
		
		case  (4):
			WS2811_SetColorALL(COLORPOOLRAINBOW[KleurCount]);
		break;
		
		case  (5):
		WS2811_SetColorALL(0x000000);
		break;
	}
	
	uint8_t Increment = 4;
	
	if (UpDown==0)
	{
		KleurCount += Increment;
	}
	else if(UpDown==1){
		KleurCount -= Increment;
	}
	
	if (KleurCount>93)
	{
		KleurCount = 93;
		UpDown = 1;
	}
	else if (KleurCount<0)
	{
		KleurCount = 0;
		UpDown = 0;
	}
	
	
	StopTimer();
}
void Sequence_Part32(uint16_t xy){
	StartTimer_16(xy);
	PartStart(32);
	WS2801_SetColor(0x00);
	WS2811_SetColorALL(0x000000);
	StopTimer();
}

void SONGMIX(void){
	while (1)
	{
		uint16_t TimerBijhouden=0;
		uint8_t TimerUpdateCorrection=0; 
		#define UpdateTime 25
		TimerUpdateCorrection = UpdateTime;
		General_Counter=0;
		PartCount = 1;
		SectionFlag=0;
		HelpFlag=0;
		UpDown=0;
		KleurCount=0;
		
	#ifndef DONE
	// Part 1
	All_Off();
	StartTimer_16(700-70);StopTimer();  /* --2--15006--15006-- n */
	// Part 1
	
	StartTimer_16(30); uint8_t Color_Temp; uint8_t LoopCount=80;StopTimer(); /* --3--15927--921-- Begin Loop */
	WS2801_Fade(5,80,1); StopTimer();
	
	// Part 2  
	Sequence_Part2(185); 
	Sequence_Part2(370);  /* --4--16297--370-- Marker 677 */
	Sequence_Part2(418);  /* --5--16715--418-- Marker 678 */ 
	Sequence_Part2(318);  /* --6--17033--318-- 4/4 */
	Sequence_Part2(744); /* --7--17777--744-- Marker 679 */ 
	Sequence_Part2(328); /* --8--18105--328-- Marker 07 */
	Sequence_Part2(362); /* --9--18467--362-- Marker 680 */
	Sequence_Part2(378); /* --10--18845--378-- Marker 681 */
	Sequence_Part2(355); /* --11--19200--355-- Marker 10 */
	Sequence_Part2(1027);  /* --12--20227--1027-- 1/4 */
	Sequence_Part2(435);  /* --13--20662--435-- Marker 682 */
	Sequence_Part2(412);  /* --14--21074--412-- Marker 683 */
	Sequence_Part2(291);  /* --15--21365--291-- Marker 14 */
	Sequence_Part2(778);  /* --16--22143--778-- Marker 684 */
	Sequence_Part2(323);  /* --17--22466--323-- Marker 16 */
	Sequence_Part2(370);  /* --18--22836--370-- Marker 685 */
	Sequence_Part2(397);  /* --19--23233--397-- Marker 686 */
	// Part 2
	
	// Part 3
	Sequence_Part3(1354);  /* --20--24587--1354-- 1/5 */
	Sequence_Part3(394);  /* --21--24981--394-- Marker 687 */
	Sequence_Part3(404);  /* --22--25385--404-- Marker 688 */
	Sequence_Part3(315);  /* --23--25700--315-- Marker 28 */
	Sequence_Part3(800);  /* --24--26500--800-- Marker 689 */
	Sequence_Part3(244);  /* --25--26744--244-- Marker 23 */
	Sequence_Part3(456);  /* --26--27200--456-- Marker 690 */
	Sequence_Part3(400);  /* --27--27600--400-- Marker 691 */
	Sequence_Part3(270);  /* --28--27870--270-- Marker 25 */
	Sequence_Part3(1130);  /* --29--29000--1130-- 1/5 */
	Sequence_Part3(379);  /* --30--29379--379-- Marker 692 */
	Sequence_Part3(391);  /* --31--29770--391-- Marker 693 */
	Sequence_Part3(265);  /* --32--30035--265-- Marker 32 */
	Sequence_Part3(838);  /* --33--30873--838-- Marker 694 */
	Sequence_Part3(289);  /* --34--31162--289-- Marker 36 */
	Sequence_Part3(393);  /* --35--31555--393-- Marker 695 */
	Sequence_Part3(378);  /* --36--31933--378-- Marker 696 */
	Sequence_Part3(1368);  /* --37--33301--1368-- 1/5 */
	Sequence_Part3(436);  /* --38--33737--436-- Marker 697 */
	Sequence_Part3(380);  /* --39--34117--380-- Marker 698 */
	Sequence_Part3(299);  /* --40--34416--299-- Marker 42 */
	Sequence_Part3(828);  /* --41--35244--828-- Marker 699 */
	Sequence_Part3(296);  /* --42--35540--296-- 1/4 */
	Sequence_Part3(178);  /* --43--35718--178-- Marker 871 */
	Sequence_Part3(205);  /* --44--35923--205-- Marker 700 */
	Sequence_Part3(376);  /* --45--36299--376-- Marker 701 */
	Sequence_Part3(336);  /* --46--36635--336-- Marker 48 */
	Sequence_Part3(796);  /* --47--37431--796-- Marker 872 */
	Sequence_Part3(253);  /* --48--37684--253-- 1/4 */
	Sequence_Part3(401);  /* --49--38085--401-- Marker 702 */
	Sequence_Part3(399);  /* --50--38484--399-- Marker 703 */
	Sequence_Part3(319);  /* --51--38803--319-- Marker 53 */
	Sequence_Part3(797);  /* --52--39600--797-- Marker 704 */
	Sequence_Part3(290);  /* --53--39890--290-- Marker 57 */
	Sequence_Part3(368);  /* --54--40258--368-- Marker 706 */
	Sequence_Part3(408);  /* --55--40666--408-- Einde Loop */
	Sequence_Part3(592);  /* --56--41258--592-- Marker 873 */
	// Part 3
	
	
	// Part 4
	Sequence_Part4(742);  /* --57--42000--742-- 1/13 */
	Sequence_Part4(532);  /* --58--42532--532-- Marker 708 */
	Sequence_Part4(321);  /* --59--42853--321-- Marker 733 */
	Sequence_Part4(301);  /* --60--43154--301-- Marker 63 */
	Sequence_Part4(413);  /* --61--43567--413-- Marker 734 */
	Sequence_Part4(344);  /* --62--43911--344-- Marker 735 */
	Sequence_Part4(371);  /* --63--44282--371-- Marker 66 */
	Sequence_Part4(359);  /* --64--44641--359-- Marker 736 */
	Sequence_Part4(379);  /* --65--45020--379-- Marker 737 */
	Sequence_Part4(304);  /* --66--45324--304-- Marker 70 */
	Sequence_Part4(573);  /* --67--45897--573-- Marker 738 */
	Sequence_Part4(549);  /* --68--46446--549-- 1/11 */
	Sequence_Part4(469);  /* --69--46915--469-- Marker 90 */
	Sequence_Part4(364);  /* --70--47279--364-- Marker 91 */
	Sequence_Part4(236);  /* --71--47515--236-- Marker 92 */
	Sequence_Part4(451);  /* --72--47966--451-- Marker 93 */
	Sequence_Part4(377);  /* --73--48343--377-- Marker 94 */
	Sequence_Part4(257);  /* --74--48600--257-- Marker 95 */
	Sequence_Part4(483);  /* --75--49083--483-- Marker 96 */
	Sequence_Part4(361);  /* --76--49444--361-- Marker 97 */
	Sequence_Part4(285);  /* --77--49729--285-- Marker 98 */
	Sequence_Part4(398);  /* --78--50127--398-- 11/11 */
	Sequence_Part4(382);  /* --79--50509--382-- Marker 442 */
	Sequence_Part4(291);  /* --80--50800--291-- 1/12 */
	Sequence_Part4(400);  /* --81--51200--400-- Marker 100 */
	Sequence_Part4(429);  /* --82--51629--429-- Marker 101 */
	Sequence_Part4(271);  /* --83--51900--271-- Marker 102 */
	Sequence_Part4(393);  /* --84--52293--393-- Marker 103 */
	Sequence_Part4(390);  /* --85--52683--390-- Marker 104 */
	Sequence_Part4(285);  /* --86--52968--285-- Marker 105 */
	Sequence_Part4(381);  /* --87--53349--381-- Marker 106 */
	Sequence_Part4(435);  /* --88--53784--435-- Marker 107 */
	Sequence_Part4(240);  /* --89--54024--240-- Marker 108 */
	Sequence_Part4(360);  /* --90--54384--360-- Marker 109 */
	Sequence_Part4(216);  /* --91--54600--216-- Marker 443 */
	Sequence_Part4(282);  /* --92--54882--282-- 12/12 */
	Sequence_Part4(282);  /* --93--55164--282-- 1/10 */
	Sequence_Part4(419);  /* --94--55583--419-- Marker 111 */
	Sequence_Part4(368);  /* --95--55951--368-- Marker 112 */
	Sequence_Part4(280);  /* --96--56231--280-- Marker 113 */
	Sequence_Part4(435);  /* --97--56666--435-- Marker 114 */
	Sequence_Part4(364);  /* --98--57030--364-- Marker 115 */
	Sequence_Part4(298);  /* --99--57328--298-- Marker 116 */
	Sequence_Part4(409);  /* --100--57737--409-- Marker 117 */
	Sequence_Part4(393);  /* --101--58130--393-- Marker 118 */
	Sequence_Part4(293);  /* --102--58423--293-- n */
	Sequence_Part4(777);  /* --103--59200--777-- Marker 874 */
	// Part 4
	
	
	// Part 5
	Sequence_Part5(354);  /* --104--59554--354-- 1/4 */
	Sequence_Part5(446);  /* --105--60000--446-- Marker 78 */
	Sequence_Part5(361);  /* --106--60361--361-- Marker 79 */
	Sequence_Part5(326);  /* --107--60687--326-- Marker 80 */
	Sequence_Part5(795);  /* --108--61482--795-- 1/5 */
	Sequence_Part5(267);  /* --109--61749--267-- Marker 82 */
	Sequence_Part5(419);  /* --110--62168--419-- Marker 83 */
	Sequence_Part5(358);  /* --111--62526--358-- Marker 84 */
	Sequence_Part5(360);  /* --112--62886--360-- Marker 85 */
	Sequence_Part5(1003);  /* --113--63889--1003-- 1/4 */
	Sequence_Part5(428);  /* --114--64317--428-- Marker 87 */
	Sequence_Part5(402);  /* --115--64719--402-- Marker 88 */
	Sequence_Part5(335);  /* --116--65054--335-- Marker 89 */
	Sequence_Part5(766);  /* --117--65820--766-- 1/4 */
	Sequence_Part5(275);  /* --118--66095--275-- Marker 120 */
	Sequence_Part5(398);  /* --119--66493--398-- Marker 121 */
	Sequence_Part5(417);  /* --120--66910--417-- Marker 122 */
	Sequence_Part5(1018);  /* --121--67928--1018-- Marker 875 */
	// Part 5
	
	SectionFlag = 1;
	// Part 6
	Sequence_Part6(372);StopTimer();  /* --122--68307--379-- Marker 124 */
	Sequence_Part6(268);StopTimer();  /* --123--68573--266-- Marker 876 */
	Sequence_Part6(273);StopTimer();  /* --124--68844--271-- Marker 126 */
	Sequence_Part6(304);StopTimer();  /* --125--69148--304-- Marker 877 */
	Sequence_Part6(254);StopTimer();  /* --126--69400--252-- Marker 128 */
	Sequence_Part6(285);StopTimer();
	Sequence_Part6(207);SectionFlag=2;StopTimer();  /* --127--69883--483-- Marker 130 */
	
	Sequence_Part6(307);StopTimer();  /* --128--70200--317-- Marker 878 */
	Sequence_Part6(284);StopTimer();  /* --129--70484--284-- Marker 131 */
	Sequence_Part6(396);StopTimer();  /* --130--70880--396-- Marker 132 */
	Sequence_Part6(357);StopTimer();  /* --131--71237--357-- Marker 133 */
	Sequence_Part6(309);StopTimer();  /* --132--71546--309-- Marker 134 */
	Sequence_Part6(490);SectionFlag=1;StopTimer();  /* --133--72036--490-- Marker 135 */
	
	Sequence_Part6(337);StopTimer();  /* --134--72373--337-- Marker 879 */
	Sequence_Part6(259);StopTimer();  /* --135--72632--259-- Marker 138 */
	Sequence_Part6(268);StopTimer();  /* --136--72900--268-- Marker 444 */
	Sequence_Part6(181);StopTimer();  /* --137--73081--181-- Marker 139 */
	Sequence_Part6(379);StopTimer();  /* --138--73460--379-- Marker 445 */
	Sequence_Part6(271);StopTimer();  /* --139--73731--271-- Marker 141 */
	Sequence_Part6(244);StopTimer();  /* --140--73975--244-- Marker 880 */
	Sequence_Part6(190);SectionFlag=2;StopTimer();  /* --141--74160--185-- Marker 143 */
	
	Sequence_Part6(369);StopTimer();  /* --142--74529--369-- Marker 881 */
	Sequence_Part6(311);StopTimer();  /* --143--74840--311-- Marker 145 */
	Sequence_Part6(360);StopTimer();  /* --144--75200--360-- Marker 146 */
	Sequence_Part6(412);StopTimer();  /* --145--75612--412-- Marker 147 */
	Sequence_Part6(253);StopTimer();  /* --146--75865--253-- Marker 148 */
	Sequence_Part6(318);SectionFlag=3;StopTimer();  /* --147--76183--318-- Marker 446 */
	
	Sequence_Part6(536);StopTimer();  /* --148--76717--534-- 1/ */
	Sequence_Part6(305);StopTimer();  /* --149--77022--305-- Marker 150 */
	Sequence_Part6(280);StopTimer();  /* --150--77300--278-- Marker 151 */
	Sequence_Part6(244);StopTimer();  /* --151--77544--244-- Marker 447 */
	Sequence_Part6(285);StopTimer();  /* --152--77827--283-- Marker 152 */
	Sequence_Part6(244);StopTimer();  /* --153--78071--244-- Marker 153 */
	Sequence_Part6(284);StopTimer();  /* --154--78353--282-- Marker 154 */
	Sequence_Part6(266);SectionFlag=2;StopTimer();  /* --155--78615--262-- Marker 155 */
	
	Sequence_Part6(302);StopTimer();  /* --156--78917--302-- 1/ */
	Sequence_Part6(136);StopTimer();  /* --157--79050--133-- Marker 882 */
	Sequence_Part6(152);StopTimer();  /* --158--79200--150-- Marker 157 */
	Sequence_Part6(374);StopTimer();  /* --159--79574--374-- Marker 158 */
	Sequence_Part6(380);StopTimer();  /* --160--79952--378-- Marker 159 */
	Sequence_Part6(276);StopTimer();  /* --161--80223--271-- Marker 160 */
	Sequence_Part6(526);SectionFlag=1;StopTimer();  /* --162--80742--519-- Marker 161 */
	
	Sequence_Part6(317);StopTimer();  /* --163--81059--317-- Marker 883 */
	Sequence_Part6(268);StopTimer();  /* --164--81327--268-- Marker 163 */
	Sequence_Part6(291);StopTimer();  /* --165--81618--291-- Marker 164 */
	Sequence_Part6(182);StopTimer();  /* --166--81800--182-- Marker 448 */
	Sequence_Part6(358);StopTimer();  /* --167--82156--356-- Marker 165 */
	Sequence_Part6(260);StopTimer();  /* --168--82416--260-- Marker 166 */
	Sequence_Part6(271);StopTimer();  /* --169--82687--271-- Marker 167 */
	Sequence_Part6(205);SectionFlag=2;StopTimer();  /* --170--82886--199-- Marker 168 */
	
	Sequence_Part6(350);StopTimer();  /* --171--83236--350-- 1/ */
	Sequence_Part6(153);StopTimer();  /* --172--83389--153-- Marker 884 */
	Sequence_Part6(132);StopTimer();  /* --173--83521--132-- n */
	Sequence_Part6(279);StopTimer();  /* --174--83800--279-- Marker 885 */
	Sequence_Part6(127);StopTimer();  /* --175--83927--127-- Marker 171 */
	Sequence_Part6(373);StopTimer();  /* --176--84300--373-- Marker 172 */
	Sequence_Part6(285);StopTimer();  /* --177--84585--285-- Marker 173 */
	Sequence_Part6(291);StopTimer();  /* --178--84876--291-- 1/ */
	Sequence_Part6(202);StopTimer();  /* --179--85076--200-- Marker 886 */
	// Part 6
	
	// Part 7
	Sequence_Part7(343);  /* --180--85419--343-- Marker 887 */
	Sequence_Part7(342);  /* --181--85761--342-- Marker 449 */
	SectionFlag = 3;
	Sequence_Part7(439);  /* --182--86200--439-- 1/ */
	SectionFlag = 4;
	Sequence_Part7(633);  /* --183--86833--633-- Marker 178 */
	SectionFlag = 3;
	Sequence_Part7(460);  /* --184--87293--460-- Marker 179 */
	SectionFlag = 4;
	Sequence_Part7(607);  /* --185--87900--607-- Marker 180 */
	SectionFlag = 1;
	Sequence_Part7(334);  /* --186--88234--334-- Marker 181 */
	Sequence_Part7(434);  /* --187--88668--434-- Marker 182 */
	Sequence_Part7(309);  /* --188--88977--309-- Marker 183 */
	Sequence_Part7(423);  /* --189--89400--423-- Marker 888 */
	Sequence_Part7(359);  /* --190--89863--463-- Marker 186 */
	Sequence_Part7(356);  /* --191--90116--253-- Marker 450 */
	SectionFlag = 3;
	Sequence_Part7(446);  /* --192--90562--446-- Marker 187 */
	SectionFlag = 4;
	Sequence_Part7(618);  /* --193--91180--618-- 1/ */
	SectionFlag = 3; 
	Sequence_Part7(469);  /* --194--91649--469-- Marker 189 */
	SectionFlag = 4;
	Sequence_Part7(586);  /* --195--92235--586-- Marker 190 */
	SectionFlag = 1;
	Sequence_Part7(365);  /* --196--92600--365-- Marker 191 */
	Sequence_Part7(461);  /* --197--93061--461-- Marker 192 */
	Sequence_Part7(282);  /* --198--93343--282-- Marker 193 */
	SectionFlag = 5;
	Sequence_Part7(1105);  /* --199--94448--1105-- 1/ */
	// Part 7
	
	// Part 8
	WS2801_Fade(53,15,0);
	// Sequence_Part8(1025);  /* --200--95473--1025-- Marker 889 */
	// 
	// Sequence_Part8(591);  /* --201--96064--591-- Marker 194 */
	SectionFlag = 2;
	Sequence_Part8(583);  /* --202--96647--583-- n */
	// Part 8
	
	// Part 9
	SectionFlag = 1;
	Sequence_Part9(453);  /* --203--97100--453-- Marker 196 */
	Sequence_Part9(400);  /* --204--97500--400-- Marker 197 */
	Sequence_Part9(276);  /* --205--97776--276-- Marker 198 */
	Sequence_Part9(424);  /* --206--98200--424-- Marker 199 */
	Sequence_Part9(370);  /* --207--98570--370-- Marker 200 */
	Sequence_Part9(296);  /* --208--98866--296-- Marker 201 */
	Sequence_Part9(415);  /* --209--99281--415-- Marker 205 */
	Sequence_Part9(386);  /* --210--99667--386-- Marker 207 */
	Sequence_Part9(266);  /* --211--99933--266-- Marker 208 */
	Sequence_Part9(495);  /* --212--100428--495-- Marker 209 */
	
	Sequence_Part9(572);  /* --213--101000--572-- Marker 203 */
	SectionFlag = 2;
	Sequence_Part9(450);  /* --214--101450--450-- Marker 211 */
	SectionFlag = 1;
	Sequence_Part9(388);  /* --215--101838--388-- Marker 212 */
	Sequence_Part9(269);  /* --216--102107--269-- Marker 213 */
	Sequence_Part9(406);  /* --217--102513--406-- Marker 214 */
	Sequence_Part9(387);  /* --218--102900--387-- Marker 215 */
	Sequence_Part9(300);  /* --219--103200--300-- Marker 216 */
	Sequence_Part9(426);  /* --220--103626--426-- Marker 217 */
	Sequence_Part9(374);  /* --221--104000--374-- Marker 218 */
	Sequence_Part9(288);  /* --222--104288--288-- Marker 219 */
	Sequence_Part9(452);  /* --223--104740--452-- Marker 220 */
	Sequence_Part9(361);  /* --224--105101--361-- Marker 221 */
	
	Sequence_Part9(270);  /* --225--105371--270-- Marker 222 */
	SectionFlag = 2;
	Sequence_Part9(429);  /* --226--105800--429-- Marker 223 */
	SectionFlag = 1;
	Sequence_Part9(384);  /* --227--106184--384-- Marker 224 */
	Sequence_Part9(277);  /* --228--106461--277-- Marker 225 */
	Sequence_Part9(405);  /* --229--106866--405-- Marker 226 */
	Sequence_Part9(400);  /* --230--107266--400-- Marker 227 */
	Sequence_Part9(280);  /* --231--107546--280-- Marker 228 */
	Sequence_Part9(386);  /* --232--107932--386-- Marker 229 */
	Sequence_Part9(450);  /* --233--108382--450-- Marker 230 */
	Sequence_Part9(270);  /* --234--108652--270-- Marker 231 */
	Sequence_Part9(348);  /* --235--109000--348-- Marker 232 */
	SectionFlag = 2;
	Sequence_Part9(731);  /* --236--109731--731-- Marker 234 */
	SectionFlag = 1;
	Sequence_Part9(442);  /* --237--110173--442-- Marker 451 */
	SectionFlag = 2;
	Sequence_Part9(385);  /* --238--110558--385-- Marker 235 */
	
	Sequence_Part9(260);  /* --239--110818--260-- Marker 236 */
	Sequence_Part9(364);  /* --240--111182--364-- Marker 237 */
	Sequence_Part9(455);  /* --241--111637--455-- Marker 238 */
	Sequence_Part9(240);  /* --242--111877--240-- Marker 239 */
	Sequence_Part9(439);  /* --243--112316--439-- Marker 240 */
	Sequence_Part9(395);  /* --244--112711--395-- Marker 452 */
	
	Sequence_Part9(275);  /* --245--112986--275-- Marker 453 */
	SectionFlag = 1;
	Sequence_Part9(167);  /* --246--113303--317-- Marker 890 */
	// Part 9
	
	// Part 10
	//Sequence_Part10(3760);  /* --247--117063--3760-- Lights fully out */
	Sequence_Part10(1320);  /* --247--117063--3760-- Lights fully out */
	Sequence_Part10(10330);  /* --248--127393--10330-- Storing */
	Sequence_Part10(172);  /* --249--127565--172-- Marker 246 */
	Sequence_Part10(268);  /* --250--127833--268-- n */
	Sequence_Part10(331);  /* --251--128164--331-- Marker 248 */
	Sequence_Part10(277);  /* --252--128441--277-- Marker 249 */
	Sequence_Part10(307);  /* --253--128748--307-- AAN */
	// Part 10
	
	// Part 11
	Sequence_Part11(1120);  /* --254--129900--1152-- Marker 250 */
	// Part 11
	
	// Part 12
	Sequence_Part12(279);  /* --255--130179--279-- Marker 251 */
	Sequence_Part12(186);  /* --256--130365--186-- Marker 252 */
	Sequence_Part12(249);  /* --257--130614--249-- Marker 253 */
	Sequence_Part12(226);  /* --258--130840--226-- Marker 254 */
	Sequence_Part12(230);  /* --259--131070--230-- Marker 255 */
	Sequence_Part12(230);  /* --260--131300--230-- Marker 256 */
	Sequence_Part12(100);  /* --261--131400--100-- Marker 257 */
	Sequence_Part12(131);  /* --262--131531--131-- Marker 258 */
	Sequence_Part12(243);  /* --263--131774--243-- Marker 259 */
	Sequence_Part12(226);  /* --264--132000--226-- Marker 260 */
	Sequence_Part12(226);  /* --265--132226--226-- Marker 261 */
	Sequence_Part12(235);  /* --266--132461--235-- Marker 262 */
	Sequence_Part12(239);  /* --267--132700--239-- Marker 263 */
	Sequence_Part12(238);  /* --268--132938--238-- Marker 264 */
	Sequence_Part12(218);  /* --269--133156--218-- Marker 265 */
	Sequence_Part12(244);  /* --270--133400--244-- Marker 266 */
	Sequence_Part12(217);  /* --271--133617--217-- Marker 267 */
	Sequence_Part12(241);  /* --272--133858--241-- Marker 268 */
	Sequence_Part12(245);  /* --273--134103--245-- Marker 269 */
	Sequence_Part12(232);  /* --274--134335--232-- Marker 270 */
	Sequence_Part12(236);  /* --275--134571--236-- Marker 271 */
	Sequence_Part12(120);  /* --276--134691--120-- Marker 272 */
	Sequence_Part12(109);  /* --277--134800--109-- Marker 273 */
	Sequence_Part12(232);  /* --278--135032--232-- Marker 274 */
	Sequence_Part12(48);  /* --279--135080--48-- Marker 275 */
	Sequence_Part12(70);  /* --280--135150--70-- Marker 276 */
	Sequence_Part12(112);  /* --281--135262--112-- Marker 277 */
	Sequence_Part12(238);  /* --282--135500--238-- Marker 278 */
	Sequence_Part12(244);  /* --283--135744--244-- Marker 279 */
	Sequence_Part12(223);  /* --284--135967--223-- Marker 280 */
	Sequence_Part12(233);  /* --285--136200--233-- Marker 281 */
	Sequence_Part12(222);  /* --286--136422--222-- Marker 282 */
	Sequence_Part12(246);  /* --287--136668--246-- Marker 283 */
	Sequence_Part12(99);  /* --288--136767--99-- Marker 454 */
	Sequence_Part12(111);  /* --289--136878--111-- Marker 284 */
	Sequence_Part12(252);  /* --290--137130--252-- Marker 285 */
	Sequence_Part12(97);  /* --291--137227--97-- Marker 286 */
	Sequence_Part12(127);  /* --292--137354--127-- Death March */
	// Part 12
	
	// Part 13
	Sequence_Part13(486);  /* --293--137840--486-- Marker 288 */
	Sequence_Part13(232);  /* --294--138072--232-- Marker 891 */
	Sequence_Part13(245);  /* --295--138317--245-- Marker 289 */
	Sequence_Part13(235);  /* --296--138552--235-- Marker 892 */
	Sequence_Part13(211);  /* --297--138763--211-- Marker 290 */
	Sequence_Part13(220);  /* --298--138983--220-- Marker 893 */
	Sequence_Part13(217);  /* --299--139200--217-- Marker 291 */
	Sequence_Part13(268);  /* --300--139468--268-- Marker 894 */
	Sequence_Part13(204);  /* --301--139672--204-- Marker 293 */
	Sequence_Part13(228);  /* --302--139900--228-- Marker 895 */
	Sequence_Part13(262);  /* --303--140162--262-- Marker 295 */
	Sequence_Part13(256);  /* --304--140418--256-- Marker 896 */
	Sequence_Part13(182);  /* --305--140600--182-- Marker 297 */
	Sequence_Part13(258);  /* --306--140858--258-- Marker 897 */
	Sequence_Part13(210);  /* --307--141068--210-- 1/ */
	Sequence_Part13(245);  /* --308--141313--245-- Marker 898 */
	Sequence_Part13(252);  /* --309--141565--252-- Marker 302 */
	Sequence_Part13(235);  /* --310--141800--235-- Marker 899 */
	Sequence_Part13(252);  /* --311--142052--252-- Marker 304 */
	Sequence_Part13(110);  /* --312--142162--110-- Marker 900 */
	Sequence_Part13(128);  /* --313--142290--128-- Marker 901 */
	Sequence_Part13(210);  /* --314--142500--210-- Marker 307 */
	Sequence_Part13(245);  /* --315--142745--245-- Marker 902 */
	Sequence_Part13(216);  /* --316--142961--216-- Marker 309 */
	Sequence_Part13(277);  /* --317--143238--277-- Marker 455 */
	Sequence_Part13(226);  /* --318--143464--226-- Marker 456 */
	SectionFlag = 5;
	Sequence_Part13(224);  /* --319--143688--224-- Marker 457 */
	SectionFlag = 6;
	Sequence_Part13(234);  /* --320--143922--234-- Marker 313 */
	// Part 13
	
	// Part 14
	Sequence_Part14(741);  /* --321--144663--741-- Marker 458 */
	// Part 14
	
	// Part 15
	Sequence_Part15(206);  /* --322--144869--206-- 1/ */
	Sequence_Part15(270);  /* --323--145139--270-- Marker 315 */
	Sequence_Part15(217);  /* --324--145356--217-- Marker 316 */
	Sequence_Part15(212);  /* --325--145568--212-- Marker 317 */
	Sequence_Part15(267);  /* --326--145835--267-- Marker 318 */
	Sequence_Part15(211);  /* --327--146046--211-- Marker 320 */
	Sequence_Part15(276);  /* --328--146322--276-- Marker 319 */
	Sequence_Part15(223);  /* --329--146545--223-- Marker 321 */
	Sequence_Part15(228);  /* --330--146773--228-- Marker 322 */
	Sequence_Part15(266);  /* --331--147039--266-- Marker 903 */
	Sequence_Part15(244);  /* --332--147283--244-- Marker 324 */
	Sequence_Part15(192);  /* --333--147475--192-- Marker 904 */
	Sequence_Part15(244);  /* --334--147719--244-- Marker 327 */
	Sequence_Part15(251);  /* --335--147970--251-- Marker 905 */
	Sequence_Part15(230);  /* --336--148200--230-- Marker 329 */
	Sequence_Part15(226);  /* --337--148426--226-- n */
	Sequence_Part15(231);  /* --338--148657--231-- Marker 331 */
	Sequence_Part15(257);  /* --339--148914--257-- Marker 907 */
	Sequence_Part15(241);  /* --340--149155--241-- Marker 333 */
	Sequence_Part15(229);  /* --341--149384--229-- Marker 908 */
	Sequence_Part15(260);  /* --342--149644--260-- Marker 335 */
	Sequence_Part15(240);  /* --343--149884--240-- Marker 909 */
	Sequence_Part15(243);  /* --344--150127--243-- Marker 337 */
	Sequence_Part15(241);  /* --345--150368--241-- Marker 910 */
	Sequence_Part15(212);  /* --346--150580--212-- Marker 338 */
	Sequence_Part15(264);  /* --347--150844--264-- Marker 911 */
	Sequence_Part15(254);  /* --348--151098--254-- Marker 340 */
	Sequence_Part15(288);  /* --349--151386--288-- Marker 912 */
	Sequence_Part15(206);  /* --350--151592--206-- Marker 342 */
	Sequence_Part15(442);  /* --351--152034--442-- Marker 344 */
	Sequence_Part15(245);  /* --352--152279--245-- Marker 459 */
	// Part 15
	
	// Part 16
	Sequence_Part16(321);  /* --353--152600--321-- Marker 314 */
	Sequence_Part16(450);  /* --354--153050--450-- Marker 348 */
	Sequence_Part16(490);  /* --355--153540--490-- Marker 349 */
	Sequence_Part16(480);  /* --356--154020--480-- Marker 350 */
	Sequence_Part16(265);  /* --357--154285--265-- Marker 351 */
	Sequence_Part16(238);  /* --358--154523--238-- Marker 352 */
	Sequence_Part16(500);  /* --359--155023--500-- Marker 353 */
	Sequence_Part16(545);  /* --360--155568--545-- Marker 913 */
	Sequence_Part16(432);  /* --361--156000--432-- Marker 346 */
	Sequence_Part16(294);  /* --362--156294--294-- Marker 460 */
	Sequence_Part16(243);  /* --363--156537--243-- Marker 345 */
	Sequence_Part16(236);  /* --364--156773--236-- Marker 914 */
	Sequence_Part16(199);  /* --365--156972--199-- Marker 354 */
	Sequence_Part16(271);  /* --366--157243--271-- Marker 915 */
	Sequence_Part16(229);  /* --367--157472--229-- Marker 355 */
	Sequence_Part16(304);  /* --368--157776--304-- Marker 916 */
	Sequence_Part16(168);  /* --369--157944--168-- Marker 356 */
	Sequence_Part16(279);  /* --370--158223--279-- Marker 917 */
	Sequence_Part16(239);  /* --371--158462--239-- Marker 357 */
	Sequence_Part16(338);  /* --372--158800--338-- Marker 918 */
	Sequence_Part16(142);  /* --373--158942--142-- Marker 358 */
	Sequence_Part16(258);  /* --374--159200--258-- Marker 919 */
	Sequence_Part16(252);  /* --375--159452--252-- Marker 359 */
	Sequence_Part16(291);  /* --376--159743--291-- Marker 920 */
	Sequence_Part16(176);  /* --377--159919--176-- Marker 360 */
	Sequence_Part16(257);  /* --378--160176--257-- Marker 921 */
	Sequence_Part16(186);  /* --379--160362--186-- Marker 361 */
	Sequence_Part16(309);  /* --380--160671--309-- Marker 922 */
	Sequence_Part16(149);  /* --381--160820--149-- Marker 362 */
	Sequence_Part16(80);  /* --382--160900--80-- Marker 923 */
	Sequence_Part16(256);  /* --383--161156--256-- Marker 924 */
	Sequence_Part16(222);  /* --384--161378--222-- Marker 363 */
	Sequence_Part16(266);  /* --385--161644--266-- Marker 925 */
	Sequence_Part16(238);  /* --386--161882--238-- Marker 364 */
	Sequence_Part16(218);  /* --387--162100--218-- Marker 926 */
	Sequence_Part16(153);  /* --388--162253--153-- Marker 927 */
	Sequence_Part16(104);  /* --389--162357--104-- Marker 365 */
	Sequence_Part16(273);  /* --390--162630--273-- Marker 928 */
	Sequence_Part16(210);  /* --391--162840--210-- Marker 366 */
	Sequence_Part16(260);  /* --392--163100--260-- Marker 929 */
	Sequence_Part16(227);  /* --393--163327--227-- Marker 367 */
	Sequence_Part16(247);  /* --394--163574--247-- Marker 930 */
	Sequence_Part16(239);  /* --395--163813--239-- Marker 368 */
	Sequence_Part16(246);  /* --396--164059--246-- Marker 931 */
	Sequence_Part16(227);  /* --397--164286--227-- Marker 369 */
	Sequence_Part16(229);  /* --398--164515--229-- Marker 932 */
	Sequence_Part16(253);  /* --399--164768--253-- Marker 370 */
	Sequence_Part16(271);  /* --400--165039--271-- Marker 933 */
	Sequence_Part16(219);  /* --401--165258--219-- Marker 371 */
	Sequence_Part16(242);  /* --402--165500--242-- Marker 934 */
	Sequence_Part16(269);  /* --403--165769--269-- Marker 372 */
	Sequence_Part16(247);  /* --404--166016--247-- Marker 935 */
	Sequence_Part16(195);  /* --405--166211--195-- Marker 373 */
	Sequence_Part16(310);  /* --406--166521--310-- Marker 936 */
	Sequence_Part16(167);  /* --407--166688--167-- Marker 374 */
	Sequence_Part16(158);  /* --408--166846--158-- Marker 937 */
	Sequence_Part16(154);  /* --409--167000--154-- Marker 461 */
	Sequence_Part16(217);  /* --410--167217--217-- Marker 375 */
	// Part 16
	
	// Part 17
	Sequence_Part17(258);  /* --411--167475--258-- Marker 938 */
	Sequence_Part17(237);  /* --412--167712--237-- Marker 376 */
	Sequence_Part17(242);  /* --413--167954--242-- Marker 939 */
	Sequence_Part17(277);  /* --414--168231--277-- Marker 377 */
	Sequence_Part17(243);  /* --415--168474--243-- Marker 940 */
	Sequence_Part17(180);  /* --416--168654--180-- Marker 378 */
	Sequence_Part17(298);  /* --417--168952--298-- Marker 941 */
	Sequence_Part17(245);  /* --418--169197--245-- Marker 379 */
	Sequence_Part17(264);  /* --419--169461--264-- Marker 942 */
	Sequence_Part17(184);  /* --420--169645--184-- Marker 380 */
	Sequence_Part17(279);  /* --421--169924--279-- Marker 943 */
	Sequence_Part17(231);  /* --422--170155--231-- Marker 381 */
	Sequence_Part17(260);  /* --423--170415--260-- Marker 944 */
	Sequence_Part17(185);  /* --424--170600--185-- Marker 347 */
	Sequence_Part17(241);  /* --425--170841--241-- Marker 946 */
	// Part 17
	
	// Part 18
	Sequence_Part18(141);  /* --426--170982--141-- Marker 945 */
	Sequence_Part18(190);  /* --427--171172--190-- Marker 382 */
	Sequence_Part18(440);  /* --428--171612--440-- Marker 383 */
	Sequence_Part18(213);  /* --429--171825--213-- Marker 947 */
	Sequence_Part18(228);  /* --430--172053--228-- Marker 384 */
	Sequence_Part18(312);  /* --431--172365--312-- Marker 948 */
	Sequence_Part18(208);  /* --432--172573--208-- Marker 385 */
	Sequence_Part18(367);  /* --433--172940--367-- Marker 949 */
	Sequence_Part18(260);  /* --434--173200--260-- Marker 386 */
	Sequence_Part18(277);  /* --435--173477--277-- Marker 950 */
	Sequence_Part18(208);  /* --436--173685--208-- Marker 387 */
	Sequence_Part18(227);  /* --437--173912--227-- Marker 951 */
	Sequence_Part18(193);  /* --438--174105--193-- Marker 388 */
	Sequence_Part18(263);  /* --439--174368--263-- Marker 952 */
	Sequence_Part18(232);  /* --440--174600--232-- Marker 389 */
	Sequence_Part18(400);  /* --441--175000--400-- Marker 953 */
	Sequence_Part18(155);  /* --442--175155--155-- Marker 390 */
	Sequence_Part18(245);  /* --443--175400--245-- Marker 954 */
	Sequence_Part18(254);  /* --444--175654--254-- Marker 391 */
	Sequence_Part18(308);  /* --445--175962--308-- Marker 955 */
	Sequence_Part18(290);  /* --446--176252--290-- Marker 392 */
	Sequence_Part18(348);  /* --447--176600--348-- Marker 393 */
	Sequence_Part18(400);  /* --448--177000--400-- Marker 394 */
	Sequence_Part18(200);  /* --449--177200--200-- Marker 956 */
	Sequence_Part18(239);  /* --450--177439--239-- Marker 957 */
	Sequence_Part18(148);  /* --451--177587--148-- Marker 395 */
	Sequence_Part18(413);  /* --452--178000--413-- Marker 396 */
	Sequence_Part18(319);  /* --453--178319--319-- Marker 958 */
	Sequence_Part18(227);  /* --454--178546--227-- Marker 397 */
	Sequence_Part18(525);  /* --455--179071--525-- Marker 398 */
	Sequence_Part18(460);  /* --456--179531--460-- Marker 399 */
	Sequence_Part18(469);  /* --457--180000--469-- n */
	Sequence_Part18(547);  /* --458--180547--547-- Marker 401 */
	Sequence_Part18(291);  /* --459--180838--291-- Marker 959 */
	Sequence_Part18(204);  /* --460--181042--204-- Marker 402 */
	// Part 18
	
	// Part 19
	Sequence_Part19(290);  /* --461--181332--290-- Marker 403 */
	Sequence_Part19(203);  /* --462--181535--203-- Marker 960 */
	Sequence_Part19(265);  /* --463--181800--265-- Marker 961 */
	Sequence_Part19(244);  /* --464--182044--244-- Marker 404 */
	Sequence_Part19(222);  /* --465--182266--222-- Marker 962 */
	Sequence_Part19(235);  /* --466--182501--235-- Marker 405 */
	Sequence_Part19(258);  /* --467--182759--258-- Marker 963 */
	Sequence_Part19(245);  /* --468--183004--245-- Marker 406 */
	Sequence_Part19(196);  /* --469--183200--196-- Marker 964 */
	Sequence_Part19(210);  /* --470--183410--210-- Marker 407 */
	Sequence_Part19(290);  /* --471--183700--290-- Marker 965 */
	Sequence_Part19(243);  /* --472--183943--243-- Marker 408 */
	Sequence_Part19(237);  /* --473--184180--237-- Marker 462 */
	Sequence_Part19(440);  /* --474--184620--440-- Marker 966 */
	Sequence_Part19(314);  /* --475--184934--314-- Marker 409 */
	Sequence_Part19(243);  /* --476--185177--243-- Marker 967 */
	Sequence_Part19(177);  /* --477--185354--177-- Marker 410 */
	Sequence_Part19(246);  /* --478--185600--246-- Marker 968 */
	Sequence_Part19(235);  /* --479--185835--235-- Marker 411 */
	Sequence_Part19(265);  /* --480--186100--265-- Marker 969 */
	Sequence_Part19(200);  /* --481--186300--200-- Marker 412 */
	Sequence_Part19(276);  /* --482--186576--276-- Marker 970 */
	Sequence_Part19(210);  /* --483--186786--210-- Marker 413 */
	Sequence_Part19(230);  /* --484--187016--230-- Marker 971 */
	Sequence_Part19(282);  /* --485--187298--282-- Marker 414 */
	Sequence_Part19(218);  /* --486--187516--218-- Marker 972 */
	Sequence_Part19(213);  /* --487--187729--213-- Marker 415 */
	Sequence_Part19(286);  /* --488--188015--286-- Marker 973 */
	Sequence_Part19(207);  /* --489--188222--207-- Marker 416 */
	Sequence_Part19(252);  /* --490--188474--252-- Marker 974 */
	Sequence_Part19(230);  /* --491--188704--230-- Marker 417 */
	Sequence_Part19(259);  /* --492--188963--259-- Marker 975 */
	Sequence_Part19(199);  /* --493--189162--199-- Marker 418 */
	Sequence_Part19(261);  /* --494--189423--261-- Marker 976 */
	Sequence_Part19(210);  /* --495--189633--210-- Marker 419 */
	Sequence_Part19(244);  /* --496--189877--244-- Marker 977 */
	Sequence_Part19(249);  /* --497--190126--249-- Marker 420 */
	Sequence_Part19(270);  /* --498--190396--270-- Marker 421 */
	Sequence_Part19(225);  /* --499--190621--225-- Marker 422 */
	Sequence_Part19(364);  /* --500--190985--364-- Marker 978 */
	Sequence_Part19(94);  /* --501--191079--94-- Marker 423 */
	Sequence_Part19(271);  /* --502--191350--271-- Marker 979 */
	Sequence_Part19(250);  /* --503--191600--250-- Marker 424 */
	Sequence_Part19(242);  /* --504--191842--242-- Marker 425 */
	Sequence_Part19(427);  /* --505--192269--427-- Marker 980 */
	Sequence_Part19(294);  /* --506--192563--294-- Marker 427 */
	Sequence_Part19(237);  /* --507--192800--237-- Marker 981 */
	Sequence_Part19(202);  /* --508--193002--202-- Marker 428 */
	Sequence_Part19(253);  /* --509--193255--253-- Marker 982 */
	Sequence_Part19(232);  /* --510--193487--232-- Marker 429 */
	Sequence_Part19(213);  /* --511--193700--213-- Marker 983 */
	Sequence_Part19(300);  /* --512--194000--300-- Marker 430 */
	Sequence_Part19(265);  /* --513--194265--265-- Marker 984 */
	Sequence_Part19(216);  /* --514--194481--216-- Marker 431 */
	Sequence_Part19(254);  /* --515--194735--254-- Marker 985 */
	Sequence_Part19(169);  /* --516--194904--169-- Marker 432 */
	Sequence_Part19(276);  /* --517--195180--276-- Marker 986 */
	Sequence_Part19(227);  /* --518--195407--227-- Marker 433 */
	Sequence_Part19(253);  /* --519--195660--253-- Marker 987 */
	Sequence_Part19(242);  /* --520--195902--242-- Marker 434 */
	Sequence_Part19(266);  /* --521--196168--266-- Marker 988 */
	Sequence_Part19(247);  /* --522--196415--247-- Marker 435 */
	Sequence_Part19(308);  /* --523--196723--308-- Marker 989 */
	Sequence_Part19(108);  /* --524--196831--108-- Marker 436 */
	Sequence_Part19(339);  /* --525--197170--339-- Marker 990 */
	Sequence_Part19(184);  /* --526--197354--184-- Marker 437 */
	Sequence_Part19(246);  /* --527--197600--246-- Marker 991 */
	Sequence_Part19(264);  /* --528--197864--264-- Marker 438 */
	Sequence_Part19(256);  /* --529--198120--256-- Marker 463 */
	Sequence_Part19(220);  /* --530--198340--220-- Marker 992 */
	Sequence_Part19(322);  /* --531--198662--322-- Marker 464 */
	Sequence_Part19(138);  /* --532--198800--138-- Marker 465 */
	Sequence_Part19(290);  /* --533--199090--290-- Marker 993 */
	Sequence_Part19(229);  /* --534--199319--229-- Marker 466 */
	Sequence_Part19(293);  /* --535--199612--293-- Marker 994 */
	Sequence_Part19(234);  /* --536--199846--234-- Marker 467 */
	Sequence_Part19(244);  /* --537--200090--244-- Marker 995 */
	Sequence_Part19(248);  /* --538--200338--248-- Marker 468 */
	Sequence_Part19(250);  /* --539--200588--250-- Marker 996 */
	Sequence_Part19(225);  /* --540--200813--225-- Marker 469 */
	Sequence_Part19(268);  /* --541--201081--268-- Marker 997 */
	Sequence_Part19(231);  /* --542--201312--231-- Marker 470 */
	Sequence_Part19(258);  /* --543--201570--258-- Marker 998 */
	Sequence_Part19(227);  /* --544--201797--227-- Marker 471 */
	Sequence_Part19(272);  /* --545--202069--272-- Marker 999 */
	Sequence_Part19(130);  /* --546--202199--130-- Marker 472 */
	Sequence_Part19(361);  /* --547--202560--361-- Marker 473 */
	Sequence_Part19(246);  /* --548--202806--246-- Marker 474 */
	Sequence_Part19(249);  /* --549--203055--249-- Marker 475 */
	Sequence_Part19(222);  /* --550--203277--222-- Marker 476 */
	// Part 19
	
	// Part 20
	Sequence_Part20(1158);  /* --551--204435--1158-- Marker 477 */
	// Part 20
	// Part 21
	SectionFlag = 1;
	Sequence_Part21(174);  /* --552--204609--174-- Marker 487 */
	SectionFlag = 0;
	Sequence_Part21(127);  /* --553--204736--127-- Marker 1000 */
	Sequence_Part21(132);  /* --554--204868--132-- Marker 488 */
	Sequence_Part21(115);  /* --555--204983--115-- Marker 1001 */ 
	Sequence_Part21(107);  /* --556--205090--107-- Marker 485 */
	Sequence_Part21(110);  /* --557--205200--110-- Marker 1002 */
	Sequence_Part21(101);  /* --558--205301--101-- Marker 484 */
	Sequence_Part21(130);  /* --559--205431--130-- Marker 1003 */
	Sequence_Part21(119);  /* --560--205550--119-- Marker 486 */
	Sequence_Part21(79);  /* --561--205629--79-- Marker 1005 */
	Sequence_Part21(101);  /* --562--205730--101-- Marker 1004 */
	SectionFlag = 1;
	Sequence_Part21(216);  /* --563--205946--216-- Marker 478 */
	SectionFlag = 0;
	Sequence_Part21(213);  /* --564--206159--213-- Marker 1007 */
	SectionFlag = 1;
	Sequence_Part21(216);  /* --565--206375--216-- Marker 479 */
	SectionFlag = 0;
	Sequence_Part21(244);  /* --566--206619--244-- Marker 1006 */
	SectionFlag = 1;
	Sequence_Part21(213);  /* --567--206833--214-- Marker 480 */
	SectionFlag = 0;
	Sequence_Part21(194);  /* --568--207027--194-- Marker 481 */
	
	Sequence_Part21(218);  /* --569--207245--218-- Marker 1008 */
	SectionFlag = 1;
	Sequence_Part21(197);  /* --570--207443--198-- Marker 482 */
	SectionFlag = 0;
	Sequence_Part21(242);  /* --571--207685--242-- Marker 1009 */
	SectionFlag = 1;
	Sequence_Part21(187);  /* --572--207872--187-- Marker 483 */
	SectionFlag = 0;
	Sequence_Part21(239);  /* --573--208111--239-- Marker 489 */
	
	Sequence_Part21(121);  /* --574--208316--205-- Marker 490 */
	
	Sequence_Part21(295);  /* --575--208527--211-- Marker 491 */
	Sequence_Part21(206);  /* --576--208733--206-- Marker 492 */
	Sequence_Part21(227);  /* --577--208960--227-- Marker 493 */
	Sequence_Part21(174);  /* --578--209134--174-- Marker 1010 */
	SectionFlag = 1;
	Sequence_Part21(240);  /* --579--209374--240-- Marker 494 */
	SectionFlag = 0;
	Sequence_Part21(207);  /* --580--209581--207-- Marker 1011 */
	SectionFlag = 1;
	Sequence_Part21(208);  /* --581--209789--208-- Marker 495 */
	SectionFlag = 0;
	Sequence_Part21(186);  /* --582--209975--186-- Marker 1012 */
	SectionFlag = 1;
	Sequence_Part21(254);  /* --583--210229--254-- Marker 496 */
	Sequence_Part21(214);  /* --584--210443--214-- n */
	SectionFlag = 0;
	Sequence_Part21(226);  /* --585--210669--226-- Marker 1013 */
	SectionFlag = 1;
	Sequence_Part21(209);  /* --586--210878--209-- Marker 498 */
	SectionFlag = 0;
	Sequence_Part21(252);  /* --587--211130--252-- Marker 1014 */
	SectionFlag = 1;
	Sequence_Part21(170);  /* --588--211360--230-- Marker 499 */
	SectionFlag = 0;
	Sequence_Part21(250);  /* --589--211552--192-- Marker 1015 */
	Sequence_Part21(186);  /* --590--211738--186-- Marker 500 */
	Sequence_Part21(229);  /* --591--211967--229-- Marker 1016 */
	Sequence_Part21(233);  /* --592--212200--233-- Marker 1017 */
	Sequence_Part21(206);  /* --593--212406--206-- Marker 501 */
	Sequence_Part21(194);  /* --594--212600--194-- Marker 1018 */
	SectionFlag = 1;
	Sequence_Part21(226);  /* --595--212826--226-- Marker 502 */
	SectionFlag = 0;
	Sequence_Part21(198);  /* --596--213024--198-- Marker 1019 */
	SectionFlag = 1;
	Sequence_Part21(221);  /* --597--213245--221-- Marker 503 */
	SectionFlag = 0;
	Sequence_Part21(185);  /* --598--213430--185-- Marker 1020 */
	SectionFlag = 1;
	Sequence_Part21(241);  /* --599--213671--241-- Marker 504 */
	Sequence_Part21(215);  /* --600--213886--215-- Marker 505 */
	SectionFlag = 0;
	Sequence_Part21(214);  /* --601--214100--214-- Marker 1021 */
	SectionFlag = 1;
	Sequence_Part21(202);  /* --602--214302--202-- Marker 506 */
	SectionFlag = 0;
	Sequence_Part21(240);  /* --603--214542--240-- Marker 1022 */
	SectionFlag = 1;
	Sequence_Part21(193);  /* --604--214735--193-- Marker 507 */
	SectionFlag = 0;
	Sequence_Part21(242);  /* --605--214977--242-- Marker 1023 */
	Sequence_Part21(188);  /* --606--215165--188-- Marker 508 */
	Sequence_Part21(243);  /* --607--215408--243-- Marker 1024 */
	Sequence_Part21(192);  /* --608--215600--192-- Marker 1025 */
	Sequence_Part21(241);  /* --609--215841--241-- Marker 509 */
	Sequence_Part21(209);  /* --610--216050--209-- Marker 1026 */
	SectionFlag = 1;
	Sequence_Part21(224);  /* --611--216274--224-- Marker 510 */
	SectionFlag = 0;
	Sequence_Part21(226);  /* --612--216500--226-- Marker 1027 */
	SectionFlag = 1;
	Sequence_Part21(186);  /* --613--216686--186-- Marker 511 */
	SectionFlag = 0;
	Sequence_Part21(232);  /* --614--216918--232-- Marker 1028 */
	SectionFlag = 1;
	Sequence_Part21(208);  /* --615--217126--208-- Marker 512 */
	Sequence_Part21(205);  /* --616--217331--205-- Marker 513 */
	SectionFlag = 0;
	Sequence_Part21(169);  /* --617--217500--169-- Marker 1029 */
	SectionFlag = 1;
	Sequence_Part21(268);  /* --618--217768--268-- Marker 514 */
	SectionFlag = 0;
	Sequence_Part21(212);  /* --619--217980--212-- Marker 1030 */
	Sequence_Part21(162);  /* --620--218222--242-- Marker 515 */
	// Part 21
	
	
	
	// Part 22
	
	Sequence_Part22(268);  /* --621--218411--189-- Marker 1031 */
	SectionFlag = 1;
	Sequence_Part22(216);  /* --622--218627--216-- Marker 516 */
	SectionFlag = 0;
	Sequence_Part22(232);  /* --623--218859--232-- Marker 1032 */
	SectionFlag = 1;
	Sequence_Part22(184);  /* --624--219043--184-- Marker 517 */
	SectionFlag = 0;
	Sequence_Part22(216);  /* --625--219259--216-- Marker 1033 */
	SectionFlag = 1;
	Sequence_Part22(186);  /* --626--219445--186-- Marker 518 */
	SectionFlag = 0;
	Sequence_Part22(255);  /* --627--219700--255-- Marker 1034 */
	SectionFlag = 1;
	Sequence_Part22(213);  /* --628--219913--213-- Marker 519 */
	SectionFlag = 0;
	Sequence_Part22(229);  /* --629--220142--229-- Marker 1035 */
	SectionFlag = 1;
	Sequence_Part22(190);  /* --630--220332--190-- Marker 520 */
	SectionFlag = 0;
	Sequence_Part22(218);  /* --631--220550--218-- Marker 1036 */
	SectionFlag = 1;
	Sequence_Part22(222);  /* --632--220772--222-- Marker 521 */
	SectionFlag = 0;
	Sequence_Part22(228);  /* --633--221000--228-- Marker 1037 */
	SectionFlag = 1;
	Sequence_Part22(185);  /* --634--221185--185-- Marker 522 */
	SectionFlag = 0;
	Sequence_Part22(253);  /* --635--221438--253-- Marker 1038 */
	SectionFlag = 1;
	Sequence_Part22(187);  /* --636--221625--187-- Marker 523 */
	SectionFlag = 0;
	Sequence_Part22(199);  /* --637--221824--199-- Marker 1039 */
	SectionFlag = 1;
	Sequence_Part22(223);  /* --638--222047--223-- Marker 524 */
	SectionFlag = 0;
	Sequence_Part22(235);  /* --639--222282--235-- Marker 1040 */
	SectionFlag = 1;
	Sequence_Part22(181);  /* --640--222463--181-- Marker 525 */
	SectionFlag = 0;
	Sequence_Part22(222);  /* --641--222685--222-- Marker 1041 */
	SectionFlag = 1;
	Sequence_Part22(201);  /* --642--222886--201-- Marker 526 */
	SectionFlag = 0;
	Sequence_Part22(214);  /* --643--223100--214-- Marker 1042 */
	SectionFlag = 1;
	Sequence_Part22(188);  /* --644--223288--188-- Marker 527 */
	SectionFlag = 0;
	Sequence_Part22(229);  /* --645--223517--229-- Marker 1043 */
	SectionFlag = 1;
	Sequence_Part22(187);  /* --646--223704--187-- Marker 528 */
	SectionFlag = 0;
	Sequence_Part22(258);  /* --647--223962--258-- Marker 1044 */
	SectionFlag = 1;
	Sequence_Part22(182);  /* --648--224144--182-- Marker 529 */
	SectionFlag = 0;
	Sequence_Part22(230);  /* --649--224374--230-- Marker 1045 */
	SectionFlag = 1;
	Sequence_Part22(227);  /* --650--224601--227-- Marker 530 */
	SectionFlag = 0;
	Sequence_Part22(270);  /* --651--224871--270-- Marker 1046 */
	SectionFlag = 1;
	Sequence_Part22(198);  /* --652--225069--198-- Marker 531 */
	SectionFlag = 0;
	Sequence_Part22(231);  /* --653--225300--231-- Marker 1047 */
	SectionFlag = 1;
	Sequence_Part22(171);  /* --654--225471--171-- Marker 532 */
	SectionFlag = 0;
	Sequence_Part22(244);  /* --655--225715--244-- Marker 1048 */
	SectionFlag = 1;
	Sequence_Part22(179);  /* --656--225894--179-- Marker 533 */
	SectionFlag = 0;
	Sequence_Part22(256);  /* --657--226150--256-- Marker 1049 */
	SectionFlag = 1;
	Sequence_Part22(191);  /* --658--226341--191-- Marker 534 */
	SectionFlag = 0;
	Sequence_Part22(268);  /* --659--226609--268-- Marker 1050 */
	SectionFlag = 1;
	Sequence_Part22(134);  /* --660--226743--134-- Marker 535 */
	SectionFlag = 0;
	Sequence_Part22(274);  /* --661--227017--274-- Marker 1051 */
	SectionFlag = 1;
	Sequence_Part22(152);  /* --662--227169--152-- Marker 536 */
	SectionFlag = 0;
	Sequence_Part22(231);  /* --663--227400--231-- Marker 1052 */
	SectionFlag = 1;
	Sequence_Part22(196);  /* --664--227596--196-- Marker 537 */
	SectionFlag = 0;
	Sequence_Part22(204);  /* --665--227800--204-- Marker 1053 */
	SectionFlag = 1;
	Sequence_Part22(257);  /* --666--228057--257-- Marker 538 */
	SectionFlag = 0;
	Sequence_Part22(219);  /* --667--228276--219-- Marker 1054 */
	SectionFlag = 1;
	Sequence_Part22(183);  /* --668--228459--183-- Marker 539 */
	SectionFlag = 0;
	Sequence_Part22(253);  /* --669--228712--253-- Marker 1055 */
	SectionFlag = 1;
	Sequence_Part22(169);  /* --670--228881--169-- Marker 540 */
	SectionFlag = 0;
	Sequence_Part22(263);  /* --671--229144--263-- Marker 1056 */
	SectionFlag = 1;
	Sequence_Part22(192);  /* --672--229336--192-- Marker 541 */
	SectionFlag = 0;
	Sequence_Part22(228);  /* --673--229564--228-- Marker 1057 */
	SectionFlag = 1;
	Sequence_Part22(186);  /* --674--229750--186-- Marker 542 */
	SectionFlag = 0;
	Sequence_Part22(227);  /* --675--229977--227-- Marker 1058 */
	SectionFlag = 1;
	Sequence_Part22(191);  /* --676--230168--191-- Marker 543 */
	SectionFlag = 0;
	Sequence_Part22(246);  /* --677--230414--246-- Marker 1059 */
	SectionFlag = 1;
	Sequence_Part22(166);  /* --678--230580--166-- Marker 544 */
	SectionFlag = 0;
	Sequence_Part22(241);  /* --679--230821--241-- Marker 1060 */
	SectionFlag = 1;
	Sequence_Part22(192);  /* --680--231013--192-- Marker 545 */
	SectionFlag = 0;
	Sequence_Part22(248);  /* --681--231261--248-- Marker 1061 */
	SectionFlag = 1;
	Sequence_Part22(209);  /* --682--231470--209-- Marker 546 */
	SectionFlag = 0;
	Sequence_Part22(247);  /* --683--231717--247-- Marker 1062 */
	SectionFlag = 1;
	Sequence_Part22(193);  /* --684--231910--193-- Marker 547 */
	// Part 22
	
	// Part 23
	Sequence_Part23(238);  /* --685--232148--238-- Marker 1063 */
	Sequence_Part23(182);  /* --686--232330--182-- Marker 548 */
	Sequence_Part23(229);  /* --687--232559--229-- Marker 1064 */
	Sequence_Part23(180);  /* --688--232739--180-- Marker 549 */
	Sequence_Part23(150);  /* --689--232889--150-- Marker 1065 */
	Sequence_Part23(89);  /* --690--232978--89-- Marker 550 */
	Sequence_Part23(184);  /* --691--233162--184-- Marker 1066 */
	Sequence_Part23(245);  /* --692--233407--245-- Marker 551 */
	Sequence_Part23(158);  /* --693--233565--158-- Marker 1067 */
	Sequence_Part23(286);  /* --694--233851--286-- Marker 552 */
	Sequence_Part23(208);  /* --695--234059--208-- Marker 1068 */
	Sequence_Part23(190);  /* --696--234249--190-- Marker 553 */
	Sequence_Part23(205);  /* --697--234454--205-- Marker 554 */
	Sequence_Part23(231);  /* --698--234685--231-- Marker 1069 */
	Sequence_Part23(206);  /* --699--234891--206-- Marker 555 */
	Sequence_Part23(209);  /* --700--235100--209-- Marker 1070 */
	Sequence_Part23(238);  /* --701--235338--238-- Marker 556 */
	Sequence_Part23(215);  /* --702--235553--215-- Marker 1071 */
	Sequence_Part23(197);  /* --703--235750--197-- Marker 557 */
	Sequence_Part23(198);  /* --704--235948--198-- Marker 1072 */
	Sequence_Part23(211);  /* --705--236159--211-- Marker 558 */
	Sequence_Part23(169);  /* --706--236328--169-- Marker 1073 */
	Sequence_Part23(172);  /* --707--236500--172-- Marker 559 */
	Sequence_Part23(138);  /* --708--236638--138-- Marker 1074 */
	Sequence_Part23(193);  /* --709--236831--193-- Marker 560 */
	Sequence_Part23(151);  /* --710--236982--151-- Marker 1075 */
	Sequence_Part23(296);  /* --711--237278--296-- Marker 561 */
	Sequence_Part23(232);  /* --712--237510--232-- Marker 1076 */
	Sequence_Part23(184);  /* --713--237694--184-- Marker 562 */
	Sequence_Part23(191);  /* --714--237885--191-- Marker 563 */
	Sequence_Part23(249);  /* --715--238134--249-- Marker 1077 */
	Sequence_Part23(233);  /* --716--238367--233-- Marker 1078 */
	Sequence_Part23(246);  /* --717--238613--246-- Marker 1079 */
	// Part 23
	
	// Part 24
	Sequence_Part24(180);  /* --718--238793--180-- Marker 564 */
	Sequence_Part24(220);  /* --719--239013--220-- Marker 1080 */
	Sequence_Part24(164);  /* --720--239177--164-- Marker 565 */
	Sequence_Part24(223);  /* --721--239400--223-- Marker 1081 */
	Sequence_Part24(229);  /* --722--239629--229-- Marker 566 */
	Sequence_Part24(208);  /* --723--239837--208-- Marker 1082 */
	Sequence_Part24(179);  /* --724--240016--179-- Marker 567 */
	Sequence_Part24(227);  /* --725--240243--227-- Marker 1083 */
	Sequence_Part24(199);  /* --726--240442--199-- Marker 568 */
	Sequence_Part24(208);  /* --727--240650--208-- Marker 1084 */
	Sequence_Part24(229);  /* --728--240879--229-- Marker 569 */
	Sequence_Part24(213);  /* --729--241092--213-- Marker 1085 */
	Sequence_Part24(249);  /* --730--241341--249-- Marker 1086 */
	Sequence_Part24(197);  /* --731--241538--197-- Marker 1087 */
	Sequence_Part24(250);  /* --732--241788--250-- Marker 571 */
	Sequence_Part24(193);  /* --733--241981--193-- Marker 1088 */
	Sequence_Part24(215);  /* --734--242196--215-- Marker 572 */
	Sequence_Part24(217);  /* --735--242413--217-- n */
	Sequence_Part24(188);  /* --736--242601--188-- Marker 573 */
	Sequence_Part24(247);  /* --737--242848--247-- Marker 1090 */
	Sequence_Part24(205);  /* --738--243053--205-- Marker 574 */
	Sequence_Part24(233);  /* --739--243286--233-- Marker 1091 */
	Sequence_Part24(194);  /* --740--243480--194-- Marker 575 */
	Sequence_Part24(233);  /* --741--243713--233-- Marker 1092 */
	Sequence_Part24(164);  /* --742--243877--164-- Marker 576 */
	Sequence_Part24(123);  /* --743--244000--123-- Marker 1093 */
	Sequence_Part24(133);  /* --744--244133--133-- Marker 1094 */
	Sequence_Part24(166);  /* --745--244299--166-- Marker 577 */
	Sequence_Part24(101);  /* --746--244400--101-- Marker 1095 */
	Sequence_Part24(141);  /* --747--244541--141-- Marker 1096 */
	Sequence_Part24(199);  /* --748--244740--199-- Marker 578 */
	Sequence_Part24(245);  /* --749--244985--245-- Marker 1097 */
	Sequence_Part24(212);  /* --750--245197--212-- Marker 579 */
	Sequence_Part24(241);  /* --751--245438--241-- Marker 1098 */
	Sequence_Part24(187);  /* --752--245625--187-- Marker 580 */
	Sequence_Part24(199);  /* --753--245824--199-- Marker 1099 */
	Sequence_Part24(201);  /* --754--246025--201-- Marker 581 */
	Sequence_Part24(243);  /* --755--246268--243-- Marker 1100 */
	Sequence_Part24(170);  /* --756--246438--170-- Marker 582 */
	Sequence_Part24(262);  /* --757--246700--262-- Marker 1101 */
	Sequence_Part24(167);  /* --758--246867--167-- Marker 583 */
	Sequence_Part24(222);  /* --759--247089--222-- Marker 1102 */
	Sequence_Part24(194);  /* --760--247283--194-- Marker 584 */
	Sequence_Part24(256);  /* --761--247539--256-- Marker 1103 */
	Sequence_Part24(219);  /* --762--247758--219-- Marker 585 */
	Sequence_Part24(221);  /* --763--247979--221-- Marker 1104 */
	Sequence_Part24(191);  /* --764--248170--191-- Marker 586 */
	Sequence_Part24(191);  /* --765--248361--191-- Marker 1105 */
	Sequence_Part24(218);  /* --766--248579--218-- Marker 587 */
	Sequence_Part24(289);  /* --767--248868--289-- Marker 1106 */
	Sequence_Part24(165);  /* --768--249033--165-- Marker 588 */
	Sequence_Part24(246);  /* --769--249279--246-- Marker 1107 */
	Sequence_Part24(212);  /* --770--249491--212-- Marker 589 */
	Sequence_Part24(233);  /* --771--249724--233-- Marker 1108 */
	Sequence_Part24(151);  /* --772--249875--151-- Marker 590 */
	Sequence_Part24(246);  /* --773--250121--246-- Marker 1109 */
	Sequence_Part24(191);  /* --774--250312--191-- Marker 591 */
	Sequence_Part24(234);  /* --775--250546--234-- Marker 1110 */
	Sequence_Part24(182);  /* --776--250728--182-- Marker 592 */
	Sequence_Part24(220);  /* --777--250948--220-- Marker 1111 */
	Sequence_Part24(252);  /* --778--251200--252-- Marker 593 */
	Sequence_Part24(235);  /* --779--251435--235-- Marker 594 */
	Sequence_Part24(215);  /* --780--251650--215-- Marker 1112 */
	Sequence_Part24(215);  /* --781--251865--215-- Marker 595 */
	Sequence_Part24(201);  /* --782--252066--201-- Marker 596 */
	Sequence_Part24(211);  /* --783--252277--211-- Marker 597 */
	Sequence_Part24(191);  /* --784--252468--191-- Marker 598 */
	// Part 24
	
	// Part 25
	Sequence_Part25(232);  /* --785--252700--232-- Marker 1113 */
	Sequence_Part25(187);  /* --786--252887--187-- Marker 599 */
	Sequence_Part25(231);  /* --787--253118--231-- Marker 1114 */
	Sequence_Part25(178);  /* --788--253296--178-- Marker 600 */
	Sequence_Part25(225);  /* --789--253521--225-- Marker 1115 */
	Sequence_Part25(201);  /* --790--253722--201-- Marker 601 */
	SectionFlag = 1;
	Sequence_Part25(291);  /* --791--254013--291-- Marker 602 */
	Sequence_Part25(378);  /* --792--254391--378-- Marker 603 */
	Sequence_Part25(215);  /* --793--254606--215-- Marker 604 */
	SectionFlag = 0;
	Sequence_Part25(244);  /* --794--254850--244-- Marker 1116 */
	Sequence_Part25(220);  /* --795--255070--220-- Marker 605 */
	Sequence_Part25(211);  /* --796--255281--211-- Marker 1117 */
	Sequence_Part25(212);  /* --797--255493--212-- Marker 606 */
	Sequence_Part25(222);  /* --798--255715--222-- Marker 1118 */
	Sequence_Part25(215);  /* --799--255930--215-- Marker 607 */
	Sequence_Part25(189);  /* --800--256119--189-- Marker 1119 */
	Sequence_Part25(209);  /* --801--256328--209-- Marker 608 */
	Sequence_Part25(228);  /* --802--256556--228-- Marker 1120 */
	Sequence_Part25(219);  /* --803--256775--219-- Marker 609 */
	Sequence_Part25(190);  /* --804--256965--190-- Marker 1121 */
	Sequence_Part25(191);  /* --805--257156--191-- Marker 610 */
	SectionFlag = 1;
	Sequence_Part25(260);  /* --806--257416--260-- Marker 611 */
	Sequence_Part25(382);  /* --807--257798--382-- Marker 612 */
	Sequence_Part25(221);  /* --808--258019--221-- Marker 613 */
	SectionFlag = 0;
	Sequence_Part25(233);  /* --809--258252--233-- Marker 1122 */
	Sequence_Part25(176);  /* --810--258428--176-- Marker 614 */
	Sequence_Part25(254);  /* --811--258682--254-- Marker 1123 */
	Sequence_Part25(193);  /* --812--258875--193-- Marker 615 */
	Sequence_Part25(250);  /* --813--259125--250-- Marker 1124 */
	Sequence_Part25(201);  /* --814--259326--201-- Marker 616 */
	Sequence_Part25(239);  /* --815--259565--239-- Marker 1125 */
	Sequence_Part25(180);  /* --816--259745--180-- Marker 617 */
	Sequence_Part25(239);  /* --817--259984--239-- Marker 1126 */
	Sequence_Part25(201);  /* --818--260185--201-- Marker 618 */
	Sequence_Part25(256);  /* --819--260441--256-- Marker 1127 */
	Sequence_Part25(181);  /* --820--260622--181-- Marker 619 */
	Sequence_Part25(211);  /* --821--260833--211-- Marker 1128 */
	Sequence_Part25(198);  /* --822--261031--198-- Marker 620 */
	Sequence_Part25(239);  /* --823--261270--239-- Marker 1129 */
	Sequence_Part25(166);  /* --824--261436--166-- Marker 621 */
	Sequence_Part25(251);  /* --825--261687--251-- Marker 1130 */
	Sequence_Part25(221);  /* --826--261908--221-- Marker 622 */
	Sequence_Part25(202);  /* --827--262110--202-- Marker 1131 */
	Sequence_Part25(207);  /* --828--262317--207-- Marker 624 */
	Sequence_Part25(261);  /* --829--262578--261-- Marker 1132 */
	Sequence_Part25(161);  /* --830--262739--161-- Marker 625 */
	Sequence_Part25(218);  /* --831--262957--218-- Marker 1133 */
	Sequence_Part25(182);  /* --832--263139--182-- Marker 1134 */
	Sequence_Part25(161);  /* --833--263300--161-- Marker 1135 */
	Sequence_Part25(330);  /* --834--263630--330-- Marker 626 */
	Sequence_Part25(370);  /* --835--264000--370-- Marker 1136 */
	Sequence_Part25(285);  /* --836--264285--285-- Marker 627 */
	Sequence_Part25(172);  /* --837--264457--172-- Marker 1137 */
	SectionFlag = 1;
	Sequence_Part25(247);  /* --838--264704--247-- Marker 628 */
	Sequence_Part25(196);  /* --839--264900--196-- Marker 629 */
	SectionFlag = 0;
	Sequence_Part25(226);  /* --840--265126--226-- Marker 1138 */
	Sequence_Part25(223);  /* --841--265349--223-- Marker 630 */
	Sequence_Part25(219);  /* --842--265568--219-- Marker 1139 */
	Sequence_Part25(221);  /* --843--265789--221-- Marker 631 */
	Sequence_Part25(412);  /* --844--266201--412-- Marker 632 */
	Sequence_Part25(218);  /* --845--266419--218-- Marker 1140 */
	Sequence_Part25(222);  /* --846--266641--222-- Marker 633 */
	Sequence_Part25(184);  /* --847--266825--184-- Marker 1141 */
	Sequence_Part25(264);  /* --848--267089--264-- Marker 634 */
	Sequence_Part25(176);  /* --849--267265--176-- Marker 1142 */
	Sequence_Part25(236);  /* --850--267501--236-- Marker 635 */
	Sequence_Part25(236);  /* --851--267737--236-- Marker 636 */
	SectionFlag = 1;
	Sequence_Part25(179);  /* --852--267916--179-- Marker 1143 */
	Sequence_Part25(219);  /* --853--268135--219-- Marker 637 */
	Sequence_Part25(218);  /* --854--268353--218-- Marker 638 */
	SectionFlag = 0;
	Sequence_Part25(202);  /* --855--268555--202-- Marker 1144 */
	Sequence_Part25(221);  /* --856--268776--221-- Marker 639 */
	Sequence_Part25(255);  /* --857--269031--255-- Marker 1145 */
	Sequence_Part25(220);  /* --858--269251--220-- Marker 640 */
	Sequence_Part25(209);  /* --859--269460--209-- Marker 1146 */
	Sequence_Part25(248);  /* --860--269708--248-- Marker 641 */
	Sequence_Part25(162);  /* --861--269870--162-- Marker 1147 */
	Sequence_Part25(240);  /* --862--270110--240-- Marker 642 */
	Sequence_Part25(209);  /* --863--270319--209-- Marker 1148 */
	Sequence_Part25(221);  /* --864--270540--221-- Marker 643 */
	Sequence_Part25(202);  /* --865--270742--202-- Marker 1149 */
	Sequence_Part25(197);  /* --866--270939--197-- Marker 644 */
	SectionFlag = 1;
	Sequence_Part25(161);  /* --867--271100--161-- Marker 1150 */
	SectionFlag = 0;
	Sequence_Part25(300);  /* --868--271400--300-- Marker 1151 */
	SectionFlag = 1;
	Sequence_Part25(173);  /* --869--271573--173-- Marker 646 */
	Sequence_Part25(208);  /* --870--271781--208-- Marker 647 */
	SectionFlag = 0;
	Sequence_Part25(235);  /* --871--272016--235-- Marker 1152 */
	Sequence_Part25(191);  /* --872--272207--191-- Marker 648 */
	Sequence_Part25(217);  /* --873--272424--217-- Marker 1153 */
	Sequence_Part25(216);  /* --874--272640--216-- Marker 649 */
	Sequence_Part25(217);  /* --875--272857--217-- Marker 1154 */
	Sequence_Part25(203);  /* --876--273060--203-- Marker 650 */
	Sequence_Part25(222);  /* --877--273282--222-- Marker 1155 */
	Sequence_Part25(187);  /* --878--273469--187-- Marker 651 */
	Sequence_Part25(231);  /* --879--273700--231-- Marker 1156 */
	Sequence_Part25(191);  /* --880--273891--191-- Marker 652 */
	Sequence_Part25(259);  /* --881--274150--259-- Marker 1157 */
	Sequence_Part25(199);  /* --882--274349--199-- Marker 653 */
	Sequence_Part25(222);  /* --883--274571--222-- Marker 1158 */
	Sequence_Part25(169);  /* --884--274740--169-- Marker 654 */
	Sequence_Part25(220);  /* --885--274960--220-- Marker 1159 */
	Sequence_Part25(214);  /* --886--275174--214-- Marker 655 */
	Sequence_Part25(246);  /* --887--275420--246-- Marker 1160 */
	Sequence_Part25(183);  /* --888--275603--183-- Marker 656 */
	Sequence_Part25(221);  /* --889--275824--221-- Marker 1161 */
	Sequence_Part25(216);  /* --890--276040--216-- Marker 657 */
	Sequence_Part25(227);  /* --891--276267--227-- n */
	Sequence_Part25(237);  /* --892--276504--237-- Marker 658 */
	Sequence_Part25(196);  /* --893--276700--196-- Marker 1164 */
	Sequence_Part25(200);  /* --894--276900--200-- Marker 1163 */
	Sequence_Part25(200);  /* --895--277100--200-- Marker 1165 */
	Sequence_Part25(257);  /* --896--277357--257-- Marker 659 */
	Sequence_Part25(189);  /* --897--277546--189-- Marker 1166 */
	Sequence_Part25(224);  /* --898--277770--224-- Marker 1167 */
	SectionFlag = 1;
	Sequence_Part25(230);  /* --899--278000--230-- Marker 1168 */
	Sequence_Part25(227);  /* --900--278227--227-- Marker 660 */
	// Part 25
	
	// Part 26
	Sequence_Part26(198);  /* --901--278425--198-- Marker 1169 */
	Sequence_Part26(214);  /* --902--278639--214-- Marker 661 */
	Sequence_Part26(202);  /* --903--278841--202-- Marker 1170 */
	Sequence_Part26(218);  /* --904--279059--218-- Marker 1171 */
	Sequence_Part26(207);  /* --905--279266--207-- Marker 662 */
	Sequence_Part26(185);  /* --906--279451--185-- Marker 1172 */
	Sequence_Part26(242);  /* --907--279693--242-- Marker 663 */
	Sequence_Part26(184);  /* --908--279877--184-- Marker 1173 */
	Sequence_Part26(276);  /* --909--280153--276-- Marker 664 */
	Sequence_Part26(223);  /* --910--280376--223-- Marker 1174 */
	Sequence_Part26(193);  /* --911--280569--193-- Marker 665 */
	Sequence_Part26(253);  /* --912--280822--253-- Marker 666 */
	Sequence_Part26(178);  /* --913--281000--178-- Marker 1175 */
	Sequence_Part26(210);  /* --914--281210--210-- Marker 667 */
	Sequence_Part26(190);  /* --915--281400--190-- Marker 1176 */
	Sequence_Part26(237);  /* --916--281637--237-- Marker 668 */
	Sequence_Part26(187);  /* --917--281824--187-- Marker 1177 */
	Sequence_Part26(230);  /* --918--282054--230-- Marker 709 */
	Sequence_Part26(222);  /* --919--282276--222-- Marker 1178 */
	Sequence_Part26(211);  /* --920--282487--211-- Marker 710 */
	Sequence_Part26(224);  /* --921--282711--224-- Marker 669 */
	Sequence_Part26(189);  /* --922--282900--189-- Marker 1179 */
	Sequence_Part26(227);  /* --923--283127--227-- Marker 670 */
	Sequence_Part26(234);  /* --924--283361--234-- Marker 1180 */
	Sequence_Part26(213);  /* --925--283574--213-- Marker 671 */
	Sequence_Part26(211);  /* --926--283785--211-- Marker 1181 */
	Sequence_Part26(191);  /* --927--283976--191-- Marker 672 */
	SectionFlag = 20;
	Sequence_Part26(201+130);  /* --928--284177--201-- n */
	Sequence_Part26(1362-350+140);  /* --929--285539--1362-- Marker 674 */
	// Part 26
	
	// Part 27
	WS2801_Fade((835/80)-2,80,1);
	//Sequence_Part27(835);  /* --930--286374--835-- Marker 739 */
	
	Sequence_Part27(357);  /* --931--286731--357-- Marker 740 */
	Sequence_Part27(383);  /* --932--287114--383-- Marker 741 */
	Sequence_Part27(344);  /* --933--287458--344-- Marker 742 */
	Sequence_Part27(423);  /* --934--287881--423-- Marker 743 */
	Sequence_Part27(369);  /* --935--288250--369-- Marker 744 */
	Sequence_Part27(413);  /* --936--288663--413-- Marker 745 */
	Sequence_Part27(387);  /* --937--289050--387-- Marker 746 */
	Sequence_Part27(416);  /* --938--289466--416-- Marker 747 */
	Sequence_Part27(388);  /* --939--289854--388-- Marker 748 */
	Sequence_Part27(346);  /* --940--290200--346-- Marker 749 */
	Sequence_Part27(419);  /* --941--290619--419-- Marker 750 */
	SectionFlag = 2;
	Sequence_Part27(416);  /* --942--291035--416-- Marker 751 */
	Sequence_Part27(706+80);  /* --943--291741--706-- Marker 676 */
	// Part 27
	#endif
	
	// Part 28
	Sequence_Part28(339);  /* --944--292080--339-- Marker 711 */
	SectionFlag = 0;
	Sequence_Part28(441);  /* --945--292521--441-- Marker 712 */
	SectionFlag = 1;
	Sequence_Part28(342);  /* --946--292863--342-- Marker 713 */
	SectionFlag = 0;
	Sequence_Part28(396);  /* --947--293259--396-- Marker 714 */
	SectionFlag = 1;
	Sequence_Part28(414);  /* --948--293673--414-- Marker 715 */
	Sequence_Part28(327);  /* --949--294000--327-- Marker 716 */
	Sequence_Part28(369);  /* --950--294369--369-- Marker 717 */
	Sequence_Part28(159);  /* --951--294528--159-- Marker 718 */
	SectionFlag = 0;
	Sequence_Part28(342);  /* --952--294870--342-- Marker 719 */
	SectionFlag = 1;
	Sequence_Part28(292);  /* --953--295162--292-- Marker 720 */
	SectionFlag = 0;
	Sequence_Part28(482);  /* --954--295644--482-- Marker 721 */
	SectionFlag = 1;
	Sequence_Part28(356);  /* --955--296000--356-- Marker 722 */
	SectionFlag = 0;
	Sequence_Part28(421);  /* --956--296421--421-- Marker 723 */
	SectionFlag = 1;
	Sequence_Part28(343);  /* --957--296764--343-- Marker 724 */
	Sequence_Part28(386);  /* --958--297150--386-- Marker 725 */
	Sequence_Part28(273);  /* --959--297423--273-- Marker 726 */
	Sequence_Part28(233);  /* --960--297656--233-- Marker 727 */
	SectionFlag = 0;
	Sequence_Part28(244);  /* --961--297900--244-- Marker 728 */
	SectionFlag = 1;
	Sequence_Part28(415);  /* --962--298315--415-- Marker 729 */
	SectionFlag = 0;
	Sequence_Part28(385);  /* --963--298700--385-- Marker 752 */
	SectionFlag = 1;
	Sequence_Part28(359);  /* --964--299059--359-- Marker 730 */
	SectionFlag = 0;
	Sequence_Part28(441);  /* --965--299500--441-- Marker 731 */
	SectionFlag = 1;
	Sequence_Part28(374);  /* --966--299874--374-- Marker 732 */
	SectionFlag = 0;
	Sequence_Part28(526);  /* --967--300400--526-- Marker 753 */
	SectionFlag = 1;
	Sequence_Part28(191);  /* --968--300683--283-- Marker 754 */
	SectionFlag = 0;
	Sequence_Part28(536);  /* --969--301128--445-- Marker 755 */
	SectionFlag = 1;
	Sequence_Part28(354);  /* --970--301482--354-- Marker 756 */
	SectionFlag = 0;
	Sequence_Part28(418);  /* --971--301900--418-- Marker 757 */
	SectionFlag = 1;
	Sequence_Part28(287);  /* --972--302187--287-- Marker 758 */
	Sequence_Part28(683);  /* --973--302870--683-- Marker 759 */ 
	Sequence_Part28(160);  /* --974--303030--160-- Marker 760 */
	Sequence_Part28(257);  /* --975--303287--257-- Marker 761 */
	Sequence_Part28(113);  /* --976--303400--113-- Marker 762 */
	Sequence_Part28(263);  /* --977--303663--263-- Marker 763 */
	Sequence_Part28(121);  /* --978--303784--121-- Marker 764 */
	// Part 29
	
	// Part 30
	Sequence_Part30(416);  /* --979--304200--416-- Marker 765 */
	Sequence_Part30(288);  /* --980--304488--288-- Marker 766 */
	SectionFlag = 0;
	Sequence_Part30(491);  /* --981--304979--491-- Marker 767 */
	SectionFlag = 1;
	Sequence_Part30(305);  /* --982--305284--305-- Marker 768 */
	SectionFlag = 0;
	Sequence_Part30(456);  /* --983--305740--456-- Marker 769 */
	SectionFlag = 1;
	Sequence_Part30(330);  /* --984--306070--330-- Marker 770 */
	Sequence_Part30(347);  /* --985--306417--347-- Marker 771 */
	Sequence_Part30(254);  /* --986--306671--254-- Marker 772 */
	Sequence_Part30(263);  /* --987--306934--263-- Marker 773 */
	SectionFlag = 0;
	Sequence_Part30(388);  /* --988--307322--388-- Marker 774 */
	SectionFlag = 1;
	Sequence_Part30(246);  /* --989--307568--246-- Marker 775 */
	SectionFlag = 0;
	Sequence_Part30(411);  /* --990--307979--411-- Marker 776 */
	SectionFlag = 1;
	Sequence_Part30(421);  /* --991--308400--421-- Marker 777 */
	SectionFlag = 0;
	Sequence_Part30(411);  /* --992--308811--411-- Marker 778 */
	SectionFlag = 1;
	Sequence_Part30(348);  /* --993--309159--348-- Marker 779 */
	Sequence_Part30(385);  /* --994--309544--385-- Marker 780 */
	Sequence_Part30(268);  /* --995--309812--268-- Marker 781 */
	Sequence_Part30(308);  /* --996--310120--308-- Marker 782 */
	SectionFlag = 0;
	Sequence_Part30(265);  /* --997--310385--265-- Marker 783 */
	SectionFlag = 1;
	Sequence_Part30(304);  /* --998--310770--385-- Marker 784 */
	SectionFlag = 0;
	Sequence_Part30(472);  /* --999--311163--393-- Marker 785 */
	SectionFlag = 1;
	Sequence_Part30(337);  /* --1000--311500--337-- Marker 786 */
	SectionFlag = 0;
	Sequence_Part30(419);  /* --1001--311919--419-- Marker 787 */
	SectionFlag = 1;
	Sequence_Part30(340);  /* --1002--312259--340-- Marker 788 */
	SectionFlag = 0;
	Sequence_Part30(423);  /* --1003--312682--423-- Marker 789 */
	SectionFlag = 1;
	Sequence_Part30(401);  /* --1004--313083--401-- Marker 790 */
	SectionFlag = 0;
	Sequence_Part30(353);  /* --1005--313436--353-- Marker 791 */
	SectionFlag = 1;
	Sequence_Part30(390);  /* --1006--313826--390-- Marker 792 */
	SectionFlag = 0;
	Sequence_Part30(358);  /* --1007--314184--358-- Marker 793 */
	SectionFlag = 1;
	Sequence_Part30(389);  /* --1008--314573--389-- Marker 794 */
	SectionFlag = 0;
	Sequence_Part30(410);  /* --1009--314983--410-- Marker 795 */
	SectionFlag = 2;
	Sequence_Part30(403);  /* --1010--315386--403-- Marker 796 */ 
	SectionFlag = 3;
	Sequence_Part30(414);  /* --1011--315800--414-- Marker 797 */
	Sequence_Part30(720);  /* --1012--316536--736-- Marker 798 */
	// Part 30
	
	
	HelpFlag = 0;
	SectionFlag = 0;
	Sequence_Part31(400); /* --983--316936--400-- Marker 799 */
	Sequence_Part31(416); /* --984--317352--416-- Marker 800 */
	Sequence_Part31(224); /* --985--317576--224-- Marker 801 */
	Sequence_Part31(359); /* --986--317935--359-- Marker 802 */
	Sequence_Part31(365); /* --987--318300--365-- Marker 803 */
	
	Sequence_Part31(789); /* --988--319089--789-- Marker 804 */
	Sequence_Part31(415); /* --989--319504--415-- Marker 805 */
	Sequence_Part31(156); /* --990--319660--156-- Marker 806 */
	Sequence_Part31(394); /* --991--320054--394-- Marker 807 */
	Sequence_Part31(388); /* --992--320442--388-- Marker 808 */
	Sequence_Part31(379); /* --993--320821--379-- Marker 809 */
	Sequence_Part31(266); /* --994--321087--266-- Marker 810 */
	Sequence_Part31(330); /* --995--321417--330-- Marker 811 */
	Sequence_Part31(383); /* --996--321800--383-- Marker 1304 */
	Sequence_Part31(200); /* --997--322000--200-- Marker 812 */
	Sequence_Part31(581); /* --998--322581--581-- Marker 813 */
	Sequence_Part31(169); /* --999--322750--169-- Marker 814 */
	Sequence_Part31(317); /* --1000--323067--317-- Marker 815 */
	Sequence_Part31(455); /* --1001--323522--455-- Marker 816 */
	Sequence_Part31(392); /* --1002--323914--392-- Marker 817 */
	Sequence_Part31(231); /* --1003--324145--231-- Marker 818 */
	Sequence_Part31(367); /* --1004--324512--367-- Marker 819 */
	Sequence_Part31(576); /* --1005--325088--576-- Marker 820 */
	Sequence_Part31(429); /* --1006--325517--429-- Marker 821 */
	Sequence_Part31(358); /* --1007--325875--358-- Marker 822 */
	Sequence_Part31(372); /* --1008--326247--372-- Marker 823 */
	Sequence_Part31(389); /* --1009--326636--389-- Marker 824 */
	Sequence_Part31(352); /* --1010--326988--352-- Marker 825 */
	Sequence_Part31(251); /* --1011--327239--251-- Marker 826 */
	Sequence_Part31(389); /* --1012--327629--390-- Marker 1305 */
	Sequence_Part31(250); /* -------- Marker 827 */
	Sequence_Part31(169); /* --1013--328049--420-- Marker 1306 */
	Sequence_Part31(132); /* --1014--328181--132-- Marker 828 */
	HelpFlag = 1;
	SectionFlag = 0;
	Sequence_Part31(568); /* --1015------ Marker 829*/
	Sequence_Part31(196); /* --1015--328946--765-- Marker 830 */
	Sequence_Part31(354); /* --1016--329300--354-- Marker 831 */
	Sequence_Part31(421); /* --1017--329721--421-- Marker 832 */
	Sequence_Part31(343); /* --1018--330064--343-- Marker 833 */
	Sequence_Part31(265); /* --1019--330329--265-- Marker 834 */
	Sequence_Part31(391); /* --1020--330720--391-- Marker 835 */
	Sequence_Part31(795); /* --1021--331515--795-- Marker 836 */
	Sequence_Part31(369); /* --1022--331884--369-- Marker 837 */
	Sequence_Part31(148); /* --1023--332032--148-- Marker 838 */
	Sequence_Part31(409); /* --1024--332441--409-- Marker 839 */
	Sequence_Part31(373); /* --1025--332814--373-- Marker 1307 */
	HelpFlag = 2;
	Sequence_Part31(331); /* --1026--333145--331-- Marker 840 */
	Sequence_Part31(309); /* --1027--333454--309-- Marker 841 */
	HelpFlag = 1;
	Sequence_Part31(1306); /* --1028--334760--1306-- Marker 844 */
	Sequence_Part31(222); /* --1029--334982--222-- Marker 845 */
	Sequence_Part31(162); /* --1030--335144--162-- Marker 846 */
	Sequence_Part31(370); /* --1031--335514--370-- Marker 847 */
	Sequence_Part31(365); /* --1032--335879--365-- Marker 848 */
	Sequence_Part31(321); /* --1033--336200--321-- Marker 849 */
	Sequence_Part31(515); /* --1034--336715--515-- Marker 850 */
	Sequence_Part31(216); /* --1035--336931--216-- Marker 851 */
	Sequence_Part31(748); /* --1036--337679--748-- Marker 852 */
	Sequence_Part31(558); /* --1037--338237--558-- Marker 855 */
	Sequence_Part31(363); /* --1038--338600--363-- Marker 856 */
	Sequence_Part31(400); /* --1039--339000--400-- Marker 857 */
	Sequence_Part31(370); /* --1040--339370--370-- Marker 858 */
	Sequence_Part31(398); /* --1041--339768--398-- Marker 859 */
	Sequence_Part31(754); /* --1042--340522--754-- Marker 860 */
	HelpFlag = 3;
	SectionFlag = 0;
	Sequence_Part31(819); /* -------- Marker 861 */
	Sequence_Part31(374); /* --1043--341716--1194-- Marker 862 */
	Sequence_Part31(369); /* --1044--342085--369-- Marker 863 */
	Sequence_Part31(385); /* --1045--342470--385-- Marker 864 */
	Sequence_Part31(456); /* --1046--342926--456-- Marker 865 */
	Sequence_Part31(324); /* --1047--343250--324-- Marker 866 */
	Sequence_Part31(412); /* --1048--343662--412-- Marker 867 */
	Sequence_Part31(248); /* --1049--343910--248-- Marker 868 */
	Sequence_Part31(245); /* --1050--344155--245-- Marker 869 */
	Sequence_Part31(292); /* --1051--344447--292-- Marker 870 */
	Sequence_Part31(383); /* --1052--344830--383-- Marker 1209 */
	Sequence_Part31(419); /* --1053--345249--419-- Marker 1210 */
	Sequence_Part31(363); /* --1054--345612--363-- Marker 1211 */
	Sequence_Part31(403); /* --1055--346015--403-- Marker 1182 */
	Sequence_Part31(415); /* --1056--346430--415-- Marker 1215 */
	Sequence_Part31(334); /* --1057--346764--334-- Marker 1212 */
	Sequence_Part31(287); /* --1058--347051--287-- Marker 1213 */
	Sequence_Part31(249); /* --1059--347300--249-- Marker 1214 */
	Sequence_Part31(263); /* --1060--347563--263-- Marker 1183 */
	Sequence_Part31(381); /* --1061--347944--381-- Marker 1216 */
	Sequence_Part31(279); /* --1062--348223--279-- Marker 1217 */
	Sequence_Part31(437); /* --1063--348660--437-- Marker 1218 */
	Sequence_Part31(417); /* --1064--349077--417-- Marker 1184 */
	Sequence_Part31(354); /* --1065--349431--354-- Marker 1219 */
	Sequence_Part31(413); /* --1066--349844--413-- Marker 1185 */
	Sequence_Part31(405); /* --1067--350249--405-- Marker 1220 */
	Sequence_Part31(417); /* --1068--350666--417-- Marker 1186 */
	Sequence_Part31(274); /* --1069--350940--274-- Marker 1221 */
	Sequence_Part31(318); /* --1070--351258--318-- Marker 1222 */
	Sequence_Part31(489); /* --1071--351747--489-- Marker 1223 */
	Sequence_Part31(470); /* --1072--352217--470-- Marker 1224 */
	Sequence_Part31(283); /* --1073--352500--283-- Marker 1225 */
	Sequence_Part31(163); /* --1074--352663--163-- Marker 1226 */
	Sequence_Part31(186); /* --1075--352849--186-- Marker 1227 */
	Sequence_Part31(151); /* --1076--353000--151-- Marker 1228 */
	Sequence_Part31(276); /* --1077--353276--276-- Marker 1229 */
	Sequence_Part31(142); /* --1078--353418--142-- Marker 1230 */
	Sequence_Part31(305); /* --1079--353723--305-- Marker 1187 */
	Sequence_Part31(353); /* --1080--354076--353-- Marker 1231 */
	Sequence_Part31(424); /* --1081--354500--424-- Marker 1188 */
	Sequence_Part31(353); /* --1082--354853--353-- Marker 1232 */
	Sequence_Part31(427); /* --1083--355280--427-- Marker 1189 */
	Sequence_Part31(404); /* --1084--355684--404-- Marker 1233 */
	Sequence_Part31(360); /* --1085--356044--360-- Marker 1190 */
	Sequence_Part31(220); /* --1086--356264--220-- Marker 1234 */
	Sequence_Part31(283); /* --1087--356547--283-- Marker 1235 */
	Sequence_Part31(327); /* --1088--356874--327-- Marker 1191 */
	Sequence_Part31(287); /* --1089--357161--287-- Marker 1236 */
	Sequence_Part31(439); /* --1090--357600--439-- Marker 1237 */
	Sequence_Part31(314); /* --1091--357914--314-- Marker 1238 */
	Sequence_Part31(486); /* --1092--358400--486-- Marker 1192 */
	Sequence_Part31(324); /* --1093--358724--324-- Marker 1239 */
	Sequence_Part31(421); /* --1094--359145--421-- Marker 1240 */
	Sequence_Part31(301); /* --1095--359446--301-- Marker 1241 */
	Sequence_Part31(254); /* --1096--359700--254-- Marker 1242 */
	Sequence_Part31(240); /* --1097--359940--240-- Marker 1193 */
	Sequence_Part31(360); /* --1098--360300--360-- Marker 1243 */
	Sequence_Part31(400); /* --1099--360700--400-- Marker 1244 */
	Sequence_Part31(358); /* --1100--361058--358-- Marker 1245 */
	Sequence_Part31(383); /* --1101--361441--383-- Marker 1246 */
	Sequence_Part31(376); /* --1102--361817--376-- Marker 1247 */
	Sequence_Part31(458); /* --1103--362275--458-- Marker 1248 */
	Sequence_Part31(345); /* --1104--362620--345-- Marker 1249 */
	Sequence_Part31(476); /* --1105--363096--476-- Marker 1194 */
	Sequence_Part31(344); /* --1106--363440--344-- Marker 1250 */
	Sequence_Part31(396); /* --1107--363836--396-- Marker 1251 */
	Sequence_Part31(346); /* --1108--364182--346-- Marker 1252 */
	Sequence_Part31(418); /* --1109--364600--418-- Marker 1195 */
	Sequence_Part31(376); /* --1110--364976--376-- Marker 1253 */
	Sequence_Part31(374); /* --1111--365350--374-- Marker 1254 */
	Sequence_Part31(191); /* --1112--365541--191-- Marker 1255 */
	Sequence_Part31(297); /* --1113--365838--297-- Marker 1256 */
	Sequence_Part31(277); /* --1114--366115--277-- Marker 1196 */
	Sequence_Part31(366); /* --1115--366481--366-- Marker 1257 */
	Sequence_Part31(381); /* --1116--366862--381-- Marker 1258 */
	Sequence_Part31(338); /* --1117--367200--338-- Marker 1259 */
	Sequence_Part31(456); /* --1118--367656--456-- Marker 1197 */
	Sequence_Part31(344); /* --1119--368000--344-- Marker 1260 */
	Sequence_Part31(400); /* --1120--368400--400-- Marker 1261 */
	Sequence_Part31(300); /* --1121--368700--300-- Marker 1262 */
	Sequence_Part31(300); /* --1122--369000--300-- Marker 1263 */
	Sequence_Part31(233); /* --1123--369233--233-- Marker 1198 */
	Sequence_Part31(296); /* --1124--369529--296-- Marker 1264 */
	Sequence_Part31(352); /* --1125--369881--352-- Marker 1265 */
	Sequence_Part31(519); /* --1126--370400--519-- Marker 1266 */
	Sequence_Part31(358); /* --1127--370758--358-- Marker 1199 */
	Sequence_Part31(371); /* --1128--371129--371-- Marker 1267 */
	Sequence_Part31(409); /* --1129--371538--409-- Marker 1268 */
	Sequence_Part31(362); /* --1130--371900--362-- Marker 1269 */
	Sequence_Part31(357); /* --1131--372257--357-- Marker 1200 */
	Sequence_Part31(386); /* --1132--372643--386-- Marker 1270 */
	Sequence_Part31(475); /* --1133--373118--475-- Marker 1201 */
	Sequence_Part31(358); /* --1134--373476--358-- Marker 1271 */
	Sequence_Part31(373); /* --1135--373849--373-- Marker 1272 */
	Sequence_Part31(299); /* --1136--374148--299-- Marker 1273 */
	Sequence_Part31(171); /* --1137--374319--171-- Marker 1274 */
	Sequence_Part31(264); /* --1138--374583--264-- Marker 1275 */
	Sequence_Part31(138); /* --1139--374721--138-- Marker 1276 */
	Sequence_Part31(311); /* --1140--375032--311-- Marker 1277 */
	Sequence_Part31(149); /* --1141--375181--149-- Marker 1278 */
	Sequence_Part31(238); /* --1142--375419--238-- Marker 1202 */
	Sequence_Part31(411); /* --1143--375830--411-- Marker 1279 */
	Sequence_Part31(358); /* --1144--376188--358-- Marker 1280 */
	Sequence_Part31(412); /* --1145--376600--412-- Marker 1281 */
	Sequence_Part31(381); /* --1146--376981--381-- Marker 1282 */
	Sequence_Part31(361); /* --1147--377342--361-- Marker 1283 */
	Sequence_Part31(382); /* --1148--377724--382-- Marker 1284 */
	Sequence_Part31(310); /* --1149--378041--317-- Marker 1285 */
	Sequence_Part31(459); /* --1150--378500--459-- Marker 1203 */
	Sequence_Part31(369); /* --1151--378869--369-- Marker 1286 */
	Sequence_Part31(411); /* --1152--379280--411-- Marker 1287 */
	Sequence_Part31(356); /* --1153--379636--356-- Marker 1288 */
	Sequence_Part31(407); /* --1154--380043--407-- Marker 1204 */
	Sequence_Part31(342); /* --1155--380385--342-- Marker 1289 */
	Sequence_Part31(378); /* --1156--380763--378-- Marker 1290 */
	Sequence_Part31(370); /* --1157--381133--370-- Marker 1291 */
	Sequence_Part31(503); /* --1158--381636--503-- Marker 1205 */
	Sequence_Part31(416); /* --1159--382052--416-- Marker 1292 */
	Sequence_Part31(369); /* --1160--382421--369-- Marker 1293 */
	Sequence_Part31(427); /* --1161--382848--427-- Marker 1294 */
	Sequence_Part31(378); /* --1162--383226--378-- Marker 1295 */
	Sequence_Part31(349); /* --1163--383575--349-- Marker 1296 */
	Sequence_Part31(360); /* --1164--383935--360-- Marker 1297 */
	Sequence_Part31(365); /* --1165--384300--365-- Marker 1298 */
	
	Sequence_Part31(400); /* -------- Marker 1206 */
	Sequence_Part31(320); /* --1166--385020--720-- Marker 1299 */
	Sequence_Part31(380); /* --1167--385400--380-- Marker 1300 */
	Sequence_Part31(391); /* --1168--385791--391-- Marker 1301 */
	Sequence_Part31(409); /* --1169--386200--409-- Marker 1207 */
	Sequence_Part31(329); /* --1170--386529--329-- Marker 1302 */
	Sequence_Part31(361); /* --1171--386890--361-- Marker 1303 */
	Sequence_Part32(376); /* --1172--387266--376-- Marker 1208 */
	Sequence_Part32(5429); /* --1207--390000--2734-- FIN */
	
	StartTimer_16(14332+450);All_Off();StopTimer();
	}

}
