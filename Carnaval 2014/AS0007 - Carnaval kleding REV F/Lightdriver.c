/*
 * Lightdriver.c
 *
 * Created: 7/02/2014 23:08:02
 *  Author: Robbe Van Assche
 */ 
#include "GlobalDefinitions.h"
#include "Initializations.h"
#include "Lightdriver.h"
#include "Light Functions.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>



//WS2801
uint8_t WS2801[0];

void WS2801_Update(void){
	for(uint8_t bits = 8 ; bits != 0; bits--) {
	 	WS2801_CKI_LOW;
	 	uint8_t mask = (1 << (bits-1));
	 	if(WS2801[0] & mask){
	 	WS2801_SDI_HIGH;
		}
	 	else{WS2801_SDI_LOW;}
	 	WS2801_CKI_HIGH;
	}

	for(uint8_t bits = 17 ; bits != 0; bits--) {
		WS2801_CKI_LOW;
		WS2801_SDI_LOW;
		_delay_us(1);
		WS2801_CKI_HIGH;
	}
	
	_delay_us(500);
}

void WS2801_SetColor(uint8_t Color){
	WS2801[0] = Color;
}

void WS2801_Fade(uint16_t Delay, uint8_t MaxMinVal0to80, uint8_t In1Out0){
uint8_t LoopCount=MaxMinVal0to80;
uint8_t Color_Temp;
if(In1Out0==0){Color_Temp=0xF0;}
do {
	StartTimer_16(Delay); if(In1Out0){Color_Temp+=0x01;}else{Color_Temp-=0x10;}WS2801_SetColor(Color_Temp); StopTimer();
} while (--LoopCount);
}

uint8_t WS2801_GetColor(void){
	return WS2801[0];
}
  
//WS2811
uint32_t WS2811[WS2811_ledcount];




void WS2811_Update(void){
	uint8_t led = WS2811_ledcount;
	do {
		uint32_t mask = 0x00800000;
		do {
			if ((WS2811[led]&mask)==0) {
				// Send a '0' HI: 250nS LOW: 1000nS
				WS2811_HIGH;  // Hi (start)
				//WS2811_TimerHandler(TIM_Delay_250);
				NOP_250;
				WS2811_LOW;  // Low
				//WS2811_TimerHandler(TIM_Delay_1000);
				NOP_1000;
			}
			else {
				// Send a '1' HI: 600nS LOW: 650nS
				WS2811_HIGH;  // Hi (start)
				//WS2811_TimerHandler(TIM_Delay_600);
				NOP_600;
				WS2811_LOW;  // Low
				//WS2811_TimerHandler(TIM_Delay_650);
				NOP_650;
				
			}	 
		} while (mask>>= 1);
	}while (led--);
	_delay_us(70);
}
void WS2811_SetColor(uint32_t Color, uint8_t Pixel){
		WS2811[Pixel] = Color;
}
void WS2811_SetColorALL(uint32_t Color){
	uint8_t x = WS2811_ledcount;
	do {
		WS2811[x] = Color;
	} while (x--);
}	

void WS2811_SetColorPixelIntensity(uint8_t ColorBlue0Green8Red16, uint8_t Pixel,uint8_t Intensity0to7, uint8_t ADD1No0){
	if (ADD1No0){
		WS2811[Pixel] |= (uint32_t)(1<<Intensity0to7)<<ColorBlue0Green8Red16;
	}
	else if (ADD1No0==3)
	{
		WS2811[Pixel] &= ~((uint32_t)(1<<Intensity0to7)<<ColorBlue0Green8Red16);
	}
	else if (ADD1No0==4)
	{
		WS2811[Pixel] += (uint32_t)(1<<Intensity0to7)<<ColorBlue0Green8Red16;
	}
	else{
		WS2811[Pixel] = (uint32_t)(1<<Intensity0to7)<<ColorBlue0Green8Red16;
	}
	
}

