#include "sdk_cfg.h"
#include "key_drv.h"
#include "timer.h"
#include "order.h"

#include "key_drv_vr.h"

#if defined(_SUPPORT_VR_)

static key_vr_t key_vr;

#if defined(_VR_SCAN_IO_)

#if defined(_VR_SEG_CODE_SIGNAL_)
    #define VR_UP_CN_0                  0x07
    #define VR_UP_CN_1                  0x38
    #define VR_DOWN_CN_0                0x0B
    #define VR_DOWN_CN_1                0x34
//------------------------------------------------------------    
#elif defined(_VR_SEC_CODE_DOUBLE_)
    #define VR_UP_CN_0                  0x87
    #define VR_DOWN_CN_0                0x4B
#endif            	  

static void key_vr_scan(void *param)//±àÂëÆ÷É¨Ãè¼ì²â
{
	uint8 b = 0;

	if(VOL_POSITIVE_GET())b = 1;
	if(VOL_NEGATIVE_GET())b +=2;
	if((key_vr.Vrs & (_B1_ | _B0_))==b)return;
	key_vr.Vrs<<=2;
	key_vr.Vrs|=b;
#if defined(_VR_SEG_CODE_SIGNAL_)
	key_vr.Vrs &= 0x3F;
	printf("key_vr.Vrs:0x%x\n",key_vr.Vrs);
	switch(key_vr.Vrs)
	{	 
		case VR_UP_CN_0:
		case VR_UP_CN_1:
		#if defined(_VR_SCAN_CHANGE_)
			key_vr.Value--;
		#else
			key_vr.Value++;
		#endif
			break;
		case VR_DOWN_CN_0:
		case VR_DOWN_CN_1:
		#if defined(_VR_SCAN_CHANGE_)
			key_vr.Value++;
		#else
			key_vr.Value--;
		#endif
			break;
	}
#elif defined(_VR_SEC_CODE_DOUBLE_)
	switch(key_vr.Vrs)
	{	 
		case VR_UP_CN_0:
		#if defined(_VR_SCAN_CHANGE_)
			key_vr.Value--;
		#else
			key_vr.Value++;
		#endif
			break;
		case VR_DOWN_CN_0:
		#if defined(_VR_SCAN_CHANGE_)
			key_vr.Value++;
		#else
			key_vr.Value--;
		#endif
			break;
	}
#endif
}

#else

#if defined(_YS_SPECIAL_VR_)
#define VR_AD0							0x2f//
#define VR_AD1							0x34//
#define VR_AD2							0x38//
#else
#define VR_AD0							0x18//
#define VR_AD1							0x22//
#define VR_AD2							0x2A//
#endif

#define VR_ADC_MAX						0x3F
#define VR_F_BIT						0x3F

#if defined(_VR_SEG_CODE_SIGNAL_)
#define VR_UP_CN_0						24
#define VR_UP_CN_1						33
#define VR_DOWN_CN_0					9
#define VR_DOWN_CN_1					18
#elif defined(_VR_SEC_CODE_DOUBLE_)
#define VR_UP_CN_0						8
#define VR_UP_CN_1						24
#define VR_DOWN_CN_0					4
#define VR_DOWN_CN_1					36
#endif

//uint8 time_isr;
static void key_vr_scan(void *param)//±àÂëÆ÷É¨Ãè¼ì²â
{
	/*if(time_isr)
	{
		AUDIO_MUTE_OFF();
		time_isr = FALSE;
	}
	else
	{
		AUDIO_MUTE_ON();
		time_isr = TRUE;
	}*/
	
	uint8 ad_val = GET_ENCODE_DATA;
	
#if defined(_ENABLE_VR_KEY_)
	if(ad_val < 3)return;
#endif

	if(key_vr.ad_val != ad_val)
	{
		key_vr.ad_val = ad_val;
		//key_printf("vr val:0x%x\n", ad_val);// ±àÂëÆ÷Öµ
		if( ad_val < ( ( ( VR_ADC_MAX - VR_AD2 ) / 2 ) + VR_AD2 ) )
		{
			uint8 ad_state;
			if( ad_val < ( ( ( VR_AD1 - VR_AD0 ) / 2 ) + VR_AD0 ) )ad_state = 0;
			else if( ad_val < ( ( ( VR_AD2 - VR_AD1 ) / 2 ) + VR_AD1 ) )ad_state = 1;
			else ad_state = 2;
						
			if((key_vr.Vrs & (_B1_ | _B0_)) != ad_state)
			{
				key_vr.Vrs <<= 2;
				key_vr.Vrs  |= ad_state;
				//key_printf("key_vr.Vrs:%d\n", key_vr.Vrs & VR_F_BIT);

				#if defined(_TY2200A_SPECIAL_VR_)
					if((key_vr.Vrs & VR_F_BIT) == 17)
					{
						key_vr.vr = 1;
					}
					
					if(key_vr.vr)
					{
						if((key_vr.Vrs & VR_F_BIT) == 4)
						{
							key_vr.vr = 0;
							return;
						}
					}
				#endif
				
				switch(key_vr.Vrs & VR_F_BIT)
				{
				#if 0
				case VR_UP_CN_0:
			#if !defined(_VR_SEC_CODE_DOUBLE_)
				case VR_UP_CN_1:
			#endif
					key_vr.Value++;
					break;
				case VR_DOWN_CN_0:
			#if !defined(_VR_SEC_CODE_DOUBLE_)
				case VR_DOWN_CN_1:
			#endif
					key_vr.Value--;
					break;

				#else
					case VR_UP_CN_0:
					case VR_UP_CN_1:
				#if defined(_EVB_SPECIAL_VR_)
					case 8:
					case 17:
				#endif
					#if defined(_VR_SCAN_CHANGE_)
						key_vr.Value--;
					#else
						key_vr.Value++;
					#endif
						break;
					case VR_DOWN_CN_0:
					case VR_DOWN_CN_1:
			    #if defined(_EVB_SPECIAL_VR_)
			    	case 4:
			    	case 34:
			    #endif
					#if defined(_VR_SCAN_CHANGE_)
						key_vr.Value++;
					#else
						key_vr.Value--;
					#endif
						break;
				#endif
				}
			}
		}
	}
}
#endif

