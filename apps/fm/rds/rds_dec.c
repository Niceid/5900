#include "sdk_cfg.h"
#include "rds/rds_dec.h"

#if defined(_ENABLE_RADIO_RDS_)

static rds_dec_t rds_dec;

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

#define RDS_MJD_CODE_MIN				(51543)
#define RDS_MJD_CODE_MAX				(88037)

static const uint16 rds_mdj_tab[]=
{
	    0,  366,  731, 1096, 1461, 1827, 2192, 2557, 2922, 3288,
	 3653, 4018, 4383, 4749, 5114, 5479, 5844, 6210, 6575, 6940,
	 7305, 7671, 8036, 8401, 8766, 9132, 9497, 9862,10227,10593,
	10958,11323,11688,12054,12419,12784,13149,13515,13880,14245,
	14610,14976,15341,15706,16071,16437,16802,17167,17532,17898,
	18263,18628,18993,19359,19724,20089,20454,20820,21185,21550,
	21915,22281,22646,23011,23376,23742,24107,24472,24837,25203,
	25568,25933,26298,26664,27029,27394,27759,28125,28490,28855,
	29220,29586,29951,30316,30681,31047,31412,31777,32142,32508,
	32873,33238,33603,33969,34334,34699,35064,35430,35795,36160
};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static void rds_ta_decode(void)
{
	rds_dec.decode.ta_have = TRUE;
	
	if(rds_dec.input.block_bl & _B4_)
	{
		rds_dec.decode.ta = ENABLE;
	}
	else 
	{
		rds_dec.decode.ta = DISABLE;
	}
}

static void rds_ms_ta_decode(void)
{	 
	rds_ta_decode();
	
	if(rds_dec.input.block_bl & _B3_)
	{
		rds_dec.decode.ms = ENABLE;
	}
	else 
	{
		rds_dec.decode.ms = DISABLE;
	}
}

static void rds_psn_decode(uint16 block, uint8 pos, rds_psn_t *psn)
{	
	uint16 check_value;
	
 	pos &= 0x03;
	rds_dec_printf("rds_psn_decode: 0x%x,\n",block);
	
	if((_u16to8h_(block) < 0x20)||(_u16to8l_(block) < 0x20))return;
	
	check_value = _u8tou16_(psn->buffer[pos*2+0], psn->buffer[pos*2+1]);
	rds_dec_printf("rds_psn_decode checkVale: 0x%x,\n",check_value);
	
	if(check_value != block)
	{
		if(check_value)
		{
			memset(psn->buffer,0,sizeof(psn->buffer));
			psn->ctrl = 0;
		}
	}
	else return;
	
	psn->buffer[pos*2+0] = _u16to8h_(block);
	psn->buffer[pos*2+1] = _u16to8l_(block);
	
	psn->ctrl |= (1<<pos);
	rds_dec_printf("rds_psn_decode3: 0x%x\n",psn->ctrl);
		
	if(psn->ctrl == 0x0F)
	{
		memcpy(psn->data, psn->buffer, sizeof(psn->buffer));
		
		psn->succeed = TRUE;
			
		rds_dec_printf("psn--->%c%c%c%c%c%c%c%c\n", 
					psn->data[0],psn->data[1],
					psn->data[2],psn->data[3],
					psn->data[4],psn->data[5],
					psn->data[6],psn->data[7]);
	}
}