void WS2811_SetGroupColor(uint32_t Color, uint8_t Groupnmbr, uint8_t Even0Odd1All2){
	
	uint8_t StartPix = 0;
	uint8_t EndPix = 0;
	
	
	if (Groupnmbr==1){
		StartPix = Group1_Start;
		EndPix = Group1_End;
	}
	else if (Groupnmbr==2){
		StartPix = Group2_Start;
		EndPix = Group2_End;
	}
	else if (Groupnmbr==3){
		StartPix = Group3_Start;
		EndPix = Group3_End;
	}
	else if (Groupnmbr==4){
		StartPix = Group4_Start;
		EndPix = Group4_End;
	}
	
	for(uint8_t t=StartPix; t<EndPix+1; t++){
		uint8_t UpdateFlag = 1;
		
		if (Even0Odd1All2==0){
			if (!(t & 1)){
				UpdateFlag = 0;
			}
		}
		else if (Even0Odd1All2==1){
			if (t & 1){
				UpdateFlag = 0;
			}
		}
		else if (Even0Odd1All2==2){
				UpdateFlag = 0;
		}
		
		if (UpdateFlag==0)
		{
			WS2811_SetColor(Color,t);
		}
		
	}
}

void WS2811_SetGroupPixel(uint32_t Color, uint8_t Groupnmbr, uint8_t GroupMember){
	uint8_t StartPix = 0;
	
	if (Groupnmbr==1){
		StartPix = Group1_Start;
	}
	else if (Groupnmbr==2){
		StartPix = Group2_Start;
	}
	else if (Groupnmbr==3){
		StartPix = Group3_Start;
	}
	else if (Groupnmbr==4){
		StartPix = Group4_Start;
	}
		WS2811_SetColor(Color,StartPix+GroupMember-1);
}


void WS2811_GroupIntensify(uint8_t ColorBlue0Green8Red16, uint8_t Group,uint8_t Intensity0to7, uint8_t ADD1No0){
	uint8_t StartPix;
	uint8_t EndPix;
	if (Group==1){
		StartPix = Group1_Start;
		EndPix = Group1_End;
	}
	else if (Group==2){
		StartPix = Group2_Start;
		EndPix = Group2_End;
	}
	else if (Group==3){
		StartPix = Group3_Start;
		EndPix = Group3_End;
	}
	else if (Group==4){
		StartPix = Group4_Start;
		EndPix = Group4_End;
	}
	for(uint8_t t=StartPix; t<EndPix+1; t++){
		WS2811_SetColorPixelIntensity(ColorBlue0Green8Red16, t, Intensity0to7, ADD1No0);
	}
}

