#include "sdk_cfg.h"
#include "timer.h"
#include "key_drv.h"
#include "common/flash_cfg.h"
#include "vm/vm_api.h"
#include "clock.h"
#include "sys_detect.h"
#include "hw_cpu.h"
#include "cpu.h"
#include "dac/dac.h"
#include "dac/ladc.h"
#include "irq_api.h"
#include "uart_api.h"
#include "uart_param.h"
#include "rtc.h"
#include "update.h"
#include "mcpwm_api.h"
#include "setup.h"


#define BOARD_DEBUG
#ifdef BOARD_DEBUG
#define board_printf 	ew_printf
#define board_puts 		ew_puts
#else
#define board_printf(...)
#define board_puts(...)
#endif


#define FLASH_BASE_ADDR 0x1000000

extern OS_TCB  *OSTCBCur[1];
extern u8 vmm_fat[192];
extern u32 pm_usd_map[(MAX_SDRAM_SIZE + ((32*PAGE_SIZE)-1))/(32*PAGE_SIZE)];
extern void set_sydf_header_base(u32 base);
extern void set_cache_base(u32 base);
extern void bt_app_cfg();
extern void set_bt_osc_internal_cfg(u8 sel_l,u8 sel_r );
extern void set_pwrmd(u8 sm);
extern u8 read_reset_power();
extern int main_task_init(void);
extern int TaskBtTest();

APP_USE_FLASH_SYS_CFG app_use_flash_cfg;

AT(.ver_magic_start)
const char ver_start_magic[] = {0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x5F, 0x73, 0x74, 0x61, 0x72, 0x74, 0x00};

AT(.ver_magic_end)
const char ver_end_magic[] = {0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x5F, 0x65, 0x6E, 0x64, 0x00};

const u8 irq_index_2_prio_tab[][2] =
{
	{IRQ_TIME0_IDX      ,0},// 0
	{IRQ_TIME1_IDX      ,1},// 1
	{IRQ_TIME2_IDX      ,3},
#if defined(_ENABLE_BEEP_)
	{IRQ_TIME3_IDX		,3},
#else
	{IRQ_TIME3_IDX      ,0},
#endif
	{IRQ_USB_SOF_IDX    ,2},
	{IRQ_USB_CTRL_IDX   ,2},
	{IRQ_RTC_IDX        ,0},
	{IRQ_ALINK_IDX      ,2},
	{IRQ_DAC_IDX        ,2},
	{IRQ_PORT_IDX       ,0},
	{IRQ_SPI0_IDX       ,0},
	{IRQ_SPI1_IDX       ,0},
	{IRQ_SD0_IDX        ,0},
	{IRQ_SD1_IDX        ,0},
	{IRQ_UART0_IDX      ,0},
#if defined(_SUPPORT_DISC_)||defined(_ENABLE_DAB_UART_)
	{IRQ_UART1_IDX      ,3},//0
#else
	{IRQ_UART1_IDX		,0},
#endif
#if defined(DAB_MODLE)||defined(_USER_TFT_DISP_)||defined(_SUPPORT_CAN_UART_)||defined(_AUDIO_DRIVER_AP3768_)
#if defined(_ENABLE_DAB_UART_)
	{IRQ_UART2_IDX		,0},
#else
	{IRQ_UART2_IDX      ,3},
#endif
#else
	{IRQ_UART2_IDX      ,0},
#endif
	{IRQ_PAP_IDX        ,0},
	{IRQ_IIC_IDX        ,0},
	{IRQ_SARADC_IDX     ,0},
	{IRQ_FM_HWFE_IDX    ,1},
	{IRQ_FM_IDX         ,1},
	{IRQ_FM_LOFC_IDX    ,1},
	{IRQ_BREDR_IDX      ,2},
	{IRQ_BT_CLKN_IDX    ,2},
	{IRQ_BT_DBG_IDX     ,1},
	{IRQ_BT_PCM_IDX     ,2},
	{IRQ_SRC_IDX        ,2},
	{IRQ_EQ_IDX         ,2},
	{IRQ_BLE_IDX        ,2},
	{IRQ_NFC_IDX        ,1},
	{IRQ_CTM_IDX		,1},
	{IRQ_SOFT0_IDX      ,0},
	{IRQ_SOFT_IDX       ,0},
	{IRQ_EXCEPTION_IDX  ,0},
};


