#include "pt2313/pt2313.h"
#include "iic.h"
#include "setup.h"
#include "ui_api.h"

#if defined(_FY_RADIO_FM_TDA778x_GATE_)||defined(_TUNER_DRIVER_SI474x_)||defined(_XC3318_TDA778x_GATE_)
#include "tuner_drv.h"
#endif


extern uint8 fm_inside_check(void);


static pt2313_audio_t pt2313_audio;

#if defined(_ENABLE_PT2313_I2C_)||defined(_AUDIO_I2C_WITH_LCD_)
static void audio_scl_init(void)		{	I2C_SCL_INIT1();				};
static void audio_scl_h(void)			{	I2C_SCL_H1();					};
static void audio_scl_l(void)			{	I2C_SCL_L1();					};
static void audio_sda_init(void)		{	I2C_SDA_INIT1();				};
static void audio_sda_h(void)		{	I2C_SDA_H1();					};
static void audio_sda_l(void)			{	I2C_SDA_L1();					};
static uint8 audio_sda_get(void)		{	return (I2C_SDA_GET1()?1:0);	};
#else
static void audio_scl_init(void)		{	I2C_SCL_INIT();					};
static void audio_scl_h(void)			{	I2C_SCL_H();					};
static void audio_scl_l(void)			{	I2C_SCL_L();					};
static void audio_sda_init(void)		{	I2C_SDA_INIT();					};
static void audio_sda_h(void)		{	I2C_SDA_H();					};
static void audio_sda_l(void)			{	I2C_SDA_L();					};
static uint8 audio_sda_get(void)		{	return (I2C_SDA_GET()?1:0);		};
#endif

static const i2c_gpio_t audio_gpio =
{
	audio_scl_init,
	audio_scl_h,
	audio_scl_l,
	audio_sda_init,
	audio_sda_h,
	audio_sda_l,
	audio_sda_get,
};

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

#if (AUDIO_VOLUME_MAX == 36)
static const uint8 VolumeTab[37]=
{
	63,58,54,50,46,42,39,37,35,33,31,29,27,25,23,
	21,20,19,18,17,16,15,14,13,12,11,10, 9,
	 8,7, 6, 5,4, 3, 2,1,0,
};
#elif (AUDIO_VOLUME_MAX == 30)
#if defined(_KLD3000_VOLUME_STEP_)
static const uint8 VolumeTab[31]=
{
    63,50,45,41,38,36,34,32,30,28,
    26,24,22,20,18,17,16,15,14,13,
    12,11,10, 9, 8, 7, 6, 5, 4, 3,
    2,
};
#else
static const uint8 VolumeTab[31]=
{
	63,50,45,41,39,37,35,33,31,29,
	27,25,23,21,19,17,16,15,14,13,
	12,11,10, 9, 8, 7, 6, 5, 4, 3,
	2,1,0,
};
#endif
#elif (AUDIO_VOLUME_MAX == 31)
static const uint8 VolumeTab[32]=
{
	63,45,41,39,37,35,33,31,29,
	27,25,23,21,19,17,16,15,14,13,
	12,11,10, 9, 8, 7, 6, 5, 4, 3,
	2,1,0,
};
#elif (AUDIO_VOLUME_MAX == 32)
static const uint8 VolumeTab[33]=
{
	63,50,45,41,39,37,35,33,31,29,
	27,25,23,21,19,17,16,15,14,13,
	12,11,10, 9, 8, 7, 6, 5, 4, 3,
	2,1,0,
};
#elif (AUDIO_VOLUME_MAX == 40)
#if defined(_FY7000D_VOLUME_STEP_)
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,11,10,9,8,7,6,5,4,3,3,2,2,1,1
};
#elif defined(_CHANGE_BL7890_VLOUME_STEP_)
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,0,0
};
#elif defined(_JTI2230_VOL_STEP_)
#if defined(_CHANGE_SPECIAL_VOL_STEP)
static const uint8 VolumeTab[41]=
{
	63,49,47,45,43,41,39,37,35,33,
	31,30,29,28,27,26,25,25,24,24,
	23,23,22,21,20,19,18,17,16,15,
	14,13,12,11,10, 9, 8, 7, 6, 5,
	4,
};
#else
static const uint8 VolumeTab[41]=
{
	63,49,47,45,43,41,39,37,35,33,
	31,29,28,27,26,25,24,23,22,21,
	20,19,18,17,16,15,14,13,12,11,
	10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
	0,
};
#endif
#elif defined(_JYL3802E_VOLUME_STEP_)
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,11,10,9,8,7,6,5,5,4,4,3,3,2,2
};
#elif defined(_ENABLE_PT2313_TEST1_)//25W
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5
};
#elif defined(_ENABLE_PT2313_TEST2_)//20W
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8,7
};
#elif defined(_WLS1403_VOLUME_STEP1_)
static const uint8 VolumeTab[41]=
{
	63,60,57,55,54,53,52,51,50,49,
	48,47,46,45,44,43,42,41,40,39,
	38,37,36,35,34,33,32,31,30,29,
	28,27,26,25,24,23,22,21,20,19,
	18,
};
#elif defined(_ENABLE_PT2313_VOL_)
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,24,23,23,22,22,21,21,
20,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3
};
#elif defined(_FY7000B_PT2313_VOLUME_)
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,11,10,9,8,7,7,6,6,5,5,4,4,3,3
};
#else
static const uint8 VolumeTab[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,0,0
};
#endif

#elif (AUDIO_VOLUME_MAX == 35)
static const uint8 VolumeTab[36]=
{
	63,58,54,50,46,42,39,37,35,33,31,29,27,25,23,
	21,19,18,17,16,15,14,13,12,11,10, 9,
	 8,7, 6, 5,4, 3, 2,1,0,
};

