#include "sdk_cfg.h"
#include "audio_drv.h"
#include "dac/dac.h"
#include "dac/dac_api.h"
#include "setup.h"
#include "bt.h"
#include "fm_engine.h"
#include "dac/eq.h"
#include "dac/ladc.h"

static uint8 aud_channel = AUDIO_CHANNEL_MAX;
void reg_aux_rec_exit(void * fun);

uint8 audio_drv_channel(void)
{
	return aud_channel;
}

#if defined(_SUPPORT_TWO_PT2313_)
#include "pt2313A/pt2313A.c"
#endif

#if defined(_SUPPORT_AUDIO_DRIVER_)

#if defined(_AUDIO_DRIVER_PT2313_)
	#include "pt2313/pt2313.c"
#elif defined(_AUDIO_DRIVER_OT2348_)
	#include "ot2348/ot2348.c"
#elif defined(_AUDIO_DRIVER_TDA7419_)
	#include "tda7419/tda7419.c"
#elif defined(_AUDIO_DRIVER_AP3768_)
	#include "ap3768/ap3768.c"
#elif defined(_AUDIO_DRIVER_PT7314_)
	#include "pt7314/pt7314.c"
#endif

void audio_drv_init(void)
{
	audio_reg_init();

	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), 		FALSE);
	audio_drv_ctrl(AUD_CTRL_BASS, 	setup.aud_bass, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_TREBLE, setup.aud_treble, 	FALSE);
#if defined(_FAD_BAL_FUNTION_)
    audio_drv_ctrl(AUD_CTRL_FL, 	setup.aud_fl,		FALSE);
    audio_drv_ctrl(AUD_CTRL_FR, 	setup.aud_fr,		FALSE);
    audio_drv_ctrl(AUD_CTRL_RL, 	setup.aud_rl,		FALSE);
    audio_drv_ctrl(AUD_CTRL_RR, 	setup.aud_rr,		FALSE);
#else
	audio_drv_ctrl(AUD_CTRL_LR, 	setup.aud_bal, 		FALSE);
#if defined(_SPECIAL_TL2000_SET_)
	audio_drv_ctrl(AUD_CTRL_A_VOLUME, setup.aud_A_volume, 		FALSE);
	audio_drv_ctrl(AUD_CTRL_B_VOLUME, setup.aud_B_volume, 		FALSE);
#else
	audio_drv_ctrl(AUD_CTRL_FR, 	setup.aud_fad, 		FALSE);
#endif
#endif
#if defined(_ENABLE_7419_MIDDLE_)
	audio_drv_ctrl(AUD_CTRL_MIDDLE,	setup.aud_middle, 	FALSE);
#endif
	audio_drv_ctrl(AUD_CTRL_LOUD, 	setup.aud_loud, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_EQ, 	setup.aud_eq, 		FALSE);
	audio_drv_ctrl(AUD_CTRL_MUTE, 	ENABLE, 			FALSE);
#if defined(_SUPPORT_SUBWOOFER_MODE2_)||defined(_SUPPORT_SUBWOOFER_)
	audio_drv_ctrl(AUD_CTRL_SUBWOOFER, setup.aud_subwoofer, FALSE);
#endif
#if defined(_ENABLE_AP3768_SUBWOOFER_)
	audio_drv_ctrl(AUD_CTRL_SUBWOOFER1, setup.aud_sub, FALSE);
	audio_drv_ctrl(AUD_CTRL_SUBWOOFER2, setup.aud_sub1, FALSE);
#endif
#if defined(AUDIO_SUB_LEVEL_SET)
	audio_drv_ctrl(AUD_CTRL_SUB_CUT, setup.aud_sub_ctrl, FALSE);
#endif
#if defined(_SUPPORT_10_EQ_FUNTION_)
	audio_drv_ctrl(AUD_CTRL_60HZ,	setup.aud_60hz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_150HZ,	setup.aud_150hz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_400HZ,	setup.aud_400hz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_1KHZ,	setup.aud_1khz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_2KHZ,	setup.aud_2khz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_3KHZ,	setup.aud_3khz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_7KHZ,	setup.aud_7khz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_10KHZ,	setup.aud_10khz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_15KHZ,	setup.aud_15khz, 	FALSE);
    audio_drv_ctrl(AUD_CTRL_20KHZ,	setup.aud_20khz, 	FALSE);
#endif
	audio_reg_set();

#if defined(_SUPPORT_TWO_PT2313_)
	audio_reg_init1();
	audio_drv_ctrl1(AUD_CTRL_VOLUME, get_cur_vol2(), 		FALSE);
	audio_drv_ctrl1(AUD_CTRL_BASS, 	0, 	FALSE);
	audio_drv_ctrl1(AUD_CTRL_TREBLE, 0, 	FALSE);
	audio_drv_ctrl1(AUD_CTRL_LR, 	0, 		FALSE);
	audio_drv_ctrl1(AUD_CTRL_FR, 	0, 		FALSE);
	audio_drv_ctrl1(AUD_CTRL_LOUD, 	0, 	FALSE);
	audio_drv_ctrl1(AUD_CTRL_EQ, 	0, 		FALSE);
	audio_drv_ctrl1(AUD_CTRL_MUTE, 	ENABLE, 			FALSE);
	audio_reg_set1();
#endif

}

void audio_drv_exit(void)
{
#if !defined(_AUDIO_DRIVER_AP3768_)
	audio_reg_init();
	audio_reg_set();
#endif

#if defined(_SUPPORT_TWO_PT2313_)
    audio_reg_init1();
    audio_reg_set1();
#endif
}

