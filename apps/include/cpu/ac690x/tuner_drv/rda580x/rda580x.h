#ifndef __RDA580x_H__
#define __RDA580x_H__

#if defined(_TUNER_DRIVER_RDA580x_)


#define RDA580x_I2C_ADDR					(0x20)
#define RDA580x_I2C_SPECIAL_ADDR     		(0x2C)

#define RDA580x_I2C_RATE					(20)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

typedef enum
{
    RDA580x_TUNER_BAND_USA = 0,				//china/USA band   	87---108MHZ
    RDA580x_TUNER_BAND_JAPAN,				//Japan band   		76---91MHZ
    RDA580x_TUNER_BAND_WORD,				//word band    		76---108MHZ
    RDA580x_TUNER_BAND_OIRT,				//oirt band  			50---76MHZ
}rda580x_tuner_band_e;

typedef enum
{
	RDA580x_TUNER_SPAN_100KHZ = 0,
	RDA580x_TUNER_SPAN_200KHZ,
	RDA580x_TUNER_SPAN_50KHZ,
    RDA580x_TUNER_SPAN_25KHZ,
}rda580x_tuner_span_e;


typedef struct{
	struct{
		uint16 	enable:1;			//Power up Enable
		uint16 	soft_reset:1;		//Soft Reset
		uint16	new_method:1;		//
		uint16	rda_en:1;			//Rda Enable	
		uint16 	clk_mode:3;			//ClkMode 000=32.768KHz(default) 001=12MHz 101=24MHz 010=13MHz 110=26MHz 011=19.2MHz 111=38.4MHz
		uint16	seek_mode:1;		//
		uint16	seek:1;				//
		uint16	seek_up:1;			//
		uint16 	clk_32k_ext_en:1;	//0=enable 1=disenable
		uint16 	rclk:1;
		uint16	bass:1;				//Bass Boost
		uint16	mono:1;				//Mono Select
		uint16	mute_dis:1;			//Mute Disable
		uint16	hiz_dis:1;			//Audio Output High-Z Disable
	}reg2;

	struct{
		uint16	span:2;				//Channel Spacing
		uint16	band:2;				//Band Select
		uint16	tune:1;				//Tune
		uint16	:1;
		uint16	chan:10;			//Channel Select	
	}reg3;

	struct{
		uint16	gpio_1:2;
		uint16	gpio_2:2;
		uint16	gpio_3:2;
		uint16	i2c_en:1;			//I2S Bus Enable
		uint16	:1;
		uint16	afc_dis:1;
		uint16	soft_mute:1;		//De-emphasis
		uint16	:1;
		uint16	de:1;				//De-emphasis
		uint16	:2;	
		uint16	stc_int_en:1;		//Seek/Tune Complete Interrupt Enable
		uint16	:1;		
	}reg4;			
	
	struct{
		uint16	volume:4;			//Volume
		uint16	lna_current_sel:2;	//Lna Working Current Bit
		uint16	lna_port_sel:2;		//Lna Input Port Selecion Bit
		uint16	seek_th:7;			//RSSI Seek Threshold
		uint16	int_mode:1;
	}reg5;

	struct{
		uint16	r_dely:1;			//
		uint16	l_dely:1;			//
		uint16	sclk_o_edge:1;		//
		uint16	sw_o_edge:1;		//
		uint16	i2s_sw_cnt:4;		//
		uint16	ws_i_edge:1;		//
		uint16	data_signed:1;		//
		uint16	sclk_i_edge:1;		//
		uint16	sw_lr:1;			//
		uint16	i2s_mode:1;			//
		uint16	open_mode:2;		//
		uint16	:1;	
	}reg6;
	
	struct{
		uint16	freq_mode:1;		//
		uint16	soft_blend_en:1;	//
		uint16	seek_th:6;			//
		uint16	:1;
		uint16	band_mode:1;		//
		uint16	th_soft_blend:5;	//
		uint16	:1;
	}reg7;
	
	uint16	reg8;
	uint16	reg9;	

	struct{
		uint16	chan:10;			//Read Channel
		uint16	st:1;				//Stereo Indicator
		uint16	:1;		
		uint16	:1;		
		uint16	sf:1;				//Seek Fail/Band Limit 
		uint16	stc:1;				//Seek/Tune Complete
		uint16	:1;		
	}reg10; 

	struct{
		uint16	blerb:2;			//
		uint16	blera:2;			//
		uint16	abcd_en:1;			//
		uint16	:2;					//
		uint16	ready:1;			//
		uint16	valid:1;			//
		uint16	rssi:7;				//RSSI (Received Signal Strength Indicator). 			
	}reg11;

	uint16	reg12;
	uint16	reg13;
	uint16	reg14_device_id;		//Device Id
	uint16	reg15;
}__attribute__ ((packed)) rda580x_reg_t;


typedef struct{
	int8	TunerAutoSeek;
	uint8 	tuner_band;
	uint8  	init_succeed;
	uint8	freq_change_flag;
	uint8	freq_rssi_max;
	
	uint16 	now_freq;
	uint16 	min_freq;
	uint16 	max_freq;
	uint32	last_sys_timer;
	
	rda580x_reg_t reg;
}rda580x_ctrl_t;

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

#if defined(RDA580x_TUNER_RSSI_FM_VALUE)
	#define RDA580x_TUNER_RSSI_FM_DEFAULT				(RDA580x_TUNER_RSSI_FM_VALUE)								//	dBu
#else
	#define RDA580x_TUNER_RSSI_FM_DEFAULT				(23)														//	dBu
#endif

#define RDA580x_TUNER_RSSI_FM							RDA580x_TUNER_RSSI_FM_DEFAULT								//	dBu

#if !defined(RDA580x_TUNER_RSSI_LOC_FM)
#define RDA580x_TUNER_RSSI_LOC_FM						(RDA580x_TUNER_RSSI_FM + 20)								//	dBu
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

extern const tuner_drv_fun_t tuner_rda580x_drv_fun;


#endif

#endif /* __RDA580x_H__ */

