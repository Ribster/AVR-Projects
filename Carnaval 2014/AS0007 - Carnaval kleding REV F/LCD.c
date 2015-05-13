/*==========================================================================
The LCD connection is the same as Nokia LCD5110 and  is a?? Bit Pant Demo??
Just for ElecFreaks TFT01-2.2SP, which use SPI serial port and 240x320 pixel.
The driver is ILI9341.
 
by Elecfreaks
==========================================================================*/ 
#include "LCD.h"
#include <util/delay.h>
#include <avr/io.h>
#include "Initializations.h"

void init_SPI(void){

        /*  set the MOSI, SCK and SS line as output
                SS needs to be set as output if this controller
                is the only Master 
        */
        SPI_DDR |= (1<<MOSI) | (1<<SCK) | (1<<CS) | (1<<MISO);
		DDRD |= 1<<DC | 1<<RESET;
        
        /*  configure SPI 
                SPE = SPI Enable bit; always has to be set
                MSTR = Master bit; controls wether controller is Master or Slave; 1 = Master, 0 = Slave
                SPR0 = sets SPI clock to F_CPU / 16
                
                Please refer to the controller's datasheet 
                for a complete description of all available config bits
        */
        SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);  
		/*SPSR |= (1<<SPI2X);*/
		// | (1<<SPR0)     
}

void send_SPI(uint8_t sData){
        CS_LOW;
        SPDR = sData;
        while( !( SPSR & (1<<SPIF)) ){} // wait for end of transmission
		CS_HIGH;
}
 
void LCD_Wr_C(char VL)  
{   
  DC_LOW;
  send_SPI(VL);
}
 
void LCD_Wr(char VL)    
{
  DC_HIGH;
  send_SPI(VL);
}
 
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
   LCD_Wr_C(0x2a);
   LCD_Wr(x1>>8);
   LCD_Wr(x1);
   LCD_Wr(x2>>8);
   LCD_Wr(x2);
 
   LCD_Wr_C(0x2b);
   LCD_Wr(y1>>8);
   LCD_Wr(y1);
   LCD_Wr(y2>>8);
   LCD_Wr(y2);
 
   LCD_Wr_C(0x2C);         				 
}
 