void WS2811_Rotate(uint8_t CW1CCW0, uint8_t GpNmbr1to5){
	if (GpNmbr1to5==1){
		uint32_t tempColor = WS2811[Group1_Start];
		if (CW1CCW0==1){
			WS2811[Group1_Start] = WS2811[Group1_End];
			WS2811[Group1_End] = WS2811[Group1_Start+4];
			WS2811[Group1_Start+4] = WS2811[Group1_Start+3];
			WS2811[Group1_Start+3] = WS2811[Group1_Start+2];
			WS2811[Group1_Start+2] = WS2811[Group1_Start+1];
			WS2811[Group1_Start+1] = tempColor;
		}
		else if (CW1CCW0==0){
			WS2811[Group1_Start] = WS2811[Group1_Start+1];
			WS2811[Group1_Start+1] = WS2811[Group1_Start+2];
			WS2811[Group1_Start+2] = WS2811[Group1_Start+3];
			WS2811[Group1_Start+3] = WS2811[Group1_Start+4];
			WS2811[Group1_Start+4] = WS2811[Group1_End];
			WS2811[Group1_End] = tempColor;
		}
	}
	else if (GpNmbr1to5==2|GpNmbr1to5==3){
		uint8_t StartNmbr;
		
		if (GpNmbr1to5==2){
			StartNmbr = Group2_Start;
		}
		else if (GpNmbr1to5==3){
			StartNmbr = Group3_Start;
		}
		
		uint32_t tempColor = WS2811[StartNmbr+1];
		
		if (CW1CCW0==1){
			if (GpNmbr1to5==2|GpNmbr1to5==3){
				WS2811[StartNmbr+1] = WS2811[StartNmbr+2];
				WS2811[StartNmbr+2] = WS2811[StartNmbr+3];
				WS2811[StartNmbr+3] = WS2811[StartNmbr+4];
				WS2811[StartNmbr+4] = tempColor;
			}
		}
		else if (CW1CCW0==0){
			if (GpNmbr1to5==2|GpNmbr1to5==3){
				WS2811[StartNmbr+1] = WS2811[StartNmbr+4];
				WS2811[StartNmbr+4] = WS2811[StartNmbr+3];
				WS2811[StartNmbr+3] = WS2811[StartNmbr+2];
				WS2811[StartNmbr+2] = tempColor;
			}
		}
	}
	else if (GpNmbr1to5==4|GpNmbr1to5==5){
		uint8_t StartNmbr;
		uint8_t EndNmbr;
		if (GpNmbr1to5==4){
			StartNmbr = Group4_Start+1;
		}
		else if (GpNmbr1to5==5){
			StartNmbr = Group4_Start+2;
		}
		
		uint32_t tempColor = WS2811[StartNmbr];
		
		if (CW1CCW0==1){
			WS2811[StartNmbr] = WS2811[StartNmbr+2];
			WS2811[StartNmbr+2] = WS2811[StartNmbr+4];
			WS2811[StartNmbr+4] = WS2811[StartNmbr+6];
			WS2811[StartNmbr+6] = tempColor;
		}
		else if (CW1CCW0==0){
			WS2811[StartNmbr] = WS2811[StartNmbr+6];
			WS2811[StartNmbr+6] = WS2811[StartNmbr+4];
			WS2811[StartNmbr+4] = WS2811[StartNmbr+2];
			WS2811[StartNmbr+2] = tempColor;
		}
	}
	
}