void key_vr_value(void)
{
	if(key_vr.Value!=0)
	{
		if(key_vr.Value >0)
		{
			if(key_vr.Value > 80)key_vr.Value = 80;
			if(key_vr.Value >5)
			{
				key_vr.Value -= 3;
			}
			else
			{
				key_vr.Value--;
			}
			send_key_msg(MSG_VOL_ADD);
		}
		else
		{
			if(key_vr.Value < -80)key_vr.Value = -80;
			if(key_vr.Value < -5)
			{
				key_vr.Value += 3;
			}
			else
			{
				key_vr.Value++;
			}
			send_key_msg(MSG_VOL_SUB);
		}
	}
}

void key_vr_init(void)
{
	memset(&key_vr, 0, sizeof(key_vr));
	
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(key_vr_scan, NULL, 4);
#else
	timer_reg_func(key_vr_scan, NULL, 2);
#endif
}
#endif

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/

#if defined(_SUPPORT_VR2_)

static key_vr2_t key_vr2;



//#if defined(ADC_USER_VOLTAGE_FLUCTUATION_)
#define VR2_AD0							0x18//
#define VR2_AD1							0x22//
#define VR2_AD2							0x2A//

#define VR2_ADC_MAX						0x3F
#define VR2_F_BIT						0x3F

/*#else
#define VR2_AD0							0x17//
#define VR2_AD1							0x1D//
#define VR2_AD2							0x27//

#define VR2_ADC_MAX						0x3C
#define VR2_F_BIT						0x3F
#endif*/

#if defined(_VR_SEG_CODE_SIGNAL_)
#define VR2_UP_CN_0						24
#define VR2_UP_CN_1						33
#define VR2_DOWN_CN_0					9
#define VR2_DOWN_CN_1					18
#elif defined(_VR_SEC_CODE_DOUBLE_)
#define VR2_UP_CN_0						8
#define VR2_UP_CN_1						24
#define VR2_DOWN_CN_0					4
#define VR2_DOWN_CN_1					36
#endif

static void key_vr2_scan(void *param)//±àÂëÆ÷É¨Ãè¼ì²â
{
	uint8 ad_val = GET_ENCODE2_DATA;

	if(key_vr2.ad_val != ad_val)
	{
		key_vr2.ad_val = ad_val;
			
		if( ad_val < ( ( ( VR2_ADC_MAX - VR2_AD2 ) / 2 ) + VR2_AD2 ) )
		{
			uint8 ad_state;
			if( ad_val < ( ( ( VR2_AD1 - VR2_AD0 ) / 2 ) + VR2_AD0 ) )ad_state = 0;
			else if( ad_val < ( ( ( VR2_AD2 - VR2_AD1 ) / 2 ) + VR2_AD1 ) )ad_state = 1;
			else ad_state = 2;
						
			if((key_vr2.Vrs & (_B1_ | _B0_)) != ad_state)
			{
				key_vr2.Vrs <<= 2;
				key_vr2.Vrs  |= ad_state;
				
				//key_printf("key_vr2.Vrs:%d\n", key_vr2.Vrs & VR2_F_BIT);
				switch(key_vr2.Vrs & VR2_F_BIT)
				{
					case VR2_UP_CN_0:
					case VR2_UP_CN_1:
					#if defined(_VR_SCAN_CHANGE_)||defined(_VR2_SCAN_CHANGE_)
						key_vr2.Value--;
					#else
						key_vr2.Value++;
					#endif
						break;
					case VR2_DOWN_CN_0:
					case VR2_DOWN_CN_1:
					#if defined(_VR_SCAN_CHANGE_)||defined(_VR2_SCAN_CHANGE_)
						key_vr2.Value++;
					#else
						key_vr2.Value--;
					#endif
						break;
				}
			}
		}
	}
}

void key_vr2_value(void)
{
	if(key_vr2.Value!=0)
	{
		if(key_vr2.Value >0)
		{
			if(key_vr2.Value > 80)key_vr2.Value = 80;
			if(key_vr2.Value >5)
			{
				key_vr2.Value -= 3;
			}
			else
			{
				key_vr2.Value--;
			}
			send_key_msg(MSG_VOL_ADD2);
		}
		else
		{
			if(key_vr2.Value < -80)key_vr2.Value = -80;
			if(key_vr2.Value < -5)
			{
				key_vr2.Value += 3;
			}
			else
			{
				key_vr2.Value++;
			}
			send_key_msg(MSG_VOL_SUB2);
		}
	}
}

void key_vr2_init(void)
{
    memset(&key_vr2, 0, sizeof(key_vr2));
    
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(key_vr2_scan, NULL, 4);
#else
	timer_reg_func(key_vr2_scan, NULL, 2);
#endif
}
#endif