#if defined(_AUDIO_DRIVER_AP3768_)
u8 rds_on = 0;
#endif
void audio_drv_ctrl(uint8 ctrl, int para, uint8 respond)
{//PT2313
	uint8 result;

	switch(ctrl)
	{
		case AUD_CTRL_CHANNEL:
			{
				int channel = para;
				aud_drv_printf("%s, channel:%d, l:%d, r:%d\n", __FUNCTION__, channel, dac_ctl.sys_vol_l, dac_ctl.sys_vol_r);
				switch(channel)
				{
			    #if defined(_AUDIO_DRIVER_AP3768_)
			    	case AUDIO_CHANNEL_RDS_ON:
		    			ew_puts("AUDIO_CHANNEL_RDS_ON\n");
		    			rds_on = 1;
			    		JL_PORTA->DIR |=  (BIT(1)|BIT(2));
			    		JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
			    		//reg_aux_rec_exit(ladc_close);
			    		//amux_channel_en(LINEIN_CHANNEL,1);
			    	    dac_mute(1, 0);
			    	    digit_auto_mute_set(0, -1, -1, -1);
			    	    amux_channel_en(LINEIN_CHANNEL, 1);
			    	    amux_ch_combine(1,1);
			    	    dac_AmuxGain_en(1);
			    	    JL_AUDIO->DAA_CON2 |= (BIT(12)|BIT(7));
			    	    JL_AUDIO->DAA_CON2 &= ~(BIT(13));
			    	    os_time_dly(10);
			    	    set_sys_vol(30, 30, FADE_OFF);
			    	    dac_mute(0, 0);
			    		break;
			    	case AUDIO_CHANNEL_RDS_OFF:
		    			ew_puts("AUDIO_CHANNEL_RDS_OFF\n");
		    			rds_on = 0;
			    		//amux_channel_en(LINEIN_CHANNEL,0);
			    	    //dac_mute(1, 0);
		    			amux_ch_combine(0,0);
		    			amux_channel_en(LINEIN_CHANNEL, 0);
			    	    JL_AUDIO->DAA_CON2 |= (BIT(12)|BIT(7));
			    		digit_auto_mute_set(1,-1,-1,-1); // 开自动mute
			    	   //os_time_dly(10);
			    	   //dac_mute(0, 0);
			    		break;
			    #endif
				
				#if defined(_ENABLE_RECORD_RADIO_)
					case AUDIO_CHANNEL_RADIO_ON:
		    			ew_puts("AUDIO_CHANNEL_RADIO_ON\n");
    					JL_PORTA->DIR |=  (BIT(1)|BIT(2));
    					JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
    					//reg_aux_rec_exit(ladc_close);
    					dac_channel_on(MUSIC_CHANNEL, FADE_ON);
     					dac_set_samplerate(48000,0);
     					ladc_reg_init(ENC_LINE_LR_CHANNEL,LADC_SR48000);
     					amux_channel_en(LINEIN_CHANNEL,1);
    					os_time_dly(20);//wait amux channel capacitance charge ok
    					set_sys_vol(30, 30, FADE_OFF);
    					break;
					case AUDIO_CHANNEL_RADIO_OFF:
		    			ew_puts("AUDIO_CHANNEL_RADIO_OFF\n");
    					dac_channel_off(MUSIC_CHANNEL, FADE_ON);
    					ladc_close(ENC_LINE_LR_CHANNEL);
    					amux_channel_en(LINEIN_CHANNEL,0);
						break;
				#elif defined(_ENABLE_PT7314_INSIDE_)
					case AUDIO_CHANNEL_RADIO_ON:
						JL_PORTA->DIR |=  (BIT(1)|BIT(2));
						JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
						dac_channel_on(FM_IIC_CHANNEL, FADE_OFF);
						os_time_dly(20);//wait amux channel capacitance charge ok
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
						
					case AUDIO_CHANNEL_RADIO_OFF:
						dac_channel_off(FM_IIC_CHANNEL, FADE_OFF);
						break;
				#else
					case AUDIO_CHANNEL_RADIO_ON:
						dac_channel_on(FM_INSI_CHANNEL, FADE_OFF);
						dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
						dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_RADIO_OFF:
						dac_channel_off(FM_INSI_CHANNEL, FADE_OFF);
						break;
				#endif
				
					case AUDIO_CHANNEL_MUSIC_ON:
						dac_channel_on(MUSIC_CHANNEL, FADE_OFF);
                   #if defined(_CHANGE_AUDIO_STEP_)
				        dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
						dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
				   #else
						if(bt_task_work == TRUE)
						{
						#if defined(_ENABLE_CHANNEL_GAIN_)
    						dac_ctl.sys_vol_l = 23;
    						dac_ctl.sys_vol_r = 23;
						#else
							dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
							dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
						#endif
						}
						else
						{
							dac_ctl.sys_vol_l = 27;
							dac_ctl.sys_vol_r = 27;
						}
				   #endif		
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_MUSIC_OFF:
						dac_channel_off(MUSIC_CHANNEL, FADE_OFF);
						break;

			    #if defined(_AUX_INSIDE_)||defined(ENABLE_DAB_PT2313_CHANNEL)||defined(_AUX_INSIDE_MCU_)
			    	case AUDIO_CHANNEL_AUX_ON:
						JL_PORTA->DIR |=  (BIT(1)|BIT(2));
						JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
						dac_channel_on(LINEIN_CHANNEL, FADE_OFF);
						os_time_dly(20);//wait amux channel capacitance charge ok
					#if defined(_ENABLE_CHANNEL_GAIN_)
    					set_sys_vol(25, 25, FADE_OFF);
					#else
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
					#endif
			    		break;
			    	case AUDIO_CHANNEL_AUX_OFF:
			    		dac_channel_off(LINEIN_CHANNEL, FADE_OFF);
			    		break;
				#elif defined(_ENABLE_PT7314_INSIDE_)
    				case AUDIO_CHANNEL_AUX_ON:
                        JL_PORTB->DIR |=  (BIT(11)|BIT(12));
                        JL_PORTB->DIE &= ~(BIT(11)|BIT(12));
                        //reg_aux_rec_exit(ladc_close);
    					dac_channel_on(LINEIN_CHANNEL, FADE_OFF);
    					os_time_dly(20);//wait amux channel capacitance charge ok
    					set_sys_vol(27, 27, FADE_OFF);
    					break;
    				case AUDIO_CHANNEL_AUX_OFF:
    					dac_channel_off(LINEIN_CHANNEL, FADE_OFF);
    					break;
				#elif defined(_ENABLE_AUX_CHANNEL_)
    				case AUDIO_CHANNEL_AUX_ON:
    					JL_PORTA->DIR |=  (BIT(3)|BIT(4));
    					JL_PORTA->DIE &= ~(BIT(3)|BIT(4));
						JL_PORTA->PD  &= ~(BIT(3));//PA3 default pull_down
    					dac_channel_on(FM_IIC_CHANNEL, FADE_OFF);
    					os_time_dly(20);//wait amux channel capacitance charge ok
    					set_sys_vol(27, 27, FADE_OFF);
    					break;
    				case AUDIO_CHANNEL_AUX_OFF:
    					dac_channel_off(FM_IIC_CHANNEL, FADE_OFF);
					#if defined(_ENABLE_MUTE_AUX_)
    					set_sys_vol(0, 0, FADE_OFF);
					#endif
    					break;
			    #else
			    	case AUDIO_CHANNEL_AUX_ON:
			    		dac_channel_on(MUSIC_CHANNEL, FADE_OFF);
			    		dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
			    		dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
			    		set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
			    		break;
			    	case AUDIO_CHANNEL_AUX_OFF:
			    		dac_channel_off(MUSIC_CHANNEL, FADE_OFF);
			    		break;
			    #endif

			    #if defined(_SUPPORT_DISC_)
			    	case AUDIO_CHANNEL_DISC_ON:
			    		dac_channel_on(MUSIC_CHANNEL, FADE_OFF);
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
			    		break;

			    	case AUDIO_CHANNEL_DISC_OFF:
			    		dac_channel_off(MUSIC_CHANNEL, FADE_OFF);
			    		break;
			    #endif

				#if defined(_ENABLE_DAB_VOLUME_)
					case AUDIO_CHANNEL_DAB_ON:
						JL_PORTA->DIR |=  (BIT(1)|BIT(2));
						JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
                        reg_aux_rec_exit(ladc_close);
						dac_channel_on(LINEIN_CHANNEL, FADE_OFF);
						os_time_dly(20);//wait amux channel capacitance charge ok
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
			    		break;

					case AUDIO_CHANNEL_DAB_OFF:
			    		dac_channel_off(LINEIN_CHANNEL, FADE_OFF);
						break;
				#elif defined(ENABLE_PT2312_CHANNEL)||defined(ENABLE_PT2313_CHANNEL)
    				case AUDIO_CHANNEL_DAB_ON:
    					break;
    				case AUDIO_CHANNEL_DAB_OFF:
    					break;		
				#elif defined(ENABLE_DAB_PT2313_CHANNEL)
    				case AUDIO_CHANNEL_DAB_ON:
    					dac_channel_on(MUSIC_CHANNEL, FADE_OFF);
    					dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
    					dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
    					set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
    					break;
    				case AUDIO_CHANNEL_DAB_OFF:
    					dac_channel_off(MUSIC_CHANNEL, FADE_OFF);
    					break;
				#endif

				#if defined(_SUPPORT_TAPE_)
    				case AUDIO_CHANNEL_TAPE_ON:
    					JL_PORTA->DIR |=  (BIT(1)|BIT(2));
    					JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
    					//reg_aux_rec_exit(ladc_close);
    					dac_channel_on(MUSIC_CHANNEL, FADE_ON);
     					dac_set_samplerate(48000,0);
     					ladc_reg_init(ENC_LINE_LR_CHANNEL,LADC_SR48000);
     					amux_channel_en(LINEIN_CHANNEL,1);
    					os_time_dly(20);//wait amux channel capacitance charge ok
    					set_sys_vol(30, 30, FADE_OFF);
    					break;

    				case AUDIO_CHANNEL_TAPE_OFF:
    					dac_channel_off(MUSIC_CHANNEL, FADE_ON);
    					ladc_close(ENC_LINE_LR_CHANNEL);
    					amux_channel_en(LINEIN_CHANNEL,0);
    					break;
				#endif
				
				#if defined(_SUPPORT_MIC_OK_)
					case AUDIO_CHANNEL_MIC_ON:
						JL_PORTA->DIR |=  (BIT(1)|BIT(2));
						JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
						dac_channel_on(LINEIN_CHANNEL, FADE_OFF);
						os_time_dly(20);//wait amux channel capacitance charge ok
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					
					case AUDIO_CHANNEL_MIC_OFF:
			    		dac_channel_off(LINEIN_CHANNEL, FADE_OFF);
						break;
				#endif
                
                #if defined(_SUPPORT_AUX1_)
					case AUDIO_CHANNEL_AUX1_ON:
						dac_channel_on(LINEIN_CHANNEL, FADE_OFF);
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					
					case AUDIO_CHANNEL_AUX1_OFF:
			    		dac_channel_off(LINEIN_CHANNEL, FADE_OFF);
						break;
                #endif
					default:
						channel = AUDIO_CHANNEL_MAX;
						break;
				}
				aud_channel = channel;
			}
			break;

		default:
			break;
	}

	result = audio_reg_ctrl(ctrl, para);

	if(result && respond)
	{
		audio_reg_set();
	}
}

