#ifndef __EXT_MEMORY__
#define __EXT_MEMORY__

#include "easyway_config.h"
#include "typedef.h"


#if defined(_SUPPORT_EXT_MEMORY_)

#define EXT_MEMORY_DEBUG

#ifdef EXT_MEMORY_DEBUG
    #define ext_memory_puts     ew_puts
    #define ext_memory_printf   ew_printf
#else
    #define ext_memory_puts(...)
    #define ext_memory_printf(...)
#endif


#define EXT_MEMORY_POLLING_TIME			500
#define EXT_MEMORY_TASK_NAME        	"ExtMemoryTask"

extern void ext_memory_init(void);
extern uint8 ext_memory_msg(int, void *, void *);
extern void ext_memory_clear(void);

#endif


#endif/*__EXT_MEMORY__*/

