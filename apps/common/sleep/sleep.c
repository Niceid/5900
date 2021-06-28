#include "sdk_cfg.h"
#include "rtc.h"
#include "sys_detect.h"
#include "dac/dac.h"
#include "sleep.h"


#define SLEEP_DEBUG
#ifdef SLEEP_DEBUG
#define sleep_printf 	ew_printf
#define sleep_puts 		ew_puts
#else
#define sleep_printf(...)
#define sleep_puts(...)
#endif


enum{
	WAKEUP_0 = 0,
	WAKEUP_1,
	WAKEUP_2,
	WAKEUP_3,
	WAKEUP_4,
	WAKEUP_5,
	WAKEUP_6,
	WAKEUP_7,
	WAKEUP_8,
	WAKEUP_9,
	WAKEUP_10,
	WAKEUP_11,
	WAKEUP_12,
	WAKEUP_13,
	WAKEUP_14,
	WAKEUP_15,
};

#define WAKEUP_UP_EDGE				0x0F
#define WAKEUP_DOWN_EDGE			0xF0

extern void soft_poweroff(void);
extern void set_keep_osci_flag(uint8);

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*enter sleep mode wakeup IO setting*/
static void enter_sleep_mode_set(u16 wakeup_cfg , u8 wakeup_edge)
{
	close_wdt();

	dac_off_control(); //close dac mudule

    JL_AUDIO->LADC_CON = 0;
    JL_AUDIO->ADA_CON0 = 0;
    JL_AUDIO->ADA_CON1 = 0;
    JL_AUDIO->ADA_CON2 = 0;

	JL_AUDIO->DAA_CON0 = 0;
	JL_AUDIO->DAA_CON1 = 0;
	JL_AUDIO->DAA_CON2 = 0;
	JL_AUDIO->DAA_CON3 = 0;
	JL_AUDIO->DAA_CON4 = 0;
	JL_AUDIO->DAA_CON5 = 0;

	JL_WAKEUP->CON0 = 0;      //wakeup enbale
	JL_WAKEUP->CON1 = 0;      //wakeup edge
	JL_WAKEUP->CON2 = 0xffff; //wakeup pending(clear)

	switch(wakeup_cfg)
	{
		case WAKEUP_0:
			break;

		case WAKEUP_1:
			break;

		case WAKEUP_2:
			sleep_puts("WAKEUP_2\n");
			JL_WAKEUP->CON2 |= BIT(2);
			JL_PORTA->DIR |= BIT(8);
			JL_PORTA->DIE |= BIT(8);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTA->PU |= BIT(8);
				JL_PORTA->PD &= ~BIT(8);
				JL_WAKEUP->CON1 |= BIT(2);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTA->PU &= ~BIT(8);
				JL_PORTA->PD |= BIT(8);
				JL_WAKEUP->CON1 &= ~BIT(2);
			}
			JL_WAKEUP->CON0 |= BIT(2);
			break;

		case WAKEUP_3:
			sleep_puts("WAKEUP_3\n");
			JL_WAKEUP->CON2 |= BIT(3);
			JL_PORTA->DIR |= BIT(10);
			JL_PORTA->DIE |= BIT(10);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTA->PU |= BIT(10);
				JL_PORTA->PD &= ~BIT(10);
				JL_WAKEUP->CON1 |= BIT(3);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTA->PU &= ~BIT(10);
				JL_PORTA->PD |= BIT(10);
				JL_WAKEUP->CON1 &= ~BIT(3);
			}
			JL_WAKEUP->CON0 |= BIT(3);
			break;

		case WAKEUP_4:
			sleep_puts("WAKEUP_4\n");
			JL_WAKEUP->CON2 |= BIT(4);
			JL_PORTA->DIR |= BIT(14);
			JL_PORTA->DIE |= BIT(14);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTA->PU |= BIT(14);
				JL_PORTA->PD &= ~BIT(14);
				JL_WAKEUP->CON1 |= BIT(4);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTA->PU &= ~BIT(14);
				JL_PORTA->PD |= BIT(14);
				JL_WAKEUP->CON1 &= ~BIT(4);
			}
			JL_WAKEUP->CON0 |= BIT(4);
			break;
		case WAKEUP_5:
			break;
		case WAKEUP_6:
			break;
		case WAKEUP_7:
			break;
		case WAKEUP_8:
			sleep_puts("WAKEUP_8\n");
			JL_WAKEUP->CON2 |= BIT(8);
			JL_PORTA->DIR |= BIT(3);
			JL_PORTA->DIE |= BIT(3);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTA->PU |= BIT(3);
				JL_PORTA->PD &= ~BIT(3);
				JL_WAKEUP->CON1 |= BIT(8);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTA->PU &= ~BIT(3);
				JL_PORTA->PD |= BIT(3);
				JL_WAKEUP->CON1 &= ~BIT(8);
			}
			JL_WAKEUP->CON0 |= BIT(8);
			break;
		case WAKEUP_9:
			sleep_puts("WAKEUP_9\n");
			JL_WAKEUP->CON2 |= BIT(9);
			JL_PORTA->DIR |= BIT(5);
			JL_PORTA->DIE |= BIT(5);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTA->PU |= BIT(5);
				JL_PORTA->PD &= ~BIT(5);
				JL_WAKEUP->CON1 |= BIT(9);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTA->PU &= ~BIT(5);
				JL_PORTA->PD |= BIT(5);
				JL_WAKEUP->CON1 &= ~BIT(9);
			}
			JL_WAKEUP->CON0 |= BIT(9);
			break;
		case WAKEUP_10:
			sleep_puts("WAKEUP_10\n");
			JL_WAKEUP->CON2 |= BIT(10);
			JL_PORTB->DIR |= BIT(0);
			JL_PORTB->DIE |= BIT(0);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTB->PU |= BIT(0);
				JL_PORTB->PD &= ~BIT(0);
				JL_WAKEUP->CON1 |= BIT(10);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTB->PU &= ~BIT(0);
				JL_PORTB->PD |= BIT(0);
				JL_WAKEUP->CON1 &= ~BIT(10);
			}
			JL_WAKEUP->CON0 |= BIT(10);
			break;
		case WAKEUP_11:
			sleep_puts("WAKEUP_11\n");
			JL_WAKEUP->CON2 |= BIT(11);
			JL_PORTB->DIR |= BIT(2);
			JL_PORTB->DIE |= BIT(2);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTB->PU |= BIT(2);
				JL_PORTB->PD &= ~BIT(2);
				JL_WAKEUP->CON1 |= BIT(11);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTB->PU &= ~BIT(2);
				JL_PORTB->PD |= BIT(2);
				JL_WAKEUP->CON1 &= ~BIT(11);
			}
			JL_WAKEUP->CON0 |= BIT(11);
			break;
		case WAKEUP_12:
			sleep_puts("WAKEUP_12\n");
			JL_WAKEUP->CON2 |= BIT(12);
			JL_PORTB->DIR |= BIT(9);
			JL_PORTB->DIE |= BIT(9);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTB->PU |= BIT(9);
				JL_PORTB->PD &= ~BIT(9);
				JL_WAKEUP->CON1 |= BIT(12);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTB->PU &= ~BIT(9);
				JL_PORTB->PD |= BIT(9);
				JL_WAKEUP->CON1 &= ~BIT(12);
			}
			JL_WAKEUP->CON0 |= BIT(12);
			break;
		case WAKEUP_13:
			sleep_puts("WAKEUP_13\n");
			JL_WAKEUP->CON2 |= BIT(13);
			JL_PORTB->DIR |= BIT(11);
			JL_PORTB->DIE |= BIT(11);
			if(wakeup_edge == WAKEUP_DOWN_EDGE)
			{
				sleep_puts("WAKEUP_DOWN_EDGE\n");
				JL_PORTB->PU |= BIT(11);
				JL_PORTB->PD &= ~BIT(11);
				JL_WAKEUP->CON1 |= BIT(13);
			}
			else
			{
				sleep_puts("WAKEUP_UP_EDGE\n");
				JL_PORTB->PU &= ~BIT(11);
				JL_PORTB->PD |= BIT(11);
				JL_WAKEUP->CON1 &= ~BIT(13);
			}
			JL_WAKEUP->CON0 |= BIT(13);
			break;
		case WAKEUP_14:
			break;
		case WAKEUP_15:
			break;

		default:
			return;
	}
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
#if defined(_enable_rtc_sleep_if_aganin_)
u8 rtc_sleep_flag=0;
#endif