void LCD_Init(void)
{
	init_SPI();
 
    RESET_LOW;
	_delay_ms(100);
	RESET_HIGH;
 
        LCD_Wr_C(0xCB);
        LCD_Wr(0x39);
        LCD_Wr(0x2C);
        LCD_Wr(0x00);
        LCD_Wr(0x34);
        LCD_Wr(0x02);
        
        LCD_Wr_C(0xCF); // Power Control B
        LCD_Wr(0x00);
        LCD_Wr(0XC1);
        LCD_Wr(0X30);
        
        LCD_Wr_C(0xE8); // DRIVER_TIMING_CONTROL_A
        LCD_Wr(0x85);
        LCD_Wr(0x00);
        LCD_Wr(0x78);
        
        LCD_Wr_C(0xEA); // DRIVER_TIMING_CONTROL_B
        LCD_Wr(0x00);
        LCD_Wr(0x00);
        
        LCD_Wr_C(0xED);
        LCD_Wr(0x64);
        LCD_Wr(0x03);
        LCD_Wr(0X12);
        LCD_Wr(0X81);
        
        LCD_Wr_C(0xF7);
        LCD_Wr(0x20);
 
        LCD_Wr_C(0xC0);    //Power control 
        LCD_Wr(0x09);   //VRH[5:0] 3V3
 
        LCD_Wr_C(0xC1);    //Power control 
        LCD_Wr(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_Wr_C(0xC5);    //VCOM control 
        LCD_Wr(0x3e);   //Contrast
        LCD_Wr(0x28); 
 
        LCD_Wr_C(0xC7);    //VCOM control2 
        LCD_Wr(0x86);   //--
 
        LCD_Wr_C(0x36);    // Memory Access Control 
        LCD_Wr(0x48);   //C8	   //48 68???//28 E8 ???
 
        LCD_Wr_C(0x3A);    // COLMOD: Pixel Format Set
        LCD_Wr(0x55); 
 
        LCD_Wr_C(0xB1);    // Frame control
        LCD_Wr(0x00);  
        LCD_Wr(0x18); 
 
        LCD_Wr_C(0xB6);    // Display Function Control 
        LCD_Wr(0x08); 
        LCD_Wr(0x82);
        LCD_Wr(0x27);  
/* 
        LCD_Write_COM(0xF2);    // 3Gamma Function Disable 
        LCD_Write_DATA(0x00); 
 
        LCD_Write_COM(0x26);    //Gamma curve selected 
        LCD_Write_DATA(0x01); 
 
        LCD_Write_COM(0xE0);    //Set Gamma 
        LCD_Write_DATA(0x0F); 
        LCD_Write_DATA(0x31); 
        LCD_Write_DATA(0x2B); 
        LCD_Write_DATA(0x0C); 
        LCD_Write_DATA(0x0E); 
        LCD_Write_DATA(0x08); 
        LCD_Write_DATA(0x4E); 
        LCD_Write_DATA(0xF1); 
        LCD_Write_DATA(0x37); 
        LCD_Write_DATA(0x07); 
        LCD_Write_DATA(0x10); 
        LCD_Write_DATA(0x03); 
        LCD_Write_DATA(0x0E); 
        LCD_Write_DATA(0x09); 
        LCD_Write_DATA(0x00); 
 
        LCD_Write_COM(0XE1);    //Set Gamma 
        LCD_Write_DATA(0x00); 
        LCD_Write_DATA(0x0E); 
        LCD_Write_DATA(0x14); 
        LCD_Write_DATA(0x03); 
        LCD_Write_DATA(0x11); 
        LCD_Write_DATA(0x07); 
        LCD_Write_DATA(0x31); 
        LCD_Write_DATA(0xC1); 
        LCD_Write_DATA(0x48); 
        LCD_Write_DATA(0x08); 
        LCD_Write_DATA(0x0F); 
        LCD_Write_DATA(0x0C); 
        LCD_Write_DATA(0x31); 
        LCD_Write_DATA(0x36); 
        LCD_Write_DATA(0x0F); 
*/
        LCD_Wr_C(0x11);    //Exit Sleep 
        _delay_ms(120); 
 
        LCD_Wr_C(0x29);    //Display on 
        LCD_Wr_C(0x2c);   
		
		
}
 
void MaxWindow(char RED, char GREEN, char BLUE)
{
	Window(0, 0, 240, 320, RED, GREEN, BLUE);
}

void Window(unsigned int xMin, unsigned int yMin, unsigned int xMax, unsigned int yMax, char wRED, char wGREEN, char wBLUE){
	Address_set(xMin,yMin,xMax,yMax);
	for(int i=yMin;i<yMax+1;i++)
	{
		for (int j=xMin;j<xMax+1;j++)
		{
			LCD_Wr(wBLUE);
			LCD_Wr(wGREEN);
			LCD_Wr(wRED);
		}
	}
}

void CharEngine(uint16_t TempCharacter[11], uint16_t xZero, uint16_t yZero){
			for (uint8_t i = 0; i<12; i++)
			{
				for (uint8_t j = 11; j>0; j--)
				{
					if (TempCharacter[i] & (1<<j))
					{
						for (int16_t Maxwidth = WidthCharacters; Maxwidth>0; Maxwidth-=12)
						{
							uint16_t xTemp =xZero-j;
							uint16_t yTemp =yZero-i;
							Window(xTemp, yTemp, xTemp, yTemp, ColorCharactersBlack, ColorCharactersBlack, ColorCharactersBlack);
						}
					}
				}
			}
}

void CharacterWrite(uint8_t Character, uint8_t NumberTile){
	uint16_t xMinTile;
	uint16_t xMaxTile;
	uint16_t yMinTile;
	uint16_t yMaxTile;
	
	if (NumberTile == Tile_1){
		xMinTile = Tile_1_Dimensions[0];
		yMinTile = Tile_1_Dimensions[1];
		xMaxTile = Tile_1_Dimensions[2];
		yMaxTile = Tile_1_Dimensions[3];
		
	}
	else if (NumberTile == Tile_2){
		xMinTile = Tile_2_Dimensions[0];
		yMinTile = Tile_2_Dimensions[1];
		xMaxTile = Tile_2_Dimensions[2];
		yMaxTile = Tile_2_Dimensions[3];
	}		
	else if (NumberTile == Tile_3){
		xMinTile = Tile_3_Dimensions[0];
		yMinTile = Tile_3_Dimensions[1];
		xMaxTile = Tile_3_Dimensions[2];
		yMaxTile = Tile_3_Dimensions[3];
	}
	else if (NumberTile == Tile_4){
		xMinTile = Tile_4_Dimensions[0];
		yMinTile = Tile_4_Dimensions[1];
		xMaxTile = Tile_4_Dimensions[2];
		yMaxTile = Tile_4_Dimensions[3];
	}
	else if (NumberTile == Tile_5){
		xMinTile = Tile_5_Dimensions[0];
		yMinTile = Tile_5_Dimensions[1];
		xMaxTile = Tile_5_Dimensions[2];
		yMaxTile = Tile_5_Dimensions[3];
	}
	else if (NumberTile == Tile_6){
		xMinTile = Tile_6_Dimensions[0];
		yMinTile = Tile_6_Dimensions[1];
		xMaxTile = Tile_6_Dimensions[2];
		yMaxTile = Tile_6_Dimensions[3];
	}
	
	
	if (Character == CharTileOne){
		CharEngine(Char_1, (xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters/2)), (yMaxTile-HeightCharacters*2));
	}
	else if (Character == CharTileTwo){
		CharEngine(Char_2, (xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters/2)), (yMaxTile-HeightCharacters*2));
	}
	else if (Character == CharTileThree){
		CharEngine(Char_3, (xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters/2)), (yMaxTile-HeightCharacters*2));
	}
	else if (Character == CharTileFour){
		CharEngine(Char_4, (xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters/2)), (yMaxTile-HeightCharacters*2));
	}
	else if (Character == CharTileFive){
		CharEngine(Char_5, (xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters/2)), (yMaxTile-HeightCharacters*2));
	}
	else if (Character == CharTileSix){
		CharEngine(Char_6, (xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters/2)), (yMaxTile-HeightCharacters*2));
	}
	else if (Character == CharStateIdle){
		
	}
	else if (Character == CharStateGood){
		CharEngine(Char_O, xMinTile+((xMaxTile-xMinTile)/2), (yMinTile+HeightCharacters*2));
		CharEngine(Char_K, xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters), (yMinTile+HeightCharacters*2));
	}
	else if (Character == CharStateBad){
		CharEngine(Char_N, xMinTile+((xMaxTile-xMinTile)/2)-(WidthCharacters/2), (yMinTile+HeightCharacters*2));
		CharEngine(Char_O, xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters/2), (yMinTile+HeightCharacters*2));
		CharEngine(Char_K, xMinTile+((xMaxTile-xMinTile)/2)+(WidthCharacters)+(WidthCharacters/2), (yMinTile+HeightCharacters*2));
	}
}

