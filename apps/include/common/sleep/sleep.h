#ifndef __SLEEP_H__
#define __SLEEP_H__


extern void set_poweroff_wakeup_io(void);
extern void set_sleep_mode_wakeup_io(void);
extern void set_sleep_before_close_irq(void);

extern void system_lowpower_api(void);
extern void system_reset_api(void);

#endif /* __SLEEP_H__ */