#else//------------------------------------------------------------------------------------------------------------------------------

uint8 audio_reg_ctrl(uint8 ctrl, int para)
{
	return 1;
}

void audio_reg_set(void)
{
	return ;
}

#if (AUDIO_VOLUME_MAX == 30)
static const uint8 VolumeTab[31]=
{
	0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,30,
};
#endif

static const int8 bas_trb[15]=
{
	7,8,9,10,11,12,13,
	14,
	15,16,17,18,19,20,21
};

#if defined(_HY1037_EQ_FUNTION_)
static const int8 tre_trb[15]=
{
	22,23,24,25,26,27,28,
	29,
	30,30,31,31,31,32,32
};
#else
static const int8 tre_trb[15]=
{
	22,23,24,25,26,27,28,
	29,
	30,31,32,33,34,35,36
};
#endif

void audio_drv_init(void)
{
	audio_drv_ctrl(AUD_CTRL_MUTE, 	ENABLE, 			FALSE);
	dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = get_cur_vol();
	audio_drv_ctrl(AUD_CTRL_VOLUME, get_cur_vol(), TRUE);
	audio_drv_ctrl(AUD_CTRL_BASS, 	setup.aud_bass, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_TREBLE, setup.aud_treble, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_LR, 	setup.aud_bal, 		FALSE);
	audio_drv_ctrl(AUD_CTRL_FR, 	setup.aud_fad, 		FALSE);
	audio_drv_ctrl(AUD_CTRL_LOUD, 	setup.aud_loud, 	FALSE);
	audio_drv_ctrl(AUD_CTRL_EQ, 	setup.aud_eq, 		FALSE);
}