#elif (AUDIO_VOLUME_MAX == 43)
static const uint8 VolumeTab[44]=
{
	63,60,57,54,52,50,48,46,44,42,
	40,38,36,34,32,31,30,29,28,27,
	26,24,21,20,19,18,17,16,15,14,
	13,12,11,10, 9, 8, 7,6, 5, 4,
	3, 2, 1,0,
};
#elif (AUDIO_VOLUME_MAX == 46)
#if defined(_ENABLE_VOL_STEP_)
static const uint8 VolumeTab[47]=
{
	63,60,57,54,52,50,48,46,44,42,
	40,38,36,34,33,32,31,30,30,29,
	28,28,27,26,26,25,24,23,22,21,
	20,19,18,17,16,13,12,11,10, 9,
	8, 7, 6, 5, 4, 3,2,
};
#else
static const uint8 VolumeTab[47]=
{
	63,60,57,54,52,50,48,46,44,42,
	40,38,36,34,32,31,30,29,28,27,
	26,25,24,23,22,21,20,19,18,17,
	16,15,14,13,12,11,10, 9, 8, 7,
	6, 5, 4, 3, 2, 1,0,
};
#endif
#elif (AUDIO_VOLUME_MAX == 50)
static const uint8 VolumeTab[51]=
{
	63,61,59,57,56,54,52,50,48,46,
	44,42,40,38,36,35,34,33,32,31,
	30,29,28,27,26,25,24,23,22,21,
	20,19,18,17,16,15,14,13,12,11,
	10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
	0,
};
#elif (AUDIO_VOLUME_MAX == 63)
#if defined(_HY505_SPECIAL_VOLUME_)
static const uint8 VolumeTab[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,37,34,31,28,26,25,25,24,24,
	23,23,22,22,21,21,20,20,19,19,
	18,18,17,17,16,16,15,15,14,14,
	13,13,12,12,11,11,10,10,9,9,8,8,7,
	7, 6, 6, 5, 5, 4, 4 ,3, 3, 2,2
};
#elif defined(_CY2200_SPECIAL_VOLUME_)
static const uint8 VolumeTab[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,37,34,32,30,30,29,29,28,28,
	27,27,26,26,25,25,24,24,23,23,
	22,22,21,21,20,20,19,19,18,18,
	17,17,16,16,15,15,14,14,13,13,12,12,11,
	11, 10, 10, 9, 9, 8, 8 ,7, 7, 6,6
};
#elif defined(_TE2011_SPECIAL_VOLUME_)
static const uint8 VolumeTab[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,37,34,34,33,33,32,32,31,31,
	30,30,29,29,28,28,27,27,26,26,
	25,25,24,24,23,23,22,22,21,21,
	20,20,19,19,18,18,17,17,16,16,
	15,15,14,14,13,13,12,12,11,11,10,10, 9,9
};
#elif defined(_WLS5260_SPECIAL_VOLUME_)
static const uint8 VolumeTab[64]=
{
	63,55,52,49,46,44,41,39,36,34, //0
	31,29,27,25,24,24,23,23,22,22,
	21,21,20,20,19,19,18,18,17,17,
	16,16,15,15,14,14,13,13,12,12,
	11,11,10,10, 9, 9, 8, 8, 7, 7,
	 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, //5
	 1, 1, 0, 0  				   //6
};
#elif defined(_KV3930_SPECIAL_VOLUME_)
static const uint8 VolumeTab[64]=
{
	63,62,61,60,59,58,56,54,53,51,
	49,47,46,45,43,39,39,38,37,36,
	35,34,33,32,31,30,29,28,27,26,
	25,24,23,22,21,20,19,18,17,16,
	15,14,13,12,11,10,9,8,7, 7,
	6, 6, 5,5, 4, 4, 3, 3, 2, 2,
	1, 1, 0,0
};
#elif defined(_MR5258_SPECIAL_VOLUME_)
static const uint8 VolumeTab[64]=
{
	63,62,61,60,59,58,57,56,55,54,
	53,52,51,50,49,48,47,46,45,44,
	43,42,41,40,39,38,37,36,35,34,
	33,32,31,30,29,28,27,26,25,24,
	23,22,21,20,19,18,17,16,15,14,
	13,12,11,10, 9, 8, 7, 6, 5, 4,
	3, 2, 1,0
};
#elif defined(_GX_CHANGE_VOL_STEP_)
static const uint8 VolumeTab[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,39,39,38,38,37,37,36,36,35,
	35,34,32,32,31,31,30,29,29,28,
	28,27,27,26,26,25,25,24,24,23,
	21,19,17,15,14,12,11,10,10,9,8,7,6,
	5, 4, 4, 3, 3, 2, 2 ,1, 1, 0,0
};
#else
static const uint8 VolumeTab[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,37,34,31,28,26,24,23,22,21,
	21,20,20,19,19,18,18,17,17,17,
	16,16,15,15,14,14,13,13,12,12,
	11,11,10,10,9,9,8,8,7,7,6,6,5,
	5, 4, 4, 3, 3, 2, 2 ,1, 1, 0,0
};
#endif
#elif (AUDIO_VOLUME_MAX == 62)
static const uint8 VolumeTab[63]=
{
	63,55,52,49,46,44,41,39,36,34,
	31,29,27,25,24,24,23,23,22,22,
	21,21,20,20,19,19,18,18,17,17,
	16,16,15,15,14,14,13,13,12,12,
	11,11,10,10, 9, 9, 8, 8, 7, 7,
	 6, 6, 5, 5, 4, 4, 3, 3, 2, 2,
	 1, 1, 0,
};
#elif (AUDIO_VOLUME_MAX == 60)
static const uint8 VolumeTab[61]=
{
	63,56,51,46,42,39,36,33,
	31,29,27,25,24,24,23,23,22,22,
	21,21,20,20,19,19,18,18,17,17,
	16,16,15,15,14,14,13,13,12,12,
	11,11,10,10, 9, 9, 8, 8, 7, 7,
	 6, 6, 5, 5, 4, 4, 3, 3, 2, 2,
	 1, 1, 0,
};
#elif (AUDIO_VOLUME_MAX == 100)
static const uint8 VolumeTab[101]=
{
	63,62,62,61,61,60,60,59,59,58,
	58,57,57,56,56,55,55,54,54,53,
	53,52,52,51,51,50,50,49,48,47,
	46,45,44,43,42,41,40,39,38,37,
	36,35,34,33,32,31,30,29,28,27,
	26,25,24,24,23,23,22,22,21,21,
	20,20,19,19,18,18,17,17,16,16,
	15,15,14,14,13,13,12,12,11,11,
	10,10, 9, 9, 8, 8, 7, 7, 6, 6,
	 5, 5, 4, 4, 3, 3, 2, 2, 1, 1,
	0
};
#endif

