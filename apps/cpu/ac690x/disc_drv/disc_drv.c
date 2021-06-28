#include "includes.h"
#include "msg.h"
#include "disp.h"
#include "disc_drv/disc_drv.h"
#include "disc/disc.h"
#include "disc/disc_order.h"
#if defined(_OPEN_SYS_POWER_WHEN_DISC_IN_OUT)
#include "setup.h"
#endif
#include "key_drv_ad.h"
#include "setup.h"

#if defined(_SUPPORT_DISC_)

DISC_DRV_t disc_drv;

#if defined(_NO_MOVEMENT_ONCE_MOVE_)
static  u8 movement_flag;
#endif

static void DiscOutputPower(uint8 power)
{
	ew_puts("DiscOutputPower\n");
	disc_drv.power = power;
	if(power)
	{
	#if defined(_ENABLE_DISC_EJECT_)
		if(DETECT_DISC_CHECK())
		{
			DISC_POWER_ON();
		}
		else
		{
			if(DetectDiscIn())
			{
				DISC_POWER_ON();
			}
		}
	#else
		DISC_POWER_ON();
	#endif
	}
	else
	{
		if(!disc_drv.servo_work)
		{
			DISC_POWER_OFF();
		}
	}
}

static void DiscOutputState(DISC_OUTPUT_e state)
{
	ew_puts("DiscOutputState\n");
	switch(state)
	{
		case DISC_OUTPUT_IDLE:
			DISC_OPEN_OFF();
			DISC_CLOSE_OFF();
			DiscOutputPower(DISABLE);
			break;
		case DISC_OUTPUT_OPEN:
			DISC_OPEN_ON();
			DISC_CLOSE_OFF();
			DiscOutputPower(ENABLE);
			break;
		case DISC_OUTPUT_CLOSE:
			DISC_OPEN_OFF();
			DISC_CLOSE_ON();
			DiscOutputPower(ENABLE);
			break;
		case DISC_OUTPUT_LOCK:
			DISC_OPEN_ON();
			DISC_CLOSE_ON();
			DiscOutputPower(ENABLE);
			break;
	}
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

static void DiscIdleInit(void)
{
	//ew_puts("DiscIdleInit\n");
	memset(&disc_drv.idle, 0, sizeof(disc_drv.idle));

	disc_drv.idle.detect_in 	= DetectDiscIn();
	disc_drv.idle.detect_out 	= DetectDiscOut();
	disc_drv.idle.detect_stop 	= DetectDiscStop();
	disc_drv.idle.time = (15000/DISC_POLLING_TIME);
}

static void DiscIdleRun(void)
{
	//ew_puts("DiscIdleRun\n");
#if defined(_NO_MOVEMENT_ONCE_MOVE_)
	if((!DetectDiscStop())
		&&!movement_flag)
#else
	if(!DetectDiscStop())
#endif
	{
		if(DetectDiscIn() != disc_drv.idle.detect_in)
		{
			if(DetectDiscIn())
			{
				disc_drv.auto_run = FALSE;
				disc_drv.next_state = DISC_STATE_CLOSE;
			}
		}
		else if(DetectDiscOut() != disc_drv.idle.detect_out)
		{
			if(DetectDiscOut())
			{
				disc_drv.auto_run = FALSE;
				disc_drv.next_state = DISC_STATE_CLOSE;
			}
		}
		else
		{
			if(DetectDiscIn() || DetectDiscOut())
			{
				if((!disc_drv.frist)&& disc_drv.idle.time)disc_drv.idle.time--;
				else
				{
					disc_drv.auto_run = TRUE;
					disc_drv.next_state = DISC_STATE_CLOSE;
				}
			}
		}
	}

	disc_drv.idle.detect_in 	= DetectDiscIn();
	disc_drv.idle.detect_out 	= DetectDiscOut();
	disc_drv.idle.detect_stop 	= DetectDiscStop();
}

static void DiscIdleExit(void)
{
	disc_drv.frist = FALSE;

	//ew_puts("DiscIdleExit\n");
}

/*-----------------------------------------------------------------------------------------------------------------------*/

static void DiscOpenInit(void)
{
	//ew_puts("DiscOpenInit\n");
	memset(&disc_drv.open, 0, sizeof(disc_drv.open));

	if(!disc_drv.eject)
	{
    	disc_drv.open.time = (10000/DISC_POLLING_TIME);//10000
		DiscOutputState(DISC_OUTPUT_OPEN);
	#if defined(_OPEN_SYS_POWER_WHEN_DISC_IN_OUT)
		if(!global.sys_power)
		{
			SYS_POW_ON();
		}
	#endif
	}
	else
	{
		disc_drv.open.time = (3000/DISC_POLLING_TIME);
	}
}

static void DiscOpenRun(void)
{
	//ew_puts("DiscOpenRun\n");
	if(disc_drv.eject)
	{
		if(disc_drv.open.time)
		{
			disc_drv.open.time--;
		}
		else
		{
			DiscOutputState(DISC_OUTPUT_OPEN);
			disc_drv.open.time = (10000/DISC_POLLING_TIME);//10000
			disc_drv.eject = FALSE;
		}
	}
	else
	{
		if(disc_drv.open.time)
		{
			disc_drv.open.time--;
			if(!DetectDiscOut())
			{
				disc_drv.next_state = DISC_STATE_IDLE;
			}
		}
		else
		{
			disc_drv.next_state = DISC_STATE_IDLE;
		}
	}
}

static void DiscOpenExit(void)
{
	//ew_puts("DiscOpenExit\n");
	DiscOutputState(DISC_OUTPUT_IDLE);
	disc_drv.frist = FALSE;

#if defined(_OPEN_SYS_POWER_WHEN_DISC_IN_OUT)
	if(!global.sys_power)
	{
		SYS_POW_OFF();
	}
#endif
	if(disc_drv.eject_disp_flag)
	{
		disp_send_param(DISP_EVENT_EJECT, FALSE);
		disc_drv.eject_disp_flag = FALSE;
	}
}

/*-----------------------------------------------------------------------------------------------------------------------*/

static void DiscCloseInit(void)
{
	//ew_puts("DiscCloseInit\n");
	memset(&disc_drv.close, 0, sizeof(disc_drv.close));
	disc_drv.close.time = (10000/DISC_POLLING_TIME);
	disc_drv.close.plase_delay = (150/DISC_POLLING_TIME);
#if defined(_OPEN_SYS_POWER_WHEN_DISC_IN_OUT)
	if(!global.sys_power)
	{
		SYS_POW_ON();
	}
#endif
#if defined(WHEN_DISC_IN_MUTE)
	global.disc_mute_flag =	TRUE;
#endif
	DiscOutputState(DISC_OUTPUT_CLOSE);
}

static void DiscCloseRun(void)
{
	//ew_puts("DiscCloseRun\n");
	if(disc_drv.close.time && DetectDiscIn())
	{
	#if defined(WHEN_DISC_IN_MUTE)
		global.disc_mute_flag =	TRUE;
	#endif

		disc_drv.close.time--;
		if(DetectDiscStop())
		{
			if(disc_drv.close.plase_delay)disc_drv.close.plase_delay--;
			else
			{
				disc_drv.next_state = DISC_STATE_IDLE;
			}
		}
		else
		{
			disc_drv.close.plase_delay = (150/DISC_POLLING_TIME);
		}
	}
	else
	{
	#if defined(_NO_MOVEMENT_ONCE_MOVE_)
		if(!disc_drv.close.time)
			movement_flag = TRUE;
	#endif
		disc_drv.next_state = DISC_STATE_IDLE;
	}
}

static void DiscCloseExit(void)
{
	//ew_puts("DiscCloseExit\n");
	DiscOutputState(DISC_OUTPUT_IDLE);
	disc_drv.frist = FALSE;
#if defined(_OPEN_SYS_POWER_WHEN_DISC_IN_OUT)
	if(!global.sys_power)
	{
		SYS_POW_OFF();
	}
#endif
}

/*-----------------------------------------------------------------------------------------------------------------------*/

static void DiscNoDiscInit(void)
{
	//ew_puts("DiscNoDiscInit\n");
	memset(&disc_drv.no_disc, 0, sizeof(disc_drv.no_disc));
	disc_drv.no_disc.time = (3000/DISC_POLLING_TIME);
	DiscOutputState(DISC_OUTPUT_OPEN);
}

static void DiscNoDiscRun(void)
{
	//ew_puts("DiscNoDiscRun\n");
	if(disc_drv.no_disc.time)
	{
		disc_drv.no_disc.time--;
		if(DetectDiscIn())
		{
			disc_drv.auto_run = FALSE;
			disc_drv.next_state = DISC_STATE_CLOSE;
		}
	}
	else
	{
		disc_drv.next_state = DISC_STATE_IDLE;
	}
}

static void DiscNoDiscExit(void)
{
	//ew_puts("DiscNoDiscExit\n");
	DiscOutputState(DISC_OUTPUT_IDLE);
	disc_drv.frist = FALSE;
#if defined(NEED_NO_DISC_DISP)
	if(disc_drv.eject_disp_flag)
	{
		disp_send_param(DISP_EVENT_NO_DISC, FALSE);
		disc_drv.eject_disp_flag = FALSE;
	}
#endif
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

static const DISC_MODE_FUNC_t DiscStateFunc[]=
{
	{	DiscIdleInit,		DiscIdleRun,		DiscIdleExit		},
	{	DiscOpenInit,		DiscOpenRun,		DiscOpenExit		},
	{	DiscCloseInit,		DiscCloseRun,		DiscCloseExit		},
	{	DiscNoDiscInit,		DiscNoDiscRun,		DiscNoDiscExit		},
};

void disc_drv_init(void)
{
	memset(&disc_drv, 0, sizeof(disc_drv));
	disc_drv.time = 2000/DISC_POLLING_TIME;
}

void disc_drv_key_open(void)
{
	uint8 eject_flag = FALSE;
	
	if(global.disc_open_flag)
	{
		ew_puts("disc_drv_key333\n");
		global.disc_time++;
		if(global.disc_time > 1500/DISC_POLLING_TIME)// 1.5Ãë
		{
			global.disc_open_flag = FALSE;
			global.disc_time = 0;

    		switch(disc_drv.state)
    		{
    			case DISC_STATE_IDLE:
    				if(DetectDiscStop() && DetectDiscIn())
    				{
    					disc_drv.next_state = DISC_STATE_OPEN;
    					if(disc_drv.eject_func != NULL)
    					{
    						disc_drv.eject = disc_drv.eject_func(TRUE);
    					}
    					eject_flag = TRUE;
    				}
    				else if(DetectDiscIn())
    				{
    					disc_drv.auto_run = FALSE;
    					disc_drv.next_state = DISC_STATE_CLOSE;
    		
    				}
    				else
    				{
    				#if !defined(_ENABLE_REJECT_)
    					disc_drv.next_state = DISC_STATE_NO_DISC;
    				#endif
    				
    				#if defined(NEED_NO_DISC_DISP)
    					eject_flag = TRUE;
    				#endif
    				}
    				break;
    		
    			case DISC_STATE_OPEN:
    				if(DetectDiscIn())
    				{
    					disc_drv.next_state = DISC_STATE_CLOSE;
    				}
    				else
    				{
    					disc_drv.next_state = DISC_STATE_NO_DISC;
    				}
    				break;
    		
    			case DISC_STATE_CLOSE:
    				if(DetectDiscIn())
    				{
    					disc_drv.next_state = DISC_STATE_OPEN;
    					eject_flag = TRUE;
    				}
    				else
    				{
    					disc_drv.next_state = DISC_STATE_NO_DISC;
    				}
    				break;
    		
    			case DISC_STATE_NO_DISC:
    				if(DetectDiscIn())
    				{
    					disc_drv.next_state = DISC_STATE_CLOSE;
    				}
    				break;
    			}
    		}
	}
	else
	{
		//ew_puts("disc_drv_key444\n");
		if(global.disc_count == 5)
		{
			ew_puts("disc_drv_key555\n");
			disc_order_send(SERVO_ORDER_EJECT);
			global.disc_count = 0;
		}
		global.disc_time = 0;
	}

	if(eject_flag && strcmp(curr_task->name,"IdleTask"))
	{
	#if defined(NEED_NO_DISC_DISP)
		if(disc_drv.next_state == DISC_STATE_NO_DISC)
		{
			ew_printf("next_state:DISC_STATE_NO_DISC\n");
			disp_send_param(DISP_EVENT_NO_DISC, TRUE);
		}
		else
	#endif
		{
			disp_send_param(DISP_EVENT_EJECT, TRUE);
		}
		disc_drv.eject_disp_flag = TRUE;
		
		global.disc_open = FALSE;
	}

}

void disc_drv_process(void)
{
	disc_servo_process();

	if(global.disc_open)
	{
		disc_drv_key_open();
	}

	if(disc_drv.disable)return;
	if(disc_drv.time)
	{
		disc_drv.time--;
		return;
	}

	if(disc_drv.next_state != disc_drv.state)
	{
		disc_drv.mode = DISC_MODE_EXIT;
	}

_DISC_STATE_AGAIN:
	switch(disc_drv.mode)
	{
		case DISC_MODE_INIT:
			DiscStateFunc[disc_drv.state].init();
			disc_drv.mode = DISC_MODE_RUN;
			goto _DISC_STATE_AGAIN;

		case DISC_MODE_RUN:
			DiscStateFunc[disc_drv.state].run();
			break;

		case DISC_MODE_EXIT:
			DiscStateFunc[disc_drv.state].exit();
			disc_drv.state = disc_drv.next_state;
			disc_drv.mode = DISC_MODE_INIT;
			goto _DISC_STATE_AGAIN;
	}
}

void disc_drv_key(void)
{
	ew_printf("%s, now_state:%d\n", __FUNCTION__, disc_drv.state);

	uint8 eject_flag = FALSE;

#if defined(_SPECIAL_EJECT_KEY_)
	if(!DETECT_DISC_CHECK())
	{
		return;
	}
#endif

	if(compare_task_name(PLAYER_DISC_TASK_NAME))
	{
		ew_puts("disc_drv_key111\n");
		global.disc_open = TRUE;
		disc_order_send(SERVO_ORDER_EJECT);
	}
	else
    {
    	switch(disc_drv.state)
    	{
    		case DISC_STATE_IDLE:
    			if(DetectDiscStop() && DetectDiscIn())
    			{
    				disc_drv.next_state = DISC_STATE_OPEN;
    				if(disc_drv.eject_func != NULL)
    				{
    					disc_drv.eject = disc_drv.eject_func(TRUE);
    				}
    				eject_flag = TRUE;
    			}
    			else if(DetectDiscIn())
    			{
    				disc_drv.auto_run = FALSE;
    				disc_drv.next_state = DISC_STATE_CLOSE;
    			}
    			else
    			{
    			#if !defined(_ENABLE_REJECT_)
    				disc_drv.next_state = DISC_STATE_NO_DISC;
    			#endif
    			
    			#if defined(NEED_NO_DISC_DISP)
    				eject_flag = TRUE;
    			#endif
    			}
    			break;
    	
    		case DISC_STATE_OPEN:
    			if(DetectDiscIn())
    			{
    				disc_drv.next_state = DISC_STATE_CLOSE;
    			}
    			else
    			{
    				disc_drv.next_state = DISC_STATE_NO_DISC;
    			}
    			break;
    	
    		case DISC_STATE_CLOSE:
    			if(DetectDiscIn())
    			{
    				disc_drv.next_state = DISC_STATE_OPEN;
    				eject_flag = TRUE;
    			}
    			else
    			{
    				disc_drv.next_state = DISC_STATE_NO_DISC;
    			}
    			break;
    	
    		case DISC_STATE_NO_DISC:
    			if(DetectDiscIn())
    			{
    				disc_drv.next_state = DISC_STATE_CLOSE;
    			}
    			break;
    	}
		
		if(eject_flag && strcmp(curr_task->name,"IdleTask"))
		{
		#if defined(NEED_NO_DISC_DISP)
			if(disc_drv.next_state == DISC_STATE_NO_DISC)
			{
				ew_printf("next_state:DISC_STATE_NO_DISC\n");
				disp_send_param(DISP_EVENT_NO_DISC, TRUE);
			}
			else
		#endif
			{
				disp_send_param(DISP_EVENT_EJECT, TRUE);
			}
			disc_drv.eject_disp_flag = TRUE;
		}
	}
}

#if defined(_ENABLE_REJECT_)
void disc_drv_key1(void)
{
	ew_printf("%s, now_state:%d\n", __FUNCTION__, disc_drv.state);

	uint8 eject_flag = FALSE;
	if(compare_task_name(PLAYER_DISC_TASK_NAME))
	{
		ew_puts("disc_drv_key111\n");
		global.disc_open = TRUE;
		disc_order_send(SERVO_ORDER_EJECT);
	}
	else
	{
		switch(disc_drv.state)
		{
			case DISC_STATE_IDLE:
				if(DetectDiscStop() && DetectDiscIn())
				{
					disc_drv.next_state = DISC_STATE_OPEN;
					if(disc_drv.eject_func != NULL)
					{
						disc_drv.eject = disc_drv.eject_func(TRUE);
					}
					eject_flag = TRUE;
				}
				else if(DetectDiscIn())
				{
					//disc_drv.auto_run = FALSE;
					//disc_drv.next_state = DISC_STATE_CLOSE;
				}
				else
				{
					disc_drv.next_state = DISC_STATE_NO_DISC;
				#if defined(NEED_NO_DISC_DISP)
					eject_flag = TRUE;
				#endif
				}
				break;
		
			case DISC_STATE_OPEN:
				if(DetectDiscIn())
				{
					disc_drv.next_state = DISC_STATE_CLOSE;
				}
				else
				{
					disc_drv.next_state = DISC_STATE_NO_DISC;
				}
				break;
		
			case DISC_STATE_CLOSE:
				if(DetectDiscIn())
				{
					disc_drv.next_state = DISC_STATE_OPEN;
					eject_flag = TRUE;
				}
				else
				{
					disc_drv.next_state = DISC_STATE_NO_DISC;
				}
				break;
		
			case DISC_STATE_NO_DISC:
				if(DetectDiscIn())
				{
					disc_drv.next_state = DISC_STATE_CLOSE;
				}
				break;
		}
		
		if(eject_flag && strcmp(curr_task->name,"IdleTask"))
		{
		#if defined(NEED_NO_DISC_DISP)
			if(disc_drv.next_state == DISC_STATE_NO_DISC)
			{
				ew_printf("next_state:DISC_STATE_NO_DISC\n");
				disp_send_param(DISP_EVENT_NO_DISC, TRUE);
			}
			else
		#endif
			{
				disp_send_param(DISP_EVENT_EJECT, TRUE);
			}
			disc_drv.eject_disp_flag = TRUE;
		}
	}
}
#endif

void disc_drv_disable(void)
{
	if(!disc_drv.disable)
	{
		disc_drv.disable = TRUE;
		if(disc_drv.state != DISC_STATE_IDLE)
		{
			DiscStateFunc[disc_drv.state].exit();
		}
	}
}

uint8 disc_drv_auto(uint8 reset)
{
	uint8 auto_run = disc_drv.auto_run;
	if(reset)
	{
		disc_drv.auto_run = FALSE;
	}
	return auto_run;
}

void disc_drv_eject(U_FUNC_U func)
{
	disc_drv.eject_func = func;
}

#endif