/*----------------------------------------------------------------------------*/
/**@brief  异常错误中断服务程序
   @param
   @return
   @note
 */
/*----------------------------------------------------------------------------*/
void exception_isr(void )
{
    u32 tmp;

    ///board_printf rets & reti
    asm("mov %0, RETS " : "=r" (tmp));

	board_printf("\nRETS=0x%x\n", tmp);

	board_printf("DEBUG_MSG = 0x%x\n", JL_DEBUG->MSG);
	board_printf("PRP MMU_MSG = 0x%x\n", JL_DEBUG->PRP_MMU_MSG);
	board_printf("LSB MMU MSG = 0x%x\n", JL_DEBUG->LSB_MMU_MSG_CH);
	board_printf("PRP WR LIMIT MSG = 0x%x\n", JL_DEBUG->PRP_WR_LIMIT_MSG);
	board_printf("LSB WR LIMIT MSG = 0x%x\n", JL_DEBUG->LSB_WR_LIMIT_CH);

	asm("mov %0, RETI " : "=r" (tmp));

	board_printf("\nRETI=0x%x\n", tmp);

     //JL_POWER->CON |= BIT(4); //异常后软复位
    while(1);
    /* JL_POWER->CON |= BIT(4); //异常后软复位 */
}
/* IRQ_REGISTER(IRQ_EXCEPTION_IDX, exception_isr);
 */

/*----------------------------------------------------------------------------*/
/**@brief  poweroff_ram_fun
   @param
   @return
   @note

 */
/*----------------------------------------------------------------------------*/
void poweroff_ram_fun()
{
    // close_wdt();//关看门狗
    uart_init(UART_BAUD_RAE); //<串口波特率
    delay(10000);
//    putchar('X');
//    putchar('K');
//    putchar('O');
    //ew_puts("\n  ------ run ram ------- \n");
}


/*$PAGE*/