#if defined(_FY_RADIO_FM_TDA778x_GATE_)
#if (AUDIO_VOLUME_MAX == 100)
static const uint8 VolumeTab1[101]=
{
	63,62,62,61,61,60,60,59,59,58,
	58,57,57,56,56,55,55,54,54,53,
	53,52,52,51,51,50,50,49,48,47,
	46,45,44,43,42,41,40,39,38,37,
	36,35,34,33,32,31,30,29,28,27,
	26,25,24,24,23,23,22,22,21,21,
	20,20,19,19,18,18,17,17,16,16,
	15,15,14,14,13,13,12,12,11,11,
	10,10, 9, 9, 8, 8, 7, 7, 6, 6,
	 5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
	2
};
#elif (AUDIO_VOLUME_MAX == 63)
#if defined(_FY7000DVD_FM_TDA778x_GATE_)
static const uint8 VolumeTab1[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,37,34,31,29,28,27,27,26,26,
	25,25,24,24,23,23,22,22,21,21,
	20,20,19,19,18,18,17,17,16,16,
	15,15,14,14,13,13,12,12,11,11,10,10,9,
	9, 8, 8, 7, 7, 6, 6 ,5, 5, 4,4
};
#else
static const uint8 VolumeTab1[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,37,34,31,28,26,25,25,24,24,
	23,23,22,22,21,21,20,20,19,19,
	18,18,17,17,16,16,15,15,14,14,
	13,13,12,12,11,11,10,10,9,9,8,8,7,
	7, 6, 6, 5, 5, 4, 4 ,3, 3, 2,2
};
#endif

#else
#if defined(_FY7000DVD_FM_TDA778x_GATE_)
static const uint8 VolumeTab1[41]=
{
63,52,47,45,44,43,42,41,39,37,36,35,34,33,32,31,29,26,25,25,23,21,
20,18,17,16,14,12,11,10,9,9,8,8,7,7,6,6,5,5,4
};
#else
static const uint8 VolumeTab1[41]=
{
63,52,47,45,44,43,42,41,39,37,36,35,34,33,32,31,29,26,25,25,23,21,
20,18,17,16,14,12,10,9,8,7,6,6,5,5,4,4,3,3,2
};
#endif
#endif
#endif

#if defined(_ZSBY5209_PLAY_OUTPUT_)
static const uint8 VolumeTab1[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
17,16,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8
};
#endif

#if defined(_XC3318_TDA778x_GATE_)
static const uint8 VolumeTab1[41]=
{
63,52,47,45,44,43,42,41,39,37,36,35,34,33,32,31,29,26,25,25,23,21,
20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,6,5,5,4
};
#endif

#if defined(_SL_PLAYER_GATE_)
static const uint8 VolumeTab1[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,20,19,19,
18,18,17,16,15,14,13,12,11,10,9,9,8,8,7,7,6,6,5
};
#endif

#if defined(_SL6803_PLAYER_GATE_)
static const uint8 VolumeTab1[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,24,23,22,21,20,19,19,
18,18,17,16,15,14,13,12,11,10,9,9,8,8,7,6,5,4,3
};
#endif

#if defined(_TUNER_DRIVER_SI474x_)
static const uint8 VolumeTab2[41]=
{
63,52,47,42,35,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,
16,15,14,13,12,11,10,9,8,7,6,5,4,4,3,3,2,2,1
};
#endif

#if defined(_HKY1032_RADIO_OUTPUT_)
static const uint8 VolumeTab4[41]=
{
63,52,47,43,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,
23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5
};
#endif

#if defined(_HKY1033_PLAY_OUTPUT_)
static const uint8 VolumeTab1[64]=
{
	63,61,59,57,56,54,51,48,45,43,
	40,37,34,31,30,30,29,29,28,28,
	27,27,26,26,25,25,24,24,23,23,
	23,22,22,21,21,21,20,20,19,19,
	18,18,17,17,16,16,15,15,14,14, 
	13,13,12,12,11,11,10,10, 9, 9,
	8, 8,7,7
};
#endif

#if (AUDIO_ATTEN_WIDE == 10)
static const uint8 PhTabe[]=
{
	0,		/*0*/
	3,		/*1*/
	6,		/*2*/
	9,		/*3*/
	12,		/*4*/
	15,		/*5*/
	18,		/*6*/
	21,		/*7*/
	24,		/*8*/
	27,		/*9*/
	31,		/*10*/
};
#elif (AUDIO_ATTEN_WIDE == 7)
static const uint8 PhTabe[]=
{
	0,		/*0*/
	4,		/*1*/
	8,		/*2*/
	12,		/*3*/
	16,		/*4*/
	21,		/*5*/
	26,		/*6*/
	31,		/*7*/
};
#elif defined(_SPECIAL_PT2313_SET_)
static const uint8 PhTabe[]=
{
	0,		/*0*/
	3,		/*1*/
	6,		/*2*/
	9,		/*3*/
	12, 	/*4*/
	15, 	/*5*/
	18, 	/*6*/
	21, 	/*7*/
	23, 	/*8*/
	25, 	/*9*/
	27, 	/*10*/
	29, 	/*11*/
	31, 	/*12*/
};
#endif

#if defined(_SPECIAL_TL2000_SET_)
#if defined(AUDIO_VOLUME_MAX_30_)
static const uint8 FR_VOL_TAB[]=
{
	31,30,29,28,27,26,25,24,23,22,
	21,20,19,18,17,16,15,14,13,12,
	11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
};
#else
static const uint8 FR_VOL_TAB[]=
{
	31,30,29,28,27,26,25,24,23,
	22,21,21,20,20,19,19,18,18,
	17,17,16,16,15,15,14,14,13,13,
	12,12,11,11,10,10,9,9,8,8,7,7,
	6,6, 5, 5, 4, 4, 3, 3, 2, 2, 1,
};
#endif
#endif

#if defined(_SPECIAL_PT2313_SET_)
static const uint8 ToneTabe[]=
{
	 0, 1, 2,2, 3,3, 4, 4,5, 5,6,6,
	15,
	14,14,13,13,12,12,11,11,10,10,9,8
};
#elif defined(_JYL3802E_BAS_TRE_)
static const uint8 ToneTabe[]=
{
	 3, 3, 4, 4, 5, 5, 6,
	15,
	14,13,13,12,12, 11, 11
};
#elif defined(_JYL_BAS_TRE_)
static const uint8 ToneTabe[]=
{
	 2, 2, 3, 3, 4, 5, 6,
	15,
	14,13,12,11,11, 10, 10
};
#else
static const uint8 ToneTabe[]=
{
	 0, 1, 2, 3, 4, 5, 6,
	15,
	14,13,12,11,10, 9, 8
};
#endif

