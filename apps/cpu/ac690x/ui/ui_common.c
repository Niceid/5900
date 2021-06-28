#include "sdk_cfg.h"
#include "setup.h"
#include "ui_res.h"
#include "ui_api.h"
#include "ui_common.h"


vfd_disp_t vfd_disp;

static void VfdDispNullInit(uint8 *str,uint16 para)
{
	memset(&vfd_disp.Ctrl, 0,sizeof(vfd_disp.Ctrl));
}
static void VfdDispNullPolling(void)
{
}

static void VfdDispTimeInit(uint8 *str,uint16 para)
{
	int Len;
	uint16 NullPos;
	uint16 StartPos;

	memset(vfd_disp.Disp,0,sizeof(vfd_disp.Disp));
	Len = strlen((char *)str);

	if(Len == VFD_DISP_SIZE)
	{
		memcpy(vfd_disp.Disp, str, Len);
	}
	else if(Len > VFD_DISP_SIZE)
	{
		Len = VFD_DISP_SIZE;
		memcpy(vfd_disp.Disp, str, Len);
		vfd_disp.Disp[Len] = '\0';
	}
	else
	{
		StartPos = 0;
		NullPos   = (VFD_DISP_SIZE - Len);

		if(NullPos/2)
		{
			vfd_disp.Disp[StartPos ++] = ' ';
			NullPos --;
		}

		if(NullPos)
		{
			memset(vfd_disp.Disp  + StartPos,  ' ',  NullPos/2);
			StartPos += (NullPos/2);
			memcpy(vfd_disp.Disp + StartPos, str,  Len);
			StartPos += Len;
			memset(vfd_disp.Disp  + StartPos,  ' ',  NullPos/2);
		}

		Len = VFD_DISP_SIZE;
	}

	if(para&VFD_DISP_TIME_CLOCK)
	{
		SET_VFD_CLOCK(TRUE);
	}
	else
	{
	#if defined(_XZC1668_SPECIAL_DISPLAY_)||defined(_WLS316_SPECIAL_DISPLAY_)||defined(_SL6812A_SPECIAL_DISPLAY_)||defined(SPECIAL_SLA6799_DISP)||defined(_ENABLE_PLAY_DISPLAY_)//上下曲直接显示时间
		if(global.song_state)
		{
			global.song_state = FALSE;
		}
		else
		{
			SET_VFD_CLOCK(FALSE);
		}
	#else
		SET_VFD_CLOCK(FALSE);
	#endif
	}

	para &= (~VFD_DISP_TIME_CLOCK);

	if(para)
	{
		SET_VFD_PARA(TRUE);
	}
	else
	{
		SET_VFD_PARA(FALSE);
	}

	SET_VFD_LENGTH(Len);
	SET_VFD_TIME(para);
	SET_VFD_TYPE(VFD_DISP_TYPE_TIME);
}
static void VfdDispTimePolling(void)
{
	if(GET_VFD_PARA())
	{
		if(GET_VFD_TIME())
		{
			GET_VFD_TIME() --;
		}
		else
		{
			VdfDispString(NULL,VFD_DISP_TYPE_NULL,0);
		}
	}
	memcpy(vfd_disp.VfdDisp + VFD_DISP_POS, vfd_disp.Disp, VFD_DISP_SIZE);
}

static void VfdDispRollInit(uint8 *str,uint16 para)
{
	int Len;
	uint16 NullPos;
	uint16 StartPos;

	memset(vfd_disp.Disp,0,sizeof(vfd_disp.Disp));

	StartPos = 0;

	if(!(para & VFD_DISP_ROLL_CUT_NULL))
	{
		memset(vfd_disp.Disp + StartPos,' ',VFD_DISP_SIZE);StartPos += VFD_DISP_SIZE;
	}

	Len = strlen((char *)str);

	if((para & VFD_DISP_ROLL_ADD_NULL) && (Len < VFD_DISP_SIZE))
	{
		NullPos = (VFD_DISP_SIZE - Len);
		if(NullPos/2)
		{
			vfd_disp.Disp[StartPos ++] = ' ';
			NullPos --;
		}

		if(NullPos)
		{
			memset(vfd_disp.Disp + StartPos,  	' ',  NullPos/2);	StartPos += (NullPos/2);
			memcpy(vfd_disp.Disp + StartPos, 	str,  Len);			StartPos += Len;
			memset(vfd_disp.Disp + StartPos,  	' ',  NullPos/2);	StartPos += (NullPos/2);
		}
	}
	else
	{
		memcpy(vfd_disp.Disp + StartPos,str,Len);StartPos += Len;
	}
	Len = StartPos;

	if(!(para & VFD_DISP_ROLL_CUT_NULL) && (para & VFD_DISP_ROLL_CUT_LEN))
	{
		Len -= VFD_DISP_SIZE;
	}

	para &=  (~(VFD_DISP_ROLL_CUT_NULL | VFD_DISP_ROLL_ADD_NULL | VFD_DISP_ROLL_CUT_LEN));

	SET_VFD_LENGTH(Len);
	SET_VFD_POINT(0);
	SET_VFD_CLOCK(FALSE);
	if(para >= VFD_DISP_POLLING_TIME)
	{
		para -= VFD_DISP_POLLING_TIME;
	}
	SET_VFD_RATE(para);
	SET_VFD_RATE_TIME(0);
	SET_VFD_TYPE(VFD_DISP_TYPE_ROLL);
}
static void VfdDispRollPolling(void)
{
	if(GET_VFD_RATE_TIME())GET_VFD_RATE_TIME()--;
	else
	{
		if(GET_VFD_POINT() <= GET_VFD_LENGTH())
		{
			memcpy(vfd_disp.VfdDisp + VFD_DISP_POS, vfd_disp.Disp + GET_VFD_POINT(), VFD_DISP_SIZE);
			SET_VFD_RATE_TIME(GET_VFD_RATE());
			GET_VFD_POINT()++;
		}
		else
		{
			VdfDispString(NULL,VFD_DISP_TYPE_NULL,0);
		}
	}
}

