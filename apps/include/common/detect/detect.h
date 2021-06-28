#ifndef __DETECT_H__
#define __DETECT_H__


#define DETECT_DEBUG
#ifdef DETECT_DEBUG
#define detect_printf 		ew_printf
#define detect_puts 		ew_puts
#else
#define detect_printf(...)
#define detect_puts(...)
#endif


enum
{
    DETECT_OFF = 0,
    DETECT_ON,
    DETECT_NULL,
};

typedef uint8 (*detect_func)(void);

typedef struct
{
    uint8 			sample_state;
    uint8 			sample_on;
	uint8 			sample_off;
	uint8 			sample_cnt;
	uint8 			check_state;
    int 			dev_state;
	detect_func 	func;
	uint16 			usbdelay;
#if defined(USB_DELAY_CHECK)
	uint16			usbdelayoff;
#endif
#if defined(_DISC_OUT_DELAY_CHECK_)
	uint16			discdelayoff;
#endif
}detect_var_t;


extern void detect_init_api(void);
extern void detect_check_api(uint32);


#endif/*__DETECT_H__*/