#if defined(_JL8253_BAS_FUNTION_)
static const uint8 TreTabe[]=
{
	 0, 1, 2, 3, 4, 5, 6,
	13,
	12,11,10,10,9, 9, 8
};
#endif

#if defined(_JYL_TRE_FUNTION)
static const uint8 TreTabe[]=
{
	 3, 4, 4, 5, 5, 6, 6,
	15,
	14,14,13,13,12, 12, 11
};
#endif

#if defined(_SLA6802_EQ_FUNTION_)
static const int8 EqTabe[][2]=
{
	{0,0}, 		 //	EQ_FLAT
	{-2,-1},	 //	EQ_CLASSIC
	{2,1},	    //	EQ_POP
	{3,1},		//	EQ_ROCK
	{0,2}		//	EQ_JAZZ
};
#elif defined(_SLA6803_EQ_FUNTION_)
static const int8 EqTabe[][2]=
{
	{0,0}, 		 //	EQ_FLAT
	{1,-1},	    //	EQ_CLASSIC
	{4,0},	    //	EQ_POP
	{3,1},		//	EQ_ROCK
	{0,2}		//	EQ_JAZZ
};
#elif defined(_XM007_EQ_FUNTION_)
static const int8 EqTabe[][2]=
{
    {-2,-2},         //    EQ_FLAT
    {3,3},        	//    EQ_CLASSIC
    {-6,-3},    	//    EQ_POP
    {5,2},        	//    EQ_ROCK
    {1,4}        	//    EQ_JAZZ	
};
#elif defined(_SLA6802_SPECIAL_EQ_)
static const int8 EqTabe[][2]=
{
	{0,0}, 		 //	EQ_FLAT
	{3,-1},	    //	EQ_CLASSIC
	{4,0},	    //	EQ_POP
	{3,1},		//	EQ_ROCK
	{0,2}		//	EQ_JAZZ
};
#elif defined(_TY2200_EQ_FUNTION_)
static const int8 EqTabe[][2]=
{
	{5,-1},   //	EQ_FLAT
	{5,-3},	 //	EQ_CLASSIC
	{6,-2},	    //	EQ_POP
	{5,3},		//	EQ_ROCK
	{1,4}		//	EQ_JAZZ
};
#else
static const int8 EqTabe[][2]=
{
	{0,0}, 		//	EQ_FLAT
	{1,1},		//	EQ_CLASSIC
	{-2,-1},	//	EQ_POP
	{3,1},		//	EQ_ROCK
	{0,2}		//	EQ_JAZZ
};
#endif
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
static uint8 pt2313_audio_init;
#endif

#if defined(_ENABLE_AMP_LIGHT_)
static void audio_mute_ctrl(uint8 mute)
{
	AUDIO_MUTE_INIT();
#if defined(_ENABLE_RCA_MUTE_)
	RCA_MUTE_INIT();
#endif
	if(mute)
	{
	#if defined(_MUTE_ON_CLOSE_AMP_)
    	if(pt2313_audio_init)
    	{
    		pt2313_audio_init = 0;
    	}
    	else
    	{
    		AMP_CONTROL_OFF();
    	}
    	OSTimeDly(16);
	#else
		if(global.amp_power)
		{
			if(pt2313_audio_init)
			{
				pt2313_audio_init = 0;
			}
			else
			{
				AMP_CONTROL_OFF();
			}
			OSTimeDly(16);
			global.amp_power = FALSE;
		}
	#endif
    	AUDIO_MUTE_ON();
    #if defined(_ENABLE_RCA_MUTE_)
		if(setup.aud_vol != 0)
		{
			RCA_MUTE_ON();
		}
    #endif
    #if defined(_SUPPORT_SUBWOOFER2_)&&0
    	if(setup.aud_sub)
    	{
    		SYS_SUB_OFF();
    	}
    	if(setup.aud_sub1)
    	{
    		SYS_SUB2_OFF();
    	}
    #endif
	}
	else
	{
		AUDIO_MUTE_OFF();
	#if defined(_ENABLE_RCA_MUTE_)
		RCA_MUTE_OFF();
	#endif
	#if defined(_MUTE_ON_CLOSE_AMP_)
    	OSTimeDly(5);
    	AMP_CONTROL_ON();
	#else
		if(global.amp_power)
		{
			OSTimeDly(16);
			AMP_CONTROL_ON();
			global.amp_power = FALSE;
		}
	#endif
    #if defined(_SUPPORT_SUBWOOFER2_)&&0
    	if(setup.aud_sub)
    	{
    		SYS_SUB_ON();
    	}
    	if(setup.aud_sub1)
    	{
    		SYS_SUB2_ON();
    	}
    #endif
	}
}

#else
static void audio_mute_ctrl(uint8 mute)
{
	AUDIO_MUTE_INIT();
	if(mute)
	{
	#if defined(_BT_SPECIAL_MUTE_)
	if(global.bt_mute_flag)
		{
			global.bt_mute_flag = FALSE;
		}
		else
		{
			AUDIO_MUTE_ON();
		}
	#else
		#if defined(_MR889_SPECAIL_KEY_)
			if(setup.CONTROL_SPK)
			{
				AUDIO_MUTE_OUT();
				AUDIO_MUTE_HIGH();
			}
			else
			{
				AUDIO_MUTE_ON();
			}
		#else
        #if defined(_SUPPORT_AMP_CONTROL_)
			if(pt2313_audio_init)
			{
				pt2313_audio_init = 0;
			}
			else
			{
        		AMP_CONTROL_OFF();
			}
			if(global.number_flag)
			{
				global.number_flag = 0 ;
			}
			else
			{
				OSTimeDly(16);
			}
        	AUDIO_MUTE_ON();
		#if defined(_SUPPORT_SUBWOOFER2_)
        	if(setup.aud_sub)
        	{
        		SYS_SUB_OFF();
        	}
        	if(setup.aud_sub1)
        	{
        		SYS_SUB2_OFF();
        	}
		#endif
		#elif defined(_ENABLE_BEEP_)
		if(setup.aud_beep)
		{
			if(global.beep_time)
			{
				AUDIO_MUTE_OFF();
			#if defined(SET_BEEP_FUNCV)
				global.beep_mute_flag = TRUE;
			#endif
			}
			else
			{
				AUDIO_MUTE_ON();
			}
		}
		else
		{
			AUDIO_MUTE_ON();
		}
        #else
			AUDIO_MUTE_ON();
		#endif
	#endif
	#endif
	}
	else
	{
	#if defined(_MR889_SPECAIL_KEY_)
		if(!setup.CONTROL_SPK)
		{
			AUDIO_MUTE_OUT();
			AUDIO_MUTE_LOW();
		}
		else
		{
			AUDIO_MUTE_OFF();
		}
	#else
		AUDIO_MUTE_OFF();
    #if defined(_SUPPORT_AMP_CONTROL_)
	#if defined(_SUPPORT_SUBWOOFER2_)
    	if(setup.aud_sub)
    	{
    		SYS_SUB_ON();
    	}
    	if(setup.aud_sub1)
    	{
    		SYS_SUB2_ON();
    	}
	#endif
		if(global.number_flag)
		{
			global.number_flag = 0;
		}
		else
		{
			OSTimeDly(16);
		}
    	AMP_CONTROL_ON();
    #endif
	#endif
	}
}
#endif

