#ifndef __UART_H__
#define __UART_H__
#include "sdk_cfg.h"

//#define UART_BAUD_RAE		9600

#define UART_BAUD_RAE		115200

#define DAB_UART_BAUD_RAE		9600

s32  uart_init(u32 baud);
s32 uart_set_baud(u32 baud);
s32 EQ_uart_init(void);
#ifdef TD_ADD_USER_UART
extern u8 gRecvStep;
extern u8 gRecvData;
extern u8 gRecvTime;
extern u8 gC912detOk;
extern u8 gRecvCheckSum;
extern u8 Disp_dab_buf[32];

s32 uart_interrupt_receive_init(u32 baud);
extern void uart_interrupt_receive_write(char a);
extern void Close_Uart2(void);
extern void printf_dabInfo(void);
#endif

#if defined(_SUPPORT_DISC_)
s32 uart_interrupt_receive_init(u32 baud);
extern void uart_updata_receive_cmd(void);
extern void uart_interrupt_receive_write(char a);
#endif

#if defined(__EW_DEBUG__)

void putchar(char a);
void put_u4hex(u8 dat);
void put_u8hex(u8 dat);
void put_u16hex(u16 dat);
void put_u32hex(u32 dat);
void put_u64hex(u64 dat);
void put_u32hex0(u32 dat);

void put_buf(u8 *buf, u32 len );
int puts(const char *out);
void printf_buf(u8 *buf, u32 len);

int ew_puts(const char *);
#define ew_printf		printf

#define ew_black_printf		black_printf
#define ew_red_printf		red_printf
#define ew_green_printf		green_printf
#define ew_yellow_printf	yellow_printf
#define ew_blue_printf		blue_printf
#define ew_volet_printf		volet_printf
#define ew_dark_printf		dark_green_printf
#define ew_white_printf		white_printf

#else


#define ew_puts(...)
#define ew_printf(...)

#define ew_black_printf(...)
#define ew_red_printf(...)
#define ew_green_printf(...)
#define ew_yellow_printf(...)
#define ew_blue_printf(...)
#define ew_volet_printf(...)
#define ew_dark_printf(...)
#define ew_white_printf(...)


void putchar(char a);
void put_u4hex(u8 dat);
void put_u8hex(u8 dat);
void put_u16hex(u16 dat);
void put_u32hex(u32 dat);
void put_u64hex(u64 dat);
void put_u32hex0(u32 dat);

void put_buf(u8 *buf, u32 len);
int puts(const char *out);
void printf_buf(u8 *buf, u32 len);


#endif

#endif /* __UART_H__ */

