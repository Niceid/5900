#include "sdk_cfg.h"
#include "key_drv.h"
#include "dac/dac_api.h"
#include "sys_detect.h"
#include "dac/dac.h"
#include "update.h"
#include "ui_drv.h"
#include "timer.h"
#include "setup.h"


#define UPDATE_SUCCESS_END 		0x88
#define DEVICE_FIRST_START  	BIT(31)


static void reset_clk(void)
{
	u32 * Time_cnt_p = (u32 *)0x19FE8;
	(*Time_cnt_p) = 0;
}

void led_update_start(void)
{
	ew_puts("led_update_start\n");

	SYS_POW_INIT();
    SYS_POW_ON();
    SYS_POW_IN();
#if defined(_KLD_SYS5VPWR_SOFT_CTRL)
	SYS_5VPOW_INIT();
	SYS_5VPOW_HIGH();
#endif
#if defined(_ENABLE_UPGRADE_POWER_)
	SYS_CHARGE_INIT();
	SYS_CHARGE_ON();
#endif
    ui_drv_updata(1);
}

#if defined(_VFD_DRIVER_IC_TM1729_)
extern void WriteDDRAM(void);
#endif
void led_update_finish(void)
{
	ew_puts("led_update_finish\n");

	SYS_POW_INIT();
    SYS_POW_ON();
#if defined(_KLD_SYS5VPWR_SOFT_CTRL)
	SYS_5VPOW_INIT();
	SYS_5VPOW_HIGH();
#endif
	ui_drv_updata(0);
#if defined(_VFD_DRIVER_IC_TM1729_)
	WriteDDRAM();
#endif
}


static u32 update_flag = 0;
void update_check(u32 res)
{
    update_flag = res;
}

bool device_is_first_start(void)
{
    ew_printf("update_flag=0x%x\n", update_flag);

    if(update_flag&DEVICE_FIRST_START)
    {
        ew_puts("\n=================device_is_first_start=========================\n");
        return true;
    }
    return false;
}

bool device_is_updata_over(void)
{
	if((update_flag&0xff) == UPDATE_SUCCESS_END)
	{
        ew_puts("\n=================device_is_updata_over=========================\n");
        return true;
	}
    return false;
}

/*----------------------------------------------------------------------------*/
/**@brief   update deal
   @param   option=UPDATE_OK_RUN:system run directly when update successfully
                  =UPDATE_OK_WHILE:keep remainding when update successfully
   @return  void
   @note    You can select remaind modes between led_display and key_tone, or
            define any other ways you prefer.
 */
/*----------------------------------------------------------------------------*/

void update_deal(u8 option)
{
    if((update_flag&0xff) != UPDATE_SUCCESS_END)
	{
        ew_puts("Normal PowerUp\n");
        return;
    }

    ew_puts("\n==========================================\n");
    ew_puts("   Update Success    ");
    ew_puts("\n==========================================\n");

    if(option == UPDATE_OK_RUN)
	{
        ew_puts("update run\n");
        return;
    }

    led_update_finish();
	
	reset_clk();

	u8 key_voice_cnt = 0;
    while(1)
    {
        clear_wdt();
        delay(0xAFFFFF);
        if((++key_voice_cnt) >= 3)
        {
			/*
		#ifdef	TD_ADD_ALEN 
			open_wdt(0x05);
			while(1);
		#else
            JL_POWER->CON |= BIT(4); //»Ì∏¥Œª
			//soft_power_ctl(PWR_OFF);
		#endif*/
			JL_POWER->CON |= BIT(4); //»Ì∏¥Œª
        }
    }
}

