#ifndef __DAC_PARAM_H__
#define __DAC_PARAM_H__

#include "typedef.h"

//=============================================================
//
//					LADC PARAM DEFINE
//
//=============================================================

///ladc samplerate define
#define LADC_SR44100    44100
#define LADC_SR48000   	48000 
#define LADC_SR32000  	32000  
#define LADC_SR22050    22050
#define LADC_SR24000    24000
#define LADC_SR16000    16000
#define LADC_SR11025    11025
#define LADC_SR12000    12000
#define LADC_SR8000     8000 

///ladc encode channel define

//ldac_channel
#define ENC_LINE_LEFT_CHANNEL       0      //linein???
#define ENC_LINE_RIGHT_CHANNEL      1      //linein???
#define ENC_MIC_CHANNEL             2
#define ENC_LINE_LR_CHANNEL         3      //linein????
#define ENC_DAC_LEFT_CHANNEL		4	   //dac left channel
#define ENC_DAC_RIGHT_CHANNEL		5	   //dac right channel

//other channel
#define ENC_FM_CHANNEL       		6      //FM??
#define ENC_BT_CHANNEL       		7      //??
#define ENC_USER_CHANNEL     		8      //?????

//=============================================================
//
//					DAC PARAM DEFINE
//
//=============================================================
enum
{
    SR_441K = 0x0,
    SR_48K,
    SR_32K,
    SR_2205K = 0x4,
    SR_24K,
    SR_16K,
    SR_11025K = 0x8,
    SR_12K = 0x9,
    SR_8K = 0xA,
};

enum
{
    DAC_NO_ERR                     =0,
    DAC_DRV_ISRFUN_RDEF_ERR        =-1000,
};

//?????????0x00ff,(channel_mode & 0xFF00)
enum
{
    DAC_DIGITAL_DVOL = 1,       ///<????,?????????
    DAC_DIGITAL_AVOL,           ///<????,?????????
    DAC_DIGITAL_D_A_VOL,        ///<????,??????????????????
    DAC_AMUX0,                 
    DAC_AMUX1,                
    DAC_AMUX2,                  
	DAC_AMUX0_L_ONLY ,          
	DAC_AMUX0_R_ONLY ,          
	DAC_AMUX1_L_ONLY ,          
	DAC_AMUX1_R_ONLY ,          
	DAC_AMUX2_L_ONLY ,		    
	DAC_AMUX2_R_ONLY ,          
	DAC_AMUX_DAC_L,             ///<DAC_L input,DAC_R output
	DAC_AMUX_DAC_R,             ///<DAC_R input,DAC_L output
    LINE_ONE_IN_TWO_OUT,        ///
    LINE_TWO_IN_ONE_OUT,        ///
};

//BIT (channel_mode & 0xFF00)
enum
{
    DAC_SET_AVOL_BIT = 8,       ///<??????,??DAC_DIGITAL_D_A_VOL??:DAC_DIGITAL_D_A_VOL|BIT(DAC_SET_AVOL_BIT)
    DAC_SET_DVOL_BIT ,       ///<??????,??DAC_DIGITAL_D_A_VOL??:DAC_DIGITAL_D_A_VOL|BIT(DAC_SET_AVOL_BIT)
};

#define LDO_1   1 //LDO1
#define LDO_2   2 //LDO2

#define DAC_SAMPLE_CON      (0)
#define DAC_SAMPLE_POINT    (32 * (1 << DAC_SAMPLE_CON))
#define DAC_BUF_LEN         (DAC_SAMPLE_POINT*2*2)
#define DAC_DUAL_BUF_LEN    (DAC_SAMPLE_POINT * 2)

#define OUTPUT_BUF_SIZE  (576*2*2*3)      ///dac_cbuf size

#define DAC_L_R_CHANNEL   0    ///Left and Right Channel
#define DAC_L_CHANNEL     1    ///Left Channel
#define DAC_R_CHANNEL     2    ///Right Channel

#define DAC_ISEL5U      BIT(0)
#define DAC_ISEL_HALF	BIT(2)
#define DAC_ISEL_THIRD  BIT(1)

#endif