void set_g_sys_cfg(FLASH_SYS_CFG *sys_cfg,u32 cfg_addr)
{
    u8 cfg_data[16];
    u8 *cfg_ptr;
    u32 i;
    board_printf("cfg_addr =  %08x \n",cfg_addr);
    memcpy(sys_cfg, (void*)(FLASH_BASE_ADDR + cfg_addr), sizeof(FLASH_SYS_CFG));
    app_use_flash_cfg.cfg_zone_addr = sys_cfg->flash_cfg.cfg_zone_addr;
    app_use_flash_cfg.flash_size = sys_cfg->flash_cfg.flash_size;
    app_use_flash_cfg.cfg_zone_size = sys_cfg->flash_cfg.cfg_zone_size;

    board_printf("cfg_zone_addr =%08x \n",app_use_flash_cfg.cfg_zone_addr);
    board_printf("cfg_zone_size =%08x \n",app_use_flash_cfg.cfg_zone_size);

    cfg_ptr=(u8*)(FLASH_BASE_ADDR+app_use_flash_cfg.cfg_zone_addr);
    for(i=0;i<app_use_flash_cfg.cfg_zone_size;i+=16)
    {
        u32 *tmp;
        memcpy(cfg_data,cfg_ptr,16);
        if(memcmp((const char *)cfg_data,"BTIF",4)==0)
        {
            tmp=(u32 *)(cfg_data+4);
            app_use_flash_cfg.btif_addr=*tmp;
            tmp=(u32 *)(cfg_data+8);
            app_use_flash_cfg.btif_len=*tmp;
            board_printf("btif_addr =%08x \n",app_use_flash_cfg.btif_addr);
            board_printf("btif_len =%08x \n",app_use_flash_cfg.btif_len);
        }
        else if(memcmp((const char *)cfg_data,"VMIF",4)==0)
		{
			tmp=(u32 *)(cfg_data+4);
			app_use_flash_cfg.vmif_addr=*tmp;
			tmp=(u32 *)(cfg_data+8);
			app_use_flash_cfg.vmif_len=*tmp;
			board_printf("VM_addr =%08x \n",app_use_flash_cfg.vmif_addr);
			board_printf("VM_len =%08x \n",app_use_flash_cfg.vmif_len);
		}
		else
		{
			tmp=(u32 *)(cfg_data+4);
			board_printf("USER_addr =%08x \n",*tmp);
			tmp=(u32 *)(cfg_data+8);
			board_printf("USER_len =%08x \n",*tmp);
		}
        cfg_ptr+=16;
    }

#if 1
    board_printf("flash_id : %x\n",   sys_cfg->flash_cfg.flash_id);
    board_printf("flash_size : %x\n",   sys_cfg->flash_cfg.flash_size);
    board_printf("flash_file_size : %x\n",   sys_cfg->flash_cfg.flash_file_size);
    board_printf("sdfile_head_addr : %x\n",   sys_cfg->flash_cfg.sdfile_head_addr);
    board_printf("spi_run_mode : %x\n",   sys_cfg->flash_cfg.spi_run_mode);
    board_printf("spi_div : %x\n",   sys_cfg->flash_cfg.spi_div);
    board_printf("flash_base : %x\n",   sys_cfg->flash_cfg.flash_base);
    board_printf("protected_arg : %x\n",   sys_cfg->flash_cfg.protected_arg);
    board_printf("cfg_zone_addr : %x\n",   sys_cfg->flash_cfg.cfg_zone_addr);
    board_printf("cfg_zone_size : %x\n",   sys_cfg->flash_cfg.cfg_zone_size);
#if 0
    board_printf("sdram_refresh_cycle : %x\n",   sys_cfg->sdr_cfg.sdram_refresh_cycle);
    board_printf("sdram_refresh_time : %x\n",   sys_cfg->sdr_cfg.sdram_refresh_time);
    board_printf("sdram_cas_latency : %x\n",   sys_cfg->sdr_cfg.sdram_cas_latency);
    board_printf("sdram_trfc : %x\n",   sys_cfg->sdr_cfg.sdram_trfc);
    board_printf("sdram_trp : %x\n",   sys_cfg->sdr_cfg.sdram_trp);
    board_printf("sdram_trcd : %x\n",   sys_cfg->sdr_cfg.sdram_trcd);
    board_printf("sdram_colum : %x\n",   sys_cfg->sdr_cfg.sdram_colum);
    board_printf("sdram_clk_hd_en : %x\n",   sys_cfg->sdr_cfg.sdram_clk_hd_en);
    board_printf("sdram_trrd : %x\n",   sys_cfg->sdr_cfg.sdram_trrd);
    board_printf("sdram_clkoe : %x\n",   sys_cfg->sdr_cfg.sdram_clkoe);
    board_printf("sdram_size : %x\n",   sys_cfg->sdr_cfg.sdram_size);
    board_printf("sdram_fast_clock_rate : %x\n",   sys_cfg->sdr_cfg.sdram_fast_clock_rate);
#endif
    board_printf("pll_sel : %x\n",   sys_cfg->clk_cfg.pll_sel);
    board_printf("osc_freq : %x\n",   sys_cfg->clk_cfg.osc_freq);
    board_printf("osc_src :%x\n",   sys_cfg->clk_cfg.osc_src);
    board_printf("osc_hc_en : %x\n",   sys_cfg->clk_cfg.osc_hc_en);
    board_printf("osc_1pin_en : %x\n",   sys_cfg->clk_cfg.osc_1pin_en);
    board_printf("---------------------------------------------\r\n");
#endif
}
void wake_io_check(void)
{
	u8 reg;
 	reg = get_wake_up_io();
	printf("wake up io:0x%x 0x%x\n",reg,(JL_POWER->CON>>5));

#if defined(_ENABLE_RTC_SLEEP_)
	extern uint32 vm_reset_flag;
	extern uint8 rtc_reset_flag;
	rtc_reset_flag = 0;
#if defined(_CLEAN_MEMORY_)
    if(((reg!=0) &&(reg!=0x0F)) ||(JL_POWER->CON>>5 ==0x04))
    {
    	vm_reset_flag = 0;
    	rtc_reset_flag = 0xAA;
    }
    else
    {
    	vm_reset_flag = -1;
    	rtc_reset_flag = 0;
    }
#else
	if((reg!=0)  ||(JL_POWER->CON>>5 ==0x03))
	{
		puts("vm_reset_flag1111\n");
	#if defined(_USB_UPG_CLEAN_MEMORY_)
		vm_reset_flag = -1;
		rtc_reset_flag = 0;
	#else
		vm_reset_flag = 0;
		rtc_reset_flag = 0xAA;
	#if defined(_DISABLE_AUTO_POWER_)
		global.power_flag = TRUE;
	#endif
	#endif
	}
	else
	{
		puts("vm_reset_flag2222\n");
		vm_reset_flag = -1;
		rtc_reset_flag = 0;
	}
#endif
	//printf("vm_reset_flag %d\n",vm_reset_flag);
#endif

    if(reg & BIT(0))
    {
		puts("-WK_PR0\n");
    }
    else if(reg & BIT(1))
    {
		puts("-WK_PR1\n");
    }
    else if(reg & BIT(2))
    {
		puts("-WK_PR2\n");
    }
    else if(reg & BIT(3))
    {
		puts("-WK_PR3\n");
    }
}
void mcpwm_test()
{
	/* MCPWMCH0_H  PA5     MCPWMCH0_L   PA6  */
	/* MCPWMCH1_H  PB1     MCPWMCH1_L   PB2  */
	/* MCPWMCH2_H  PB5     MCPWMCH2_L   PB6  */
	/* MCPWMCH3_H  PB7     MCPWMCH3_L   PB8  */
	/* MCPWMCH4_H  PA10    MCPWMCH4_L   PA11 */
	/* MCPWMCH5_H  PC1     MCPWMCH5_L   PC2  */

	mcpwm_module_on(MCPWM_INCREASE_REDUCE_MODE , SYS_Hz , MCPWM_CLK_DIV8 , MCPWM_USE_DENOMINATOR);

	mcpwm0_init(MCPWMCH0_L_OPPOSITE , MCPWMCH0_H | MCPWMCH0_L);
	set_mcpwm0(10000L,11);

	mcpwm1_init(MCPWMCH1_L_OPPOSITE , MCPWMCH1_H | MCPWMCH1_L);
	set_mcpwm1(20000L,22);

	mcpwm2_init(MCPWMCH2_L_OPPOSITE , MCPWMCH2_H | MCPWMCH2_L);
	set_mcpwm2(30000L,33);

	mcpwm3_init(MCPWMCH3_L_OPPOSITE , MCPWMCH3_H | MCPWMCH3_L);
	set_mcpwm3(40000L,44);

	mcpwm4_init(MCPWMCH4_L_OPPOSITE , MCPWMCH4_H | MCPWMCH4_L);
	set_mcpwm4(50000L,55);

	mcpwm5_init(MCPWMCH5_L_OPPOSITE , MCPWMCH5_H | MCPWMCH5_L);
	set_mcpwm5(60000L,66);

	/* mcpwm_module_off(MCPWMCH0_H); */

	close_wdt();
	while(1);
}

