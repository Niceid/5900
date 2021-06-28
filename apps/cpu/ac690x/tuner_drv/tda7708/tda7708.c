#include "tda7708/tda7708.h"
#include "rds/rds_dec.h"

#include "setup.h"

#if defined(_TUNER_DRIVER_TDA7708_)

static tda7708_ctrl_t *tda7708_ctrl = NULL;
static const uint16 fm_tda7708_deal_spot_tab[] = {9600};
static const uint16 am_tda7708_deal_spot_tab[] = {};
extern volatile U32 g_rand_seed;
static uint8 tda7708_cmd_check = FALSE;
static uint32 tda7708_pilotth = 0;
tda7708_area_e tda7708_area = TDA7708_AREA_EUROPE;
RadioStuct Radio;
RadioDetect Radio_detect;
#define DEFAULT 	0xFFFFFFFF
#define PILOT_THS_ADDR 		0x01B05D

#if defined(_ENABLE_RADIO_RDS_)
RDSData_Struct RDSData;
RDSAppDataStruct RDSApp;
extern void RDSDataExtract(eTuner_Channel tunerChannel);
#endif

extern void Star_Cmd_Tuner_Seek_End (eTuner_Channel tunerChannel, U32 SeekEndAction);
extern void Star_Cmd_Tuner_Get_Seek_Status(eTuner_Channel tunerChannel);
extern void Star_Cmd_Tuner_Set_BB_IF(U32 BBConfig);
extern void TunerWaitReady(eTuner_Channel tunerChannel);
extern uint8 tda7708_cmd_set_manual_seek(int FreqIncrement);
extern void Wait1ms(unsigned char time);

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static void delay_1us(uint8 count)
{
	uint8 i,j;
	for(i = 0;i < count; i++)
	{
		for(j = 0; j < 10; j++)
		{
			asm volatile ("nop");
		}
	}
}

static uint8 Write_TDA7708_RegMap(const uint8 *value, uint16 length)
{
	if(!length)return TRUE;
	i2c_data_init(TDA7708_I2C_ADDR, TDA7708_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(value, length);
	i2c_data_exit();
	return result;
}

static uint8 Read_TDA7708_RegMap(uint8 *value, uint16 length)
{
	if(!length)return TRUE;

	i2c_data_init(TDA7708_I2C_ADDR, TDA7708_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_r(value, length);
	i2c_data_exit();
	return result;
}

static uint8 check_detect_tda7708(void)
{
	i2c_data_init(TDA7708_I2C_ADDR, TDA7708_I2C_RATE, &radio_gpio);
	uint8 result = i2c_data_w(NULL, 0);
	i2c_data_exit();

	return result;
}

static uint8 tda7708_i2c_download(const uint8* value)
{
	while((*value) != 0xFF)
	{
		uint8 length = (*value++) * 4;
		uint8 buffer[3 + length];

		buffer[0] = ((*value++) & 0x01)|0xE0;
		buffer[1] = (*value++);
		buffer[2] = (*value++);
		memcpy(&buffer[3], value, length);
		value += length;

		if(!i2c_data_w(buffer, 3 + length))return FALSE;
	}

	return TRUE;
}

static uint8 Write_TDA7708_Download(const uint8 *value)
{
	i2c_data_init(TDA7708_I2C_ADDR, TDA7708_I2C_RATE, &radio_gpio);
	uint8 result = tda7708_i2c_download(value);
	i2c_data_exit();

	return result;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
static uint8 tda7708_direct_write(tda7708_com_e com_type, uint8 check)
{
	uint8 iRet = FALSE;
	switch(com_type)
	{
		case TDA7708_COM_1:
			{
				uint8 buffer[3 + 4];
				uint16 length = 0;
				
				buffer[length++] = (tda7708_order.addr[0] & 0x01)|0x80;
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];
				buffer[length++] = tda7708_order.buffer[0];
				buffer[length++] = tda7708_order.buffer[1];
				buffer[length++] = tda7708_order.buffer[2];
				buffer[length++] = tda7708_order.buffer[3];
				
				iRet = Write_TDA7708_RegMap(buffer, length);
			}
			break;
			
		case TDA7708_COM_2:
			{
				uint8 buffer[3 + 3];
				uint16 length = 0;
				
				buffer[length++] = (tda7708_order.addr[0] & 0x01)|0x90;
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];
				buffer[length++] = tda7708_order.buffer[0];
				buffer[length++] = tda7708_order.buffer[1];
				buffer[length++] = tda7708_order.buffer[2];
				
				iRet = Write_TDA7708_RegMap(buffer, length);
			}
			break;
			
		case TDA7708_COM_3:
			{
				uint8 buffer[3 + 4 * tda7708_order.length + 4];
				uint16 checksum[4] = {0, 0, 0, 0};
				uint16 length = 0;
				
				buffer[length++] = (tda7708_order.addr[0] & 0x01)|0xE0;
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];
				
				uint16 index;
				for(index = 0; index < tda7708_order.length; index++)
				{
					buffer[length++] = tda7708_order.buffer[index * 4 + 0];
					buffer[length++] = tda7708_order.buffer[index * 4 + 1];
					buffer[length++] = tda7708_order.buffer[index * 4 + 2];
					buffer[length++] = tda7708_order.buffer[index * 4 + 3];
					
					if(check)
					{
						uint8 j;
						for(j = 3; j >= 1;j--)
						{
							checksum[j] += tda7708_order.buffer[index * 4 + j];
							if(checksum[j] >= 256)
							{
								checksum[j - 1] ++;
								checksum[j] -= 256;
							}
						}
						checksum[0] += tda7708_order.buffer[index * 4];
						if(checksum[0] >= 256)
						{
							checksum[0] -= 256;
						}
					}
				}
				
				if(check)
				{
					buffer[length++] = _u16to8l_(checksum[0]);
					buffer[length++] = _u16to8l_(checksum[1]);
					buffer[length++] = _u16to8l_(checksum[2]);
					buffer[length++] = _u16to8l_(checksum[3]);
				}
				
				iRet = Write_TDA7708_RegMap(buffer, length);
			}
			break;
			
		case TDA7708_COM_4:
			{
				uint8 buffer[3 + 3 * tda7708_order.length + 3];
				uint16 checksum[3] = {0, 0, 0};
				uint16 length = 0;
				
				buffer[length++] = (tda7708_order.addr[0] & 0x01)|0xF0;
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];
				
				uint16 index;
				for(index = 0; index < tda7708_order.length; index++)
				{
					buffer[length++] = tda7708_order.buffer[index * 3 + 0];
					buffer[length++] = tda7708_order.buffer[index * 3 + 1];
					buffer[length++] = tda7708_order.buffer[index * 3 + 2];
					
					if(check)
					{
						uint8 j;
						for(j = 2; j >= 1;j--)
						{
							checksum[j] += tda7708_order.buffer[index * 3 + j];
							if(checksum[j] >= 256)
							{
								checksum[j - 1] ++;
								checksum[j] -= 256;
							}
						}
						checksum[0] += tda7708_order.buffer[index * 3];
						if(checksum[0] >= 256)
						{
							checksum[0] -= 256;
						}
					}
				}
				
				if(check)
				{
					buffer[length++] = _u16to8l_(checksum[0]);
					buffer[length++] = _u16to8l_(checksum[1]);
					buffer[length++] = _u16to8l_(checksum[2]);
				}
				
				iRet = Write_TDA7708_RegMap(buffer, length);
			}
			break;
			
		default:
			break;
	}

	return iRet;
}


static uint8 tda7708_direct_read(tda7708_com_e com_type)
{	
	uint8 iRet = FALSE;
	switch(com_type)
	{
		case TDA7708_COM_1:
			{
				uint8 buffer[3];
				uint16 length = 0;
				
				buffer[length++] = tda7708_order.addr[0];
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];

				iRet = Write_TDA7708_RegMap(buffer, length);
				if(iRet)
				{
					delay_1us(50);
					iRet = Read_TDA7708_RegMap(tda7708_order.buffer, 4);
				}
			}			
			break;
			
		case TDA7708_COM_2:
			{
				uint8 buffer[3];
				uint16 length = 0;
				
				buffer[length++] = (tda7708_order.addr[0] & 0x01)|0x10;
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];
				
				iRet = Write_TDA7708_RegMap(buffer, length);
				if(iRet)
				{
					delay_1us(50);
					iRet = Read_TDA7708_RegMap(tda7708_order.buffer, 3);
				}
			}
			break;
			
		case TDA7708_COM_3:
			{
				uint8 buffer[3];
				uint16 length = 0;
				
				buffer[length++] = (tda7708_order.addr[0] & 0x01)|0x60;
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];
				
				iRet = Write_TDA7708_RegMap(buffer, length);
				if(iRet)
				{
					delay_1us(50);
					iRet = Read_TDA7708_RegMap(tda7708_order.buffer, tda7708_order.length * 4);
				}
			}
			break;
			
		case TDA7708_COM_4:
			{
				uint8 buffer[3];
				uint16 length = 0;
				
				buffer[length++] = (tda7708_order.addr[0] & 0x01)|0x70;
				buffer[length++] = tda7708_order.addr[1];
				buffer[length++] = tda7708_order.addr[2];
				
				iRet = Write_TDA7708_RegMap(buffer, length);
				if(iRet)
				{
					delay_1us(50);
					iRet = Read_TDA7708_RegMap(tda7708_order.buffer, tda7708_order.length * 3);
				}
			}
			break;
			
		default:
			break;
	}

	return iRet;
}

static uint8 tda7708_cmd_write_read(uint8 cmd, uint8 write_length, uint8 read_length)
{
	uint8 check_1, check_2;
	
	tda7708_order.length	= write_length + 1;
	tda7708_order.addr[0]	= 0x01;
	tda7708_order.addr[1]	= 0x90;
	tda7708_order.addr[2]	= 0x00;
	
	check_1 = tda7708_order.buffer[0] = (cmd & 0xF0) >> 4;
	check_2 = tda7708_order.buffer[1] = (cmd & 0x0F) << 4;
	tda7708_order.buffer[2] = tda7708_order.length & 0x1F;
	
	if(!tda7708_direct_write(TDA7708_COM_4, TRUE))return FALSE;
	
	if(read_length)
	{
		tda7708_order.length	= read_length;
		tda7708_order.addr[0]	= 0x01;
		tda7708_order.addr[1]	= 0x90;
		tda7708_order.addr[2]	= 0x0A;
		
		if(!tda7708_direct_read(TDA7708_COM_4))return FALSE;
		
		if(tda7708_cmd_check)
		{
			if((tda7708_order.buffer[0] != check_1) || (tda7708_order.buffer[1] != check_2))return FALSE;
		}
	}
	
	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

//I2C boot code
static const uint8 TDA7708_I2CBootData[]=
{
	1 ,0x01,0xAE,0x00,0x00,0x00,0x03,0x00,
	16 ,0x01,0xC0,0x00,0xFB,0x80,0x10,0x00,0xFB,0xC6,0x26,0x00,0xFB,0xC6,0x40,0x00,0xFB,0xC6,0x42,0x00,0xFB,0xC6,0x44,0x00,0xFB,0xC6,0x4C,0x00,0xFB,0xC6,0x84,0x00,0xFF,0xF7,0x00,0x00,0xE1,0xFF,0xE0,0x1B,0x90,0x00,0x70,0x53,0x92,0x00,0x50,0x6B,0x92,0x00,0x90,0x1F,0xFB,0x27,0x00,0x00,0xFF,0xE6,0x02,0x98,0xFB,0x80,0x22,0x00,0xFB,0x80,0x00,0x80,
	32 ,0x01,0xC0,0x11,0xE0,0x7F,0xE0,0x00,0xE4,0x00,0x00,0x00,0xC0,0x80,0xA0,0x20,0xFB,0x27,0x02,0x43,0x4C,0x62,0x40,0x00,0xFF,0xF7,0x00,0x00,0xD8,0x00,0x00,0x20,0xFB,0x10,0x22,0x43,0x4C,0x62,0x00,0x00,0xC0,0x00,0x20,0x20,0x91,0xC0,0x00,0x00,0xFB,0x95,0x6C,0x40,0xC0,0x00,0x60,0x20,0x91,0xC0,0x00,0x00,0xFB,0x10,0x20,0x00,0xFB,0x8A,0x68,0x40,0xFC,0x04,0x00,0x1B,0x94,0x01,0x40,0x00,0xC0,0x00,0x20,0x20,0x94,0x73,0x60,0x1B,0x90,0x01,0xC0,0x00,0xA8,0x00,0x40,0x00,0x81,0x01,0x40,0x00,0xA0,0x01,0xC0,0x1B,0xFC,0x04,0x00,0x65,0xFB,0x80,0x5C,0x06,0xFB,0x80,0x52,0x01,0xFF,0xF7,0x00,0x00,0xA0,0x01,0xC0,0x6B,0xA0,0x75,0x00,0x00,0xFB,0x10,0x30,0x25,0xFC,0x04,0x10,0x43,
	32 ,0x01,0xC0,0x31,0xFC,0x04,0x00,0x27,0x90,0x75,0x00,0x1B,0x90,0x01,0xC0,0x00,0xFB,0xD2,0x06,0x40,0xAC,0x01,0x40,0x00,0xFC,0x04,0x00,0x61,0xFB,0x80,0x74,0x04,0xFB,0x10,0x20,0x00,0xFB,0xF7,0xCA,0x40,0xFB,0x10,0x20,0x00,0xFB,0xC8,0x2A,0x40,0xFB,0x10,0x20,0x00,0xFB,0xC4,0x8A,0x40,0xFB,0x10,0x20,0x00,0xFB,0x82,0x9E,0x40,0xA0,0x5D,0x00,0x00,0xC0,0x00,0x24,0xC4,0xFB,0x80,0x8A,0x04,0xFB,0x10,0x20,0x00,0xFB,0xC9,0x6A,0x40,0xA0,0x02,0x20,0x00,0xFB,0x10,0x20,0x25,0xC0,0x09,0x50,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xA0,0x5D,0x00,0x00,0xC0,0x00,0x24,0xC4,0xFB,0x80,0xB2,0x04,0xA0,0x01,0x00,0x00,0xFC,0x04,0x00,0x65,0xFB,0x80,0xAE,0x06,0xFB,0x10,0x20,0x00,
	32 ,0x01,0xC0,0x51,0xFB,0x89,0x6C,0x40,0xA0,0x42,0x00,0x00,0xFF,0xA8,0x04,0x87,0xFB,0x80,0xAE,0x04,0xFB,0x10,0x20,0x00,0xFB,0x86,0xB4,0x40,0xFB,0x10,0x20,0x00,0xFB,0x88,0x94,0x40,0xFB,0x10,0x20,0x00,0xFB,0xF0,0xAC,0x40,0xFB,0x10,0x20,0x00,0xFB,0xF5,0x72,0x40,0xFB,0x10,0x20,0x00,0xFB,0xC8,0x02,0x40,0xFB,0x10,0x20,0x00,0xFB,0xC7,0xEA,0x40,0xA2,0x01,0x00,0x00,0xA0,0x01,0x80,0x00,0xE1,0x09,0x00,0x00,0x4F,0xE7,0x00,0x00,0x40,0x27,0x00,0x00,0xA2,0x01,0x00,0x00,0xA0,0x01,0xA0,0x00,0xE1,0x09,0x20,0x00,0xFB,0x80,0x50,0x01,0x4F,0xE7,0x00,0x00,0x40,0x27,0x00,0x00,0xFF,0x27,0x00,0x00,0xFF,0xF7,0x00,0x00,0xFF,0xF7,0x00,0x00,0xA0,0x01,0xE0,0x00,0xE0,0x39,0xE0,0x00,
	32 ,0x01,0xC0,0x71,0x48,0x27,0x02,0x25,0xFC,0x04,0x00,0x33,0xFC,0x04,0x04,0xC6,0xFB,0x80,0xF0,0x0B,0xE4,0x26,0x00,0x00,0x4E,0x00,0x20,0x00,0x4E,0x25,0x00,0x00,0x4E,0x10,0xA9,0x25,0x4E,0x00,0x4C,0x06,0xFC,0x04,0x04,0x82,0x44,0x00,0x69,0x00,0x4E,0x00,0x84,0x01,0xFC,0x04,0x00,0x33,0x4C,0x67,0x00,0x00,0xFC,0x04,0x00,0x75,0xC0,0x00,0x24,0x08,0xC0,0x00,0xF0,0x20,0x7C,0x67,0x00,0x00,0xFC,0x04,0x00,0x46,0xFC,0x04,0x05,0x06,0xFC,0x04,0x00,0x33,0xFC,0x04,0x00,0x49,0xFC,0x04,0x00,0x42,0x08,0x67,0x10,0x1F,0x96,0xC0,0x00,0x00,0x94,0xE0,0x00,0x00,0xDE,0x0A,0xC0,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,
	32 ,0x01,0xC0,0x91,0xFB,0x81,0x1E,0x06,0xFB,0x80,0x00,0x80,0xE4,0x0E,0xC0,0x00,0xE0,0x10,0x00,0x00,0x48,0x00,0x20,0x00,0x4E,0x00,0x48,0x00,0xA0,0x22,0x40,0x24,0xFC,0x04,0x09,0x00,0xFC,0x04,0x02,0x24,0xFC,0x04,0x00,0x33,0xFF,0xA8,0x04,0x80,0xFB,0x81,0x40,0x04,0xFB,0x81,0x42,0x01,0xFF,0xF7,0x00,0x00,0x4E,0x00,0xC0,0x00,0x4E,0x25,0x00,0x00,0x00,0x67,0x09,0x00,0xFC,0x04,0x08,0x24,0xFC,0x04,0x04,0x06,0xFC,0x04,0x14,0x82,0xFC,0x04,0x00,0x37,0xFB,0x80,0x00,0x81,0x08,0x67,0x00,0x00,0x08,0xA7,0x00,0x00,0xA0,0x07,0x00,0x00,0xFC,0x40,0x00,0x00,0xE4,0x7A,0x60,0x00,0xFC,0xA4,0x00,0x18,0xFD,0x20,0x00,0x40,0xFF,0xF7,0x00,0x00,0x48,0xA5,0x40,0x1B,0x48,0xA5,0x48,0x00,
	32 ,0x01,0xC0,0xB1,0xFE,0x27,0x00,0x06,0x48,0xA5,0x4C,0x06,0x48,0xA5,0x0C,0x06,0xFE,0x27,0x00,0x09,0x48,0xA5,0x6C,0x06,0xFC,0x04,0x0C,0x06,0xFC,0x04,0x04,0x02,0xFD,0x28,0x00,0x00,0xFF,0xF7,0x00,0x00,0xE0,0x65,0x00,0x33,0xE4,0x72,0x20,0x00,0x48,0x25,0x00,0x00,0x44,0x27,0x04,0x86,0x00,0x67,0x00,0x00,0xFC,0x04,0x08,0x00,0xFC,0x04,0x04,0x02,0xFC,0x04,0x00,0x33,0x08,0x67,0x00,0x3B,0x00,0xE7,0x00,0x00,0xFC,0x04,0x04,0xE6,0xFB,0x81,0x94,0x0D,0xFF,0xF7,0x00,0x00,0x4E,0x00,0x20,0x00,0x20,0xA7,0x00,0x00,0x4E,0x2E,0x00,0x00,0xFC,0x04,0x09,0x25,0x4E,0x00,0xCC,0x06,0xFC,0x04,0x04,0x82,0x08,0xC0,0x89,0x00,0xFC,0x04,0x00,0x24,0x09,0x27,0x00,0x00,0xA0,0x2C,0x40,0x00,
	32 ,0x01,0xC0,0xD1,0x4E,0x00,0xA8,0x00,0x4E,0x00,0x44,0x01,0x4C,0x67,0x00,0x00,0xFC,0x04,0x00,0x75,0xC0,0x00,0x24,0x08,0xC0,0x01,0xF0,0x20,0x7C,0x67,0x00,0x00,0xFC,0x04,0x00,0x46,0xFC,0x04,0x05,0x06,0xFC,0x04,0x00,0x33,0x4E,0x00,0x69,0x00,0xFC,0x04,0x00,0x24,0xFB,0x80,0x00,0x81,0xFC,0x04,0x00,0x49,0x08,0xA7,0x00,0x00,0xAC,0x79,0x40,0x00,0xFC,0x04,0x00,0x61,0xFB,0x82,0x06,0x05,0xFF,0xF7,0x00,0x00,0xE0,0x5E,0x00,0x00,0x01,0x27,0x00,0x00,0xAC,0x7B,0xE0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x00,0x0C,0x02,0x27,0x00,0x00,0xAC,0x7C,0x00,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x00,0x0A,0x00,0xA7,0x00,0x00,0xAC,0x7C,0x20,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x00,0x0E,
	32 ,0x01,0xC0,0xF1,0xA0,0x5D,0x40,0x00,0xAC,0x7C,0x40,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x00,0x0C,0xA0,0x5D,0x40,0x00,0xAC,0x6C,0xC0,0x55,0x90,0x5D,0x40,0x00,0xC0,0x01,0x80,0x20,0x90,0x61,0x40,0x00,0xA0,0x5D,0x40,0x00,0xA0,0x00,0x60,0x25,0xFC,0x04,0x00,0x4D,0xFB,0x82,0x0A,0x01,0xFC,0x04,0x00,0x41,0x90,0x5D,0x40,0x00,0xFB,0x10,0x20,0x00,0xFB,0xBA,0xF2,0x40,0xFB,0x82,0x0A,0x00,0xFC,0x04,0x00,0x1B,0x90,0x5D,0x40,0x00,0xFB,0x80,0x00,0x81,0xA0,0x64,0x80,0x00,0x80,0x5C,0x20,0x00,0xA0,0x5E,0xA0,0x00,0xAC,0x73,0x40,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x1E,0x08,0xA0,0x76,0xA0,0x00,0xFC,0x04,0x00,0x65,0xFB,0x82,0x2A,0x06,0xA0,0x5F,0xE0,0x00,0x80,0x74,0xC0,0x00,
	32 ,0x01,0xC1,0x11,0xFB,0x10,0x20,0x00,0xFB,0xC0,0xC6,0x40,0xA0,0x74,0xC0,0x00,0x80,0x5F,0xE0,0x00,0xFB,0x80,0x00,0x81,0xC0,0x23,0x00,0x20,0x90,0x5C,0x20,0x00,0xE0,0x5E,0x00,0x00,0x01,0x67,0x00,0x00,0xAC,0x7D,0x60,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x72,0x0C,0x01,0xA7,0x00,0x00,0xAC,0x7D,0x80,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x72,0x0C,0x00,0xA7,0x00,0x00,0xAC,0x7D,0xA0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x72,0x0C,0x01,0x27,0x00,0x00,0xAC,0x7D,0xC0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x72,0x0A,0x00,0xE7,0x00,0x00,0xAC,0x7D,0xE0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x72,0x0A,0xA0,0x76,0xA0,0x00,0xC0,0x00,0x24,0xC4,0xFB,0x82,0x72,0x06,0x03,0x67,0x00,0x00,
	32 ,0x01,0xC1,0x31,0xC0,0x00,0xE4,0xC4,0xFB,0x82,0x72,0x0A,0xFC,0x04,0x10,0x1F,0x2B,0x27,0x00,0x00,0xC0,0x00,0xF0,0x20,0x2B,0x67,0x00,0x00,0xC0,0x00,0x30,0x20,0x2B,0xA7,0x00,0x00,0xFB,0x80,0x00,0x81,0xC0,0x27,0x80,0x20,0x90,0x5C,0x20,0x00,0xE0,0x5E,0x00,0x00,0x01,0xA7,0x00,0x00,0xAC,0x73,0x20,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x82,0x98,0x0A,0x03,0x67,0x00,0x00,0xC0,0x00,0xC4,0xC4,0xFB,0x82,0x98,0x0A,0xFC,0x04,0x00,0x1B,0x0B,0x27,0x00,0x00,0xC0,0x00,0xC0,0x20,0x0B,0x67,0x00,0x00,0xD0,0x00,0x00,0x20,0xD0,0x00,0x24,0x48,0x4C,0x67,0x00,0x00,0x0B,0xA7,0x00,0x00,0xFB,0x80,0x00,0x81,0xA0,0x64,0x40,0x00,0x80,0x5C,0x20,0x00,0xE2,0x5C,0x20,0x00,0x11,0xE7,0x00,0x00,
	32 ,0x01,0xC1,0x51,0xFC,0x04,0x00,0x65,0xFB,0x82,0xDE,0x04,0xFC,0x04,0x00,0x1B,0x19,0xE7,0x00,0x00,0xDC,0x00,0x00,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0xA0,0x00,0x00,0xA8,0xA0,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x82,0xB2,0x06,0xA8,0xC0,0x00,0x00,0xFF,0xA8,0x04,0x85,0xFB,0x82,0xC8,0x05,0xFF,0xF7,0x00,0x00,0xAA,0xE0,0x00,0x00,0xFB,0x82,0xCA,0x01,0xFF,0xF7,0x00,0x00,0xFF,0xA8,0x02,0x85,0xFF,0xA8,0x00,0x85,0x94,0xC0,0x00,0x00,0x86,0xE0,0x00,0x00,0xDC,0x00,0x00,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x82,0xD6,0x06,0xFB,0x83,0xA6,0x00,0x10,0x67,0x00,0x00,0x10,0xA7,0x00,0x25,
	32 ,0x01,0xC1,0x71,0xFC,0x04,0x04,0xE6,0xFB,0x82,0xEA,0x04,0xC0,0x00,0x30,0x20,0x39,0xA7,0x00,0x00,0xC0,0x00,0x64,0xC8,0xFB,0x83,0x5C,0x0A,0xC0,0x00,0x04,0xC8,0xFB,0x83,0x5C,0x08,0x10,0xA7,0x00,0x00,0xC0,0x00,0x64,0xC4,0xFB,0x83,0x00,0x0C,0xAC,0x59,0x00,0x00,0xFC,0x04,0x10,0x21,0x38,0xE7,0x00,0x00,0x39,0x67,0x00,0x00,0x10,0x67,0x00,0x00,0xFC,0x04,0x00,0x25,0x18,0xA7,0x00,0x63,0xFB,0x83,0x24,0x06,0x11,0xA7,0x00,0x00,0xFC,0x04,0x00,0x65,0xFB,0x83,0x1E,0x04,0xA0,0x64,0xA0,0x00,0xA8,0x79,0xE0,0x25,0x54,0x27,0x00,0x25,0x1A,0xA7,0x00,0x00,0x10,0xE7,0x00,0x00,0xFB,0x83,0x24,0x01,0xFC,0x04,0x00,0x25,0x19,0x67,0x00,0x00,0x11,0x67,0x00,0x00,0xFC,0x04,0x00,0x25,
	32 ,0x01,0xC1,0x91,0x18,0xE7,0x00,0x00,0x10,0xE7,0x00,0x00,0xAC,0x59,0x40,0x25,0xAC,0x59,0x20,0x49,0x11,0x27,0x00,0x41,0x18,0xE7,0x04,0xE6,0xFB,0x83,0x34,0x04,0xC0,0x00,0x20,0x20,0x19,0xE7,0x00,0x00,0xFC,0x04,0x00,0x1B,0x19,0xA7,0x00,0x00,0x11,0xE7,0x00,0x00,0xFC,0x04,0x00,0x65,0xFB,0x83,0x4A,0x04,0xA0,0x64,0xC0,0x00,0xFB,0x10,0x20,0x25,0xC0,0x34,0x90,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x83,0xA6,0x00,0x10,0x67,0x00,0x00,0xC0,0x00,0x24,0xC4,0xFB,0x83,0xA6,0x04,0x58,0x27,0x00,0x00,0xFB,0x10,0x20,0x25,0xC0,0x35,0xB0,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x83,0xA6,0x00,0xC0,0x01,0x44,0xC8,0xFB,0x83,0x82,0x06,0x10,0x67,0x00,0x00,
	32 ,0x01,0xC1,0xB1,0xFC,0x04,0x00,0x25,0x18,0xA7,0x00,0x00,0x11,0xA7,0x00,0x00,0xFC,0x04,0x00,0x65,0xFB,0x83,0xA6,0x04,0xC0,0x00,0x30,0x20,0xA8,0x59,0x60,0x00,0x39,0xE7,0x00,0x25,0x18,0xE7,0x00,0x1B,0x19,0xA7,0x00,0x00,0xA0,0x64,0xC0,0x00,0xFB,0x10,0x20,0x25,0xC0,0x38,0x10,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x83,0xA6,0x00,0xC0,0x01,0x64,0xC8,0xFB,0x83,0xA6,0x06,0x10,0x67,0x00,0x00,0xFC,0x04,0x00,0x25,0x18,0xA7,0x00,0x00,0x11,0xA7,0x00,0x00,0xFC,0x04,0x00,0x65,0xFB,0x83,0xA6,0x04,0xC0,0x00,0x30,0x20,0xA8,0x59,0x80,0x00,0x39,0xE7,0x00,0x25,0x18,0xE7,0x00,0x1B,0x19,0xA7,0x00,0x00,0xA0,0x64,0xC0,0x00,0xFB,0x10,0x20,0x25,0xC0,0x3A,0x70,0x20,
	32 ,0x01,0xC1,0xD1,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x80,0x00,0x80,0xFB,0x10,0x20,0x00,0xFB,0xC2,0x94,0x40,0xA0,0x65,0x80,0x00,0xA0,0x0F,0x60,0x25,0xFC,0x04,0x10,0x25,0xFC,0x04,0x10,0x4B,0xFC,0x04,0x00,0x27,0x90,0x5D,0xC0,0x1B,0xA0,0x0B,0xC0,0x00,0xFF,0xA8,0x04,0x81,0xFB,0x83,0xC0,0x04,0xC0,0x00,0x20,0x20,0xFB,0x80,0x00,0x81,0xFF,0xF7,0x00,0x00,0x90,0x76,0xA0,0x00,0xE0,0x75,0x80,0x00,0x48,0x27,0x00,0x00,0xFC,0x04,0x00,0x65,0xFB,0x84,0x70,0x05,0xFF,0xF7,0x00,0x00,0xE4,0x7E,0x00,0x00,0xDE,0x01,0x00,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0xA0,0x00,0x00,0xA8,0xA0,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x83,0xDA,0x06,0x00,0x67,0x00,0x00,
	32 ,0x01,0xC1,0xF1,0xAA,0xE0,0x00,0x25,0xFC,0x04,0x04,0xD6,0xFB,0x83,0xEE,0x0D,0xFF,0xF7,0x00,0x00,0x4E,0x00,0x20,0x00,0x4E,0x25,0x00,0x00,0xFC,0x04,0x08,0x80,0x4E,0x00,0x8C,0x06,0xFC,0x04,0x04,0x82,0x08,0x67,0x00,0x00,0xA0,0x65,0x80,0x00,0x4E,0x00,0xA8,0x00,0x4E,0x00,0xC4,0x01,0xFC,0x04,0x00,0x33,0x4C,0x67,0x00,0x00,0xFC,0x04,0x00,0x75,0xC0,0x00,0x24,0x08,0xC0,0x00,0xD0,0x20,0x7C,0x67,0x00,0x00,0xFC,0x04,0x00,0x46,0x00,0x67,0x05,0x06,0xFC,0x04,0x00,0x33,0x4E,0x00,0xE0,0x49,0x4E,0x00,0x40,0x41,0xFC,0x04,0x08,0x33,0x4C,0x67,0x12,0x0C,0x08,0xA6,0x10,0x37,0x4E,0x00,0x69,0x80,0xFC,0x04,0x00,0x24,0xFC,0x04,0x00,0x49,0x08,0xE7,0x00,0x00,0x01,0x27,0x00,0x00,
	32 ,0x01,0xC2,0x11,0xA2,0x0D,0x60,0x25,0xFC,0x04,0x04,0xD6,0xFB,0x84,0x2E,0x0D,0xFF,0xF7,0x00,0x00,0x4E,0x01,0x20,0x00,0x4E,0x01,0x00,0x00,0xFC,0x04,0x08,0x80,0x4E,0x01,0x4C,0x06,0xFC,0x04,0x04,0x82,0x4C,0x67,0x00,0x00,0x09,0x01,0x68,0x00,0x4E,0x01,0x84,0x01,0xFC,0x04,0x00,0x33,0x4C,0x67,0x00,0x00,0xFC,0x04,0x00,0x75,0xC0,0x00,0x24,0x08,0xC0,0x00,0xD0,0x20,0x7C,0x67,0x00,0x00,0xFC,0x04,0x00,0x46,0xFC,0x04,0x05,0x06,0xFC,0x04,0x00,0x33,0x4E,0x01,0xA0,0x49,0x4E,0x01,0xC0,0x41,0x09,0x67,0x00,0xFB,0xFC,0x04,0x04,0x09,0x09,0xA7,0x00,0x00,0x00,0xE7,0x00,0x00,0x01,0xA7,0x03,0xC5,0xFC,0x04,0x02,0x43,0xC0,0x02,0x15,0x64,0x84,0xC0,0x04,0x0B,0x94,0xE0,0x00,0x00,
	32 ,0x01,0xC2,0x31,0xDC,0xA0,0x00,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x84,0x6A,0x06,0xFB,0x80,0x00,0x80,0xA0,0x76,0xC0,0x00,0xFC,0x04,0x00,0x65,0xFB,0x84,0x82,0x04,0xA0,0x76,0xE0,0x00,0xFB,0x10,0x20,0x25,0xC0,0x48,0x30,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x80,0x00,0x80,0xE0,0x76,0xE0,0x00,0x00,0xA7,0x00,0x00,0x20,0x67,0x00,0x25,0xFC,0x04,0x04,0xD6,0xFB,0x84,0x94,0x0D,0xFF,0xF7,0x00,0x00,0xA4,0x79,0xE0,0x00,0xA4,0x79,0xC0,0x00,0xFC,0x04,0x08,0x80,0xFC,0x04,0x0C,0x06,0xFC,0x04,0x04,0x82,0x08,0xA7,0x10,0x1F,0xA4,0x7A,0x00,0x00,0xFC,0x04,0x04,0xC9,0xFB,0x84,0xAA,0x0B,
	32 ,0x01,0xC2,0x51,0xFF,0xF7,0x00,0x00,0xA0,0x7A,0x20,0x00,0xA0,0x7A,0x40,0x00,0xCF,0xFF,0xF0,0x20,0xFC,0x04,0x12,0x47,0xFC,0x04,0x00,0x25,0x0A,0x67,0x00,0x00,0xA4,0x7A,0x60,0x00,0x2A,0x27,0x10,0x1F,0x00,0xA7,0x00,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x84,0xBC,0x0A,0xCF,0xFF,0xF0,0x20,0xFC,0x04,0x12,0x47,0x29,0xE7,0x00,0x00,0x00,0xE7,0x00,0x00,0xA4,0x7B,0xC0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x84,0xCE,0x0B,0xFF,0xF7,0x00,0x00,0xA2,0x7B,0xE0,0x00,0xA2,0x7C,0x00,0x00,0xA4,0x7C,0x20,0x26,0x0A,0xA7,0x04,0xC5,0xFB,0x84,0xDA,0x0B,0xFF,0xF7,0x00,0x00,0xA2,0x7C,0x40,0x00,0xA2,0x7C,0x60,0x00,0xFC,0x04,0x00,0x26,0x0A,0xE7,0x00,0x00,0xA0,0x7D,0x60,0x00,0xFB,0x80,0x00,0x81,
	32 ,0x01,0xC2,0x71,0xFC,0x04,0x00,0x25,0x44,0x27,0x00,0x00,0xE0,0x76,0xE0,0x00,0x00,0xE7,0x00,0x00,0xA2,0x65,0x80,0x25,0xFC,0x04,0x04,0xD6,0xFB,0x84,0xF6,0x0D,0xFF,0xF7,0x00,0x00,0xA4,0x7A,0xA0,0x00,0xA4,0x7A,0x80,0x00,0xFC,0x04,0x08,0x80,0xFC,0x04,0x0C,0x06,0xFC,0x04,0x04,0x82,0x08,0xE7,0x00,0x00,0xA4,0x7A,0xC0,0x00,0x00,0xE7,0x00,0x00,0xA4,0x7A,0xE8,0x00,0xA4,0x7B,0x04,0x01,0xFC,0x04,0x00,0x33,0x4C,0x67,0x00,0x00,0xFC,0x04,0x00,0x75,0xC0,0x00,0x24,0x08,0xC0,0x00,0xD0,0x20,0x7C,0x67,0x00,0x00,0xFC,0x04,0x00,0x46,0xFC,0x04,0x05,0x06,0xFC,0x04,0x00,0x33,0xFC,0x04,0x00,0x49,0x09,0x27,0x00,0x00,0xA4,0x7B,0x20,0x00,0xA0,0x00,0x69,0x00,0xFC,0x04,0x02,0x24,
	32 ,0x01,0xC2,0x91,0xFC,0x04,0x00,0x4D,0x0B,0x27,0x00,0x00,0xA4,0x7B,0x40,0x00,0x00,0xE7,0x00,0x00,0xA4,0x7B,0x68,0x00,0xA4,0x7B,0x84,0x01,0xFC,0x04,0x00,0x33,0x4C,0x67,0x00,0x00,0xFC,0x04,0x00,0x75,0xC0,0x00,0x24,0x08,0xC0,0x00,0xD0,0x20,0x7C,0x67,0x00,0x00,0xFC,0x04,0x00,0x46,0xFC,0x04,0x05,0x06,0xFC,0x04,0x00,0x33,0xFC,0x04,0x00,0x49,0x09,0x67,0x00,0x00,0xA4,0x7B,0xA0,0x00,0xA0,0x00,0x69,0x00,0xFC,0x04,0x02,0x24,0xFC,0x04,0x00,0x4D,0x0B,0x67,0x00,0x00,0xA0,0x7D,0x80,0x00,0xFB,0x80,0x00,0x81,0xFC,0x04,0x00,0x25,0x44,0x27,0x00,0x00,0xE0,0x77,0xC0,0x00,0x01,0xE7,0x00,0x00,0xA2,0x77,0x00,0x25,0xFC,0x04,0x04,0xD6,0xFB,0x85,0x66,0x0D,0xFF,0xF7,0x00,0x00,
	32 ,0x01,0xC2,0xB1,0xA4,0x7C,0xA0,0x00,0xA4,0x7C,0x80,0x00,0xFC,0x04,0x08,0x80,0x02,0x67,0x0C,0x06,0xFC,0x04,0x04,0x82,0x09,0xE7,0x00,0x25,0xFC,0x04,0x04,0xD6,0xFB,0x85,0x78,0x0B,0xFF,0xF7,0x00,0x00,0xA4,0x7C,0xA0,0x00,0xA4,0x7C,0x80,0x00,0xFC,0x04,0x08,0x80,0xA4,0x7C,0xCC,0x06,0x02,0x27,0x0C,0xA6,0x0A,0x67,0x08,0x24,0x01,0xE7,0x04,0x06,0xFC,0x04,0x04,0x82,0x0A,0x27,0x00,0x33,0xA4,0x7C,0xF4,0x46,0x22,0x67,0x09,0x80,0x02,0xA7,0x04,0x96,0x22,0xE7,0x18,0x24,0xFC,0x04,0x14,0x07,0xFC,0x04,0x1D,0x27,0x2A,0xA7,0x08,0xA4,0xA4,0x7D,0x04,0x16,0xFC,0x04,0x04,0x82,0x0A,0xE7,0x00,0x00,0x02,0xA7,0x00,0x00,0xFC,0x04,0x08,0x00,0xC0,0x00,0x30,0x20,0xFC,0x04,0x04,0xC2,
	32 ,0x01,0xC2,0xD1,0xFB,0x85,0xAA,0x0F,0xFF,0xF7,0x00,0x00,0x03,0xA7,0x00,0x00,0xFC,0x04,0x10,0xFF,0x2B,0x27,0x04,0x07,0x0B,0xA7,0x00,0x00,0xFB,0x80,0x00,0x81,0xA0,0x7D,0xA0,0x00,0x80,0x76,0xE0,0x00,0xE0,0x78,0xB0,0x1F,0x02,0x27,0x00,0x00,0xFC,0x04,0x00,0x55,0x0A,0x27,0x00,0x00,0xC0,0x00,0x04,0xC8,0xFB,0x85,0xCA,0x0E,0x01,0xE7,0x00,0x00,0x29,0xE7,0x00,0x25,0xA4,0x7D,0x20,0x00,0x09,0xA7,0x00,0x21,0x0A,0x27,0x00,0x00,0xCF,0xFF,0xE0,0x20,0x01,0xA7,0x02,0x43,0xC0,0x00,0x04,0xC4,0xFB,0x85,0xD6,0x0A,0x90,0x78,0x20,0x00,0x90,0x78,0x40,0x00,0xA0,0x7D,0xC0,0x00,0xFB,0x10,0x20,0x25,0xC0,0x5E,0x10,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x80,0x00,0x81,
	32 ,0x01,0xC2,0xF1,0xA0,0x7D,0x40,0x00,0x80,0x76,0xE0,0x00,0xDE,0x02,0x40,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0xA0,0x00,0x00,0xA8,0xA0,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x85,0xEE,0x06,0xAA,0xC0,0x00,0x00,0xFC,0x04,0x02,0x46,0x6C,0x67,0x00,0x1B,0x82,0x77,0x00,0x00,0xE2,0x77,0xC0,0x00,0x94,0xC0,0x00,0x00,0x4E,0x2C,0x40,0x00,0xBC,0xE0,0x00,0x00,0xDE,0x09,0xE0,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x0C,0x06,0x4E,0x2C,0x40,0x00,0xBC,0xE0,0x00,0x00,0xDE,0x09,0xC0,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,
	32 ,0x01,0xC3,0x11,0xFB,0x86,0x1E,0x06,0x4E,0x2C,0x40,0x00,0xBC,0xE0,0x00,0x00,0xDE,0x0A,0x40,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x30,0x06,0x4E,0x2C,0x40,0x00,0xBC,0xE0,0x00,0x00,0xDE,0x09,0x80,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x42,0x06,0x4E,0x2C,0x40,0x00,0xBC,0xE0,0x00,0x00,0xDE,0x09,0xA0,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x54,0x06,0x4E,0x2C,0x40,0x00,0xBC,0xE0,0x00,0x00,0xDE,0x09,0x40,0x20,0xFC,0x04,0x02,0x43,
	32 ,0x01,0xC3,0x31,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x66,0x06,0x4E,0x2C,0x40,0x00,0xBC,0xE0,0x00,0x00,0xDE,0x09,0x00,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x78,0x06,0xFB,0x80,0x00,0x80,0xDE,0x00,0x00,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0xA2,0x77,0xA0,0x00,0x86,0xC0,0x00,0x00,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x8C,0x06,0xFB,0x80,0x00,0x80,0xDE,0x00,0x00,0x20,0xFC,0x04,0x02,0x43,0xFC,0x04,0x03,0x43,0x94,0xA0,0x00,0x00,0xA8,0xA0,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0x9C,0x06,
	32 ,0x01,0xC3,0x51,0xAA,0xC0,0x00,0x00,0x82,0x77,0xA0,0x00,0xFF,0xA8,0x02,0xE3,0x96,0xC0,0x00,0x00,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0xAC,0x06,0xFB,0x80,0x00,0x80,0xA0,0x67,0x00,0x00,0xC0,0x00,0x24,0xC4,0xFB,0x87,0x2E,0x06,0xA0,0x59,0xE0,0x00,0xFC,0x04,0x00,0x65,0xFB,0x87,0x2E,0x06,0xAC,0x78,0x60,0x00,0xFB,0x10,0x20,0x21,0xC0,0x6C,0xB0,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xE0,0x67,0x20,0x00,0x48,0x27,0x00,0x00,0xC0,0x00,0x64,0xC4,0xFB,0x87,0x06,0x07,0xFF,0xF7,0x00,0x00,0xE4,0x77,0x20,0x00,0xC0,0x00,0x80,0x20,0x44,0x00,0xA0,0x00,0x4E,0x00,0xE0,0x21,0x08,0xE7,0x00,0x1B,0x94,0xC0,0x00,0x00,0xBC,0xE0,0x00,0x00,
	32 ,0x01,0xC3,0x71,0xC6,0x00,0x40,0x20,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0xE6,0x06,0xC6,0x00,0x60,0x20,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0xF0,0x06,0x4E,0x01,0x00,0x00,0xBC,0xC0,0x00,0x00,0xC6,0x00,0x20,0x20,0x94,0x80,0x00,0x00,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x86,0xFE,0x06,0xFB,0x87,0x2E,0x00,0xC0,0x00,0x84,0xC4,0xFB,0x87,0x2A,0x06,0x00,0xE7,0x00,0x00,0xFC,0x04,0x00,0x55,0x08,0xE7,0x00,0x63,0xFB,0x87,0x2E,0x06,0xC0,0x00,0x40,0x20,0x44,0x27,0x00,0x00,0xFB,0x10,0x20,0x00,0xFB,0xDE,0x86,0x40,0xAC,0x4D,0x40,0x00,0xBC,0xC0,0x00,0x00,0xC6,0x00,0x20,0x20,0x94,0x80,0x00,0x00,
	32 ,0x01,0xC3,0x91,0xA8,0x80,0x00,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x87,0x22,0x06,0xFB,0x87,0x2E,0x00,0xFB,0x10,0x20,0x00,0xFB,0xF1,0x4A,0x40,0xFB,0x80,0x00,0x80,0xA0,0x42,0xA0,0x00,0xC0,0x00,0x64,0xC4,0xFB,0x87,0x54,0x06,0xA0,0x48,0x00,0x00,0xFC,0x04,0x00,0x55,0x90,0x48,0x00,0x63,0xFB,0x87,0x58,0x06,0xA4,0x42,0xA0,0x00,0xFC,0x04,0x00,0x69,0x90,0x42,0xA0,0x00,0xA0,0x37,0xE0,0x00,0xA4,0x67,0xC0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x87,0x58,0x0C,0xA0,0x42,0x00,0x00,0xFF,0xA8,0x00,0x8D,0x90,0x42,0x00,0x00,0xFB,0x87,0x58,0x00,0xFB,0x10,0x20,0x00,0xFB,0xD9,0x76,0x40,0xFB,0x80,0x00,0x80,0xA0,0x42,0xA0,0x00,0xC0,0x00,0xA4,0xC4,0xFB,0x87,0x86,0x06,0xA2,0x48,0x00,0x00,
	32 ,0x01,0xC3,0xB1,0xFC,0x04,0x00,0x56,0x90,0x48,0x00,0x63,0xFB,0x87,0x8A,0x06,0xFB,0x10,0x20,0x6D,0x90,0x42,0xA0,0x00,0xA0,0x72,0x60,0x00,0xFC,0x04,0x00,0x25,0xC0,0x77,0x70,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xA0,0x1F,0x20,0x00,0xA4,0x74,0xE0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x87,0x8A,0x0C,0xA0,0x42,0x00,0x00,0xFF,0xA8,0x00,0x8D,0x90,0x42,0x00,0x00,0xFB,0x87,0x8A,0x00,0xFB,0x10,0x20,0x00,0xFB,0xDB,0x3A,0x40,0xFB,0x80,0x00,0x80,0xFB,0x10,0x20,0x00,0xFB,0xDE,0xD0,0x40,0xAC,0x01,0x40,0x00,0xFC,0x04,0x00,0x21,0xFF,0xA8,0x05,0x02,0xFB,0x87,0xAC,0x04,0xA0,0x4C,0x00,0x00,0xFF,0xA8,0x02,0x90,0x90,0x4C,0x00,0x00,0xA0,0x12,0x20,0x00,0xAC,0x44,0xC0,0x00,
	32 ,0x01,0xC3,0xD1,0xFC,0x04,0x04,0xC5,0xFB,0x87,0xAC,0x08,0xA0,0x4C,0x00,0x00,0xFF,0xA8,0x00,0x90,0x90,0x4C,0x00,0x00,0xFB,0x80,0x00,0x80,0xA0,0x42,0x60,0x00,0xC0,0x01,0x04,0xC4,0xFB,0x87,0xB8,0x06,0xFB,0x10,0x20,0x00,0xFB,0x86,0x80,0x40,0xFB,0x10,0x20,0x00,0xFB,0xE1,0x98,0x40,0xFB,0x80,0x00,0x80,0xA0,0x42,0x00,0x00,0xFF,0xA8,0x04,0x8E,0xFB,0x87,0xCE,0x06,0xA0,0x42,0x60,0x00,0xC0,0x00,0x44,0xC4,0xFB,0x87,0xCE,0x06,0xFB,0x10,0x20,0x00,0xFB,0x86,0x94,0x40,0xFB,0x10,0x20,0x00,0xFB,0xE3,0x68,0x40,0xFB,0x80,0x00,0x80,0xA0,0x42,0x60,0x00,0xC0,0x00,0x44,0xC4,0xFB,0x87,0xE0,0x06,0xFB,0x10,0x20,0x00,0xFB,0x86,0x94,0x40,0xFB,0x87,0xE8,0x00,0xC0,0x01,0xE4,0xC4,
	32 ,0x01,0xC3,0xF1,0xFB,0x87,0xE8,0x06,0xFB,0x10,0x20,0x00,0xFB,0x86,0x80,0x40,0xFB,0x10,0x20,0x00,0xFB,0xE5,0xA4,0x40,0xFB,0x80,0x00,0x80,0xA0,0x42,0x60,0x00,0xC0,0x00,0x44,0xC4,0xFB,0x87,0xFE,0x06,0xFB,0x10,0x20,0x00,0xFB,0x86,0x94,0x40,0xFB,0x10,0x20,0x00,0xFB,0xD9,0x04,0x40,0xFB,0x88,0x0A,0x00,0xC0,0x02,0x24,0xC4,0xFB,0x88,0x0A,0x06,0xFB,0x10,0x20,0x00,0xFB,0x86,0x80,0x40,0xFB,0x10,0x20,0x00,0xFB,0xD9,0x60,0x40,0xFB,0x10,0x20,0x00,0xFB,0xE7,0x9C,0x40,0xFB,0x80,0x00,0x80,0xA0,0x45,0x00,0x00,0xFC,0x04,0x00,0x65,0xFB,0x88,0x60,0x06,0xAC,0x42,0x40,0x00,0xBC,0x79,0xA0,0x00,0xC0,0x00,0x20,0x20,0x90,0x59,0xE0,0x6D,0x90,0x45,0x00,0x00,0xA0,0x4C,0x00,0x00,
	32 ,0x01,0xC4,0x11,0xFF,0xA8,0x02,0x91,0x90,0x4C,0x00,0x00,0xA0,0x42,0x00,0x00,0xFF,0xA8,0x00,0x86,0xA8,0x01,0x40,0x00,0xFF,0xA8,0x04,0x80,0xFB,0x88,0x4A,0x05,0xFF,0xF7,0x00,0x00,0x90,0x42,0x00,0x00,0xA0,0x22,0x40,0x00,0xFF,0xA8,0x00,0x80,0xFF,0xA8,0x01,0x03,0x90,0x22,0x40,0x00,0xA0,0x5C,0x40,0x00,0x80,0x49,0x40,0x00,0xC0,0x00,0x20,0x20,0x90,0x5C,0x40,0x00,0xFB,0x10,0x20,0x00,0xFB,0x86,0x94,0x40,0xFB,0x88,0x56,0x00,0xA8,0x01,0x40,0x00,0xFF,0xA8,0x04,0x82,0xFB,0x88,0x56,0x04,0xA0,0x22,0x40,0x00,0xFF,0xA8,0x00,0x80,0x90,0x22,0x40,0x00,0xA8,0x41,0xA0,0x00,0x80,0x42,0x40,0x00,0xFB,0x88,0x92,0x01,0xA4,0x49,0x00,0x00,0xBC,0x45,0x80,0x00,0xC0,0x02,0x04,0xC4,
	32 ,0x01,0xC4,0x31,0xFB,0x88,0x8E,0x06,0xA8,0x01,0x40,0x6D,0xFF,0xA8,0x04,0x80,0xFB,0x88,0x80,0x05,0xFF,0xF7,0x00,0x00,0x90,0x45,0x00,0x00,0xA0,0x22,0x40,0x00,0xFF,0xA8,0x02,0x80,0xFF,0xA8,0x03,0x03,0x90,0x22,0x40,0x00,0xA0,0x49,0x40,0x00,0x80,0x5C,0x40,0x00,0xFB,0x10,0x20,0x00,0xFB,0x86,0x80,0x40,0xFB,0x88,0x92,0x00,0xA8,0x01,0x40,0x00,0xFF,0xA8,0x04,0x82,0xFB,0x88,0x92,0x04,0xA0,0x22,0x40,0x00,0xFF,0xA8,0x02,0x80,0x90,0x22,0x40,0x00,0xFB,0x88,0x92,0x00,0xFB,0x10,0x20,0x00,0xFB,0xEB,0xC2,0x40,0xFB,0x80,0x00,0x80,0xE0,0x6E,0x00,0x00,0x48,0x27,0x00,0x00,0xC0,0x00,0x24,0xC4,0xFB,0x89,0x40,0x07,0xFF,0xF7,0x00,0x00,0xE4,0x75,0xA0,0x00,0xA0,0x59,0xE0,0x00,
	32 ,0x01,0xC4,0x51,0xFC,0x04,0x00,0x65,0xFB,0x89,0x6A,0x06,0x00,0x67,0x00,0x00,0xC0,0x04,0x04,0xC4,0xFB,0x88,0xCA,0x06,0xC0,0x03,0xE0,0x20,0xA8,0x7A,0x00,0x00,0x08,0x67,0x00,0x00,0x80,0x74,0x60,0x00,0xA8,0x01,0x40,0x00,0xFF,0xA8,0x04,0x81,0xFB,0x89,0x3E,0x04,0xA8,0x0A,0xC0,0x1B,0x80,0x74,0x80,0x00,0xA8,0x0A,0xE0,0x00,0x80,0x74,0xA0,0x00,0xAC,0x00,0xA0,0x00,0xBC,0x0A,0xC0,0x00,0x94,0x0A,0xE0,0x00,0xFB,0x89,0x6A,0x00,0xC0,0x03,0xE4,0xC4,0xFB,0x88,0xF2,0x06,0xA0,0x74,0x60,0x00,0xFC,0x04,0x00,0x55,0x90,0x74,0x60,0x00,0xC0,0x00,0x04,0xC8,0xFB,0x89,0x3E,0x0E,0xA8,0x01,0x40,0x00,0xC0,0x04,0x00,0x20,0xFF,0xA8,0x04,0x81,0xFB,0x88,0xEC,0x05,0xFF,0xF7,0x00,0x00,
	32 ,0x01,0xC4,0x71,0x08,0x67,0x00,0x00,0xA4,0x74,0x80,0x00,0xBC,0x0A,0xC0,0x00,0xA4,0x74,0xA0,0x00,0xBC,0x0A,0xE0,0x00,0xFB,0x10,0x20,0x00,0xFB,0xF3,0x22,0x40,0xFB,0x89,0x6A,0x00,0xC0,0x02,0x84,0xC4,0xFB,0x89,0x3A,0x06,0x00,0xA7,0x00,0x00,0xAC,0x7A,0x20,0x25,0xA4,0x66,0x88,0x00,0xFC,0x04,0x04,0xC1,0xFB,0x89,0x08,0x09,0xFF,0xF7,0x00,0x00,0x00,0xE7,0x00,0x00,0xFC,0x04,0x04,0xE6,0xFB,0x89,0x1A,0x0A,0xA8,0x75,0xE0,0x00,0xFB,0x10,0x20,0x25,0x09,0x67,0x00,0x00,0xA0,0x74,0x20,0x00,0xFC,0x04,0x00,0x25,0xC0,0x91,0x90,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x89,0x34,0x00,0xA8,0x75,0xC0,0x00,0xFB,0x10,0x20,0x25,0x09,0x67,0x00,0x00,0xA0,0x73,0xE0,0x00,
	32 ,0x01,0xC4,0x91,0xFC,0x04,0x00,0x25,0xC0,0x92,0xB0,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xA0,0x74,0x40,0x00,0xFB,0x10,0x20,0x25,0xC0,0x93,0x50,0x20,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xFB,0x89,0x6A,0x01,0xC0,0x04,0x00,0x20,0x90,0x6E,0x20,0x00,0xFB,0x10,0x20,0x00,0xFB,0xF3,0x22,0x40,0xFB,0x89,0x6A,0x00,0xC0,0x00,0x44,0xC4,0xFB,0x89,0x66,0x06,0xA8,0x01,0x40,0x00,0xFF,0xA8,0x04,0x81,0xFB,0x89,0x60,0x04,0xA8,0x0A,0xC0,0x00,0xAC,0x00,0xA0,0x00,0xFC,0x04,0x04,0xC5,0xFB,0x89,0x60,0x06,0xAC,0x0A,0xE0,0x00,0xFC,0x04,0x00,0x61,0xFB,0x89,0x60,0x06,0xA4,0x74,0x80,0x00,0xBC,0x0A,0xC0,0x00,0xA4,0x74,0xA0,0x00,0xBC,0x0A,0xE0,0x00,0xFB,0x10,0x20,0x00,
	32 ,0x01,0xC4,0xB1,0xFB,0xF3,0x22,0x40,0xFB,0x89,0x6A,0x00,0xFB,0x10,0x20,0x00,0xFB,0xF3,0x22,0x40,0xFB,0x80,0x00,0x80,0xA0,0x59,0xE0,0x00,0xFC,0x04,0x00,0x65,0xFB,0x89,0xB8,0x07,0xFF,0xF7,0x00,0x00,0xE0,0x66,0x80,0x00,0xA1,0x07,0x60,0x00,0xC0,0x00,0x04,0xC4,0xFB,0x89,0x82,0x0B,0xFF,0xF7,0x00,0x00,0xAE,0x00,0xA0,0x00,0x5C,0x27,0x00,0x00,0xA6,0x5B,0xA0,0x00,0x48,0x27,0x12,0x8D,0xAC,0x78,0x90,0xCA,0xFC,0x04,0x10,0x37,0xFC,0x04,0x09,0xA5,0xFC,0x04,0x0C,0x06,0xFC,0x04,0x04,0x82,0x44,0x27,0x00,0x00,0xA1,0x07,0x40,0x00,0xC0,0x00,0x04,0xC4,0xFB,0x89,0x9E,0x0B,0xFF,0xF7,0x00,0x00,0xAE,0x00,0xA0,0x00,0x5C,0x27,0x00,0x00,0x20,0x67,0x12,0x8D,0xA6,0x5B,0xA0,0x00,
	32 ,0x01,0xC4,0xD1,0xFC,0x04,0x10,0xCA,0xFC,0x04,0x10,0x37,0xFC,0x04,0x09,0xA6,0xAC,0x78,0xAC,0x86,0xFC,0x04,0x0D,0xA6,0x08,0x67,0x00,0x00,0x00,0xA7,0x00,0x00,0xFC,0x04,0x00,0x25,0xFC,0x04,0x0C,0x06,0xFC,0x04,0x04,0x82,0x08,0xA7,0x00,0x00,0xFB,0x80,0x00,0x80,0xFB,0x10,0x20,0x00,0xFB,0xBA,0xD4,0x40,0xFB,0x10,0x20,0x00,0xFB,0x80,0xDE,0x40,0xFB,0x80,0x00,0x80,0xC1,0xFB,0xA0,0x20,0x90,0x03,0x60,0x00,0xFB,0x10,0x20,0x00,0xFB,0xC6,0x8C,0x40,0xFB,0x80,0x00,0x80,0xC0,0x00,0xA0,0x20,0x90,0x03,0xE0,0x00,0xA6,0x02,0xC0,0x00,0xAA,0x7A,0x40,0x22,0xC0,0x02,0xC4,0x48,0xFC,0x04,0x05,0x76,0xFF,0xA8,0x05,0x00,0xFB,0x89,0xE8,0x05,0xFF,0xF7,0x00,0x00,0xFC,0x06,0x40,0x00,
	32 ,0x01,0xC4,0xF1,0xFB,0x89,0xEA,0x01,0x4E,0x25,0x00,0x00,0xFC,0x04,0x00,0x21,0xC0,0xA4,0xC0,0x20,0xC0,0x9F,0x30,0x20,0xFB,0x10,0x20,0x00,0xFA,0x48,0x00,0x00,0xFB,0x80,0x01,0x00,0xA0,0x03,0xE0,0x00,0xFC,0x04,0x00,0x55,0x90,0x03,0xE0,0x00,0xC0,0x00,0x04,0xC8,0xFB,0x89,0xD2,0x0A,0xFB,0x80,0x00,0x81,0xA0,0x02,0xC0,0x00,0x80,0x02,0xE0,0x00,0xFB,0x10,0x20,0x00,0xFB,0xAC,0x66,0x40,0xA2,0x30,0xC0,0x00,0xE4,0x34,0xD0,0x26,0xA2,0x0C,0x00,0x00,0xE0,0x30,0xC4,0xD7,0xFB,0x8A,0x16,0x0B,0xAE,0x34,0xC0,0x00,0xA0,0x30,0xE0,0x00,0x4E,0x10,0x20,0x00,0xE4,0x19,0x98,0xED,0xE2,0x17,0xCD,0xC7,0x4E,0x00,0x24,0x82,0x44,0x27,0x00,0x00,0xFB,0xAC,0xD6,0x01,0x0C,0x75,0x00,0x00,
	32 ,0x01,0xC5,0x11,0xA0,0x02,0x60,0x00,0xFB,0x80,0x00,0x80,0xFB,0x10,0x20,0x00,0xFB,0xAE,0xFA,0x40,0xE0,0x31,0x00,0x00,0x68,0x27,0x00,0x00,0xA2,0x0D,0x70,0x26,0xE4,0x35,0xA4,0xD7,0xFB,0x8A,0x3A,0x0B,0xE2,0x30,0x00,0x00,0x4E,0x35,0x00,0x00,0x4E,0x10,0x20,0x00,0x30,0x67,0x18,0xED,0x4E,0x10,0x4D,0xC7,0xFC,0x04,0x04,0x82,0x44,0x27,0x00,0x00,0x0C,0x67,0x00,0x00,0xFB,0xAF,0x54,0x01,0xA0,0x02,0x60,0x00,0xE0,0x1E,0x80,0x00,0xFB,0x80,0x00,0x80,0xFC,0x04,0x00,0x6A,0xC0,0x05,0xC4,0xC8,0xFB,0x8A,0x54,0x06,0xC0,0x02,0xC0,0x20,0xFB,0x80,0x00,0x81,0xFF,0xF7,0x00,0x00,0x90,0x02,0xC0,0x00,0xC1,0xFB,0xC0,0x20,0x90,0x03,0x60,0x00,0xFB,0x10,0x20,0x00,0xFB,0xC6,0x8C,0x40,
	32 ,0x01,0xC5,0x31,0xFB,0x10,0x20,0x00,0xFB,0xCA,0x6A,0x40,0xFB,0x80,0x00,0x80,0xFB,0x10,0x20,0x00,0xFB,0x8A,0x7A,0x40,0xC1,0xFB,0xE0,0x20,0x90,0x03,0x60,0x00,0xFB,0x10,0x20,0x00,0xFB,0xC6,0x8C,0x40,0xFB,0x80,0x00,0x81,0xC0,0x02,0x00,0x20,0x90,0x65,0xE0,0x00,0xC0,0xA5,0xA0,0x20,0x94,0x01,0x80,0x00,0xC0,0x9C,0x40,0x20,0x94,0x01,0xC0,0x00,0xFB,0x80,0x00,0x81,0xC0,0xA9,0xC0,0x20,0x94,0x02,0x40,0x00,0xA8,0x01,0x40,0x00,0xC4,0x68,0xA0,0x20,0xFF,0xA8,0x04,0x80,0xFB,0x8A,0x96,0x05,0xA2,0x02,0x20,0x00,0x82,0x02,0xA0,0x00,0xC0,0x9C,0xE0,0x20,0xFB,0x80,0x00,0x81,0xFF,0xF7,0x00,0x00,0x90,0x02,0x20,0x00,0xC1,0xFB,0x80,0x20,0x90,0x03,0x60,0x00,0xFB,0x10,0x20,0x00,
	5 ,0x01,0xC5,0x51,0xFB,0xC6,0x8C,0x40,0xFB,0x80,0x00,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	32 ,0x01,0xCF,0x0B,0xB0,0x00,0x00,0xB6,0xD0,0x00,0x00,0xCD,0xCE,0x00,0x00,0x01,0x01,0x81,0x03,0x00,0x1B,0x00,0x00,0x93,0x7C,0x26,0x66,0x66,0x01,0x01,0x00,0x00,0x36,0x00,0x00,0x00,0x35,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x01,0x80,0x03,0x00,0xB0,0x00,0x00,0xB6,0xD0,0x00,0x00,0xCD,0xCE,0x00,0x00,0x02,0x01,0x81,0x03,0x00,0x35,0x20,0x00,0x00,0x34,0x00,0x20,0xC5,0x33,0x00,0x7F,0xBE,0x32,0xFB,0x05,0x1F,0x31,0x00,0x20,0xC5,0x30,0x00,0x20,0xC5,0x03,0x01,0x01,0x00,0x5A,0x00,0x08,0x00,0x70,0x00,0x00,0x01,0x6F,0x20,0x00,0x00,0xE4,0x00,0x04,0xDD,0x02,0x01,0x00,0x00,0x36,0x00,0x00,0x00,0x35,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x01,0x80,0x03,0x00,0xCF,0x00,0x00,0x00,
	32 ,0x01,0xCF,0x2B,0x00,0x80,0x01,0x00,0xFE,0x04,0x00,0x00,0xFD,0x02,0x00,0x00,0xFC,0x00,0x00,0x00,0xFB,0xFF,0xE0,0x00,0xFA,0x01,0x40,0x00,0xF9,0x00,0x23,0xE9,0xF8,0x02,0xC6,0xC6,0xF7,0x7F,0xFF,0xFF,0xF6,0x20,0x00,0x00,0xF5,0x06,0xCF,0x42,0xF4,0xD9,0x89,0x37,0xF3,0x00,0x19,0x00,0xF2,0x39,0x99,0x9A,0xF1,0x0D,0x4B,0x01,0xF0,0x0D,0x4B,0x01,0xB0,0x00,0x00,0x5B,0xD0,0x00,0x00,0x01,0x97,0x51,0x75,0x48,0x96,0xFF,0xFD,0xAB,0x95,0x2A,0xA9,0x78,0x94,0x00,0x03,0x98,0x93,0x40,0x00,0x00,0x92,0x00,0x03,0x98,0x91,0x7F,0xC0,0x00,0xDE,0x39,0xBC,0xBF,0xDD,0x12,0x9C,0xE8,0xDC,0xF4,0xDE,0xDD,0xDB,0xF8,0xC3,0x41,0xDA,0x05,0x84,0xEC,0xD9,0x03,0xF1,0xA2,0xD8,0xFC,0xD5,0xBA,
	32 ,0x01,0xCF,0x4B,0xD7,0xFD,0xAC,0x5A,0xD6,0x02,0x52,0x2A,0xD5,0x02,0xDD,0x90,0xD4,0x00,0xBA,0xC4,0xD3,0xFF,0xCF,0x34,0xCE,0x00,0x00,0x01,0x12,0x81,0x03,0x00,0xCC,0x00,0x2A,0x45,0xFC,0x00,0x02,0x39,0xFD,0x00,0x00,0xA9,0xFB,0x00,0x01,0xE3,0x02,0x01,0x01,0x00,0x5D,0x01,0x00,0x00,0x58,0x20,0x00,0x00,0x29,0x00,0x05,0x13,0x25,0x00,0x05,0x01,0x02,0x01,0x00,0x00,0xAF,0x00,0x00,0x00,0xAD,0x00,0x00,0x00,0xAC,0x7F,0xFF,0xFF,0x36,0x00,0x00,0x00,0x35,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0xEE,0x00,0x02,0xF3,0xED,0x00,0x02,0xDA,0xEC,0x00,0x02,0xAB,0xEB,0x00,0x02,0x73,0xEA,0x00,0x02,0x42,0xE9,0x00,0x29,0x00,0xE8,0x73,0x33,0x33,0xE7,0x01,0x00,0x00,0xE6,0x02,0x8F,0x5C,
	32 ,0x01,0xCF,0x6B,0xE5,0x02,0x8F,0x5C,0xE4,0x7F,0xF0,0x00,0xE3,0x00,0x40,0x00,0xE2,0x01,0x00,0x00,0xE1,0x05,0x94,0x9D,0xE0,0x00,0x14,0x00,0xDF,0x00,0x14,0x00,0xDE,0x05,0x94,0x9D,0xDD,0x00,0x20,0x00,0xDC,0x10,0x00,0x00,0xDB,0xFF,0x46,0x2E,0xDA,0x2D,0x6E,0x53,0xD9,0x01,0x00,0x00,0xD8,0x10,0x00,0x00,0xD7,0xFF,0x46,0x2E,0xD6,0x2D,0x6E,0x53,0xD5,0x02,0xC6,0xC6,0xD4,0x00,0x07,0x2F,0xD3,0x00,0x51,0xEB,0xD2,0x00,0x1A,0x00,0xD1,0x00,0x12,0x00,0xD0,0x02,0x07,0xFE,0xCF,0x00,0x07,0x2F,0xCE,0x00,0x0E,0x5E,0xCD,0x00,0x00,0x00,0xCC,0x00,0x00,0x00,0xCB,0x00,0x00,0x00,0xC9,0x00,0x00,0x00,0xC8,0x00,0x00,0x00,0xC7,0x00,0x00,0x00,0xC6,0x00,0x00,0x00,0xC5,0x00,0x00,0x00,
	32 ,0x01,0xCF,0x8B,0xBA,0x00,0x00,0x00,0xB9,0x00,0x00,0x00,0xB7,0x00,0x02,0x42,0xB6,0x7F,0xFF,0xFF,0x19,0x80,0x03,0x00,0xC7,0x68,0xFF,0x6C,0xC1,0x00,0x11,0x0F,0xBA,0x08,0xB4,0x4D,0xD1,0x01,0x47,0xAE,0xB3,0x02,0x8F,0x5C,0xB1,0x00,0x00,0x00,0xAF,0x06,0xF5,0x40,0xC5,0x20,0x00,0x00,0x99,0x0A,0x00,0x00,0xEF,0x03,0x00,0x00,0xEE,0x00,0x01,0x00,0xED,0x10,0x00,0x00,0xEC,0x0C,0x00,0x00,0xEB,0x01,0xD0,0x00,0x9A,0x01,0x20,0x00,0x82,0x7D,0xF8,0xAA,0x7D,0x08,0x00,0x00,0x7F,0x00,0x50,0x00,0x85,0x09,0x37,0x4C,0x84,0x0C,0x97,0xD1,0x83,0x1C,0x44,0x56,0x82,0x1C,0x44,0x56,0x81,0x7D,0xF8,0xAA,0x7D,0x19,0x99,0x9A,0x68,0x06,0x01,0xF0,0x72,0x06,0x01,0xF0,0x71,0x7D,0xF8,0xAA,
	32 ,0x01,0xCF,0xAB,0x67,0x1C,0x44,0x56,0x02,0x33,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x20,0x00,0x32,0x10,0x00,0x00,0x31,0x00,0x06,0x00,0x30,0x04,0x00,0x00,0x2F,0x00,0x20,0x00,0x2E,0x03,0xB0,0x00,0x2D,0x03,0x00,0x00,0x2C,0x18,0x00,0x00,0xE2,0x00,0x00,0x01,0xE1,0x11,0x00,0x00,0xE0,0x01,0x00,0x00,0xDF,0x00,0x20,0x00,0xCF,0x00,0x11,0xC2,0xCA,0x00,0x00,0x00,0xD2,0x20,0x85,0x68,0x16,0x81,0x03,0x00,0xA2,0x00,0x00,0x01,0xA1,0x00,0x0F,0xFF,0xF0,0x03,0x50,0x00,0xCF,0x00,0x06,0x6A,0xC7,0x00,0x00,0x04,0xBB,0x33,0x00,0x00,0xBA,0x30,0x00,0x00,0xB9,0x00,0x00,0x00,0xB8,0x10,0x00,0x00,0x04,0x81,0x02,0x00,0xA7,0x94,0x00,0x00,0x3E,0x90,0x00,0x00,0x91,0x00,0x03,0xAD,
	21 ,0x01,0xCF,0xCB,0x8F,0x00,0x03,0x98,0x8C,0x00,0x03,0xC6,0x89,0x00,0x04,0x08,0x87,0x00,0x03,0xF7,0x86,0x00,0x03,0xD7,0x84,0x00,0x03,0xDF,0x82,0x00,0x03,0xEA,0x21,0x00,0x01,0x08,0x0C,0x00,0x01,0xD4,0x23,0x00,0x00,0xE0,0x06,0x80,0x03,0x00,0x9A,0x41,0x36,0x22,0x98,0x00,0x00,0x00,0xE1,0x00,0x01,0xD4,0x01,0x80,0x02,0x00,0x0A,0x00,0x05,0x44,0x00,0x80,0x00,0x00,0x00,0x03,0x0F,0x15,0x00,0x05,0x0F,0x2B,0x00,0x04,0x0F,0x8F,0x00,0x05,0x0F,0xDB,
	4 ,0x01,0xCF,0xFC,0x7A,0x00,0x00,0x00,0x79,0x01,0x32,0x0E,0x01,0x00,0x02,0x00,0x00,0x01,0x0F,0xFE,
	1 ,0x01,0x90,0x0A,0x00,0x00,0x00,0x00,
	1 ,0x01,0x90,0xFF,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB3,0xFF,0x00,0x00,0x00,0x00,
	1 ,0x01,0x40,0x05,0x00,0x03,0x3F,0x8C,
	2 ,0x01,0x40,0x01,0x00,0x02,0x7B,0x6E,0x00,0x02,0x7B,0xDE,
	1 ,0x01,0x40,0x04,0x00,0x41,0x07,0x80,
	1 ,0x01,0x10,0x00,0x00,0x00,0x05,0x01,
	1 ,0x00,0x10,0x00,0x00,0x00,0x00,0x00,
	1 ,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,
	1 ,0x00,0xF0,0x00,0x00,0x00,0x00,0x09,
	32 ,0x00,0x18,0x00,0x00,0x19,0x00,0x14,0x20,0x0F,0x00,0x09,0x00,0x19,0x00,0x15,0x22,0x0F,0x00,0x09,0x00,0x19,0x00,0x16,0x24,0x0F,0x00,0x09,0x00,0x19,0x00,0x17,0x26,0x0F,0x00,0x09,0x00,0x19,0x00,0x18,0x28,0x0F,0x00,0x09,0x00,0x19,0x00,0x64,0x30,0x0F,0x00,0x09,0x00,0x19,0x00,0x65,0x32,0x0F,0x00,0x09,0x00,0x19,0x00,0x66,0x34,0x0F,0x00,0x09,0x00,0x19,0x00,0x67,0x36,0x0F,0x00,0x09,0x00,0x19,0x00,0x68,0x38,0x0F,0x00,0x09,0x00,0x19,0x00,0x1E,0x20,0x0F,0x00,0x0A,0x00,0x19,0x00,0x1F,0x22,0x0F,0x00,0x0A,0x00,0x19,0x00,0x20,0x24,0x0F,0x00,0x0A,0x00,0x19,0x00,0x21,0x26,0x0F,0x00,0x0A,0x00,0x19,0x00,0x22,0x28,0x0F,0x00,0x0A,0x00,0x19,0x00,0x6E,0x30,0x0F,0x00,0x0A,
	32 ,0x00,0x18,0x20,0x00,0x19,0x00,0x6F,0x32,0x0F,0x00,0x0A,0x00,0x19,0x00,0x70,0x34,0x0F,0x00,0x0A,0x00,0x19,0x00,0x71,0x36,0x0F,0x00,0x0A,0x00,0x19,0x00,0x72,0x38,0x0F,0x00,0x0A,0x00,0x19,0x00,0x19,0x20,0x0F,0x00,0x08,0x00,0x19,0x00,0x1A,0x22,0x0F,0x00,0x08,0x00,0x19,0x00,0x1B,0x24,0x0F,0x00,0x08,0x00,0x19,0x00,0x1C,0x26,0x0F,0x00,0x08,0x00,0x19,0x00,0x1D,0x28,0x0F,0x00,0x08,0x00,0x19,0x00,0x69,0x30,0x0F,0x00,0x08,0x00,0x19,0x00,0x6A,0x32,0x0F,0x00,0x08,0x00,0x19,0x00,0x6B,0x34,0x0F,0x00,0x08,0x00,0x19,0x00,0x6C,0x36,0x0F,0x00,0x08,0x00,0x19,0x00,0x6D,0x38,0x0F,0x00,0x08,0x00,0x19,0x00,0x23,0x20,0x0F,0x00,0x0F,0x00,0x19,0x00,0x24,0x22,0x0F,0x00,0x0F,
	32 ,0x00,0x18,0x40,0x00,0x19,0x00,0x25,0x24,0x0F,0x00,0x0F,0x00,0x19,0x00,0x26,0x26,0x0F,0x00,0x0F,0x00,0x19,0x00,0x27,0x28,0x0F,0x00,0x0F,0x00,0x19,0x00,0x73,0x30,0x0F,0x00,0x0F,0x00,0x19,0x00,0x74,0x32,0x0F,0x00,0x0F,0x00,0x19,0x00,0x75,0x34,0x0F,0x00,0x0F,0x00,0x19,0x00,0x76,0x36,0x0F,0x00,0x0F,0x00,0x19,0x00,0x77,0x38,0x0F,0x00,0x0F,0x00,0x19,0x00,0x28,0x20,0x0F,0x00,0x0E,0x00,0x19,0x00,0x29,0x22,0x0F,0x00,0x0E,0x00,0x19,0x00,0x2A,0x24,0x0F,0x00,0x0E,0x00,0x19,0x00,0x2B,0x26,0x0F,0x00,0x0E,0x00,0x19,0x00,0x2C,0x28,0x0F,0x00,0x0E,0x00,0x19,0x00,0x78,0x30,0x0F,0x00,0x0E,0x00,0x19,0x00,0x79,0x32,0x0F,0x00,0x0E,0x00,0x19,0x00,0x7A,0x34,0x0F,0x00,0x0E,
	32 ,0x00,0x18,0x60,0x00,0x19,0x00,0x7B,0x36,0x0F,0x00,0x0E,0x00,0x19,0x00,0x7C,0x38,0x0F,0x00,0x0E,0x00,0x19,0x00,0x37,0x38,0x03,0x00,0x04,0x00,0x19,0x00,0x36,0x38,0x03,0x00,0x05,0x00,0x19,0x00,0x31,0x21,0x0E,0x06,0x06,0x00,0x19,0x00,0x81,0x31,0x0E,0x06,0x06,0x00,0x10,0x00,0x02,0x10,0x17,0x00,0x05,0x00,0x10,0x00,0x01,0x10,0x17,0x00,0x04,0x00,0x19,0x00,0x38,0x22,0x10,0x00,0x05,0x00,0x19,0x00,0x88,0x32,0x10,0x00,0x05,0x00,0x10,0x00,0x02,0x12,0x17,0x00,0x07,0x00,0x10,0x00,0x01,0x12,0x17,0x00,0x06,0x00,0x19,0x00,0x39,0x24,0x10,0x00,0x05,0x00,0x19,0x00,0x89,0x34,0x10,0x00,0x05,0x00,0x10,0x00,0x02,0x14,0x03,0x00,0x06,0x00,0x10,0x00,0x01,0x14,0x03,0x00,0x07,
	32 ,0x00,0x18,0x80,0x00,0x19,0x00,0x3A,0x25,0x10,0x00,0x05,0x00,0x10,0x00,0x02,0x15,0x03,0x00,0x04,0x00,0x10,0x00,0x01,0x15,0x03,0x00,0x05,0x00,0x19,0x00,0x3B,0x26,0x10,0x00,0x05,0x00,0x10,0x00,0x02,0x16,0x03,0x00,0x11,0x00,0x10,0x00,0x01,0x16,0x03,0x00,0x10,0x00,0x19,0x00,0x3C,0x27,0x10,0x00,0x07,0x00,0x10,0x00,0x02,0x27,0x19,0x00,0x59,0x00,0x10,0x00,0x01,0x27,0x19,0x00,0x58,0x00,0x19,0x00,0x3D,0x28,0x10,0x00,0x07,0x00,0x19,0x00,0x2D,0x21,0x0E,0x06,0x10,0x00,0x19,0x00,0x7D,0x31,0x0E,0x06,0x10,0x00,0x19,0x00,0x2E,0x21,0x0E,0x06,0x11,0x00,0x19,0x00,0x7E,0x31,0x0E,0x06,0x11,0x00,0x19,0x00,0x2F,0x21,0x0E,0x06,0x08,0x00,0x19,0x00,0x7F,0x31,0x0E,0x06,0x08,
	32 ,0x00,0x18,0xA0,0x00,0x19,0x00,0x30,0x21,0x0E,0x06,0x09,0x00,0x19,0x00,0x80,0x31,0x0E,0x06,0x09,0x00,0x19,0x00,0x32,0x21,0x0E,0x06,0x18,0x00,0x19,0x00,0x82,0x31,0x0E,0x06,0x18,0x00,0x19,0x00,0x33,0x21,0x0E,0x06,0x19,0x00,0x19,0x00,0x83,0x31,0x0E,0x06,0x19,0x00,0x0E,0x00,0x0C,0x23,0x19,0x00,0x46,0x00,0x0E,0x00,0x0C,0x33,0x19,0x00,0x96,0x00,0x0E,0x00,0x0D,0x23,0x19,0x00,0x47,0x00,0x0E,0x00,0x0D,0x33,0x19,0x00,0x97,0x00,0x16,0x00,0x04,0x25,0x19,0x00,0x48,0x00,0x15,0x00,0x01,0x25,0x19,0x08,0x48,0x00,0x16,0x00,0x04,0x35,0x19,0x00,0x98,0x00,0x15,0x00,0x01,0x35,0x19,0x08,0x98,0x00,0x16,0x00,0x05,0x25,0x19,0x00,0x49,0x00,0x15,0x00,0x02,0x25,0x19,0x08,0x49,
	32 ,0x00,0x18,0xC0,0x00,0x16,0x00,0x05,0x35,0x19,0x00,0x99,0x00,0x15,0x00,0x02,0x35,0x19,0x08,0x99,0x00,0x17,0x00,0x0A,0x30,0x19,0x00,0x4B,0x00,0x17,0x00,0x0B,0x30,0x19,0x00,0x4A,0x00,0x0E,0x00,0x28,0x23,0x19,0x00,0x4C,0x00,0x0E,0x00,0x28,0x33,0x19,0x00,0x9C,0x00,0x0E,0x00,0x29,0x23,0x19,0x00,0x4D,0x00,0x0E,0x00,0x29,0x33,0x19,0x00,0x9D,0x00,0x0E,0x00,0x2A,0x23,0x19,0x00,0x4E,0x00,0x0E,0x00,0x2A,0x33,0x19,0x00,0x9E,0x00,0x0E,0x00,0x2B,0x23,0x19,0x00,0x4F,0x00,0x0E,0x00,0x2B,0x33,0x19,0x00,0x9F,0x00,0x0F,0x08,0x06,0x41,0x19,0x00,0xB4,0x00,0x0E,0x04,0x00,0x17,0x19,0x00,0xB6,0x00,0x0E,0x04,0x01,0x17,0x19,0x00,0xB7,0x00,0x0E,0x04,0x02,0x17,0x19,0x00,0xB8,
	32 ,0x00,0x18,0xE0,0x00,0x0E,0x04,0x03,0x17,0x19,0x00,0xB9,0x00,0x0E,0x04,0x04,0x17,0x19,0x00,0xBA,0x00,0x0E,0x04,0x05,0x17,0x19,0x00,0xBB,0x00,0x0E,0x04,0x06,0x17,0x19,0x00,0xBC,0x00,0x0E,0x04,0x07,0x19,0x19,0x00,0xBD,0x00,0x0E,0x04,0x08,0x19,0x19,0x00,0xBE,0x00,0x0E,0x04,0x09,0x19,0x19,0x00,0xBF,0x00,0x19,0x08,0xFF,0x19,0x01,0x00,0x00,0x00,0x0E,0x04,0x0A,0x19,0x19,0x00,0xC0,0x00,0x0E,0x04,0x0B,0x19,0x19,0x00,0xC1,0x00,0x0E,0x04,0x0C,0x19,0x19,0x00,0xC2,0x00,0x0E,0x04,0x0D,0x19,0x19,0x00,0xC3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	1 ,0x00,0xF0,0x5B,0x00,0x11,0x00,0x00,
	11 ,0x00,0xF0,0x68,0x14,0x7B,0x00,0x00,0x7F,0xEF,0x00,0x00,0x04,0x19,0x00,0x00,0x7F,0xDE,0x00,0x00,0x00,0x0A,0x00,0x00,0xFF,0xF8,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x04,0x00,0x00,0x20,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x40,0x00,0x00,0x00,
	25 ,0x00,0xF0,0x74,0x20,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x01,0x60,0x00,0x00,0x01,0x70,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x20,0x00,0x00,0x7E,0xB7,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x20,0x00,0x00,0x02,0x50,0x00,0x00,0x10,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x0A,0x00,0x00,0x08,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0xFA,0x00,0x00,0x00,0x1C,0x00,0x00,0x2C,0x88,0x00,0x00,
	8 ,0x00,0xF0,0x8E,0x0F,0xA0,0x00,0x00,0x00,0x1C,0x00,0x00,0x07,0xD0,0x00,0x00,0x7F,0xFF,0x00,0x00,0x00,0x0A,0x00,0x00,0xFC,0x18,0x00,0x00,0x00,0x32,0x00,0x00,0x00,0x7D,0x00,0x00,
	18 ,0x00,0xF0,0x48,0x02,0x00,0x00,0x00,0x7D,0xF1,0x00,0x00,0x7F,0xFF,0x00,0x00,0x7E,0xB8,0x00,0x00,0x00,0x14,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x2F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x80,0x00,0x00,0x15,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x11,0x00,0x00,0x06,0x66,0x00,0x00,0x0C,0xCC,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,
	2 ,0x00,0xE0,0x08,0x00,0x00,0x00,0x00,0x7F,0xFF,0x00,0x00,
	2 ,0x00,0xE0,0x14,0x20,0x00,0x00,0x00,0x60,0x00,0x00,0x00,
	2 ,0x00,0xE0,0x21,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
	2 ,0x00,0xE5,0x00,0x00,0x19,0x02,0x00,0x00,0x00,0x00,0x9A,
	2 ,0x00,0xE3,0x0E,0x7F,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	2 ,0x00,0xE3,0x1E,0x7F,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	2 ,0x00,0xE3,0x4E,0x7F,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	1 ,0x01,0x70,0x00,0x00,0x00,0x00,0x03,
	1 ,0x01,0x70,0x02,0x80,0x00,0x00,0x00,
	1 ,0x00,0x30,0x01,0x00,0x00,0x01,0x00,
	1 ,0x01,0x00,0x00,0x00,0x00,0x00,0x01,
	1 ,0x01,0x00,0x04,0x00,0x19,0x00,0x00,
	1 ,0x00,0xE0,0x00,0x00,0x00,0x08,0x01,
	2 ,0x00,0xF0,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,
	1 ,0x00,0xF8,0x02,0x00,0x00,0x00,0x01,
	1 ,0x01,0x50,0x00,0x00,0x00,0x10,0x31,
	1 ,0x00,0x10,0x00,0x00,0x00,0x00,0x11,
	1 ,0x01,0x30,0x00,0x00,0x00,0x00,0x05,
	0xFF,
};

static const uint8 TDA7708_FMSWPatchData[]=
{
	0xFF
};


static const uint8 TDA7708_FMCustomWSPData[]=
{

	1 ,0x01,0xB3,0xC7,0x64,0x29,0xE2,0x5C,
	1 ,0x01,0xB1,0x54,0x16,0x00,0x00,0x00,
	2 ,0x01,0xB1,0xAB,0x80,0x00,0x00,0x00,0xB9,0x99,0x99,0x9A,
	1 ,0x01,0xA3,0xD7,0x05,0x1E,0xB8,0x00,
	1 ,0x01,0xA3,0xDD,0x00,0x14,0x00,0x00,
	2 ,0x01,0xA3,0xE8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB2,0x2D,0x00,0x00,0x04,0x00,
	1 ,0x01,0xB2,0xCA,0x00,0x00,0x01,0x00,
	1 ,0x01,0xB3,0x88,0x06,0x00,0x00,0x00,
	1 ,0x01,0xB2,0xD3,0x00,0x04,0xA0,0x00,
	1 ,0x01,0xB2,0xDD,0x06,0x00,0x00,0x00,
	1 ,0x01,0xB2,0xDF,0x00,0x02,0x00,0x00,
	3 ,0x01,0xB2,0xE7,0x22,0x00,0x00,0x00,0x01,0x50,0x00,0x00,0x0C,0xD0,0x00,0x00,
	1 ,0x01,0xB2,0xEB,0x0B,0x90,0x00,0x00,
	2 ,0x01,0xB2,0xEF,0x20,0x00,0x00,0x00,0x0F,0x60,0x00,0x00,
	1 ,0x01,0xB2,0xF8,0x09,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x01,0x04,0x00,0x00,0x00,
	3 ,0x01,0xB3,0x06,0x00,0x00,0x00,0x00,0x06,0x70,0x00,0x00,0x10,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x0C,0x08,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x0E,0x0D,0x00,0x00,0x00,
	2 ,0x01,0xB3,0x13,0x1A,0x00,0x00,0x00,0x1A,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x1E,0x08,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x27,0x16,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x26,0x12,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x28,0x00,0x60,0x00,0x00,
	1 ,0x01,0xB3,0x2C,0x44,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x32,0x1B,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x36,0x21,0x00,0x00,0x00,
	1 ,0x01,0xB3,0xED,0x22,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x99,0x01,0x00,0x00,0x00,
	1 ,0x01,0xB3,0xEC,0x13,0x00,0x00,0x00,
	1 ,0x01,0xB2,0xE9,0x2C,0xD0,0x00,0x00,
	1 ,0x01,0xB2,0xF0,0x30,0x00,0x00,0x00,
	1 ,0x01,0xB2,0xE0,0x0B,0x30,0x00,0x00,
	1 ,0x01,0xB2,0xD9,0x6F,0x00,0x00,0x00,
	1 ,0x01,0xB2,0xDF,0x00,0x07,0x00,0x00,
	1 ,0x01,0xB2,0xF8,0x0F,0x00,0x00,0x00,
	1 ,0x01,0xB2,0xF5,0x38,0x00,0x00,0x00,
	1 ,0x01,0xB3,0x1E,0x0A,0x00,0x00,0x00,
	1 ,0x01,0xB2,0xD7,0x00,0x60,0x00,0x00,
	1 ,0x01,0xB0,0x75,0x6D,0x44,0xAE,0xD4,
	1 ,0x01,0xB0,0x74,0x12,0xBB,0x51,0x2C,
	1 ,0x01,0xB0,0xE0,0x0D,0x1B,0x71,0x76,
	1 ,0x01,0xB0,0xD4,0x13,0x66,0x66,0x66,
	4 ,0x01,0xB0,0x70,0x06,0x84,0xCD,0xAC,0xF6,0xC8,0x4F,0x23,0x04,0xF4,0x70,0x4F,0x7B,0x0B,0x8F,0xB1,
	1 ,0x01,0xB0,0x9F,0xC0,0x53,0x85,0x4C,
	1 ,0x01,0xB0,0x9E,0x40,0x36,0x28,0xE8,
	1 ,0x01,0xB1,0xB5,0xF9,0x99,0x99,0x9A,
	1 ,0x01,0xB1,0xB4,0x50,0x00,0x00,0x00,
	1 ,0x01,0xB1,0xB3,0x00,0x01,0x4F,0x8B,
	1 ,0x01,0xB1,0xB2,0x0A,0x3D,0x70,0xA4,
	1 ,0x01,0xB0,0xE9,0x15,0x8C,0x3B,0xFF,
	1 ,0x01,0xB0,0xE6,0x76,0xFB,0x00,0x00,
	1 ,0x01,0xB1,0x6F,0x7F,0xFF,0xFF,0xFF,
	1 ,0x01,0xB0,0xB4,0x00,0x00,0xDC,0xC1,
	1 ,0x01,0xB1,0xA7,0x0A,0x3D,0x70,0xA4,
	1 ,0x01,0xB1,0xB0,0xF9,0x99,0x99,0x9A,
	1 ,0x01,0xB1,0xAF,0x50,0x00,0x00,0x00,
	2 ,0x01,0xB1,0xAD,0x11,0x11,0x11,0x11,0x00,0x0D,0x1B,0x71,
	1 ,0x01,0xB1,0x70,0x08,0xF5,0xC2,0x8F,
	1 ,0x01,0xB1,0x87,0x01,0x58,0xED,0x23,
	1 ,0x01,0xB0,0xCB,0x4C,0xCC,0xCC,0xCD,
	1 ,0x01,0xB1,0x45,0xEC,0xFB,0x8B,0xB0,
	1 ,0x01,0xB1,0x40,0xC9,0xC9,0xF5,0x77,
	1 ,0x01,0xB1,0x4A,0xF5,0x5A,0xCE,0xDA,
	2 ,0x01,0xB1,0x41,0xD0,0x1B,0xD9,0x14,0xD5,0xF6,0x75,0xD1,
	1 ,0x01,0xB1,0x49,0xF3,0xDF,0x4E,0xCF,
	1 ,0x01,0xB1,0x48,0xF2,0x45,0x9A,0x49,
	1 ,0x01,0xB1,0x4D,0x21,0xE2,0x74,0x45,
	1 ,0x01,0xB1,0x50,0x3C,0x0F,0x8F,0xC0,
	1 ,0x01,0xB1,0x3F,0xC3,0x12,0xBE,0x57,
	1 ,0x01,0xB1,0x51,0x43,0xFB,0x27,0xE3,
	1 ,0x01,0xB1,0x3D,0xB4,0xE6,0xE8,0x96,
	1 ,0x01,0xB1,0x47,0xF0,0x92,0x3C,0xFD,
	1 ,0x01,0xB1,0x3E,0xBC,0x10,0xB5,0xB4,
	1 ,0x01,0xB1,0x4F,0x33,0xA4,0xFB,0x54,
	1 ,0x01,0xB1,0x4E,0x2A,0xDC,0xA3,0x10,
	1 ,0x01,0xB1,0x52,0x4B,0x51,0x44,0xAB,
	1 ,0x01,0xB1,0x46,0xEE,0xCB,0xED,0x5C,
	1 ,0x01,0xB1,0xEC,0x80,0x5A,0x0D,0xB6,
	1 ,0x01,0xB1,0xEF,0x3F,0xA5,0xF2,0x4A,
	1 ,0x01,0xB1,0xED,0x3F,0xD2,0xF9,0x25,
	1 ,0x01,0xB1,0x3C,0xAD,0xBF,0xC1,0x54,
	1 ,0x01,0xB1,0x44,0xEB,0x2B,0xD5,0xAB,
	1 ,0x01,0xB1,0x4C,0x18,0xEB,0x96,0xFF,
	1 ,0x01,0xB0,0xCF,0x0C,0xF7,0xA0,0x9E,
	1 ,0x01,0xB0,0xCD,0x00,0x00,0x07,0x00,
	1 ,0x01,0xB0,0xCC,0x68,0x8A,0x00,0x00,
	1 ,0x01,0xB0,0xCE,0x7F,0xFF,0xFF,0xFF,
	1 ,0x01,0xB1,0xE1,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB1,0xE0,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB1,0xDF,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB1,0x81,0x73,0x33,0x33,0x33,
	1 ,0x01,0xB1,0x72,0x73,0x33,0x33,0x33,
	1 ,0x01,0xB0,0xD5,0xFF,0xE0,0xA4,0xA0,
	1 ,0x01,0xB0,0x5D,0x01,0x08,0x80,0x79,
	1 ,0x01,0xB0,0xAF,0x33,0x33,0x33,0x33,
	1 ,0x01,0xB0,0xAE,0x33,0x33,0x33,0x33,
	1 ,0x01,0xB1,0x69,0x0B,0xDB,0x5A,0x01,
	1 ,0x01,0xB0,0xD6,0x40,0x87,0xFE,0x39,
	1 ,0x01,0xB0,0xDC,0x01,0x58,0xED,0x23,
	1 ,0x01,0xB0,0x93,0x01,0x13,0xF0,0xE9,
	1 ,0x01,0xB0,0xE5,0x00,0x03,0x73,0x03,
	1 ,0x01,0xB1,0x94,0xBC,0xA1,0xAF,0x28,
	1 ,0x01,0xB0,0xC9,0x0E,0xD2,0x30,0x81,
	1 ,0x01,0xB1,0xEE,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB1,0xF0,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB1,0x53,0x52,0x05,0x25,0x00,
	1 ,0x01,0xB1,0x43,0xDB,0x4F,0x9E,0x00,
	1 ,0x01,0xB1,0x4B,0xF6,0xB5,0x86,0x00,
	1 ,0x01,0xB0,0x95,0x0A,0x00,0x00,0x00,
	1 ,0x01,0xB0,0x97,0x7F,0xFF,0xFF,0x00,
	1 ,0x01,0xB0,0x96,0x24,0x80,0x00,0x00,
	1 ,0x01,0xB0,0x94,0x02,0x00,0x00,0x00,
	1 ,0x01,0xB1,0x7E,0x28,0xF5,0xC2,0x00,
	1 ,0x01,0xB0,0xEA,0x53,0x33,0x33,0x00,
	2 ,0x01,0xB2,0xE7,0x30,0x00,0x00,0x00,0x0E,0xD0,0x00,0x00,
	1 ,0x01,0xB3,0xE1,0x00,0x00,0x04,0x00,
	2 ,0x01,0xB2,0xEF,0x30,0x00,0x00,0x00,0x10,0x60,0x00,0x00,
	1 ,0x01,0xB2,0xCF,0x00,0x02,0x80,0x00,
	0xFF,

};

static const uint8 TDA7708_AMSWPatchData[]=
{
	0xFF
};

static const uint8 TDA7708_AMCustomWSPData[]=
{

	1 ,0x01,0xB3,0xC7,0x40,0xBE,0xDC,0xCE,
	4 ,0x01,0xB0,0x6A,0x00,0x8D,0x8A,0x00,0x00,0x8D,0x8A,0x00,0x73,0x90,0x23,0x00,0x96,0xA9,0x92,0x00,
	1 ,0x01,0xB0,0x69,0x9C,0x0F,0x13,0x00,
	1 ,0x01,0xB0,0x61,0x68,0x00,0x00,0x00,
	1 ,0x01,0xB3,0xD0,0x00,0x02,0xCD,0x00,

	1 ,0x01,0xB0,0xB7,0x44,0x3D,0x1F,0x00,
	1 ,0x01,0xB0,0xB9,0xFF,0xFF,0xEB,0x00,
	2 ,0x01,0xB0,0xC3,0x24,0xF1,0x18,0x00,0x17,0x5D,0x31,0x00,
	1 ,0x01,0xB0,0xB8,0x00,0x00,0x11,0x00,
	1 ,0x01,0xB0,0xC5,0x03,0xB1,0xB5,0x00,
	0xFF,

};

static const uint8 TDA7708_WXSWPatchData[]=
{
	0xFF
};

static const uint8 TDA7708_WXCustomWSPData[]=
{

	1 ,0x01,0xB3,0xC7,0x7F,0xFF,0xFF,0xFF,
	1 ,0x01,0xB0,0x8B,0xFF,0xFC,0xF8,0x89,
	1 ,0x01,0xB0,0x8A,0x44,0x94,0x2B,0x7C,
	3 ,0x01,0xB0,0x90,0x13,0x35,0xDE,0xA9,0x2A,0xDE,0x57,0xDF,0x01,0xEB,0xC9,0x77,
	1 ,0x01,0xB0,0x8C,0x00,0x00,0x0A,0x00,
	1 ,0x01,0xB0,0x8F,0x0C,0xCC,0xCC,0xCD,
	2 ,0x01,0xB0,0x8D,0x00,0x01,0xA3,0x6E,0x00,0x08,0x31,0x27,
	1 ,0x01,0xB0,0x78,0x7F,0xFF,0xFF,0xFF,
	1 ,0x01,0xB0,0x76,0x03,0x7A,0xA1,0x5E,
	1 ,0x01,0xB0,0x86,0x00,0x00,0x03,0x00,
	1 ,0x01,0xB0,0x89,0x47,0xED,0x3D,0xC2,
	1 ,0x01,0xB0,0x85,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB0,0x84,0x00,0x00,0x00,0x00,
	1 ,0x01,0xB0,0x78,0x7F,0xFF,0xFF,0x00,
	0xFF,

};

const uint8 TDA7708_FMFEReg[]=
{

	0x03,0x02,0x00,
	0x03,0x50,0x10,
	0xF8,0x3F,0x99,
	0xB0,0xE6,0x16,
	0x16,0x76,0x04,
	0x8E,0x10,0x00,
	0x18,0x0E,0xFB,
	0x08,0xB2,0x05,
	0x00,0x00,0x00,
	
};

const uint8 TDA7708_WXFEReg[]=
{

	0x03,0x02,0x00,
	0x03,0x30,0x20,
	0xF8,0x3F,0xD9,
	0xB0,0xE6,0x16,
	0x16,0x76,0x04,
	0x8E,0x10,0x00,
	0x18,0x0E,0xFB,
	0x08,0xB2,0x05,
	0x00,0x00,0x00,

};

const uint8 TDA7708_AMFEReg[]=
{

	0x26,0x03,0x9E,
	0x01,0x42,0x17,
	0xFC,0x00,0x08,
	0xB7,0x00,0x35,
	0xB0,0x8E,0x04,
	0x8E,0x10,0x00,
	0x38,0x3E,0xFB,
	0x08,0xB2,0x03,
	0x00,0x00,0x00,

};

static const uint8 CMOST_Firmware[1857] = 
{
  /* 0x14001 - 0x14002: 8 bytes, 2 words */
  0x01,0x40,0x01,
  0x00,0x08,
  0xFE,0x7F,0xDC,0xFF, 0xFE,0x7F,0xDC,0xFF, 
  /* 0x14010 - 0x14012: 12 bytes, 3 words */
  0x01,0x40,0x10,
  0x00,0x0C,
  0x00,0xFF,0x00,0xFF, 0x00,0x00,0x00,0x00, 0x00,0xFE,0x00,0xFE, 
  /* 0x21E80: 4 bytes, 1 word */
  0x02,0x1E,0x80,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x21FA0: 4 bytes, 1 word */
  0x02,0x1F,0xA0,
  0x00,0x04,
  0x00,0x00,0x01,0x00, 
  /* 0x20200: 4 bytes, 1 word */
  0x02,0x02,0x00,
  0x00,0x04,
  0x00,0x00,0x00,0x01, 
  /* 0x23000 - 0x23108: 1060 bytes, 265 words */
  0x02,0x30,0x00,
  0x04,0x24,
  0xFB,0x80,0x10,0x00, 0xFB,0xC8,0xDA,0x00, 0xFB,0xC8,0xEC,0x00, 0xFB,0xC8,0xF6,0x00, 0xFB,0xC9,0x00,0x00, 0xFB,0xC9,0x10,0x00, 0xFB,0xC9,0x24,0x00, 0xFF,0xF7,0x00,0x00, 
  0xE1,0xFF,0xE0,0x1B, 0x90,0x00,0x70,0x53, 0x92,0x00,0x50,0x6B, 0x92,0x00,0x90,0x1F, 0xFB,0x27,0x00,0x00, 0xFF,0xE6,0x02,0x98, 0xFB,0xC6,0x4C,0x00, 0xFB,0x9E,0xA6,0x00, 
  0xFB,0x80,0x52,0x00, 0xFB,0x9E,0xDA,0x00, 0xFB,0x80,0x82,0x00, 0xFB,0x9E,0xFE,0x00, 0xFB,0x80,0x36,0x00, 0xFB,0x9F,0xA4,0x00, 0xFB,0x81,0x38,0x00, 0xFB,0x9F,0xA8,0x00, 
  0xFB,0x81,0x4E,0x00, 0xFF,0xF7,0x00,0x00, 0xFB,0x80,0x00,0x80, 0xA2,0x01,0xC0,0x00, 0xC0,0x00,0x44,0xD4, 0xFB,0x80,0x50,0x06, 0xA2,0xC3,0x20,0x00, 0xFF,0xA8,0x04,0xCE, 
  0xFB,0x80,0x48,0x05, 0xA1,0xCD,0x80,0x00, 0xA6,0x1E,0xC0,0x00, 0xAF,0x7E,0xE0,0x00, 0xF0,0x02,0x08,0x40, 0xFB,0xD0,0x28,0x01, 0x21,0x6D,0x12,0x2C, 0xFC,0x06,0x14,0x97, 
  0xFB,0x80,0x00,0x80, 0xA8,0x5F,0x40,0x00, 0xA9,0x48,0xE0,0xF5, 0xA8,0x5F,0x24,0x26, 0xA9,0x48,0xD0,0xF5, 0xA9,0x0B,0x94,0x27, 0xFB,0x10,0x20,0x00, 0xFB,0x9F,0xDA,0x40, 
  0xA8,0x5F,0x80,0x00, 0xA9,0x49,0x20,0xF5, 0xA8,0x5F,0x64,0x26, 0xFB,0x9F,0xDA,0x01, 0xA9,0x49,0x10,0xF5, 0xA9,0x0B,0xB4,0x27, 0xFB,0x80,0x00,0x80, 0xA2,0xBA,0x80,0x55, 
  0xC0,0x00,0xA4,0xD4, 0xFB,0x80,0x7A,0x07, 0xAD,0x4A,0xE0,0x00, 0xA0,0xBB,0x00,0x00, 0xA2,0xBF,0x80,0x00, 0xFB,0xDF,0x12,0x01, 0x82,0xBF,0x80,0x00, 0x82,0xBA,0x80,0x00, 
  0xFB,0x80,0x00,0x80, 0xFB,0x10,0x20,0x00, 0xFB,0xC6,0xEC,0x40, 0xA0,0x00,0xC0,0x00, 0xC0,0x00,0x64,0xC4, 0xFB,0x80,0x90,0x04, 0xC0,0x00,0xC4,0xC4, 0xFB,0x80,0x88,0x06, 
  0xC1,0xE7,0xA0,0x20, 0x90,0x03,0x60,0x00, 0xFB,0x10,0x20,0x00, 0xFB,0x9E,0xBC,0x40, 0xC1,0xE5,0xA0,0x20, 0x90,0x03,0x60,0x00, 0xFB,0x10,0x20,0x00, 0xFB,0x9E,0xBC,0x40, 
  0xC3,0x53,0xC0,0x20, 0x94,0xA3,0xA0,0x00, 0xC0,0x03,0xE0,0x20, 0x90,0x0D,0x20,0x00, 0xC0,0x1D,0x20,0x20, 0x95,0x78,0x40,0x00, 0xC0,0x1C,0xA0,0x20, 0x94,0x59,0xE0,0x00, 
  0xC3,0xC7,0xC0,0x20, 0x94,0x8B,0xA0,0x00, 0xC0,0x1B,0xE0,0x20, 0x94,0x91,0xE0,0x00, 0xC0,0x16,0xA0,0x20, 0x94,0x80,0x00,0x00, 0xC0,0x06,0xE0,0x20, 0x95,0x58,0xE0,0x00, 
  0xC3,0xC0,0x60,0x20, 0xFB,0x10,0x22,0x23, 0x4C,0x67,0x00,0x1B, 0xFB,0x9F,0xD2,0x40, 0xFB,0x80,0x00,0x80, 0xA2,0xC6,0x00,0x00, 0xC0,0x00,0xF4,0x34, 0xC0,0x00,0x24,0xCC, 
  0xFB,0x81,0x32,0x06, 0xA2,0xC6,0xC0,0x00, 0xFC,0x04,0x00,0x56, 0x88,0xC6,0xC0,0x66, 0xFB,0x81,0x32,0x06, 0xA0,0xC4,0xE0,0x00, 0xAD,0x7D,0x00,0x00, 0xAD,0x7C,0xE8,0x00, 
  0xFC,0x04,0x02,0xF4, 0xE5,0xBB,0x20,0x13, 0xA4,0x18,0x07,0xC9, 0xFC,0x04,0x02,0x14, 0xAF,0xBA,0xF4,0x46, 0xBA,0xC6,0xD0,0x3F, 0x4E,0x3E,0x22,0xF5, 0xFC,0x04,0x00,0x13, 
  0xFC,0x04,0x02,0x73, 0xA0,0x17,0x74,0x46, 0xAC,0x00,0xA4,0x05, 0x6C,0x67,0x10,0x3F, 0x5C,0x65,0x50,0x21, 0xF8,0x04,0x18,0x03, 0x4E,0x25,0x48,0x00, 0xFC,0x04,0x02,0x44, 
  0x4E,0x25,0x44,0x09, 0xFC,0x04,0x04,0x96, 0xFC,0x04,0x00,0x3B, 0xFC,0x04,0x10,0x43, 0x5C,0x3E,0x20,0x00, 0xF8,0x04,0x58,0x03, 0x4E,0x25,0x48,0x00, 0xFC,0x04,0x02,0x44, 
  0x4E,0x25,0x44,0x09, 0xFC,0x04,0x04,0x96, 0xFC,0x04,0x00,0x3B, 0xFC,0x04,0x10,0x43, 0xA0,0xC3,0x20,0x00, 0xFF,0xA8,0x00,0x8D, 0xFF,0xA8,0x03,0x0E, 0xFC,0x04,0x04,0xDF, 
  0xFB,0x81,0x26,0x0A, 0xFF,0xA8,0x03,0x0D, 0xFF,0xA8,0x01,0x0E, 0xFC,0x04,0x04,0xE6, 0xFB,0x81,0x32,0x04, 0x90,0xC3,0x20,0x00, 0xA9,0x74,0x80,0x00, 0xFB,0x10,0x20,0x00, 
  0xFB,0x9F,0xD2,0x40, 0xC4,0xAA,0xC0,0x20, 0xFB,0x80,0x01,0x00, 0xFB,0x80,0x00,0x80, 0x01,0x88,0x40,0x00, 0x20,0x76,0x00,0x00, 0xC0,0x02,0x00,0x20, 0x01,0x67,0x00,0x45, 
  0x20,0xEE,0x14,0x25, 0xFF,0xA8,0x04,0xCA, 0xFB,0x81,0x4A,0x04, 0xFC,0x04,0x00,0x47, 0xC0,0x03,0x84,0x28, 0xFB,0xEB,0x3E,0x00, 0xFB,0x80,0x00,0x80, 0x00,0xE7,0x00,0x00, 
  0x11,0x67,0x10,0x25, 0xFF,0xA8,0x04,0x91, 0xFB,0x81,0x58,0x04, 0xFF,0xA8,0x01,0xF6, 0x28,0xE7,0x00,0x00, 0xFB,0x10,0x20,0x00, 0xFB,0xEC,0x9A,0x40, 0xFB,0x80,0x00,0x80, 
  0xFF,0xB0,0x13,0x01, 0xFC,0x04,0x00,0x1B, 0xCB,0x88,0x10,0x20, 0xFB,0x80,0x01,0x40, 0xFB,0x80,0x00,0x80, 0xA0,0x0C,0xE0,0x00, 0xCB,0x87,0xE0,0x20, 0xC0,0x00,0x64,0xC4, 
  0xFB,0x81,0xB8,0x06, 0x22,0x67,0x00,0x00, 0xFF,0xB0,0x13,0x01, 0xFC,0x04,0x00,0x26, 0x0A,0x67,0x00,0x00, 0xC0,0x02,0x05,0x74, 0xC0,0x00,0x24,0xC8, 0xFB,0x81,0x98,0x06, 
  0xC0,0x8B,0x00,0x20, 0x08,0xA7,0x00,0x00, 0xC0,0x00,0x60,0x20, 0x08,0xE7,0x00,0x00, 0xA0,0xC4,0xE0,0x00, 0xFC,0x24,0x03,0x95, 0xF2,0x02,0xE0,0x00, 0x18,0x67,0x00,0x00, 
  0xC0,0x00,0x40,0x20, 0xFB,0x81,0xB2,0x01, 0x18,0xE7,0x00,0x00, 0xC0,0x16,0x00,0x20, 0xC0,0x97,0x80,0x20, 0x08,0xA7,0x00,0x00, 0xC0,0x01,0x00,0x20, 0x08,0xE7,0x00,0x00, 
  0xFC,0x24,0x00,0x00, 0xF2,0x02,0xE0,0x00, 0xA0,0xC4,0xE0,0x00, 0xFC,0x04,0x03,0x95, 0xA0,0xC5,0x40,0x00, 0x54,0x27,0x10,0x25, 0x38,0x67,0x10,0x1F, 0x38,0xA7,0x00,0x00, 
  0xC0,0x16,0x00,0x20, 0xFB,0xC2,0x78,0x01, 0x90,0x83,0x20,0x00, 0xAC,0x00,0x80,0x00, 0xFB,0x80,0x00,0x81, 0xFF,0xF7,0x00,0x00, 0x90,0x83,0x20,0x00, 0x00,0x67,0x00,0x00, 
  0xFF,0xA8,0x00,0x80, 0x08,0x67,0x00,0x00, 0xC7,0x79,0x60,0x20, 0xFB,0x80,0x01,0x00, 0xFB,0x80,0x00,0x80, 0xFB,0xFA,0x86,0x01, 0xAC,0x00,0xA0,0x00, 0xBC,0x5C,0xC0,0x00, 
  0xFB,0x80,0x00,0x80, 0xC0,0x1D,0xB0,0x20, 0xFB,0xFB,0x0A,0x01, 0xFB,0x10,0x20,0x00, 0xFA,0x48,0x00,0x00, 0xAC,0x00,0xC0,0x00, 0xFF,0xA8,0x05,0x0E, 0xFB,0x81,0xEA,0x05, 
  0xB8,0xC6,0xC0,0x00, 0xA9,0x74,0x80,0x00, 0xFB,0x9F,0xD2,0x01, 0xFF,0xA8,0x03,0x0D, 0x90,0xC3,0x20,0x00, 0xFF,0xA8,0x01,0x0D, 0x90,0xC3,0x20,0x00, 0xFB,0x10,0x20,0x00, 
  0xFB,0x9F,0xD2,0x40, 0x21,0x67,0x00,0x00, 0x03,0xA7,0x12,0xF6, 0xC0,0x00,0xE4,0x24, 0x4E,0x36,0x22,0x96, 0xE7,0x7D,0x30,0x17, 0x6C,0x7D,0xA2,0x67, 0xFC,0x04,0x14,0x59, 
  0xFC,0x04,0x04,0x19, 0xFC,0x04,0x04,0xD7, 0xFB,0x82,0x0A,0x08, 0xFC,0x04,0x04,0xF6, 0xFB,0x82,0x0E,0x0A, 0xAD,0xBB,0x00,0x00, 0xB8,0xC6,0xC0,0x00, 0xFB,0x80,0x00,0x80, 
  0x00,0x00,0x00,0x00, 
  /* 0x23F08 - 0x23F41: 232 bytes, 58 words */
  0x02,0x3F,0x08,
  0x00,0xE8,
  0x3C,0x0E,0x10,0x00, 0x00,0x81,0x06,0x00, 0xE5,0x06,0x55,0x18, 0xE4,0x06,0x54,0x50, 0xE3,0x06,0x15,0xD0, 0xE2,0x05,0xFA,0x78, 0xE1,0x05,0x89,0xF8, 0xE0,0x05,0x89,0x30, 
  0xDF,0x05,0x1E,0xF0, 0xDE,0x04,0xBB,0xB8, 0x04,0x01,0x02,0x00, 0xE8,0x1F,0x8E,0x4C, 0xE7,0xE3,0xE3,0x76, 0xE6,0x14,0xFA,0x1E, 0xE5,0x12,0xDC,0xF7, 0xE4,0x0F,0xC2,0xB1, 
  0xE3,0x1C,0x10,0xB4, 0xE2,0x1F,0x8E,0x4C, 0xE1,0xC7,0xC6,0xF2, 0xE0,0x1F,0x90,0xB8, 0xDF,0xE0,0x5A,0x59, 0xDE,0x15,0x44,0x24, 0xDD,0x13,0x61,0x39, 0xDC,0x0F,0xC4,0x54, 
  0xDB,0x1F,0x86,0x8A, 0xDA,0x1F,0x90,0xB8, 0xD9,0xC0,0xB4,0xBA, 0xD8,0x00,0x00,0x0F, 0xD7,0x00,0x02,0xC8, 0x09,0x01,0x0D,0x00, 0x47,0xB0,0x00,0x0B, 0x46,0xBB,0xB0,0x00, 
  0x01,0x01,0x0A,0x00, 0x8D,0xFC,0x04,0x00, 0x00,0x81,0x01,0x00, 0xF9,0x00,0x00,0x65, 0x00,0x80,0x03,0x00, 0x00,0x01,0x0F,0x09, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
  0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
  0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x05,0x0F,0x2C, 0x02,0x00,0x00,0x00, 0x01,0x02,0x06,0x00, 
  0x01,0x01,0x00,0x00, 0x00,0x01,0x0F,0x40, 
  /* 0x29FA0: 4 bytes, 1 word */
  0x02,0x9F,0xA0,
  0x00,0x04,
  0x00,0x00,0x01,0x00, 
  /* 0x28200: 4 bytes, 1 word */
  0x02,0x82,0x00,
  0x00,0x04,
  0x00,0x00,0x00,0x01, 
  /* 0x2B000 - 0x2B01D: 120 bytes, 30 words */
  0x02,0xB0,0x00,
  0x00,0x78,
  0xFB,0x80,0x10,0x00, 0xFB,0xA1,0xEA,0x00, 0xFB,0xA2,0x0A,0x00, 0xFB,0xA2,0x16,0x00, 0xFB,0xA2,0x20,0x00, 0xFB,0xA2,0x30,0x00, 0xFB,0xA2,0x28,0x00, 0xFB,0xA1,0xFA,0x00, 
  0xE1,0xFF,0xE0,0x1B, 0x90,0x00,0x70,0x53, 0x92,0x00,0x50,0x6B, 0x92,0x00,0x90,0x1F, 0xFB,0x27,0x00,0x00, 0xFF,0xE6,0x02,0x98, 0xFB,0xA0,0x00,0x00, 0xFB,0x9F,0x10,0x00, 
  0xFB,0x80,0x26,0x00, 0xFF,0xF7,0x00,0x00, 0xFB,0x80,0x00,0x80, 0xFB,0x10,0x20,0x00, 0xFB,0xA0,0xCA,0x40, 0xC0,0xF0,0x40,0x20, 0x90,0x03,0x20,0x00, 0xFB,0x10,0x20,0x00, 
  0xFB,0x9F,0xA2,0x40, 0xFB,0x9F,0xA2,0x01, 0xC0,0xF0,0x20,0x20, 0x90,0x03,0x20,0x00, 0xFB,0x80,0x00,0x80, 0x00,0x00,0x00,0x00, 
  /* 0x2B757 - 0x2B786: 192 bytes, 48 words */
  0x02,0xB7,0x57,
  0x00,0xC0,
  0x00,0xEF,0xC8,0x3C, 0x64,0x05,0x99,0x9A, 0x01,0x01,0x07,0x00, 0xD8,0x1C,0xCC,0xCD, 0xDA,0x03,0x33,0x33, 0xD1,0x02,0xAA,0xAB, 0xD0,0x15,0x55,0x55, 0xCA,0x08,0x00,0x00, 
  0xFF,0x0D,0xD0,0xF1, 0xF7,0xFE,0xCC,0xCD, 0xF6,0x19,0x99,0x9A, 0xF0,0x1A,0x00,0x00, 0xF1,0x06,0x00,0x00, 0x05,0x01,0x06,0x00, 0x17,0xF6,0x00,0x00, 0x16,0x33,0x33,0x33, 
  0x00,0x2F,0x40,0x00, 0x66,0x0A,0xE8,0xBA, 0x6E,0x07,0x11,0xC6, 0x6F,0x0D,0x21,0x01, 0x5A,0x28,0x51,0xEC, 0x5B,0x09,0xBA,0x5E, 0x7C,0x06,0xD4,0x22, 0x7D,0x00,0x07,0x2F, 
  0x62,0xFF,0x45,0xD1, 0x68,0x10,0x28,0x9F, 0x69,0x04,0x0A,0x28, 0x6A,0xEF,0xBA,0x1B, 0x61,0x2E,0x8B,0xA3, 0x4B,0x19,0x33,0x33, 0x4C,0x01,0xE6,0x66, 0x6B,0x14,0x32,0xC7, 
  0x75,0x02,0xC6,0xC6, 0x09,0x81,0x02,0x00, 0xD2,0x17,0x91,0x07, 0xD3,0x05,0xE4,0x42, 0xD4,0x02,0x8A,0xB7, 0xDD,0x00,0x01,0x33, 0xDE,0x00,0x01,0xCC, 0xDB,0x01,0x65,0x56, 
  0xDC,0x02,0xC6,0xC6, 0x03,0x81,0x01,0x00, 0x00,0x02,0x07,0x64, 0x00,0x02,0x07,0x80, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
  /* 0x21FE0 - 0x21FE2: 12 bytes, 3 words */
  0x02,0x1F,0xE0,
  0x00,0x0C,
  0x00,0x00,0x01,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x01,0x00, 
  /* 0x29FE0 - 0x29FE2: 12 bytes, 3 words */
  0x02,0x9F,0xE0,
  0x00,0x0C,
  0x00,0x00,0x01,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x01,0x00, 
  /* 0x14008: 4 bytes, 1 word */
  0x01,0x40,0x08,
  0x00,0x04,
  0x00,0x00,0x00,0x20, 
  /* 0x21800: 4 bytes, 1 word */
  0x02,0x18,0x00,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x21902: 4 bytes, 1 word */
  0x02,0x19,0x02,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x14008: 4 bytes, 1 word */
  0x01,0x40,0x08,
  0x00,0x04,
  0x00,0x00,0x00,0x30, 
  /* 0x21800: 4 bytes, 1 word */
  0x02,0x18,0x00,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x21902: 4 bytes, 1 word */
  0x02,0x19,0x02,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x01000: 4 bytes, 1 word */
  0x00,0x10,0x00,
  0x00,0x04,
  0x00,0x00,0x00,0x20, 
  /* 0x01000: 4 bytes, 1 word */
  0x00,0x10,0x00,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x21C9E - 0x21C9F: 8 bytes, 2 words */
  0x02,0x1C,0x9E,
  0x00,0x08,
  0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
  /* 0x21C3A: 4 bytes, 1 word */
  0x02,0x1C,0x3A,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x21C3E: 4 bytes, 1 word */
  0x02,0x1C,0x3E,
  0x00,0x04,
  0x00,0x00,0x00,0x00, 
  /* 0x0A010: 4 bytes, 1 word */
  0x00,0xA0,0x10,
  0x00,0x04,
  0x00,0x11,0x00,0x00, 
  /* 0x14012: 4 bytes, 1 word */
  0x01,0x40,0x12,
  0x00,0x04,
  0x00,0xFF,0x00,0xFF, 
  /* TERMINATION */
  0xFF,0xFF,0xFF
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void Star_Write(eTuner_Access_Mode accessMode, uint8 byteNum, uint8 *pData)
{
	uint8 mode = (accessMode == ACCESS_DIRECT) ? MODE_DIRECT_WRITE : MODE_CMD_WRITE;
	
	*pData = (*pData & 0x03) | mode;
	Write_TDA7708_RegMap(pData, byteNum);
}

/*************************************************************************************
Function		: Star_Read
Description	: Read data from Star TDA7707

Parameters	:
			 eTuner_Access_Mode  accessMode:  Tuner  access mode
				ACCESS_DIRECT,
				ACCESS_COMMAND,
				
			wordNum : the word number which need to be read out from the pAddr and following address.
			pAddr : the tuner's command interface buffer address pointer
			pData : the data buffer to get the data from tuner.
		
Return Value	: None		
*************************************************************************************/
void Star_Read( eTuner_Access_Mode accessMode, uint8 wordNum, uint8 *pAddr, uint8 *pData)
{
	uint8 iRet = FALSE;
	//int i;
	int byteNum;
	uint8 mode;
	
	byteNum = wordNum * ((accessMode == ACCESS_DIRECT)  ? 4 : 3) ;  //Direct access is 32 bits mode, command access is 24 bits mode
	mode = (accessMode == ACCESS_DIRECT) ? MODE_DIRECT_READ : MODE_CMD_READ;
		
	*pAddr = (*pAddr & 0x0F ) | mode;
	
	iRet = Write_TDA7708_RegMap(pAddr, 3);
		
	if(iRet)
	{
		delay_1us(50);
		iRet = Read_TDA7708_RegMap(pData, byteNum);
		/*
		int i;
		for(i = 0; i < byteNum; i++)
		{
			cout("0x%x ", pData[i]);
		}
		cout("\n");
		*/
	}
}

#define MAX_SIZE_PER_SEND  64

uint8 Star_Download_BootCode(uint8 req)
{
	static uint16 byteNum = 0,startNum = 0, i = 0;		/* Normally bootcode size < 65k */
	static U32 addr = 0;

	uint8 sendByteNum,j,ret = FALSE;
	uint8 tempData[MAX_SIZE_PER_SEND+4];
	
	if(req)
	{
		byteNum = 0;
		startNum = 0;
		addr = 0;
		i = 0;
		ret = FALSE;
		goto exit;
	}
	
	for(;;)
	{
		if(byteNum == 0)
		{
			if ((CMOST_Firmware[i] != 0xFF) || (CMOST_Firmware[i + 1] != 0xFF) || (CMOST_Firmware[i + 2] != 0xFF))
			{
				byteNum = CMOST_Firmware[i + 3] * 256 + CMOST_Firmware[i + 4];   //bytes number (data)      
				addr = 0;
				addr = CMOST_Firmware[i++];
				addr <<= 8;
				addr |= CMOST_Firmware[i++];
				addr <<= 8;
				addr |= CMOST_Firmware[i++];
				startNum = i;
			}
			else
			{
				ret = TRUE;
				goto exit;
			}
		}

		if(byteNum != 0)
		{
			if(byteNum > MAX_SIZE_PER_SEND)
				sendByteNum = MAX_SIZE_PER_SEND;
			else
				sendByteNum = byteNum;

			tempData[0] = (unsigned char)((addr >> 16) & 0xff);
			tempData[1] = (unsigned char)((addr >> 8) & 0xff);
			tempData[2] = (unsigned char)(addr& 0xff);
			
			for(j = 0; j < sendByteNum; j++)
			{
				tempData[3+j] = CMOST_Firmware[startNum+2+j] ;
			}

			Star_Write(ACCESS_DIRECT, sendByteNum + 3, tempData);

			startNum += sendByteNum;
			byteNum -= sendByteNum;
			addr += sendByteNum/4;

			i = startNum+2;
			ret = FALSE;
		}
	}
exit:
	return ret;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
static uint8 tda7708_cmd_set_mem(uint32 menAddr, uint32 menData)
{
	if((menAddr <= 0x019fff) || (menAddr >= 0x01c000 && menAddr <= 0x01ffff))
	{
		tda7708_write_addr[0] 	= _u32to8hl_(menAddr);
		tda7708_write_addr[1]	= _u32to8lh_(menAddr);	
		tda7708_write_addr[2]	= _u32to8ll_(menAddr);	
		tda7708_write_addr[3]	= _u32to8hh_(menData);
		tda7708_write_addr[4]	= _u32to8hl_(menData);
		tda7708_write_addr[5]	= _u32to8lh_(menData);
		tda7708_write_addr[6]	= _u32to8hl_(menData);
		tda7708_write_addr[7]	= _u32to8lh_(menData);
		tda7708_write_addr[8]	= _u32to8ll_(menData);
		
		return tda7708_cmd_write_read(CmdCode_WriteDMAmem, 3, 1);
	}
	else if( menAddr >= 0x01a000 && menAddr <= 0x01afff)
	{
		tda7708_write_addr[0] 	= _u32to8hl_(menAddr);
		tda7708_write_addr[1]	= _u32to8lh_(menAddr);	
		tda7708_write_addr[2]	= _u32to8ll_(menAddr);	
		tda7708_write_addr[3]	= _u32to8hl_(menData);
		tda7708_write_addr[4]	= _u32to8lh_(menData);
		tda7708_write_addr[5]	= _u32to8ll_(menData);
		
		return tda7708_cmd_write_read(CmdCode_WriteXmen, 2, 1);
	}
	else if( menAddr >= 0x01b000 && menAddr <= 0x01bfff )
	{
		tda7708_write_addr[0] 	= _u32to8hl_(menAddr);
		tda7708_write_addr[1]	= _u32to8lh_(menAddr);	
		tda7708_write_addr[2]	= _u32to8ll_(menAddr);	
		tda7708_write_addr[3]	= _u32to8hl_(menData);
		tda7708_write_addr[4]	= _u32to8lh_(menData);
		tda7708_write_addr[5]	= _u32to8ll_(menData);
		
		return tda7708_cmd_write_read(CmdCode_WriteYmen, 2, 1);
	}

	return FALSE;
}

static uint8 tda7708_cmd_get_mem(uint32 menAddr, uint8 Numword)
{
	if((menAddr <= 0x019fff) || (menAddr >= 0x01c000 && menAddr <= 0x01ffff))
	{		
		tda7708_write_addr[0] 	= _u32to8hl_(menAddr);
		tda7708_write_addr[1]	= _u32to8lh_(menAddr);	
		tda7708_write_addr[2]	= _u32to8ll_(menAddr);	
		tda7708_write_addr[3]	= 0x00;
		tda7708_write_addr[4]	= 0x00;
		tda7708_write_addr[5]	= Numword * 2;
		
		return tda7708_cmd_write_read(CmdCode_ReadDMAmem, 2, Numword * 2 + 1);	
	}
	else if( menAddr >= 0x01a000 && menAddr <= 0x01afff)
	{
		tda7708_write_addr[0] 	= _u32to8hl_(menAddr);
		tda7708_write_addr[1]	= _u32to8lh_(menAddr);	
		tda7708_write_addr[2]	= _u32to8ll_(menAddr);	
		tda7708_write_addr[3]	= 0x00;
		tda7708_write_addr[4]	= 0x00;
		tda7708_write_addr[5]	= Numword;
		
		return tda7708_cmd_write_read(CmdCode_ReadXmen, 2, Numword + 1);
	}
	else if( menAddr >= 0x01b000 && menAddr <= 0x01bfff )
	{
		tda7708_write_addr[0] 	= _u32to8hl_(menAddr);
		tda7708_write_addr[1]	= _u32to8lh_(menAddr);	
		tda7708_write_addr[2]	= _u32to8ll_(menAddr);	
		tda7708_write_addr[3]	= 0x00;
		tda7708_write_addr[4]	= 0x00;
		tda7708_write_addr[5]	= Numword;
		
		return tda7708_cmd_write_read(CmdCode_ReadYmen, 2, Numword + 1);
	}

	return FALSE;
}

static uint8 tda7708_cmd_set_stereo(uint8 stereo)
{
	if(stereo)
    {   
    	if(!tda7708_cmd_set_mem(PILOT_THS_ADDR, tda7708_pilotth))return FALSE;
    }
    else
    {
        if(!tda7708_cmd_set_mem(PILOT_THS_ADDR, 0x7fffff))return FALSE;
		if(!tda7708_cmd_get_mem(PILOT_THS_ADDR, 1))return FALSE;
    }
	
	return TRUE;
}

static uint8 tda7708_cmd_set_dsp_mode(tda7708_dsp_e dsp_mode)
{
	tda7708_write_addr[0]=0x01;//Para 1
	tda7708_write_addr[1]=0xa0;
	tda7708_write_addr[2]=0x06;

	tda7708_write_addr[3]=0x0;//Para2,
	tda7708_write_addr[4]=0x0;
	tda7708_write_addr[5]=dsp_mode;

	return tda7708_cmd_write_read(CmdCode_WriteXmen, 2, 1);

}

static uint8 tda7708_cmd_set_freq(tda7708_band_e band, uint16 set_freq)
{
	uint32 freq;

	switch(band)
	{
		case TDA7708_BAND_FM:
			freq = set_freq * 10;
			break;
			
		case TDA7708_BAND_WB:
			freq = set_freq + 100000;
			break;
			
		case TDA7708_BAND_AM:
			freq = set_freq;
			break;
			
		case TDA7708_BAND_SW:
			freq = (set_freq - 1384) * 5;
			break;
			
		default:
			freq = set_freq;
			break;
	}
	
	tda7708_write_addr[0] = _u32to8hl_(freq);
	tda7708_write_addr[1] = _u32to8lh_(freq);
	tda7708_write_addr[2] = _u32to8ll_(freq);
	
	return tda7708_cmd_write_read(CmdCode_SetFreq, 1, 1);
	
}

static uint8 tda7708_cmd_set_FEReg(tda7708_band_e band, uint8 start, uint8 num)
{
	uint8 i;
	
	if(num == 0)
	{
		return tda7708_cmd_write_read(CmdCode_SetFEReg, 0, 1);
	}
	else
	{
		//set FE registers to user-specified values, starting from specified subaddress
		tda7708_write_addr[0] = 0x00;//para 1---Subaddress
		tda7708_write_addr[1] = 0x00;
		tda7708_write_addr[2] = start;
	
		if(start == 15)
		{
			if(band == TDA7708_BAND_FM)
			{
				tda7708_write_addr[3] = TDA7708_FMFEReg[8*3 + 0];
				tda7708_write_addr[4] = TDA7708_FMFEReg[8*3 + 1];
				tda7708_write_addr[5] = TDA7708_FMFEReg[8*3 + 2];
			}
			else if(band == TDA7708_BAND_WB)
			{
				tda7708_write_addr[3] = TDA7708_WXFEReg[8*3 + 0];
				tda7708_write_addr[4] = TDA7708_WXFEReg[8*3 + 1];
				tda7708_write_addr[5] = TDA7708_WXFEReg[8*3 + 2];
			}
			else
			{
				tda7708_write_addr[3] = TDA7708_AMFEReg[8*3 + 0];
				tda7708_write_addr[4] = TDA7708_AMFEReg[8*3 + 1];
				tda7708_write_addr[5] = TDA7708_AMFEReg[8*3 + 2];
			}
		}
		else
		{
			if(band == TDA7708_BAND_FM)
			{
				for(i = 0; i < num; i++)
				{
					tda7708_write_addr[3 + i*3] = TDA7708_FMFEReg[start*3 + i*3 + 0];
					tda7708_write_addr[4 + i*3] = TDA7708_FMFEReg[start*3 + i*3 + 1];
					tda7708_write_addr[5 + i*3] = TDA7708_FMFEReg[start*3 + i*3 + 2];
				}
			}
			else if(band == TDA7708_BAND_WB)
			{
				for(i=0; i<num; i++)
				{
					tda7708_write_addr[3 + i*3] = TDA7708_WXFEReg[start*3 + i*3 + 0];
					tda7708_write_addr[4 + i*3] = TDA7708_WXFEReg[start*3 + i*3 + 1];
					tda7708_write_addr[5 + i*3] = TDA7708_WXFEReg[start*3 + i*3 + 2];
				}
			}
			else
			{
				for(i=0; i<num; i++)
				{
					tda7708_write_addr[3 + i*3] = TDA7708_AMFEReg[start*3 + i*3 + 0];
					tda7708_write_addr[4 + i*3] = TDA7708_AMFEReg[start*3 + i*3 + 1];
					tda7708_write_addr[5 + i*3] = TDA7708_AMFEReg[start*3 + i*3 + 2];
				}
			}
		}

		return tda7708_cmd_write_read(CmdCode_SetFEReg, num + 1, 1);
	}
}

static uint8 tda7708_cmd_set_band(tda7708_band_e band, uint16 min_freq, uint16 max_freq)
{		
	uint8 index = 0;
	
	if(band == TDA7708_BAND_WB)
	{
		UINT32 MinFreq = min_freq + 100000;
		UINT32 MaxFreq = max_freq + 100000;
		
		tda7708_write_addr[index++] = 0x00;
		tda7708_write_addr[index++] = 0x00;
		tda7708_write_addr[index++] = 0x01;
		
		tda7708_write_addr[index++] = _u32to8hl_(MinFreq);
		tda7708_write_addr[index++] = _u32to8lh_(MinFreq);
		tda7708_write_addr[index++] = _u32to8ll_(MinFreq);
		
		tda7708_write_addr[index++] = _u32to8hl_(MaxFreq);
		tda7708_write_addr[index++] = _u32to8lh_(MaxFreq);
		tda7708_write_addr[index++] = _u32to8ll_(MaxFreq);
		
		return tda7708_cmd_write_read(CmdCode_SetBand, 3, 1);
	}
	else if(band == TDA7708_BAND_FM)
	{
		uint32 MinFreq = min_freq * 10;
		uint32 MaxFreq = max_freq * 10;
		
		tda7708_write_addr[index++] = 0x00;
		tda7708_write_addr[index++] = 0x00;
		tda7708_write_addr[index++] = 0x01;
		
		tda7708_write_addr[index++] = _u32to8hl_(MinFreq);
		tda7708_write_addr[index++] = _u32to8lh_(MinFreq);
		tda7708_write_addr[index++] = _u32to8ll_(MinFreq);
		
		tda7708_write_addr[index++] = _u32to8hl_(MaxFreq);
		tda7708_write_addr[index++] = _u32to8lh_(MaxFreq);
		tda7708_write_addr[index++] = _u32to8ll_(MaxFreq);
		
		return tda7708_cmd_write_read(CmdCode_SetBand, 3, 1);
	}
	else if(band == TDA7708_BAND_AM)
	{
		uint32 MinFreq = min_freq;
		uint32 MaxFreq = max_freq;
		
		if(((tda7708_area == TDA7708_AREA_USA)||(tda7708_area==TDA7708_AREA_LATIN)))
		{
			tda7708_write_addr[index++] = 0x00;
			tda7708_write_addr[index++] = 0x00;
			tda7708_write_addr[index++] = 0x03;
			
			tda7708_write_addr[index++] = _u32to8hl_(MinFreq);
			tda7708_write_addr[index++] = _u32to8lh_(MinFreq);
			tda7708_write_addr[index++] = _u32to8ll_(MinFreq);
			
			tda7708_write_addr[index++] = _u32to8hl_(MaxFreq);
			tda7708_write_addr[index++] = _u32to8lh_(MaxFreq);
			tda7708_write_addr[index++] = _u32to8ll_(MaxFreq);
			
			return tda7708_cmd_write_read(CmdCode_SetBand, 3, 1);
		}
		else
		{
			tda7708_write_addr[index++] = 0x00;
			tda7708_write_addr[index++] = 0x00;
			tda7708_write_addr[index++] = 0x02;
			
			tda7708_write_addr[index++] = _u32to8hl_(MinFreq);
			tda7708_write_addr[index++] = _u32to8lh_(MinFreq);
			tda7708_write_addr[index++] = _u32to8ll_(MinFreq);
			
			tda7708_write_addr[index++] = _u32to8hl_(MaxFreq);
			tda7708_write_addr[index++] = _u32to8lh_(MaxFreq);
			tda7708_write_addr[index++] = _u32to8ll_(MaxFreq);
			
			return tda7708_cmd_write_read(CmdCode_SetBand, 3, 1);
		}
	}
	else if(band == TDA7708_BAND_SW)
	{
		UINT32 MinFreq = (min_freq - 1384) * 5;
		UINT32 MaxFreq = (max_freq - 1384) * 5;
		
		tda7708_write_addr[index++] = 0x00;
		tda7708_write_addr[index++] = 0x00;
		tda7708_write_addr[index++] = 0x02;
		
		tda7708_write_addr[index++] = _u32to8hl_(MinFreq);
		tda7708_write_addr[index++] = _u32to8lh_(MinFreq);
		tda7708_write_addr[index++] = _u32to8ll_(MinFreq);
		
		tda7708_write_addr[index++] = _u32to8hl_(MaxFreq);
		tda7708_write_addr[index++] = _u32to8lh_(MaxFreq);
		tda7708_write_addr[index++] = _u32to8ll_(MaxFreq);
		
		return tda7708_cmd_write_read(CmdCode_SetBand, 3, 1);
	}
	
	return FALSE;
}

static uint8 tda7708_cmd_set_seekTH(uint32 MeasCycle, uint32 fieldstrengthTH, uint32 AdjTH, uint32 DetuneTH, uint32 MultipathTH, uint32 QualityTH)
{	
	uint8 index = 0;
	
	tda7708_write_addr[index++] = _u32to8hl_(MeasCycle);
	tda7708_write_addr[index++] = _u32to8lh_(MeasCycle);
	tda7708_write_addr[index++] = _u32to8ll_(MeasCycle);

	tda7708_write_addr[index++] = _u32to8hl_(fieldstrengthTH);
	tda7708_write_addr[index++] = _u32to8lh_(fieldstrengthTH);
	tda7708_write_addr[index++] = _u32to8ll_(fieldstrengthTH);
	
	tda7708_write_addr[index++] = _u32to8hl_(AdjTH);
	tda7708_write_addr[index++] = _u32to8lh_(AdjTH);
	tda7708_write_addr[index++] = _u32to8ll_(AdjTH);
	
	tda7708_write_addr[index++] = _u32to8hl_(DetuneTH);
	tda7708_write_addr[index++] = _u32to8lh_(DetuneTH);
	tda7708_write_addr[index++] = _u32to8ll_(DetuneTH);

	tda7708_write_addr[index++] = _u32to8hl_(MultipathTH);
	tda7708_write_addr[index++] = _u32to8lh_(MultipathTH);
	tda7708_write_addr[index++] = _u32to8ll_(MultipathTH);
	
	tda7708_write_addr[index++] = _u32to8hl_(QualityTH);
	tda7708_write_addr[index++] = _u32to8lh_(QualityTH);
	tda7708_write_addr[index++] = _u32to8ll_(QualityTH);
	
	return tda7708_cmd_write_read(CmdCode_SetSeekTH, 6, 1);
}

uint8 tda7708_cmd_set_manual_seek(int FreqIncrement)
{
	tda7708_write_addr[0] = _u32to8hl_(FreqIncrement);
	tda7708_write_addr[1] = _u32to8lh_(FreqIncrement);
	tda7708_write_addr[2] = _u32to8ll_(FreqIncrement);
	
	return tda7708_cmd_write_read(CmdCode_StartManuSeek, 1, 1);
}

static uint8 tda7708_cmd_set_autoseek(int FreqIncrement, uint8 AutoSeekEnd)
{
	tda7708_write_addr[0] = _u32to8hl_(FreqIncrement);
	tda7708_write_addr[1] = _u32to8lh_(FreqIncrement);
	tda7708_write_addr[2] = _u32to8ll_(FreqIncrement);
	
	tda7708_write_addr[3] = 0x00;
	tda7708_write_addr[4] = 0x00;
	tda7708_write_addr[5] = AutoSeekEnd;	//AutoSeekEnd=1: automatically unmute the audio and leave the seek mode after a search stop condition has been detected
	
	return tda7708_cmd_write_read(CmdCode_StartAutoSeek, 2, 1);
}

static uint8 tda7708_cmd_set_seekend(uint8 SeekEndAction)
{
	Star_Cmd_Tuner_Seek_End(eTuner_Channel_Main,  (SeekEndAction == SeekStopUnmuted) ? 0 : 1);
	return TRUE;
}

static uint8 tda7708_cmd_set_start_align(uint8 AlignMode)
{
	tda7708_write_addr[0] = 0x00;
	tda7708_write_addr[1] = 0x00;
	tda7708_write_addr[2] = AlignMode & 0x03;
	
	return tda7708_cmd_write_read(CmdCode_StartAlignment, 1, 1);
}

static uint8 tda7708_cmd_set_IR_type(uint8 type)
{
	tda7708_write_addr[0] = 0x00;
	tda7708_write_addr[1] = 0x00;
	tda7708_write_addr[2] = type;
	
	return tda7708_cmd_write_read(CmdCode_IR, 1, 1);
}

static uint8 tda7708_cmd_set_BB_IF(uint8 type)
{
	tda7708_write_addr[0] = 0x00;
	tda7708_write_addr[1] = 0x00;
	tda7708_write_addr[2] = type;
	
	return tda7708_cmd_write_read(CmdCode_IR, 1, 1);
}

static uint8 tda7708_cmd_set_DDC(void)
{
	return tda7708_cmd_write_read(CmdCode_SetDDC, 0, 1);
}

static uint8 tda7708_cmd_set_MuSICA(uint8 val)
{
	tda7708_write_addr[0] = 0x00;
	tda7708_write_addr[1] = 0x00;
	tda7708_write_addr[2] = val;
	
	return tda7708_cmd_write_read(CmdCode_MuSICA, 1, 1);
}

static uint8 tda7708_cmd_set_DynIS(uint8 mode)
{
	/*Para1----0x000000 means disable the dynamic image selection;
			     0x000001 means DynIS is enabled without the cyclic rechecking of the optimum injection side.
			     0x000002, the DynIS is enabled with the cyclic rechecking of the optimum injection side.*/
	/*no parameter----mode=0xff*/
	
	if(mode == 0xFF)
	{
		return tda7708_cmd_write_read(CmdCode_SetDynIS, 0, 1);
	}
	else
	{
		tda7708_write_addr[0] = 0x00;
		tda7708_write_addr[1] = 0x00;
		tda7708_write_addr[2] = mode;
		
		return tda7708_cmd_write_read(CmdCode_SetDynIS, 1, 1);
	}
}

static uint8 tda7708_cmd_set_diss(uint8 mode, uint8 bandwidth)
{
	tda7708_write_addr[0] = 0x00;		//Para1
	tda7708_write_addr[1] = 0x00;
	tda7708_write_addr[2] = mode;
	
	if((mode == Diss_FM_Manual)||(mode == Diss_FM_Special))
	{
		tda7708_write_addr[3] = 0x00;	//Para2
		tda7708_write_addr[4] = 0x00;
		tda7708_write_addr[5] = bandwidth;
		
		return tda7708_cmd_write_read(CmdCode_SetDiss, 2, 1);
	}
	else
	{
		return tda7708_cmd_write_read(CmdCode_SetDiss, 1, 1);
	}
}

static uint8 tda7708_cmd_set_becoeff(const uint8 *p_be_coeff)
{
	uint8 num,num_word;
	uint8 i;
	uint32 addr;
	uint8 cmdcode = 0;
	uint8 offset;

	while(*p_be_coeff != 0xff)
	{
		num = num_word = p_be_coeff[0];
		addr = _u8tou32_(0x00, p_be_coeff[1], p_be_coeff[2], p_be_coeff[3]);
		offset = 0;
		
		if((addr <= 0x019fff)||((addr >= 0x01c000)&&(addr <= 0x01ffff)))
		{
			cmdcode = CmdCode_WriteDMAmem;
		}
		else if((addr >= 0x01a000)&&(addr <= 0x01afff))
		{
			cmdcode = CmdCode_WriteXmen;
		}
		else if((addr >= 0x01b000)&&(addr <= 0x01bfff))
		{
			cmdcode = CmdCode_WriteYmen;
		}
		
		if(cmdcode == CmdCode_WriteDMAmem)
		{
			for(i = 0; i < num; i++)
			{
				tda7708_write_addr[0] = _u32to8hl_(addr);		//Para 1  memory address
				tda7708_write_addr[1] = _u32to8lh_(addr);
				tda7708_write_addr[2] = _u32to8ll_(addr);
				tda7708_write_addr[3] = p_be_coeff[4 + i*4];	//Para 2
				tda7708_write_addr[4] = p_be_coeff[5 + i*4];
				tda7708_write_addr[5] = p_be_coeff[6 + i*4];
				tda7708_write_addr[6] = tda7708_write_addr[4];
				tda7708_write_addr[7] = tda7708_write_addr[5];
				tda7708_write_addr[8] = p_be_coeff[7 + i*4];
				
				if(!tda7708_cmd_write_read(cmdcode, 3, 1))return FALSE;
				
				addr++;
			}
			
			p_be_coeff += num_word*4 + 4;
		}
		else if((cmdcode == CmdCode_WriteXmen)||(cmdcode == CmdCode_WriteYmen))
		{
			while(num > 7)
			{
				tda7708_write_addr[0] = _u32to8hl_(addr);		//Para 1  memory address
				tda7708_write_addr[1] = _u32to8lh_(addr);
				tda7708_write_addr[2] = _u32to8ll_(addr);
				
				for(i = 0; i < 7; i++)
				{
					tda7708_write_addr[3 + i*3] = p_be_coeff[4 + i*4 + offset];//Para 2
					tda7708_write_addr[4 + i*3] = p_be_coeff[5 + i*4 + offset];
					tda7708_write_addr[5 + i*3] = p_be_coeff[6 + i*4 + offset];
				}
				
				if(!tda7708_cmd_write_read(cmdcode, 8, 1))return FALSE;
				
				addr++;
				offset += 4*7;
				num -= 7;
			}
			
			tda7708_write_addr[0] = _u32to8hl_(addr);		//Para 1  memory address
			tda7708_write_addr[1] = _u32to8lh_(addr);
			tda7708_write_addr[2] = _u32to8ll_(addr);
			
			for(i = 0; i < num; i++)
			{
				tda7708_write_addr[3 + i*3] = p_be_coeff[4 + i*4 + offset];//Para 2
				tda7708_write_addr[4 + i*3] = p_be_coeff[5 + i*4 + offset];
				tda7708_write_addr[5 + i*3] = p_be_coeff[6 + i*4 + offset];
			}
			
			if(!tda7708_cmd_write_read(cmdcode, num+1, 1))return FALSE;
			
			p_be_coeff += num_word*4 + 4;
		}
	}
	
	return TRUE;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

static uint8 tda7708_cmd_init_stereo(void)
{
	if(!tda7708_cmd_get_mem(PILOT_THS_ADDR, 1))return FALSE;
	
	tda7708_pilotth = _u8tou32_(0x00, tda7708_read_addr[3], tda7708_read_addr[4], tda7708_read_addr[5]);
	return TRUE;
}

static uint8 tda7708_cmd_get_stereo(uint8 *stereo)
{
	if(!tda7708_cmd_get_mem(0x01a0fa, 1))return FALSE;
	
	if((tda7708_read_addr[3] == 0x7F)&&(tda7708_read_addr[4] == 0xFF)&&(tda7708_read_addr[5] == 0xFC))//means the pilot is higher than the threshold ST_PILOT_DET_TH
	{
		*stereo = 1;
	}
	else
	{
		*stereo = 0;
	}
	
	return TRUE;
}

static uint8 tda7708_cmd_get_TDSR(uint8 *AlignDone, uint8 *IS, uint8 *TDBusy)
{
	if(!tda7708_cmd_write_read(CmdCode_ReadTDS, 0, 1+1+1))return FALSE;	//add read out checksum
	
	if(tda7708_read_addr[4] & 0x04)
	{
		*AlignDone = 1;
	}
	else
	{
		*AlignDone = 0;
	}
	
	if(tda7708_read_addr[4] & 0x08)
	{
		*IS = 1;
	}
	else
	{
		*IS = 0;
	}
	
	if(tda7708_read_addr[5] & 0x40)
	{
		*TDBusy = 1;
	}
	else
	{
		*TDBusy = 0;
	}
	
	return TRUE;
}

static uint8 tda7708_cmd_get_FEReg(uint8 FESubaddress, uint8 num)
{
	tda7708_write_addr[0] = 0x00;
	tda7708_write_addr[1] = 0x00;
	tda7708_write_addr[2] = FESubaddress;
	
	tda7708_write_addr[3] = 0x00;
	tda7708_write_addr[4] = 0x00;
	tda7708_write_addr[5] = num;
	
	return tda7708_cmd_write_read(CmdCode_ReadFEReg, 2, num + 1);
}

static uint8 tda7708_cmd_get_seekstatus(tda7708_band_e band, tda7708_status_t *status, eTuner_Channel tunerChannel,u8 times)
{
	U32 freq;
	s8 qualSmeter = 0;
	s8 qualAdj = 0;
	u8 qualDT = 0;
	u8 qualMP = 0;
	//u8 qualDev = 0;
	u8 qualMPX = 0;
	//u8 qualIFBW =0;
	u8 qualSNR = 0;

	if(times == 0)
	{
		Radio.ChkCounter = 0;
		return FALSE;
	}

	Star_Cmd_Tuner_Get_Seek_Status(tunerChannel);
	
	freq   = (U32)(Radio.DataBuff[3] & 0x1F) << 16 ; //RF frequency
	freq |= (U32)(Radio.DataBuff[4] & 0xFF) << 8 ;
	freq |= (U32)(Radio.DataBuff[5] & 0xFF);
	
	Radio.BgScanFreq = freq;
	
	qualSmeter  = ((Radio.DataBuff[7] & 0x80) ? 0 : Radio.DataBuff[7]);
	qualDT   = Radio.DataBuff[8];
	qualMP  = Radio.DataBuff[9];
	qualMPX = Radio.DataBuff[10];
	qualSNR = Radio.DataBuff[11];
	qualAdj = ((Radio.DataBuff[12] & 0x80) ? 0 : Radio.DataBuff[12]);

	printf("freq:%d\n", freq);
	printf("qualSmeter:%d, qualDT:%d, qualMP:%d, qualMPX:%d, qualSNR:%d, qualAdj:%d\n", qualSmeter, qualDT, qualMP, qualMPX, qualSNR, qualAdj);
	printf("qualSmeter:%d, qualDT:%d, qualMP:%d, qualMPX:%d, qualSNR:%d, qualAdj:%d\n", Radio.Smeter, Radio.Detuning, Radio.Multipath, Radio.MpxNoise, Radio.SNR, Radio.AdjChannel);
	printf("Radio.ChkCounter:%d\n", Radio.ChkCounter);
	
	if(Tuner.SeekType == eSeekAuto)
	{
		Radio.F_GoodStation    = (Radio.DataBuff[3] & SEEK_STATUS_SEEKOK) ? 1 : 0;
		Radio.F_SeekFullCycle = (Radio.DataBuff[3] & 0x40) ? 1 : 0;		/*BIT 22*/
			
	}
	else 
	{
		if(Radio.Freq == Radio.BackupFreq)
			Radio.F_SeekFullCycle = 1;
		else
			Radio.F_SeekFullCycle = 0;
	}

	if(Radio.ChkCounter == 0)
	{
		Radio.ChkCounter++;
		Radio.Smeter	   = qualSmeter;
		Radio.Detuning = qualDT;
		Radio.Multipath = qualMP;
		Radio.MpxNoise = qualMPX;
		Radio.SNR = qualSNR;
		Radio.AdjChannel = qualAdj;
	}
	else
	{
		Radio.ChkCounter++;
		Radio.Smeter	   = Radio.Smeter - Radio.Smeter/Radio.ChkCounter + qualSmeter/Radio.ChkCounter;
		Radio.Detuning = Radio.Detuning - Radio.Detuning/Radio.ChkCounter + qualDT/Radio.ChkCounter;
		Radio.Multipath = Radio.Multipath - Radio.Multipath/Radio.ChkCounter + qualMP/Radio.ChkCounter;
		Radio.MpxNoise = Radio.MpxNoise - Radio.MpxNoise/Radio.ChkCounter + qualMPX/Radio.ChkCounter;
		Radio.SNR = Radio.SNR - Radio.SNR/Radio.ChkCounter+qualSNR/Radio.ChkCounter;
		Radio.AdjChannel = Radio.AdjChannel - Radio.AdjChannel/Radio.ChkCounter + qualAdj/Radio.ChkCounter;
		
	}
	
	status->smeter 		= Radio.Smeter;
	status->detuning		= Radio.Detuning;
	status->multipath		= Radio.Multipath;
	status->adj_channel 	= Radio.AdjChannel;
	status->SNR 			= Radio.SNR;
	status->MpxNoise 		= Radio.MpxNoise;
	
	if(Radio.ChkCounter >= times)
		return TRUE;
	else
		return FALSE;
}

static uint8 tda7708_cmd_get_smeter(tda7708_band_e band, uint8 *smeter)
{
	if(band == TDA7708_BAND_FM)
	{
		if(!tda7708_cmd_get_mem(0x01a03e, 2))return FALSE;
		*smeter = tda7708_read_addr[3];
		printf("now station------>smeter:%d\n", *smeter);
	}
	else if(band == TDA7708_BAND_AM)
	{
		if(!tda7708_cmd_get_mem(0x01a1b9, 2))return FALSE;
		*smeter = tda7708_read_addr[3];
	}
	else return FALSE;
	
	return TRUE;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8 tda7708_wait_busy(void)
{
	uint8 AlignDone;
	uint8 IS;
	uint8 TDBusy;
	uint16 time_over = 1000;
	
	while(--time_over)
	{
		if(!tda7708_cmd_get_TDSR(&AlignDone, &IS, &TDBusy))return FALSE;
		if(!TDBusy)return TRUE;
		Wait1ms(1);//1MS
	}
	
	return FALSE;
}

static uint8 tda7708_startup_reset(void)
{
	uint8 iRet = FALSE;
	
	tda7708_order.addr[0] 	= 0x01;
	tda7708_order.addr[1] 	= 0x40;
	tda7708_order.addr[2] 	= 0x1e;

	iRet = tda7708_direct_read(TDA7708_COM_1);
	
	if(iRet)
	{
		if ((( tda7708_order.buffer[0] == 0xFF) && ( tda7708_order.buffer[1] == 0xFF) && ( tda7708_order.buffer[2] == 0xFF) && ( tda7708_order.buffer[3] == 0xFF) )   ||
		   (( tda7708_order.buffer[0] == 0x00) && ( tda7708_order.buffer[1] == 0x00) && ( tda7708_order.buffer[2] == 0x00) && ( tda7708_order.buffer[3] == 0x00) )  )
		{
			iRet = FALSE;
		}
		else
		{
			iRet = TRUE;
		}
	}
	return iRet;
}

static uint8 tda7708_startup_sequence(void)
{
	uint8 iRet = FALSE;
	
	tda7708_order.addr[0] 	= 0x02;
	tda7708_order.addr[1] 	= 0x01;
	tda7708_order.addr[2] 	= 0x80;
	
	iRet = tda7708_direct_read(TDA7708_COM_1);
		
	if((tda7708_order.buffer[0] != 0xAF)
	 ||(tda7708_order.buffer[1] != 0xFE)
	 ||(tda7708_order.buffer[2] != 0x42)
	 ||(tda7708_order.buffer[3] != 0x00))
	{
		iRet = FALSE;
	}
	else
	{
		iRet = TRUE;
	}
	
	return iRet;
}

static uint8 tda7708_check_ESD_crashed(void)
{		
	uint8 read_smeter_log[3];
	uint8 read_sysco_reg1[3];
	uint8 k1 = 0,k2 = 0,k3 = 0,k4 = 0;
	
	tda7708_cmd_check = TRUE;
	
	if(!tda7708_cmd_get_mem(0x01a0f9, 1))//FM_Smeter(log)
	{
		k4 = 1;
	}
	read_smeter_log[0] = tda7708_read_addr[3]; 
	read_smeter_log[1] = tda7708_read_addr[4]; 
	read_smeter_log[2] = tda7708_read_addr[5];
	
	if(!tda7708_cmd_get_mem(0x014001, 1))//SYSCO_REG1
	{
		k4 = 1;
	}
	read_sysco_reg1[0] = tda7708_read_addr[3]; 
	read_sysco_reg1[1] = tda7708_read_addr[4];
	read_sysco_reg1[2] = tda7708_read_addr[5];
	
	tda7708_cmd_check = FALSE;
	
	if((read_sysco_reg1[1] != 0x02)&&(read_sysco_reg1[2] != 0x7B))      //TDA7703=0X00027B,TDA7706M=0X80027B
	{
		k1 = 1;
	}
	if((read_sysco_reg1[1] == read_smeter_log[1])&&(read_sysco_reg1[2] == read_smeter_log[2]))
	{
		k2 = 1;
	}
	if((read_smeter_log[0] == 0)&&(read_smeter_log[1] == 0)&&(read_smeter_log[2] == 0)) 
	{
		k3 = 1;
	}
	
	if((k1==1)||(k2==1)||(k3==1)||(k4==1))   
	{
		return FALSE;
	}
	
	return TRUE;
}

static uint8 tda7708_set_mute(uint8 mute)
{
	if(mute == ON)
	{
		
	}
	else
	{
		
	}
	return TRUE;
}

static uint8 tda7708_set_loc(tda7708_band_e band, uint8 loc)
{
	if(loc == ON)
	{
		switch(band)
		{
			case TDA7708_BAND_FM:
				if(!tda7708_cmd_set_seekTH(0x000500, TDA7708_TUNER_RSSI_LOC_FM, 0x400000, 0x0a0000, 0x5fffff, 0x554000))return FALSE;
				break;
				
			case TDA7708_BAND_AM:
			case TDA7708_BAND_SW:
				if(!tda7708_cmd_set_seekTH(0x000700, TDA7708_TUNER_RSSI_LOC_AM, 0x7fffff, 0x200000, 0x7fffff, 0x000000))return FALSE;
				break;
				
			case TDA7708_BAND_WB:
				if(!tda7708_cmd_set_seekTH(0x000500, 0xC30000, 0x300000, 0x060000, 0x100000, 0x000000))return FALSE;
				break;
				
			default:
				return FALSE;
		}		
	}
	else
	{
		switch(band)
		{
			case TDA7708_BAND_FM:
				if(!tda7708_cmd_set_seekTH(0x000500, TDA7708_TUNER_RSSI_FM, 0x400000, 0x0a0000, 0x5fffff, 0x554000))return FALSE;
				break;
				
			case TDA7708_BAND_AM:
			case TDA7708_BAND_SW:
				if(!tda7708_cmd_set_seekTH(0x000700, TDA7708_TUNER_RSSI_AM, 0x7fffff, 0x200000, 0x7fffff, 0x000000))return FALSE;
				break;
				
			case TDA7708_BAND_WB:
				if(!tda7708_cmd_set_seekTH(0x000500, 0xC30000, 0x300000, 0x060000, 0x100000, 0x000000))return FALSE;
				break;
				
			default:
				return FALSE;
		}	
	}
	
	return TRUE;
}

static uint8 tda7708_set_stereo(uint8 stereo)
{
	if(stereo == ON)
	{
		if(!tda7708_cmd_set_stereo(ON))return FALSE;
	}
	else
	{
		if(!tda7708_cmd_set_stereo(OFF))return FALSE;
	}
	
	return TRUE;
}

static uint8 tda7708_hit_download(tda7708_hit_e hit_type)
{	
	uint8 r = FALSE;
	switch(hit_type)
	{
		case TDA7708_HIT_BOOT:
			r = Write_TDA7708_Download(TDA7708_I2CBootData);
			break;
			
		case TDA7708_HIT_FM_PATCH:
			r = tda7708_cmd_set_becoeff(TDA7708_FMSWPatchData);
			break;
			
		case TDA7708_HIT_AM_PATCH:
			r = tda7708_cmd_set_becoeff(TDA7708_AMSWPatchData);
			break;
			
		case TDA7708_HIT_WX_PATCH:
			r = tda7708_cmd_set_becoeff(TDA7708_WXSWPatchData);
			break;
			
		case TDA7708_HIT_FM_WSP:
			r = tda7708_cmd_set_becoeff(TDA7708_FMCustomWSPData);
			break;
			
		case TDA7708_HIT_AM_WSP:
			r = tda7708_cmd_set_becoeff(TDA7708_AMCustomWSPData);
			break;
			
		case TDA7708_HIT_WX_WSP:
			r = tda7708_cmd_set_becoeff(TDA7708_WXCustomWSPData);
			break;
			
		default: 
			break;
	}
	return r;
}

static uint8 tda7708_set_freq(tda7708_band_e band, uint16 freq)
{	
	if(!tda7708_wait_busy())return FALSE;

	switch(band)
	{
		case TDA7708_BAND_FM:
			if(!tda7708_cmd_set_freq(band, freq))return FALSE;
			if(!tda7708_cmd_set_diss(Diss_FM_Manual, Diss_FM_Manual_BW4))return FALSE;
			OSTimeDly(10);
			if(!tda7708_cmd_set_diss(Diss_FM_Auto, 0))return FALSE;
			if(!tda7708_hit_download(TDA7708_HIT_FM_WSP))return FALSE;
			break;
			
		case TDA7708_BAND_WB:
			if(!tda7708_cmd_set_freq(band, freq))return FALSE;
			if(!tda7708_cmd_set_diss(Diss_WX_Neutral, 0))return FALSE;
			break;
			
		case TDA7708_BAND_AM:
			if(!tda7708_cmd_set_freq(band, freq))return FALSE;
			if(!tda7708_cmd_set_diss(Diss_AM_Neutral, 0))return FALSE;
			if(!tda7708_hit_download(TDA7708_HIT_AM_WSP))return FALSE;
			break;
			
		case TDA7708_BAND_SW:
			if(!tda7708_cmd_set_freq(band, freq))return FALSE;
			if(!tda7708_cmd_set_diss(Diss_AM_Neutral, 0))return FALSE;
			break;
			
		default:
			return FALSE;
	}
	
	return TRUE;
}

static uint8 tda7708_seek_on(tda7708_band_e band, int step)
{
	switch(band)
	{
		case TDA7708_BAND_FM:
			if(!tda7708_cmd_set_manual_seek(step * 10))return FALSE;
			break;
			
		case TDA7708_BAND_AM:
			if(!tda7708_cmd_set_manual_seek(step))return FALSE;
			break;
			
		case TDA7708_BAND_WB:
			if(!tda7708_cmd_set_manual_seek(step))return FALSE;
			break;
			
		case TDA7708_BAND_SW:
			if(!tda7708_cmd_set_manual_seek(step * 5))return FALSE;
			break;
			
		default:
			return FALSE;
	}
	return TRUE;
}

//This function is just a temporary function to be compatible with original digitalradio.c structure.
#if 1
void RadioGetMaxMinFreq(unsigned char  bandReq, unsigned long *pMaxFreq, unsigned long *pMinFreq, unsigned long *pBandFreq)
{
	if (bandReq == TDA7708_BAND_FM)
	{
		*pMinFreq  = (unsigned long)Radio.MinFreq  * 10;	   // In kHz,
		*pMaxFreq = (unsigned long)Radio.MaxFreq * 10;
		*pBandFreq = (unsigned long)Radio.Freq* 10;
	}
	else if (bandReq == TDA7708_BAND_AM)
	{
		*pMinFreq = (unsigned long)((Radio.MinFreq ) );
		*pMaxFreq = (unsigned long)((Radio.MaxFreq ) );
		*pBandFreq = (unsigned long)((Radio.Freq) );
	}
	else if (bandReq == TDA7708_BAND_SW)
	{
		*pMinFreq = (unsigned long)((Radio.MinFreq - 1384) * 5);
		*pMaxFreq = (unsigned long)((Radio.MaxFreq - 1384) * 5);
		*pBandFreq = (unsigned long)((Radio.Freq- 1384) * 5);
	}
	else
	{
		*pMinFreq  = (unsigned long)(Radio.MinFreq);
		*pMaxFreq = (unsigned long)(Radio.MaxFreq);
		*pBandFreq = (unsigned long)(Radio.Freq);
	}
}
#else
void RadioGetMaxMinFreq(unsigned char  bandReq, unsigned long *pMaxFreq, unsigned long *pMinFreq, unsigned long *pBandFreq, unsigned char *pSeekStep)
{
	if (bandReq == TDA7708_BAND_FM)
	{
		*pMinFreq  = (unsigned long)Radio.MinFreq  * 10;	   // In kHz,
		*pMaxFreq = (unsigned long)Radio.MaxFreq * 10;
		*pBandFreq = (unsigned long)Radio.Freq* 10;
		*pSeekStep = Radio.SeekStep * 10;
	}
	else if (bandReq == TDA7708_BAND_AM)
	{
		*pMinFreq = (unsigned long)((Radio.MinFreq ) );
		*pMaxFreq = (unsigned long)((Radio.MaxFreq ) );
		*pBandFreq = (unsigned long)((Radio.Freq) );
		*pSeekStep = Radio.SeekStep;
	}
	else if (bandReq == TDA7708_BAND_SW)
	{
		*pMinFreq = (unsigned long)((Radio.MinFreq - 1384) * 5);
		*pMaxFreq = (unsigned long)((Radio.MaxFreq - 1384) * 5);
		*pBandFreq = (unsigned long)((Radio.Freq- 1384) * 5);
		*pSeekStep = Radio.SeekStep * 5;
	}
	else
	{
		*pMinFreq  = (unsigned long)(Radio.MinFreq);
		*pMaxFreq = (unsigned long)(Radio.MaxFreq);
		*pBandFreq = (unsigned long)(Radio.Freq);
		*pSeekStep = Radio.SeekStep;
	}
}
#endif

static uint8 Star_checkdspinit(void)
{
	uint8 iRet = FALSE;
	SET_BUFFER_WITH_U32(Radio.AddressBuff, 0x020180);
	Star_Read(ACCESS_DIRECT, 1, Radio.AddressBuff, Radio.DataBuff);

	//printf("Star_checkdspinit:DataBuff[0]:0x%x,DataBuff[1]:0x%x,DataBuff[2]:0x%x,DataBuff[3]:0x%x\n",Radio.DataBuff[0],Radio.DataBuff[1],Radio.DataBuff[2],Radio.DataBuff[3]);
	//printf("AFFE42:0x%x,0x%x,0x%x,0x%x\n",_u32to8hh_(TUNER_INIT_SUCESS_AFFE42),_u32to8hl_(TUNER_INIT_SUCESS_AFFE42),_u32to8lh_(TUNER_INIT_SUCESS_AFFE42),_u32to8ll_(TUNER_INIT_SUCESS_AFFE42));

	if((Radio.DataBuff[0] != _u32to8hh_(TUNER_INIT_SUCESS_AFFE42))
	||(Radio.DataBuff[1]  != _u32to8hl_(TUNER_INIT_SUCESS_AFFE42)) 
	||(Radio.DataBuff[2]  != _u32to8lh_(TUNER_INIT_SUCESS_AFFE42)) 
	||(Radio.DataBuff[3]  != _u32to8ll_(TUNER_INIT_SUCESS_AFFE42)))
	{
		iRet = FALSE;
	}
	else
	{
		/* ----- Download configuration  -----*/
		//Specialized digital BB interface through commands Tuner_Conf_BB_IF (0x04), Tuner_Conf_BB_SAI (0x05), Tuner_Conf_JESD204 (0x06)
		Star_Cmd_Tuner_Set_BB_IF(0x000000);   //BB interface disabled
		//Star_Cmd_Tuner_Set_BB_IF(0x080000);	//audio mode8, for TDA7708LX32 GPIO6 as I2S data out
		//Star_Cmd_Tuner_Set_BB_IF(0x010000);	//audio mode1, for TDA7708LX64/52 GPIO0 as I2S data out

		//digital audio interface through command 
		//Star_Cmd_Tuner_Set_Audio_IF(0x03, 0x980330);//48KHz, 32L+32R, master
		//Star_Cmd_Tuner_Set_Audio_IF(0x13, 0x980330);//48KHz, 32L+32R, slave
		iRet = TRUE;
	}
	return iRet;
}

/*************************************************************************************
Function		: Star_Download_CustomizedCoeffs
Description	: Write data in array(FMCustomWSPData, AMCustomWSPData and WBCustomWSPData) to Star 
			by write-mem command
			
Parameter 	: None

Return Value	: None
*************************************************************************************/

void Star_Download_CustomizedCoeffs(void)
{
#ifdef STAR_CUSTOMIZED_SETTING
	int i,	 array_size;
	array_size = sizeof(coeffInit_TDA7708)/sizeof(tCoeffInit);
	
	for (i = 0;  i < array_size; i++)
	{
		Star_Cmd_Tuner_Write(coeffInit_TDA7708[i].coeffAddr, &(coeffInit_TDA7708[i].coeffVal), 1);
	}
#endif
}
TunerStruct Tuner;

/*********************************************
	Function:		Wait10us
	Description:	
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:40:55
*********************************************/
void Wait10us(unsigned char time)
{
     //unsigned char i;
     for(; time > 0; time--)
     {
     	delay_1us(1);
     }
}

/*********************************************
	Function:		Wait1ms
	Description:	
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:40:54
*********************************************/
void Wait1ms(unsigned char time)
{
     for(; time > 0; time--)
     {
	  	 Wait10us(100);
     }
}

/*************************************************************************************
Function		: Star_Calculate_CheckSum
Description	: Calculate the checksum for command access, and set CheckSum, as it's command usage,
			  one word data is always 3 bytes.
			  
Parameters	:
			wordNum = the parameter number + 1 command header, 
			that is the number of the words which need to do summing.
		
Return Value	: the check sum value in U32.
*************************************************************************************/
static U32 Star_Calculate_CheckSum(u8 wordNum)
{
	int i;
	u8  *pData = Radio.DataBuff;
	U32  checkSum = 0;
	
	for (i = 0; i <  wordNum * 3; i++) { checkSum += (U32)(*pData++) << ((2 - i%3) * 8); }	//Claculate checksum
	return checkSum & 0x00FFFFFF;
}


/*************************************************************************************
Function		: Star_SetBuffer_CheckSum
Description	: Calculate the checksum for command access, and set CheckSum, as it's command usage,
			  one word data is always 3 bytes.
			  
Parameters	:
			wordNum : the parameter number + 1 command header
		
Return Value	: None		
*************************************************************************************/
static void Star_SetBuffer_CheckSum(u8 wordNum)
{
	U32  checkSum = Star_Calculate_CheckSum(wordNum);
	
	SET_BUFFER_WITH_U32(Radio.DataBuff + 3 * wordNum, checkSum);
}

/*************************************************************************************
Function		: Star_Read_Bytes
Description	: Read data from Star TDA7707

Parameters	:
			 eTuner_Access_Mode  accessMode:  Tuner  access mode
				ACCESS_DIRECT,
				ACCESS_COMMAND,
				
			wordNum : the word number which need to be read out from the pAddr and following address.
			pData : the data buffer to get the data from tuner.
		
Return Value	: None		
*************************************************************************************/
void Star_Read_Bytes( eTuner_Access_Mode accessMode, u8 wordNum, u8 *pData)
{
	//int i;
	int byteNum;
	u8 mode;
	
	byteNum = wordNum * ((accessMode == ACCESS_DIRECT)  ? 4 : 3) ;  //Direct access is 32 bits mode, command access is 24 bits mode
	mode = (accessMode == ACCESS_DIRECT) ? MODE_DIRECT_READ : MODE_CMD_READ;
	Read_TDA7708_RegMap( pData, byteNum);
 
}
/*************************************************************************************
Function		: Star_CmdProtocol_WR
Description	: Write data and read data from Star for commands layer

Parameters	:
			outParamNum : the parameter number with the command, not included the command header and checksum
			inParamNum   : the parameter number (in 24bit words) which need read out from Star
						  if it's from 32bits address, the inParamNum should be doubled.
						  
						  if inParamNum == CMD_READIN_ANSWER_HEADER_ONLY, only read in the answer command header
						  if inParamNum == CMD_READIN_PARAM_NUM_FROM_HEADER, read in the answer comand header first, 
						  	 then got the following parameters num from the header and read out.
		
Return Value	: None		
*************************************************************************************/
void Star_CmdProtocol_WR(u8 outParamNum, u8 inParamNum)
{
	//u8 *pAddr = Radio.AddressBuff;
	//u8 *pData = Radio.DataBuff;
	u8   num, cmdID;
	U32 checkSum = 0;
	//int i;

	TunerWaitReady(Radio.TunerChannel);
	
	cmdID = Radio.DataBuff[1];

	SET_BUFFER_WITH_U32(Radio.AddressBuff, ADDRESS_OF_CMD_REG_BUFFER);     //Command address, fixed

	//(parameter number + 1command header + 1 checkSum + 1 address) * 3 bytes
	Star_Write(ACCESS_COMMAND, (outParamNum + 3) * 3,  Radio.AddressBuff);    
	Wait10us(9);     // Wait 4 audio circle

	if (inParamNum == CMD_READIN_PARAM_NUM_FROM_HEADER)
	{
		Star_Read(ACCESS_COMMAND, 1, Radio.AddressBuff, Radio.DataBuff);
		num = Radio.DataBuff[2];
		//Star_Read(ACCESS_COMMAND, num+2, Radio.AddressBuff, Radio.DataBuff);
		Star_Read_Bytes( ACCESS_COMMAND, num + 1, &Radio.DataBuff[3]);
	}
	else
	{
		if (inParamNum == CMD_READIN_ANSWER_HEADER_ONLY)
		{ 
			//Read in ( 1 answer header , no checksum) words
			Star_Read(ACCESS_COMMAND, 1, Radio.AddressBuff, Radio.DataBuff);   
		}
		else
		{
			//Read in (parameter num + 1 answer header + 1 checksum) words 
			Star_Read(ACCESS_COMMAND, inParamNum + 2, Radio.AddressBuff, Radio.DataBuff);    
		} 
	}

#ifdef CMD_CHECK_ERROR	
	Tuner.TunerError = Radio.DataBuff[0] &0xE0;
	if (cmdID != Radio.DataBuff[1]) Tuner.TunerError |= ERR_CMD_CMD_ID_NOT_SAME;
	if (Tuner.TunerError) return;
	
	if (inParamNum > CMD_READIN_ANSWER_HEADER_ONLY)
	{
		num = Radio.DataBuff[2] * 3;
		checkSum   = (U32)Radio.DataBuff[num + 3] << 16;    //Got the checksum in buffer
		checkSum |= (U32)Radio.DataBuff[num + 4] << 8;
		checkSum |= (U32)Radio.DataBuff[num + 5];
	}
	if (checkSum != Star_Calculate_CheckSum(Radio.DataBuff[2] + 1)) Tuner.TunerError |= ERR_CMD_CHECKSUM_ANSWER;
#endif
}

/*************************************************************************************
Function 		: Star_Cmd_Tuner_Ping
Description	: This command can be used to verify the “health status” of the tuner. 
			The car-radio MCU needs to periodically verify that the tuner is still operating correctly and is not “stuck”
			(for example as a consequence of an ESD to the car-radio in case the layout does not sufficiently protect the tuner): 
			to do so, the present command can be sent with an arbitrary 24 bit word as its parameter. 
			If the tuner is still operating correctly, it replies with a bit-inversed version of the word sent by the MCU.
			
Parameters	:
			checkData, (any value)
			
Return Value	: None
*************************************************************************************/
void Star_Cmd_Tuner_Ping(U32 checkData)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,  0x00, CMD_CODE_TUNER_PING, 1); //Command Header, 1 parameters
	SET_BUFFER_WITH_U32(CMD_PARAM(1), checkData);	 //Parameter 1, checkData
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, 1);		//Out 1 parameters, and  return 1 BIT-INVERSED data of the checkData
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Read
Description	: This command performs a read operation on any memory cell of the TDA7707. 
			 At most 30 consecutive words (DSP memory case) 
			 or 15 words (non-DSP memory case) can be read out.
			 
Parameters	:
			regAddr :  TDA7707 DSP memory Addr or non-DSP memory addr.
			paramNum : parameter number need to read out from regAddr and following address.
			
Return Value	: None			
*************************************************************************************/
void Star_Cmd_Tuner_Read(U32 regAddr, u8 paramNum)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_READ, 2);    //Command Header, 2  parameters
	SET_BUFFER_WITH_U32(CMD_PARAM(1), regAddr);  				 //parameter 1, reg addr   Radio.DataBuff[3, 4,5]
	SET_BUFFER_WITH_U8(CMD_PARAM(2), 0x00, 0x00, paramNum);  	 //parameter 2, data number need to read out  Radio.DataBuff[6, 7, 8]
	SET_BUFFER_CHECKSUM(3);  //Set Checksum to buffer
	
	//Check regAddr bit19, 0: 32bits non DSP mem; 1 : 24bits DSP mem.
	//Out:  2 parameters; In: 2*paramNum if it's 32bits memory.
	Star_CmdProtocol_WR(2, paramNum * (2 -((regAddr >> 19) & 0x01)));
}


/*************************************************************************************
Function		: Star_Cmd_Tuner_Write
Description	: This command performs a write operation to any memory cell. 
			At most 29 consecutive words (DSP memory case) or 14 words (non-DPS memory case) can be written.
			
Parameters	:
			 regAddr :  TDA7707 DSP memory Addr or non-DSP memory addr.
			 pData   :  the data buffer pointer, which need write to tuner.
			 paramNum : parameter number.
			 
Return Value	: None			
*************************************************************************************/
void Star_Cmd_Tuner_Write(U32 regAddr, const U32* pData, u8 paramNum)
{
	u8   wordNum = 1;
	U32 dataTemp;
	int i;
	
	SET_BUFFER_WITH_U32(CMD_PARAM(1), regAddr); //parameter 1,	reg addr
	if ((regAddr >> 19) & 0x01)   // DSP memory, 24 bits
	{
		for (i = 0; i < paramNum; i++)
		{
			dataTemp = *(pData + i);
			SET_BUFFER_WITH_U32(CMD_PARAM(2 +  i), dataTemp);
		}
		wordNum += paramNum;
	}
	else   // non-DSP memory, 32bits
	{
		for (i = 0; i < paramNum; i++)
		{	
			dataTemp = *(pData + i);
			SET_BUFFER_WITH_U32(CMD_PARAM(2 + 2 * i), (dataTemp >> 8) ); //two bytes @MSB
			SET_BUFFER_WITH_U32(CMD_PARAM(3 + 2 * i), dataTemp );   	//two bytes @LSB
		}
		wordNum += paramNum  * 2;
	}
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_WRITE, wordNum); //Command Header
	SET_BUFFER_CHECKSUM(wordNum + 1);  //Set Checksum to buffer

	Star_CmdProtocol_WR(wordNum, CMD_READIN_ANSWER_HEADER_ONLY);  //Only read in the answer header
}


/*************************************************************************************
Function		: Star_Cmd_Tuner_Change_Band
Description 	: This command executes the entire band change procedure on a specified channel.
			Additionally specific processing features will be activated (e.g. VPA).
			For DAB and DRM the corresponding digital base band interface is activated when this command is invoked;
			for HD radio (both AM and FM) the digital base band activation, as well as the blend operation, depends on the value of parameter 3.
			This command additionally sets the band limits and step for Seek operations.
			
Parameters	:
		tunerChannel :
			eTuner_Channel_Main   		:  foreground channel
			eTuner_Channel_Background  	:  background channel
			eTuner_Channel_Both   		:  both channel
			
		tunerMode :			
			0x000000: Standby (no reception)
			0x000001: FM
			0x000002: Weather Band (WX)
			0x000003: DAB Band 3
			0x000004: DAB L-Band
			0x000005: AM EU
			0x000006: AM US
			0x000007: DRM30
			0x000008: DRM+
			0x000009: not used
			0x00000A: not used
			0x00000B: not used
			0x00000C: not used
			0x00000D: not used
			0x00000E: Reserved – do not use
			0x00000F: Reserved – do not use
			
		tunerProcessFeature : 
			Bit [0] : FM VPA enable/disable2		
					0: FM VPA off	1: FM VPA on (FM only)
			Bit [1] : Antenna diversity enable / disable (currently not implemented)
					0: antenna diversity off	1: antenna diversity on (FM foreground channel only)
			Bit [2] : 0
			Bit [3] : 0
			Bit [4] : HD radio digital BB interface and filter enable/disable
					0: off			1: on
			Bit [5] : HD radio onchip blending enable/disable
					0: off			1: on
			Bit [6] : 0
			Bit [7] : 0
			
		minFreq   : Lower band limit [kHz]
		maxFreq  : Upper band limit [kHz]
		seekStep : Seek step [kHz]
		
Return Value	: None		
*************************************************************************************/
//void Star_Cmd_Tuner_Change_Band(eTuner_Channel tunerChannel, eTuner_Mode tunerMode, uTunerProcessFeature tunerProcessFeature, u32 minFreq, u32 maxFreq, u32 seekStep)
void Star_Cmd_Tuner_Change_Band(eTuner_Channel tunerChannel, eTuner_Mode tunerMode, uTunerProcessFeature tunerProcessFeature, U32 minFreq, U32 maxFreq)
{
	u8 paramNum = 2;
	
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);  //Parameter 1, Tuner channel
	SET_BUFFER_WITH_U8(CMD_PARAM(2), 0x00, 0x00, tunerMode);       //parameter 2, Tuner Mode
	
	if (tunerProcessFeature.value != DEFAULT)   //Optional parameters
	{
		SET_BUFFER_WITH_U8(CMD_PARAM(3), 0x00, 0x00, tunerProcessFeature.value & 0xFF);  //parameter 3, Processing features
		paramNum++ ;

		if (minFreq != DEFAULT)
		{
			SET_BUFFER_WITH_U32(CMD_PARAM(4), minFreq); 	//parameter 4, Lower band limit  [kHz]
			paramNum++ ;

			if (maxFreq != DEFAULT)
			{
				SET_BUFFER_WITH_U32(CMD_PARAM(5), maxFreq);  //parameter 5, Upper band limit  [kHz]
				paramNum++ ;

				/*if (seekStep != DEFAULT)
				{
					SET_BUFFER_WITH_U32(CMD_PARAM(6), seekStep);  //parameter 6, Seek step  [kHz]
					paramNum++ ;
				}*/
			}
		}
	}
	
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_CHANGE_BAND, paramNum);	//Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}


/*************************************************************************************
Function		: Star_Cmd_Tuner_Tune
Description	: This command tunes the corresponding channel to the specified frequency. 
			For AM/FM bands it additionally performs muting and unmuting (foreground tuner only) 
			and initializing weak signal processing and quality detectors.
			
Parameters	:
		tunerChannel :
			eTuner_Channel_Main   		:  foreground channel
			eTuner_Channel_Background:  background channel
			eTuner_Channel_Both   		:  both channel
			
		freq : Frequency [kHz]
		injectionSide : Injection side, 	[optional]
			INJECTION_SIDE_AUTO 	0x00
			INJECTION_SIDE_LOW		0x01   //Force low injection side, exception table ignored (not applicable for DAB bands)
			INJECTION_SIDE_HIGH 	0x02   //Force high injection side, exception table ignored (not applicable for DAB bands)

		DRMFreqOffset : DRM Frequency Offset  (In kHz);		When send to tuner, it will x 4 [kHz]
		
Return Value	: None			
*************************************************************************************/
void Star_Cmd_Tuner_Tune(eTuner_Channel tunerChannel, U32 freq, U32 injectionSide, U32 DRMFreqOffset)
{
	u8 paramNum = 2;

	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);  //Parameter 1, Tuner channel
	SET_BUFFER_WITH_U32(CMD_PARAM(2), freq );    				    //parameter 2, frequency  [kHz]
	
	if (injectionSide != DEFAULT) 
	{
		SET_BUFFER_WITH_U8(CMD_PARAM(3), 0x00, 0x00, injectionSide & 0xFF);   //parameter 3, injection side
		paramNum++ ;
		
		if(DRMFreqOffset != DEFAULT)
		{
			DRMFreqOffset *= 4;
			SET_BUFFER_WITH_U32(CMD_PARAM(4), freq );    	 //parameter 4, DRM Frequency Offset   [kHz]
			paramNum++ ;
		} 
	}

	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_TUNE, paramNum);	//Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}


/*************************************************************************************
Function		: Star_Cmd_Tuner_Set_FM_Proc
Description	: For FM reception, switches operation mode dynamically and seamlessly between phase diversity (VPA) and dual tuner.
			To switch to VPA operation, both channels have to be previously set to FM mode and tuned to the same frequency.
			
Parameters 	:
		VPAmode : FM Tuner proc mode,  
			1 : VPA mode	(VPA on)
			0 : dual tuner mode, (VPA off)
			
Return Value	: None			
*************************************************************************************/
void Star_Cmd_Tuner_Set_FM_Proc(u8 VPAmode)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_SET_FM_PROC, 1);	//Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, (VPAmode ? 1 : 0));  //Parameter 1, Tuner FM proc mode
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Get_Reception_Quality
Description	: This command returns the AM/FM/WX quality information of the foreground 
			or background channel during normal reception mode (thus not including Seek nor RDS AF check)
			consisting of: RF fieldstrength, baseband fieldstrength, Detune, Multipath, Adjacent Channel 
			and Stereo/Mono flag (a deviation indication is also returned in addition to the quality information).
			
Parameters	:
		tunerChannel :
			eTuner_Channel_Main   		:  foreground channel
			eTuner_Channel_Background:  background channel
			
Return Value	: None		
*************************************************************************************/
void Star_Cmd_Tuner_Get_Reception_Quality(eTuner_Channel tunerChannel)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,     0x00, CMD_CODE_TUNER_GET_RECEPTION_QUALITY, 1);  //Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);		//Parameter 1, Tuner channel
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, 3);   //Out 1 parameter, and will return 3 parameters
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Get_Channel_Quality
Description	: This command returns the AM/FM/WX quality information of the foreground 
			or background channel during normal reception mode (thus not including Seek nor RDS AF check)
			consisting of: RF fieldstrength, baseband fieldstrength, Detune, Multipath, Adjacent Channel 
			and Stereo/Mono flag (a deviation indication is also returned in addition to the quality information).
			
Parameters	:
		VPAmode : FM Tuner proc mode,  
			1 : VPA mode	(VPA on)
			0 : dual tuner mode, (VPA off)
			
		tunerChannel :
			eTuner_Channel_Main   		:  foreground channel
			eTuner_Channel_Background:  background channel
			
Return Value	: None		
*************************************************************************************/
void Star_Cmd_Tuner_Get_Channel_Quality(u8 VPAmode, eTuner_Channel tunerChannel)
{
	if (VPAmode)
	{
		SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_GET_CHANNEL_QUALITY, 0);  //Command Header
		SET_BUFFER_CHECKSUM(1);  //Set Checksum to buffer
		Star_CmdProtocol_WR(0, 6);   //Out 1 parameter, and will return 6 parameters
	}
	else
	{
		SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_GET_CHANNEL_QUALITY, 1);  //Command Header
		SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);		//Parameter 1, Tuner channel
		SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
		Star_CmdProtocol_WR(1, 3);   //Out 1 parameter, and will return 3 parameters
	}
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Audio_Mute
Description	: This command mutes and unmutes the audio outputs
			
Parameters	:
			muteAction :
				0x000000: mute audio
				0x000003: unmute audio
Return Value	: None		
*************************************************************************************/
void Star_Cmd_Tuner_Audio_Mute(eMuteAction muteAction)
{	
	SET_BUFFER_WITH_U8(CMD_HEADER,  0x00, CMD_CODE_TUNER_MUTE, 1); //Command Header, 1 parameters
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, muteAction);	//Parameter 1, Tuner channel
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_ANSWER_HEADER_ONLY);	//Out 1 parameters, and only return the answer header.
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_FM_Stereo_Mode
Description	: This command is able to force the mono or stereo operation mode of the tuner when in FM mode
			
Parameters	:
			stereoMode :
				0x000000: auto mono-stereo reception
				0x000001: force mono reception
				(0x000002: force stereo reception, not used)
Return Value	: None
*************************************************************************************/
void Star_Cmd_Tuner_FM_Stereo_Mode(eStereoMode forceMono)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,  0x00, CMD_CODE_TUNER_FM_STEREO_MODE, 1); //Command Header, 1 parameters
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, forceMono);	//Parameter 1, Tuner channel
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_ANSWER_HEADER_ONLY);	//Out 1 parameters, and only return the answer header.
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Set_VICS_Output
Description	: This command enables the VICS signal output as FSK modulated, 76 kHz-bandpass filtered MPX signal from pin IRQ (pin 50).
			
Parameters	:
			tunerChannel :  tuner Channel
				0x000000: VICS output disabled
				0x000001: VICS output enabled from foreground channel
				0x000002: VICS output enabled from background channel
				
Return Value	: None						
*************************************************************************************/
void Star_Cmd_Tuner_Set_VICS_Output(eTuner_Channel tunerChannel)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_SET_VICS_OUTPUT, 1); //Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, tunerChannel
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_ANSWER_HEADER_ONLY);	  //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_BB_IF_On_Off
Description 	: This command is used to turn the digital base band interface on and off 
			(after setting its operation mode through commands 0x04, 0x05 and 0x06).
			
Parameters	:
			IFEnable
				0x000000: interface disabled
				0x000001: interface enabled
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_BB_IF_On_Off(u8 IFEnable)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_DEVICE_BB_IF_ON_OFF, 1); //Command Header
	SET_BUFFER_WITH_U32(CMD_PARAM(1), (IFEnable ? 1 : 0));	//Parameter 1, IFEnable
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_ANSWER_HEADER_ONLY);	  //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_BB_IF_On_Off
Description 	: This command sets the baseband processing mode and configures various base band processing test modes. 
			The command is documented only as far as the part needed to perform XTAL alignment is concerned.
			
			The XTAL alignment procedure is described in user AN 7 - XTAL alignment. 
			After the operations described in said chapter have been completed, 
			the tuner needs to be reset and undergo the full start-up procedure again to revert to normal operating mode.
			Command Device_Set_BB_Proc (0x30) cannot be used for this.
			
Parameters	: None
			Parameters are fixed
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_Set_BB_Proc(void)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_DEVICE_Set_BB_PROC, 2); //Command Header
	SET_BUFFER_WITH_U32(CMD_PARAM(1), 0x000001);	//Parameter 1,
	SET_BUFFER_WITH_U32(CMD_PARAM(2), 0x000010);	//Parameter 1,	enter XTAL alignment mode
	SET_BUFFER_CHECKSUM(3);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(2, CMD_READIN_ANSWER_HEADER_ONLY);	  //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_RDSBuffer_Set
Description	: This command is used to set the RDS buffer configuration.
			The command flushes the RDS buffer, configures the CSR (Control Status Register) 
			and the NBR (Number Block Register). The NBR register contains the number of collected RDS blocks (NRQST)
			and the number of blocks containing PI information (NumPI), which trigger a notification to the MCU 
			either via interrupt or via flag polling. The buffer supports a maximum of 20 RDS blocks (total buffer length of 22).
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
			CSR : CSR (Control Status Register)
			NBR : NBR (Number Block Register)
			
Return Value	: None		
*************************************************************************************/
void Star_Cmd_Tuner_RDSBuffer_Set(eTuner_Channel tunerChannel, U32 CSR, U32 NBR)
{	
	SET_BUFFER_WITH_U8  (CMD_HEADER, 	0x00, CMD_CODE_TUNER_RDSBUFFER_SET, 3); //Command Header, 3 parameters
	SET_BUFFER_WITH_U8  (CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, Tuner channel
	SET_BUFFER_WITH_U32(CMD_PARAM(2), CSR);  //Parameter 2, CSR
	SET_BUFFER_WITH_U32(CMD_PARAM(3), NBR); //Parameter 3, NBR
	SET_BUFFER_CHECKSUM(4);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(3, CMD_READIN_ANSWER_HEADER_ONLY);	//Out 3 parameters, and only return the answer header.
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_RDSBuffer_Read
Description	: This command returns the collected RDS blocks and clears the RDS buffer of a specific RDS module. 
			If this command is sent without parameters, then the RDS data are provided from the module 
			which generated the RDS buffer ready interrupt. In case the RDS buffer is empty the RDS Read 
			Notification Register is the only response.
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
				
Return Value	: None			
*************************************************************************************/
void Star_Cmd_Tuner_RDSBuffer_Read(eTuner_Channel tunerChannel)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,     0x00, CMD_CODE_TUNER_RDSBUFFER_READ, 1);   	//Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);					//Parameter 1, Tuner channel
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_PARAM_NUM_FROM_HEADER);
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_AF_Start
Description	: This command is used to start the verification of the quality of a series of AF frequencies. 
			It is always terminated by a command Tuner_AF_End (0x21).
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
			alterFreq : Alternative frequency (kHz)
			antSelection : Antenna selection for VPA mode (meaningless if not in VPA mode)

Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_AF_Start (eTuner_Channel tunerChannel, U32 alterFreq, U32 antSelection)
{
	u8 paramNum = 1;

	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, Tuner channel

	if (alterFreq != DEFAULT)
	{
		SET_BUFFER_WITH_U32(CMD_PARAM(2), alterFreq);   //Parameter 2, alterFreq
		paramNum++;
		
		if (antSelection != DEFAULT)
		{
			SET_BUFFER_WITH_U32(CMD_PARAM(3), antSelection);  //Parameter 3, antSelection
			paramNum++;
		}
	}
	
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_AF_START, paramNum); //Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, 3);   //Read in 3 parameters
}


/*************************************************************************************
Function		: Star_Cmd_Tuner_AF_End
Description	: This command is used to stop the verification of the quality of a series of AF frequencies 
			previously initiated with command Tuner_AF_Start (0x20). 
			The commands can be used both to stop the sequence and to abort it. The tuner is restored to 
			the configuration (single channel FM mode or VPA mode) it had prior to the activation of the AF start sequence.
			The frequency at which reception must be restored can be chosen between the previous reception frequency, 
			the latest checked AF frequency or any other specified frequency.
			
			In the command answer the command provides the AF quality of last measured AF frequency.
			Additionally this command provides timing information to a companion processor IC about its current state 
			via the status bits, in case the I2S or JESD204 interfaces are activated in the corresponding mode.

Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
				
			freqAfterAFEnd :Reception frequency after AF end (kHz)
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_AF_End(eTuner_Channel tunerChannel, U32 freqAfterAFEnd)
{
	SET_BUFFER_WITH_U8  (CMD_HEADER,     0x00, CMD_CODE_TUNER_AF_END, 2); //Command Header
	SET_BUFFER_WITH_U8  (CMD_PARAM(1), 0x00, 0x00, tunerChannel);		//Parameter 1, Tuner channel
	SET_BUFFER_WITH_U32(CMD_PARAM(2), freqAfterAFEnd);  			//Parameter 2, freqAfterAFEnd
	SET_BUFFER_CHECKSUM(3);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(2, 3);   //Read in 3 parameters
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_AF_Check
Description	: This command is used to perform a fast verification of the quality of one AF frequency.

Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
			alterFreq : Alternative frequency (kHz)
			antSelection : Antenna selection for VPA mode (meaningless if not in VPA mode)
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_AF_Check(eTuner_Channel tunerChannel, U32 alterFreq, U32 antSelection)
{
	u8 paramNum = 2;

	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, Tuner channel
	SET_BUFFER_WITH_U32(CMD_PARAM(2), alterFreq);					//Parameter 2, alterFreq

	if (antSelection != DEFAULT)   //Optional parameter
	{
		SET_BUFFER_WITH_U32(CMD_PARAM(3),  antSelection);  			//Parameter 3, antSelection
		paramNum++;
	}
	
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_AF_CHECK, paramNum); //Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer

	Star_CmdProtocol_WR(paramNum, 3);   //Discards the data, as it's useless in normal cases.
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_AF_Switch
Description	: This command is used to perform a fast tuning operation to an AF frequency. 
			 At the end of the operation, the tuner goes into normal reception mode on said frequency. 
			 This command furthermore resets the RDS buffer.
			 
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
			alterFreq : Alternative frequency (kHz)
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_AF_Switch(eTuner_Channel tunerChannel, U32 alterFreq)
{
	u8 paramNum = 2;
	
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, Tuner channel
	SET_BUFFER_WITH_U32(CMD_PARAM(2), alterFreq);				//Parameter 2, alterFreq

	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_AF_SWITCH, paramNum); //Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Get_AF_Quality
Description	: This command returns the FM quality information of the foreground or background channel 
			as a result of an RDS AF check operation. The quality information consists of:
			RF fieldstrength, baseband fieldstrength, Detune, Multipath, Adjacent Channel.
			 
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
				
Return Value	: None						
*************************************************************************************/
void Star_Cmd_Tuner_Get_AF_Quality(eTuner_Channel tunerChannel)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,     0x00, CMD_CODE_TUNER_GET_AF_QUALITY, 1); //Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);		//Parameter 1, Tuner channel
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, 3);   //Read in 3 parameters
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Seek_Start
Description	: This command initiates a full automatic or manual seek procedure including quality measurement 
			and evaluation (automatic mode).
			In automatic mode the command executes consecutive seek steps according to either the band configuration
			set by command Tuner_Change_Band (0x0A) or to the content of the optional parameter 3. 
			It stops either after a full band scan cycle has been performed or if a good station has been found 
			according to the selected thresholds. This command can also be used to abort the Seek procedure, 
			if issued while automatic seek is running and with bit 2 of parameter 2 set to 0.
			
			In manual mode the command executes one seek step according to either the band configuration set 
			by command Tuner_Change_Band (0x0A) or to the content of the optional parameter 3. 
			In manual mode parameter 2 has to be sent only for the first seek step of a new seek cycle. 
			Consecutive seek steps are executed upon sending this command again, only with parameter 1 
			using the previously sent configuration.
			
			In case of FM VPA configuration this command stops reception and does not switch the system to double tuner mode.
			In case of DAB Band 3 or L-Band, if parameter 3 is bigger than 100000 kHz, the command assumes 
			that the next seek frequency has been entered and tunes the next channel frequency, 
			not the frequency indicated by parameter 3.
			
			Supported bands: FM, AM, WX, DAB (manual mode only).
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
			seekConfig :
			seekStep :   seek step [kHz] (allowed for all bands) or
						seek frequency [kHz] (for DAB B3 and DAB L-band)
						
Return Value	: None								
*************************************************************************************/
void Star_Cmd_Tuner_Seek_Start (eTuner_Channel tunerChannel, U32 seekConfig, U32 seekStep)
{
	u8 paramNum = 1;
	
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, Tuner channel
	
	//For manual seek, the first command start must have seekConfig, 
	//but not required for subsequent issues of command 0x26 in manual mode.
	if (seekConfig != DEFAULT)   //Optional parameters
	{
		SET_BUFFER_WITH_U32(CMD_PARAM(2), seekConfig);	//Parameter 2, seekConfig
		paramNum++;
		
		if (seekStep != DEFAULT)
		{
			SET_BUFFER_WITH_U32(CMD_PARAM(3), seekStep);	//Parameter 3, seekStep
			paramNum++;
		}
	}
	
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_SEEK_START, paramNum); //Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Seek_End
Description	: This command can be either used to leave Seek mode when a frequency with an acceptable quality is found, 
			or to terminate automatic seek mode.
			The command also resets the RDS buffer.
			Supported bands: FM, AM, WX, DAB (manual mode only)
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
				
			SeekEndAction :	 Seek end audio status
				0x000000: exit seek and unmute (default)
				0x000001: exit seek and keep audio muted
				
Return Value	: None						
*************************************************************************************/
void Star_Cmd_Tuner_Seek_End (eTuner_Channel tunerChannel, U32 SeekEndAction)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,     0x00, CMD_CODE_TUNER_SEEK_END, 2); 	//Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);			   	//Parameter 1, Tuner channel
	SET_BUFFER_WITH_U8(CMD_PARAM(2), 0x00, 0x00, SeekEndAction & 0x01);   	//Parameter 2, SeekEndAction
	SET_BUFFER_CHECKSUM(3);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(2, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Get_Seek_Status
Description	: This command returns the status of a currently active seek operation together with 
			quality information of the most recently tested frequency.
			The quality information consists of:
				RF fieldstrength, baseband fieldstrength, Detune, Multipath, Adjacent Channel and 
				Stereo/Mono flag (a deviation indication is also returned in addition to the quality information).
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main  	 	:  foreground channel
				eTuner_Channel_Background:  background channel
				
Return Value	: None
*************************************************************************************/
void Star_Cmd_Tuner_Get_Seek_Status(eTuner_Channel tunerChannel)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_GET_SEEK_STATUS, 1); //Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, Tuner channel
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, 4);	//Read in 3 parameters
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Set_Seek_Threshold
Description	: This command sets the thresholds for automatic seek stop for FM, AM and WX band.
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel
			fstBBTH : threshold on fstBB (base band field strenth)
			detTH : threshold on det (detuning)
			adjTH :  threshold on adj (adjacent channel)
			mpTH : threshold on mp (multipath)
			snrTH : threshold on SNR
			mpxTH : threshold on MPX noise
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_Set_Seek_Threshold(eTuner_Channel tunerChannel, u8 fstBBTH, u8 detTH, u8 adjTH, u8 mpTH, u8 snrTH, u8 mpxTH, u8 coChTH)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,     0x00, CMD_CODE_TUNER_SET_SEEK_TRESHOLD, 4); //Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, tunerChannel);	//Parameter 1, Tuner channel
	SET_BUFFER_WITH_U8(CMD_PARAM(2), 0x00, fstBBTH, detTH);	 	//Parameter 2, Seek threshold
	SET_BUFFER_WITH_U8(CMD_PARAM(3), mpTH, mpxTH, snrTH);   		//Parameter 3, seek threshold
	SET_BUFFER_WITH_U8(CMD_PARAM(4), adjTH, coChTH, 0x00);   		//Parameter 4, seek threshold
	SET_BUFFER_CHECKSUM(5);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(4, CMD_READIN_ANSWER_HEADER_ONLY);     //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Set_Blend
Description	: This command sets the audio blend operation mode for digital reception standards in case the I2S audio 
			from an external digital decoder is brought back into the TDA7707. In case of HD reception, 
			the output from the audio blend block is fed outside in an analog format through the on-board DACs and in digital format via I2S. 
			In case of any other standards (DAB and DRM) the output from the audio blend block is fed outside only in an analog format through the on-board DACs.
			The command sets the way audio is selected.
			
Parameters	:
			blendMode :  Blend mode
				00: automatic via GPIO
				01: force analog signal
				10:force digital source input (HD)
				11: force digital source input (non-HD)
				
Return Value	: None						
*************************************************************************************/
void Star_Cmd_Tuner_Set_Blend (u8 blendMode)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,     0x00, CMD_CODE_TUNER_SET_HD_BLEND, 1); //Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, blendMode);	//Parameter 1, blendMode
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_ANSWER_HEADER_ONLY);	  //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Conf_HD_Blend
Description 	: This command sets the HD radio blend parameters
			
Parameters	:
			T_AD_gain_step :  parameter to program analog-to-digital blend time.
						T_AD_gain_step [fractional] = 21.93us/T_AD, 
						where T_AD is the analog-to-digital blend time in seconds
			T_DA_gain_step: parameter to program digital-to-analog blend time.
						T_DA_gain_step [fractional] = 21.93us/T_DA, 
						where T_DA is the digital-to-analog blend time in seconds
			AV_gain: analog signal level gain (fractional)					
			DV_gain: digital signal level gain (fractional)
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_Conf_HD_Blend(U32 T_AD_gain_step, U32 T_DA_gain_step, U32 AV_gain, U32 DV_gain)
{
	SET_BUFFER_WITH_U8  (CMD_HEADER,	0x00, CMD_CODE_TUNER_CONF_HD_BLEND, 4); //Command Header
	SET_BUFFER_WITH_U32(CMD_PARAM(1), T_AD_gain_step);  	//Parameter 1, T_AD_gain_step
	SET_BUFFER_WITH_U32(CMD_PARAM(2), T_DA_gain_step);  	//Parameter 2, T_DA_gain_step
	SET_BUFFER_WITH_U32(CMD_PARAM(3), AV_gain);   			//Parameter 3, AV_gain
	SET_BUFFER_WITH_U32(CMD_PARAM(4), DV_gain);  			//Parameter 4, DV_gain	
	SET_BUFFER_CHECKSUM(5);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(4, CMD_READIN_ANSWER_HEADER_ONLY);	  //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Set_Audio_IF
Description	: This command controls the audio DAC and audio SAI input and output channel.
			
Parameters	:
			SAIMode :  Audio SAI config
				Bit [2] : audio SAI input enable/disable	0: off	1: on
				Bit [1] : audio SAI output enable/disable	0: off	1: on
				Bit [0] : audio DAC enable/disable		0: off	1: on
				
			SAIConfig: 	
				Bit [10] : SAI Word Select Configuration
					0: SAI WS is 50% duty cycle (default)
					1: SAI WS is a single pulse signal
				Bit [9] : 	SAI Audio Data Sign Extension
					0: SAI Audio Data Sign is not extended to MSB, filled with zeros
					1: SAI Audio Data Sign is extended to MSB (default)
				Bit [8] : SAI Audio WS Polarity
					0: SAI Audio WS Polarity is high (1 Right / 0 Left)
					1: SAI Audio WS Polarity is low (0 Right / 1 Left) (default)
				Bit [7] : SAI Audio Revert Data Sequence
					0: SAI Audio Data is not reverted (not twizzeld) (default)
					1: SAI Audio Data is reverted (twizzeld)
				Bit [6] : SAI Audio Data Delay
					0: SAI Audio Data is delayed vs. WS edge (I2S mode) (default)
					1: SAI Audio Data is not delayed vs. WS edge
				Bit [5] : SAI Audio Data Length
					0: SAI Audio Data length is 16L + 16R
					1: SAI Audio Data length is 32L + 32R (default)
				Bit [4] : SAI Audio Data Clock Polarity
					0: SAI Audio Clock is not inverted
					1: SAI Audio Clock is inverted (default)
				Bit [3:0] : 0x0
			
Return Value	: None						
*************************************************************************************/
void Star_Cmd_Tuner_Set_Audio_IF(u8 SAIMode, U32 SAIConfig)
{
	SET_BUFFER_WITH_U8(CMD_HEADER,     0x00, CMD_CODE_TUNER_SET_AUDIO_IF, 2); //Command Header
	SET_BUFFER_WITH_U8(CMD_PARAM(1), 0x00, 0x00, SAIMode);	//Parameter 1, audioSAI
	SET_BUFFER_WITH_U32(CMD_PARAM(2), SAIConfig);	//Parameter 2, SAIConfig
	SET_BUFFER_CHECKSUM(3);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(2, CMD_READIN_ANSWER_HEADER_ONLY);	  //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Set_BB_IF
Description	: This command configures the digital Base Band interface. Only one type of BB interface 
			can be activated at any given time.
			
Parameters	:
			BBConfig :	Baseband interface configuration
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_Set_BB_IF(U32 BBConfig)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_SET_BB_IF, 1); //Command Header
	SET_BUFFER_WITH_U32(CMD_PARAM(1), BBConfig);	//Parameter 1, BBConfig
	SET_BUFFER_CHECKSUM(2);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(1, CMD_READIN_ANSWER_HEADER_ONLY);	  //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Conf_BB_SAI
Description	: This command configures the baseband SAI interface.
			The SAI is configured either in MUX mode or AFE mode.
				I and Q data length is 16bit each. Data alignment in one frame is:
				Without status bits: I(16) + Q(16)
				With status bits: I(16) + S1(16) + Q(16) + S2(16)
			MUX mode:
				Symmetrical word clock - one sample delayed (I2S mode), start of new frame at falling edge
			AFE mode:
				Word clock is a one-bit clock burst, start of new frame at falling edge
				The content of the status bits is described in Table 7.
			This command must be issued before the Tuner_Set_BB_IF (0x04) command.
			
Parameters	:
			mode  :	SAI mode
			config : 	Configuration
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_Conf_BB_SAI(U32 mode, U32 config)
{
	u8 paramNum = 1;
	
	SET_BUFFER_WITH_U32(CMD_PARAM(1), mode);  //Parameter 1, mode
	
	if (config != DEFAULT)  //Optional parameter
	{
		SET_BUFFER_WITH_U32(CMD_PARAM(2), config); //Parameter 2, config
		paramNum++;
	}
	
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_CONF_BB_SAI, paramNum);  //Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Conf_JESD204
Description	: This command configures the JESD204 baseband interface.
			Note: Configuration should be changed with “0 Logic” on SYNCH input. 
			        Test id Changing is considered a new configuration. 
			         This command must be issued before the Tuner_Set_BB_IF (0x04) command.
			         
Parameters	:
			mode  : mode
				0x000000: DAB mode: FM 1024 kHz
				0x000001: DAB mode: FM 512 kHz
				0x000002: HD mode
				0x000003: Test Mode
			config :	Configuration
			testNumber : test number
			testChar : Char to be sent as test char
			paramILAM : ILA M parameter
			paramILAK : ILA K parameter
			
Return Value	: None		
*************************************************************************************/
void Star_Cmd_Tuner_Conf_JESD204(U32 mode, U32 config, U32 testNumber, U32 testChar, U32 paramILAM, U32 paramILAK)
{
	u8 paramNum = 2;
	
	SET_BUFFER_WITH_U32(CMD_PARAM(1), mode); 	//Parameter 1, mode
	SET_BUFFER_WITH_U32(CMD_PARAM(2), config); 	//Parameter 2, config
	
	if (testNumber != DEFAULT)  //Optional parameters
	{
		SET_BUFFER_WITH_U32(CMD_PARAM(3),  testNumber);	 //Parameter 3, testNumber
		paramNum++; 
		
		if (testChar != DEFAULT)
		{
			SET_BUFFER_WITH_U32(CMD_PARAM(4),  testChar);  //Parameter 4, testChar
			paramNum++; 
			
			if (paramILAM != DEFAULT)
			{
				SET_BUFFER_WITH_U32(CMD_PARAM(5),  paramILAM);	  //Parameter 5, paramILAM
				paramNum++; 

				if (paramILAK != DEFAULT)
				{
					SET_BUFFER_WITH_U32(CMD_PARAM(6),  paramILAK);	 //Parameter 6, paramILAK
					paramNum++; 
				}
			}
		}
	}
	
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_TUNER_CONF_JESD204, paramNum);  //Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);   //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Set_DISS
Description	: This command is used to force the DISS status for debugging and fine-tuning operations. 
			It must not be used in the normal course of operations.
			The command is valid for FM only.
			
Parameters	:
			tunerChannel :
				eTuner_Channel_Main 		:  foreground channel
				eTuner_Channel_Background:  background channel
				eTuner_Channel_Both		:  Both
			mode:
				0x000000: automatic control on foreground channel, no VPA
				0x000001: manual control, no detectors operating
				0x000002: manual control, detectors operating (this mode must be used for foreground channel to perform DISS parameter setting)
				0x000003: automatic control, VPA mode
			filterIndex : 0x000000 .. 0x000009: index of filter to be forced in manual mode
			
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_Set_DISS(eTuner_Channel tunerChannel, U32 mode, U32 filterIndex)
{
	u8 paramNum = 2;
	
	SET_BUFFER_WITH_U8	(CMD_PARAM(1), 0x00, 0x00, tunerChannel);  //Parameter 1, Tuner channel
	SET_BUFFER_WITH_U32	(CMD_PARAM(2),  mode); 	//Parameter 2, mode

	if (filterIndex != DEFAULT)	//Optional parameters
	{
		SET_BUFFER_WITH_U32(CMD_PARAM(3),  filterIndex);	//Parameter 3, filterIndex
		paramNum++; 
	}
	
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_DEVICE_SET_DISS, paramNum); //Command Header
	SET_BUFFER_CHECKSUM(paramNum + 1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(paramNum, CMD_READIN_ANSWER_HEADER_ONLY);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_Cmd_Tuner_Get_WSP_Status
Description	: This command is used to return the status of the FM weak signal processing section of the foreground channel 
			and the DISS BW of both foreground and background channels. It must not be used in the normal course of operations.
			The command is valid for AM and FM only.
			
Parameters	: None
Return Value	: None					
*************************************************************************************/
void Star_Cmd_Tuner_Get_WSP_Status(void)
{
	SET_BUFFER_WITH_U8(CMD_HEADER, 0x00, CMD_CODE_DEVICE_GET_WSP_STATUS, 0); //Command Header
	SET_BUFFER_CHECKSUM(1);  //Set Checksum to buffer
	
	Star_CmdProtocol_WR(0, CMD_READIN_PARAM_NUM_FROM_HEADER);   //Only read in the answer header
}

/*************************************************************************************
Function		: Star_VPA_Switch
Description	: Switch VPA on/off.

Parameters	: 
			VPAmode : 
				ON		1
				OFF 		0
			
Return Value	: None 		
*************************************************************************************/
void Star_VPA_Switch(u8 VPAmode)
{
	Star_Cmd_Tuner_Set_FM_Proc(VPAmode); 
}
	
/*************************************************************************************
Function		: Star_ReadTunerStatus
Description	: Get the tuner quality information and the stereo indicator command

Parameters	:
			band: indicate the band 
Return Value	: None
*************************************************************************************/
void Star_ReadTunerStatus(u8 band)
{
	Star_Cmd_Tuner_Get_Reception_Quality(eTuner_Channel_Main);

	if (band == FM) 
	{
		Radio.Smeter	= ((Radio.DataBuff[4] & 0x80) ? Radio.DataBuff[4] - 256 : Radio.DataBuff[4]);//fstBB 
		Radio.Detuning	= Radio.DataBuff[5];	//detSlow  
		Radio.Multipath = Radio.DataBuff[6];	//Mp  
		Radio.MpxNoise	= Radio.DataBuff[7];	//MpxNoise
		Radio.SNR		= Radio.DataBuff[8];	//SNR 
		Radio.AdjChannel= ((Radio.DataBuff[9] & 0x80) ? Radio.DataBuff[9] - 256 : Radio.DataBuff[9]);//Adj
		Radio.Deviation = Radio.DataBuff[11] & 0xFE;	//Deviation
		Radio.F_Stereo	= Radio.DataBuff[11] & 0x01;	//Stereo

		Star_Cmd_Tuner_Get_WSP_Status();
		Radio.IFBW	=	Radio.DataBuff[3]&0x0F; //fg diss BW		
	}
	else  //AM WX
	{
		Radio.Smeter	= ((Radio.DataBuff[4] & 0x80) ? Radio.DataBuff[4] - 256 : Radio.DataBuff[4]);//fstBB 
		Radio.Detuning	= Radio.DataBuff[5];   //Detuning
		Radio.AdjChannel	= ((Radio.DataBuff[9] & 0x80) ? Radio.DataBuff[9] - 256 : Radio.DataBuff[9]);//Adj
	}
}

/*************************************************************************************
Function		: Star_GetAverageQuality
Description	: Get the reception average quality, AverageNum <256

Paramters	: 
			averageNum : the average num to calculate the quality
			
Return Value	: None
*************************************************************************************/
void Star_GetAverageQuality (u8 averageNum, uint8  bandReq , tda7708_status_t *status)
{
	int qualSmeter = 0;
	int qualAdj = 0;
	U32 qualDT = 0;
	U32 qualMP = 0;
	U32 qualDev = 0;
	U32 qualMPX = 0;
	U32 qualIFBW =0;
	U32 qualSNR =0;
	int i; 

	if(bandReq < TDA7708_BAND_AM)//FM
	{
		for(i = 0; i < averageNum; i++)
		{
			Star_ReadTunerStatus(FM);
			qualSmeter += Radio.Smeter;
			qualAdj += Radio.AdjChannel;
			qualDT += Radio.Detuning;
			qualMP += Radio.Multipath;
			qualDev += Radio.Deviation;  
			qualMPX += Radio.MpxNoise;
			qualIFBW += Radio.IFBW;
			qualSNR += Radio.SNR;
			Wait10us(5);
		}
		if(qualSmeter)
		Radio_detect.Smeter     = (qualSmeter / averageNum);
		if(qualAdj)
		Radio_detect.AdjChannel	= (qualAdj / averageNum);
		if(qualDT)
		Radio_detect.Detuning	= (qualDT / averageNum);
		if(qualMP)
		Radio_detect.Multipath  = (qualMP / averageNum);
		if(qualDev)
		Radio_detect.Deviation  = (qualDev / averageNum);
		if(qualMPX)
		Radio_detect.MpxNoise	= (qualMPX / averageNum);
		if(qualIFBW)
		Radio_detect.IFBW       = (qualIFBW / averageNum);
		if(qualSNR)
		Radio_detect.SNR	    = (qualSNR / averageNum);
	}
	else    // AM, WX
	{
		Star_ReadTunerStatus(AM);	//for WX, AM, not FM
		for(i = 0; i < averageNum; i++)
		{
			Star_ReadTunerStatus(AM);   //for WX, AM, not FM
			qualSmeter += Radio.Smeter;
			qualDT += Radio.Detuning;
			qualAdj += Radio.AdjChannel;
			Wait10us(5);
		}
		if(qualSmeter)
		Radio_detect.Smeter     = qualSmeter / averageNum;
		if(qualDT)
		Radio_detect.Detuning   = qualDT / averageNum;
		if(qualAdj)
		Radio_detect.AdjChannel = qualAdj / averageNum;
	}
}

/*************************************************************************************
Function		: Star_StartManualSeek
Description	: It's used to manual seek(A decrement can be achieved by sending a negative increment step parameter.)

Parameters	:  
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel

			seekDirection: the direction of seek/ seek up: 0;  seek down : 1;
			seekStep: the seek step in kHz
			  
Return Value	: None
*************************************************************************************/
void Star_StartManualSeek(eTuner_Channel tunerChannel, u8 seekDirection, U32 seekStep )
{
	U32 seekConfig = SEEK_MANUAL;
	
	seekConfig  |=  seekDirection << 1;
	Star_Cmd_Tuner_Seek_Start(tunerChannel, seekConfig, seekStep);
}


/*************************************************************************************
Function		: Star_StartAutoSeek
Description	: The AutoSeek command starts an automatic seek operation
Parameters	: 
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel

			seekDirection : seek up, seek down
			seekEndAction  : after seek end, audio unmuted or muted;
			averageNum  : set the average times to read the quality and caluate average value.
			seekStep       : seek step
			lastSeekFreq  : Last seek frequency (for scan procedures)
					0: set currently tuned frequency as last seek frequency
						(used to start seek)
					1: do not modify frequency saved as last seek frequency (used to continue seek)
Return Value	: None			
*************************************************************************************/
void Star_StartAutoSeek(eTuner_Channel tunerChannel, u8 seekDirection,  u8 seekEndAction, u8 averageNum, U32 seekStep, u8 lastSeekFreq)
{
	U32 seekConfig =  SEEK_AUTO;

	seekConfig |=  seekDirection    << 1;
	seekConfig |=  seekEndAction << 2; 
	seekConfig |=  averageNum    << 4;
	seekConfig |=  lastSeekFreq << 8;
	
	Star_Cmd_Tuner_Seek_Start(tunerChannel, seekConfig, seekStep);
}


/*************************************************************************************
Function		: Star_GetTunerBusyStatus
Description	: Get the named tuner busy status by check regsiter SCSR0
Parameters	: 
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel

Return Value	: 1 : Busy, 0 : Not busy
*************************************************************************************/
u8 Star_GetTunerBusyStatus(eTuner_Channel tunerChannel)
{
	u8 busyStatus;
	u8 AddressBuff[3];
	u8 DataBuff[8];
	
	SET_BUFFER_WITH_U32(AddressBuff, SCSR0_ADDRESS);
	Star_Read(ACCESS_DIRECT, 1, AddressBuff, DataBuff);

	if (tunerChannel == eTuner_Channel_Main)
	{
		busyStatus = DataBuff[2] & 0x01;
	}
	else
	{
		busyStatus = (DataBuff[1]  >> 4 ) & 0x01;
	}

	return busyStatus; 
}


/*************************************************************************************
Function		: TunerInit
Description	: Initialize the pins of MCU which are used for tuner
Parameters	: None
Return Value	: None
*************************************************************************************/
void TunerInit(void)
{
	Tuner.TunerError = 0x00;
	Tuner.TunerErrorCnt = 0;
	
	Tuner.VPAmode = OFF; 


	Tuner.mainFreq = 0x00;
	Tuner.backgroundFreq = 0x00;

	Tuner.rdsConfig[0].CSR = 0x00;		//Main Channel
	Tuner.rdsConfig[0].NBR = 0x00;
	Tuner.rdsConfig[1].CSR = 0x00;		//Background Channel
	Tuner.rdsConfig[1].NBR = 0x00;
}

/*************************************************************************************
Function		: TunerWaitReady
Description	: Wait till tuner is ready, till the SCSR0 busybit was cleared. 
Parameters	: 
			tunerChannel :
				eTuner_Channel_Main   		:  foreground channel
				eTuner_Channel_Background:  background channel

Return Value	: 1 : Busy, 0 : Not busy
*************************************************************************************/
#if 1
void TunerWaitReady(eTuner_Channel tunerChannel)
{
	u8 busyStatus, timeOutCount;
	
	timeOutCount = 2;
	busyStatus = Star_GetTunerBusyStatus(tunerChannel);
	
	while ( (busyStatus == 1) && (timeOutCount > 0))
	{
		Wait10us(10);
		busyStatus = Star_GetTunerBusyStatus(tunerChannel);
		timeOutCount --;
	}
}
#else
void TunerWaitReady(eTuner_Channel tunerChannel)
{
	u8 busyStatus, timeOutCount;
	
	timeOutCount = 50;
	busyStatus = Star_GetTunerBusyStatus(tunerChannel);
	
	while ( (busyStatus == 1) && (timeOutCount > 0))
	{
		Wait1ms(1);
		busyStatus = Star_GetTunerBusyStatus(tunerChannel);		
		timeOutCount --;
	}
}
#endif

/*************************************************************************************
Function		: TunerPreSetting
Description	: Tuner pre-setting for I2C mode before download bootcode
Parameters	: None
Return Value	: None
*************************************************************************************/
void TunerPreSetting(void)
{
	u8 preDataSetting[8] = {0x01, 0x40, 0x08, 0x0, 0x0, 0x0, 0x2, 0};

	/* This step is needed for silicon TDA7707DA and TDA7708BC. It is not needed for new coming silicon, but if present, does not harm the start-up of these silicon versions.  */
	Star_Write(ACCESS_DIRECT, 8,  preDataSetting);  
}

/*************************************************************************************
Function		: TunerPreChecking
Description	: Tuner pre-setting before download bootcode
Parameters	: None
Return Value	: U8 ,  error happens if not 0
*************************************************************************************/
u8 TunerPreChecking(void)
{
	u8 err = 0;
	
	SET_BUFFER_WITH_U32(Radio.AddressBuff, 0x01401E);
	Star_Read(ACCESS_DIRECT, 1, Radio.AddressBuff, Radio.DataBuff);

	//printf("TunerPreChecking:DataBuff[0]:0x%x,DataBuff[1]:0x%x,DataBuff[2]:0x%x,DataBuff[3]:0x%x\n",Radio.DataBuff[0],Radio.DataBuff[1],Radio.DataBuff[2],Radio.DataBuff[3]);
	if ((( Radio.DataBuff[0] == 0xFF) && ( Radio.DataBuff[1] == 0xFF) && ( Radio.DataBuff[2] == 0xFF) && ( Radio.DataBuff[3] == 0xFF) )   ||
	   (( Radio.DataBuff[0] == 0x00) && ( Radio.DataBuff[1] == 0x00) && ( Radio.DataBuff[2] == 0x00) && ( Radio.DataBuff[3] == 0x00) )  )
	{
		err = 1;
	}
	
	return err;
}

/*************************************************************************************
Function		: TunerSubChipChecking
Description	: 
Parameters	: None
Return Value	: U8 ,  error happens if not 0
*************************************************************************************/
u8 TunerSubChipChecking(void)
{
	u8 err = 0;
	
	SET_BUFFER_WITH_U32(Radio.AddressBuff, 0x014022);
	Star_Read(ACCESS_DIRECT, 1, Radio.AddressBuff, Radio.DataBuff);
	
	//printf("TunerSubChipChecking:DataBuff[0]:0x%x,DataBuff[1]:0x%x,DataBuff[2]:0x%x,DataBuff[3]:0x%x\n",Radio.DataBuff[0],Radio.DataBuff[1],Radio.DataBuff[2],Radio.DataBuff[3]);

	if ((( Radio.DataBuff[0] == 0xFF) && ( Radio.DataBuff[1] == 0xFF) && ( Radio.DataBuff[2] == 0xFF) && ( Radio.DataBuff[3] == 0xFF) )   ||
	   (( Radio.DataBuff[0] == 0x00) && ( Radio.DataBuff[1] == 0x00) && ( Radio.DataBuff[2] == 0x00) && ( Radio.DataBuff[3] == 0x00) )  )
	{
		err = 1;
	}
	
	return err;
}

/*************************************************************************************
Function		: TunerCheckChipType
Description	: 
Parameters	: None
Return Value	: None
*************************************************************************************/
u8 TunerCheckChipType(void)
{
	Radio.ChipType = 1;
	if (TunerPreChecking())
	{
		return FALSE;
	}

	Radio.ChipType = 1;
	if(Radio.DataBuff[2] == 'E' && Radio.DataBuff[3] == 'B')//tda7707
	{
		Radio.ChipType = 0;
	}
	else if(Radio.DataBuff[2] == 'D' && Radio.DataBuff[3] == 'A')//tda7707
	{
		Radio.ChipType = 0;
	}
	else if(Radio.DataBuff[2] == 'C' && Radio.DataBuff[3] == 'B')//tda7708CB
	{
		Radio.ChipType = 1;
	}
	else //if(Radio.DataBuff[2] = 'B' && Radio.DataBuff[3] = 'A')
	{
		TunerSubChipChecking();
		if(Radio.DataBuff[3] == 2 ||Radio.DataBuff[3] == 4 || Radio.DataBuff[3] == 7 )//LX
		{
			Radio.ChipType = 2;
		}
		else if(Radio.DataBuff[3] == 3 ||Radio.DataBuff[3] == 6 )//L
		{
			Radio.ChipType = 3;
		}
		else if(Radio.DataBuff[3] == 1)//B
		{
			Radio.ChipType = 4;
		}
	}
	return TRUE;
}

/*************************************************************************************
Function		: TunerChangeBand
Description	: Change band control sequence(change to the specified band and tune to 
			 the specified frequency as well)
			 
Parameters	: 
			tunerChannel :
				eTuner_Channel_Main 	 :	 foreground channel
				eTuner_Channel_Background	:  background channel
				
			bandReq : the band request
			
Return Value	: None 		
*************************************************************************************/
#if 0
void TunerChangeBand(eTuner_Channel tunerChannel,  uint8  bandReq , uint16 min_Freq, uint16 max_Freq, uint16 seek_Step, uint32 current_Freq)
{
	printf("%s, band:%d, max_freq:%d, min_freq:%d, freq:%d, seekStep:%d\n", __FUNCTION__, bandReq, max_Freq, min_Freq, current_Freq, seek_Step);
	u8    seekStep;
	u32  minFreq, maxFreq, currentFreq;
	uTunerProcessFeature tunerProcessFeature;
	
	tunerProcessFeature.value = 0;
	
	Radio.MinFreq = min_Freq;
	Radio.MaxFreq = max_Freq;
	Radio.Freq = current_Freq;
	Radio.SeekStep= seek_Step;
	
	RadioGetMaxMinFreq(bandReq, &maxFreq, &minFreq, &currentFreq, &seekStep);   	// To be compatible with the original ELITE/HIT2 sw.
		
	printf("%s, band:%d, max_freq:%d, min_freq:%d, freq:%d, seekStep:%d\n", __FUNCTION__, bandReq, maxFreq, minFreq, currentFreq, seekStep);
	
	if (bandReq == TDA7708_BAND_FM)
	{
		tunerProcessFeature.value	= (Tuner.VPAmode ? 1 : 0);

		Tuner.Band = eTuner_Mode_FM;
		Star_Cmd_Tuner_Change_Band(eTuner_Channel_Main, Tuner.Band, tunerProcessFeature, minFreq, maxFreq, seekStep);

		Wait1ms(20);   

	#if 1//def AUTOSEEK
		Star_Cmd_Tuner_Set_Seek_Threshold(eTuner_Channel_Main, SEEK_FM_FS_THRESHOLD, SEEK_FM_DT_THRESHOLD, SEEK_FM_ADJ_THRESHOLD, SEEK_FM_MPP_THRESHOLD, SEEK_FM_SNR_THRESHOLD, SEEK_FM_MPXN_THRESHOLD, SEEK_THRESHOLD_NOTUSED);
	#endif

		TunerSetFreq(eTuner_Channel_Main, currentFreq);
	}	
	else if (bandReq == TDA7708_BAND_AM|| bandReq == TDA7708_BAND_SW)	//Change to AM band from other band
	{
		/* set the band's range and tune to the frequency in this band as well */
		if((tda7708_area == TDA7708_AREA_EUROPE)||(tda7708_area == TDA7708_AREA_OIRT)||(tda7708_area == TDA7708_AREA_JAPAN))
		{
			Tuner.Band = eTuner_Mode_AM_EU;
			Star_Cmd_Tuner_Change_Band(eTuner_Channel_Main, Tuner.Band, tunerProcessFeature, minFreq, maxFreq, seekStep);
			Wait1ms(10);  
			/* SetSeekParameter if use auto seek method in Star, need fine tune the parameters */
#if 1//def AUTOSEEK
			Star_Cmd_Tuner_Set_Seek_Threshold(eTuner_Channel_Main, SEEK_AM_FS_THRESHOLD, SEEK_AM_DT_THRESHOLD, SEEK_AM_ADJ_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_AM_SNR_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_THRESHOLD_NOTUSED);
#endif
		}
		else
		{
			Tuner.Band = eTuner_Mode_AM_US;
			Star_Cmd_Tuner_Change_Band(eTuner_Channel_Main, Tuner.Band, tunerProcessFeature, minFreq, maxFreq, seekStep);
			Wait1ms(10);  
			/* SetSeekParameter if use auto seek method in Star, need fine tune the parameters */
#if 1//def AUTOSEEK
			Star_Cmd_Tuner_Set_Seek_Threshold(eTuner_Channel_Main, SEEK_AM_FS_THRESHOLD, SEEK_AM_DT_THRESHOLD, SEEK_AM_ADJ_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_AM_SNR_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_THRESHOLD_NOTUSED);
#endif
		}
		
		Wait1ms(20);   
		
		TunerSetFreq(eTuner_Channel_Main, currentFreq);

	}

}
#endif

/*************************************************************************************
Function		: TunerSetFreq
Description	: Change frequencey control sequence(change to the specified requency in current band)

Parameters	:   
			tunerChannel :
			  	eTuner_Channel_Main		 :   foreground channel
			  	eTuner_Channel_Background   :  background channel
			freq : the frequeny to be set
			
Return Value	: None
*************************************************************************************/
void TunerSetFreq(eTuner_Channel tunerChannel, U32 freq)
{
	Star_Cmd_Tuner_Tune(tunerChannel, freq, INJECTION_SIDE_AUTO, DEFAULT);
	
	Wait1ms(1);
}


/*************************************************************************************
Function		: TunerSeekOn
Description	: Activate Seek mode(seekon), to be compatible with original HIT2/ELITE s/w.

Parameters	: 
			tunerChannel :
				eTuner_Channel_Main 	 :	 foreground channel
				eTuner_Channel_Background	:  background channel
				
			seekStep: the seek step, 
			  	if seekStep > 0  	seek up
			  	else			 	seek down
			  
Return Value	: None
*************************************************************************************/
void TunerSeekOn(eTuner_Channel tunerChannel, signed long seekStep)
{
	//int i;
	u8	seekDir;
#ifdef AUTOSEEK
	u8 averageNum;
#endif
	U32 freqStep;
	
	seekDir = (seekStep > 0) ? SEEK_UP : SEEK_DOWN; 
	freqStep	= (seekStep > 0) ? seekStep : seekStep * (-1);

#ifndef AUTOSEEK
	Star_StartManualSeek(tunerChannel,	seekDir, freqStep);
	Tuner.SeekType = eSeekManu;
#else
	averageNum = SEEK_FM_GET_QUALITY_CNT_N2;	 // calculate average quality in SEEK_QUAL_AVERAGE_COUNT times
	Star_StartAutoSeek(tunerChannel, seekDir, SEEK_END_MUTED, averageNum, freqStep, 0);
	Tuner.SeekType = eSeekAuto;
	Tuner.seekDir = seekDir;
	Tuner.seekStep = freqStep;
#endif

}



/*************************************************************************************
Function		: TunerSeekOff
Description	: Exit from seek mode(seekon)

Parameters	:  
			tunerChannel :
			  	eTuner_Channel_Main		 :   foreground channel
			  	eTuner_Channel_Background   :  background channel
			SeekEndOption
				SeekStopMuted			0x02
				SeekStopUnmuted 			0x03
				
Return Value	: None
*************************************************************************************/
void TunerSeekOff(eTuner_Channel tunerChannel, u8 SeekEndOption)
{
	Star_Cmd_Tuner_Seek_End(tunerChannel,  (SeekEndOption == SeekStopUnmuted) ? 0 : 1);
	Tuner.SeekType = eSeekIdle;
}

/*************************************************************************************
Function		: TunerSeekContinue
Description	: Seek continue in manual mode

Parameters	: 
			tunerChannel :
			  	eTuner_Channel_Main		 :   foreground channel
			  	eTuner_Channel_Background   :  background channel
			  	
Return Value	: None
*************************************************************************************/
void TunerSeekContinue(eTuner_Channel tunerChannel)
{
#ifndef AUTOSEEK   // Manual seek
	Star_Cmd_Tuner_Seek_Start(tunerChannel, DEFAULT, DEFAULT);	

#else
	Star_StartAutoSeek(tunerChannel, Tuner.seekDir , SEEK_END_MUTED, SEEK_FM_GET_QUALITY_CNT_N2, Tuner.seekStep, 1);
#endif
	
}


u8 TunerJudgeGoodStation(void)
{
	s8 Smeter = 0, Adj = 0;
	u8 Det = 0,Mpp = 0,Mpx = 0,Snr = 0,Dev = 0;

	Smeter = Radio.Smeter;
	Det = Radio.Detuning;
	Mpp = Radio.Multipath;
	Mpx = Radio.MpxNoise;
	Snr = Radio.SNR;
	Adj = Radio.AdjChannel;
	Dev = Radio.Deviation;	
	
	if(Tuner.Band == eTuner_Mode_FM)
	{
		if ( Smeter   >= SEEK_FM_FS_THRESHOLD    &&  
		      Det < SEEK_FM_DT_THRESHOLD     &&
		      Mpp	  < SEEK_FM_MPP_THRESHOLD && 
		      Mpx   < SEEK_FM_MPXN_THRESHOLD   && 
		      Snr	  > SEEK_FM_SNR_THRESHOLD  &&
		      Adj	  < SEEK_FM_ADJ_THRESHOLD)
			return TRUE;
		else
		    return FALSE;
	}
	else if (Tuner.Band == eTuner_Mode_WX)
	{	// WX
		if ( Smeter >= SEEK_WX_FS_THRESHOLD  &&  
			Det < SEEK_WX_DT_THRESHOLD)
			return TRUE;
		else
		    return FALSE;	
	}
	else 
	{	// AM
		if ( (Smeter >= SEEK_AM_FS_THRESHOLD  &&  
			Det < SEEK_AM_DT_THRESHOLD && 
			Adj < SEEK_AM_ADJ_THRESHOLD && 
			Snr >= SEEK_AM_SNR_THRESHOLD))
			return TRUE;
		else
		    return FALSE;		
	}
		
}

/*************************************************************************************
Function		: TunerReadSeekStatus
Description	: read out the seek status(frequency,quality info and seek mode status)
Parameters	: 
			tunerChannel :
			  	eTuner_Channel_Main		 :   foreground channel
			  	eTuner_Channel_Background   :  background channel

Return Values	: None
*************************************************************************************/
void TunerReadSeekStatus(eTuner_Channel tunerChannel, tda7708_status_t *status)
{
	U32 freq;
	int i, smeter   = 0;
	U32 detuning  = 0;
	U32 multiPath = 0;
	U32 mpxNoise = 0;
	//int   ADJp = 0;
	int   adjChannel = 0;
	U32 SNR = 0;
	u8   bGoodStation = 0;

#ifdef AUTOSEEK   /* Auto seek */

	// Not fully implemented as it's depend on the firmware which has not finished.
	Star_Cmd_Tuner_Get_Seek_Status(tunerChannel);

	freq   = (U32)(Radio.DataBuff[3] & 0x1F) << 16 ; //RF frequency
	freq |= (U32)(Radio.DataBuff[4] & 0xFF) << 8 ;
	freq |= (U32)(Radio.DataBuff[5] & 0xFF);
	
	smeter	   = ((Radio.DataBuff[7] & 0x80) ? Radio.DataBuff[7] - 256 : Radio.DataBuff[7]);
	detuning   = Radio.DataBuff[8];

	Radio.F_GoodStation    = (Radio.DataBuff[3] & SEEK_STATUS_SEEKOK) ? 1 : 0;
	Radio.F_SeekFullCycle = (Radio.DataBuff[3] & 0x40) ? 1 : 0; 	/*BIT 22*/

	printf("smeter:%d, detuning:%d, freq:%d, Radio.F_GoodStation:%d, Radio.F_SeekFullCycle:%d\n", smeter, detuning, freq, Radio.F_GoodStation, Radio.F_SeekFullCycle);
	if (Tuner.Band ==  eTuner_Mode_FM)//FM
	{
		multiPath  = Radio.DataBuff[9];
		mpxNoise = Radio.DataBuff[10];
		SNR 		= Radio.DataBuff[11];
		adjChannel = ((Radio.DataBuff[12] & 0x80) ? Radio.DataBuff[12] - 256 : Radio.DataBuff[12]);

		printf("multiPath:%d, mpxNoise:%d, SNR:%d, adjChannel:%d\n", multiPath, mpxNoise, SNR, adjChannel);

		RadioRefreshFreq (freq);	/* To be compatible with the original radio source code at this moment */

		if (Radio.F_GoodStation == 0)
		{
			Radio.F_TunerDispReq = 1;
			
			return;
		}

		if ( tunerChannel == eTuner_Channel_Main)
		{
			Radio.Smeter	   = smeter;
			Radio.Detuning = detuning;
			Radio.Multipath = multiPath;
			Radio.MpxNoise = mpxNoise;
			Radio.SNR = SNR;
			Radio.AdjChannel = adjChannel;
		}
			
		
		Radio.F_TunerDispReq = 1;
		
	}
	else  //AM, WX
	{
		if (Tuner.Band != eTuner_Mode_WX)
		{
			adjChannel =  ((Radio.DataBuff[12] & 0x80) ? Radio.DataBuff[12] - 256 : Radio.DataBuff[12]);
		}

		printf("AM smeter:%d, detuning:%d, freq:%d, adjChannel:%d\n", smeter, detuning, freq, adjChannel);
		{
			RadioRefreshFreq (freq);
	 
			Radio.Smeter	   = smeter;
			Radio.Detuning = detuning;
			if (Tuner.Band != eTuner_Mode_WX)
			{			
				Radio.AdjChannel = adjChannel;
			}
			Radio.F_TunerDispReq = 1;
		}

	}
	
#else	// Manual seek
	if (Tuner.Band ==  eTuner_Mode_FM)//FM
	{
		Wait1ms(SEEK_FM_WAIT_T1);

		Radio.F_GoodStation = 0;

		for ( i = 0; i < SEEK_FM_GET_QUALITY_CNT_N1;  i++)
		{		
			Star_Cmd_Tuner_Get_Seek_Status(tunerChannel);
			smeter += ((Radio.DataBuff[7] & 0x80) ? Radio.DataBuff[7] - 256 : Radio.DataBuff[7]);
			Wait1ms(SEEK_FM_WAIT_T2);
		}
		smeter /= SEEK_FM_GET_QUALITY_CNT_N1;
		
		freq   = (U32)(Radio.DataBuff[3] & 0x1F) << 16 ;	 /*RF frequency, @the last read */
		freq |= (U32)(Radio.DataBuff[4] & 0xFF) << 8 ;
		freq |= (U32)(Radio.DataBuff[5] & 0xFF);

		{
			RadioRefreshFreq (freq);		/* To be compatible with the original radio source code at this moment */
		}

		if (smeter <= SEEK_FM_FS_THRESHOLD - SEEK_FM_FS_THRESHOLD_DELTA)
		{
			//Pre-checking for smeter, if smeter is not good enough, continue seek.
			Radio.F_SeekFullCycle = (Radio.DataBuff[3] & 0x40) ? 1 : 0; 	/*BIT 22*/
			Radio.F_TunerDispReq = 1;
			
			return;
		}

		Wait1ms(SEEK_FM_WAIT_T3);	//pre-checking smeter is good enough. wait more time.
		
		smeter = 0;
		for ( i = 0; i < SEEK_FM_GET_QUALITY_CNT_N2;  i++)
		{		
			Star_Cmd_Tuner_Get_Seek_Status(tunerChannel);
			smeter	   += ((Radio.DataBuff[7] & 0x80) ? Radio.DataBuff[7] - 256 : Radio.DataBuff[7]);
			detuning   += Radio.DataBuff[8];
			multiPath  += Radio.DataBuff[9];
			mpxNoise += Radio.DataBuff[10];
			SNR += Radio.DataBuff[11];
			adjChannel += ((Radio.DataBuff[12] & 0x80) ? Radio.DataBuff[12] - 256 : Radio.DataBuff[12]);

			Wait1ms(SEEK_FM_WAIT_T4);
		}
		
		smeter /= SEEK_FM_GET_QUALITY_CNT_N2;
		detuning /= SEEK_FM_GET_QUALITY_CNT_N2;
		multiPath /= SEEK_FM_GET_QUALITY_CNT_N2;
		mpxNoise /= SEEK_FM_GET_QUALITY_CNT_N2;
		SNR /= SEEK_FM_GET_QUALITY_CNT_N2;
		adjChannel /= SEEK_FM_GET_QUALITY_CNT_N2;			
	
		if ( tunerChannel == eTuner_Channel_Main)
		{
			Radio.Smeter	   = smeter;	
			Radio.Detuning = detuning;
			Radio.Multipath = multiPath;
			Radio.MpxNoise = mpxNoise;
			Radio.SNR = SNR;
			Radio.AdjChannel = adjChannel; 
		}		
		
		printf("Fm smeter:%d, detuning:%d, freq:%d, Radio.F_GoodStation:%d, Radio.F_SeekFullCycle:%d\n", smeter, detuning, freq, Radio.F_GoodStation, Radio.F_SeekFullCycle);
		printf("Fm multiPath:%d, mpxNoise:%d, SNR:%d, adjChannel:%d\n", multiPath, mpxNoise, SNR, adjChannel);
		if ( smeter   >= SEEK_FM_FS_THRESHOLD	 &&  
			  detuning < SEEK_FM_DT_THRESHOLD	  &&
			  multiPath   < SEEK_FM_MPP_THRESHOLD && 
			  mpxNoise	 < SEEK_FM_MPXN_THRESHOLD	&& 
			  SNR	  > SEEK_FM_SNR_THRESHOLD  &&
			  adjChannel	  < SEEK_FM_ADJ_THRESHOLD)	  
		{
			Radio.F_GoodStation = 1;
			status->station = bGoodStation;
		}		
		
		Radio.F_SeekFullCycle = (Radio.DataBuff[3] & 0x40) ? 1 : 0; 	/*BIT 22*/
		Radio.F_TunerDispReq = 1;
		
	}
	else  //AM, WX
	{
		Wait1ms(SEEK_AMWX_WAIT_T1);

		bGoodStation = 0;
		
		for ( i = 0; i < SEEK_AMWX_GET_QUALITY_CNT_N1;	i++)
		{
			Star_Cmd_Tuner_Get_Seek_Status(tunerChannel);
			smeter		 += ((Radio.DataBuff[7] & 0x80) ? Radio.DataBuff[7] - 256 : Radio.DataBuff[7]);
			detuning	 += Radio.DataBuff[8];
			
			if (Tuner.Band != eTuner_Mode_WX)
			{
				SNR += Radio.DataBuff[11];
				adjChannel +=  ((Radio.DataBuff[12] & 0x80) ? Radio.DataBuff[12] - 256 : Radio.DataBuff[12]);
			}
			Wait1ms(SEEK_AMWX_WAIT_T2);
		}
		
		smeter /= SEEK_AMWX_GET_QUALITY_CNT_N1;
		detuning /= SEEK_AMWX_GET_QUALITY_CNT_N1;
		
		if (Tuner.Band != eTuner_Mode_WX)
		{		
			adjChannel /= SEEK_AMWX_GET_QUALITY_CNT_N1;
			SNR  /= SEEK_AMWX_GET_QUALITY_CNT_N1;
		}
		
		freq   = (U32)(Radio.DataBuff[3] & 0x1F) << 16 ;	 /*RF frequency, @the last read */
		freq |= (U32)(Radio.DataBuff[4] & 0xFF) << 8 ;
		freq |= (U32)(Radio.DataBuff[5] & 0xFF);

		{
			RadioRefreshFreq (freq);
	 
			Radio.Smeter	   = smeter;	
			Radio.Detuning = detuning;
			if (Tuner.Band != eTuner_Mode_WX)
			{			
				Radio.AdjChannel = adjChannel;
			}
			Radio.F_TunerDispReq = 1;
		}
		
		printf("smeter:%d, detuning:%d, freq:%d, Radio.F_GoodStation:%d, Radio.F_SeekFullCycle:%d\n", smeter, detuning, freq, Radio.F_GoodStation, Radio.F_SeekFullCycle);

		if (Tuner.Band == eTuner_Mode_WX)
		{	// WX
			if ( smeter >= SEEK_WX_FS_THRESHOLD  &&  detuning < SEEK_WX_DT_THRESHOLD)
			{
				bGoodStation = 1;
			}
		}
		else
		{	// AM
			if ( (smeter >= SEEK_AM_FS_THRESHOLD  &&  detuning < SEEK_AM_DT_THRESHOLD && adjChannel < SEEK_AM_ADJ_THRESHOLD && SNR >= SEEK_AM_SNR_THRESHOLD))
			{
				bGoodStation = 1;
			}				
		}
		
		Radio.F_SeekFullCycle = (Radio.BackupFreq == Radio.Freq) ? 1 : 0;
		Radio.F_GoodStation = bGoodStation;
		status->station = bGoodStation;

	}
#endif
}

/*************************************************************************************
Function		: TunerDSPStuckCheck
Description	: Check if the DSP stuck happened.
Parameters	: None
Return Value	: 
			  1 : DSP stuck happened
			  0 : DSP works normally  
*************************************************************************************/
int TunerDSPStuckCheck(void)
{
	static U32 checkData;
	U32 rtnData;

	Star_Cmd_Tuner_Ping(checkData);

	rtnData  = ~checkData;
	checkData++;
	if (checkData > 0xFFFFFF) checkData = 0x0;
	
	if ((Radio.DataBuff[3] == ((rtnData >> 16) & 0xFF)) &&
	     (Radio.DataBuff[4] == ((rtnData >> 8)   & 0xFF)) &&
	     (Radio.DataBuff[5] == (rtnData & 0xFF)))
	{
		return 0;
	}
	return 1;
}


/*************************************************************************************
Function		: TunerRDSBufferRead
Description	: RDS buffer read
Parameter	: eTuner_Channel tunerChannel 
			
Return Value	: None
*************************************************************************************/
#if defined(_ENABLE_RADIO_RDS_)

#define EONBuffSize		15

void TunerRDSBufferRead(eTuner_Channel tunerChannel )
{
	Star_Cmd_Tuner_RDSBuffer_Read(tunerChannel);  
}

/*************************************************************************************
Function		: TunerRDSBufferReset
Description	: RDS buffer Reset
Parameter	: eTuner_Channel tunerChannel
	
Return Value	: None
*************************************************************************************/
void TunerRDSBufferReset(eTuner_Channel tunerChannel)
{
	int index  = tunerChannel - 1;
	
	if (index < 0) index = 0;

	if (Tuner.rdsConfig[index].CSR & 0x01) 
	{
		Tuner.rdsConfig[index].CSR |= 0x01 << RDS_CSR_RESET;	//Reset
		Star_Cmd_Tuner_RDSBuffer_Set(tunerChannel, Tuner.rdsConfig[index].CSR, Tuner.rdsConfig[index].NBR);
	}
}

/*************************************************************************************
Function		: RDSBufferInit
Description	: RDS data buffer init.
Parameters	: eTimer : RDS data timer type
			  bForceReset : force reset RDS BUFFER
			  
Return Value	: None.
***********************************************************************************/
void RDSBufferInit(eTuner_Channel tunerChannel, u8 bForceReset)
{
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;

	/*Reset RDS decoder to speed up the resynchronization*/
	if (bForceReset && RDSApp.States.Flags.F_AFEnable) 
	{
		TunerRDSBufferReset(tunerChannel);
	}
	
	pRDSData_Active->RawDataBuffer.BlockCnt = 0;
	pRDSData_Active->BlocksBuffer.BlockCnt = 0;
	pRDSData_Active->BlocksBuffer.LastBlockCnt = 0;
}

/*********************************************
	Function:		ChangFreq16BitTo8Bit
	Description:	
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:37:01
*********************************************/
u8 ChangFreq16BitTo8Bit(u16 inputfreq)
{
	return ((inputfreq - 8750) / 10);
}

/*********************************************
	Function:		RDSData_Init
	Description:	
	Write/Modify:	Perry.Liang
	Time:		2006-1-24- 15:37:24
*********************************************/
void RDSData_Init(eTuner_Channel tunerChannel, u8 bResetRDSBuffer)
{
	u8 i, j;
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;

	RDSBufferInit(tunerChannel, bResetRDSBuffer);

	//Flags
	pRDSData_Active->DecodedData.Flags.REAL_F_BlockA = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_BlockB = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_BlockC = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_BlockCp = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_BlockD = 0;
	
	pRDSData_Active->DecodedData.Flags.REAL_F_RDSStation = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_RDSStationSlow = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetOneGroup = 0;

	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewPI = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewPTY = 0;
	pRDSData_Active->DecodedData.Flags.REAL_TA = 0;
	pRDSData_Active->DecodedData.Flags.REAL_TALast = 0;
	pRDSData_Active->DecodedData.Flags.REAL_TP = 0;
	pRDSData_Active->DecodedData.Flags.REAL_TPLast = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewTP = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewTA = 0;
	pRDSData_Active->DecodedData.Flags.REAL_EONTA = 0;
	pRDSData_Active->DecodedData.Flags.REAL_EONTALast = 0;
	pRDSData_Active->DecodedData.Flags.REAL_EONTP = 0;
	pRDSData_Active->DecodedData.Flags.REAL_EONTPLast = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewEONTP = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewEONTA = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_PSGet0 = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_PSGet1 = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_PSGet2 = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_PSGet3 = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_PSExist = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewPS = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewCT = 0;

	//PI infor
	pRDSData_Active->DecodedData.PIInfor.REAL_PILast2 = 0;
	pRDSData_Active->DecodedData.PIInfor.REAL_PILast = 0;
	pRDSData_Active->DecodedData.PIInfor.REAL_PI = 0;
	
	//PTY infor
	pRDSData_Active->DecodedData.PTYInfor.REAL_PTY = 0;
	pRDSData_Active->DecodedData.PTYInfor.REAL_PTYLast = 0;
	
	//PS infor
	pRDSData_Active->DecodedData.PSInfor.REAL_PSCRC = 0;
	pRDSData_Active->DecodedData.PSInfor.REAL_PSCRCTemp = 0;

	pRDSData_Active->DecodedData.Blocks.REAL_MaxLevel = 0;
	

	//if (RDSApp_GetState(RDS_APP_PI_SEEK)) return;

	pRDSData_Active->DecodedData.PIInfor.REAL_PIBackup = 0;

	if (!pRDSData_Active->DecodedData.Flags.REAL_F_InEONTA)
	{
		for (i = 1; i < 25; i++)
		{
			pRDSData_Active->DecodedData.AFInfor.REAL_AF[i] = 0xFF;
		}
		
		pRDSData_Active->DecodedData.Flags.REAL_F_AFMethodB = 0;
		pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] = 0xFF;
		pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] = 0xFF;		
	}
	
	for (i = 0; i < 25; i++)
	{
		pRDSData_Active->DecodedData.EONInfor.REAL_EONAF[i] = 0xFF;
	}

	if (FreqInAM(Radio.Freq) || FreqInJapanFM(Radio.Freq) || FreqInWBFM(Radio.Freq))
	{
		pRDSData_Active->DecodedData.AFInfor.REAL_AF[0] = 0xFF;
	}
	else
	{
		pRDSData_Active->DecodedData.AFInfor.REAL_AF[0] = ChangFreq16BitTo8Bit(Radio.Freq);
	}
	
	if (!pRDSData_Active->DecodedData.Flags.REAL_F_InEONTA)
	{
		pRDSData_Active->DecodedData.AFInfor.REAL_AFNum = 1;
	}
	
	pRDSData_Active->DecodedData.EONInfor.REAL_EONAFNum = 0;
	pRDSData_Active->DecodedData.EONInfor.REAL_EONPI = 0;
	for (j = 0; j < EONBuffSize; j++)
	{
		pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] = 0;
		pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j] = 0;
		for (i = 0; i < 25; i++)
		{
			pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAF[j][i] = 0xFF;
		}
		
		for (i = 0; i < 8; i++)
		{
			pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][i] = 0;
		}
	}
	
	pRDSData_Active->DecodedData.Flags.REAL_F_NoMatchEON = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_InEONTA = 0;

	pRDSData_Active->DecodedData.Flags.REAL_F_GetNewAF = 0;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetAFList = 0;
}

/*************************************************************************************
Function		: RDSInit
Description	: Used for RDS Application initiation.
Parameters	: None
Return Value	: None
*************************************************************************************/
void RDSInit(void)
{
	eTuner_Channel tunerChannel = eTuner_Channel_Main;

	RDSApp.States.Flags.F_AFEnable = 0;
	RDSApp.States.Flags.F_REGEnable = 0;
	RDSApp.States.Flags.F_PIMuteEnable = 1;

	RDSApp.States.Flags.F_EONEnable = 1;

	RDSData_Init (tunerChannel, TRUE);
}

/*************************************************************************************
Function		: TunerRDSBufferDisable
Description	: RDS buffer Disable
Parameter	: eTuner_Channel tunerChannel 
			
Return Value	: None
*************************************************************************************/
void TunerRDSBufferDisable(eTuner_Channel tunerChannel )
{
	int index  = tunerChannel - 1;
	
	if (index < 0) index = 0;
	
	if (Tuner.rdsConfig[index].CSR & 0x01) Tuner.rdsConfig[index].CSR &= 0xFE;
	Star_Cmd_Tuner_RDSBuffer_Set(tunerChannel, Tuner.rdsConfig[index].CSR, Tuner.rdsConfig[index].NBR);
}

/*************************************************************************************
Function		: TunerRDSBufferEnable
Description	: RDS buffer Enable

Parameters	:	  
			  modeIRQ 	:  	
			  		1: interrupt mode; 
			  		0: polling mode;
			  		
Return Value	: None
*************************************************************************************/
void TunerRDSBufferEnable(eTuner_Channel tunerChannel, u8 modeIRQ)
{
	U32 rdsCSR, rdsNBR;
	int index = tunerChannel - 1;
	
	if (index < 0) index = 0;
	
	//Reset,   err filter,  modeIRQ, RDS, Enable
	rdsCSR   = 0x01 << RDS_CSR_ENA;					//Enable
	rdsCSR |= 0x01 << RDS_CSR_RESET;   				//Reset
	rdsCSR |= 0x02 << RDS_CSR_ERRTHRESH;   			//err filter	
	rdsCSR |= modeIRQ << RDS_CSR_IREN;          		//IREN, up to modeIRQ

	rdsNBR = RDS_NORMALMODE_NRQST;
	
	Star_Cmd_Tuner_RDSBuffer_Set(tunerChannel, rdsCSR, rdsNBR);

	Tuner.rdsConfig[index].CSR = rdsCSR;
	Tuner.rdsConfig[index].NBR = rdsNBR;
}

/*************************************************************************************
Function		: RDSDataPushToGroup
Description	: Sepetrate RDS blocks by group, and put the blocks to group data buffer.
Parameters	: tunerChannel : main or background tuner channel.
			  
Return Value	: None.
***********************************************************************************/
void RDSDataPushToGroup(eTuner_Channel tunerChannel)
{
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;

	if ((pRDSData_Active->BlocksBuffer.BlockCnt < pRDSData_Active->BlocksBuffer.LastBlockCnt) && 
	     (pRDSData_Active->BlocksBuffer.LastBlockCnt != 0) &&
	     ( pRDSData_Active->BlocksBuffer.F_GetBlockA ||
	       pRDSData_Active->BlocksBuffer.F_GetBlockB || 
	       pRDSData_Active->BlocksBuffer.F_GetBlockC || 
	       pRDSData_Active->BlocksBuffer.F_GetBlockCp || 
	       pRDSData_Active->BlocksBuffer.F_GetBlockD))
	{
		pRDSData_Active->BlocksBuffer.BlockLastA 	= pRDSData_Active->BlocksBuffer.BlockA;
		pRDSData_Active->BlocksBuffer.BlockLastB 	= pRDSData_Active->BlocksBuffer.BlockB;
		pRDSData_Active->BlocksBuffer.BlockLastC 	= pRDSData_Active->BlocksBuffer.BlockC;
		pRDSData_Active->BlocksBuffer.BlockLastCp 	= pRDSData_Active->BlocksBuffer.BlockCp;
		pRDSData_Active->BlocksBuffer.BlockLastD 	= pRDSData_Active->BlocksBuffer.BlockD;

		pRDSData_Active->BlocksBuffer.F_BlockA 	= pRDSData_Active->BlocksBuffer.F_GetBlockA;
		pRDSData_Active->BlocksBuffer.F_BlockB 	= pRDSData_Active->BlocksBuffer.F_GetBlockB;
		pRDSData_Active->BlocksBuffer.F_BlockC 	= pRDSData_Active->BlocksBuffer.F_GetBlockC;
		pRDSData_Active->BlocksBuffer.F_BlockCp 	= pRDSData_Active->BlocksBuffer.F_GetBlockCp;
		pRDSData_Active->BlocksBuffer.F_BlockD 	= pRDSData_Active->BlocksBuffer.F_GetBlockD;

		pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;

		pRDSData_Active->BlocksBuffer.F_GetOneGroup = 1;

		pRDSData_Active->DecodedData.Flags.REAL_F_BlockA = 0;
		pRDSData_Active->DecodedData.Flags.REAL_F_BlockB = 0;
		pRDSData_Active->DecodedData.Flags.REAL_F_BlockC = 0;
		pRDSData_Active->DecodedData.Flags.REAL_F_BlockCp = 0;

		if (pRDSData_Active->BlocksBuffer.F_BlockA)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_BlockA = 1;
			pRDSData_Active->DecodedData.Blocks.REAL_BlockLastA = pRDSData_Active->BlocksBuffer.BlockLastA;	
		}
		
		if (pRDSData_Active->BlocksBuffer.F_BlockB)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_BlockB = 1;
			pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB = pRDSData_Active->BlocksBuffer.BlockLastB;		
		}
		
		if (pRDSData_Active->BlocksBuffer.F_BlockC)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_BlockC = 1;
			pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC = pRDSData_Active->BlocksBuffer.BlockLastC;
		}
		
		if (pRDSData_Active->BlocksBuffer.F_BlockCp)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_BlockCp = 1;
			pRDSData_Active->DecodedData.Blocks.REAL_BlockLastCp = pRDSData_Active->BlocksBuffer.BlockLastCp;
		} 
			
		if (pRDSData_Active->BlocksBuffer.F_BlockD)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_BlockD = 1;
			pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD = pRDSData_Active->BlocksBuffer.BlockLastD;
		}

		pRDSData_Active->DecodedData.Flags.REAL_F_GetOneGroup = 1;
	}
}

/*************************************************************************************
Function		: RDSDataProcessBlock
Description	: For RDS Data raw blocks procession
Parameters	: tunerChannel : main or background tuner channel.
			  
Return Value	: None.
***********************************************************************************/
#if 1
void RDSDataProcessBlock(eTuner_Channel tunerChannel)
{
	u8 errCnt, cType, blockID;
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;

	errCnt  = (pRDSData_Active->BlocksBuffer.RDSData[0] & 0x70) >> 4;
	cType 	= (pRDSData_Active->BlocksBuffer.RDSData[0] & 0x04) >> 2;
	blockID	= (pRDSData_Active->BlocksBuffer.RDSData[0] & 0x03);

	//printf("errCnt:%d, cType:%d, blockID:%d\n", errCnt, cType, blockID);
	if(blockID == 0x00)  //block A
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 1;
		RDSDataPushToGroup(tunerChannel);

		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 1;
			
			pRDSData_Active->BlocksBuffer.BlockA   = (u16)(pRDSData_Active->BlocksBuffer.RDSData[1]) << 8;
			pRDSData_Active->BlocksBuffer.BlockA |= (u16)(pRDSData_Active->BlocksBuffer.RDSData[2]);
			
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;

			tda7708_ctrl->tuner_rds.get_block |= 0x01;
			tda7708_ctrl->tuner_rds.block_a = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_a:0x%04x\n",tda7708_info.tuner_rds.block_a);
			tda7708_ctrl->tuner_rds.block_cnt = 2;
			
			tda7708_ctrl->tuner_rds.quality = 0xFF;
			
			rds_block_a_dec(_u16to8h_(pRDSData_Active->BlocksBuffer.BlockA), _u16to8l_(pRDSData_Active->BlocksBuffer.BlockA));
			//rds_block_a_dec(_u16to8h_(tda7708_ctrl->tuner_rds.block_a), _u16to8l_(tda7708_ctrl->tuner_rds.block_a));
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xFE;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
		}

		pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
	}
	else if (blockID == 0x01)	//block B
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 2;
		RDSDataPushToGroup(tunerChannel);
		
		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 1;
			pRDSData_Active->BlocksBuffer.BlockB   = (u16)pRDSData_Active->BlocksBuffer.RDSData[1] << 8;
			pRDSData_Active->BlocksBuffer.BlockB |= (u16)pRDSData_Active->BlocksBuffer.RDSData[2];
			
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;
			
			tda7708_ctrl->tuner_rds.get_block |= 0x02;
			tda7708_ctrl->tuner_rds.block_b = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_b:0x%04x\n",tda7708_info.tuner_rds.block_b);
			tda7708_ctrl->tuner_rds.block_cnt = 3;
			
			tda7708_ctrl->tuner_rds.quality = 0xFF;
			rds_block_b_dec(_u16to8h_(pRDSData_Active->BlocksBuffer.BlockB), _u16to8l_(pRDSData_Active->BlocksBuffer.BlockB));
			//rds_block_b_dec(_u16to8h_(tda7708_ctrl->tuner_rds.block_b), _u16to8l_(tda7708_ctrl->tuner_rds.block_b));
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xFC;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
		}
		pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
		
	}
	else if (blockID == 0x02)	//block C or C'
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 3;
		RDSDataPushToGroup(tunerChannel);
		
		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockC   = 1;
			pRDSData_Active->BlocksBuffer.F_GetBlockCp = cType;
			
			pRDSData_Active->BlocksBuffer.BlockC   = (u16)pRDSData_Active->BlocksBuffer.RDSData[1] << 8;
			pRDSData_Active->BlocksBuffer.BlockC |= (u16)pRDSData_Active->BlocksBuffer.RDSData[2];
			
			pRDSData_Active->BlocksBuffer.BlockCp = pRDSData_Active->BlocksBuffer.BlockC;
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;

			tda7708_ctrl->tuner_rds.get_block |= 0x04;
			tda7708_ctrl->tuner_rds.block_c = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_c:0x%04x\n",tda7708_info.tuner_rds.block_c);
			tda7708_ctrl->tuner_rds.block_cnt = 4;
			
			tda7708_ctrl->tuner_rds.quality = 0xFF;
			rds_block_c_dec(_u16to8h_(pRDSData_Active->BlocksBuffer.BlockC), _u16to8l_(pRDSData_Active->BlocksBuffer.BlockC));
			//rds_block_c_dec(_u16to8h_(tda7708_ctrl->tuner_rds.block_c), _u16to8l_(tda7708_ctrl->tuner_rds.block_c));
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xF8;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
			pRDSData_Active->BlocksBuffer.BlockC = 0;
		}
		pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
	}
	else if (blockID == 0x03)	//block D
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 4;
		RDSDataPushToGroup(tunerChannel);
		
		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockD = 1;
			
			pRDSData_Active->BlocksBuffer.BlockD   = (u16)pRDSData_Active->BlocksBuffer.RDSData[1] << 8;
			pRDSData_Active->BlocksBuffer.BlockD |= (u16)pRDSData_Active->BlocksBuffer.RDSData[2];
			
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;
			
			tda7708_ctrl->tuner_rds.get_block |= 0x08;
			tda7708_ctrl->tuner_rds.block_d = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_d:0x%04x\n",tda7708_info.tuner_rds.block_d);
			tda7708_ctrl->tuner_rds.block_cnt = 1;
			
			//cout("--->tda7708_info.tuner_rds.get_block:0x%04x:::0x%x\n",tda7708_info.tuner_rds.get_block, (tda7708_info.tuner_rds.get_block & 0x0F));
			if((tda7708_ctrl->tuner_rds.get_block & 0x0F) == 0x0F)
			{
				tda7708_ctrl->tuner_rds.get_block = 0;
				
				rds_input_t input;
			
				input.block_ah = _u16to8h_(tda7708_ctrl->tuner_rds.block_a);
				input.block_al = _u16to8l_(tda7708_ctrl->tuner_rds.block_a);
				input.block_bh = _u16to8h_(tda7708_ctrl->tuner_rds.block_b);
				input.block_bl = _u16to8l_(tda7708_ctrl->tuner_rds.block_b);
				input.block_ch = _u16to8h_(tda7708_ctrl->tuner_rds.block_c);
				input.block_cl = _u16to8l_(tda7708_ctrl->tuner_rds.block_c);
				input.block_dh = _u16to8h_(tda7708_ctrl->tuner_rds.block_d);
				input.block_dl = _u16to8l_(tda7708_ctrl->tuner_rds.block_d);
			
				tda7708_ctrl->tuner_rds.quality = 0xFF;
			
				rds_stream_dec(&input);
			}
			else if(tda7708_ctrl->tuner_rds.get_block & 0x08)
			{
				tda7708_ctrl->tuner_rds.quality = 0xFF;
				rds_block_d_dec(_u16to8h_(pRDSData_Active->BlocksBuffer.BlockD), _u16to8l_(pRDSData_Active->BlocksBuffer.BlockD));
				//rds_block_d_dec(_u16to8h_(tda7708_ctrl->tuner_rds.block_d), _u16to8l_(tda7708_ctrl->tuner_rds.block_d));
			}
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xF0;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
			pRDSData_Active->BlocksBuffer.BlockD = 0;
		}
	}

	pRDSData_Active->BlocksBuffer.LastBlockCnt = pRDSData_Active->BlocksBuffer.BlockCnt;
	RDSData_SetDataBlock(tunerChannel, pRDSData_Active->BlocksBuffer.F_RDSStation);
}

#else
void RDSDataProcessBlock(eTuner_Channel tunerChannel)
{
	u8 errCnt, cType, blockID;
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;

	errCnt  = (pRDSData_Active->BlocksBuffer.RDSData[0] & 0x70) >> 4;
	cType 	= (pRDSData_Active->BlocksBuffer.RDSData[0] & 0x04) >> 2;
	blockID	= (pRDSData_Active->BlocksBuffer.RDSData[0] & 0x03);

	cout("errCnt:%d, cType:%d, blockID:%d\n", errCnt, cType, blockID);
	if (blockID == 0x00)  //block A
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 1;
		RDSDataPushToGroup(tunerChannel);

		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 1;
			
			pRDSData_Active->BlocksBuffer.BlockA   = (u16)(pRDSData_Active->BlocksBuffer.RDSData[1]) << 8;
			pRDSData_Active->BlocksBuffer.BlockA |= (u16)(pRDSData_Active->BlocksBuffer.RDSData[2]);
			
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;

			tda7708_ctrl->tuner_rds.get_block |= 0x01;
			tda7708_ctrl->tuner_rds.block_a = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_a:0x%04x\n",tda7708_info.tuner_rds.block_a);
			tda7708_ctrl->tuner_rds.block_cnt = 2;
			
			tda7708_ctrl->tuner_rds.quality = 0xFF;
			
			rds_block_a_dec(_u16to8h_(pRDSData_Active->BlocksBuffer.BlockA), _u16to8l_(pRDSData_Active->BlocksBuffer.BlockA));
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xFE;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
		}

		pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
	}
	else if (blockID == 0x01)	//block B
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 2;
		RDSDataPushToGroup(tunerChannel);
		
		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 1;
			pRDSData_Active->BlocksBuffer.BlockB   = (u16)pRDSData_Active->BlocksBuffer.RDSData[1] << 8;
			pRDSData_Active->BlocksBuffer.BlockB |= (u16)pRDSData_Active->BlocksBuffer.RDSData[2];
			
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;
			
			tda7708_ctrl->tuner_rds.get_block |= 0x02;
			tda7708_ctrl->tuner_rds.block_b = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_b:0x%04x\n",tda7708_info.tuner_rds.block_b);
			tda7708_ctrl->tuner_rds.block_cnt = 3;
			
			tda7708_ctrl->tuner_rds.quality = 0xFF;
			rds_block_b_dec(_u16to8h_(pRDSData_Active->BlocksBuffer.BlockA), _u16to8l_(pRDSData_Active->BlocksBuffer.BlockA));
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xFC;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
		}
		pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
		pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
		
	}
	else if (blockID == 0x02)	//block C or C'
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 3;
		RDSDataPushToGroup(tunerChannel);
		
		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockC   = 1;
			pRDSData_Active->BlocksBuffer.F_GetBlockCp = cType;
			
			pRDSData_Active->BlocksBuffer.BlockC   = (u16)pRDSData_Active->BlocksBuffer.RDSData[1] << 8;
			pRDSData_Active->BlocksBuffer.BlockC |= (u16)pRDSData_Active->BlocksBuffer.RDSData[2];
			
			pRDSData_Active->BlocksBuffer.BlockCp = pRDSData_Active->BlocksBuffer.BlockC;
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;

			tda7708_ctrl->tuner_rds.get_block |= 0x04;
			tda7708_ctrl->tuner_rds.block_c = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_c:0x%04x\n",tda7708_info.tuner_rds.block_c);
			tda7708_ctrl->tuner_rds.block_cnt = 4;
			
			tda7708_ctrl->tuner_rds.quality = 0xFF;
			rds_block_c_dec(_u16to8h_(pRDSData_Active->BlocksBuffer.BlockC), _u16to8l_(pRDSData_Active->BlocksBuffer.BlockC));
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xF8;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
			pRDSData_Active->BlocksBuffer.BlockC = 0;
		}
		pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
	}
	else if (blockID == 0x03)	//block D
	{
		pRDSData_Active->BlocksBuffer.BlockCnt = 4;
		RDSDataPushToGroup(tunerChannel);
		
		if (errCnt <= 0x02)   //ERRCOUNT <= 0x02, 
		{
			pRDSData_Active->BlocksBuffer.F_GetBlockD = 1;
			
			pRDSData_Active->BlocksBuffer.BlockD   = (u16)pRDSData_Active->BlocksBuffer.RDSData[1] << 8;
			pRDSData_Active->BlocksBuffer.BlockD |= (u16)pRDSData_Active->BlocksBuffer.RDSData[2];
			
			pRDSData_Active->BlocksBuffer.F_RDSStation = 1;
			
			tda7708_ctrl->tuner_rds.get_block |= 0x08;
			tda7708_ctrl->tuner_rds.block_d = _u8tou16_((pRDSData_Active->BlocksBuffer.RDSData[1]),(pRDSData_Active->BlocksBuffer.RDSData[2]));
			//cout("--->block_d:0x%04x\n",tda7708_info.tuner_rds.block_d);
			tda7708_ctrl->tuner_rds.block_cnt = 1;
			
			//cout("--->tda7708_info.tuner_rds.get_block:0x%04x:::0x%x\n",tda7708_info.tuner_rds.get_block, (tda7708_info.tuner_rds.get_block & 0x0F));
			if((tda7708_ctrl->tuner_rds.get_block & 0x0F) == 0x0F)
			{
				tda7708_ctrl->tuner_rds.get_block = 0;
				
				rds_input_t input;
			
				input.block_ah = _u16to8h_(tda7708_ctrl->tuner_rds.block_a);
				input.block_al = _u16to8l_(tda7708_ctrl->tuner_rds.block_a);
				input.block_bh = _u16to8h_(tda7708_ctrl->tuner_rds.block_b);
				input.block_bl = _u16to8l_(tda7708_ctrl->tuner_rds.block_b);
				input.block_ch = _u16to8h_(tda7708_ctrl->tuner_rds.block_c);
				input.block_cl = _u16to8l_(tda7708_ctrl->tuner_rds.block_c);
				input.block_dh = _u16to8h_(tda7708_ctrl->tuner_rds.block_d);
				input.block_dl = _u16to8l_(tda7708_ctrl->tuner_rds.block_d);
			
				tda7708_ctrl->tuner_rds.quality = 0xFF;
			
				rds_stream_dec(&input);
			}
			else if(tda7708_ctrl->tuner_rds.get_block & 0x08)
			{
				tda7708_ctrl->tuner_rds.quality = 0xFF;
				rds_block_d_dec(_u16to8h_(tda7708_ctrl->tuner_rds.block_d), _u16to8l_(tda7708_ctrl->tuner_rds.block_d));
			}
		}
		else
		{
			tda7708_ctrl->tuner_rds.get_block &= 0xF0;
			pRDSData_Active->BlocksBuffer.F_GetBlockA = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockB = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockC = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockCp = 0;
			pRDSData_Active->BlocksBuffer.F_GetBlockD = 0;
			pRDSData_Active->BlocksBuffer.BlockD = 0;
		}
	}

	pRDSData_Active->BlocksBuffer.LastBlockCnt = pRDSData_Active->BlocksBuffer.BlockCnt;
	RDSData_SetDataBlock(tunerChannel, pRDSData_Active->BlocksBuffer.F_RDSStation);
}
#endif

/*************************************************************************************
Function		: RDSBufferFetchBlocks
Description	: Used to retrieve Tuner's (TDA7707) RDS buffer to get the RDS raw blocks. 
Parameters	: tunerChannel : main or background tuner channel.
			  
Return Value	: None.
***********************************************************************************/
void RDSBufferFetchBlocks(eTuner_Channel tunerChannel )
{
	u8 BlockNum;
	int index = tunerChannel - 1;
	RDSData_Process *pRDSData_Active;

	pRDSData_Active = &(RDSData.RDSData_Main);

	//puts("RDSBufferFetchBlocks\n");
	do {
		TunerRDSBufferRead(tunerChannel);
		//printf("Fetch 0x%x-0x%x-0x%x-0x%x-0x%x-0x%x-\n", Radio.DataBuff[0], Radio.DataBuff[1], Radio.DataBuff[2], Radio.DataBuff[3], Radio.DataBuff[4], Radio.DataBuff[5]);
		if (Radio.DataBuff[2] <= 1)  return;		//Radio.DataBuff[2] = N+1  N=BlockNum + RNR
		//puts("RDSBufferFetchBlocks111\n");
		if ((Radio.DataBuff[3] & 0x40) == 0x00)  return;   //not sync	

		//puts("RDSBufferFetchBlocks222\n");
	#ifdef RDS_BOFL_RESET		
		if ((Radio.DataBuff[3] & 0x20) > 0x00)    //BOFL, Buffer overflow
		{
			//reset the buffer
			TunerRDSBufferDisable(tunerChannel);
			TunerRDSBufferEnable(tunerChannel, 0x00);
			return;
		}
	#endif

		//puts("RDSBufferFetchBlocks333\n");
		BlockNum = Radio.DataBuff[2] - 0x01;
		if (BlockNum > 0)
		{
			memcpy((pRDSData_Active->RawDataBuffer.BufferData + pRDSData_Active->RawDataBuffer.BlockCnt * 3), (Radio.DataBuff + 6), BlockNum * 3);
		}
		/*
		int i;
		for(i = 0; i < BlockNum * 3; i++)
		{
			cout("0x%x ", Radio.DataBuff[6 + i]);
		}
		cout("\n");
		*/
		pRDSData_Active->RawDataBuffer.BlockCnt = /*pRDSData_Active->RawDataBuffer.BlockCnt + */BlockNum;
		
		//printf("pRDSData_Active->RawDataBuffer.BlockCnt:0x%x \n", pRDSData_Active->RawDataBuffer.BlockCnt);

		//puts("RDSBufferFetchBlocks444\n");
		RDSDataExtract(eTuner_Channel_Main);
	}while(((Radio.DataBuff[3] & 0x80) > 0x00));	//DataRDY = 1, Read out, 
}

/*************************************************************************************
Function		: RDSDataPushToGroup
Description	: RDS Data extraction per group, check group ID, extract detail data.
Parameters	: tunerChannel : main or background tuner channel.
			  
Return Value	: None.
***********************************************************************************/
#if 1
void RDSDataExtractGroup(eTuner_Channel tunerChannel)
{
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;
	
	rds_input_t input;
	
	input.block_ah = _u16to8h_(pRDSData_Active->BlocksBuffer.BlockA);
	input.block_al = _u16to8l_(pRDSData_Active->BlocksBuffer.BlockA);
	input.block_bh = _u16to8h_(pRDSData_Active->BlocksBuffer.BlockB);
	input.block_bl = _u16to8l_(pRDSData_Active->BlocksBuffer.BlockB);
	input.block_ch = _u16to8h_(pRDSData_Active->BlocksBuffer.BlockC);
	input.block_cl = _u16to8l_(pRDSData_Active->BlocksBuffer.BlockC);
	input.block_dh = _u16to8h_(pRDSData_Active->BlocksBuffer.BlockD);
	input.block_dl = _u16to8l_(pRDSData_Active->BlocksBuffer.BlockD);
	
	//printf("enter--->rds_stream_dec\n");
	
	rds_stream_dec(&input);
}
#else
void RDSDataExtractGroup(eTuner_Channel tunerChannel)
{
	U8 i, j, flag;
	U16 temp;
	UCharInt AFtemp;
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;

	if (!pRDSData_Active->DecodedData.Flags.REAL_F_GetOneGroup) return;
	pRDSData_Active->DecodedData.Flags.REAL_F_GetOneGroup = 0;
	
	if (pRDSData_Active->DecodedData.Blocks.REAL_Level < 20)
	{
		pRDSData_Active->DecodedData.Blocks.REAL_Level++;
	}

	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA)
	{
		// PI
		pRDSData_Active->DecodedData.PIInfor.REAL_PI = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastA;

		if (pRDSData_Active->DecodedData.PIInfor.REAL_PI == pRDSData_Active->DecodedData.PIInfor.REAL_PILast && pRDSData_Active->DecodedData.PIInfor.REAL_PILast != pRDSData_Active->DecodedData.PIInfor.REAL_PILast2)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_GetNewPI = 1;
			if (pRDSData_Active->DecodedData.PIInfor.REAL_PIBackup == 0)
			{
				pRDSData_Active->DecodedData.PIInfor.REAL_PIBackup = pRDSData_Active->DecodedData.PIInfor.REAL_PI;
			}
		}

		pRDSData_Active->DecodedData.PIInfor.REAL_PILast2 = pRDSData_Active->DecodedData.PIInfor.REAL_PILast;
		pRDSData_Active->DecodedData.PIInfor.REAL_PILast = pRDSData_Active->DecodedData.PIInfor.REAL_PI;

		if ((pRDSData_Active->DecodedData.PIInfor.REAL_PI != 0) && (pRDSData_Active->DecodedData.PIInfor.REAL_PI == pRDSData_Active->DecodedData.PIInfor.REAL_PIBackup) &&
		    (!RDSApp_GetState(RDS_APP_PI_SEEK) ) && 
		    (  RDSApp_GetState(RDS_APP_AF_ENABLE)) &&
		    (RDSStrategyData.AFData.REAL_AFCheckMode == AFCHECK_MODE_IDLE))
		{
			RDSStrategy_ResetPISeekTimer();
		}
	}

	if (RDSStrategyData.AFData.REAL_AFCheckMode !=  AFCHECK_MODE_IDLE) return;

	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA && pRDSData_Active->DecodedData.Flags.REAL_F_BlockB)
	{
		pRDSData_Active->DecodedData.PIInfor.REAL_PI = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastA;
		if (pRDSData_Active->DecodedData.PIInfor.REAL_PI == pRDSData_Active->DecodedData.PIInfor.REAL_PIBackup)
		{
			// PTY
			pRDSData_Active->DecodedData.PTYInfor.REAL_PTY = (U8)((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x03E0) >> 5);
			
			if (pRDSData_Active->DecodedData.PTYInfor.REAL_PTYLast != pRDSData_Active->DecodedData.PTYInfor.REAL_PTY)
			{
				pRDSData_Active->DecodedData.PTYInfor.REAL_PTYLast = pRDSData_Active->DecodedData.PTYInfor.REAL_PTY;
				pRDSData_Active->DecodedData.Flags.REAL_F_GetNewPTY = 1;
			}

			// TP
			if (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0400)
			{
				pRDSData_Active->DecodedData.Flags.REAL_TP = 1;
			}
			else
			{
				pRDSData_Active->DecodedData.Flags.REAL_TP = 0;
			}
			
			if (pRDSData_Active->DecodedData.Flags.REAL_TPLast != pRDSData_Active->DecodedData.Flags.REAL_TP)
			{
				pRDSData_Active->DecodedData.Flags.REAL_TPLast = pRDSData_Active->DecodedData.Flags.REAL_TP;
				pRDSData_Active->DecodedData.Flags.REAL_F_GetNewTP = 1;
			}

			// Group
			pRDSData_Active->DecodedData.REAL_Group = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB >> 11);

			// TA
			if (GROUP0(pRDSData_Active->DecodedData.REAL_Group)  || GROUP15B (pRDSData_Active->DecodedData.REAL_Group) )
			{
				if (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0010)
				{
					pRDSData_Active->DecodedData.Flags.REAL_TA = 1;
				}
				else
				{
					pRDSData_Active->DecodedData.Flags.REAL_TA = 0;
				}
				
				if (pRDSData_Active->DecodedData.Flags.REAL_TALast != pRDSData_Active->DecodedData.Flags.REAL_TA)
				{
					pRDSData_Active->DecodedData.Flags.REAL_TALast = pRDSData_Active->DecodedData.Flags.REAL_TA;
					pRDSData_Active->DecodedData.Flags.REAL_F_GetNewTA=1;
				}
			}
		}
	}
		
	// PS
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA &&pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && GROUP0(pRDSData_Active->DecodedData.REAL_Group) && pRDSData_Active->DecodedData.Flags.REAL_F_BlockD)
	{
		pRDSData_Active->DecodedData.PIInfor.REAL_PI = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastA;

		i = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB) & 0x03;
		if ( ((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF) >= 0x20) &&
		      (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD>>8) >= 0x20)
		{
			switch(i)
			{
				case 0:
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet0 = 1;	
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[1] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[0] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
					break;
					
				case 1:
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet1 = 1;
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[3] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[2] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
					break;
					
				case 2:
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet2 = 1;
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[5] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[4] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
					break;
					
				case 3:
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet3 = 1;
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[7] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[6] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
					break;
			}
		}
		
		if (pRDSData_Active->DecodedData.Flags.REAL_F_PSGet0  && pRDSData_Active->DecodedData.Flags.REAL_F_PSGet1  &&  pRDSData_Active->DecodedData.Flags.REAL_F_PSGet2 && pRDSData_Active->DecodedData.Flags.REAL_F_PSGet3)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet0 = 0;
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet1 = 0;
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet2 = 0;
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet3 = 0;

			temp = 0;
			for (i = 0; i < 8; i++)
			{
				temp += pRDSData_Active->DecodedData.PSInfor.REAL_PS[i];
			}

			if (temp != pRDSData_Active->DecodedData.PSInfor.REAL_PSCRC && temp ==pRDSData_Active->DecodedData.PSInfor.REAL_PSCRCTemp)
			{
				pRDSData_Active->DecodedData.PSInfor.REAL_PSCRC = temp;
				pRDSData_Active->DecodedData.Flags.REAL_F_GetNewPS = 1;
				pRDSData_Active->DecodedData.Flags.REAL_F_PSExist = 1;
			}

			pRDSData_Active->DecodedData.PSInfor.REAL_PSCRCTemp = temp;
		}
	}

	// TIME
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && GROUP4A(pRDSData_Active->DecodedData.REAL_Group) && pRDSData_Active->DecodedData.Flags.REAL_F_BlockC && pRDSData_Active->DecodedData.Flags.REAL_F_BlockD)
	{
		pRDSData_Active->DecodedData.MJD_TimeInfor.REAL_TimeHour = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 12) + ((U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x0001)<<4);
		pRDSData_Active->DecodedData.MJD_TimeInfor.REAL_TimeMin = (U8)((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD&0x0FC0) >> 6);
		pRDSData_Active->DecodedData.MJD_TimeInfor.REAL_MJD = ((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB&0x0003) << 15) + (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 1);

		temp = (U16)pRDSData_Active->DecodedData.MJD_TimeInfor.REAL_TimeHour * 60 + (U16)pRDSData_Active->DecodedData.MJD_TimeInfor.REAL_TimeMin + 1440;
		if (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x0020)
		{
			temp -= ((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x001F) * 30);
		}
		else
		{
			temp += ((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD&0x001F) * 30);
		}
		temp = temp % 1440;
		pRDSData_Active->DecodedData.MJD_TimeInfor.REAL_TimeHour = temp /60;
		pRDSData_Active->DecodedData.MJD_TimeInfor.REAL_TimeMin = temp % 60;

		pRDSData_Active->DecodedData.Flags.REAL_F_GetNewCT = 1;	
	}

	// AF
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA && pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && GROUP0A(pRDSData_Active->DecodedData.REAL_Group) && pRDSData_Active->DecodedData.Flags.REAL_F_BlockC)
	{
		pRDSData_Active->DecodedData.PIInfor.REAL_PI = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastA;

		AFtemp.Dbyte = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC;
		// LF/MF
		if (AFtemp.byte[0] == 250)
		{
			AFtemp.byte[0] = 0xFF;
			AFtemp.byte[1] = 0xFF;
		}
		
		if (pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] == 0xFF && pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] == 0xFF)
		{
			// first time, just backup
			pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] = AFtemp.byte[0];
			pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] = AFtemp.byte[1];
		}
		else
		{
			if (pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] != 0xFE && pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] != 0xFE)
			{
				if ((pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] == AFtemp.byte[0] || pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] == AFtemp.byte[0]
					|| pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] == AFtemp.byte[1] || pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] == AFtemp.byte[1])
					&&(pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] != AFtemp.byte[0] || pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] != AFtemp.byte[1]))
				{
					// get method B
					pRDSData_Active->DecodedData.Flags.REAL_F_AFMethodB = 1;
				}
				pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[0] = 0xFE;
				pRDSData_Active->DecodedData.AFInfor.REAL_AFMethod[1] = 0xFE;
			}

			if (pRDSData_Active->DecodedData.Flags.REAL_F_AFMethodB && AFtemp.byte[0] > AFtemp.byte[1])
			{
				AFtemp.byte[0] = 0xFF;
				AFtemp.byte[1] = 0xFF;
			}

			for ( i = 0; i < 25; i++)
			{
				if (AFtemp.byte[0] == pRDSData_Active->DecodedData.AFInfor.REAL_AF[i] && AFtemp.byte[0] != 0xFF)
				{
					AFtemp.byte[0] = 0xFF;
				}
			}

			if (AFtemp.byte[0] < 205 && pRDSData_Active->DecodedData.AFInfor.REAL_AFNum < 25)
			{
				pRDSData_Active->DecodedData.AFInfor.REAL_AF[pRDSData_Active->DecodedData.AFInfor.REAL_AFNum] = AFtemp.byte[0];
				pRDSData_Active->DecodedData.AFInfor.REAL_AFNum++;
				pRDSData_Active->DecodedData.Flags.REAL_F_GetNewAF = 1;
				RDSStrategyData.Timers.REAL_AFTimer = 50;
			}

			for (i = 0; i < 25; i++)
			{
				if (AFtemp.byte[1] == pRDSData_Active->DecodedData.AFInfor.REAL_AF[i] && AFtemp.byte[1] != 0xFF)
				{
					AFtemp.byte[1] = 0xFF;
				}
			}
			
			if (AFtemp.byte[1] < 205 && pRDSData_Active->DecodedData.AFInfor.REAL_AFNum < 25)
			{
				pRDSData_Active->DecodedData.AFInfor.REAL_AF[pRDSData_Active->DecodedData.AFInfor.REAL_AFNum] = AFtemp.byte[1];
				pRDSData_Active->DecodedData.AFInfor.REAL_AFNum++;
				pRDSData_Active->DecodedData.Flags.REAL_F_GetNewAF = 1;
				RDSStrategyData.Timers.REAL_AFTimer = 50;
			}
			
			if (RDSStrategyData.Timers.REAL_AFTimer == 0 &&  pRDSData_Active->DecodedData.AFInfor.REAL_AFNum > 1 &&  pRDSData_Active->DecodedData.Flags.REAL_F_GetNewAF)
			{
				pRDSData_Active->DecodedData.Flags.REAL_F_GetNewAF = 0;
				pRDSData_Active->DecodedData.Flags.REAL_F_GetAFList = 1;
			}
		}

	}

	// PS (RBDS)
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA  &&  pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && pRDSData_Active->DecodedData.Flags.REAL_F_BlockC && pRDSData_Active->DecodedData.Flags.REAL_F_BlockD && GROUP15A(pRDSData_Active->DecodedData.REAL_Group))
	{
		pRDSData_Active->DecodedData.PIInfor.REAL_PI = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastA;

		i = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB) & 0x01;
		if ((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF) >= 0x20 
			&& (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8) >= 0x20
			&&(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF) >= 0x20 
			&& (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8) >= 0x20)
		{
			switch(i)
			{
				case 0:
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet0 = 1;
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet1 = 1;
					
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[1] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[0] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[3] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[2] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
					break;
					
				case 1:
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet2 = 1;
					pRDSData_Active->DecodedData.Flags.REAL_F_PSGet3 = 1;
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[5] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[4] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[7] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF);
					pRDSData_Active->DecodedData.PSInfor.REAL_PS[6] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
					break;
			}
		}
		
		if (pRDSData_Active->DecodedData.Flags.REAL_F_PSGet0 && pRDSData_Active->DecodedData.Flags.REAL_F_PSGet1 && pRDSData_Active->DecodedData.Flags.REAL_F_PSGet2 && pRDSData_Active->DecodedData.Flags.REAL_F_PSGet3)
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet0 = 0;
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet1 = 0;
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet2 = 0;
			pRDSData_Active->DecodedData.Flags.REAL_F_PSGet3 = 0;

			temp = 0;
			for (i = 0; i < 8; i++)
			{
				temp += pRDSData_Active->DecodedData.PSInfor.REAL_PS[i];
			}

			if (temp != pRDSData_Active->DecodedData.PSInfor.REAL_PSCRC && temp == pRDSData_Active->DecodedData.PSInfor.REAL_PSCRCTemp)
			{
				pRDSData_Active->DecodedData.PSInfor.REAL_PSCRC = temp;
				pRDSData_Active->DecodedData.Flags.REAL_F_GetNewPS = 1;
				pRDSData_Active->DecodedData.Flags.REAL_F_PSExist = 1;
			}

			pRDSData_Active->DecodedData.PSInfor.REAL_PSCRCTemp = temp;
		}
	}
	
// EON
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA && pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && GROUP14A(pRDSData_Active->DecodedData.REAL_Group) && pRDSData_Active->DecodedData.Flags.REAL_F_BlockC && pRDSData_Active->DecodedData.Flags.REAL_F_BlockD)
	{
		pRDSData_Active->DecodedData.PIInfor.REAL_PI = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastA;

		temp = (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB) & 0x000F;

		switch(temp)
		{
			// EON PS
			case 0:
				for (j = 0; j < EONBuffSize; j++)
				{
					if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD)
					{
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][1] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF);
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][0] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8);
					}
				}
				break;
				
			case 1:
				for (j = 0; j < EONBuffSize; j++)
				{
					if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD)
					{
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][3] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF);
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][2] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8);
					}
				}
				break;
				
			case 2:
				for (j = 0; j < EONBuffSize; j++)
				{
					if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD)
					{
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][5] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF);
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][4] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8);
					}
				}
				break;
				
			case 3:
				for (j = 0; j <EONBuffSize; j++)
				{
					if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD)
					{
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][7] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF);
						pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPS[j][6] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8);
					}
				}
				break;

			// EON AF
			case 4:
				AFtemp.Dbyte = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC;

				for (j = 0; j < EONBuffSize; j++)
				{
					if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == 0 ||pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD)
					{
						if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == 0)
						{
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD;
						}

						for (i = 0; i < 25; i++)
						{
							if (AFtemp.byte[0] == pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAF[j][i])AFtemp.byte[0] = 0xFF;
							if (AFtemp.byte[1] == pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAF[j][i])AFtemp.byte[1] = 0xFF;
						}
						
						if (AFtemp.byte[0] < 205 && pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j] < 25)
						{
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAF[j][pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j]] = AFtemp.byte[0];
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j]++;
						}
						
						if (AFtemp.byte[1] < 205 && pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j] < 25)
						{
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAF[j][pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j]] = AFtemp.byte[1];
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j]++;
						}
						break;
					}
				}
				break;

			case 5:
			case 6:
			case 7:
			case 8:
				AFtemp.Dbyte = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC;

				for (j = 0; j < EONBuffSize; j++)
				{
					if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == 0 || pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD)
					{
						if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == 0)
						{
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] = pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD;
						}

						for (i = 0; i < 25; i++)
						{
							if (AFtemp.byte[1] == pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAF[j][i])  AFtemp.byte[1] = 0xFF;
						}
						
						if (AFtemp.byte[1] < 205 && pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j] < 25)
						{
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAF[j][pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j]] = AFtemp.byte[1];
							pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONAFNum[j]++;
						}

						break;
					}
				}

				break;

		}
	}

	// EON TP
	// EON TA
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA && pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && pRDSData_Active->DecodedData.Flags.REAL_F_BlockCp && pRDSData_Active->DecodedData.Flags.REAL_F_BlockD && GROUP14B(pRDSData_Active->DecodedData.REAL_Group))
	{
		flag = 0;
		for (j = 0; j < EONBuffSize; j++)
		{
			if (pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] == pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD && pRDSData_Active->DecodedData.EONInfor.REAL_EONSave_EONPI[j] != 0)
			{
				if (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0010)
				{
					REAL_EONSave_EONTP(pRDSData_Active->DecodedData, j) = 1;
				}
				else
				{
					REAL_EONSave_EONTP(pRDSData_Active->DecodedData, j) = 0;
				}
				
				if (REAL_EONSave_EONTPLast(pRDSData_Active->DecodedData, j) !=  REAL_EONSave_EONTP(pRDSData_Active->DecodedData, j))
				{
					REAL_EONSave_EONTPLast(pRDSData_Active->DecodedData, j) = REAL_EONSave_EONTP(pRDSData_Active->DecodedData, j);
					REAL_EONSave_F_GetNewEONTP(pRDSData_Active->DecodedData, j) = 1;
				}

				if (pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0008)
				{
					REAL_EONSave_EONTA(pRDSData_Active->DecodedData, j) = 1;
				}
				else
				{
					REAL_EONSave_EONTA(pRDSData_Active->DecodedData, j) = 0;
				}
				
				if (REAL_EONSave_EONTALast(pRDSData_Active->DecodedData, j) !=  REAL_EONSave_EONTA(pRDSData_Active->DecodedData, j))
				{
					REAL_EONSave_EONTALast(pRDSData_Active->DecodedData, j) = REAL_EONSave_EONTA(pRDSData_Active->DecodedData, j);
					REAL_EONSave_F_GetNewEONTA(pRDSData_Active->DecodedData, j) = 1;
				}

				flag = 1;
			}
		}
		
		pRDSData_Active->DecodedData.EONInfor.REAL_EONExitTimer = 50;
	}

	// RT
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA && pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && GROUP2A(pRDSData_Active->DecodedData.REAL_Group) && pRDSData_Active->DecodedData.Flags.REAL_F_BlockC && pRDSData_Active->DecodedData.Flags.REAL_F_BlockD)
	{
		if (((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0010) && (!pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag))
			||((!(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0010)) && (pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag)))
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag = ~pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag;
			for (i = 0; i < 64; i++)
			{
				pRDSData_Active->DecodedData.REAL_RT[i] = 0;
			}
		}
		
		i = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB) & 0x0F;
		pRDSData_Active->DecodedData.REAL_RT[i*4      ] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC >> 8);
		pRDSData_Active->DecodedData.REAL_RT[i*4 + 1] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastC & 0x00FF);
		pRDSData_Active->DecodedData.REAL_RT[i*4 + 2] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
		pRDSData_Active->DecodedData.REAL_RT[i*4 + 3] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD & 0x00FF);

		pRDSData_Active->DecodedData.Flags.REAL_F_GetNewRT = 1;
	}

	// RT 2B
	/*
	if (pRDSData_Active->DecodedData.Flags.REAL_F_BlockA && pRDSData_Active->DecodedData.Flags.REAL_F_BlockB && GROUP2B(pRDSData_Active->DecodedData.REAL_Group)  && pRDSData_Active->DecodedData.Flags.REAL_F_BlockD)
	{

		if (((pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0010) && (!pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag))
			||((!(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB & 0x0010)) && (pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag)))
		{
			pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag = ~pRDSData_Active->DecodedData.Flags.REAL_F_RTABFlag;
			for (i = 0; i < 64; i++)
			{
				pRDSData_Active->DecodedData.REAL_RT[i] = 0;
			}
		}
		
		i = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastB) & 0x0F;
		pRDSData_Active->DecodedData.REAL_RT[i * 2] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD >> 8);
		pRDSData_Active->DecodedData.REAL_RT[i * 2 + 1] = (U8)(pRDSData_Active->DecodedData.Blocks.REAL_BlockLastD& 0x00FF);

		pRDSData_Active->DecodedData.Flags.REAL_F_GetNewRT = 1;
	}
	*/
	
}
#endif

/*************************************************************************************
Function		: RDSDataExtract
Description	: Internal function for RDS Data extraction which will process the blocks one by one and 
			  extract data by group if group data buffer is not empty.
Parameters	: tunerChannel : main or background tuner channel.
			  
Return Value	: None.
***********************************************************************************/
#if 1
void RDSDataExtract(eTuner_Channel tunerChannel)
{
#if 1
    int i;
    RDSData_Process *pRDSData_Active;
    pRDSData_Active =  &(RDSData.RDSData_Main) ;
    
    //printf("pRDSData_Active->RawDataBuffer.BlockCnt:%d\n", pRDSData_Active->RawDataBuffer.BlockCnt);
    
    if(pRDSData_Active->RawDataBuffer.BlockCnt == 0)  return;
    if(pRDSData_Active->RawDataBuffer.BlockCnt > 0)
    {
    	for(i = 0; i < pRDSData_Active->RawDataBuffer.BlockCnt; i++)
    	{		
    		pRDSData_Active->BlocksBuffer.RDSData[0] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 0];
    		pRDSData_Active->BlocksBuffer.RDSData[1] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 1];
    		pRDSData_Active->BlocksBuffer.RDSData[2] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 2];
			RDSDataProcessBlock(tunerChannel);
    	}
		RDSDataExtractGroup(tunerChannel);//2021.03.18增加
    }

#else
	int i;
	RDSData_Process *pRDSData_Active;
	pRDSData_Active =  &(RDSData.RDSData_Main) ;
	
	//cout("pRDSData_Active->RawDataBuffer.BlockCnt:%d\n", pRDSData_Active->RawDataBuffer.BlockCnt);
	
	if (pRDSData_Active->RawDataBuffer.BlockCnt == 0)  return;
	if ( pRDSData_Active->RawDataBuffer.BlockCnt > 0)
	{
		for (i = 0; i < pRDSData_Active->RawDataBuffer.BlockCnt; i++)
		{		
			pRDSData_Active->BlocksBuffer.RDSData[0] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 0];
			pRDSData_Active->BlocksBuffer.RDSData[1] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 1];
			pRDSData_Active->BlocksBuffer.RDSData[2] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 2];
			RDSDataProcessBlock(tunerChannel);
		}
		RDSDataExtractGroup(tunerChannel);//2021.03.18增加
	}
#endif
}

#else
void RDSDataExtract(eTuner_Channel tunerChannel)
{
	int i;
	RDSData_Process *pRDSData_Active;
	pRDSData_Active =  &(RDSData.RDSData_Main) ;
	
	//cout("pRDSData_Active->RawDataBuffer.BlockCnt:%d\n", pRDSData_Active->RawDataBuffer.BlockCnt);
	
	if (pRDSData_Active->RawDataBuffer.BlockCnt == 0)  return;
	if ( pRDSData_Active->RawDataBuffer.BlockCnt > 0)
	{
		for (i = 0; i < pRDSData_Active->RawDataBuffer.BlockCnt; i++)
		{		
			pRDSData_Active->BlocksBuffer.RDSData[0] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 0];
			pRDSData_Active->BlocksBuffer.RDSData[1] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 1];
			pRDSData_Active->BlocksBuffer.RDSData[2] = pRDSData_Active->RawDataBuffer.BufferData[i * 3 + 2];
			RDSDataProcessBlock(tunerChannel);
		}
	}
}
#endif

/*************************************************************************************
Function		: RDSData_SetDataBlock
Description	: Set RDS data F_RDSStation.
Parameters	: tunerChannel : main or background tuner channel.
			  input : the F_RDSStation
			  
Return Value	: None.
***********************************************************************************/
void RDSData_SetDataBlock(eTuner_Channel tunerChannel, u8 input)
{
	RDSData_Process *pRDSData_Active;
	
	pRDSData_Active =  &(RDSData.RDSData_Main) ;
	
	pRDSData_Active->DecodedData.Flags.REAL_F_RDSStation = input;
}
#endif

/*********************************************
	Function:		SeekOff
	Description:	exit auto seek or auto seek continue
*********************************************/
void SeekOff(unsigned char SeekEndAction)
{

	if (SeekEndAction == SeekContinue)
	{
		TunerSeekContinue(Radio.TunerChannel);
	}
	else
	{
		TunerSeekOff(Radio.TunerChannel, SeekEndAction);
	}
}


void TunerSearchFreq(eTuner_Channel tunerChannel,U32 TargetFreq)
{
	Star_Cmd_Tuner_Seek_Start(tunerChannel, 0x08, TargetFreq);
}


u8 TunerCheckReady(eTuner_Channel tunerChannel,u8 req)
{
	u8 busyStatus;
	static u8 BusyBitChkCounter = 0;
	if(req)
	{
		BusyBitChkCounter = 0;
		return TRUE;
	}
	Wait1ms(1);
	busyStatus = Star_GetTunerBusyStatus(tunerChannel);	
	BusyBitChkCounter++;
	if((busyStatus == 0) || (BusyBitChkCounter >= 20))
	{
		BusyBitChkCounter = 0;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}	

/*************************************************************************************
Function		: TunerStatusUpdate
Description	: Read out the tuner current quality information and stereo indicator and/or some debug status

Parameters	: None

Return Values	: None
*************************************************************************************/
void TunerStatusUpdate(void)
{
	//Star_GetAverageQuality(3);
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
void tda7708_tuner_init(void)
{
	Tuner.TunerError = 0x00;
	Tuner.TunerErrorCnt = 0;
	
	Tuner.VPAmode = OFF; 

	Tuner.mainFreq = 0x00;
	Tuner.backgroundFreq = 0x00;

	Tuner.rdsConfig[0].CSR = 0x00;		//Main Channel
	Tuner.rdsConfig[0].NBR = 0x00;
	Tuner.rdsConfig[1].CSR = 0x00;		//Background Channel
	Tuner.rdsConfig[1].NBR = 0x00;
}

static uint8 tda7708_set_init(void)
{
	ew_puts("tda7708_set_init\n");
	if(!tda7708_ctrl->init_succeed)
	{
		tda7708_tuner_init();

		if(!TunerCheckChipType())return FALSE;
		
		Wait1ms(2);//2MS
		Star_Download_BootCode(1);
		Wait1ms(2);//1MS
		if(!Star_Download_BootCode(0))return FALSE;
		OSTimeDly(30);//300MS
				
		if(!Star_checkdspinit())return FALSE;
	}

	ew_puts("tda7708_set_init_ok\n");
    return TRUE;
}

static tda7708_band_e tda7708_get_band(void)
{
	switch(tda7708_ctrl->TunerBand)
	{
		case TDA7708_TUNER_BAND_REC_FM:
			return TDA7708_BAND_FM;

		case TDA7708_TUNER_BAND_REC_MW:
			return TDA7708_BAND_AM;

		case TDA7708_TUNER_BAND_REC_SW:
			return TDA7708_BAND_SW;

		case TDA7708_TUNER_BAND_REC_BW:
			return TDA7708_BAND_WB;

		default:
			return TDA7708_BAND_MAX;
	}
}

//static uint8 tda7708_change_band(eTuner_Channel tunerChannel,  uint8  bandReq , uint16 min_Freq, uint16 max_Freq, uint16 seek_Step, uint32 current_Freq)
void tda7708_change_band(tda7708_band_e band, uint16 min_freq, uint16 max_freq, uint32 current_freq)
{
	printf("%s, band:%d, max_freq:%d, min_freq:%d\n", __FUNCTION__, band, max_freq, min_freq);
	U32  minFreq, maxFreq, currentFreq;
	uTunerProcessFeature tunerProcessFeature;
	
	tunerProcessFeature.value = 0;

	Radio.MinFreq = min_freq;
	Radio.MaxFreq = max_freq;
	Radio.Freq = current_freq;
	
	//RadioGetMaxMinFreq(bandReq, &maxFreq, &minFreq, &currentFreq, &seekStep);		// To be compatible with the original ELITE/HIT2 sw.
	RadioGetMaxMinFreq(band, &maxFreq, &minFreq, &currentFreq);		// To be compatible with the original ELITE/HIT2 sw.
		
	printf("%s, band:%d, max_freq:%d, min_freq:%d\n", __FUNCTION__, band, maxFreq, minFreq);
	
	if(band == TDA7708_BAND_FM)
	{
		tunerProcessFeature.value	= (Tuner.VPAmode ? 1 : 0);

		Tuner.Band = eTuner_Mode_FM;
		Star_Cmd_Tuner_Change_Band(eTuner_Channel_Main, Tuner.Band, tunerProcessFeature, minFreq, maxFreq);

		Wait1ms(20);   

	//#if 1//def AUTOSEEK
		Star_Cmd_Tuner_Set_Seek_Threshold(eTuner_Channel_Main, SEEK_FM_FS_THRESHOLD, SEEK_FM_DT_THRESHOLD, SEEK_FM_ADJ_THRESHOLD, SEEK_FM_MPP_THRESHOLD, SEEK_FM_SNR_THRESHOLD, SEEK_FM_MPXN_THRESHOLD, SEEK_THRESHOLD_NOTUSED);
	//#endif

		TunerSetFreq(eTuner_Channel_Main, currentFreq);
	}	
	else if(band == TDA7708_BAND_AM|| band == TDA7708_BAND_SW)	//Change to AM band from other band
	{
		/* set the band's range and tune to the frequency in this band as well */
		if((tda7708_area == TDA7708_AREA_EUROPE)||(tda7708_area == TDA7708_AREA_OIRT)||(tda7708_area == TDA7708_AREA_JAPAN))
		{
			Tuner.Band = eTuner_Mode_AM_EU;
			Star_Cmd_Tuner_Change_Band(eTuner_Channel_Main, Tuner.Band, tunerProcessFeature, minFreq, maxFreq);
			Wait1ms(10);  
			/* SetSeekParameter if use auto seek method in Star, need fine tune the parameters */
		//#if 1//def AUTOSEEK
			Star_Cmd_Tuner_Set_Seek_Threshold(eTuner_Channel_Main, SEEK_AM_FS_THRESHOLD, SEEK_AM_DT_THRESHOLD, SEEK_AM_ADJ_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_AM_SNR_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_THRESHOLD_NOTUSED);
		//#endif
		}
		else
		{
			Tuner.Band = eTuner_Mode_AM_US;
			Star_Cmd_Tuner_Change_Band(eTuner_Channel_Main, Tuner.Band, tunerProcessFeature, minFreq, maxFreq);
			Wait1ms(10);  
			/* SetSeekParameter if use auto seek method in Star, need fine tune the parameters */
		//#if 1//def AUTOSEEK
			Star_Cmd_Tuner_Set_Seek_Threshold(eTuner_Channel_Main, SEEK_AM_FS_THRESHOLD, SEEK_AM_DT_THRESHOLD, SEEK_AM_ADJ_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_AM_SNR_THRESHOLD, SEEK_THRESHOLD_NOTUSED, SEEK_THRESHOLD_NOTUSED);
		//#endif
		}
		
		Wait1ms(20);   
		
		TunerSetFreq(eTuner_Channel_Main, currentFreq);
	}
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
#if defined(_ENABLE_RADIO_RDS_)
static void tda7708_rds_reset(void)
{
	if(tda7708_ctrl->tuner_rds.enable)
	{
		rds_dec_reset();
		tda7708_ctrl->tuner_rds.discard = 1;
		tda7708_ctrl->tuner_rds.quality = 0;

		tda7708_ctrl->tuner_rds.block_cnt = 1;
		tda7708_ctrl->tuner_rds.get_block = 0;
	}
}

//u8 mute=0;
static void tda7708_rds_decode(void)
{
	/*if(mute)
	{
		mute =0;
		AUDIO_MUTE_OFF();
	}
	else
	{
		mute = 1;
		AUDIO_MUTE_ON();
	}*/

	if(!tda7708_ctrl->tuner_rds.enable)return;

	if(tda7708_ctrl->tuner_rds.quality >= tda7708_ctrl->tuner_rds.factor)
	{
		tda7708_ctrl->tuner_rds.quality -= tda7708_ctrl->tuner_rds.factor;
	}
	rds_dec_timer();

	RDSBufferFetchBlocks(eTuner_Channel_Main);
}
#endif

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

static uint8 tuner_tda7708_detect(void);
static uint8 tuner_tda7708_init(radio_band_e, uint16, uint16, uint16);
static uint8 tuner_tda7708_exit(void);
static uint8 tuner_tda7708_freq(uint16);
static uint8 tuner_tda7708_status(engine_status_t *);
static uint8 tuner_tda7708_search(uint16, engine_status_t *);
static uint8 tuner_tda7708_hard(engine_hard_t *hard);
static uint8 tuner_tda7708_setting(fm_setting_e, void *);
static uint8 tuner_tda7708_info(fm_info_e, void *);


static uint8 tuner_tda7708_detect(void)
{
	uint8 r = check_detect_tda7708();

	tuner_printf("tuner_tda7708_detect:%d\n", r);
	//return r;
	return 1;
}

static uint8 tuner_tda7708_init(radio_band_e band, uint16 min_freq, uint16 max_freq, uint16 freq)
{
	ew_printf("%s, band:%d, min_freq:%d, max_freq:%d, freq:%d\n", __FUNCTION__, band, min_freq, max_freq, freq);

	if(tda7708_ctrl == NULL)
	{
		tda7708_ctrl = (tda7708_ctrl_t *)malloc_fun(tda7708_ctrl, sizeof(tda7708_ctrl_t), 0);
		if(tda7708_ctrl == NULL)
		{
			return FALSE;
		}
		tda7708_ctrl->area = TDA7708_AREA_EUROPE;
	}

	ew_puts("tda7708_memory_ok\n");

	switch(band)
	{
		case RADIO_BAND_FM:
			if(min_freq > max_freq)goto tda7708_init_error;
			if(min_freq < 6400)goto tda7708_init_error;
			if(max_freq > 10800)goto tda7708_init_error;
			tda7708_ctrl->TunerBand = TDA7708_TUNER_BAND_REC_FM;
			break;

		case RADIO_BAND_AM:
	    #if defined(_SEEK_REFRESH_FREQ_)
	    	global.pty_flag = 0;
	    #endif
			if(min_freq > max_freq)goto tda7708_init_error;
			if(min_freq < 520)goto tda7708_init_error;
			if(max_freq > 1710)goto tda7708_init_error;
			tda7708_ctrl->TunerBand = TDA7708_TUNER_BAND_REC_MW;
			break;

		default:
			goto tda7708_init_error;
			break;
	}

	if(tda7708_ctrl->band == band)
	{
		if(tda7708_ctrl->init_succeed)
		{
			if(tda7708_ctrl->now_freq == freq)
			{
				tda7708_ctrl->min_freq = min_freq;
				tda7708_ctrl->max_freq = max_freq;
				if(!tuner_tda7708_setting(TUR_SET_MUTE, OFF))goto tda7708_init_error;
				return TRUE;
			}
			else
			{
				goto tda7708_init_freq;
			}
		}
		else
		{
			goto tda7708_init_error;
		}
	}

	if(!tda7708_set_init())goto tda7708_init_error;

tda7708_init_freq:
	ew_puts("tda7708_init_freq\n");
	//if(!tda7708_change_band(tda7708_get_band(), min_freq, max_freq, freq))return FALSE;
	tda7708_change_band(tda7708_get_band(), min_freq, max_freq, freq);
#if defined(_ENABLE_RADIO_RDS_)
	RDSInit();
	TunerRDSBufferEnable(eTuner_Channel_Main,0);   //Polling mode
#endif
	tda7708_ctrl->band = band;
	tda7708_ctrl->min_freq = min_freq;
	tda7708_ctrl->max_freq = max_freq;
	tda7708_ctrl->init_succeed = TRUE;

	engine_status_t status;
	if(!tuner_tda7708_search(freq, &status))goto tda7708_init_error;
	//OSTimeDly(2);
	//if(!tuner_tda7708_search(freq, &status))goto tda7708_init_error;
	if(!tuner_tda7708_setting(TUR_SET_MUTE, OFF))goto tda7708_init_error;
	ew_puts("TDA7708 Init Succeed!!!!!\n");
	return TRUE;

tda7708_init_error:
	free_fun((void **)&tda7708_ctrl);
	ew_puts("TDA7708 Init Fail!!!!!\n");
	return FALSE;
}

static uint8 tuner_tda7708_exit(void)
{
	ew_puts("tuner_tda7708_exit\n");
	if(tda7708_ctrl)
	{
		free_fun((void **)&tda7708_ctrl);
	}

	return TRUE;
}

static uint8 tuner_tda7708_freq(uint16 freq)
{
	if(tda7708_ctrl == NULL)return FALSE;
	if(!tda7708_ctrl->init_succeed)return FALSE;
	if(freq < tda7708_ctrl->min_freq)return FALSE;
	if(freq > tda7708_ctrl->max_freq)return FALSE;

	ew_printf("%s, freq:%d\n", __FUNCTION__, freq);

	unsigned long pBandFreq;
	if(tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_FM)
	{
		pBandFreq = (unsigned long)freq* 10;
	}
	else if (tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_MW)
	{
		pBandFreq = (unsigned long)((freq) );
	}
	else if (tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_SW)
	{
		pBandFreq = (unsigned long)((freq- 1384) * 5);
	}
	else
	{
		pBandFreq = (unsigned long)(freq);
	}
	
	//if(tda7708_info.auto_seek)
	//	TunerSearchFreq(eTuner_Channel_Main, pBandFreq);
	//else
	TunerSetFreq(eTuner_Channel_Main, pBandFreq);

#if defined(_ENABLE_RADIO_RDS_)
	tda7708_rds_reset();
#endif

	tda7708_ctrl->now_freq = freq;
	tda7708_ctrl->signal = 0;
	tda7708_ctrl->last_sys_timer = g_rand_seed;
	return TRUE;
}

static uint8 tuner_tda7708_status(engine_status_t *status)
{
	if(tda7708_ctrl == NULL)return FALSE;
	if(!tda7708_ctrl->init_succeed)return FALSE;

	uint8 fm_mode 	= (tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_FM) ? 1 : 0;
	status->band 	= fm_mode ? RADIO_BAND_FM : RADIO_BAND_AM;
	status->freq 	= tda7708_ctrl->now_freq;
	status->signal 	= tda7708_ctrl->signal;
	status->stc 	= FALSE;
	status->valid   = FALSE;
	status->stereo 	= FALSE;
	memset(&Radio_detect, 0, sizeof(Radio_detect));

	tda7708_status_t *Getstatus = NULL;
	memset(Getstatus, 0, sizeof(tda7708_status_t));
	Star_GetAverageQuality(3, tda7708_get_band(), Getstatus);
		
#if defined(_SEEK_REFRESH_FREQ_)
	if(setup.rds.af && setup.rds.ta && global.pty_flag)
	{
		OSTimeDly(1);
	}
#endif
	
	status->signal	= Radio_detect.Smeter;
	status->stc = TRUE;

	if(tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_FM)
	{			
		//printf("status->freq:%d\n",status->freq);
		//printf("smeter:0x%x\n",Radio_detect.Smeter);
		//printf("detuning:0x%x\n",Radio_detect.Detuning);
		//printf("multipath:0x%x\n",Radio_detect.Multipath);
		//printf("MpxNoise:0x%x\n",Radio_detect.MpxNoise);
		//printf("SNR:0x%x\n",Radio_detect.SNR);
		//printf("adj_channel:0x%x\n",Radio_detect.AdjChannel);
		if(Radio_detect.Smeter > 0xf0)
		{
			Radio_detect.Smeter = 0;
		}
		if(Radio_detect.AdjChannel > 0xfe)
		{
			Radio_detect.AdjChannel = 0;
		}
		if(Radio_detect.Smeter	>= TDA7708_TUNER_RSSI_FM &&  
		  Radio_detect.Detuning	< SEEK_FM_DT_THRESHOLD	&&
		  Radio_detect.Multipath< SEEK_FM_MPP_THRESHOLD && 
		  Radio_detect.MpxNoise	< SEEK_FM_MPXN_THRESHOLD&& 
		  Radio_detect.SNR 		> SEEK_FM_SNR_THRESHOLD &&
		  Radio_detect.AdjChannel > SEEK_FM_ADJ_THRESHOLD)
		{
			status->valid = TRUE;
		}
		else
		{
			status->valid = FALSE;
		}

		if(status->signal > TDA7708_TUNER_ST_RSSI)
		{
			if(Radio.F_Stereo)
			{
				status->stereo = TRUE;
			}
		}
	}
	else if (tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_MW)
	{
		printf("smeter:%d, detuning:%d, adj_channel:%d\n", Radio_detect.Smeter, Radio_detect.Detuning, Radio_detect.AdjChannel);
		if(Radio_detect.Smeter > 0xd0)
		{
			Radio_detect.Smeter = 0;
		}
		if((Radio_detect.Smeter		>= TDA7708_TUNER_RSSI_AM &&  
			Radio_detect.Detuning	< SEEK_AM_DT_THRESHOLD && 
			Radio_detect.AdjChannel > SEEK_AM_ADJ_THRESHOLD))
		{
			status->valid = TRUE;
		}
		else
		{
			status->valid = FALSE;
		}
	}
	else
	{
		if((Radio_detect.Smeter >= SEEK_WX_FS_THRESHOLD)  &&  
		(Radio_detect.Detuning	< SEEK_WX_DT_THRESHOLD))
		{
			status->valid = TRUE;
		}
		else
		{
			status->valid = FALSE;
		}
	}

	/*if(status->valid)//不要停50K步进电台
	{
		if(ResumeCustomerInfo.Factory.RadioDisable50K){
			if(tda7708_info.now_freq % 10 == 5)
			{
				status->valid = FALSE;
			}
		}
	}*/
	return TRUE;
}

static uint8 tuner_tda7708_search(uint16 freq, engine_status_t *status)
{
	ew_printf("%s, freq:%d\n", __FUNCTION__, freq);

	if(tda7708_ctrl == NULL)return FALSE;
	if(!tda7708_ctrl->init_succeed)return FALSE;
	if(freq < tda7708_ctrl->min_freq)return FALSE;
	if(freq > tda7708_ctrl->max_freq)return FALSE;

	if(!tuner_tda7708_freq(freq))return FALSE;
		
	OSTimeDly(5);
	
	uint8 time_over = 40;
	do{
		OSTimeDly(1);
		if(!tuner_tda7708_status(status))return FALSE;
	}while(!status->stc && (--time_over));

	return TRUE;
}

static uint8 tuner_tda7708_setting(fm_setting_e type, void *set)
{
	if(tda7708_ctrl == NULL)return FALSE;
	if(!tda7708_ctrl->init_succeed)return FALSE;

	switch(type)
	{
		case TUR_SET_MUTE:
			{
				uint8 mute = (uint8)set;
				//if(!tda7708_set_mute(mute))return FALSE;
				if(mute)
					Star_Cmd_Tuner_Audio_Mute(eMute);
				else
					Star_Cmd_Tuner_Audio_Mute(eUnmute);
			}
			break;

		case TUR_SET_STEREO:
			{
				uint8 stereo = (uint8)set;
				if(tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_FM)
				{
					//if(!tda7708_set_stereo(stereo))return FALSE;
					if(stereo)
						Star_Cmd_Tuner_FM_Stereo_Mode(eAutoMode);
					else
						Star_Cmd_Tuner_FM_Stereo_Mode(eForceMono);
				}
				else return FALSE;
			}
			break;

	#if defined(_ENABLE_RADIO_RDS_)
		case TUR_SET_RDS:
			{
				typedef struct{
					uint8 enable;
					uint16 timer;
					uint8 result;
				}tuner_rds_ctrl_t;
				tuner_rds_ctrl_t *rds_ctrl = ((tuner_rds_ctrl_t *)set);

				if(rds_ctrl->enable)
				{
					if(tda7708_ctrl->TunerBand == TDA7708_TUNER_BAND_REC_FM)
					{
						if((tda7708_ctrl->min_freq >= 8750) && (tda7708_ctrl->max_freq <= 10800))
						{
							if(!tda7708_ctrl->tuner_rds.enable)
							{
								int check = FALSE;
								if(!tuner_tda7708_info(TUR_INFO_RDS, &check))return FALSE;
								if(check)
								{
									tda7708_ctrl->tuner_rds.factor = 255/(_RDS_QUALITY_TIME_/rds_ctrl->timer);
									tda7708_ctrl->tuner_rds.enable = TRUE;
									tda7708_rds_reset();
									printf("rds_ctrl->timer:%d\n",rds_ctrl->timer);
									rds_dec_det_time(rds_ctrl->timer);
									rds_ctrl->result = TRUE;
								}
							}
						}
						else return FALSE;
					}
					else return FALSE;
				}
				else
				{
					if(tda7708_ctrl->tuner_rds.enable)
					{
						tda7708_ctrl->tuner_rds.enable = FALSE;
						rds_ctrl->result = TRUE;
					}
					else return FALSE;
				}
			}
			break;

		case TUR_SET_RDS_RESET:
			{
				if(tda7708_ctrl->tuner_rds.enable)
				{
					tda7708_rds_reset();
				}
				else return FALSE;
			}
			break;

		case TUR_SET_RDS_DECODE:
			{
		    #if defined(_SEEK_REFRESH_FREQ_)
		    	if((tda7708_ctrl->tuner_rds.enable)&&(setup.rds.af || setup.rds.ta || global.pty_flag))
		    #else
		    	if(tda7708_ctrl->tuner_rds.enable)
		    #endif
				{
					tda7708_rds_decode();
				}
				else return FALSE;
			}
			break;

		case TUR_SET_RDS_EON:
			{
				if(tda7708_ctrl->tuner_rds.enable)
				{
					uint8 disable = (uint8)set;
					rds_dec_eon(disable);
				}
				else return FALSE;
			}
			break;

		case TUR_SET_RDS_SAVE:
			if(set != NULL)
			{
				if(tda7708_ctrl->tuner_rds.enable)
				{
					radio_rds_station_t *write_station = (radio_rds_station_t *)set;
					rds_dec_save(write_station);
				}
				else return FALSE;
			}
			else return FALSE;
			break;
	#endif

		default:
			return FALSE;
	}
	return TRUE;
}

static uint8 tuner_tda7708_info(fm_info_e type, void *info)
{
	switch(type)
	{
		case TUR_INFO_DRV:
			if(info != NULL)
			{
				*(uint8 *)info = TUN_DRV_TDA7708;
			}
			break;

		case TUR_INFO_FM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = TDA7708_TUNER_RSSI_FM;
			}
			break;

		case TUR_INFO_FM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = TDA7708_TUNER_RSSI_LOC_FM;
			}
			break;

		case TUR_INFO_FM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(fm_tda7708_deal_spot_tab)/sizeof(uint16);
			}
			break;

		case TUR_INFO_FM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)fm_tda7708_deal_spot_tab;
			}
			break;

		case TUR_INFO_AM_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = TDA7708_TUNER_RSSI_AM;
			}
			break;

		case TUR_INFO_AM_LOC_RSSI:
			if(info != NULL)
			{
				*(uint8 *)info = TDA7708_TUNER_RSSI_LOC_AM;
			}
			break;

		case TUR_INFO_AM_DEAD_SPOT_LENGTH:
			if(info != NULL)
			{
				*(uint8 *)info = sizeof(am_tda7708_deal_spot_tab)/sizeof(uint16);
			}
			break;

		case TUR_INFO_AM_DEAD_SPOT_TAB:
			if(info != NULL)
			{
				*(int *)info = (int)am_tda7708_deal_spot_tab;
			}
			break;

		case TUR_INFO_AUTOSEEK:
			if(info != NULL)
			{
				*(uint8 *)info = DISABLE;
			}
			break;

	#if defined(_ENABLE_RADIO_RDS_)
		case TUR_INFO_RDS:
			if(info != NULL)
			{
				*(uint8 *)info = TRUE;
			}
			break;

		case TUR_INFO_RDS_INFO:
			if(info != NULL)
			{
				if(tda7708_ctrl == NULL)return FALSE;
				if(!tda7708_ctrl->init_succeed)return FALSE;
				if(!tda7708_ctrl->tuner_rds.enable)return FALSE;

				radio_rds_info_t *rds_info = (radio_rds_info_t *)info;
				rds_dec_info(rds_info);
			}
			break;

		case TUR_INFO_RDS_APP:
			if(info != NULL)
			{
				if(tda7708_ctrl == NULL)return FALSE;
				if(!tda7708_ctrl->init_succeed)return FALSE;
				if(!tda7708_ctrl->tuner_rds.enable)return FALSE;

				radio_rds_app_t *rds_app = (radio_rds_app_t *)info;
				engine_status_t status;
				if(!tuner_tda7708_status(&status))return FALSE;
				rds_app->freq	= status.freq;
				rds_app->level	= status.signal;
				rds_app->stc	= status.stc;
				rds_app->found	= status.valid;
				rds_app->signal = tda7708_ctrl->tuner_rds.quality;
				rds_dec_app(rds_app);
			}
			break;

		case TUR_INFO_RDS_LOAD:
			if(info != NULL)
			{
				if(tda7708_ctrl == NULL)return FALSE;
				if(!tda7708_ctrl->init_succeed)return FALSE;
				if(!tda7708_ctrl->tuner_rds.enable)return FALSE;

				radio_rds_station_t *read_station = (radio_rds_station_t *)info;
				rds_dec_load(read_station);
			}
			break;
	#endif

		default:
			return FALSE;
	}
	return TRUE;
}

const tuner_drv_fun_t tuner_tda7708_drv_fun =
{
	tuner_tda7708_detect,
	tuner_tda7708_init,
	tuner_tda7708_exit,
	tuner_tda7708_freq,
	tuner_tda7708_status,
	tuner_tda7708_search,
	NULL,
	tuner_tda7708_setting,
	tuner_tda7708_info,
};

#endif