#if defined(_SUPPORT_SUBWOOFER_)
static void audio_subwoofer_ctrl(uint8 subwoofer)
{
	if(pt2313_audio.subwoofer_last != subwoofer)
	{
		AUDIO_SUB_INIT;
		if(subwoofer)
		{
			AUDIO_SUB_ON;
		}
		else
		{
			AUDIO_SUB_OFF;
		}
		pt2313_audio.subwoofer_last = subwoofer;
	}
}
#endif

static uint8 audio_switch_value(void)
{
	uint8 value;

	value = ((uint8)pt2313_audio.channel)&0x03;//低两位为2313通道
	if(pt2313_audio.loud == AUDIO_LOUDNESS_OFF)
		value|= _B2_;//B2为LOUD开关
	value |= (((uint8)pt2313_audio.gate)<<3);//bit3/bit4为增益 00:11.25

	return value;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

void audio_reg_init(void)
{
	memset(&pt2313_audio, 0, sizeof(pt2313_audio));

	pt2313_audio.channel 	= 0;
	pt2313_audio.loud 		= AUDIO_LOUDNESS_OFF;
#if defined(_SUPPORT_SUBWOOFER_)
	pt2313_audio.subwoofer	= AUDIO_SUBWOOFER_OFF;
#endif
	pt2313_audio.gate 		= AUDIO_GATE_PLUS_11_25;
	pt2313_audio.volume 	= AUDIO_VOLUME_DEFAULT;
#if defined(_SPECIAL_TL2000_SET_)
	pt2313_audio.A_volume 	= AUDIO_A_VOLUME_DEFAULT;
	pt2313_audio.B_volume 	= AUDIO_B_VOLUME_DEFAULT;
#endif
	pt2313_audio.mute_last 	= pt2313_audio.mute = ENABLE;

#if defined(_SUPPORT_AMP_CONTROL_)||defined(_ENABLE_AMP_LIGHT_)
	pt2313_audio_init = TRUE;
#endif
	audio_mute_ctrl(ENABLE);
}


void audio_reg_set(void)
{
	uint8  mute_flag;

#if defined(_SPECIAL_TL2000_SET_)
	uint32 a;
	uint32 b;
	uint32 c;
	char lr;
	uint8 TempL_Vol;
	uint8 TempR_Vol;
#endif

#if defined(_SPECIAL_2313_CONTROL_)
	uint8  i;
#endif

#if defined(_SUPPORT_SUBWOOFER_)
	uint8  subwoofer;
#endif
	uint8 index;
	int8  audio_reg[PT2313_REG_SIZE];

#if defined(_ENABLE_POWER_OFF_I2C_LOW_)
	if(compare_task_name("IdleTask")) return;
#endif

#if defined(_SUPPORT_TWO_PT2313_)
	if((pt2313_audio.mute)||((!setup.aud_vol2)&&(!setup.aud_vol)))
#else
	if((pt2313_audio.mute)||(!pt2313_audio.volume))
#endif
	{
		mute_flag = ENABLE;
	}
	else
	{
		mute_flag = DISABLE;
	}

#if defined(_SUPPORT_SUBWOOFER_)
	if(mute_flag ||(pt2313_audio.subwoofer == AUDIO_SUBWOOFER_OFF))
	{
		subwoofer = OFF;
	}
	else
	{
		subwoofer = ON;
	}
#endif

	if(pt2313_audio.mute_last != mute_flag)
	{
		if(mute_flag)
		{
			audio_mute_ctrl(TRUE);
			OSTimeDly(2);

		#if defined(_SUPPORT_SUBWOOFER_)
			audio_subwoofer_ctrl(subwoofer);
		#endif
		}
	}

	memset(audio_reg, 0, sizeof(audio_reg));

#if defined(_ENABLE_RCA_MUTE_)
	if((pt2313_audio.volume != 0)&&(mute_flag))
	{
		audio_reg[PT2313_REG_VOLUME] = VolumeTab[0];
		audio_reg[PT2313_REG_LR] = (int8)(0xC0|0x1F);
		audio_reg[PT2313_REG_RR] = (int8)(0xE0|0x1F);
		audio_reg[PT2313_REG_LF] = (int8)(0x80|0x1F);
		audio_reg[PT2313_REG_RF] = (int8)(0xA0|0x1F);
		audio_reg[PT2313_REG_SW] = (int8)(0x5C|audio_switch_value());
	}
	else
#else
	if(mute_flag)
	{
		audio_reg[PT2313_REG_VOLUME] = VolumeTab[0];
		audio_reg[PT2313_REG_LR] = (int8)(0xC0|0x1F);
		audio_reg[PT2313_REG_RR] = (int8)(0xE0|0x1F);
		audio_reg[PT2313_REG_LF] = (int8)(0x80|0x1F);
		audio_reg[PT2313_REG_RF] = (int8)(0xA0|0x1F);
		audio_reg[PT2313_REG_SW] = (int8)(0x5C|audio_switch_value());
	}
	else
#endif
	{
	#if defined(_SPECIAL_TL2000_SET_)
		audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
		/*---------------------------------------------*/
		lr = pt2313_audio.lr;

		b = ((pt2313_audio.B_volume)*AUDIO_VOLUME_MAX)/AUDIO_VOLUME_MAX;
		c = (AUDIO_VOLUME_MAX*10);
		if(pt2313_audio.lr>0)
		{
			a = ((pt2313_audio.B_volume)*AUDIO_VOLUME_MAX*(10 - lr));

			TempR_Vol = b;
			TempL_Vol = (a/c);
		}
		else if(pt2313_audio.lr<0)
		{
			lr = -lr;
			a = ((pt2313_audio.B_volume)*AUDIO_VOLUME_MAX*(10 - lr));
			TempL_Vol = b;
			TempR_Vol = (a/c);
		}
		else
		{
			TempL_Vol = TempR_Vol = pt2313_audio.B_volume;//VolumeTab[pt2313_audio.volume];
		}

		if(pt2313_audio.B_volume==0)
		{
			audio_reg[PT2313_REG_LR] = (int8)(0xC0|0x1F);
			audio_reg[PT2313_REG_RR] = (int8)(0xE0|0x1F);
		}
		else
		{
			audio_reg[PT2313_REG_LR] = FR_VOL_TAB[TempL_Vol];
			audio_reg[PT2313_REG_RR] = FR_VOL_TAB[TempR_Vol];
		}
		lr = pt2313_audio.lr;

		b = ((pt2313_audio.A_volume)*AUDIO_VOLUME_MAX)/AUDIO_VOLUME_MAX;
		c = (AUDIO_VOLUME_MAX*10);
		if(pt2313_audio.lr>0)
		{
			a = ((pt2313_audio.A_volume)*AUDIO_VOLUME_MAX*(10 - lr));

			TempR_Vol = b;
			TempL_Vol = (a/c);
		}
		else if(pt2313_audio.lr<0)
		{
			lr = -lr;
			a = ((pt2313_audio.A_volume)*AUDIO_VOLUME_MAX*(10 - lr));
			TempL_Vol = b;
			TempR_Vol = (a/c);
		}
		else
		{
			TempL_Vol = TempR_Vol = pt2313_audio.A_volume;//VolumeTab[pt2313_audio.volume];
		}
		if(pt2313_audio.A_volume==0)
		{
			audio_reg[PT2313_REG_LF] = (int8)(0x80|0x1F);
			audio_reg[PT2313_REG_RF] = (int8)(0xA0|0x1F);
		}
		else
		{
			audio_reg[PT2313_REG_LF] = FR_VOL_TAB[TempL_Vol];
			audio_reg[PT2313_REG_RF] = FR_VOL_TAB[TempR_Vol];
		}
	#else
	#if defined(_FY_RADIO_FM_TDA778x_GATE_)||defined(_XC3318_TDA778x_GATE_)
		if((compare_task_name("FMRadioTask"))&&(global.tuner_drive == TUN_DRV_TDA778x))
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab1[pt2313_audio.volume];
		}
		else
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
		}
	#elif defined(_SL_PLAYER_GATE_)||defined(_SL6803_PLAYER_GATE_)
		if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard")))
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab1[pt2313_audio.volume];
		}
		else
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
		}
	#elif defined(_TUNER_DRIVER_SI474x_)
    	if((compare_task_name("FMRadioTask"))&&(global.tuner_drive == TUN_DRV_SI473x))
    	{
    		audio_reg[PT2313_REG_VOLUME] = VolumeTab2[pt2313_audio.volume];
    	}
    	else
    	{
    		audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
    	}
	#elif defined(_HKY1032_RADIO_OUTPUT_)
		if(compare_task_name("FMRadioTask"))
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab4[pt2313_audio.volume];
		}
		else
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
		}
	#elif defined(_HKY1033_PLAY_OUTPUT_)
	    if((compare_task_name("MusicTaskUsb"))||(compare_task_name("MusicTaskCard"))||(compare_task_name("BtUserTask")))
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab1[pt2313_audio.volume];
		}
		else
		{
			audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
		}
	#elif defined(_ZSBY5209_PLAY_OUTPUT_)
      	if(compare_task_name("FMRadioTask"))
      	{
      		audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
      	}
      	else
      	{
      		audio_reg[PT2313_REG_VOLUME] = VolumeTab1[pt2313_audio.volume];
      	}
	#else
		audio_reg[PT2313_REG_VOLUME] = VolumeTab[pt2313_audio.volume];
	#endif
	
		//printf("PT2313_REG_VOLUME:%d\n",audio_reg[PT2313_REG_VOLUME]);
		//printf("pt2313_audio.volume:%d\n",pt2313_audio.volume);
		//printf("dac_ctl.sys_vol_l:%d\n",dac_ctl.sys_vol_l);
		
		/*---------------------------------------------*/
		if(pt2313_audio.lr > 0)
		{
			audio_reg[PT2313_REG_LR] =
			audio_reg[PT2313_REG_LF] = (uint8)(pt2313_audio.lr);
		}
		else
		{
			audio_reg[PT2313_REG_RR] =
			audio_reg[PT2313_REG_RF] = (uint8)(-pt2313_audio.lr);
		}
		
	#if defined(_XM007_SPECIAL_FAD_)
		if(pt2313_audio.fr > 0)
		{
			audio_reg[PT2313_REG_LF] += (uint8)(pt2313_audio.fr);
			audio_reg[PT2313_REG_RF] += (uint8)(pt2313_audio.fr);
		}
		else
		{
			audio_reg[PT2313_REG_LR] += (uint8)(-pt2313_audio.fr);
			audio_reg[PT2313_REG_RR] += (uint8)(-pt2313_audio.fr);
		}
	#else
		if(pt2313_audio.fr > 0)
		{
			audio_reg[PT2313_REG_LR] += (uint8)(pt2313_audio.fr);
			audio_reg[PT2313_REG_RR] += (uint8)(pt2313_audio.fr);
		}
		else
		{
			audio_reg[PT2313_REG_LF] += (uint8)(-pt2313_audio.fr);
			audio_reg[PT2313_REG_RF] += (uint8)(-pt2313_audio.fr);
		}
	#endif

		for(index = PT2313_REG_LR; index < (PT2313_REG_LR + 4); index ++)
		{
			if(audio_reg[index] > AUDIO_ATTEN_WIDE)
				audio_reg[index] = AUDIO_ATTEN_WIDE;

			audio_reg[index] = PhTabe[ audio_reg[index] ];
		}
	#endif

		/*---------------------------------------------*/
		audio_reg[PT2313_REG_LR] |= 0xC0;
		audio_reg[PT2313_REG_RR] |= 0xE0;
		audio_reg[PT2313_REG_LF] |= 0x80;
		audio_reg[PT2313_REG_RF] |= 0xA0;
		audio_reg[PT2313_REG_SW]  = 0x40|audio_switch_value();
		/*---------------------------------------------*/
		if(pt2313_audio.eq == AUDIO_EQ_OFF)
		{
			audio_reg[PT2313_REG_BASS] = pt2313_audio.bass;
			audio_reg[PT2313_REG_TREB] = pt2313_audio.treble;
		}
		else
		{
			audio_reg[PT2313_REG_BASS] = EqTabe[pt2313_audio.eq - 1][0];
			audio_reg[PT2313_REG_TREB] = EqTabe[pt2313_audio.eq - 1][1];
		}
	#if defined(_SPECIAL_PT2313_SET_)
    	audio_reg[PT2313_REG_BASS] = ToneTabe[audio_reg[PT2313_REG_BASS]+12];
    	audio_reg[PT2313_REG_TREB] = ToneTabe[audio_reg[PT2313_REG_TREB]+12];
	#elif defined(_JL8253_BAS_FUNTION_)
    	audio_reg[PT2313_REG_BASS] = ToneTabe[audio_reg[PT2313_REG_BASS]+7];
    	audio_reg[PT2313_REG_TREB] = TreTabe[audio_reg[PT2313_REG_TREB]+7];
	#elif defined(_JYL_TRE_FUNTION)
    	audio_reg[PT2313_REG_BASS] = ToneTabe[audio_reg[PT2313_REG_BASS]+7];
    	audio_reg[PT2313_REG_TREB] = TreTabe[audio_reg[PT2313_REG_TREB]+7];
	#else
		audio_reg[PT2313_REG_BASS] = ToneTabe[audio_reg[PT2313_REG_BASS]+7];
		audio_reg[PT2313_REG_TREB] = ToneTabe[audio_reg[PT2313_REG_TREB]+7];
	#endif
	}

	audio_reg[PT2313_REG_BASS] |= 0x60;
	audio_reg[PT2313_REG_TREB] |= 0x70;