static void rds_af_decode(uint16 block, rds_af_t *af)
{
	uint8 	i,j;
	uint8 	Len;
	uint16	Temp1,Temp2;
	uint8 	Freq1,Freq2;
	uint16	temp_block;
	
	if(af->state == RDS_AF_STATE_NULL)
	{
		af->state = RDS_AF_STATE_LENGTH;
	}
	if(!block)return;

	if((_u16to8h_(block) > 0)&&(_u16to8h_(block) <= 204))
	{		
		if((af->state == RDS_AF_STATE_LENGTH)||(af->method == RDS_AF_METHOD_B_2))
		{
			if((_u16to8l_(block) > 0)&&(_u16to8l_(block) <= 204))
			{
				if(af->method == RDS_AF_METHOD_NULL)
				{
					temp_block = _u8tou16_(af->buffer[1+0*2+0], af->buffer[1+0*2+1]);
					if(temp_block)
					{
						if(temp_block == block)return;	
						
						if((af->buffer[0] == _u16to8h_(block))||(af->buffer[0] == _u16to8l_(block)))
						{
							af->buffer[0] = af->buffer[0];
						}
						else if((af->buffer[1] == _u16to8h_(block))||(af->buffer[1] == _u16to8l_(block)))
						{
						 	af->buffer[0] = af->buffer[1];
						}
						else return;
						
						af->buffer[1+1*2+0] = _u16to8h_(block);
						af->buffer[1+1*2+1] = _u16to8l_(block);
						
						af->method = RDS_AF_METHOD_B_2;	
					}
					else 						   
					{
						af->buffer[1+0*2+0] = _u16to8h_(block);
						af->buffer[1+0*2+1] = _u16to8l_(block);
					}	
				}
				else if(af->method == RDS_AF_METHOD_B_2)
				{
					for(i = 0; i < ((_RDS_AF_LIST_MAX_-1)/2); i++)
					{
						temp_block = _u8tou16_(af->buffer[1+i*2+0], af->buffer[1+i*2+1]);
						if(temp_block == block)return;	
						if(temp_block == 0)break;
					}
					if(i == ((_RDS_AF_LIST_MAX_-1)/2))return;
					af->buffer[1+i*2+0] = _u16to8h_(block);
					af->buffer[1+i*2+1] = _u16to8l_(block);
					af->state = RDS_AF_STATE_SUCCEED;
					rds_dec_puts("rds AF method B_2 succeed!!!\n");
				}						
			}	
		}
		else if(af->state == RDS_AF_STATE_DATA)
		{
			if(af->method == RDS_AF_METHOD_NULL)
			{
				if((af->buffer[0] == _u16to8h_(block))||(af->buffer[0] == _u16to8l_(block)))
				{
					af->method = RDS_AF_METHOD_B;
				}
				else
				{
					af->method = RDS_AF_METHOD_A;
				}
			}

			if(af->method == RDS_AF_METHOD_A)
			{
				for(i = 0;i < _RDS_AF_LIST_MAX_;i ++)
				{
					if(af->buffer[i] == _u16to8h_(block))goto AF_NEXT_FREQ;
					if(af->buffer[i] == 0)break;
				}
				if(i == _RDS_AF_LIST_MAX_)return;
				af->buffer[i] = _u16to8h_(block);
				af->length ++;
			AF_NEXT_FREQ:
				if((_u16to8l_(block) > 0)&&(_u16to8l_(block) <= 204))
				{
					for(i = 0;i < _RDS_AF_LIST_MAX_;i ++)
					{
						if(af->buffer[i] == _u16to8l_(block))return;
						if(af->buffer[i] == 0)break;
					}
					if(i == _RDS_AF_LIST_MAX_)return;
					af->buffer[i] = _u16to8l_(block);
					af->length ++;
				}
				else if(_u16to8l_(block) == 205)
				{
				}
				else return;			
			}
			else if(af->method == RDS_AF_METHOD_B)
			{
				for(i = 0;i < ((_RDS_AF_LIST_MAX_-1)/2);i ++)
				{
					temp_block = _u8tou16_(af->buffer[1+i*2+0], af->buffer[1+i*2+1]);
					if(temp_block == block)return;
					if(temp_block == 0)break;
				}
				if(i == ((_RDS_AF_LIST_MAX_-1)/2))return;
				af->buffer[1+i*2+0] = _u16to8h_(block);
				af->buffer[1+i*2+1] = _u16to8l_(block);
				af->length += 2;
			}
		}
	}
	else if((_u16to8h_(block) >= 224)&&(_u16to8h_(block) <= 249))
	{
AF_LENGTH_AGAIN:
		if((af->state == RDS_AF_STATE_LENGTH)||(af->method == RDS_AF_METHOD_B_2))
		{
			memset(af, 0, sizeof(rds_af_t));
			af->total = _u16to8h_(block) - 224;
			if(_u16to8l_(block) != 205)
			{
				af->buffer[0] = _u16to8l_(block);
				af->length = 1;  
			}
			af->state = RDS_AF_STATE_DATA;
		}									
		else
		{
			if((_u16to8h_(block) - 224) != af->total)
			{
				af->state = RDS_AF_STATE_LENGTH;
				goto AF_LENGTH_AGAIN;
			}
		}			
	}
	else if(_u16to8h_(block) == 250)
	{
		if(af->state == RDS_AF_STATE_DATA)
		{
			af->length++;	
		}		
	}
	else return;

	if(af->state == RDS_AF_STATE_DATA)
	{
		if(af->total == af->length)
		{
			if(af->method == RDS_AF_METHOD_A)
			{
				for(Len = 0;Len < _RDS_AF_LIST_MAX_;Len ++)
				{
					if(af->buffer[Len] == 0)break;
				}
				
				if(Len)
				{
					for(i=0; i<(Len-1); i++)
					{
						for(j=0; j<(Len-1-i); j++)
						{
							Freq1 = af->buffer[j];
							Freq2 = af->buffer[j+1];
							if(Freq1 > Freq2)
							{
								af->buffer[j]   = Freq2;
								af->buffer[j+1] = Freq1;
							}			
						}
					}
				}		
			}
			else if(af->method == RDS_AF_METHOD_B)
			{
				for(Len = 0;Len < ((_RDS_AF_LIST_MAX_-1)/2);Len ++)
				{
					temp_block = _u8tou16_(af->buffer[1+Len*2+0], af->buffer[1+Len*2+1]);
					if(temp_block == 0)break;
				}
				
				if(Len)
				{
					for(i=0; i<(Len-1); i++)
					{
						for(j=0; j<(Len-1-i); j++)
						{
							Temp1 = _u8tou16_(af->buffer[1+j*2+0],     af->buffer[1+j*2+1]);
							Temp2 = _u8tou16_(af->buffer[1+(j+1)*2+0], af->buffer[1+(j+1)*2+1]);
							
							if(af->buffer[0] == _u16to8h_(Temp1))
							{
								Freq1 = _u16to8l_(Temp1);					
							}
							else
							{
								Freq1 = _u16to8h_(Temp1);	
							}
							
							if(af->buffer[0] == _u16to8h_(Temp2))
							{
								Freq2 = _u16to8l_(Temp2);					
							}
							else
							{
								Freq2 = _u16to8h_(Temp2);	
							}
							
							if(Freq1 > Freq2)
							{
								af->buffer[1+j*2+0] 	= _u16to8h_(Temp2);
								af->buffer[1+j*2+1] 	= _u16to8l_(Temp2);
								af->buffer[1+(j+1)*2+0] = _u16to8h_(Temp1);
								af->buffer[1+(j+1)*2+1] = _u16to8l_(Temp1);
							}			
						}
					}
				}
			}
			af->state = RDS_AF_STATE_SUCCEED;
			rds_dec_puts("rds AF method B succeed!!!\n");
		}
	}
}

