/*
*********************************************************************************************************
*                                             BR17
*
*                                             CODE
*
*                          (c) Copyright 2015-2016, ZHUHAI JIELI
*                                           All Rights Reserved
*
* File : *
* By   : jamin.li
* DATE : 11/11/2015 build this file
*********************************************************************************************************
*/
#include "sdk_cfg.h"
#include "cbuf/circular_buf.h"
#include "dac/dac.h"
#include "dac/ladc.h"
#include "rtos/task_manage.h"
#include "aec/aec_api.h"
#include "encode/encode.h"
#include "dac/dac_api.h"
#include "cpu/dac_param.h"
#include "linein.h"
#include "dac/src_api.h"
#include "echo/echo_api.h"

#define ECHO_MIC_AUTO_MUTE	0

#define	LADC_DATA_2_DAC     0  ///define for debug
#define LADC_SRC_EN			0
static cbuffer_t adc_cb;
volatile u8 ladc_ch = 0;


#define max_cnt 1600
#define wVoiceThreshold (20000UL)
#define wVoiceCnt 4

u32 adc_digit_energy_value;
u8 bAutoMute = 0;
u8 bMute = 0;
static s32 abs(s32 a);
static void mic_auto_mute(void)
{
	static u16 mute_cnt =0;
	if (adc_digit_energy_value < wVoiceThreshold){
		if (mute_cnt < max_cnt){
			mute_cnt++;
		}
		else if (mute_cnt >= max_cnt){
			if (bAutoMute != 1){
				puts("Auto Mute");
				bAutoMute = 1;
				mic_2_LR(0, 0);
			}
		}
	}
	else{
		if (bMute == 0){
			if (0 != bAutoMute){
				puts("Auto UnMute");
				bAutoMute = 0;
				mic_2_LR(1, 1);
			}
		}
		mute_cnt = 0;
	}
}

static u32 adc_energy_value(void *buffer,u16 len)
{
	static u32 total_dac_data_value=0;
	static u8 data_count=0;
	u32 dac_energy_value=0;
	u32 i;

	s16 *buf = buffer;
	len >>=1;

	for(i = 0; i < len; i++){
		dac_energy_value += abs(buf[i]);
	}
	total_dac_data_value += dac_energy_value;
	data_count++;
	if (data_count >= wVoiceCnt){
		data_count=0;
		adc_digit_energy_value = total_dac_data_value;
		total_dac_data_value = 0;
	}

	//put_u32hex(adc_digit_energy_value);
	mic_auto_mute();
	return adc_digit_energy_value;
}


/*
 *********************************************************************************************************
 *
 * Description: ladc buf��ʼ��
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note: �ϵ�ֻ��һ�Σ���Դ�����ͷ�
 *********************************************************************************************************
 */
void ladc_buf_init(void)
{
    u8 *outbuf;
    outbuf = malloc(1024);
    if (outbuf == 0)
    {
        puts("out buf malloc err\r\n");
    }

    cbuf_init(&adc_cb, outbuf, 512);
}

/*
 *********************************************************************************************************
 *
 * Description: ladc���� ��Դ���룬���ò����ص�����
 * Arguments  : max_channel--����ͨ������, channel--����ͨ������, call_back--��������ص�
 *
 * Returns    : ladc �ṹָ��
 *
 * Note: �� ladc_reg_init֮ǰ���ã��������Դ�ɵ����������Ƿ�
 *********************************************************************************************************
 */
LADC_CTL * ladc_ctl_init(u8 max_channel, u8 channel, void * call_back)
{
    LADC_CTL *ladc;
    u8 i;

    ladc = (LADC_CTL *) calloc(1, sizeof(LADC_CTL));
    ASSERT(ladc);

    ladc->source_chl = channel;

    ///����C buffer���룺rec_ctl->ladc.c_buff
    for (i = 0; i < max_channel; i++)
    {
        ladc->buff[i] = calloc(1, DAC_DUAL_BUF_LEN * 50);
        ASSERT(ladc->buff[i]);

        ladc->c_buff[i] = calloc(1, sizeof(cbuffer_t)); //<���뱣��LADC buffer��C buffer
        ASSERT(ladc->c_buff[i]);

        cbuf_init(ladc->c_buff[i], ladc->buff[i], DAC_DUAL_BUF_LEN * 50);
    }

    ladc->save_ladc_buf = (void(*)(void *, void *, u8, u32)) call_back; //дLADC���ݵ�C buffer�ĺ���

    return ladc;
}


