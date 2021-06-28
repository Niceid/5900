#include "sdk_cfg.h"
#include "key_drv.h"
#include "key_drv_ad.h"
#include "key_drv_ir.h"
#include "key_drv_vr.h"
#include "key_drv_voice.h"
#include "order.h"
#include "timer.h"
#include "setup.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

const char *keymsg_task_name = NULL;
INPUT_INFO_FORMAT KeyInfo;
INPUT_INFO_FORMAT IrInfo;

/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

static uint8 get_input_value(INPUT_INFO_FORMAT *Type, uint8 Value, uint8 BufFlag)
{
	if(BufFlag)
	{//AD按键
		Type->Buffer[1] = Type->Buffer[0];
		Type->Buffer[0] = Value;

		if(Type->Buffer[1] != Type->Buffer[0])
		{
			return FALSE;
		}
	}

	if(Value == Type->NowValue)
	{
		if(Type->ValueTime < INPUT_TIME_MAX)
		{
			Type->ValueTime++;
		}
	}
	else
	{
		Type->LastTime 	= Type->ValueTime;
		Type->ValueTime = 0;
		Type->LastValue	= Type->NowValue;
		Type->NowValue	= Value;
	}

	if((Type->LastValue != INPUT_VALUE_NULL) && (Type->NowValue != INPUT_VALUE_NULL))
	{
		return FALSE;
	}

	if(Type->LastValue == INPUT_VALUE_NULL)
	{
		Type->Value = Type->NowValue;
		Type->State = DOWN;
	}
	else
	{
		Type->Value = Type->LastValue;
		Type->State = UP;
	}
	return TRUE;
}

static void key_scan(void *param)
{
#if defined(_SUPPORT_VR_)
	key_vr_value();
#endif

#if defined(_SUPPORT_VR2_)
	key_vr2_value();
#endif

#if defined(_SUPPORT_REAL_VBAR_)
	ad_vbar_scan();
#endif
	if(get_input_value(&KeyInfo, ad_key_scan(), TRUE))//判断按键信息有没有发生变化
	{
		key_process();//根据按键信息实现功能
	}

	if(get_input_value(&IrInfo, ir_key_value(), FALSE))
	{
		ir_process();
	}
}

/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

void key_init(void)
{
	key_puts("key init\n");

	keymsg_task_name = NULL;
	memset(&KeyInfo, 0, sizeof(KeyInfo));
	memset(&IrInfo, 0, sizeof(IrInfo));

	KeyInfo.ValueTime = INPUT_TIME_MAX;
	KeyInfo.LastTime  = INPUT_TIME_MAX;
	KeyInfo.LastValue = KEY_VALUE_POWER;
	KeyInfo.NowValue  = KEY_VALUE_NULL;

	IrInfo.ValueTime = INPUT_TIME_MAX;
	IrInfo.LastTime  = INPUT_TIME_MAX;
#if defined(_REMOTE_USE_TOGETHER_)
	IrInfo.LastValue = IR_VALUE1_POWER;
	IrInfo.NowValue  = IR_VALUE1_NULL;
#else
	IrInfo.LastValue = IR_VALUE_POWER;
	IrInfo.NowValue  = IR_VALUE_NULL;
#endif

#if defined(_SUPPORT_VR_)
	key_vr_init();
#endif

#if defined(_SUPPORT_VR2_)
	key_vr2_init();
#endif

	ad_key_init();
	ir_key_init();

	order_init();

#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(key_scan, NULL, 100);//通过时间片轮转函数实现按键扫描
#else
	timer_reg_func(key_scan, NULL, 50);
#endif
}

void key_msg_register(const char *task_name, uint8 project)
{
    keymsg_task_name = task_name;
	order_project(project);
	order_select(KEY_SELECT_DEFAULT);
}

void key_msg_select(uint8 select)
{
	if(select < KEY_SELECT_MAX)
	{
		order_select(select);
	}
}


/*---------------------------------------------------------------*/