void WriteTile(uint8_t TileNumber, uint8_t TileState){
	uint8_t Color_TilebackgroundR = 0;
	uint8_t Color_TilebackgroundG = 0;
	uint8_t Color_TilebackgroundB = 0;
	uint8_t ChosenCharacterState = 0;
	uint8_t PreviousTileState = 0;
	
	if (TileNumber == Tile_1){
		if (Tile_1_State != State_Idle){
			PreviousTileState = 1;
		}
		Tile_1_State = TileState;
	}
	else if (TileNumber == Tile_2){
		if (Tile_2_State != State_Idle){
			PreviousTileState = 1;
		}
		Tile_2_State = TileState;
	}
	else if (TileNumber == Tile_3){
		if (Tile_3_State != State_Idle){
			PreviousTileState = 1;
		}
		Tile_3_State = TileState;
	}
	else if (TileNumber == Tile_4){
		if (Tile_4_State != State_Idle){
			PreviousTileState = 1;
		}
		Tile_4_State = TileState;
	}
	else if (TileNumber == Tile_5){
		if (Tile_5_State != State_Idle){
			PreviousTileState = 1;
		}
		Tile_5_State = TileState;
	}
	else if (TileNumber == Tile_6){
		if (Tile_6_State != State_Idle){
			PreviousTileState = 1;
		}
		Tile_6_State = TileState;
	}
	
	
	if (TileState == State_Idle)
	{
		Color_TilebackgroundR = TileColor_Idle[0];
		Color_TilebackgroundG = TileColor_Idle[1];
		Color_TilebackgroundB = TileColor_Idle[2];
		ChosenCharacterState = CharStateIdle;
	}
	else if (TileState == State_Good)
	{
		Color_TilebackgroundR = TileColor_Good[0];
		Color_TilebackgroundG = TileColor_Good[1];
		Color_TilebackgroundB = TileColor_Good[2];
		ChosenCharacterState = CharStateGood;
	}
	else if (TileState == State_Bad)
	{
		Color_TilebackgroundR = TileColor_Bad[0];
		Color_TilebackgroundG = TileColor_Bad[1];
		Color_TilebackgroundB = TileColor_Bad[2];
		ChosenCharacterState = CharStateBad;
	}
	
	uint16_t Tile_xMin = 0;
	uint16_t Tile_xMax = 0;
	uint16_t Tile_yMin = 0;
	uint16_t Tile_yMax = 0;
	uint8_t ChosenCharacterTile = 0;
	
	uint16_t xCharacterTile = 0;
	uint16_t yCharacterTile = 0;
	
	if (TileNumber == Tile_1){
		// Set position window
		Tile_xMin = Tile_1_Dimensions[0];
		Tile_yMin = Tile_1_Dimensions[1];
		Tile_xMax = Tile_1_Dimensions[2];
		Tile_yMax = Tile_1_Dimensions[3];
		// Set character write
		ChosenCharacterTile = CharTileOne;
	}
	else if (TileNumber == Tile_2){
		// Set position window
		Tile_xMin = Tile_2_Dimensions[0];
		Tile_yMin = Tile_2_Dimensions[1];
		Tile_xMax = Tile_2_Dimensions[2];
		Tile_yMax = Tile_2_Dimensions[3];
		// Set character write
		ChosenCharacterTile = CharTileTwo;
	}
	else if (TileNumber == Tile_3){
		// Set position window
		Tile_xMin = Tile_3_Dimensions[0];
		Tile_yMin = Tile_3_Dimensions[1];
		Tile_xMax = Tile_3_Dimensions[2];
		Tile_yMax = Tile_3_Dimensions[3];
		// Set character write
		ChosenCharacterTile = CharTileThree;
	}
	else if (TileNumber == Tile_4){
		// Set position window
		Tile_xMin = Tile_4_Dimensions[0];
		Tile_yMin = Tile_4_Dimensions[1];
		Tile_xMax = Tile_4_Dimensions[2];
		Tile_yMax = Tile_4_Dimensions[3];
		// Set character write
		ChosenCharacterTile = CharTileFour;
	}
	else if (TileNumber == Tile_5){
		// Set position window
		Tile_xMin = Tile_5_Dimensions[0];
		Tile_yMin = Tile_5_Dimensions[1];
		Tile_xMax = Tile_5_Dimensions[2];
		Tile_yMax = Tile_5_Dimensions[3];
		// Set character write
		ChosenCharacterTile = CharTileFive;
	}
	else if (TileNumber == Tile_6){
		// Set position window
		Tile_xMin = Tile_6_Dimensions[0];
		Tile_yMin = Tile_6_Dimensions[1];
		Tile_xMax = Tile_6_Dimensions[2];
		Tile_yMax = Tile_6_Dimensions[3];
		// Set character write
		ChosenCharacterTile = CharTileSix;
	}
	if (PreviousTileState){
		Window(Tile_xMin, Tile_yMin+HeightCharacters*4, Tile_xMax, Tile_yMax, Color_TilebackgroundR, Color_TilebackgroundG, Color_TilebackgroundB);
	}
	else{
		Window(Tile_xMin, Tile_yMin, Tile_xMax, Tile_yMax, Color_TilebackgroundR, Color_TilebackgroundG, Color_TilebackgroundB);
	}
	
	CharacterWrite(ChosenCharacterTile, TileNumber);
	CharacterWrite(ChosenCharacterState, TileNumber);
}

