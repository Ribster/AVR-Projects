/*
 * MPU_6050.c
 *
 * Created: 10-12-2013 22:54:42
 *  Author: Administrator
 */ 

//#define ACCMET_TEST

#ifdef ACCMET_TEST

#ifdef ACCMET_TEST
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h>  //include libm
#include "SRC/mpu6050.h"
#endif


#if MPU6050_GETATTITUDE == 0
int16_t ax = 0;
int16_t ay = 0;
int16_t az = 0;
int16_t gx = 0;
int16_t gy = 0;
int16_t gz = 0;
double axg = 0;
double ayg = 0;
double azg = 0;
double gxds = 0;
double gyds = 0;
double gzds = 0;
#endif

#if MPU6050_GETATTITUDE == 1 || MPU6050_GETATTITUDE == 2
long *ptr = 0;
double qw = 1.0f;
double qx = 0.0f;
double qy = 0.0f;
double qz = 0.0f;
double roll = 0.0f;
double pitch = 0.0f;
double yaw = 0.0f;
#endif

//init interrupt
sei();

//init mpu6050
mpu6050_init();
_delay_ms(50);

//init mpu6050 dmp processor
#if MPU6050_GETATTITUDE == 2
mpu6050_dmpInitialize();
mpu6050_dmpEnable();
_delay_ms(10);
#endif

#endif

		#ifdef ACCMET_TEST
		#if MPU6050_GETATTITUDE == 0
		mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
		mpu6050_getConvData(&axg, &ayg, &azg, &gxds, &gyds, &gzds);
		#endif

		#if MPU6050_GETATTITUDE == 1
		mpu6050_getQuaternion(&qw, &qx, &qy, &qz);
		mpu6050_getRollPitchYaw(&roll, &pitch, &yaw);
		_delay_ms(10);
		#endif

		#if MPU6050_GETATTITUDE == 2
		if(mpu6050_getQuaternionWait(&qw, &qx, &qy, &qz)) {
			mpu6050_getRollPitchYaw(qw, qx, qy, qz, &roll, &pitch, &yaw);
		}
		_delay_ms(10);
		#endif
		
		#define REGEL 0.2
		#define VERTR _delay_ms(120);
		
		if (qx>REGEL)
		{
			WS2811_SetGroupColorUpdate(Group4, COLOR_BLAUW, Neen);
			VERTR
		}
		else if (qx<(-REGEL))
		{
			WS2811_SetGroupColorUpdate(Group4, COLOR_ROOD, Neen);
			VERTR
		}
		// 			else{
			// 				WS2811_SetGroupColorUpdate(Group4, COLOR_UIT, Neen);
		// 			}
		
		if (qy>REGEL)
		{
			WS2811_SetGroupColorUpdate(Group2, COLOR_GEELGROEN, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_GEELGROEN, Neen);
			VERTR
		}
		else if (qy<(-REGEL))
		{
			WS2811_SetGroupColorUpdate(Group2, COLOR_BRUIN, Neen);
			WS2811_SetGroupColorUpdate(Group3, COLOR_BRUIN, Neen);
			VERTR
		}
		// 			else{
			// 				WS2811_SetGroupColorUpdate(Group2, COLOR_UIT, Neen);
		// 			}
		
		if (qz>REGEL)
		{
			WS2811_SetGroupColorUpdate(Group1, COLOR_WIT, Neen);
			VERTR
		}
		else if (qz<(-REGEL))
		{
			WS2811_SetGroupColorUpdate(Group1, COLOR_ZEEBLAUW, Neen);
			VERTR
		}
		// 			else{
			// 				WS2811_SetGroupColorUpdate(Group1, COLOR_UIT, Neen);
		// 			}

		#endif