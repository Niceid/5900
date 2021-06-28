#ifndef __EW730_H__
#define __EW730_H__


#if defined(_TUNER_DRIVER_EW730_)

#define EW730_I2C_ADDR				0xC8
			
#define EW730_I2C_READ_ADDR			(EW730_I2C_ADDR | _B0_)
#define EW730_I2C_WRITE_ADDR		(EW730_I2C_ADDR)

#define EW730_I2C_RATE				(20)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
enum{
	EW730_BAND_REC_FM = 0,
	EW730_BAND_REC_AM,
};



// Register 00h
#define EW730_PN				0xF000
#define EW730_MFGID				0x0FFF

// Register 01h
#define EW730_REV				0xFC00
#define EW730_DEV				0x03C0
#define EW730_FIRMWARE			0x003F

// Register 02h
#define EW730_DSMUTE			0x8000
#define EW730_DMUTE				0x4000	// will mark
#define EW730_MONO				0x2000
#define EW730_DE				0x1000
#define EW730_BLNDADJ			0x0300

#define EW730_BLNDADJ_DN12DB	0x0300
#define EW730_BLNDADJ_DN6DB		0x0200
#define EW730_BLNDADJ_UP6DB		0x0100
#define EW730_BLNDADJ_DEF		0x0000

#define EW730_SMUTER			0x00C0
#define EW730_SMUTEA			0x0030
#define EW730_VOLUME		 	0x000F

// Register 03h
#define EW730_TUNE				0x8000
#define EW730_SetCHIndex		0x7FFF

#if 1		// will set to 0
#define EW730_BAND				0x3000
#define EW730_BAND_JAPAN		0x2000
#define EW730_BAND_JAPANW		0x1000
#define EW730_BAND_USA_EUR		0x0000
#define EW730_SPACE				0x0C00
#define EW730_SPACE_50KHZ		0x0800
#define EW730_SPACE_100KHZ		0x0400
#define EW730_SPACE_200KHZ		0x0000
#define EW730_CHAN				0x03FF
#endif

// Register 04h
#define EW730_RDSIEN			0x8000
#define EW730_STCIEN			0x4000
#define EW730_AGCD				0x2000
#define EW730_RDS				0x1000

// Register 05h
#define EW730_SEEK				0x8000
#define EW730_SEEKUP			0x4000
#define EW730_SKMODE			0x2000
#define EW730_SEEKTH			0x00FF

// Register 06h
#define EW730_ENABLE			0x8000
#define EW730_DISABLE			0x4000

#define EW730_32_768K			0x0000
#define EW730_12M				0x0001
#define EW730_13M				0x0002
#define EW730_19_2M				0x0003
#define EW730_24M				0x0005
#define EW730_26M				0x0006
#define EW730_38_4M				0x0007

// Register 07h
#define EW730_XOSCEN		 	0x8000
#define EW730_SRST		 		0x0800

#define EW730_GPIO3				0x0030
#define EW730_GPIO3_HI			0x0030
#define EW730_GPIO3_LO			0x0020
#define EW730_GPIO3_ST			0x0010
#define EW730_GPIO3_HIZ			0x0000

#define EW730_GPIO2				0x000C
#define EW730_GPIO2_HI			0x000C
#define EW730_GPIO2_LO			0x0008
#define EW730_GPIO2_INT			0x0004
#define EW730_GPIO2_HIZ			0x0000

#define EW730_GPIO1				0x0003
#define EW730_GPIO1_HI			0x0003
#define EW730_GPIO1_LO			0x0002
#define EW730_GPIO1_HIZ			0x0000

// Register 08h
#define EW730_ASNR				0x003F

// Register 09h
//#define EW730_OFSTH			0xFF00
//#define EW730_QLTTH			0x00FF

#if 0
#define EW730_SMUTER			0xC000
#define EW730_SMUTER_SLOWEST	0xC000
#define EW730_SMUTER_SLOW		0x8000
#define EW730_SMUTER_FAST		0x4000
#define EW730_SMUTER_FASTEST	0x0000

#define EW730_SMUTEA			0x3000
#define EW730_SMUTEA_10DB		0x3000
#define EW730_SMUTEA_12DB		0x2000
#define EW730_SMUTEA_14DB		0x1000
#define EW730_SMUTEA_16DB		0x0000

#define EW730_RDSPRF			0x0200
#define EW730_VOLEXT			0x0100
#define EW730_SKSNR				0x00F0
#define EW730_SKCNT				0x000F

#define EW730_AHIZEN		 	0x4000
#endif

// Register 0Ah
#define EW730_RDSR				0x8000
#define EW730_STD				0x4000
#define EW730_SFBL				0x2000
#define EW730_AFCRL			0x1000
#define EW730_RDSS				0x0800
#define EW730_ST				0x0400
//#define EW730_READCHAN			0x03FF