static rds_eon_t *rds_now_eon_list(uint16 eon_pi)
{
	uint8 index;
	rds_eon_t *now_eon;
	uint8 check_flag = FALSE;
	
	if(eon_pi)
	{
		for(index = 0; index < _RDS_EON_LIST_MAX_; index++)
		{
			now_eon = &rds_dec.decode.eon[index];
			
			if(now_eon->pi)
			{
				if(now_eon->pi == eon_pi)
				{
					check_flag = TRUE;
					break;
				}
			}
			else
			{
				now_eon->pi = eon_pi;
				check_flag = TRUE;
				break;
			}
		}
	}
	
	if(check_flag)
	{
		rds_dec.ctrl.timer_eon = (_RDS_EON_TIME_/rds_dec.det_time);
		rds_dec.ctrl.timer_eon_list[index] = (_RDS_EON_TIME_/rds_dec.det_time);
		return now_eon;
	}
	
	return NULL;
}

static void rds_date_decode(void)
{	
	uint32	mjd;
	uint16	date;
	uint8	bottom,top;
	
	uint16	temp;
	uint8 	temp_min;
	uint8 	temp_hour;
	
	uint8 	offset;
	uint8 	offset_min;
	uint8 	offset_hour;
	int8	offset_day;
	
	uint8 	polarity;
	uint8 	i;
	
	temp_hour = (((rds_dec.input.block_cl&_B0_)<<4)|(rds_dec.input.block_dh>>4));
	
	temp = _u8tou16_(rds_dec.input.block_dh, rds_dec.input.block_dl);
	
	temp_min  = (uint8)((temp>>6)&0x3F);		
	
	offset = (rds_dec.input.block_dl&0x3F);

	if(offset & _B5_)
	{
		polarity = TRUE;
	}
	else
	{
		polarity = FALSE;
	}
	
	offset 	   &= 0x1F;
	offset_hour = (offset/2);
	offset_min  = ((offset%2)*30);
	
	for(i = 0; i < offset_min; i++)
	{
		if(polarity)
		{//	负
			if(temp_min)temp_min--;
			else
			{
				temp_min = 59;
				offset_hour++;
			}	
		}
		else
		{//	正
		 	if(temp_min < 59)temp_min++;
			else
			{
				temp_min = 0;
				offset_hour++;
			}		
		}		
	}
	
	offset_day = 0;
	for(i = 0;i < offset_hour; i++)
	{
		if(polarity)
		{//	负
			if(temp_hour)temp_hour--;
			else
			{
				temp_hour = 23;
				offset_day--;
			}	
		}
		else
		{//	正
		 	if(temp_hour < 23)temp_hour++;
			else
			{
				temp_hour = 0;
				offset_day++;
			}		
		}		
	}
	
	rds_dec.decode.mjd.min  = temp_min;
	rds_dec.decode.mjd.hour = temp_hour;

	mjd = _u8tou16_(rds_dec.input.block_ch, rds_dec.input.block_cl);

	mjd >>= 1;
	
	if(rds_dec.input.block_bl & _B0_)
	{
		mjd |= _B15_;
	}
	if(rds_dec.input.block_bl & _B1_)
	{
		mjd |= _B16_;
	}
	mjd += offset_day;
	
	if((mjd > (RDS_MJD_CODE_MIN-1))&&(mjd < (RDS_MJD_CODE_MAX+1)))
	{
		date 	= (uint16)(mjd - RDS_MJD_CODE_MIN);
		bottom	= 0;
		top		= 99;
		while((top - bottom)>1)
		{
			i = (top - bottom)/2;
			i += bottom;
			if(date > rds_mdj_tab[i])
			{
				bottom = i;	
			}
			else
			{
				top = i;
			} 
		}
		date -= rds_mdj_tab[bottom];
		
		rds_dec.decode.mjd.year = 100 + bottom;
		
		for(top = 1; top < 13; top++)
		{
			switch(top)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					if(date > 31)date -= 31;
					else goto RDS_MONTH_SEARCH_OVER;
					break;
				case 4:
				case 6:
				case 9:				
				case 11:
					if(date >30)date -= 30;
					else goto RDS_MONTH_SEARCH_OVER;
					break;
				case 2:
					if(rds_dec.decode.mjd.year%4)
					{
						if(date >28)date -= 28;
						else goto RDS_MONTH_SEARCH_OVER;	
					}
					else
					{
						if( date >29)date -= 29;
						else goto RDS_MONTH_SEARCH_OVER;	
					}
					break;
			}		 
		}
	RDS_MONTH_SEARCH_OVER:
		
		rds_dec.decode.mjd.month = top;
		rds_dec.decode.mjd.day   = (uint8)date;
	}
	
	rds_dec.decode.mjd.succeed = TRUE;
}		

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
static void base_info_a(void)		   			/*0 0A Basic tuning and switching information only*/
{	
	rds_ms_ta_decode();
	
	rds_af_decode(	_u8tou16_(rds_dec.input.block_ch, rds_dec.input.block_cl),
					&rds_dec.decode.af);
	
	rds_psn_decode(	_u8tou16_(rds_dec.input.block_dh, rds_dec.input.block_dl),
					rds_dec.input.block_bl&0x03,
					&rds_dec.decode.psn);
}
static void base_info_b(void)		   		/*1 0B Basic tuning and switching information only*/
{		
	rds_ms_ta_decode();
	
	rds_psn_decode(	_u8tou16_(rds_dec.input.block_dh, rds_dec.input.block_dl),
					rds_dec.input.block_bl&0x03,
					&rds_dec.decode.psn);
}
static void prog_item_a(void)		   		/*2 1A Program Item Number and slow labeling codes only*/
{}		
static void prog_item_b(void)		   		/*3 1B Program Item Number*/
{}		
static void radio_text_a(void)		   		/*4 2A RadioText only*/
{}		
static void radio_text_b(void)		   		/*5 2B RadioText only*/
{}		
static void ap_id_a(void)			   			/*6 3A Applications Identification for ODA only*/
{}			
static void oda_3_b(void)			  			/*7 3B Open Data Applications*/
{}		
static void date_time_a(void)		   			/*8 4A Clock-time and date only*/
{	
	rds_date_decode();
	
	rds_dec_printf("date_time_a--->year:%d, month:%d, day:%d, hour:%d, min:%d\n", 
						rds_dec.decode.mjd.year + 1900,
						rds_dec.decode.mjd.month,
						rds_dec.decode.mjd.day,
						rds_dec.decode.mjd.hour,
						rds_dec.decode.mjd.min);
}		
static void oda_4_b(void)			  	 		/*9 4B Open Data Applications*/
{}			
static void tdc_a(void)			   			/*10 5A Transparent Data Channels (32 channels) or ODA*/
{}				
static void tdc_b(void)			   			/*11 5B Transparent Data Channels (32 channels) or ODA*/
{}				
static void iha_a(void)			   			/*12 6A In House applications or ODA*/
{}				
static void iha_b(void)			   			/*13 6B In House applications or ODA*/
{}				
static void radio_page_a(void)	 	   		/*14 7A Radio Paging or ODA*/
{}		
static void oda_7_b(void)			   			/*15 7B Open Data Applications*/
{}			
static void tmc_a(void)			   			/*16 8A Traffic Message Channel or ODA*/
{}				
static void oda_8_b(void)			   			/*17 8B Open Data Applications*/
{}			
static void ews_a(void)			   			/*18 9A Emergency Warning System or ODA*/
{}				
static void oda_9_b(void)			   			/*19 9B Open Data Applications*/
{}			
static void pin_a(void)			   			/*20 10A Program Type Name*/
{}			
static void oda_10_b(void)		   	   		/*21 10B Open Data Applications*/
{}			
static void oda_11_a(void)		   	   		/*22 11A Open Data Applications*/
{}			
static void oda_11_b(void)		       		/*23 11B Open Data Applications*/
{}			
static void oda_12_a(void)		       		/*24 12A Open Data Applications*/
{}			
static void oda_12_b(void)		       		/*25 12B Open Data Applications*/
{}			
static void erp_a(void)		       			/*26 13A Enhanced Radio Paging or ODA*/
{}				
static void oda_13_b(void)		       		/*27 13B Open Data Applications*/
{}			
static void eon_a(void)			   			/*28 14A Enhanced Other Networks information only*/
{
	if(rds_dec.ctrl.eon_disale)return;
	
	uint8 index;
	rds_eon_t *now_eon;
	
	now_eon = rds_now_eon_list(_u8tou16_(rds_dec.input.block_dh, rds_dec.input.block_dl));
	if(now_eon == NULL)return;
	
	index = rds_dec.input.block_bl & 0x0F;

	if(rds_dec.input.block_bl & _B4_)
	{
		now_eon->tp = ENABLE;
	}
	else
	{
		now_eon->tp = DISABLE;
	}
	
	switch(index)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			rds_psn_decode( _u8tou16_(rds_dec.input.block_ch, rds_dec.input.block_cl),
					   		index,
					   		&now_eon->psn);
			break;
		case 4:
			rds_af_decode( 	_u8tou16_(rds_dec.input.block_ch, rds_dec.input.block_cl),
					  		&now_eon->af);
			break;
		case 5:
		case 6:
		case 7:
		case 8: 
			if((rds_dec.input.block_cl > 0)&&(rds_dec.input.block_cl <= 204)&&(now_eon->mapped.length < _RDS_EON_MAPPED_MAX_))
			{
				uint8 i;
				uint8 temp1,temp2;
				
				for(i = 0;i < now_eon->mapped.length; i++)
				{
					if(now_eon->mapped.freq[i] == rds_dec.input.block_cl)return;		
				}

				now_eon->mapped.freq[now_eon->mapped.length++] = rds_dec.input.block_cl;	

				for(i = (now_eon->mapped.length-1); i; i--)
				{
					temp1= now_eon->mapped.freq[i];
					temp2= now_eon->mapped.freq[i-1];

					if(temp1 < temp2)
					{
						now_eon->mapped.freq[i]  = temp2;
						now_eon->mapped.freq[i-1]= temp1;	
					}				
				}
			}
			break;
		case 9:/*AM映射，不支持*/
			break;
		
		case 12:break;
		case 13:
			now_eon->pty = (rds_dec.input.block_ch>>3);
		#if 0
			now_eon->ta_have = TRUE;
			
			if(rds_dec.input.block_cl & _B0_)
			{
				now_eon->ta  = ENABLE;
			}
			else
			{
				now_eon->ta  = DISABLE;
			}
		#endif
			break;
		case 14:break;
		default:return;
	}			   		
}						   	
static void eon_b(void)			   			/*29 14B Enhanced Other Networks information only*/
{
	if(rds_dec.ctrl.eon_disale)return;
	
	rds_eon_t *now_eon;

	now_eon = rds_now_eon_list(_u8tou16_(rds_dec.input.block_dh, rds_dec.input.block_dl));
	if(now_eon == NULL)return;
	
	now_eon->ta_have = TRUE;
	
	if(rds_dec.input.block_bl & _B3_)
	{
		now_eon->ta = ENABLE;
	}
	else
	{
		now_eon->ta = DISABLE;
	}
	
	if(rds_dec.input.block_bl & _B4_)
	{
		now_eon->tp = ENABLE;
	}
	else
	{
		now_eon->tp = DISABLE;
	}
}				
static void define_rbds_a(void)       		/*30 15A Defined in RBDS only*/
{
	if(rds_dec.ctrl.type == RDS_TYPE_RBDS)
	{
		rds_ta_decode();
	}
}			
static void fast_info_b(void)		   			/*31 15B Fast switching information only*/
{									
	rds_ms_ta_decode();
}		