void LCD_Startscreen(){
	
	Window(L_xMax+1,LCDMin,R_xMin-1,LCDyMax,0x00,0x00,0x00);
	Window(LCDMin,LR3_yMax+1,LCDxMax,LR2_yMin-1,0x00,0x00,0x00);
	Window(LCDMin,LR2_yMax+1,LCDxMax,LR1_yMin-1,0x00,0x00,0x00);
	
	WriteTile(Tile_1, State_Idle);
	WriteTile(Tile_2, State_Idle);
	WriteTile(Tile_3, State_Idle);
	WriteTile(Tile_4, State_Idle);
	WriteTile(Tile_5, State_Idle);
	WriteTile(Tile_6, State_Idle);
}	
 
void setup()
{
	LCD_Init();
	LCD_Startscreen();
}



void LCDTest(){
	#define IS_ODD(x)   ((x) & 1)
	#define IS_EVEN(x)  (!(IS_ODD(x)))
	
	if ((ButtonL1) && (ButtonL2) && (ButtonL3) && (ButtonR1) && (ButtonR2) && (ButtonR3))
	{
		LCD_Startscreen();
		_delay_ms(500);
	}
	else if (ButtonL1)
	{
		WriteTile(Tile_1, State_Good);
		WS2811_DebugTest(Group1_Start, Group1_Length);
		WriteTile(Tile_1, State_Idle);
	}
	else if (ButtonL2)
	{
		WriteTile(Tile_2, State_Good);
		WS2811_DebugTest(Group3_Start, Group3_Length);
 		WriteTile(Tile_2, State_Idle);
	}
	else if (ButtonL3)
	{
		WriteTile(Tile_3, State_Good);
		WS2811_SetMidUpdate(GroupALL_Start, GroupALL_Length, COLOR_UIT, Neen);
		WS2811_SetMidUpdate(1,1,COLOR_ROOD, Neen);
		_delay_ms(1000);
		WS2811_SetMidUpdate(2,1,COLOR_BLAUW, Neen);
		_delay_ms(1000);
		WS2811_SetMidUpdate(1,2,COLOR_UIT, Neen);
		_delay_ms(1000);
		WS2811_SetMidUpdate(1,2,COLOR_GROEN, Neen);
		_delay_ms(1000);
		WS2811_SetMidUpdate(1,2,COLOR_UIT, Neen);
		WriteTile(Tile_3, State_Idle);
	}
	else if (ButtonR1)
	{
		WriteTile(Tile_4, State_Good);
		WS2811_DebugTest(Group2_Start, Group2_Length);
 		WriteTile(Tile_4, State_Idle);
	}
	else if (ButtonR2)
	{
		WriteTile(Tile_5, State_Good);
		WS2811_DebugTest(Group4_Start, Group4_Length);
 		WriteTile(Tile_5, State_Idle);
	}
	else if (ButtonR3)
	{
		WriteTile(Tile_6, State_Good);
		WS2801_SetColorUpdate(0x10);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x20);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x30);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x40);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x50);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x60);
		_delay_ms(100);
		WS2801_SetColorUpdate(0xff);
		_delay_ms(3500);
		WS2801_SetColorUpdate(0x60);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x50);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x40);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x30);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x20);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x10);
		_delay_ms(100);
		WS2801_SetColorUpdate(0x00);
		WriteTile(Tile_6, State_Idle);
		
	}
}