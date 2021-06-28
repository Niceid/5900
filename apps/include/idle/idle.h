#ifndef __IDLE_H__
#define __IDLE_H__


#define IDLE_POLLING_TIME				(100)


#define IDLE_DEBUG
#ifdef IDLE_DEBUG
    #define idle_puts     ew_puts
    #define idle_printf   ew_printf
#else
    #define idle_puts(...)
    #define idle_printf(...)
#endif

#endif /* __IDLE_H__ */

