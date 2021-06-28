#ifndef __AR1019_H__
#define __AR1019_H__


#if defined(_TUNER_DRIVER_AR1019_)

#define AR1019_I2C_ADDR					(0x20)
#define AR1019_I2C_READ_ADDR			(AR1019_I2C_ADDR | _B0_)
#define AR1019_I2C_WRITE_ADDR			(AR1019_I2C_ADDR)

#define AR1019_I2C_RATE					(40)//20


/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#define AR1019_REG_ADDR_R0				(0x00)
#define AR1019_REG_ADDR_R1				(0x01)
#define AR1019_REG_ADDR_R2				(0x02)
#define AR1019_REG_ADDR_R3				(0x03)
#define AR1019_REG_ADDR_R4				(0x04)
#define AR1019_REG_ADDR_R5				(0x05)
#define AR1019_REG_ADDR_R6				(0x06)
#define AR1019_REG_ADDR_R7				(0x07)
#define AR1019_REG_ADDR_R8				(0x08)
#define AR1019_REG_ADDR_R9				(0x09)
#define AR1019_REG_ADDR_R10				(0x0A)
#define AR1019_REG_ADDR_R11				(0x0B)
#define AR1019_REG_ADDR_R12				(0x0C)
#define AR1019_REG_ADDR_R13				(0x0D)
#define AR1019_REG_ADDR_R14				(0x0E)
#define AR1019_REG_ADDR_R15				(0x0F)
#define AR1019_REG_ADDR_R16				(0x10)
#define AR1019_REG_ADDR_R17				(0x11)
#define AR1019_REG_ADDR_RSSI			(0x12)
#define AR1019_REG_ADDR_STATUS			(0x13)
#define AR1019_REG_ADDR_RBS				(0x14)
#define AR1019_REG_ADDR_RDS1			(0x15)
#define AR1019_REG_ADDR_RDS2			(0x16)
#define AR1019_REG_ADDR_RDS3			(0x17)
#define AR1019_REG_ADDR_RDS4			(0x18)
#define AR1019_REG_ADDR_RDS5			(0x19)
#define AR1019_REG_ADDR_RDS6			(0x1A)
#define AR1019_REG_ADDR_DEVID			(0x1B)
#define AR1019_REG_ADDR_CHIPID			(0x1C)