void audio_drv_exit(void)
{

}

void audio_drv_refur(void)
{

}

void audio_drv_ctrl(uint8 ctrl, int para, uint8 respond)
{//NONE 2313
    static uint8 buf=0;
	switch(ctrl)
	{
		case AUD_CTRL_CHANNEL:
			{
				int channel = para;
				switch(channel)
				{
				#if defined(_ENABLE_RECORD_RADIO_)
					case AUDIO_CHANNEL_RADIO_ON:
						JL_PORTA->DIR |=  (BIT(1)|BIT(2));
						JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
						JL_AUDIO->DAA_CON2 |= BIT(6);			// 功率调整
						//reg_aux_rec_exit(ladc_close);
						dac_channel_on(MUSIC_CHANNEL, FADE_ON);
						dac_set_samplerate(48000,0);
						ladc_reg_init(ENC_LINE_LR_CHANNEL,LADC_SR48000);
						amux_channel_en(LINEIN_CHANNEL,1);
						os_time_dly(20);//wait amux channel capacitance charge ok
						//set_sys_vol(30, 30, FADE_OFF);
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_RADIO_OFF:
						dac_channel_off(MUSIC_CHANNEL, FADE_ON);
						ladc_close(ENC_LINE_LR_CHANNEL);
						amux_channel_en(LINEIN_CHANNEL,0);
						JL_AUDIO->DAA_CON2 &= ~(BIT(6));
						break;
				#else
					case AUDIO_CHANNEL_RADIO_ON:
						if(fm_inside_check())
						{
							dac_channel_on(FM_INSI_CHANNEL, FADE_OFF);
						}
						else
						{
						#if !defined(_DECODER_AUDIO_CONTROL_)
						#if !defined (TD_ADD_FM_IN_AMPLIFIER)
							dac_channel_on(FM_IIC_CHANNEL, FADE_OFF);
						#endif
                            if(FM_IIC_CHANNEL == DAC_AMUX0)
							{
                                JL_PORTA->DIR |=  (BIT(1)|BIT(2));
                                JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
								ew_printf("DAC_AMUX0\n");
                            }
                            else if(FM_IIC_CHANNEL == DAC_AMUX1)
							{
                                JL_PORTA->DIR |=  (BIT(4)|BIT(3));
                                JL_PORTA->DIE &= ~(BIT(4)|BIT(3));
                                JL_PORTA->PD  &= ~(BIT(3));//PA3 default pull_down
                                ew_printf("DAC_AMUX1\n");
                            }
                            else if(FM_IIC_CHANNEL == DAC_AMUX2)
							{
                                JL_PORTB->DIR |=  (BIT(11)|BIT(12));
                                JL_PORTB->DIE &= ~(BIT(11)|BIT(12));
								ew_printf("DAC_AMUX2\n");
                            }
						#endif
						#if defined(_ENABLE_FM_CHANNEL_)
						    dac_channel_on(FM_IIC_CHANNEL, FADE_OFF);
						    if(FM_IIC_CHANNEL == DAC_AMUX0)
							{
                                JL_PORTA->DIR |=  (BIT(1)|BIT(2));
                                JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
								ew_printf("DAC_AMUX0\n");
                            }
                            else if(FM_IIC_CHANNEL == DAC_AMUX1)
							{
                                JL_PORTA->DIR |=  (BIT(4)|BIT(3));
                                JL_PORTA->DIE &= ~(BIT(4)|BIT(3));
                                JL_PORTA->PD  &= ~(BIT(3));//PA3 default pull_down
                                ew_printf("DAC_AMUX1\n");
                            }
                            else if(FM_IIC_CHANNEL == DAC_AMUX2)
							{
                                JL_PORTB->DIR |=  (BIT(11)|BIT(12));
                                JL_PORTB->DIE &= ~(BIT(11)|BIT(12));
								ew_printf("DAC_AMUX2\n");
                            }
						#endif
							JL_AUDIO->DAA_CON2 |= BIT(6);			// 功率调整
						}
						os_time_dly(20);//wait amux channel capacitance charge ok
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
					#if defined(_FM_VOLSET_USE_CHIP_)
						os_taskq_post_msg("FMEngTask", 2, 0x2100, setup.aud_vol);
					#endif
						break;

					case AUDIO_CHANNEL_RADIO_OFF:
						if(fm_inside_check())
						{
							dac_channel_off(FM_INSI_CHANNEL, FADE_OFF);
						}
						else
						{
						#if !defined(_DECODER_AUDIO_CONTROL_)
							dac_channel_off(FM_IIC_CHANNEL, FADE_OFF);
						#endif
						#if defined(_ENABLE_FM_CHANNEL_)
						    dac_channel_off(FM_IIC_CHANNEL, FADE_OFF);
						#endif
						#if defined(_FM_VOLSET_USE_CHIP_)
							os_taskq_post_msg("FMEngTask", 2, 0x2100, 0);
						#endif
					
							JL_AUDIO->DAA_CON2 &= ~(BIT(6));
						}
						break;
				#endif
				
					case AUDIO_CHANNEL_MUSIC_ON:
						dac_channel_on(MUSIC_CHANNEL, FADE_OFF);
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_MUSIC_OFF:
						dac_channel_off(MUSIC_CHANNEL, FADE_OFF);
						break;

				#if defined(_ENABLE_AUX_CHANNEL_)
				#if defined(_ENABLE_MIC_REC_)
				   case AUDIO_CHANNEL_AUX_ON:
				        JL_PORTA->DIR |=  (BIT(3)|BIT(4));
						JL_PORTA->DIE &= ~(BIT(3)|BIT(4));
						JL_PORTA->PD  &= ~(BIT(3));//PA3 default pull_down
						dac_channel_on(FM_IIC_CHANNEL, FADE_OFF);
				        dac_set_samplerate(48000,0);
     					ladc_reg_init(ENC_LINE_LR_CHANNEL,LADC_SR48000);
     					amux_channel_en(LINEIN_CHANNEL,1);
    					os_time_dly(20);//wait amux channel capacitance charge ok
    					set_sys_vol(30, 30, FADE_OFF);
						break;
					case AUDIO_CHANNEL_AUX_OFF:
						dac_channel_off(FM_IIC_CHANNEL, FADE_ON);
    					ladc_close(ENC_LINE_LR_CHANNEL);
    					amux_channel_en(LINEIN_CHANNEL,0);
						break;	
				#else
					case AUDIO_CHANNEL_AUX_ON:
						JL_PORTA->DIR |=  (BIT(3)|BIT(4));
						JL_PORTA->DIE &= ~(BIT(3)|BIT(4));
						dac_channel_on(FM_IIC_CHANNEL, FADE_OFF);
						os_time_dly(20);//wait amux channel capacitance charge ok
						//set_sys_vol(27, 26, FADE_OFF);
						set_sys_vol(30, 30, FADE_OFF);
						//set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_AUX_OFF:
						dac_channel_off(FM_IIC_CHANNEL, FADE_OFF);
						break;
				#endif		
				#else
					case AUDIO_CHANNEL_AUX_ON:
					#if defined(TD_ADD_AUX_IN_MCU)||defined(_DECODER_AUDIO_CONTROL_)
                        if(LINEIN_CHANNEL == DAC_AMUX0)
						{
                            JL_PORTA->DIR |=  (BIT(1)|BIT(2));
                            JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
                        }
                        else if(LINEIN_CHANNEL == DAC_AMUX1)
						{
                            JL_PORTA->DIR |=  (BIT(4)|BIT(3));
                            JL_PORTA->DIE &= ~(BIT(4)|BIT(3));
                            JL_PORTA->PD  &= ~(BIT(3));//PA3 default pull_down
                        }
                        else if(LINEIN_CHANNEL == DAC_AMUX2)
						{
                            JL_PORTB->DIR |=  (BIT(11)|BIT(12));
                            JL_PORTB->DIE &= ~(BIT(11)|BIT(12));
                        }
                        reg_aux_rec_exit(ladc_close);
						dac_channel_on(LINEIN_CHANNEL, FADE_OFF);
                        os_time_dly(20);//wait amux channel capacitance charge ok
					#endif
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_AUX_OFF:
						dac_channel_off(LINEIN_CHANNEL, FADE_OFF);
						break;
				#endif
				
				#if defined(_SUPPORT_REVERSE_MODEL_)
					case AUDIO_CHANNEL_REV_ON:
						JL_PORTA->DIR |=  (BIT(1)|BIT(2));
						JL_PORTA->DIE &= ~(BIT(1)|BIT(2));
						reg_aux_rec_exit(ladc_close);
						dac_channel_on(LINEIN_CHANNEL, FADE_OFF);
						os_time_dly(20);//wait amux channel capacitance charge ok
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
						
					case AUDIO_CHANNEL_REV_OFF:
						dac_channel_off(LINEIN_CHANNEL, FADE_OFF);
						break;

				#endif
								
					default:
						channel = AUDIO_CHANNEL_MAX;
						break;
				}
				aud_channel = channel;
			}
			break;

		case AUD_CTRL_MUTE:
			{
				uint8 mute = (uint8)para;
				
				if(mute)
                {
                	dac_mute(1,1);
					AUDIO_MUTE_ON();
			    #if defined(_ENABLE_RCA_MUTE_)
			    	RCA_MUTE_ON();
			    #endif
                }
                else
                {
                #if defined(_XM003_MUTE_FUNTION_)
                	if(setup.aud_vol == 0)
                	{
						dac_mute(1,1);
						AUDIO_MUTE_ON();
						global.mute_seek_flag = TRUE;
					}
					else
				#endif
					{
				    #if defined(_XM003_BT_VOL_)||defined(_HYM_BT_VOL_)
				    	audio_drv_ctrl(AUD_CTRL_VOLUME, setup.aud_vol, TRUE);
				    	OSTimeDly(1);
				    #endif

				    #if defined(_ENABLE_BAND_MUTE_)||defined(_ENABLE_POWER_MUTE_)||defined(_ENABLE_FIRST_POWER_MUTE_)
				    	if(!global.mute_flag)
				    	{
				    		AUDIO_MUTE_OFF();
    				    #if defined(_ENABLE_RCA_MUTE_)
    				    	RCA_MUTE_OFF();
    				    #endif
				    	}
				    #else
				    	AUDIO_MUTE_OFF();
				    #if defined(_ENABLE_RCA_MUTE_)
				    	RCA_MUTE_OFF();
				    #endif
				    #endif
				    	dac_mute(0,1);
					}
                }

				#if defined(_FM_VOLSET_USE_CHIP_)
					if((!fm_inside_check()) && (aud_channel == AUDIO_CHANNEL_RADIO_ON))
					{
						os_taskq_post_msg("FMEngTask", 2, 0x2100, mute ? 0 : setup.aud_vol);
					}

					if((!fm_inside_check()) && (aud_channel == AUDIO_CHANNEL_RADIO_OFF))
					{
						os_taskq_post_msg("FMEngTask", 2, 0x2100, 0);
					}
				#endif
			}
			break;
		case AUD_CTRL_BASS:
			{
				ew_puts("AUD_CTRL_BASS\n");
				int8 bass =((int8)para+7);
				ew_printf("bass:%d\n",bass);
				buf =bas_trb[bass];
				eq_mode_sw(buf);
			}
			break;
		case AUD_CTRL_TREBLE:
			{
				ew_puts("AUD_CTRL_TREBLE\n");
				int8 trb = ((int8)para+7);
				ew_printf("trb:%d\n",trb);
				buf =tre_trb[trb];
				eq_mode_sw(buf);
			}
			break;
		case AUD_CTRL_FR:
			break;
		case AUD_CTRL_LOUD:
            if(setup.aud_loud)
            {
                eq_mode_sw(37);
            }
            else
            {
            	if(setup.aud_eq == 0)
            	{
					eq_mode_sw(buf);
				}
				else
				{
					eq_mode_sw(setup.eq_mode);
				}
            }
			break;
		case AUD_CTRL_EQ:
			ew_printf("setup.aud_eq:%d\n",setup.aud_eq);
			switch(setup.aud_eq)
			{
				case AUDIO_EQ_OFF:
					setup.eq_mode = 0;
					break;
				case AUDIO_EQ_FLAT:
					setup.eq_mode = 5;
					break;
				case AUDIO_EQ_CLASSIC:
					setup.eq_mode = 3;
					break;
				case AUDIO_EQ_POP:
					setup.eq_mode = 2;
					break;
				case AUDIO_EQ_ROCK:
					setup.eq_mode = 1;
					break;
				case AUDIO_EQ_JAZZ:
					setup.eq_mode = 4;
					break;
				default:
					setup.eq_mode = 0;
					break;
			}
		#if defined(_HYM_SPECIAL_EQ_)
			if(setup.aud_eq == AUDIO_EQ_OFF)
			{
				eq_mode_sw(14);
				eq_mode_sw(29);
 			}
			else
			{
				eq_mode_sw(setup.eq_mode);
			}
		#else
		        eq_mode_sw(setup.eq_mode);
		#endif
			break;

		case AUD_CTRL_LR:
			{
            	int8 bal = (int8)para;
			#if 1
				uint32 a,b,c;
			#if defined(_DECODER_MUSIC_GAIN_)
				if(compare_task_name("FMRadioTask"))
				{
					b = ((setup.aud_vol)*AUDIO_DECODE_VOLUME_MAX)/AUDIO_VOLUME_MAX;
				}
				else
				{
				#if defined(_YS_SEL_SETTING_)
					b = ((setup.aud_vol)*26)/AUDIO_VOLUME_MAX;
				#elif defined(_HY1037_MUSIC_VOL_)
					b = ((setup.aud_vol)*23)/AUDIO_VOLUME_MAX;
				#elif defined(_HY1037_MUSIC_VOL_GAIN_)
					b = ((setup.aud_vol)*20)/AUDIO_VOLUME_MAX;
				#elif defined(_HY1037_MUSIC_VOL_STEP_)
					b = ((setup.aud_vol)*26)/AUDIO_VOLUME_MAX;
				#else
					b = ((setup.aud_vol)*25)/AUDIO_VOLUME_MAX;
				#endif
				}
			#else
		    	b = ((setup.aud_vol)*AUDIO_DECODE_VOLUME_MAX)/AUDIO_VOLUME_MAX;
			#endif
				c = (AUDIO_VOLUME_MAX*7);

		    	if(bal > 0)
		    	{
				#if defined(_DECODER_MUSIC_GAIN_)
					if(compare_task_name("FMRadioTask"))
					{
						a = ((setup.aud_vol)*AUDIO_DECODE_VOLUME_MAX*(8 - bal));
					}
					else
					{
					#if defined(_YS_SEL_SETTING_)
						a = ((setup.aud_vol)*26*(8 - bal));
					#elif defined(_HY1037_MUSIC_VOL_)
						a = ((setup.aud_vol)*23*(8 - bal));
					#elif defined(_HY1037_MUSIC_VOL_GAIN_)
					    a = ((setup.aud_vol)*20*(8 - bal));
					#elif defined(_HY1037_MUSIC_VOL_STEP_)
						a = ((setup.aud_vol)*26*(8 - bal));
					#else					
						a = ((setup.aud_vol)*25*(8 - bal));
					#endif
					}
				#else
		    		a = ((setup.aud_vol)*AUDIO_DECODE_VOLUME_MAX*(8 - bal));
				#endif

		    		dac_ctl.sys_vol_r = b;
		    		dac_ctl.sys_vol_l = (a/c);
		    	}
		    	else if(bal < 0)
		    	{
		    		bal = -bal;
				#if defined(_DECODER_MUSIC_GAIN_)
					if(compare_task_name("FMRadioTask"))
					{
						a = ((setup.aud_vol)*AUDIO_DECODE_VOLUME_MAX*(8 - bal));
					}
					else
					{
					#if defined(_YS_SEL_SETTING_)
						a = ((setup.aud_vol)*26*(8 - bal));
					#elif defined(_HY1037_MUSIC_VOL_)
						a = ((setup.aud_vol)*23*(8 - bal));
					#elif defined(_HY1037_MUSIC_VOL_GAIN_)
					    a = ((setup.aud_vol)*20*(8 - bal));
					#elif defined(_HY1037_MUSIC_VOL_STEP_)
					    a = ((setup.aud_vol)*26*(8 - bal));
					#else
						a = ((setup.aud_vol)*25*(8 - bal));
					#endif
					}
				#else
		    		a = ((setup.aud_vol)*AUDIO_DECODE_VOLUME_MAX*(8 - bal));
				#endif

		    		dac_ctl.sys_vol_l = b;
		    		dac_ctl.sys_vol_r = (a/c);
		    	}
		    	else
		    	{
					if(AUDIO_VOLUME_MAX == 63)
					{
						if((setup.aud_vol == 1)||(setup.aud_vol == 2))
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = 1;
						}
						else
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = b;
						}
					}
					else if(AUDIO_VOLUME_MAX == 100)
					{
						if((setup.aud_vol == 1)||(setup.aud_vol == 2)||(setup.aud_vol == 3))
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = 1;
						}
						else
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = b;
						}
					}
					else
					{
						if(setup.aud_vol == 1)
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = 1;
						}
						else
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = b;
						}
					}
		    	}

				//aud_drv_printf("a= %d,b= %d\n",a,b);
				//aud_drv_printf("lr_cur_sys_vol_l:%d,lr_cur_sys_vol_r:%d\n",dac_var.cur_sys_vol_l,dac_var.cur_sys_vol_r);

                set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);

			#else
                uint8 volval=VolumeTab[setup.aud_vol];
                uint8 step=(volval/7);
                dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = VolumeTab[setup.aud_vol];
            	if(bal> 0)
            	{
                    volval -= (step*bal);
                    if(bal ==7)
                    {
	                    dac_ctl.sys_vol_l =0;
                    }
                    else
                    {
	                    dac_ctl.sys_vol_l = volval;
                    }
            	}
            	else if(bal< 0)
            	{
                    volval -= (step*(7-(bal+7)));
                    if((7-(bal+7)) ==7)
                    {
                        dac_ctl.sys_vol_r =0;
                    }
                    else
                    {
                    	dac_ctl.sys_vol_r = volval;
                    }
            	}
            	else
            	{
                    dac_ctl.sys_vol_r = dac_ctl.sys_vol_l=VolumeTab[setup.aud_vol];
            	}
                set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
			#endif
			}
			break;

		case AUD_CTRL_VOLUME:
			{
				uint8 volume = (uint8)para;
			#if defined(_FM_VOLSET_USE_CHIP_)
				AUDIO_MUTE_INIT();
				if((volume == 0)||(global.aud_mute))
				{
					AUDIO_MUTE_ON();
					//aud_drv_puts("AUD_CTRL_VOLUME11\n");
				}
				else
				{
					AUDIO_MUTE_OFF();
					//aud_drv_puts("AUD_CTRL_VOLUME22\n");
				}
				if((!fm_inside_check()) && (aud_channel == AUDIO_CHANNEL_RADIO_ON))
				{
					os_taskq_post_msg("FMEngTask", 2, 0x2100, volume);
				}
				if((!fm_inside_check()) && (aud_channel == AUDIO_CHANNEL_RADIO_OFF))
				{
					os_taskq_post_msg("FMEngTask", 2, 0x2100, 0);
				}
				else
			#endif
				{
			#if 1
				uint32 x,y,z;
			
				if(volume >= AUDIO_VOLUME_MAX)
				{
					volume = AUDIO_VOLUME_MAX;
				}
			#if defined(SET_MUTE_WHEN_VOL_)
				if((volume == 0)||(global.aud_mute))
				{
					dac_mute(1,1);
					AUDIO_MUTE_ON();
				}
				else
				{
					dac_mute(0,1);
					AUDIO_MUTE_OFF();
				}
    		#endif
			#if defined(_DECODER_MUSIC_GAIN_)
    			if(compare_task_name("FMRadioTask"))
    			{
					y = ((volume*AUDIO_DECODE_VOLUME_MAX)/AUDIO_VOLUME_MAX);
    			}
    			else
    			{
				#if defined(_YS_SEL_SETTING_)
					y = ((volume*26)/AUDIO_VOLUME_MAX);
				#elif defined(_HY1037_MUSIC_VOL_)
					y = ((volume*23)/AUDIO_VOLUME_MAX);
				#elif defined(_HY1037_MUSIC_VOL_GAIN_)
				    y = ((volume*20)/AUDIO_VOLUME_MAX);
				#elif defined(_HY1037_MUSIC_VOL_STEP_)
					y = ((volume*26)/AUDIO_VOLUME_MAX);
				#else
					y = ((volume*25)/AUDIO_VOLUME_MAX);
				#endif
    			}
			#else
				y = ((volume*AUDIO_DECODE_VOLUME_MAX)/AUDIO_VOLUME_MAX);
			#endif
				z= (AUDIO_VOLUME_MAX*7);

				if(setup.aud_bal > 0)
				{
				#if defined(_DECODER_MUSIC_GAIN_)
					if(compare_task_name("FMRadioTask"))
					{
						x = ((volume)*AUDIO_DECODE_VOLUME_MAX*(8 - setup.aud_bal));
					}
					else
					{
					#if defined(_YS_SEL_SETTING_)
						x = ((volume)*26*(8 - setup.aud_bal));
					#elif defined(_HY1037_MUSIC_VOL_)
						x = ((volume)*23*(8 - setup.aud_bal));
					#elif defined(_HY1037_MUSIC_VOL_GAIN_)
					    x = ((volume)*20*(8 - setup.aud_bal));
					#elif defined(_HY1037_MUSIC_VOL_STEP_)
						x = ((volume)*26*(8 - setup.aud_bal));
					#else
						x = ((volume)*25*(8 - setup.aud_bal));
					#endif
					}
				#else
					x = ((volume)*AUDIO_DECODE_VOLUME_MAX*(8 - setup.aud_bal));
				#endif
					dac_ctl.sys_vol_r = y;
					dac_ctl.sys_vol_l = (x/z);
				}
				else if(setup.aud_bal < 0)
				{
				#if defined(_DECODER_MUSIC_GAIN_)
					if(compare_task_name("FMRadioTask"))
					{
						x = (volume*AUDIO_DECODE_VOLUME_MAX*(8 + setup.aud_bal));
					}
					else
					{
					#if defined(_YS_SEL_SETTING_)
						x = (volume*26*(8 + setup.aud_bal));
					#elif defined(_HY1037_MUSIC_VOL_)
						x = (volume*23*(8 + setup.aud_bal));
					#elif defined(_HY1037_MUSIC_VOL_GAIN_)
					    x = (volume*20*(8 + setup.aud_bal));
					#elif defined(_HY1037_MUSIC_VOL_STEP_)
						x = (volume*26*(8 + setup.aud_bal));
					#else
						x = (volume*25*(8 + setup.aud_bal));
					#endif
					}
				#else
					x = (volume*AUDIO_DECODE_VOLUME_MAX*(8 + setup.aud_bal));
				#endif
					dac_ctl.sys_vol_r = (x/z);
					dac_ctl.sys_vol_l = y;
				}
				else
				{
					if(AUDIO_VOLUME_MAX == 63)
					{
						if((volume == 1)||(volume == 2))
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = 1;
						}
						else
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = y;
						}
					}
					else if(AUDIO_VOLUME_MAX == 100)
					{
						if((volume == 1)||(volume == 2)||(volume == 3))
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = 1;
						}
						else
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = y;
						}
					}
					else
					{
						if(volume == 1)
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = 1;
						}
						else
						{
							dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = y;
						}
					}
				}

			#if defined(_XM003_MUTE_FUNTION_)
				if((setup.aud_vol == 1)&&(global.mute_seek_flag))
				{
					dac_mute(0,1);
					AUDIO_MUTE_OFF();
					global.mute_seek_flag = FALSE;
				}
			#endif

				if(is_dac_mute())/*mute状态下，加减音量解mute*/
				{
					dac_mute(0,1);
				}

				//aud_drv_printf("x= %d,y= %d\n",x,y);
				//aud_drv_printf("volume_cur_sys_vol_l:%d,volume_cur_sys_vol_r:%d\n",dac_ctl.sys_vol_l,dac_ctl.sys_vol_r);

	            set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
			#else
				if(volume >= AUDIO_VOLUME_MAX)
				{
					volume = AUDIO_VOLUME_MAX;
				}
				dac_ctl.sys_vol_r = dac_ctl.sys_vol_l = VolumeTab[volume];
	            if(is_dac_mute())/*mute状态下，加减音量解mute*/
	            {
	                dac_mute(0,1);
	            }
	            set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
			#endif
			}
			}
			break;

		default:
			break;
	}
}