static void VfdDispWinkInit(uint8 *str,uint16 para)
{
	int Len;
	uint16 NullPos;
	uint16 StartPos;

	memset(vfd_disp.Disp,0,sizeof(vfd_disp.Disp));
	Len = strlen((char *)str);

	if(Len == VFD_DISP_SIZE)
	{
		memcpy(vfd_disp.Disp,str,Len);
	}
	else if(Len > VFD_DISP_SIZE)
	{
		Len = VFD_DISP_SIZE;
		memcpy(vfd_disp.Disp,str,Len);
		vfd_disp.Disp[Len] = 0;
	}
	else
	{
		StartPos = 0;
		NullPos   = (VFD_DISP_SIZE - Len);
		if(NullPos/2)
		{
			vfd_disp.Disp[StartPos ++] = ' ';
			NullPos --;
		}

		if(NullPos)
		{
			memset(vfd_disp.Disp  + StartPos,  ' ',  NullPos/2);
			StartPos += (NullPos/2);
			memcpy(vfd_disp.Disp + StartPos, str,  Len);
			StartPos += Len;
			memset(vfd_disp.Disp  + StartPos,  ' ',  NullPos/2);
		}

		Len = VFD_DISP_SIZE;
	}

	SET_VFD_RATE(para);
	SET_VFD_RATE_TIME(0);
#if defined(_EVB_PAUSE_DISPLAY_)
	if(!global.pause_flag)
	{
		SET_VFD_CLOCK(FALSE);
	}
#else
	SET_VFD_CLOCK(FALSE);
#endif
	SET_VFD_WINK_STATUS(FALSE);
	SET_VFD_LENGTH(Len);
	SET_VFD_TYPE( VFD_DISP_TYPE_WINK);
}
static void VfdDispWinkPolling(void)
{
	if(GET_VFD_RATE_TIME())GET_VFD_RATE_TIME() --;
	else
	{
		if(GET_VFD_WINK_STATUS() == TRUE)
		{
			SET_VFD_WINK_STATUS(FALSE);
			memset(vfd_disp.VfdDisp + VFD_DISP_POS, ' ', VFD_DISP_SIZE);
	    #if defined(_EVB_PAUSE_DISPLAY_)
	    	if(global.pause_flag)
	    	{
	    		SET_VFD_CLOCK(FALSE);
	    	}
	    #endif
		}
		else
		{
			SET_VFD_WINK_STATUS(TRUE);
			memcpy(vfd_disp.VfdDisp + VFD_DISP_POS, vfd_disp.Disp, VFD_DISP_SIZE);
	    #if defined(_EVB_PAUSE_DISPLAY_)
	    	if(global.pause_flag)
	    	{
	    		SET_VFD_CLOCK(TRUE);
	    	}
	    #endif
		}
		SET_VFD_RATE_TIME(GET_VFD_RATE());
	}
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/

typedef void (*FUNC_VFD_DISP_INIT)(uint8 *,uint16);
typedef void (*FUNC_VFD_DISP_POLLING)(void);

static const FUNC_VFD_DISP_INIT FuncVfdDispInit[]=
{
	VfdDispNullInit,
	VfdDispTimeInit,
	VfdDispRollInit,
	VfdDispWinkInit,
};

static const FUNC_VFD_DISP_POLLING FuncVfdDispPolling[]=
{
	VfdDispNullPolling,
	VfdDispTimePolling,
	VfdDispRollPolling,
	VfdDispWinkPolling,

};

/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------*/

void VdfDispString(void *str,uint8 type,uint16 para)
{
#if defined(TD_ADD_LCD_NUM_4)||defined(_LCD_3_DIAPLAY_)
	uint8 info_buffer[8];
	uint8 *data_info = info_buffer;
	uint8 length;
	memset(info_buffer,' ', sizeof(info_buffer));
	if(str != NULL)
	{
	    length = sizeof((char *)str);
        if(length >8)
        {
			length =8;
        }
		memcpy(data_info, str,(int)length);
	}
	if(type < VFD_DISP_TYPE_END)
	{
	    FuncVfdDispInit[type](data_info, para);
	}
#else
	uint8 info_buffer[10];
	uint8 *data_info = info_buffer;
	uint8 length;

	memset(info_buffer, 0, sizeof(info_buffer));

	if(str != NULL)
	{
		length = strlen((char *)str)+1;
			
		if(length <= sizeof(info_buffer))
		{
			memcpy(data_info, str, (int)length - 1);
		}
		else
		{
			data_info = (uint8 *)str;
		}
	}

	if(type < VFD_DISP_TYPE_END)
	{
		FuncVfdDispInit[type](data_info, para);
	}
#endif
}

void VfdDispPolling(void)
{
	if(GET_VFD_TYPE() < VFD_DISP_TYPE_END)
	{
		FuncVfdDispPolling[GET_VFD_TYPE()]();
	}
	
	if(memcmp(vfd_disp.VfdChange,vfd_disp.VfdDisp,VFD_DISP_LENGTH))
	{
		memcpy(vfd_disp.VfdChange, vfd_disp.VfdDisp,VFD_DISP_LENGTH);
		ui_show_com(vfd_disp.VfdDisp, GET_VFD_CLOCK());
	}
}

void VfdDispInit(void)
{
	memcpy(&vfd_disp, 0, sizeof(vfd_disp));
}