/*
 *********************************************************************************************************
 *
 * Description: ladc������Դ�ͷ�
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note: �� disable_ladc ֮�����,�ϲ㲻���øú�����Ҳ�ɰ��ṹ����Դ�����ͷ�
 *********************************************************************************************************
 */
void ladc_ctl_free(LADC_CTL *ladc)
{
    u8 i;

    if(ladc != NULL)
    {
        for (i = 0; i < 4; i++)
        {
            if(NULL != ladc->c_buff[i])
            {
                free(ladc->c_buff[i]);
                ladc->c_buff[i] = NULL;
            }

            if(NULL != ladc->buff[i])
            {
                free(ladc->buff[i]);
                ladc->buff[i] = NULL;
            }
        }

        free(ladc);
    }
}
/*
 *********************************************************************************************************
 *
 * Description: read ladc cbuff��������
 * Arguments  : s16 * buf, u32 len
 *
 * Returns    : ʵ�ʶ��������ݳ���
 *
 * Note:
 *********************************************************************************************************
 */
u32 cbuf_read_adc(s16 * buf, u32 len)
{
    return cbuf_read(&adc_cb, buf, len);
}

/*
 *********************************************************************************************************
 *
 * Description: ��ͨ����˫ͨ��������copy
 * Arguments  : dest_buf,buf,point_cnt--��������
 *
 * Returns    : none
 *
 * Note:
 *********************************************************************************************************
 */

void ladc_one_to_two(u16 *dest_buf,u16 *buf,u16 point_cnt)
{
    int cnt,cnt2;
    cnt2 = 0;
    for(cnt = 0; cnt < point_cnt; cnt++)
    {
        dest_buf[cnt2] = buf[cnt];
        dest_buf[cnt2+1] = buf[cnt];
        cnt2+=2;
    }
}

/*
 *********************************************************************************************************
 *
 * Description: ladc dual channel,combine ladc channel buf
 * Arguments  : ladc_l = left adc channel buf
 *				ladc_r = right adc channel buf
 * Returns    : none
 *
 * Note:
 *********************************************************************************************************
 */
void ladc_stereo(s16 *ladc_l,s16 *ladc_r,u16 point_cnt,s16 *dest_buf)
{
	u8 i;
	for(i = 0; i < point_cnt; i++)
	{
		dest_buf[i*2]   = ladc_l[i];
		dest_buf[i*2+1] = ladc_r[i];
	}
}

/*
 *********************************************************************************************************
 *
 * Description: calculate data energy
 * Arguments  : buffer = source_data
 *				len = data length(byte)
 *				packet_cnt = total calculate packets
 * Returns    : none
 *
 * Note:
 *********************************************************************************************************
 */
static s32 abs(s32 a)
{
    if(a > 0){
        return a;
    }else{
        return (-a);
    }
}
u32 data_energy_value(void *buffer,u16 len,u16 packet_cnt)
{
    static u32 total_res = 0;
	static u32 digital_energy = 0;
    static u16 cnt = 0;
    u32 res = 0;
    u32 i;

    s16 *buf = buffer;
    len >>= 1;//convert to point


    for(i = 0; i < len; i++)
    {
        res += abs(buf[i]);
    }
    total_res += res;
    cnt++;
    if (cnt >= packet_cnt)
    {
        cnt = 0;
        digital_energy = total_res;
        total_res = 0;
    }
    return digital_energy;
}
/*
 *********************************************************************************************************
 *
 * Description: digital_volume_ctl 
 * Arguments  : buffer 
 *				point_len 
 *				ch = 1(mono),2(stereo)
 * Returns    : none
 *
 * Note:
 *********************************************************************************************************
 */
