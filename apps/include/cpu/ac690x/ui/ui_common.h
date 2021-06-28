#ifndef __UI_COMMON_H__
#define __UI_COMMON_H__
//enter here

enum{
	VFD_DISP_TYPE_NULL = 0,
	VFD_DISP_TYPE_TIME,
	VFD_DISP_TYPE_ROLL,
	VFD_DISP_TYPE_WINK,
	VFD_DISP_TYPE_END,
};


#define VFD_DISP_TIME_CLOCK					(1<<15)

#define VFD_DISP_ROLL_CUT_NULL				(1<<15)
#define VFD_DISP_ROLL_ADD_NULL				(1<<14)
#define VFD_DISP_ROLL_CUT_LEN				(1<<13)
#define VFD_DISP_CLOCK_ON					(1<<12)


#define VFD_DISP_POS						0
#if defined(_FY7000_SPECIAL_DISPLAY_)
#define VFD_DISP_SIZE						13
#elif defined(_FY5517_SPECIAL_DISPLAY_)
#define VFD_DISP_SIZE						11
#elif defined(_FIVE_CHAR_DISPLAY_)
#if defined(_MR889_SPECAIL_DISP_)
#define VFD_DISP_SIZE						7
#else
#define VFD_DISP_SIZE						6
#endif
#elif defined(_SEVEN_CHAR_DISPLAY_)
#define VFD_DISP_SIZE						7
#elif defined(_XM003_SPECIAL_DISPLAY_)
#define VFD_DISP_SIZE						7
#elif defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_SH1330_SPECIAL_DISPLAY_)
#define VFD_DISP_SIZE						10
#elif defined(_SL6812A_SPECIAL_DISPLAY_)
#define VFD_DISP_SIZE						10
#else
#define VFD_DISP_SIZE						8
#endif

#define VFD_DIPS_NUMBER_POS					(VFD_DISP_POS + VFD_DISP_SIZE)//0+8=8
#define VFD_DISP_NUMBER_SIZE				1
#define VFD_DISP_LAST_POS					(VFD_DIPS_NUMBER_POS + VFD_DISP_NUMBER_SIZE)//8+1=9
#define VFD_DISP_LENGTH						VFD_DISP_LAST_POS//9
#if defined(_SUPPORT_ID3_)
#define music_id3_info_max					100
#define ID3_INFO_LENGTH						(music_id3_info_max + VFD_DISP_LAST_POS)//100+9=109
#else
#define ID3_INFO_LENGTH						(VFD_DISP_LAST_POS)
#endif

typedef struct{
	struct{
		uint8 	Type;
		uint16 	Time;
		uint16	Para;
		uint16	Len;
		uint16	Point;
		uint8	Clock;
	}Ctrl;
	uint8 		Disp[ID3_INFO_LENGTH+VFD_DISP_LENGTH*2];//109+9*2=127
	uint8		VfdChange[VFD_DISP_LENGTH];
	uint8		VfdDisp[VFD_DISP_LENGTH];
}vfd_disp_t;

extern vfd_disp_t vfd_disp;


#define VFD_DISP_POLLING_TIME				DISP_TYPE_POLLING//100
#define GET_VFD_TIME()						(vfd_disp.Ctrl.Time)
#define SET_VFD_TIME(x)						(GET_VFD_TIME() = (x)/VFD_DISP_POLLING_TIME)//x/100
#define GET_VFD_TYPE()						(vfd_disp.Ctrl.Type)
#define SET_VFD_TYPE(x)						(GET_VFD_TYPE() = x)
#define GET_VFD_PARA()						(vfd_disp.Ctrl.Para)
#define SET_VFD_PARA(x)						(GET_VFD_PARA() = x)
#define GET_VFD_LENGTH()					(vfd_disp.Ctrl.Len)
#define SET_VFD_LENGTH(x)					(GET_VFD_LENGTH() = x)
#define GET_VFD_POINT()						(vfd_disp.Ctrl.Point)
#define SET_VFD_POINT(x)					(GET_VFD_POINT()=x)
#define GET_VFD_CLOCK()						(vfd_disp.Ctrl.Clock)
#define SET_VFD_CLOCK(x)					(GET_VFD_CLOCK()=x)
#define GET_VFD_WINK_STATUS()				(GET_VFD_POINT())
#define SET_VFD_WINK_STATUS(x)				(GET_VFD_WINK_STATUS() = x)
#define GET_VFD_RATE()						(GET_VFD_PARA())
#define SET_VFD_RATE(x)						(GET_VFD_RATE() = x)
#define GET_VFD_RATE_TIME()					(GET_VFD_TIME())
#define SET_VFD_RATE_TIME(x)				(SET_VFD_TIME(x))


extern void VdfDispString(void *, uint8, uint16);
extern void VfdDispPolling(void);
extern void VfdDispInit(void);


#endif /* __UI_COMMON_H__ */


