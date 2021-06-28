#ifndef __TYPEDEF_CONFIG_H__
#define __TYPEDEF_CONFIG_H__


typedef unsigned char 	uint8,UINT8;
typedef signed char 	int8,INT8;
typedef unsigned short 	uint16,UINT16;
typedef signed short 	int16,INT16;
typedef unsigned int 	uint32,UINT32;
typedef signed int 		int32,INT32;

#undef U32
#define U32  unsigned long

#define _B0_								(1<<0)
#define _B1_								(1<<1)
#define _B2_								(1<<2)
#define _B3_								(1<<3)
#define _B4_								(1<<4)
#define _B5_								(1<<5)
#define _B6_								(1<<6)
#define _B7_								(1<<7)

#define _B8_								(1<<8)
#define _B9_								(1<<9)
#define _B10_								(1<<10)
#define _B11_								(1<<11)
#define _B12_								(1<<12)
#define _B13_								(1<<13)
#define _B14_								(1<<14)
#define _B15_								(1<<15)

#define _B16_								(1<<16)
#define _B17_								(1<<17)
#define _B18_								(1<<18)
#define _B19_								(1<<19)
#define _B20_								(1<<20)
#define _B21_								(1<<21)
#define _B22_								(1<<22)
#define _B23_								(1<<23)

#define _B24_								(1<<24)
#define _B25_								(1<<25)
#define _B26_								(1<<26)
#define _B27_								(1<<27)
#define _B28_								(1<<28)
#define _B29_								(1<<29)
#define _B30_								(1<<30)
#define _B31_								(1<<31)


#define _u8_(Value,Pos)  					(((uint8  *)(&(Value)))[Pos])
#define _u16_(Value,Pos)					(((uint16 *)(&(Value)))[Pos])
#define _u32_(Value,Pos)					(((uint32 *)(&(Value)))[Pos])

#define _au8_(Addr,Pos)						(((uint8  *)(Addr))[Pos])
#define _au16_(Addr,Pos)					(((uint16 *)(Addr))[Pos])
#define _au32_(Addr,Pos)					(((uint32 *)(Addr))[Pos])

#define _u16to8h_(x)						((uint8)(((x)&0xFF00)>>8))		//_u8_(x,1)
#define _u16to8l_(x)						((uint8)(((x)&0xFF)>>0))			//_u8_(x,0)

#define _u32to16h_(x)						((uint16)(((x)&0xFFFF0000)>>16))	//_u16_(x,1)
#define _u32to16l_(x)						((uint16)(((x)&0xFFFF)>>0))		//_u16_(x,0)

#define _u32to8hh_(x)						((uint8)(((x)&0xFF000000)>>24))	//_u8_(x,3)
#define _u32to8hl_(x)						((uint8)(((x)&0xFF0000)>>16))		//_u8_(x,2)
#define _u32to8lh_(x)						((uint8)(((x)&0xFF00)>>8))		//_u8_(x,1)
#define _u32to8ll_(x)						((uint8)(((x)&0xFF)>>0))			//_u8_(x,0)

#define _u8tou16_(h,l)						((uint16)((((uint16)(h))<<8)  |(uint16)(l)))
#define _u16tou32_(h,l)						((uint32)((((uint32)(h))<<16) |(uint32)(l)))
#define _u8tou32_(hh,hl,lh,ll)				((uint32)((((uint32)(hh))<<24)|(((uint32)(hl))<<16)|(((uint32)(lh))<<8)|(((uint32)(ll)))))


typedef void (*V_FUNC_V)(void);
typedef void (*V_FUNC_U)(u8);
typedef uint8 (*U_FUNC_V)(void);
typedef uint8 (*U_FUNC_U)(u8);
typedef int (*ext_check_f)(int *);


#define ENABLE				(1)
#define DISABLE				(0)

#define ON					(1)
#define OFF					(0)

#define UP					(1)
#define DOWN				(0)

#define HIGH				(1)
#define LOW					(0)



//字体颜色
#define OUTPUT_COLOR_BLACK		"\033[0;1;30;40m"
#define OUTPUT_COLOR_RED		"\033[0;1;31;40m"
#define OUTPUT_COLOR_GREEN		"\033[0;1;32;40m"
#define OUTPUT_COLOR_YELLOW		"\033[0;1;33;40m"
#define OUTPUT_COLOR_BLUE		"\033[0;1;34;40m"
#define OUTPUT_COLOR_VIOLET		"\033[0;1;35;40m"
#define OUTPUT_COLOR_DARK_GREEN	"\033[0;1;36;40m"
#define OUTPUT_COLOR_WHITE		"\033[0;1;37;40m"

//回复默认颜色
#define OUTPUT_COLOR_ORIGIN		"\033[0m"


#define color_printf(c, x...)\
do{\
	char tmp[255];\
	sprintf(tmp, x);\
	printf("%s%s%s", c, tmp, OUTPUT_COLOR_ORIGIN);\
}while(0)

#define black_printf(x...)		color_printf(OUTPUT_COLOR_BLACK, x)
#define red_printf(x...)		color_printf(OUTPUT_COLOR_RED, x)
#define green_printf(x...)		color_printf(OUTPUT_COLOR_GREEN, x)
#define yellow_printf(x...)		color_printf(OUTPUT_COLOR_YELLOW, x)
#define blue_printf(x...)		color_printf(OUTPUT_COLOR_BLUE, x)
#define volet_printf(x...)		color_printf(OUTPUT_COLOR_VIOLET, x)
#define dark_green_printf(x...)	color_printf(OUTPUT_COLOR_DARK_GREEN, x)
#define white_printf(x...)		color_printf(OUTPUT_COLOR_WHITE, x)


#endif /* __TYPEDEF_CONFIG_H__ */