extern const u16 digital_vol_tab[DIGITAL_VOL_MAX_L+1];
volatile u8 digital_vol = 31;
void digital_volume_ctl(void *buffer,u16 point_len,u8 ch)
{
    s32 valuetemp;
    u32 i;
    u16 curtabvol,curtabvor;

    s16 * buf = buffer;

	//get digital_vol
    curtabvol = digital_vol_tab[digital_vol];
    curtabvor = curtabvol;

	if(ch == 2)
	{
		//stereo
		for(i=0; i<(point_len<<1); i+=2)
		{
			///left channel
			valuetemp = buf[i];
			valuetemp = (valuetemp * curtabvol) >> 14 ;
			if(valuetemp < -32768) {
				valuetemp = -32768;
			}
			else if(valuetemp > 32767) {
				valuetemp = 32767;
			}
			buf[i] = (s16)valuetemp;

			///right channel
			valuetemp = buf[i+1];
			valuetemp = (valuetemp * curtabvor) >> 14 ;
			if(valuetemp < -32768) {
				valuetemp = -32768;
			}
			else if(valuetemp > 32767) {
				valuetemp = 32767;
			}
			buf[i+1] = (s16)valuetemp;
		}
	}
	else 
	{
		//mono
		for(i=0; i<point_len; i++)
		{
			valuetemp = buf[i];
			valuetemp = (valuetemp * curtabvol) >> 14 ;
			if(valuetemp < -32768) {
				valuetemp = -32768;
			}
			else if(valuetemp > 32767) {
				valuetemp = 32767;
			}
			buf[i] = (s16)valuetemp;
		}
	}
}

#if 0
static const u8 sine_buf_32K[] =
{
    /* 0x00, 0x00, 0x00, 0x00, 0xae, 0x11, 0xae, 0x11, 0xad, 0x22, 0xad, 0x22, 0x58, 0x32, 0x58, 0x32, 0x13, */
    /* 0x40, 0x13, 0x40, 0x58, 0x4b, 0x58, 0x4b, 0xb8, 0x53, 0xb8, 0x53, 0xe0, 0x58, 0xe0, 0x58, 0x9e, 0x5a, */
    /* 0x9e, 0x5a, 0xe0, 0x58, 0xe0, 0x58, 0xb8, 0x53, 0xb8, 0x53, 0x58, 0x4b, 0x58, 0x4b, 0x13, 0x40, 0x13, */
    /* 0x40, 0x58, 0x32, 0x58, 0x32, 0xad, 0x22, 0xad, 0x22, 0xae, 0x11, 0xae, 0x11, 0x00, 0x00, 0x00, 0x00, */
    /* 0x52, 0xee, 0x52, 0xee, 0x53, 0xdd, 0x53, 0xdd, 0xa8, 0xcd, 0xa8, 0xcd, 0xed, 0xbf, 0xed, 0xbf, 0xa8, */
    /* 0xb4, 0xa8, 0xb4, 0x48, 0xac, 0x48, 0xac, 0x20, 0xa7, 0x20, 0xa7, 0x62, 0xa5, 0x62, 0xa5, 0x20, 0xa7, */
    /* 0x20, 0xa7, 0x48, 0xac, 0x48, 0xac, 0xa8, 0xb4, 0xa8, 0xb4, 0xed, 0xbf, 0xed, 0xbf, 0xa8, 0xcd, 0xa8, */
    /* 0xcd, 0x53, 0xdd, 0x53, 0xdd, 0x52, 0xee, 0x52, 0xee */

	0x00, 0x00, 0xae, 0x11, 0xad, 0x22, 0x58, 0x32,
	0x13, 0x40, 0x58, 0x4b, 0xb8, 0x53, 0xe0, 0x58,
	0x9e, 0x5a, 0xe0, 0x58, 0xb8, 0x53, 0x58, 0x4b,
	0x13, 0x40, 0x58, 0x32, 0xad, 0x22, 0xae, 0x11,
	0x00, 0x00, 0x52, 0xee, 0x53, 0xdd, 0xa8, 0xcd,
	0xed, 0xbf, 0xa8, 0xb4, 0x48, 0xac, 0x20, 0xa7,
	0x62, 0xa5, 0x20, 0xa7, 0x48, 0xac, 0xa8, 0xb4,
	0xed, 0xbf, 0xa8, 0xcd, 0x53, 0xdd, 0x52, 0xee
};
#endif
extern void aux_ladc_cbuf_write(u8 *data,u16 length);
void ladc_to_dac(void * buf,u32 len)
{
	u8 l2d_buf[DAC_DUAL_BUF_LEN*2];
	s16 * sp = buf;//(void*)sine_buf_32K;//buf;
	s16 * dp = (s16 *)l2d_buf;
	u8 cnt;

	for(cnt = 0; cnt<(len/2); cnt++)
	{
		dp[cnt*2]=sp[cnt];
		dp[cnt*2+1]=sp[cnt];
	}
	/* cbuf_write_dac(sine_buf_32K,sizeof(sine_buf_32K)); */

#if USE_SOFTWARE_EQ
	aux_ladc_cbuf_write((u8*)dp,len*2);
#else
	cbuf_write_dac(dp,len*2);
#endif
}

