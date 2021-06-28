#include "sdk_cfg.h"
#include "app_cfg.h"
#include "iic.h"
#include "common/app_cfg.h"
#include "rtos/os_api.h"
#include "rtos/os_cfg.h"
#include "common/error.h"
#include "common/msg.h"
#include "rtos/task_manage.h"
#include "timer.h"
#include "vm/vm_api.h"
#include "setup.h"
#include "setup.h"
#include "fm_engine.h"
#include "fm_band.h"
#include "ext_memory.h"
#include "e2prom/at24cxx.h"


#if defined(_SUPPORT_EXT_MEMORY_)

extern void *malloc_fun(void *ptr,u32 len,char *info);
extern void free_fun(void **ptr);

static const uint32 ext_map_id = 0x23767381;
static const char ext_map_date[] = __DATE__"-"__TIME__;

typedef struct{
	const char *cur_task;
	uint8 usb;
	uint8 card;
}sysctl_config_t;

typedef struct{
	uint32 	id;
	uint8 	date[sizeof(ext_map_date)];
	uint8 setup_check_flag;
	uint8 radio_check_flag;
	uint8 system_check_flag;
	setup_variable_t setup_config;
	radio_config_t radio_config;
	sysctl_config_t system_config;
}ext_memory_map_t;

typedef struct{
	ext_memory_map_t map;
	void *addr_setup;
	void *addr_radio;
	sysctl_config_t system_config;
}ext_memory_t;


static OS_SEM ext_memory_sem;
static ext_memory_t *ext_memory = NULL;

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
static void memory_timer(void *param)
{
	os_taskq_post_msg(EXT_MEMORY_TASK_NAME, 1, MSG_EXT_MEMORY_TIMER);
}
static uint8 memory_read_data(uint8 *data, uint16 addr, uint16 length)
{
#if defined(_E2PROM_DRIVER_AT24CXX_)
	return e2prom_read_data(data, addr, length);
#else
	return FALSE;
#endif
}

static uint8 memory_write_data(const uint8 *data, uint16 addr, uint16 length)
{
#if defined(_E2PROM_DRIVER_AT24CXX_)
	return e2prom_write_data(data, addr, length);
#else
	return FALSE;
#endif
}

static uint8 memory_modify_byte(uint8 data, uint16 addr)
{
	uint8 temp_data;
	uint8 result = FALSE;

	if(memory_write_data(&data, addr, 1))
	{
		if(memory_read_data(&temp_data, addr, 1))
		{
			if(temp_data == data)
			{
				result = TRUE;
			}
		}
	}

	return result;
}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

static uint8 check_memory_map(const uint8 *src, uint8 *dest, uint8 *check_flag, uint16 size)
{
	if(src == NULL || dest == NULL || check_flag == NULL || size == 0)return FALSE;

	uint16 i;
	//uint8 temp_data;
	uint8 exit_flag = FALSE;
	uint16 start_addr = (uint32)dest - (uint32)&ext_memory->map;

	for(i = 0; (exit_flag == FALSE) && (i < size); i++, start_addr++)
	{
		if(dest[i] != src[i])
		{
			if(memory_modify_byte(src[i], start_addr))
			{
				dest[i] = src[i];
			}
			else
			{
				//ext_memory_printf("%s---> FAIL!!!, index:%d\n", __FUNCTION__, i);
				exit_flag = TRUE;
			}
		}
	}

	if(exit_flag == FALSE)
	{
		if(*check_flag == FALSE)
		{
			start_addr = (uint32)check_flag - (uint32)&ext_memory->map;

			if(memory_modify_byte(TRUE, start_addr))
			{
				*check_flag = TRUE;
			}
			else
			{
				//ext_memory_puts("%s--->check FAIL!!!\n");
				exit_flag = TRUE;
			}
		}
	}

	return exit_flag;
}

static void ext_memory_system(void)
{
	ext_memory->system_config.cur_task = curr_task->name;
}

static void ext_memory_data_init(void)
{
	ext_memory_printf("ext_memory_data_init ---> setup_check_flag:%d, system_check_flag:%d, cur_task:%s\n",
					  ext_memory->map.setup_check_flag,
					  ext_memory->map.system_check_flag,
					  ext_memory->map.system_config.cur_task);

	ext_memory->system_config = ext_memory->map.system_config;
	if(ext_memory->map.setup_check_flag && ext_memory->map.system_check_flag)
	{
		if(ext_memory->map.setup_config.task_name != ext_memory->system_config.cur_task)
		{
			check_memory_map((uint8 *)&ext_memory->system_config.cur_task,
							 (uint8 *)&ext_memory->map.setup_config.task_name,
							 (uint8 *)&ext_memory->map.setup_check_flag,
							 sizeof(ext_memory->map.setup_config.task_name));
		}
	}
}

