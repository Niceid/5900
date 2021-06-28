#ifndef __TASK_MANAGE_H__
#define __TASK_MANAGE_H__

#include "typedef.h"

#define TASK_MANAGE_DEBUG
#ifdef TASK_MANAGE_DEBUG
#define task_manage_printf 			ew_printf
#define task_manage_puts 			ew_puts
#else
#define task_manage_printf(...)
#define task_manage_puts(...)
#endif


typedef enum
{
    RUN_TASK_SUCC = 0,          ///<��������ɹ�
    RUN_TASK_ERR_NO_TAG,        ///<û��ָ������ʲô����
    RUN_TASK_ERR_HAS_RPT,       ///<ָ���������Ѿ���������״̬
}RUN_TASK_STATUS;


typedef enum
{
    SWITCH_SPEC_TASK = 0,       ///<ָ��task
    SWITCH_NEXT_TASK,          	///<��һtask
    SWITCH_PRE_TASK,            ///<��һtask
    SWITCH_LAST_TASK,           ///<ǰһ��һtask
}TASK_SWITCH_MODE;

struct task_info
{
	const char *name;
	void (*init)(void *priv);
	void (*exit)();
};

struct task_dev_info
{
	const char *name;
	u32 (*dev_check)(void);
};

#define  TASK_REGISTER(task) 		const struct task_info task sec(.task_info)


extern struct task_info *curr_task;


void task_manager_init(void);
extern RUN_TASK_STATUS task_switch(const char *task_name, const void *priv, TASK_SWITCH_MODE cur_mode);
extern u32 compare_task_name(const char *task_name);
const char *get_last_task_name(void);
void set_last_task_name(const char *);

#endif /* __TASK_MANAGE_H__ */

