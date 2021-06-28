#ifndef TIMER_H
#define TIMER_H

#include "typedef.h"
#include "cpu/timer_api.h"
#include "typedef_config.h"

extern u8 ghalf_sec;

s32 timer0_init();
s32 timer2_init();
s32 timer3_init(void);

void set_tick_timer(u32 hz );
void init_tick_timer(u32 hz);

extern u32 make_seed(void);
extern u8 get_sys_halfsec(void);
extern u32 get_sys_2msCount(void);


extern __timer_handle  *timer0_hl;
typedef void (*timer_callback)(void *);

#if defined(_SUPPORT_PANEL_PWM_)
extern void set_panel_pwm(uint8, uint8, uint8);
extern void set_panel_enable(uint8);
#endif

#if defined(SET_BEEP_FUNCV)
extern void set_beep_ctrl(uint8 beep);
#endif

extern int32 timer_reg_func(timer_callback, void *, uint32);
extern int32 timer_unreg_func(timer_callback);
extern void clr_time_cnt(void);
extern uint32 get_time_tick(void);
extern void set_time_tick(uint32 tick);

#endif