static void ext_memory_task(void *name)
{
    int msg[3];
	uint8 check_memory = FALSE;
	uint8 sem_flag = FALSE;

    ext_memory_puts("\n***********************EXT MEMORY TASK********************\n");

	ext_memory = (ext_memory_t *)malloc_fun(ext_memory ,sizeof(ext_memory_t), 0);
	if(ext_memory == NULL)
	{
		ext_memory_puts("ext_memory err\n");
        while(1);
	}

#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(memory_timer, NULL, 1000);
#else
	timer_reg_func(memory_timer, NULL, EXT_MEMORY_POLLING_TIME);
#endif

    while(1)
    {
        os_taskq_pend(0, ARRAY_SIZE(msg), msg);

        switch(msg[0])
		{
			case SYS_EVENT_DEL_TASK:
	            ext_memory_printf("%s ---> SYS_EVENT_DEL_TASK\n", __FUNCTION__);
	            if(os_task_del_req_name(OS_TASK_SELF) == OS_TASK_DEL_REQ)
	            {
	            	timer_unreg_func(memory_timer);
	            	free_fun((void**)&ext_memory);
	                os_task_del_res_name(OS_TASK_SELF);
	            }
	            break;

			case MSG_EXT_MEMORY_INIT:
				ext_memory_printf("%s ---> MSG_EXT_MEMORY_INIT, size:%d, data:%s\n", __FUNCTION__, sizeof(ext_memory->map), ext_map_date);
			#if defined(_LK8005_SPECIAL_RESET_)&&defined(_SUPPORT_DTH_)
				if(DTH_CHECK())
				{
					ew_puts("MSG_EXT_MEMORY_INIT11111\n");
					if(memory_read_data((uint8 *)&ext_memory->map, 0, sizeof(ext_memory->map)))
					{
						if(ext_memory->map.id == ext_map_id)
						{
							if(!memcmp(&ext_memory->map.date, ext_map_date, sizeof(ext_map_date)))
							{
								ext_memory_puts("MSG_EXT_MEMORY_INIT ---> check succeed 11111<---\n");
								check_memory = TRUE;
							}
						}

						if(!check_memory)
						{
							memset(&ext_memory->map, 0, sizeof(ext_memory->map));
							ext_memory->map.id = ext_map_id;
							memcpy(&ext_memory->map.date, ext_map_date, sizeof(ext_map_date));
							if(memory_write_data((uint8 *)&ext_memory->map, 0, sizeof(ext_memory->map)))
							{
								ext_memory_puts("MSG_EXT_MEMORY_INIT ---> init succeed 2222<---\n");
								check_memory = TRUE;
							}
						}
					}
				}
				else
				{
					ew_puts("MSG_EXT_MEMORY_INIT2222\n");
					memset(&ext_memory->map, 0, sizeof(ext_memory->map));
					ext_memory->map.id = ext_map_id;
					memcpy(&ext_memory->map.date, ext_map_date, sizeof(ext_map_date));
					if(memory_write_data((uint8 *)&ext_memory->map, 0, sizeof(ext_memory->map)))
					{
						ext_memory_puts("MSG_EXT_MEMORY_INIT ---> init succeed 333<---\n");
						check_memory = TRUE;
					}
				}
			#else
    			if(memory_read_data((uint8 *)&ext_memory->map, 0, sizeof(ext_memory->map)))
    			{
    				if(ext_memory->map.id == ext_map_id)
    				{
    					if(!memcmp(&ext_memory->map.date, ext_map_date, sizeof(ext_map_date)))
    					{
    						ext_memory_puts("MSG_EXT_MEMORY_INIT ---> check succeed 11111<---\n");
    						check_memory = TRUE;
    					}
    				}

    				if(!check_memory)
    				{
    					memset(&ext_memory->map, 0, sizeof(ext_memory->map));
    					ext_memory->map.id = ext_map_id;
    					memcpy(&ext_memory->map.date, ext_map_date, sizeof(ext_map_date));
    					if(memory_write_data((uint8 *)&ext_memory->map, 0, sizeof(ext_memory->map)))
    					{
    						ext_memory_puts("MSG_EXT_MEMORY_INIT ---> init succeed 2222<---\n");
    						check_memory = TRUE;
    					}
    				}
    			}
			#endif
				sem_flag = TRUE;
				break;

			case MSG_EXT_MEMORY_SETUP:
				ext_memory_printf("%s ---> MSG_EXT_MEMORY_SETUP\n", __FUNCTION__);
				ext_memory->addr_setup = (void *)msg[1];
				ext_memory_printf("ext_memory->addr_setup:%d\n",ext_memory->addr_setup);
				ext_memory_printf("check_memory:%d\n",check_memory);
				ext_memory_printf("ext_memory->map.setup_check_flag:%d\n",ext_memory->map.setup_check_flag);
				if((ext_memory->addr_setup != NULL) && check_memory && ext_memory->map.setup_check_flag)
				{
					ext_memory_puts("MSG_EXT_MEMORY_SETUP ---> succeed\n");
					memcpy(ext_memory->addr_setup, &ext_memory->map.setup_config, sizeof(ext_memory->map.setup_config));
					if((uint8 *)msg[2] != NULL)
					{
						*((uint8 *)msg[2]) = TRUE;
					}
				}
				else
				{
					ext_memory_puts("MSG_EXT_MEMORY_SETUP ---> fail\n");
					if((uint8 *)msg[2] != NULL)
					{
						*((uint8 *)msg[2]) = FALSE;
					}
				}
				sem_flag = TRUE;
				break;

			case MSG_EXT_MEMORY_RADIO:
				ext_memory_printf("%s ---> MSG_EXT_MEMORY_RADIO\n", __FUNCTION__);
				ext_memory->addr_radio = (void *)msg[1];
				if((ext_memory->addr_radio) != NULL && check_memory && ext_memory->map.radio_check_flag)
				{
					ext_memory_puts("MSG_EXT_MEMORY_RADIO ---> succeed\n");
					memcpy(ext_memory->addr_radio, &ext_memory->map.radio_config, sizeof(ext_memory->map.radio_config));
					if((uint8 *)msg[2] != NULL)
					{
						*((uint8 *)msg[2]) = TRUE;
					}
				}
				else
				{
					ext_memory_puts("MSG_EXT_MEMORY_RADIO ---> fail\n");
					if((uint8 *)msg[2] != NULL)
					{
						*((uint8 *)msg[2]) = FALSE;
					}
				}
				sem_flag = TRUE;
				break;

			case MSG_EXT_MEMORY_TIMER:
				if(check_memory)
				{
					uint8 exit_flag = FALSE;

					if(exit_flag == FALSE)
					{
						check_memory_map((uint8 *)ext_memory->addr_setup,
										 (uint8 *)&ext_memory->map.setup_config,
										 (uint8 *)&ext_memory->map.setup_check_flag,
										 sizeof(ext_memory->map.setup_config));
					}

					if(exit_flag == FALSE)
					{
						check_memory_map((uint8 *)ext_memory->addr_radio,
										 (uint8 *)&ext_memory->map.radio_config,
										 (uint8 *)&ext_memory->map.radio_check_flag,
										 sizeof(ext_memory->map.radio_config));
					}

					if(exit_flag == FALSE)
					{
						ext_memory_system();
						check_memory_map((uint8 *)&ext_memory->system_config,
										 (uint8 *)&ext_memory->map.system_config,
										 (uint8 *)&ext_memory->map.system_check_flag,
										 sizeof(ext_memory->map.system_config));
					}
				}
				break;
			case MSG_EXT_MEMORY_RESET:
				ext_memory_puts("MSG_EXT_MEMORY_RESET\n");
				memset(&ext_memory->map, 0, sizeof(ext_memory->map));
				ext_memory->map.id = ext_map_id;
				memcpy(&ext_memory->map.date, ext_map_date, sizeof(ext_map_date));
				if(memory_write_data((uint8 *)&ext_memory->map, 0, sizeof(ext_memory->map)))
				{
					ext_memory_puts("MSG_EXT_MEMORY_INIT ---> init succeed <---\n");
				}
				break;

	        default:
	            break;
		}

		if(sem_flag)
		{
			sem_flag = FALSE;
			os_sem_set(&ext_memory_sem, 0);
			os_sem_post(&ext_memory_sem);
		}
    }
}