void WS2811_Replace(uint32_t FromColor, uint32_t ToColor){
	uint8_t x = WS2811_ledcount;
	do {
		if (WS2811[x] == FromColor)
		{
			WS2811[x] = ToColor;
		}
	} while (x--);
}
void WS2811_SWAP(uint8_t GroupFrom1or2or3or4is4inor5is4out, uint8_t GroupTo1or2or3or4is4inor5is4out){
	uint32_t SwapInput[5];
	uint32_t SwapOutput[5];
	
	if (GroupFrom1or2or3or4is4inor5is4out==1){
		SwapInput[0] = WS2811[Group1_Start];
		SwapInput[1] = WS2811[Group1_Start+2];
		SwapInput[2] = WS2811[Group1_Start+3];
		SwapInput[3] = WS2811[Group1_Start+5];
		SwapInput[4] = WS2811[Group1_Start+4];
	}
	else if(GroupFrom1or2or3or4is4inor5is4out==2){
		SwapInput[0] = WS2811[Group2_Start+1];
		SwapInput[1] = WS2811[Group2_Start+2];
		SwapInput[2] = WS2811[Group2_Start+3];
		SwapInput[3] = WS2811[Group2_Start+4];
		SwapInput[4] = WS2811[Group2_Start];
	}
	else if(GroupFrom1or2or3or4is4inor5is4out==3){
		SwapInput[0] = WS2811[Group3_Start+1];
		SwapInput[1] = WS2811[Group3_Start+2];
		SwapInput[2] = WS2811[Group3_Start+3];
		SwapInput[3] = WS2811[Group3_Start+4];
		SwapInput[4] = WS2811[Group3_Start];
	}
	else if(GroupFrom1or2or3or4is4inor5is4out==4){
		SwapInput[0] = WS2811[Group4_Start+1];
		SwapInput[1] = WS2811[Group4_Start+3];
		SwapInput[2] = WS2811[Group4_Start+5];
		SwapInput[3] = WS2811[Group4_Start+7];
		SwapInput[4] = WS2811[Group4_Start];
	}
	else if(GroupFrom1or2or3or4is4inor5is4out==5){
		SwapInput[0] = WS2811[Group4_Start+2];
		SwapInput[1] = WS2811[Group4_Start+4];
		SwapInput[2] = WS2811[Group4_Start+6];
		SwapInput[3] = WS2811[Group4_Start+8];
		SwapInput[4] = WS2811[Group4_Start];
	}
	
	if (GroupTo1or2or3or4is4inor5is4out==1){
		WS2811[Group1_Start] = SwapInput[0];
		WS2811[Group1_Start+2] = SwapInput[1];
		WS2811[Group1_Start+3] = SwapInput[2];
		WS2811[Group1_Start+5] = SwapInput[3];
		WS2811[Group1_Start+4] = SwapInput[4];
		WS2811[Group1_Start+1] = SwapInput[4];
	}
	else if(GroupTo1or2or3or4is4inor5is4out==2){
		WS2811[Group2_Start+1] = SwapInput[0];
		WS2811[Group2_Start+2] = SwapInput[1];
		WS2811[Group2_Start+3] = SwapInput[2];
		WS2811[Group2_Start+4] = SwapInput[3];
		WS2811[Group2_Start] = SwapInput[4];
	}
	else if(GroupTo1or2or3or4is4inor5is4out==3){
		WS2811[Group3_Start+1] = SwapInput[0];
		WS2811[Group3_Start+2] = SwapInput[1];
		WS2811[Group3_Start+3] = SwapInput[2];
		WS2811[Group3_Start+4] = SwapInput[3];
		WS2811[Group3_Start] = SwapInput[4];
	}
	else if(GroupTo1or2or3or4is4inor5is4out==4){
		WS2811[Group4_Start+1] = SwapInput[0];
		WS2811[Group4_Start+3] = SwapInput[1];
		WS2811[Group4_Start+5] = SwapInput[2];
		WS2811[Group4_Start+7] = SwapInput[3];
		WS2811[Group4_Start] = SwapInput[4];
	}
	else if(GroupTo1or2or3or4is4inor5is4out==5){
		WS2811[Group4_Start+2] = SwapInput[0];
		WS2811[Group4_Start+4] = SwapInput[1];
		WS2811[Group4_Start+6] = SwapInput[2];
		WS2811[Group4_Start+8] = SwapInput[3];
		WS2811[Group4_Start] = SwapInput[4];
	}
	
}