typedef void (*rds_droup_func)(void);

static const rds_droup_func rds_group_decode[]=
{
	base_info_a,	base_info_b,	prog_item_a,	prog_item_b,
	radio_text_a,	radio_text_b,	ap_id_a,		oda_3_b,
	date_time_a,	oda_4_b,		tdc_a,			tdc_b,
	iha_a,			iha_b,			radio_page_a,	oda_7_b,
	tmc_a,			oda_8_b,		ews_a,			oda_9_b,
	pin_a,			oda_10_b,		oda_11_a,		oda_11_b,
	oda_12_a,		oda_12_b,		erp_a,			oda_13_b,
	eon_a,			eon_b,			define_rbds_a,	fast_info_b,
};

/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

void rds_stream_dec(const rds_input_t *input)
{
	rds_dec_printf("%s, block_ah:0x%x, block_al:0x%x, block_bh:0x%x, block_bl:0x%x, block_ch:0x%x, block_cl:0x%x, block_dh:0x%x, block_dl:0x%x\n",
						__FUNCTION__,
						input->block_ah, 
						input->block_al,
						input->block_bh,
						input->block_bl,
						input->block_ch,
						input->block_cl,
						input->block_dh,
						input->block_dl);

	rds_dec.input = *input;
	
	rds_dec.decode.pi = _u8tou16_(rds_dec.input.block_ah, rds_dec.input.block_al);
	
	uint16 block = _u8tou16_(rds_dec.input.block_bh, rds_dec.input.block_bl);
	
	rds_dec.decode.pty = (uint8)((block>>5)&0x1F);
	
	if(rds_dec.input.block_bh & _B2_)
	{
		rds_dec.decode.tp = ENABLE;
	}
	else 
	{
		rds_dec.decode.tp = DISABLE;
	}
	
	rds_dec.ctrl.timer_over = (_RDS_OVER_TIME_/rds_dec.det_time);
	
	rds_dec_printf("pi:0x%x, tp:%d, pty:%d, block_index:%d\n", 
						rds_dec.decode.pi, 
						rds_dec.decode.tp, 
						rds_dec.decode.pty, 
						(rds_dec.input.block_bh)>>3);
	
	rds_group_decode[(rds_dec.input.block_bh)>>3]();
}

