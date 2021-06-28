#include "sdk_cfg.h"
#include "setup.h"
#include "sys_detect.h"
#if defined(_SUPPORT_C912DAB_)
#include "C912DAB\C912dab.h"
c912dab_ctrl_t *c912dabCtrl = NULL;

static uint8 tuner_c912dab_detect(void)
{	
    u16 count=60;
#ifdef TD_ADD_USER_UART
	uart_interrupt_receive_init(1200);
#endif	
    while((!gC912detOk)&&(count--))
    {
        clear_wdt();
    	uart_interrupt_receive_write(0x00);
    	OSTimeDly(5);
    }
    if(gC912detOk)
    {
    	return TRUE;
    }
	printf("c912dab_detect:0\n");
	return FALSE;
}

static uint8 tuner_c912dab_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	if(c912dabCtrl == NULL)
	{
		c912dabCtrl = (c912dab_ctrl_t *)malloc_fun(c912dabCtrl, sizeof(c912dab_ctrl_t), 0);
		if(c912dabCtrl == NULL)
		{
			free_fun((void **)&c912dabCtrl);
			return FALSE;
		}
	}
    if(band ==RADIO_BAND_FM)
    {
        c912dabCtrl->Mode =1;
        if((min_freq > max_freq)||(min_freq < 8750)||(max_freq > 10800))
        	return FALSE;
        uart_interrupt_receive_write(0x74);
        uart_interrupt_receive_write(0x34);
    }
    else//DABÊ¹ÓÃ
    {
        c912dabCtrl->Mode =2;
//        if((min_freq > max_freq)||(min_freq < 16800)||(max_freq > 24000))
//        	return FALSE;
        uart_interrupt_receive_write(0x73);
        uart_interrupt_receive_write(0x33);
    }
	return TRUE;
}

static uint8 tuner_c912dab_exit(void)
{
    free_fun((void **)&c912dabCtrl);
    gC912detOk =FALSE;
#ifdef TD_ADD_USER_UART
    Close_Uart2();
    DAB_RX_INIT();
    DAB_RX_OUT();
    DAB_RX_LOW();
    DAB_TX_INIT();
    DAB_TX_OUT();
    DAB_TX_LOW();
#endif
	return TRUE;
}

static uint8 tuner_c912dab_freq(uint16 freq)
{
 /*   u8 freq_cmd[2];
    if(c912dabCtrl->Mode == 1)
    {
	    freq_cmd[0]=((((freq-8750)/5)/0x40)|0x60);
	    freq_cmd[1]=(((freq-8750)/5)%0x40);
        uart_interrupt_receive_write(freq_cmd[0]);
        uart_interrupt_receive_write(freq_cmd[1]);
    }*/
	return TRUE;
}

static uint8 tuner_c912dab_status(engine_status_t *status)
{
    /*uart_interrupt_receive_write(0x72);
    uart_interrupt_receive_write(0x32);
    status->freq = TRUE;
    status->signal = TRUE;
    status->valid = TRUE;
    status->stereo = TRUE;
    status->band = TRUE;
    status->stc = TRUE;*/
	return TRUE;
}
const tuner_drv_fun_t tuner_c912dab_drv_fun =
{
	tuner_c912dab_detect,
	tuner_c912dab_init,
	tuner_c912dab_exit,
	tuner_c912dab_freq,
	tuner_c912dab_status,
	NULL,//tuner_qn8065_search,
	NULL,
	NULL,//tuner_qn8065_setting,
	NULL,//tuner_qn8065_info,
};

#endif