void set_poweroff_wakeup_io(void)
{
	ew_red_printf("%s\n", __FUNCTION__);

	uint8 io = 0;
	uint8 edge  = 0;
	
#if defined(_enable_rtc_sleep_if_aganin_) //进入深度rtc睡眠后会在调用一次，减少acc唤醒不了的概率  
    if(rtc_sleep_flag==1)
    {
        rtc_sleep_flag=0;
        if(ACC_CHECK())
        {
            JL_POWER->CON |= BIT(4); //reset
        }
    }
#endif
	if(ACC_GPIO == PORTR0)io |= WAKE_UP_PR0;
	if(ACC_GPIO == PORTR1)io |= WAKE_UP_PR1;
	if(ACC_GPIO == PORTR2)io |= WAKE_UP_PR2;
	if(ACC_GPIO == PORTR3)io |= WAKE_UP_PR3;

	if(ACC_GPIO == PORTR0)edge |= EDGE_PR0;
	if(ACC_GPIO == PORTR1)edge |= EDGE_PR1;
	if(ACC_GPIO == PORTR2)edge |= EDGE_PR2;
	if(ACC_GPIO == PORTR3)edge |= EDGE_PR3;
     
	soft_poweroff_wakeup_io(io,edge);
}

/*sleep mode wakeup io setting fuction*/
void set_sleep_mode_wakeup_io(void)
{
	ew_red_printf("%s\n", __FUNCTION__);
	enter_sleep_mode_set(WAKEUP_3,WAKEUP_DOWN_EDGE);
}

/*sleep mode before close IRQ callback fuction*/
void set_sleep_before_close_irq(void)
{
	ew_red_printf("%s\n", __FUNCTION__);
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

void system_lowpower_api(void)
{
 #if defined(_enable_rtc_sleep_if_aganin_)
    rtc_sleep_flag=1;
 #endif
	ew_red_printf("%s ---> SLEEP\n", __FUNCTION__);
	set_keep_osci_flag(1);
	soft_poweroff();
	ew_red_printf("%s ---> WAKE UP\n", __FUNCTION__);
}

void system_reset_api(void)
{
	// ACC 系统复位
	ew_red_printf("%s\n", __FUNCTION__);
	JL_POWER->CON |= BIT(4); //reset
}


