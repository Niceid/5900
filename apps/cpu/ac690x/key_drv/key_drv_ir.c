#include "sdk_cfg.h"
#include "timer.h"
#include "clock.h"
#include "key_drv.h"


#include IR_MAP_DIR_H

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

#define IR_PORTA(x) 		(0x00 + x)
#define IR_PORTB(x) 		(0x10 + x)
#define IR_PORTC(x) 		(0x20 + x)
#define IR_PORTD(x) 		(0x30 + x)
#define IR_USBDP    		(0x3E)
#define IR_USBDM 			(0x3F)


typedef struct
{
    u16 wData;          //<键值
    u8  bState;         //<接收状态
    u16 wUserCode;      //<用户码
    u8  boverflow;      //<红外信号超时
#if defined(_SPECIAL_IR_MANAGE_)
	u16 lastData;		//上一次键值
	u16 lastUserCode;	//上一次用户码
	u8 lastCount;		//上一次
	u8 savewData;		//保存上一次键值
#if defined(SPECIAL_CODE_DELAY_CANCEL)
	u8 DelayTime;
#endif
#endif
}IR_CODE;

static IR_CODE ir_code;
static u16 timer1_pad;
__timer_handle *ir_timer;

#if defined(_REMOTE_TWO_TOGETHER_)
static uint8 IR_USER_MAP_TAB[][2]=
{
	IR_MAP_TAB
};
static uint8 IR_USER_MAP_TAB1[][2]=
{
	IR_MAP_TAB1
};
#elif defined(_REMOTE_USE_TOGETHER_)
static uint8 IR_USER_MAP_TAB1[][2]=
{
	IR_MAP_TAB1
};
static uint8 IR_USER_MAP_TAB2[][2]=
{
	IR_MAP_TAB2
};
static uint8 IR_USER_MAP_TAB3[][2]=
{
	IR_MAP_TAB3
};
#else
static uint8 IR_USER_MAP_TAB[][2]=
{
	IR_MAP_TAB
};
#endif

/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
#if defined(_REMOTE_TWO_TOGETHER_)
static uint8 ir_change_usercode(uint8 code)
{
	uint8 Index;

	for(Index = 0; Index < (sizeof(IR_USER_MAP_TAB)/2); Index++)
	{
		if(code == IR_USER_MAP_TAB[Index][0])
		{
			return IR_USER_MAP_TAB[Index][1];
		}
	}
	return IR_VALUE_NULL;
}

static uint8 ir_change_usercode1(uint8 code)
{
	uint8 Index;

	for(Index = 0; Index < (sizeof(IR_USER_MAP_TAB1)/2); Index++)
	{
		if(code == IR_USER_MAP_TAB1[Index][0])
		{
			return IR_USER_MAP_TAB1[Index][1];
		}
	}
	return IR_VALUE1_NULL;
}
#elif defined(_REMOTE_USE_TOGETHER_)
static uint8 ir_change_usercode1(uint8 code)
{
	uint8 Index;

	for(Index = 0; Index < (sizeof(IR_USER_MAP_TAB1)/2); Index++)
	{
		if(code == IR_USER_MAP_TAB1[Index][0])
		{
			return IR_USER_MAP_TAB1[Index][1];
		}
	}
	return IR_VALUE1_NULL;
}

static uint8 ir_change_usercode2(uint8 code)
{
	uint8 Index;

	for(Index = 0; Index < (sizeof(IR_USER_MAP_TAB2)/2); Index++)
	{
		if(code == IR_USER_MAP_TAB2[Index][0])
		{
			return IR_USER_MAP_TAB2[Index][1];
		}
	}
	return IR_VALUE1_NULL;
}
static uint8 ir_change_usercode3(uint8 code)
{
	uint8 Index;

	for(Index = 0; Index < (sizeof(IR_USER_MAP_TAB3)/2); Index++)
	{
		if(code == IR_USER_MAP_TAB3[Index][0])
		{
			return IR_USER_MAP_TAB3[Index][1];
		}
	}
	return IR_VALUE1_NULL;
}
#else
static uint8 ir_change_usercode(uint8 code)
{
	uint8 Index;

	for(Index = 0; Index < (sizeof(IR_USER_MAP_TAB)/2); Index++)
	{
		if(code == IR_USER_MAP_TAB[Index][0])
		{
			return IR_USER_MAP_TAB[Index][1];
		}
	}
	return IR_VALUE_NULL;
}
#endif

