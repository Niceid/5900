#include "sdk_cfg.h"
#include "dac/dac_api.h"
#include "dac/eq_api.h"
#include "cbuf/circular_buf.h"
#include "vm/vm_api.h"
#include "key_drv.h"
#include "dac/ladc.h"
#include "cpu/dac_param.h"
#include "dac/dac.h"
#include "dac/eq.h"
#include "timer.h"
#include "play_sel/play_sel.h"
#include "fm/fm_radio.h"
#include "aec/aec_api.h"
#include "iis/dac2iis.h"

#include "echo/echo_api.h"



#define DAC_SINE_DEBUG	0

u8 dac_read_en=0;
DAC_CTL dac_ctl;

/*----------------------------------------------------------------------------*/
/** @brief:?????,?????????????,
                ???????DIGITAL_VOL_MAX_L+1
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
const u16 digital_vol_tab[DIGITAL_VOL_MAX_L+1]=
{
    0	  ,
    93	  ,
    111	  ,
    132	  ,
    158	  ,
    189	  ,
    226	  ,
    270	  ,
    323	  ,
    386	  ,//9
    462	  ,
    552	  ,
    660	  ,
    789	  ,
    943	  ,
    1127  ,
    1347  ,
    1610  ,
    1925  ,
    2301  ,//19
    2751  ,
    3288  ,
    3930  ,
    4698  ,
    5616  ,
    6713  ,
    8025  ,
    9592  ,
    11466 ,
    15200 ,//29
    16000 ,
    16384
};

u32 dac_output_counter()
{
    return 0;
}

void dac_set_remote_counter(u32 counter,u32 time)
{

}

void dac_int_enable(void)
{
	dac_read_en = 0;
	dac_ie_api(1);
#if BT_TWS
	dac_mute(0,0);
#else
	dac_mute(0,1);
#endif
}
void dac_int_disable(void)
{
#if BT_TWS
	dac_mute(1,0);
#else
	dac_mute(1,1);
#endif
	dac_ie_api(0);
}

void dac_sr_cb(u16 rate)
{
	//printf("dac_sr_cb,rate = %d\n",rate);
#if EQ_EN
	eq_samplerate_sw(rate);
#endif

	set_key_voice_sr(rate);
#if DAC2IIS_EN
    dac2iis_sr(rate);
#endif
}

void dac_power_on_time(u8 mode)
{
	if(mode == 2) {
		puts("dac_off_delay\n");
		os_time_dly(15);
		//delay(0xAFFF*70);//120ms

	}
	else if(mode == 1) {
		puts("dac_power_on quickly\n");
		//delay(0xAFFF*70);//120ms
		os_time_dly(15);
	}
	else {
		puts("dac_power_on slowly\n");
		delay(0xAFFF*300);//600ms
	}
}

const u8 sine_buf_32K[] =
{
    0x00, 0x00, 0x00, 0x00, 0xae, 0x11, 0xae, 0x11, 0xad, 0x22, 0xad, 0x22, 0x58, 0x32, 0x58, 0x32, 0x13,
    0x40, 0x13, 0x40, 0x58, 0x4b, 0x58, 0x4b, 0xb8, 0x53, 0xb8, 0x53, 0xe0, 0x58, 0xe0, 0x58, 0x9e, 0x5a,
    0x9e, 0x5a, 0xe0, 0x58, 0xe0, 0x58, 0xb8, 0x53, 0xb8, 0x53, 0x58, 0x4b, 0x58, 0x4b, 0x13, 0x40, 0x13,
    0x40, 0x58, 0x32, 0x58, 0x32, 0xad, 0x22, 0xad, 0x22, 0xae, 0x11, 0xae, 0x11, 0x00, 0x00, 0x00, 0x00,
    0x52, 0xee, 0x52, 0xee, 0x53, 0xdd, 0x53, 0xdd, 0xa8, 0xcd, 0xa8, 0xcd, 0xed, 0xbf, 0xed, 0xbf, 0xa8,
    0xb4, 0xa8, 0xb4, 0x48, 0xac, 0x48, 0xac, 0x20, 0xa7, 0x20, 0xa7, 0x62, 0xa5, 0x62, 0xa5, 0x20, 0xa7,
    0x20, 0xa7, 0x48, 0xac, 0x48, 0xac, 0xa8, 0xb4, 0xa8, 0xb4, 0xed, 0xbf, 0xed, 0xbf, 0xa8, 0xcd, 0xa8,
    0xcd, 0x53, 0xdd, 0x53, 0xdd, 0x52, 0xee, 0x52, 0xee
};
extern u32 get_dac_open_clk();
extern int __get_stereo_en_flag();
int dac_cbuf_enough(void)
{
	if(get_dac_open_clk()&&__get_stereo_en_flag())
	{
		return 0;
	}
	else
	{
		if((get_dac_cbuf_len() >= OUTPUT_BUF_SIZE /2)
#if (ECHO_EN||PITCH_EN)
		||(cbuf_get_data_len_echo()>= (cbuf_get_space_len_echo()/2))
#endif
		)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
extern void bt_rec_get_data(s16 *buf,u32 len);//????pcm????
/*----------------------------------------------------------------------------*/
/** @brief:  DAC isr callback function
    @param:
    @return:
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/
u32 read_buf[DAC_SAMPLE_POINT] AT(.dac_buf_sec);
extern int check_a2dp_en();
extern void inc_dac_cnt(u32 dac_points);
#if REC_EN
extern RECORD_OP_API * rec_bt_api;
extern RECORD_OP_API * rec_fm_api;
extern RECORD_OP_API * rec_aux_api;
#if defined(_SUPPORT_TAPE_)
extern RECORD_OP_API * rec_tape_api;
#endif
#endif // REC_EN
void dac_isr_callback(void *dac_buf,u8 buf_flag)
{
#if DAC2IIS_EN
	return;
#endif
#if DAC_SINE_DEBUG
	memcpy(dac_buf,sine_buf_32K,128);
	return;
#endif
#if BT_TWS
	inc_dac_cnt(32);
#endif

	if(0 == dac_read_en)
	{
		if(dac_cbuf_enough() != 0)	{
			memset(dac_buf,0x00,DAC_BUF_LEN);
			add_key_voice((s16*)dac_buf,DAC_SAMPLE_POINT*2);
			dac_digit_energy_value(dac_buf, DAC_BUF_LEN);
#if (ECHO_EN||PITCH_EN)
			cbuf_mixture_echo(dac_buf,DAC_BUF_LEN);
			rd_da((s16 *)dac_buf);
#endif
			return;
		}
		dac_read_en = 1;
	}

	if(dac_read_en)
	{
	 if(dac_read(read_buf,DAC_BUF_LEN) != DAC_BUF_LEN) {
		 dac_read_en = 0;
	 }
	}
	add_key_voice((s16*)read_buf,DAC_SAMPLE_POINT*2);
	
#if defined(_AUDIO_DRIVER_AP3768_)
	extern u8 rds_on;
	if(rds_on) {
		memset(read_buf,0x00,DAC_BUF_LEN);
	}
#endif

	if(is_dac_mute()) {
		memset(read_buf,0x00,DAC_BUF_LEN);
	}
	else {
#if VOCAL_REMOVER
		if((vocal_flag)&&(get_decode_nch() == 2)){
			dac_digital_lr_sub(read_buf,DAC_BUF_LEN);
		}
#endif

#if DAC_SOUNDTRACK_COMPOUND
		dac_digital_lr2one(read_buf,DAC_BUF_LEN);
#endif
	}

	dac_digit_energy_value(read_buf, DAC_BUF_LEN);
    digital_vol_ctrl(read_buf, DAC_BUF_LEN);

#if (ECHO_EN||PITCH_EN)
	if(fm_mode_var && ((fm_mode_var->scan_mode >= FM_SCAN_BUSY)|| (fm_mode_var->fm_mute)))	 ///FM������̨��ֻ��Ӧ���ְ��� //MUTE
	{
	}
	else
	{
		cbuf_mixture_echo(read_buf,DAC_BUF_LEN);
		rd_da((s16 *)read_buf);
	}
#endif

#if REC_EN
	rec_get_dac_data(rec_bt_api, (s16 *)read_buf, DAC_BUF_LEN);	//?????????
#if (REC_SOURCE == 1)//get data from dac
	rec_get_dac_data(rec_fm_api, (s16 *)read_buf, DAC_BUF_LEN);	//??FM?????
	rec_get_dac_data(rec_aux_api, (s16 *)read_buf, DAC_BUF_LEN);	//??AUX?????
	#if defined(_SUPPORT_TAPE_)
	rec_get_dac_data(rec_tape_api, (s16 *)read_buf, DAC_BUF_LEN);	//??AUX?????
	#endif
#endif

#endif

	if(aec_interface.fill_dac_echo_buf)
	{
		aec_interface.fill_dac_echo_buf((s16*)read_buf,DAC_SAMPLE_POINT << 1);
	}

#if EQ_EN
	eq_run((short*)read_buf,(short*)dac_buf,DAC_SAMPLE_POINT);
#else
	memcpy(dac_buf,read_buf,DAC_BUF_LEN);
#endif
	dac_cbuf_write_detect();
}

/*----------------------------------------------------------------------------*/
/** @brief: Audio Moudule initial
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
static u32 dac_buf[2][DAC_SAMPLE_POINT] AT(.dac_buf_sec);
#if defined(ENABLE_TWO_PHONE_CONNECT)
static u8 cdac_buf[OUTPUT_BUF_SIZE] __attribute__((aligned(4)));
#endif
void audio_buf_init(void)
{
	puts("audio buf init\n");
	dac_ctl.dma_buf_len = DAC_BUF_LEN;
	dac_ctl.cbuf_len = OUTPUT_BUF_SIZE;
	dac_ctl.dac_dma_buf = (u8*)dac_buf;
	//dac_ctl.dac_dma_buf = malloc(dac_ctl.dma_buf_len*2);
	//ASSERT(dac_ctl.dac_dma_buf);
#if defined(ENABLE_TWO_PHONE_CONNECT)
	dac_ctl.dac_cbuf = cdac_buf;///malloc(dac_ctl.cbuf_len);
#else
	dac_ctl.dac_cbuf = malloc(dac_ctl.cbuf_len);
	ASSERT(dac_ctl.dac_cbuf);
#endif

	memset(dac_ctl.dac_dma_buf,0x00,sizeof(dac_buf));
	dac_buf_init(dac_ctl.dac_dma_buf,dac_ctl.dma_buf_len,dac_ctl.dac_cbuf,dac_ctl.cbuf_len);
}

void audio_init(void)
{
#if EQ_EN
    eq_init();
#endif

    key_voice_init();

	dac_read_en = 0;

	reg_set_sr_fun(dac_sr_cb);
	reg_dac_power_on_time_fun(dac_power_on_time);
	//dac_vcm_rsel_set(0);

	memset(&dac_ctl,0x00,sizeof(DAC_CTL));
    set_max_dac_vol(ANALOG_VOL_MAX_L, ANALOG_VOL_MAX_R, DIGITAL_VOL_MAX_L,DIGITAL_VOL_MAX_R);
    set_max_sys_vol(MAX_SYS_VOL_L, MAX_SYS_VOL_R);

    reg_digital_vol_tab((void *)digital_vol_tab);

	audio_buf_init();
#if (ECHO_EN||PITCH_EN)
	echo_reg_digital_vol_tab((void *)digital_vol_tab, sizeof(digital_vol_tab)/sizeof(digital_vol_tab[0]) - 1);
	echo_set_vol(MAX_SYS_VOL_L);
#endif

#if VCOMO_EN
    dac_init_api(VCOMO_EN, LDO_SLECT, DAC_CHANNEL_SLECT,DAC_ISEL_THIRD);
#else
    dac_init_api(VCOMO_EN, LDO_SLECT, DAC_CHANNEL_SLECT,DAC_ISEL5U);
#endif
	dac_ctl.toggle = 1;
#if defined(_ENABLE_RADIO_TIME_)
	timer_reg_func(dac_check, NULL, 20);
#else
	timer_reg_func(dac_check, NULL, 10);
#endif

	dac_reg_isr_cb_api(3,dac_isr_callback);

    s32 ret;
#if defined(_ENABLE_RADIO_TIME_)
	ret = timer_reg_func(dac_fade, NULL, 4);
#else
	ret = timer_reg_func(dac_fade, NULL, 2);
#endif
	if(ret != TIMER_NO_ERR)
	{
		ew_printf("reg dac_fade err = %x\n",ret);
	}

	///read sys vol from vm
	vm_read_api(VM_SYS_VOL, &dac_ctl.sys_vol_l);
//	vm_read_api(VM_PHONE_VOL,&dac_ctl.phone_vol);
	//ew_printf("vm_vol:%d\n",dac_ctl.sys_vol_l);
	//ew_printf("phone_vol:%d\n",dac_ctl.phone_vol);
	dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = dac_ctl.phone_vol = MAX_SYS_VOL_L;

	init_sys_vol(dac_ctl.sys_vol_l,dac_ctl.sys_vol_r);
    dac_set_samplerate(32000,1);
#if DAC2IIS_EN
	dac2iis_init();
	dac_adda_en(0);
#endif
}

void dac_on_control()
{
	puts("***dac_on***");
	dac_ctl.toggle = 1;
	dac_on_api(VCOMO_EN, LDO_SLECT, DAC_CHANNEL_SLECT);
	set_sys_vol(dac_ctl.sys_vol_l,dac_ctl.sys_vol_r,FADE_ON);

}

void dac_off_control()
{
	puts("***dac_off***");
	if(play_sel_busy()) {
		puts("--busy,return\n");
		//return;
	}
	//set_sys_vol(0,0,FADE_OFF);
	dac_off_api();
	dac_ctl.toggle = 0;
}

void dac_check()
{
	if(dac_ctl.sniff_check)
	{
		//printf("[%d %d]",key_voice_busy(),dac_ctl.keyvoice_check);
		if((key_voice_busy() == 0) && (dac_ctl.keyvoice_check)) {
			puts("dac_check_end\n");
			dac_ctl.keyvoice_check = 0;
			dac_ctl.sniff_check = 0;
		}
	}
#if DAC2IIS_EN && DAC2IIS_OUTCLK_AUTO_CLOSE
	iis_outclk_auto_close(is_auto_mute());
#endif
}