void rds_block_a_dec(uint8 block_ah, uint8 block_al)
{
	rds_dec.decode.pi = _u8tou16_(block_ah, block_al);
	rds_dec.ctrl.timer_over = (_RDS_OVER_TIME_/rds_dec.det_time);
}

void rds_block_b_dec(uint8 block_bh, uint8 block_bl)
{	
	uint16 block = _u8tou16_(block_bh, block_bl);
	
	rds_dec.decode.pty = (uint8)((block>>5)&0x1F);
	
	if(block_bh & _B2_)
	{
		rds_dec.decode.tp = ENABLE;
	}
	else 
	{
		rds_dec.decode.tp = DISABLE;
	}
	
	rds_dec.ctrl.timer_over = (_RDS_OVER_TIME_/rds_dec.det_time);
}

void rds_block_c_dec(uint8 block_ch, uint8 block_cl)
{
	rds_dec.ctrl.timer_over = (_RDS_OVER_TIME_/rds_dec.det_time);
}

void rds_block_d_dec(uint8 block_dh, uint8 block_dl)
{
	rds_dec.ctrl.timer_over = (_RDS_OVER_TIME_/rds_dec.det_time);
}

void rds_dec_timer(void)
{	
	if(rds_dec.ctrl.timer_over)
	{
		rds_dec.ctrl.timer_over--;
	}
	else
	{
		rds_dec.decode.ta_have	= 0;
		rds_dec.decode.ta  		= 0;
		rds_dec.decode.tp  		= 0;
	}
	
	if(!rds_dec.ctrl.eon_disale)
	{
		if(rds_dec.ctrl.timer_eon)
		{
			rds_dec.ctrl.timer_eon --;
		}
		else
		{
			uint8 index;
			rds_eon_t *now_eon;
			
			for(index = 0; index < _RDS_EON_LIST_MAX_; index++)
			{
				now_eon = &rds_dec.decode.eon[index];
				if(now_eon->pi)
				{
					if(rds_dec.ctrl.timer_eon_list[index])
					{
						rds_dec.ctrl.timer_eon_list[index] --;
					}
					else
					{
						now_eon->ta = 0;
						now_eon->tp = 0;
					}
				}
			}
		}
	}
}