/*----------------------------------------------------------------------------*/
/**@brief  system start enter
   @param  cfg_addr - g_sys_cfg's addr ， addr - app_file's addr，res - no used
   @return
   @note

 */

/*----------------------------------------------------------------------------*/
extern void power_init(u8 mode , u32 sys_freq, u8 charge_flag);
extern void set_poweroff_wakeup_io(void);
extern void set_sleep_mode_wakeup_io(void);
extern void set_sleep_before_close_irq(void);
extern void set_poweroff_wakeup_io_handle_register(void (*handle)(),void (*sleep_io_handle)(),void (*sleep_handle)());
extern void enter_sleep_mode(void);
/*3.53v-3.34v-3.18v-3.04v-2.87v-2.73v-2.62v-2.52v*/
extern void set_sys_ldo_level(u8 level);
extern void set_use_vdc15_level(u8 level);
extern void set_bt_osc_internal_cfg(u8 sel_l,u8 sel_r );

#if defined(TD_ADD_W2DAB_UART)
extern u8 DAB_ACC;
#endif
void  board_main(u32 cfg_addr, u32 addr, u32 res,u32 update_flag)
{
    u32 tmp;
    FLASH_SYS_CFG sys_cfg;
	irq_index_tab_reg((void *)irq_index_2_prio_tab, sizeof(irq_index_2_prio_tab));

#if defined(_LCD_DRIVER_A4900B_)||defined(_LCD_DRIVER_A4901_)||defined(_LCD_LCD_COM5_SEG8)||defined(_LCD_LCD_COM5_SEG9)//直驱PA3上电或者开ACC会乱码
    JL_PORTA->PU &=~(BIT(3));
    JL_PORTA->PD &=~(BIT(3));
    JL_PORTA->DIR |= BIT(3);
#endif

#if defined(_ACC_ON_LCD_BUG_)
	LCD_CE_INIT();
	LCD_DATA_INIT();
	LCD_CLK_INIT();
	LCD_CE_OUT();
	LCD_CE_LOW();
	LCD_DATA_OUT(); 
	LCD_DATA_LOW();
	LCD_CLK_OUT();
	LCD_CLK_LOW();
	setup.pt1622_flag = TRUE;
#endif

#if defined(_ENABLE_AMPLIFIER_CONTROL_)
    AMPLIFIER_INIT();
    AMPLIFIER_OUT();
    AMPLIFIER_LOW();
#endif

    AUDIO_MUTE_ON();
#if defined(_ENABLE_RCA_MUTE_)
	RCA_MUTE_ON();
#endif
#if defined(AUDIO_MUTE_SPECIAL_SET)
	HAUDIO_MUTE_ON();
#endif

#if defined(_SL6812_ILL_LIGHT_)
#if defined(_SL6812A_LIGHT_DETECT_)
	ILL_DETECT_INIT();
	ILL_DETECT_IN();
	ILL_LIGHT_INIT();
    if(ILL_DETECT_CHECK())
    {
    	ILL_LIGHT_ON();
    }
    else
    {
    	ILL_LIGHT_OFF();
    }
#else
	ILL_LIGHT_INIT();
	ILL_LIGHT_OFF();
#endif
#endif

#if defined(_VFD_DRIVER_IC_TM1729_)
	LCD_DATA_INIT();LCD_DATA_OUT(); LCD_DATA_LOW();
	LCD_CLK_INIT(); LCD_CLK_OUT();	LCD_CLK_LOW();
#endif
#if defined(_ENABLE_USB_DELAY_)||defined(_SLEEP_MODE_USB_DONT_DETCT)
	global.pwrstarttime = FALSE;
	global.usb_det_delay = 50;
#endif

#if defined(_ENABLE_POWER_PIN_)
    dac_ldo_set(LDO_1,0);
    dac_ldo_set(LDO_2,0);
#endif

	clr_PINR_ctl();

	update_check(update_flag);
#if defined(HYJ_BT_FYEQUENCY_SET)
	set_bt_osc_internal_cfg(0,0);
#elif defined(HN6651D_BT_FYEQUENCY_SET)
	set_bt_osc_internal_cfg(13,13);
#elif defined(_CY6250_BT_FYEQUENCY_SET)
	set_bt_osc_internal_cfg(0x04,0x04);
#elif defined(_CY1040_BT_FYEQUENCY_SET)
	set_bt_osc_internal_cfg(0x05,0x05);
#elif defined(_MR1093_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0D,0x0D);
#elif defined(_KD_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0E,0x0E);
#elif defined(_HC1037_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0F,0x0F);
#elif defined(_SUPPORT_TEST_BT_)
	set_bt_osc_internal_cfg(0x0A,0x0A);
#elif defined(_BJY6218_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x1F,0x1F);
#elif defined(_RC1781_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x09,0x09);
#elif defined(_KW6295_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0A,0x0A);
#elif defined(_JYL3391_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x07,0x07);
#elif defined(_HYX5209_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0A,0x0A);//样机参数
	//set_bt_osc_internal_cfg(0x1C,0x1C);//生产参数
#elif defined(_WYD5900_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x08,0x08);//样机参数	
#elif defined(_GL3011_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x10,0x10);
#elif defined(_CL2029_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0F,0x0F);
#elif defined(_BL_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x02,0x02);
#elif defined(_JYL_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x1F,0x1F);
#elif defined(_BL7890_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0C,0x0C);
#elif defined(_FC5302_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0C,0x0C);
#elif defined(_WLS316_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x10,0x10);
#elif defined(_JYL3435_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0C,0x0C);
#elif defined(_YH1701_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x09,0x09);
#elif defined(_WLS1403_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x09,0x09);
#elif defined(_GD8821_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0F,0x0F);
#elif defined(_LK6250_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x1B,0x1B);
#elif defined(_FAS3001_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x13,0x13);
#elif defined(_JYL3861E_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0E,0x0E);
#elif defined(_LK228DA_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x13,0x13);
#elif defined(_LK_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x00,0x00);
#elif defined(_JYL3800E_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0D,0x0D);
#elif defined(_JYL3669C_BT_FYEQUENCY_SET_)
	set_bt_osc_internal_cfg(0x0C,0x0C);
#else
	set_bt_osc_internal_cfg(0x11,0x11);//0x00-0x1F//+的越大，值就越大，-的越大，值就越小
#endif
    clock_init_app(SYS_CLOCK_IN, OSC_Hz, SYS_Hz);

	 JL_SYSTEM->LVD_CON &= ~(BIT(1)|BIT(0));
	 //JL_SYSTEM->LVD_CON |= BIT(0);           ////<LVD启动电压为2.2V
	 JL_SYSTEM->LVD_CON |= BIT(1);           ////<LVD启动电压为2.4V
	// JL_SYSTEM->LVD_CON |= (BIT(1)|BIT(0));    ////<LVD启动电压为2.6V

#if defined (__EW_DEBUG__)   //璁剧疆鎵撳嵃PA3
	JL_IOMAP->CON1 &= ~( BIT(11)|BIT(12)|BIT(13));
	JL_PORTB->DIR &= ~ BIT(13);
	JL_PORTB->PU |= BIT(13);
	JL_PORTB->PD |= BIT(13);

	//JL_PORTA->DIR |= BIT(1);
	//JL_PORTA->PU &= ~ BIT(1);
	//JL_PORTA->PD &= ~ BIT(1);

	/* USB_DM_PU(1); */
	/* USB_DM_PD(1); */
	/* USB_DM_DIR(0); */
#endif

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             

    for(int i = 0; i < 1200000; i++)//2020.05.21
    {
    	asm volatile ("nop");
    }

    uart_init(UART_BAUD_RAE); //<串口波特率

#if defined(TD_ADD_W2DAB_UART)
    extern s32 W2dab_uart_init(u32 baud);
    W2dab_uart_init(DAB_UART_BAUD_RAE);
	DAB_ACC=1;
#endif

#if defined(_USER_TFT_DISP_)
    extern s32 TFT_uart_init(u32 baud);
    TFT_uart_init(57600);//115200
#endif

#if defined(_SUPPORT_CAN_UART_)
    extern s32 mcu_uart_init(u32 baud);
    mcu_uart_init(115200);
#endif

#if defined(_AUDIO_DRIVER_AP3768_)
#if !defined(_ENABLE_UART_LOW_)
	extern s32 audio_uart_init(u32 baud);
	audio_uart_init(115200);
#endif
#endif

#if defined(_SUPPORT_DISC_)
	uart_interrupt_receive_init(UART_BAUD_RAE);
#endif

	board_printf("\n\ndate:%s --- time:%s\n", __DATE__, __TIME__);
	board_puts("***************************AC69_PowerUp**********************************\n");
	board_printf("SFC_BAUD = %d\n", JL_SFC->BAUD);
    board_printf("hsb_clk_hz = %u HZ\n", clock_get_sys_freq());
    board_printf("lsb_clk_hz = %u HZ\n", clock_get_lsb_freq());
	board_printf("update_flag = 0x%x\n", update_flag);
	board_printf("chip index =%x \n",get_chip_index());

//    memcpy((u8*)buf,__DATE__,sizeof(__DATE__));
	wake_io_check();

#if defined(_ENABLE_WDT_TIME_)
	open_wdt(0x0b); 		//set wd 2s
#else
	open_wdt(0x0f);			//set wd 32s
#endif
    u8 *protect_info;
    protect_info=(u8 *)0x40000;
    printf_buf(protect_info,32);
    board_printf("--JL_POWER->CON-- : 0x%x\n", JL_POWER->CON>>5);//0:PWR_ON 1:LVD 2:WDT 3:SRST 4:POWEROFF

    set_g_sys_cfg(&sys_cfg, cfg_addr);

	__asm__ volatile ("mov %0,ie0" : "=r"(tmp));
	board_printf("ie0 =%08x \n",tmp);
	__asm__ volatile ("mov %0,icfg" : "=r"(tmp));
	board_printf("icfg =%08x \n",tmp);

	/* board_printf("syd_cfg_addr=0x%x\n", cfg_addr); */
    set_sydf_header_base(sys_cfg.flash_cfg.sdfile_head_addr);

    set_cache_base(FLASH_BASE_ADDR);

    if(vm_init_api(&sys_cfg.flash_cfg))
	{
		while(1)
		{
		   board_puts("vm_init_api err\n");
		}
	}

    vm_open_all();
    bt_app_cfg();
#if defined(DISENABLE_BLE_APP)
	extern void server_advertise_disable(void);
	server_advertise_disable();
#endif
	/* 2:LDO  3:DCDC*/
	puts("-------board_main111-------\n");
	set_use_vdc15_level(VDC15_LEV);
	puts("-------board_main222-------\n");
	power_init(PWR_MODE_SELECT , SYS_Hz,0);

	/*LDO模式默认VDDIO是3.3V DCDC模式默认是3.0V*/

	puts("-------board_main333-------\n");
#if defined(sys_ldo_level_3_3v)
	set_sys_ldo_level(2); //include VDDIO && RTC_LDO   3.31v && 3.4v
#else
	set_sys_ldo_level(SYS_LDO_NORMAL_LEVEL); //include VDDIO && RTC_LDO
#endif

#if 0    //设置打印PA3
	JL_IOMAP->CON1 &= ~( BIT(11)|BIT(12)|BIT(13));
	JL_PORTA->DIR &= ~BIT(3);
	JL_PORTA->PU |= BIT(3);
	JL_PORTA->PD |= BIT(3);

	/* USB_DM_PU(1); */
	/* USB_DM_PD(1); */
	/* USB_DM_DIR(0); */
#endif

	puts("-------board_main444-------\n");
	set_poweroff_wakeup_io_handle_register(set_poweroff_wakeup_io,set_sleep_mode_wakeup_io,set_sleep_before_close_irq);

	puts("-------board_main555-------\n");
	if(device_is_first_start()||device_is_updata_over())
	{
		board_puts("device first_start or updata_over\n");
	}

	puts("-------board_main666-------\n");
    timer0_init();
    timer2_init();
#if defined(_SUPPORT_PANEL_PWM_)||defined(_ENABLE_TIME3_COUNT_)
#if !defined(_ENABLE_ALARM_CLOCK_)
	timer3_init();
#endif
#endif

#if defined(_ENABLE_TIME3_DELAY_)
	extern void delay_1ms();
	delay_1ms();
#endif

#if 0
	PORTR_PU(PORTR0,0);
	PORTR_PD(PORTR0,0);
	PORTR_DIR(PORTR0,1);
	PORTR_DIE(PORTR0,1);
	PORTR_HD(PORTR0,1);
#endif

    sys_init();

	puts("-------sys_init_succeed-------\n");
	
#if defined(_MUSICID3_ADJUST_)
	irq_handler_register(0,exception_isr,0);
#endif
	/* enter_sleep_mode(); */


    main_task_init();
    OSStart();
    irq_global_enable();
    __asm__ volatile ("mov %0,ie0" : "=r"(tmp));
    board_printf("ie0 =%08x \n",tmp);
    __asm__ volatile ("mov %0,ie1" : "=r"(tmp));
    board_printf("ie1 =%08x \n",tmp);
	__asm__ volatile ("mov %0,icfg" : "=r"(tmp));
    board_printf("icfg =%08x \n",tmp);
#if defined (REDUCE_24M_OUT_RANGE)
	SFR(WLA_CON17, 10, 4, 0x2);   //osc HCS 默认为0x8:24M晶体输出幅度为288mv    0xc:24M晶体输出幅度为532mv
#elif defined(HZS_REDUCE_24M_OUT_RANGE)
    SFR(WLA_CON17, 10, 4, 0xf);   //osc HCS 默认为0x8:24M晶体输出幅度为288mv	0xc:24M晶体输出幅度为532mv
#else
	SFR(WLA_CON17, 10, 4, 0xc);   //osc HCS 默认为0x8:24M晶体输出幅度为288mv    0xc:24M晶体输出幅度为532mv
#endif

    while(1)
    {
        __asm__ volatile ("idle");
        __asm__ volatile ("nop");
        __asm__ volatile ("nop");
    }
}
volatile unsigned long jiffies;