void ladc_stereo_to_dac(void *buf_l,void *buf_r,u32 len)
{
	u8 l2d_buf[DAC_DUAL_BUF_LEN*2];
	s16 * sp_l = buf_l;
	s16 * sp_r = buf_r;
	s16 * dp = (s16 *)l2d_buf;
	u8 cnt;

	for(cnt = 0; cnt<(len/2); cnt++)
	{
		dp[cnt*2]=sp_l[cnt];
		dp[cnt*2+1]=sp_r[cnt];
	}
#if USE_SOFTWARE_EQ
	aux_ladc_cbuf_write((u8*)dp,len*2);
#else
	cbuf_write_dac(dp,len*2);
#endif
}

//*********************************************************
//                   interrupt
//*********************************************************
static void sr_16k_2_8K(void * buf, u32 len)
{
	u16 i;
    s16 *res = buf;
	len >>= 2;//	x/2point 	x/2/2->16K28K

	for(i = 0; i < len; i++)
	{
		res[i] = res[i*2];
	}
}

/*----------------------------------------------------------------------------*/
/**@brief  src �����ʹ������
   @param  flag -- ����
   @param
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
static void ladc_input_fun(LADC_CTL *ladc_p, void *ladc_buf,u32 buf_flag,u32 buf_len);
#if LADC_SRC_EN
volatile u8 src_toggle = 0;
static void src_output_fun(s16 *buf, u32 len)///len��byteΪ��λ
{
	//printf(" [%d] ",len);
	ladc_input_fun(p_ladc, buf, 0, len);
}
static void src_md_ctl(int flag,u16 in_rate,u16 out_rate)
{
    src_param_t src_p;

    if(flag == 1)
    {
		if(src_toggle)
			return;
        puts("src_start\n");
		src_toggle = 1;
        src_p.in_chinc = 1;
        src_p.in_spinc = 1;
        src_p.out_chinc = 1;
        src_p.out_spinc = 1;
        src_p.in_rate = in_rate;
        src_p.out_rate = out_rate;
        src_p.nchannel = 1;
        src_p.output_cbk = (void *)src_output_fun;
        src_init_api(&src_p);
    }
    else
    {
		if(src_toggle)
		{
			src_toggle = 0;
        	src_exit_api();
        	puts("src_stop\n");
		}
    }
}
#endif

static void ladc_input_fun(LADC_CTL *ladc_p, void *ladc_buf,u32 buf_flag,u32 buf_len)
{
    u8  i;
    s16 *res;
	res = (s16*)ladc_buf;

    if (NULL != ladc_p)
    {
        if (!ladc_p->save_ladc_buf) {
			puts("WARNING:ladc_p->save_ladc_buf is NULL\n");
			return;
		}

        if (ladc_p->source_chl == ENC_MIC_CHANNEL)
        {
             //mic������
#if ECHO_MIC_AUTO_MUTE
			adc_energy_value(res, DAC_DUAL_BUF_LEN);
			if(bAutoMute)
			{
				memset(res, 0x00, DAC_DUAL_BUF_LEN);
			}
#endif
            ladc_p->save_ladc_buf((void *)ladc_p, res, 0, buf_len);
        }
        else if(ladc_p->source_chl == ENC_LINE_LEFT_CHANNEL)
        {
            for(i = 0; i < 2; i++) //��������
            {
                ladc_p->save_ladc_buf((void *) ladc_p, res, i, buf_len);
            }
        }
        else if(ladc_p->source_chl == ENC_LINE_RIGHT_CHANNEL)
        {
            for(i = 0; i < 2; i++) //��������
            {
                ladc_p->save_ladc_buf((void *) ladc_p, res, i, buf_len);
            }
        }
        else if(ladc_p->source_chl == ENC_LINE_LR_CHANNEL)
        {
            //��������
            ladc_p->save_ladc_buf((void *) ladc_p, res, 0, buf_len);
            ladc_p->save_ladc_buf((void *) ladc_p, (res+DAC_DUAL_BUF_LEN), 1, buf_len);
        }
        else {
            /* putchar('E'); */
        }
    }
}