void rds_dec_reset(void)
{
	memset(&rds_dec.decode, 0, sizeof(rds_dec.decode));	
	
	rds_dec.ctrl.timer_over = 0;
	rds_dec.ctrl.timer_eon  = 0;
	memset(rds_dec.ctrl.timer_eon_list, 0, sizeof(rds_dec.ctrl.timer_eon_list));
}

void rds_dec_info(radio_rds_info_t *rds_info)
{
	rds_info->pi 	 	= rds_dec.decode.pi;
	rds_info->ta 	 	= rds_dec.decode.ta;
	rds_info->ta_have	= rds_dec.decode.ta_have;
	rds_info->tp 	 	= rds_dec.decode.tp;
	rds_info->pty 	 	= rds_dec.decode.pty;
}

void rds_dec_app(radio_rds_app_t *rds_app)
{
	if(rds_dec.ctrl.timer_over) 
	{
		rds_app->af_det = TRUE;
	}
	else
	{
		rds_app->af_det = FALSE;
	}
	if(rds_dec.ctrl.timer_eon) 
	{
		rds_app->eon_det = TRUE;
	}
	else
	{
		rds_app->eon_det = FALSE;
	}
	
	rds_app->pi 	 = rds_dec.decode.pi;
	rds_app->ms 	 = rds_dec.decode.ms;
	rds_app->ta 	 = rds_dec.decode.ta;
	rds_app->ta_have = rds_dec.decode.ta_have;
	rds_app->tp 	 = rds_dec.decode.tp;
	rds_app->pty 	 = rds_dec.decode.pty;
	
	if(rds_dec.decode.psn.succeed)
	{
		rds_app->psn.succeed = TRUE;
		memcpy(rds_app->psn.buffer, rds_dec.decode.psn.data, 8);
	}
	else
	{
		rds_app->psn.succeed = FALSE;
	}
	
	if(rds_dec.decode.af.state == RDS_AF_STATE_SUCCEED)
	{
		rds_app->af.succeed	= TRUE;
		rds_app->af.method	= rds_dec.decode.af.method;
		memcpy(rds_app->af.buffer, rds_dec.decode.af.buffer, _RDS_AF_LIST_MAX_);
	}
	else
	{
		rds_app->af.succeed	= FALSE;
	}
	
	if(rds_dec.decode.mjd.succeed)
	{
		rds_app->date.succeed 	= TRUE;
		rds_app->date.year  	= rds_dec.decode.mjd.year + 1900;
		rds_app->date.month 	= rds_dec.decode.mjd.month;
		rds_app->date.day		= rds_dec.decode.mjd.day;
		rds_app->date.hour		= rds_dec.decode.mjd.hour;
		rds_app->date.min		= rds_dec.decode.mjd.min;
	}
	else
	{
		rds_app->date.succeed = FALSE;
	}
	
	uint8 index;
	radio_rds_eon_t *dest_eon;
	rds_eon_t *sour_eon;
	
	for(index = 0; index < _RDS_EON_LIST_MAX_; index++)
	{
		dest_eon = &rds_app->eon[index];
		sour_eon = &rds_dec.decode.eon[index];
		
		dest_eon->pi 		= sour_eon->pi;
		dest_eon->ta 		= sour_eon->ta;
		dest_eon->ta_have	= sour_eon->ta_have;
		dest_eon->tp 		= sour_eon->tp;
		dest_eon->pty		= sour_eon->pty;
		
		if(sour_eon->psn.succeed)
		{
			dest_eon->psn.succeed = TRUE;
			memcpy(dest_eon->psn.buffer, sour_eon->psn.data, 8);
		}
		else
		{
			dest_eon->psn.succeed = FALSE;
		}
		
		if(sour_eon->af.state == RDS_AF_STATE_SUCCEED)
		{
			dest_eon->af.succeed = TRUE;
			dest_eon->af.method	 = sour_eon->af.method;
			memcpy(dest_eon->af.buffer, sour_eon->af.buffer, _RDS_AF_LIST_MAX_);
		}
		else
		{
			dest_eon->af.succeed = FALSE;
		}
		
		memcpy(&dest_eon->mapped, &sour_eon->mapped, sizeof(dest_eon->mapped));
	}
}