void ext_memory_init(void)
{
    uint8 err;
	ext_memory_puts("ext_memory_init\n");

	err = os_sem_create(&ext_memory_sem, 0);
	if(err != OS_NO_ERR)
	{
		ext_memory_puts("ext_memory_sem, err\n");
		return;
	}

    err = os_task_create(ext_memory_task,
                         (void*)0,
                         TaskEncWFPrio,
                    #if defined(_ENABLE_MEMORY_PRIO_)
                        12
                    #else
						10
					#endif
					#if OS_TIME_SLICE_EN > 0
                         ,1
					#endif
                         ,EXT_MEMORY_TASK_NAME);
	if(err != OS_NO_ERR)
	{
		ext_memory_puts("ext_memory_create, err\n");
		return;
	}

	ext_memory_msg(MSG_EXT_MEMORY_INIT, NULL, NULL);
	ext_memory_data_init();

	ext_memory_puts("ext_memory_init_succeed!!\n");
}

uint8 ext_memory_msg(int msg, void *send_addr, void *reply_addr)
{
	os_taskq_post_event(EXT_MEMORY_TASK_NAME, 3, msg, send_addr, reply_addr);

	uint8 err = os_sem_pend(&ext_memory_sem, 0);
	if(err != OS_NO_ERR)
    {
    	ext_memory_printf("%s:%d---->err\n", __FUNCTION__, msg);
		return FALSE;
    }

	return TRUE;
}

void ext_memory_clear(void)
{
	ext_memory->map.id = 0 ;
	ext_memory_msg(MSG_EXT_MEMORY_INIT, NULL, NULL);
	memset(&ext_memory,0,sizeof(ext_memory));
	memory_write_data((uint8 *)&ext_memory, 0, sizeof(ext_memory));
}

#endif