static void ir_timeout(void *param)
{
    ir_code.boverflow++;
    if(ir_code.boverflow > 56)
    {
        ir_code.bState = 0;
    }
}

#if defined(_ENABLE_IR_DECODE_)
void timer1_isr(void)
{
    u16 bCap1;
    u16 cap = 0;
	u16 default_timer_prd = 0;

	bCap1= (ir_timer,TIMER_CMD_GET_PRDNUM,0,NULL);
    timer_ctrl(ir_timer,TIMER_CMD_SET_CNT,0,NULL);

	default_timer_prd = timer1_pad/10;	   //给红外遥控增加百分之30的余量
	cap = bCap1 /default_timer_prd; //ms

	if((cap <= 13) && (cap >7))
	{
		ir_code.wData >>= 1;
		ir_code.bState++;
		ir_code.boverflow = 0;
	}
	else if((cap > 17) && (cap < 23))
	{
		ir_code.wData >>= 1;
		ir_code.bState++;
		ir_code.wData |= 0x8000;
		ir_code.boverflow = 0;
	}
	else if((cap == 13*10)&&(ir_code.boverflow < 8))
	{
		ir_code.bState = 0;
	}
	else if((cap < 8*10)&&(ir_code.boverflow < 5))
	{
		ir_code.bState = 0;
	}
	else if((cap >110*10)&&(ir_code.boverflow > 53))
	{
		ir_code.bState = 0;
	}
	else if((cap >20*10)&&(ir_code.boverflow > 53)) //溢出情况下 （12M 48M）
	{
		ir_code.bState = 0;
	}
	else
	{
		ir_code.boverflow = 0;
	}

    if(ir_code.bState == 16)
    {
        ir_code.wUserCode = ir_code.wData;
    }
    if(ir_code.bState == 32)
    {

    }
}

#else
void timer1_isr(void)
{
    u16 bCap1;
    u8 cap = 0;

	bCap1=timer_ctrl(ir_timer,TIMER_CMD_GET_PRDNUM,0,NULL);
    timer_ctrl(ir_timer,TIMER_CMD_SET_CNT,0,NULL);
    cap = bCap1 /timer1_pad;

    if(cap <= 1)
    {
        ir_code.wData >>= 1;
        ir_code.bState++;
        ir_code.boverflow = 0;
    }
    else if(cap == 2)
    {
        ir_code.wData >>= 1;
        ir_code.bState++;
        ir_code.wData |= 0x8000;
        ir_code.boverflow = 0;
    }
    else if((cap == 13)&&(ir_code.boverflow < 8))
    {
        ir_code.bState = 0;
    }
    else if((cap < 8)&&(ir_code.boverflow < 5))
    {
        ir_code.bState = 0;
    }
    else if((cap >110)&&(ir_code.boverflow > 53))
    {
        ir_code.bState = 0;
    }
    else if((cap >20)&&(ir_code.boverflow > 53)) //溢出情况下 （12M 48M）
    {
        ir_code.bState = 0;
    }
    else
    {
        ir_code.boverflow = 0;
    }

    if(ir_code.bState == 16)
    {
        ir_code.wUserCode = ir_code.wData;
    }
    if(ir_code.bState == 32)
    {
	#if defined(_SPECIAL_IR_MANAGE_)
		//key_printf("code:0x%x, value:0x%x\n", ir_code.wUserCode, ir_code.wData);
		ir_code.lastUserCode = ir_code.wUserCode;
		ir_code.lastData = ir_code.wData;
		ir_code.lastCount = 3;
		ir_code.bState = 0;
	#endif
    }
}
#endif