void rds_dec_load(radio_rds_station_t *station)
{
	rds_dec_puts("rds_dec_load\n");
	if(rds_dec.decode.pi)
	{
		station->pi  = rds_dec.decode.pi;
		station->ms  = rds_dec.decode.ms;
		station->pty = rds_dec.decode.pty;
		
		if(rds_dec.decode.psn.succeed)
		{
			station->psn_succeed = TRUE;
			memcpy(station->psn_buffer, rds_dec.decode.psn.data, 8);
		}
		
		if(rds_dec.decode.af.state == RDS_AF_STATE_SUCCEED)
		{
			station->af_succeed	= TRUE;
			station->af_method	= rds_dec.decode.af.method;
			memcpy(station->af_buffer, rds_dec.decode.af.buffer, _RDS_AF_LIST_MAX_);
		}
	}
}

void rds_dec_save(const radio_rds_station_t *station)
{
	rds_dec_puts("rds_dec_save\n");
	if(station->pi)
	{
		rds_dec.decode.pi	= station->pi;
		rds_dec.decode.ms	= station->ms;
		rds_dec.decode.pty 	= station->pty;
		
		if(station->psn_succeed)
		{
			rds_dec.decode.psn.succeed = TRUE;
			rds_dec.decode.psn.ctrl    = 0x0F;
			memcpy(rds_dec.decode.psn.buffer, station->psn_buffer, 8);
			memcpy(rds_dec.decode.psn.data, station->psn_buffer, 8);
		}
		
		if(station->af_succeed)
		{
			rds_dec.decode.af.state	= RDS_AF_STATE_SUCCEED;
			rds_dec.decode.af.method = station->af_method;
			memcpy(rds_dec.decode.af.buffer, station->af_buffer, _RDS_AF_LIST_MAX_);
		}
	}
}

void rds_dec_eon(uint8 disable)
{
	rds_dec.ctrl.eon_disale = disable;
	rds_dec_printf("eon_disale:%d\n", disable);
}

void rds_dec_det_time(uint16 det_time)
{
	rds_dec.det_time = det_time;
	rds_dec_printf("det_time:%d\n", det_time);
}

#endif