// Register 0Bh
#define EW730_BLERA				0xC000
#define EW730_BLERB				0x3000
#define EW730_BLERC				0x0C00
#define EW730_BLERD				0x0300
#define EW730_RSSI				0x00FF

// Register 0Ch
#define EW730_RDSA				0xFFFF

// Register 0Dh
#define EW730_RDSB				0xFFFF

// Register 0Eh
#define EW730_RDSC				0xFFFF

// Register 0Fh
#define EW730_RDSD				0xFFFF

// Register 12h
#define EW730_DMUTEL			0x0800
#define EW730_DMUTER			0x0400
#define EW730_LVOLUME			0x03E0
#define EW730_RVOLUME			0x001F

// Register 13h
#define EW730_RadioMode			0xC000
#define EW730_FMSpace			0x1F00
#define EW730_AMSWSpace			0x000F

// Register 14h
#define EW730_FMFreqTop			0x7FFF

// Register 15h
#define EW730_FMFreqBottom		0x7FFF

// Register 16h
#define EW730_AMSWFreqTop		0x7FFF

// Register 17h
#define EW730_AMSWFreqBottom	0x7FFF

// Register 1Eh
#define EW730_ReadCHIndex		0x7FFF

// Register A4h
#define EW730_ACHFLAG			0x4000

// Register ACh
#define EW730_AMMMODE			0x4000
#define EW730_AMSSIG			0x2000
#define EW730_AMGSIG			0x0800
#define EW730_AMBSIG			0x0400

// Register ADh
#define EW730_RSSISUM			0xFFFF

// Register AEh
#define EW730_RSSIINB			0xFFFF

// Register BDh
#define EW730_FREQOFFSET		0x7FFF

// Register DBh
#define EW730_BSNRNUMER 		0xFFFF

// Register DCh
#define EW730_BSNRDENOMIN		0xFFFF



#define EW730_REG_02H		(0x02)
#define EW730_REG_03H		(0x03)
#define EW730_REG_04H		(0x04)
#define EW730_REG_05H		(0x05)
#define EW730_REG_07H		(0x07)
#define EW730_REG_0AH		(0x0A)
#define EW730_REG_0BH		(0x0B)


/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct{

	uint16 chip_id;		        //CHIPID[7:0] default:0x5830 
	
	uint16 reg_02h;

	uint16 reg_03h;

	uint16 reg_04h;		
	
	uint16 reg_05h;
	
	uint16 reg_06h;
	
	uint16 reg_07h;
	
	uint16 reg_0Ah; 
	
	uint16 reg_0Bh;

	uint16 reg_10h;				//50KHz detect flag

	uint16 reg_40h;

	uint16 freq_am;				//AM Band Frequency
	uint16 read_freq_am;		//Current AM Frequency in chip
	
}__attribute__ ((packed)) ew730_reg_t;

#if defined(_ENABLE_RADIO_RDS_)
typedef struct{
	uint8	enable;
	uint8	factor;				//	
	uint8  	quality;			//	
	uint8	discard;			//	
	uint16 	ble_a;
	uint16 	ble_b;
	uint16 	ble_c;
	uint16 	ble_d;
}ew730_rds_t;
#endif

typedef struct{
	uint8 	TunerBand;
	int8	TunerAutoSeek;
	uint8 	tuner_band;
	uint8  	init_succeed;
	
	uint16 	now_freq;
	uint16 	min_freq;
	uint16 	max_freq;
	
	ew730_reg_t reg;
#if defined(_ENABLE_RADIO_RDS_)
	ew730_rds_t tuner_rds;
#endif
}ew730_ctrl_t;

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/

#if !defined(EW730_TUNER_RSSI_FM_VALUE)
	#define EW730_TUNER_RSSI_FM_VALUE				(23)//(28)//(40)
#endif

#define EW730_TUNER_RSSI_FM							EW730_TUNER_RSSI_FM_VALUE 

#if !defined(EW730_TUNER_RSSI_LOC_FM)
#if defined(_YS_SEL_SETTING_)
#define EW730_TUNER_RSSI_LOC_FM						(EW730_TUNER_RSSI_FM + 10)
#else
#define EW730_TUNER_RSSI_LOC_FM						(EW730_TUNER_RSSI_FM + 20)
#endif
#endif

#if defined(_YS_SEL_SETTING_)
#define EW730_RSSI_STO								(32)
#else
#define EW730_RSSI_STO								(20)
#endif


#if !defined(EW730_TUNER_RSSI_AM_VALUE)
	#define EW730_TUNER_RSSI_AM_VALUE				(35)//(10)// 
#endif

#define EW730_TUNER_RSSI_AM							(EW730_TUNER_RSSI_AM_VALUE)

#define EW730_TUNER_RSSI_LOC_AM						(EW730_TUNER_RSSI_AM + 20)

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

extern const tuner_drv_fun_t tuner_ew730_drv_fun;


#endif
#endif

