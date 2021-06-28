#include "sdk_cfg.h"
#include "app_cfg.h"
#include "error.h"
#include "stdarg.h"
#include "msg.h"
#include "disc/disc.h"
#include "disc/disc_order.h"
#include "disc/disc_state.h"
#include "disc_drv/disc_drv.h"

#include "dev_manage/dev_ctl.h"

#if defined(ENABLE_CHANGE_WELCOME_LOGO)
#include "setup.h"
#endif

#if defined(_SUPPORT_DISC_)

static PLAYER_STATE_t *player_state = NULL;
extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

static uint8 disc_data_receive(uint8 *buffer, uint8 *length)
{
	uint8 result = FALSE;
	
	if(disc_servo_receive(buffer, length))
	{
		if(*length)
		{
			*length = DecodeData(buffer, *length);
			if(*length)
			{
				result = TRUE;
			}
		}
	}
	
	return result;
}

static void servo_state_status(uint8 *buffer, uint8 length)
{
	if(!player_state->normal)
	{
		player_state->normal = TRUE;
		os_taskq_post_msg(player_state->name, 1, SYS_EVENT_SERVO_NORMAL);
		//disc_printf("normal:%d\n", player_state->normal);
	}

	uint8 esp_flag = (uint8)GET_SERVO_STATUS_ESP(buffer);
	if(esp_flag != player_state->esp_flag)
	{
		player_state->esp_flag = esp_flag;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_ESP, player_state->esp_flag);
		//disc_printf("esp_flag:%d\n", esp_flag);
	}

	SERVO_SOURCE_e source = (SERVO_SOURCE_e)GET_SERVO_STATUS_SOURCE(buffer);		
	if(source != player_state->source)
	{
		player_state->source = source;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_SOURCE, player_state->source);
		//disc_printf("source:%d\n", source);
	}

	SERVO_STATUS_e status = (SERVO_STATUS_e)GET_SERVO_STATUS_STATUS(buffer);
	if(status != player_state->status)
	{
		player_state->status = status;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_STATUS, player_state->status);
		//disc_printf("status:%d\n", status);
	}

	SERVO_MODE_e mode = (SERVO_MODE_e)GET_SERVO_STATUS_MODE(buffer);
	if(mode != player_state->mode)
	{
		player_state->mode = mode;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_MODE, player_state->mode);
		//disc_printf("mode:%d\n", mode);
	}

	SERVO_SEARCH_MODE_e search_mode = (SERVO_SEARCH_MODE_e)GET_SERVO_STATUS_SEARCH_MODE(buffer);
	if(search_mode != player_state->search_mode)
	{
		player_state->search_mode = search_mode;
		player_state->search_status = 0xFF;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_SEARCH_MODE, player_state->search_mode);
		//disc_printf("search_mode:%d\n", search_mode);
	}

	uint8 search_status = (uint8)GET_SERVO_STATUS_SEARCH_STATUS(buffer);
	if(search_status != player_state->search_status)
	{
		player_state->search_status = search_status;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_SEARCH_STATUS, player_state->search_status);
		//disc_printf("search_status_init:%d\n", search_status);
	}

	SERVO_MEDIA_e media = (SERVO_MEDIA_e)GET_SERVO_STATUS_MEDIA(buffer);
	if(media != player_state->media)
	{
		player_state->media = media;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_MEDIA, player_state->media);
		//disc_printf("media:%d\n", media);
	}
	
	SERVO_ERROR_e error = (SERVO_ERROR_e)GET_SERVO_STATUS_ERROR(buffer);
#if !defined(SRVR_ERROR_TURN_MODE)
	if(error != player_state->error)
#endif
	{
		player_state->error = error;
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_ERROR, player_state->error);
		//disc_printf("error:%d\n", error);
	}
}

static void servo_state_order(uint8 *buffer, uint8 length)
{
	disc_printf("%s, length:%d--[0]:%d,[1]:%d,[2]:%d\n", __FUNCTION__, length,buffer[0],buffer[1],buffer[2]);
#if defined(ENABLE_CHANGE_WELCOME_LOGO)
	switch(buffer[0])
	{
		case SERVO_DISP_LOGO_FLAG:
			setup.LogoFlag = buffer[1];
			break;
		default:
			break;
	}
#endif
	if(!player_get_normal())return;	
}

static void servo_state_vfd(uint8 *buffer, uint8 length)
{
	if(!player_get_normal())return;
	
	if(memcmp(player_state->vfd_buffer, buffer, length))
	{
		memcpy(player_state->vfd_buffer, buffer, length);
		//disc_printf("vfd:%c%c%c%c%c%c%c%c\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
		os_taskq_post_msg(player_state->name, 2, SYS_EVENT_SERVO_VFD, player_state->vfd_buffer);
	}
}

void disc_state_receive(void)
{
	//ew_puts("disc_state_receive\n");

	if(player_state == NULL)return;
	
	uint8 length = 30;
	uint8 buffer[length];
	while(disc_data_receive(buffer, &length))
	{
	#if 0
	{
		int i;
		for(i = 0; i < length; i++)
		{
			disc_printf("data[%d]=0x%02x\n", i, buffer[i]);
		}
	}
	#endif
		
		if(length > 1)
		{
			//ew_printf("buffer:%d\n",buffer[0]);
			switch(buffer[0])
			{
				case SERVO_TYPE_STATUS:
					servo_state_status(buffer + 1, length - 1);
					break;
				case SERVO_TYPE_ORDER:
					servo_state_order(buffer + 1, length - 1);
					break;
				case SERVO_TYPE_VFD:
					servo_state_vfd(buffer + 1, length - 1);
					break;
			#if defined(TD_ADD_UPDATA)
				case SERVO_TYPE_UPDATA:
					os_taskq_post_msg("MainTask",3,MSG_UPDATA,UART_UPDATA,NULL);
					break;
			#endif
			}
		}
	}
}

void disc_state_init(char *name)
{
	if(player_state == NULL)
	{
		player_state = (PLAYER_STATE_t *)malloc_fun(player_state, sizeof(PLAYER_STATE_t), 0);
		if(player_state != NULL)
		{
			player_state->name = name;
		}
	}
}

void disc_state_exit(void)
{
	if(player_state != NULL)
	{
		free_fun((void **)&player_state);
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/

uint8 player_get_normal(void)
{
	if(player_state != NULL)
	{
		return player_state->normal;
	}
	return FALSE;
}

#endif