typedef struct{
//--------------------------------------------------------------------------------------------
	struct{
		uint16	Enable:1;		//Analog and digital blocks enable signals. 
		uint16	:6;
		uint16	XoEn:1;			//Internal oscillator enable signal. Logic '0' for using external reference clock, logic '1' for enable the internal oscillator. 
		uint16	:8;
	}AR1019_REG0;

	struct{
		uint16	:1;
		uint16	Hmute:1;		//Hard mute control signal. Logic '1' for enable; logic '0' for disable 
		uint16	Smute:1;		//Soft mute control signal. Logic '1' for enable; logic '0' for disable. Disable it when testing sensitivity. 
		uint16	Mono:1;			//Forced mono control signal. Logic '1' for mono; logic '0' depends on signal strength.  
		uint16	Deemp:1;		//De-emphasis. Logic '1' for 75-us de-emphasis, logic '0' for 50-us de-emphasis.
		uint16	StcIntEn:1;
		uint16	:10;
	}AR1019_REG1;

	struct{
		uint16 Chan1_9:9;		//Channel setting control signals. From 9'd0 to 9'd511. The FM channel is mapped by the equation. 
		uint16 Tune:1;			//TUNE channel enable signals. '1' = Enable, '0' = Disable. Each tuning process must set this signal from '0' to '1' to get the enabling edge signal. The STC flag is cleared to '0' automatically if TUNE is set to '0'. 
		uint16 ExtClkSw:1;		//Logic¡¯1¡¯: External reference clock mode. Logic¡¯0¡¯: Crystal mode 
		uint16 Chan0:1;			//Channel setting control signals. From 9'd0 to 9'd511. The FM channel is mapped by the equation. 
		uint16 :4;
	}AR1019_REG2;

	struct{
		uint16 Seekth:7;		//SEEK threshold control signals. Setting too high would result in channel missing.
		uint16 Volumn:4;		//EAR amplifier analog gain control signals while volreg_sw=0.
		uint16 Band:2;			//Band control signal. 
		uint16 Space:1;			//Channel spacing setting signal for SEEK operation. Logic '1' for 100k, logic '0' for 200k. 
		uint16 Seek:1;			//SEEK channel enable signals ('1' for enable, '0' for disable). Each seeking process must set this signal from '0' to '1' to get the enabling edge signal. 
		uint16 Seekup:1;		//Seek direction control signals. Logic '1' for seek up, logic '0' for seek down.	
	}AR1019_REG3;
	
	uint16 AR1019_REG4;
	uint16 AR1019_REG5;

	struct{
		uint16 Volumn2:4;		//Volume control 2 while volreg_sw=1. 
		uint16 Volume:4;		//EAR amplifier analog gain control signals while volreg_sw=1.  
		uint16 :6;
		uint16 Ear32en_n:1;		//0: for 32 Ohm earphone: 1: Audio out to audio amplifier. 
		uint16 Volreg_Sw:1;		//: Volume control is in R6 VOLUME and VOLUME2 0: Volume control is in R3 VOLUME and in R14 VOLUME2
	}AR1019_REG6;

	struct{
		uint16 :4;
		uint16 ExtClk3:3;		//External reference clock setting.
		uint16 ExtClk2:1;		//External reference clock setting.
		uint16 ExtClk1:1;		//External reference clock setting. 
		uint16 :7;		
	}AR1019_REG7;

	uint16 AR1019_REG8;

	struct{
		uint16 ExtClk4:9;		//External reference clock setting.
		uint16 :7;		
	}AR1019_REG9;
	
	struct{
		uint16 :3;		
		uint16 SeekWrap:1;		//SEEK wrap enable signal 1:wrap 0:no wrap 
		uint16 :12;		
	}AR1019_REG10;

	struct{
		uint16 Hiloctrl_b2:1;	//High side control bit 2
		uint16 :1;
		uint16 Hiloctrl_b1:1;	//High side control bit 1
		uint16 :12;
		uint16 HiloSide:1;		//AFC high side and low side injection control.
	}AR1019_REG11;

	uint16 AR1019_REG12;
	uint16 AR1019_REG13;

	struct{
		uint16 :12;
		uint16 Volume2:4;		//Volume control 2 while volreg_sw=0. 
	}AR1019_REG14;

	struct{
		uint16 :8;
		uint16 Space50K:1;		//Channel spacing setting signal for SEEK operation. 
		uint16 :7;
	}AR1019_REG15;

	struct{
		uint16 :6;
		uint16 ExtAcClk:1;		//Logic '0': External DC couple reference clock. Logic '1' External AC couple reference clock.
		uint16 :9;
	}AR1019_REG16;

	uint16 AR1019_REG17;

	struct{
		uint16 IfCnt:9;			//IF frequency counter 
		uint16 Rssi:7;			//RF input signal strength 
	}Rssi;

	struct{
		uint16 ReadChan0:1;		//Current Channel number. Channel mapping is the same as CHAN<9:0>.
		uint16 :2;
		uint16 St:1;			//Stereo flag, '1'=Stereo, '0'=Mono
		uint16 Sf:1;			//Seek fail flag, '1'=Seek Fail, '0'=Successful
		uint16 Stc:1;			//Seek/Tune complete flag, '1'=Complete, '0'=Incomplete
		uint16 :1;
		uint16 ReadChan1_9:9;	//Current Channel number. Channel mapping is the same as CHAN<9:0>.
	}Status;

	uint16 RBS;
	uint16 RDS1;
	uint16 RDS2;
	uint16 RDS3;
	uint16 RDS4;
	uint16 RDS5;
	uint16 RDS6;
	
	struct{
		uint16 MfId:12;			//Manufacturer ID (12'h5B1)
		uint16 Version:4;		//FM radio version control (4'b0110) 
	}Devid;

	uint16 ChipId;				//FM radio IC No. (16¡¯h1010 ) 		
}__attribute__ ((packed)) ar1019_reg_t;

typedef struct{
	uint8 init_succeed;
	radio_band_e band;
	uint8 TunerAutoSeek;
	uint16 now_freq;
	uint16 min_freq;
	uint16 max_freq;
	ar1019_reg_t reg;
}ar1019_ctrl_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#if !defined(AR1019_TUNER_RSSI_FM_VALUE)
#if defined(_ENABLE_AR1019_RSSI_)
	#define AR1019_TUNER_RSSI_FM_VALUE						(17)//10//20-22db
#else
	#define AR1019_TUNER_RSSI_FM_VALUE						(10)//10//20-22db
#endif
#endif

#define AR1019_TUNER_RSSI_FM								AR1019_TUNER_RSSI_FM_VALUE


#if !defined(AR1019_TUNER_RSSI_LOC_FM_VALUE)
	#define AR1019_TUNER_RSSI_LOC_FM_VALUE					(AR1019_TUNER_RSSI_FM + 20)
#endif
#define AR1019_TUNER_RSSI_LOC_FM							AR1019_TUNER_RSSI_LOC_FM_VALUE

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

extern const tuner_drv_fun_t tuner_ar1019_drv_fun;

#endif

#endif /* __AR1019_H__ */

