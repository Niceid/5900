#include "key_state_scan.h"
#include "key_drv.h"

int ADC_TAB[ADC_TAB_SIZE] = {0,2,7,11,17,22,26,31,36,41,46,51,56,60};
int flag1 = 0;
int flag2 = 0;

Get_Key_Ifo KeyIfo[2];

int Key_State_Scan(void)
{
#if defined(GET_KEY1_DATA)||defined(GET_KEY2_DATA)
	int i;
	int key_val;
#endif

#if defined(GET_KEY1_DATA)
	key_val = GET_KEY1_DATA; //取出按键状态编码
	if(key_val < KEY_ADC_MAX2) //判断是否有按键按下
	{
		flag1++;
		if(flag1 >= 2)//消抖,再次判断是否有按键按下
		{
			for(i = 0; i < ADC_TAB_SIZE-1; )
			{
				if( key_val >= ADC_TAB[i++] && key_val < ADC_TAB[i])
					break;
			}
			KeyIfo[0].key_num = i;
			KeyIfo[0].value = key_val;
			KeyIfo[0].key_now_flagval = flag1;
			//if(KeyIfo[0].key_now_flagval >= 5)
				key_printf("Ad_Key1_Scan=%d, Index:%d\n", KeyIfo[0].value = key_val, KeyIfo[0].key_num);
				//通过下标的不同实现不同的功能
			return KeyIfo[0].key_num;
		}
		else
			key_printf("No key1 are pressed!");
	}
	else
	{
		flag1 = 0;
	}
#endif

#if defined(GET_KEY2_DATA)
		key_val = GET_KEY2_DATA; //取出按键状态编码
		if(key_val < KEY_ADC_MAX2) //判断是否有按键按下
		{
			flag2++;  //消抖
			if(flag2 >= 2)
			{
				for(i = 0; i < ADC_TAB_SIZE-1; )
				{
					if( key_val >= ADC_TAB[i++] && key_val < ADC_TAB[i])
						break;
				}
				KeyIfo[1].key_num = i;//数据的存储
				KeyIfo[1].value = key_val;
				KeyIfo[1].key_now_flagval = flag2;
				//通过下标的不同实现不同的功能
				//if(KeyIfo[1].key_now_flagval >= 5)//设置长按的时间
						key_printf("Ad_Key2_Scan=%d, Index:%d\n", KeyIfo[1].value, KeyIfo[1].key_num +ADC_TAB_SIZE - 1);
 				return KeyIfo[1].value, KeyIfo[1].key_num +ADC_TAB_SIZE - 1;
			}
			else
				key_printf("No key2 are pressed!");//点按提示信息
		}
		else
		{
			flag2 = 0;
		}
#endif
    return 0;
}