int ir_key_init(void)
{
	//timer1
    int ret = 0;
    key_puts("ir key init\n");

	IR_INIT_PIN;

    __timer_param timer_param;
    ir_timer = timer_open(TIMER1,TIMER_MAX_ISR_FUN);
    if(NULL == ir_timer)return TIMER_DRV_OPEN_ERR;

    timer_param.work_mode = TIMER_WORK_MODE_CAPTURE_IODN;
    timer_param.tick_time = 1000;//1ms

	ret = timer_init_api(ir_timer, &timer_param);
    if(ret != TIMER_NO_ERR)return ret;

#if defined(_ENABLE_IR_PIN_)
	ret = timer_ctrl(ir_timer, TIMER_CMD_IRFLT_SET, IR_PORTB(12), NULL);
#elif defined(_DISABLE_IR_FUNTION_)
	ret = timer_ctrl(ir_timer, TIMER_CMD_IRFLT_SET, IR_PORTB(30), NULL);
#else
    ret = timer_ctrl(ir_timer, TIMER_CMD_IRFLT_SET, IR_PORTB(3), NULL);
#endif
    if(ret != TIMER_NO_ERR)return ret;

    timer1_pad = timer_ctrl(ir_timer, TIMER_CMD_GET_START_PRDNUM, 0, NULL);

    ret = timer_reg_isr_callback_fun(ir_timer, 4, timer1_isr);
    if(ret != TIMER_NO_ERR)return ret;

    ret = timer_start(ir_timer);
    if(ret != TIMER_NO_ERR)return ret;

#if defined(_ENABLE_RADIO_TIME_)
	ret = timer_reg_func(ir_timeout, NULL, 4);
#else
	ret = timer_reg_func(ir_timeout, NULL, 2);
#endif
	if(ret != TIMER_NO_ERR)return ret;

    return 0;
}

uint8 ir_key_value(void)
{
#if defined(_REMOTE_USE_TOGETHER_)
	uint8 key = IR_VALUE1_NULL;
#else
	uint8 key = IR_VALUE_NULL;
#endif

#if defined(_SPECIAL_IR_MANAGE_)
	#if defined(SPECIAL_CODE_DELAY_CANCEL)
		if(ir_code.DelayTime)
			ir_code.DelayTime--;
	#endif
		if(ir_code.lastCount)
		{
			ir_code.lastCount--;
			if((((u8*)&ir_code.lastData)[0] ^ ((u8*)&ir_code.lastData)[1]) == 0xff)//强制转换为前8位数据位，以及后八位检验位，判断数据是否出错
			{
				//key_printf("ir user code:0x%x, code:0x%x--:DelayTime:%d\n", ir_code.lastUserCode,
				if(ir_code.lastUserCode == 0x52ad)//判断用户码是否正确
				{
					if(ir_code.lastData == 0xe619)
					{
					#if defined(SPECIAL_CODE_DELAY_CANCEL)
						if(ir_code.DelayTime)
					#endif
						{
	                        key = ir_change_usercode(ir_code.savewData);
						}
					#if defined(SPECIAL_CODE_DELAY_CANCEL)
						else
							ir_code.savewData = 0xF0;
					#endif

					}
					else
					{
						key = ir_change_usercode(ir_code.lastData);
					}
				}
				else if(ir_code.lastUserCode == 0x50af)
				{
					ir_code.savewData = ir_code.lastData;
					key = ir_change_usercode(ir_code.lastData);
				#if defined(SPECIAL_CODE_DELAY_CANCEL)
					ir_code.DelayTime = 6;
				#endif
				}
			}
		}

#else
    if(ir_code.bState != 32)
    {
        return key;
    }

    if((((u8*)&ir_code.wData)[0] ^ ((u8*)&ir_code.wData)[1]) == 0xff)
    {
    	key_printf("ir_code.wUserCode:0x%x,ir_code.wData:0x%x\n", ir_code.wUserCode, ir_code.wData);
	#if defined(_REMOTE_TWO_TOGETHER_)
		if(ir_code.wUserCode == IR_USER_CODE)
		{
			key = ir_change_usercode(ir_code.wData);
			key_printf("ir key:%d\n", key);
		}
		else if(ir_code.wUserCode == IR_USER_CODE1)
		{
			key = ir_change_usercode1(ir_code.wData);
			key_printf("ir1 key:%d\n", key);
		}
    #elif defined(_REMOTE_USE_TOGETHER_)
    	if(ir_code.wUserCode == IR_USER_CODE1)
    	{
    		key = ir_change_usercode1(ir_code.wData);
    		key_printf("ir1 key:%d\n", key);
    	}
    	else if(ir_code.wUserCode == IR_USER_CODE2)
    	{
    		key = ir_change_usercode2(ir_code.wData);
    		key_printf("ir2 key:%d\n", key);
    	}
    	else if(ir_code.wUserCode == IR_USER_CODE3)
    	{
    		key = ir_change_usercode3(ir_code.wData);
    		key_printf("ir3 key:%d\n", key);
    	}
	#else
		if(ir_code.wUserCode == IR_USER_CODE)
        {
            key = ir_change_usercode(ir_code.wData);
			ew_printf("ir key:%d\n", key);
        }
	#endif
    }
    else
    {
        ir_code.bState = 0;
    }
#endif
    return key;
}