#if defined(_SPECIAL_2313_CONTROL_)
	for(i=0;i<8;i++)
	{
		if(pt2313_audio.reg[i] != audio_reg[i])
		{
			pt2313_audio.reg[i] = audio_reg[i];
			i2c_data_init(PT2313_I2C_ADDR, PT2313_I2C_RATE, &audio_gpio);
			i2c_data_w1((uint8 *)(&pt2313_audio.reg[i]), 1);
			i2c_data_exit();
		}
	}
#else
    i2c_data_init(PT2313_I2C_ADDR, PT2313_I2C_RATE, &audio_gpio);
    i2c_data_w((uint8 *)audio_reg, sizeof(audio_reg));
    i2c_data_exit();
#endif

	if(pt2313_audio.mute_last != mute_flag)
	{
		if(!mute_flag)
		{
			audio_mute_ctrl(FALSE);
		}
	}

	pt2313_audio.mute_last = mute_flag;

#if defined(_SUPPORT_SUBWOOFER_)
	audio_subwoofer_ctrl(subwoofer);
#endif
}

uint8 audio_reg_ctrl(uint8 ctrl, int para)
{
	switch(ctrl)
	{
		/*!设置静音*/
		case AUD_CTRL_MUTE:
			{
				uint8 mute = (uint8)para;
				pt2313_audio.mute = mute;
			}
			break;
		/*!设置通道*/
		case AUD_CTRL_CHANNEL:
			{
				uint8 channel = (uint8)para;
				if(channel < AUDIO_CHANNEL_MAX)
				{
				#if defined(_SUPPORT_DISC_)
					switch(channel)
					{
						case AUDIO_CHANNEL_RADIO_ON:
						case AUDIO_CHANNEL_RADIO_OFF:
							if(fm_inside_check())
							{
								pt2313_audio.channel = 2;
							}
							else
							{
								pt2313_audio.channel = 0;
							}
							break;

						case AUDIO_CHANNEL_MUSIC_ON:
						case AUDIO_CHANNEL_MUSIC_OFF:
						case AUDIO_CHANNEL_AUX_ON:
						case AUDIO_CHANNEL_AUX_OFF:
							pt2313_audio.channel = 2;
							break;
                #if defined(_SUPPORT_AUX1_)
   						case AUDIO_CHANNEL_AUX1_ON:
   						case AUDIO_CHANNEL_AUX1_OFF:
   							pt2313_audio.channel = 3;
   							break;
                #endif   
						case AUDIO_CHANNEL_DISC_ON:
						case AUDIO_CHANNEL_DISC_OFF:
							pt2313_audio.channel = 1;
							break;
					}
				#else
					switch(channel)
					{
						case AUDIO_CHANNEL_RADIO_ON:
						case AUDIO_CHANNEL_RADIO_OFF:
							if(fm_inside_check())
							{
								pt2313_audio.channel = 1;
							}
							else
							{
							#if defined(_KD_ENABLE_PT2313_CHANNEL_)
								pt2313_audio.channel = 0;
							#elif defined(_ENABLE_RECORD_RADIO_)&&0
								pt2313_audio.channel = 1;
							#elif defined(_ENABLE_PT2313_CHANNEL_)
								pt2313_audio.channel = 1;
							#elif defined(_SUPPORT_TWO_PT2313_)
								pt2313_audio.channel = 2;
							#else
								pt2313_audio.channel = 0;
							#endif
							}
							break;

						case AUDIO_CHANNEL_MUSIC_ON:
						case AUDIO_CHANNEL_MUSIC_OFF:
					#if defined(_ENABLE_DAB_VOLUME_)
    					case AUDIO_CHANNEL_DAB_ON:
    					case AUDIO_CHANNEL_DAB_OFF:
					#endif
					#if defined(_SUPPORT_TAPE_)
    					case AUDIO_CHANNEL_TAPE_ON:
    					case AUDIO_CHANNEL_TAPE_OFF:
					#endif
					#if defined(_SUPPORT_MIC_OK_)
    					case AUDIO_CHANNEL_MIC_ON:
    					case AUDIO_CHANNEL_MIC_OFF:
					#endif
						#if defined(_ENABLE_PT2313_CHANNEL_)
							ew_puts("AUDIO_CHANNEL_MUSIC_ON\n");
							pt2313_audio.channel = 0;
						#else
							pt2313_audio.channel = 1;
						#endif
							break;

						case AUDIO_CHANNEL_AUX_ON:
						case AUDIO_CHANNEL_AUX_OFF:
						#if defined(ENABLE_DAB_PT2313_CHANNEL)||defined(_AUX_INSIDE_MCU_)
							pt2313_audio.channel = 1;
						#elif defined(_SUPPORT_TWO_PT2313_)
							pt2313_audio.channel = 0;
						#else
					    	pt2313_audio.channel = 2;
						#endif
							break;
					#if defined(ENABLE_PT2312_CHANNEL)
						case AUDIO_CHANNEL_DAB_ON:
						case AUDIO_CHANNEL_DAB_OFF:
					    	pt2313_audio.channel = 3;
							break;
					#elif defined(ENABLE_DAB_PT2313_CHANNEL)
    					case AUDIO_CHANNEL_DAB_ON:
    					case AUDIO_CHANNEL_DAB_OFF:
    						pt2313_audio.channel = 2;
    						break;
					#elif defined(ENABLE_PT2313_CHANNEL)
						case AUDIO_CHANNEL_DAB_ON:
    					case AUDIO_CHANNEL_DAB_OFF:
    						pt2313_audio.channel = 0;
    						break;
					#endif
					}
				#endif
				}
			}
			break;
		/*!设置放大*/
		case AUD_CTRL_GATE:
			{
				uint8 gate = (uint8)para;
				if(gate < AUDIO_GATE_PLUS_MAX)
				{
					pt2313_audio.gate = gate;
				}
			}
			break;
		/*!设置响度*/
		case AUD_CTRL_LOUD:
			{
				uint8 loud = (uint8)para;
				if(loud < AUDIO_LOUDNESS_MAX)
				{
					pt2313_audio.loud = loud;
				}
			}
			break;
		/*!设置音效*/
		case AUD_CTRL_EQ:
			{
				uint8 eq = (uint8)para;
				if(eq < AUDIO_EQ_MAX)
				{
					pt2313_audio.eq = eq;
				}
			}
			break;
		/*!设置音量*/
		case AUD_CTRL_VOLUME:
			{
				uint8 volume = (uint8)para;
				if(volume > AUDIO_VOLUME_MAX)
				{
					pt2313_audio.volume = AUDIO_VOLUME_MAX;
				}
				else
				{
					pt2313_audio.volume = volume;
				}
			}
			break;
		/*!设置低音*/
		case AUD_CTRL_BASS:
			{
				int8 bass = (int8)para;
				if(bass > AUDIO_BASS_MAX)
				{
					pt2313_audio.bass = AUDIO_BASS_MAX;
				}
				else if(bass < AUDIO_BASS_MIN)
				{
					pt2313_audio.bass = AUDIO_BASS_MIN;
				}
				else
				{
					pt2313_audio.bass = bass;
				}
			}
			break;
		/*!设置高音*/
		case AUD_CTRL_TREBLE:
			{
				int8 treble = (int8)para;
				if(treble > AUDIO_TREBLE_MAX)
				{
					pt2313_audio.treble = AUDIO_TREBLE_MAX;
				}
				else if(treble < AUDIO_TREBLE_MIN)
				{
					pt2313_audio.treble = AUDIO_TREBLE_MIN;
				}
				else
				{
					pt2313_audio.treble = treble;
				}
			}
			break;
		/*!设置左右平衡*/
		case AUD_CTRL_LR:
			{
				int8 lr = (int8)para;
				if(lr > AUDIO_LR_MAX)
				{
					pt2313_audio.lr = AUDIO_LR_MAX;
				}
				else if(lr < AUDIO_LR_MIN)
				{
					pt2313_audio.lr = AUDIO_LR_MIN;
				}
				else
				{
					pt2313_audio.lr = lr;
				}
			}
			break;
		/*!设置前后平衡*/
		case AUD_CTRL_FR:
			{
				int8 fr = (int8)para;
				if(fr > AUDIO_FR_MAX)
				{
					pt2313_audio.fr = AUDIO_FR_MAX;
				}
				else if(fr < AUDIO_FR_MIN)
				{
					pt2313_audio.fr = AUDIO_FR_MIN;
				}
				else
				{
					pt2313_audio.fr = fr;
				}
			}
			break;
	#if defined(_SUPPORT_SUBWOOFER_)
		/*!设置重低音*/
		case AUD_CTRL_SUBWOOFER:
			{
				uint8 subwoofer = (uint8)para;
				if(subwoofer < AUDIO_SUBWOOFER_MAX)
				{
					pt2313_audio.subwoofer = subwoofer;
				}
			}
			break;
	#endif
		/*!得到当前通道*/
		case AUD_GET_CHANNEL:
			*((uint8 *)para) = pt2313_audio.channel;
			return FALSE;
	#if defined(_SPECIAL_TL2000_SET_)
		/*!设置音量*/
		case AUD_CTRL_A_VOLUME:
			{
				uint8 volume = (uint8)para;
				if(volume > AUDIO_A_VOLUME_MAX)
				{
					pt2313_audio.A_volume = AUDIO_A_VOLUME_MAX;
				}
				else
				{
					pt2313_audio.A_volume = volume;
				}
			}
			break;
		/*!设置音量*/
		case AUD_CTRL_B_VOLUME:
			{
				uint8 volume = (uint8)para;
				if(volume > AUDIO_B_VOLUME_MAX)
				{
					pt2313_audio.B_volume = AUDIO_B_VOLUME_MAX;
				}
				else
				{
					pt2313_audio.B_volume = volume;
				}
			}
			break;
	#endif
		default:
			return FALSE;
	}

	return TRUE;
}

#ifdef	TD_ADD_NEW_APP
extern void Spp_Tx_Audio_ble(void);
void pt2313_reset(void)
{
	setup.aud_loud		= AUDIO_LOUDNESS_OFF;
	setup.aud_eq		= AUDIO_EQ_OFF;
	setup.aud_bass		= AUDIO_BASS_DEFAULT;
	setup.aud_treble	= AUDIO_TREBLE_DEFAULT;
	setup.aud_bal		= AUDIO_LR_DEFAULT;
	setup.aud_fad		= AUDIO_FR_DEFAULT;
	audio_drv_ctrl(AUD_CTRL_BASS, 	setup.aud_bass, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_TREBLE, setup.aud_treble, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_LR, 	setup.aud_bal,		FALSE);
	audio_drv_ctrl(AUD_CTRL_FR, 	setup.aud_fad,		FALSE);
	audio_drv_ctrl(AUD_CTRL_LOUD, 	setup.aud_loud, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_EQ, 	setup.aud_eq, 		FALSE);
    ui_show_clean();
    audio_reg_ctrl(AUD_CTRL_EQ,setup.aud_eq);
    audio_reg_set();
	Spp_Tx_Audio_ble();
}
#endif