void ladc_isr_callback(void *ladc_buf,u32 buf_flag,u32 buf_len)
{
#if LADC_SRC_EN
	u32 wlen;
#endif

	//u32 energy_value = 0;
    s16 *res;
	s16 *ladc_l,*ladc_r,*ladc_mic;

	//dual buf
	res = (s16*)ladc_buf;
	res = res + (buf_flag * DAC_SAMPLE_POINT);

	//ladc_buf offset
	ladc_l = res;
	ladc_r = res + DAC_DUAL_BUF_LEN;
	ladc_mic = res + (2*DAC_DUAL_BUF_LEN);

	//digital_vol_ctl
	//digital_volume_ctl(ladc_l,DAC_SAMPLE_POINT,1);
	//digital_volume_ctl(ladc_r,DAC_SAMPLE_POINT,1);
#if ECHO_EN
	sync_adc_sr(ladc_mic);
#endif
#if LADC_SRC_EN
	if(src_toggle) {
		wlen = src_write_data_api((u8*)ladc_mic,buf_len);
	} else {
		ladc_input_fun(p_ladc, ladc_mic, buf_flag, buf_len);
	}
#else
	ladc_input_fun(p_ladc, ladc_mic, buf_flag, buf_len);
#endif

	if (!strcmp(curr_task->name, RECORD_TASK_NAME)){
		ladc_input_fun(p_ladc_rec, ladc_mic, buf_flag, buf_len);
	}
	else{
#if (REC_SOURCE == 0)//0: rec source = ladc
		ladc_input_fun(p_ladc_rec, ladc_mic, buf_flag, buf_len);
#endif
	}

#if AUX_AD_ENABLE
#if defined(_SUPPORT_TAPE_)
	if(!strcmp(curr_task->name, LINEIN_TASK_NAME)||!strcmp(curr_task->name, TAPE_TASK_NAME))
#elif defined(_ENABLE_RECORD_RADIO_)
	if(!strcmp(curr_task->name, LINEIN_TASK_NAME)||!strcmp(curr_task->name, FM_TASK_NAME))
#elif defined(_ENABLE_MIC_REC_)
	if(!strcmp(curr_task->name, LINEIN_TASK_NAME)||!strcmp(curr_task->name, LINEIN_TASK_NAME))		
#else
	if(!strcmp(curr_task->name, LINEIN_TASK_NAME))
#endif
	{
		if((LINEIN_CHANNEL >= DAC_AMUX0)&&(LINEIN_CHANNEL <= DAC_AMUX2)) {
			//putchar('2');//ladc stereo
			ladc_stereo_to_dac(ladc_l,ladc_r,DAC_DUAL_BUF_LEN);
		} else {
			//putchar('1');//ladc mono
			if((LINEIN_CHANNEL == DAC_AMUX0_L_ONLY) || (LINEIN_CHANNEL == DAC_AMUX1_L_ONLY) || \
					(LINEIN_CHANNEL == DAC_AMUX2_L_ONLY) ||(LINEIN_CHANNEL == DAC_AMUX_DAC_L))
			{
				ladc_to_dac(ladc_l, DAC_DUAL_BUF_LEN);
			}
			else {
				ladc_to_dac(ladc_r, DAC_DUAL_BUF_LEN);
			}
		}
	}
#endif

#if LADC_DATA_OUPUT_DAC_SW
	s16 tmp_ladc_buf[DAC_SAMPLE_POINT * 2] ;
	/*ENC_LINE_LEFT_CHANNEL, ENC_LINE_RIGHT_CHANNEL or ENC_MIC_CHANNEL*/
	ladc_one_to_two((u16 *)tmp_ladc_buf, res, DAC_SAMPLE_POINT);
	/*ENC_LINE_LR_CHANNEL*/
	//ladc_stereo(res,res+DAC_DUAL_BUF_LEN,DAC_SAMPLE_POINT,tmp_ladc_buf);
    cbuf_write_dac(tmp_ladc_buf,DAC_BUF_LEN);
#endif

    ///*********������***************///
	if (curr_task) {
	    //energy_value = data_energy_value((s16 *)res, DAC_SAMPLE_POINT*2,1);
		//printf("------energy_value=%d\n",energy_value);

	    if (!strcmp(curr_task->name, BT_TASK_NAME))
	    {
             if(aec_interface.fill_adc_ref_buf)
             {
#if BT_KTV_EN
				 sr_16k_2_8K(ladc_mic, 32 * sizeof(short));
                 aec_interface.fill_adc_ref_buf(ladc_mic, 32 * sizeof(short) / 2); //��������
#else
                 aec_interface.fill_adc_ref_buf(ladc_mic, 32 * sizeof(short)); //��������
#endif
             }
         }
#if USB_PC_EN
	    if (!strcmp(curr_task->name, UDISK_TASK_NAME))
		{
			usb_mic_ladc_input(ladc_mic,DAC_SAMPLE_POINT);
		}
#endif
     }
}