void WS2811_Shift(uint8_t BackToFrontAdd0FrontToBackAdd1BackToFront2FrontToBack3, uint32_t ColorToAdd){
	uint32_t WriteBuffer[] = {ColorToAdd, WS2811[Group1_Start+2],WS2811[Group1_Start+1],WS2811[Group1_Start],WS2811[Group4_Start+4],WS2811[Group4_Start+5],WS2811[Group4_Start+6],WS2811[Group4_Start+7]};
		
	if (BackToFrontAdd0FrontToBackAdd1BackToFront2FrontToBack3==1 | BackToFrontAdd0FrontToBackAdd1BackToFront2FrontToBack3==3){
		WriteBuffer[0]=WriteBuffer[2];
		WriteBuffer[1]=WriteBuffer[3];
		WriteBuffer[2]=WriteBuffer[4];
		WriteBuffer[3]=WriteBuffer[5];
		WriteBuffer[4]=WriteBuffer[6];
		WriteBuffer[5]=WriteBuffer[7];
		WriteBuffer[6]=WS2811[Group4_Start+8];
		WriteBuffer[7]=ColorToAdd;
	}
	
	if (BackToFrontAdd0FrontToBackAdd1BackToFront2FrontToBack3==2){
		WriteBuffer[0] = WS2811[Group4_Start+8];
	}
	else if (BackToFrontAdd0FrontToBackAdd1BackToFront2FrontToBack3==3){
		WriteBuffer[7] = WS2811[Group1_Start+2];
	}
	
	WS2811_SetGroupPixel(WriteBuffer[7],4,9);
	WS2811_SetGroupPixel(WriteBuffer[6],2,3);
	WS2811_SetGroupPixel(WriteBuffer[6],2,4);
	WS2811_SetGroupPixel(WriteBuffer[6],3,3);
	WS2811_SetGroupPixel(WriteBuffer[6],3,4);
	WS2811_SetGroupPixel(WriteBuffer[6],4,2);
	WS2811_SetGroupPixel(WriteBuffer[6],4,8);
	WS2811_SetGroupPixel(WriteBuffer[5],2,1);
	WS2811_SetGroupPixel(WriteBuffer[5],3,1);
	WS2811_SetGroupPixel(WriteBuffer[5],4,1);
	WS2811_SetGroupPixel(WriteBuffer[5],4,3);
	WS2811_SetGroupPixel(WriteBuffer[5],4,7);
	WS2811_SetGroupPixel(WriteBuffer[4],2,2);
	WS2811_SetGroupPixel(WriteBuffer[4],2,5);
	WS2811_SetGroupPixel(WriteBuffer[4],3,2);
	WS2811_SetGroupPixel(WriteBuffer[4],3,5);
	WS2811_SetGroupPixel(WriteBuffer[4],4,4);
	WS2811_SetGroupPixel(WriteBuffer[4],4,6);
	WS2811_SetGroupPixel(WriteBuffer[3],4,5);
	WS2811_SetGroupPixel(WriteBuffer[2],1,1);
	WS2811_SetGroupPixel(WriteBuffer[2],1,6);
	WS2811_SetGroupPixel(WriteBuffer[1],1,2);
	WS2811_SetGroupPixel(WriteBuffer[1],1,5);
	WS2811_SetGroupPixel(WriteBuffer[0],1,3);
	WS2811_SetGroupPixel(WriteBuffer[0],1,4);
	
}

void WS2811_ShiftBackToFront(uint8_t Times, uint32_t ColorToAdd, uint8_t Add0Rotate1){
	uint8_t Flag = Times;
	do 
	{
		if (Add0Rotate1==0){
			WS2811_Shift(0, ColorToAdd);
		}
		else if (Add0Rotate1==1){
			WS2811_Shift(2, ColorToAdd);
		}
		Flag--;
	} while (Flag);
}

void WS2811_ShiftFrontToBack(uint8_t Times, uint32_t ColorToAdd, uint8_t Add0Rotate1){
	uint8_t Flag = Times;
	do
	{
		if (Add0Rotate1==0){
			WS2811_Shift(1, ColorToAdd);
		}
		else if (Add0Rotate1==1){
			WS2811_Shift(3, ColorToAdd);
		}
		Flag--;
	} while (Flag);
}

uint32_t WS2811_GetColor(uint8_t Pixel){
	return WS2811[Pixel];
}

void WS2811_SetLeft(uint32_t Color){
	WS2811_SetGroupPixel(Color,1,1);
	WS2811_SetGroupPixel(Color,1,2);
	WS2811_SetGroupPixel(Color,1,3);
	WS2811_SetGroupColor(Color,2,2);
	WS2811_SetGroupPixel(Color,4,5);
	WS2811_SetGroupPixel(Color,4,6);
	WS2811_SetGroupPixel(Color,4,7);
	WS2811_SetGroupPixel(Color,4,8);
	WS2811_SetGroupPixel(Color,4,9);
}
void WS2811_SetRight(uint32_t Color){
	WS2811_SetGroupPixel(Color,1,4);
	WS2811_SetGroupPixel(Color,1,5);
	WS2811_SetGroupPixel(Color,1,6);
	WS2811_SetGroupColor(Color,3,2);
	WS2811_SetGroupPixel(Color,4,1);
	WS2811_SetGroupPixel(Color,4,2);
	WS2811_SetGroupPixel(Color,4,3);
	WS2811_SetGroupPixel(Color,4,4);
}