#ifndef KEY_STATE_SCAN_H_
#define KEY_STATE_SCAN_H_

#define uint unsigned int
#define KEY_ADC_MAX2  63  //最大电压数字编码
#define ADC_TAB_SIZE 14

typedef struct
{
	int key_num;
	int key_now_flagval;
	int value;
}Get_Key_Ifo;

int ADC_TAB[ADC_TAB_SIZE];

int Key_State_Scan();

#endif