//*********************************************************
//                        LADC API
//*********************************************************
void ladc_reg_init(u8 ch_sel, u16 sr)
{
	ladc_ch |= BIT(ch_sel);
	printf("ladc_open:%d    %d    sr=%d\n",ch_sel,ladc_ch,sr);
#if LADC_SRC_EN
	if((ladc_ch & BIT(ENC_MIC_CHANNEL)) && (ladc_ch & (~BIT(ENC_MIC_CHANNEL))))
	{
		puts("multi_ladc_ch\n");
		sr = 48000;
		src_md_ctl(1,sr,16000);
		dac_set_samplerate(48000,0);
	}
#endif
	ladc_reg_isr_cb_api(ladc_isr_callback);
	ladc_enable(ch_sel,sr,VCOMO_EN);
}

void ladc_close(u8 ch_sel)
{
	ladc_ch &= ~BIT(ch_sel);
	printf("ladc_close:%d	%d\n",ch_sel,ladc_ch);
	ladc_disable(ch_sel);
#if LADC_SRC_EN
	if((ladc_ch & BIT(ENC_MIC_CHANNEL)) == 0) {
		src_md_ctl(0,0,0);
	}
#endif
}


//*********************************************************
//                        DAA API
//*********************************************************
void microphone_open(u8 mic_gain,u8 mic_gx2)
{
	//puts("microphone_open\n");

	dac_mute(1,1);

	//init mic IO
	JL_PORTB->DIR |=  BIT(13);
	JL_PORTB->DIE &= ~BIT(13);
	JL_PORTB->PD  &= ~BIT(13);
	JL_PORTB->PU  &= ~BIT(13);

	ladc_adcude(1);
	ladc_mic_gain(mic_gain,mic_gx2);
	ladc_mic_en(1);
	mic_2_LR(1,1);
	dac_mute(0,1);
	ladc_adcude(0);
}

void microphone_close(void)
{
	//puts("microphone_close\n");
	ladc_mic_en(0);
	mic_2_LR(0,0);
}