#endif

#if defined(_SUPPORT_TWO_PT2313_)
void audio_drv_ctrl1(uint8 ctrl, int para, uint8 respond)
{
	uint8 result;

	switch(ctrl)
	{
		case AUD_CTRL_CHANNEL:
			{
				int channel = para;
				switch(channel)
				{
					case AUDIO_CHANNEL_RADIO_ON:
						dac_channel_on(FM_INSI_CHANNEL, FADE_OFF);
						dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
						dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_RADIO_OFF:
						dac_channel_off(FM_INSI_CHANNEL, FADE_OFF);
						break;
				
					case AUDIO_CHANNEL_MUSIC_ON:
						dac_channel_on(MUSIC_CHANNEL, FADE_OFF);
						if(bt_task_work == TRUE)
						{
							dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
							dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
						}
						else
						{
							dac_ctl.sys_vol_l = 27;
							dac_ctl.sys_vol_r = 27;
						}
						set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
						break;
					case AUDIO_CHANNEL_MUSIC_OFF:
						dac_channel_off(MUSIC_CHANNEL, FADE_OFF);
						break;

			    	case AUDIO_CHANNEL_AUX_ON:
			    		dac_channel_on(MUSIC_CHANNEL, FADE_OFF);
			    		dac_ctl.sys_vol_l = MAX_SYS_VOL_L;
			    		dac_ctl.sys_vol_r = MAX_SYS_VOL_R;
			    		set_sys_vol(dac_ctl.sys_vol_l, dac_ctl.sys_vol_r, FADE_OFF);
			    		break;
			    	case AUDIO_CHANNEL_AUX_OFF:
			    		dac_channel_off(MUSIC_CHANNEL, FADE_OFF);
			    		break;

					default:
						channel = AUDIO_CHANNEL_MAX;
						break;
				}
				aud_channel = channel;
			}
			break;

		default:
			break;
	}

	result = audio_reg_ctrl1(ctrl, para);

	if(result && respond)
	{
		audio_reg_set1();
	}
}

#endif

