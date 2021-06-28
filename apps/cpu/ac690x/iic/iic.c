#include "sdk_cfg.h"
#include "iic.h"


#if defined(_ENABLE_I2C_DRV_)

static uint16 i2c_rate_delay = 0;
static uint8 i2c_dev_addr = 0;
static uint8 i2c_work_flag = FALSE;
static const i2c_gpio_t *i2c_gpio = NULL;

static void i2c_delay(void)
{
	uint16 i = i2c_rate_delay;
	
	while(i)
	{
		asm volatile ("nop");
		i--;
	}
}

void i2c_start_sig(void)
{
	i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
	i2c_gpio->i2c_sda_h();		//I2C_SDA_H();
	i2c_delay();
	i2c_gpio->i2c_sda_l();		//I2C_SDA_L();
	i2c_delay();
	i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
}

void i2c_stop_sig(void)
{
	i2c_gpio->i2c_sda_l();		//I2C_SDA_L();
	i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
	i2c_delay();
	i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
	i2c_delay();
	i2c_gpio->i2c_sda_h();		//I2C_SDA_H();
}

uint8 i2c_byte_w(uint8 data)
{
	uint8 ack;
	uint8 i;
	
	for(i = 0; i < 8; i++)
	{
		if(data & 0x80)
		{
			i2c_gpio->i2c_sda_h();	//I2C_SDA_H();
		}
		else
		{
			i2c_gpio->i2c_sda_l();	//I2C_SDA_L();
		}
		i2c_delay();
		i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
		i2c_delay();
		data <<= 1;
		i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
	}
	
	// Get ACK
//	i2c_gpio->i2c_sda_h();			//I2C_SDA_H();
    I2C_SDA_IN();
	i2c_delay();
	i2c_gpio->i2c_scl_h();			//I2C_SCL_H();
	i2c_delay();
	ack = i2c_gpio->i2c_sda_get();	//I2C_SDA_GET()
	i2c_gpio->i2c_scl_l();			//I2C_SCL_L();
	i2c_delay();
	
	return ack;
}

uint8 i2c_byte_r(uint8 ack)
{
	uint8 i;
	uint8 data = 0;
	
//	i2c_gpio->i2c_sda_h();		//I2C_SDA_H();
	
    I2C_SDA_IN();
	for(i = 0; i < 8; i++)
	{
		i2c_delay();
		i2c_gpio->i2c_scl_h();	//I2C_SCL_H();
		i2c_delay();
		data = (data << 1) | i2c_gpio->i2c_sda_get();
		i2c_gpio->i2c_scl_l();	//I2C_SCL_L();
	}
	
	// Set ACK !!
    I2C_SDA_OUT();
    i2c_delay();
	if(ack)
	{
		i2c_gpio->i2c_sda_h();	//I2C_SDA_H();
	}
	else
	{
		i2c_gpio->i2c_sda_l();	//I2C_SDA_L();
	}
	i2c_delay();
	i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
	i2c_delay();
	i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
	i2c_delay();
	
	return data;
}

void i2c_data_init(uint8 dev_addr, uint16 rate, const i2c_gpio_t *gpio)
{
	while(i2c_work_flag)
	{//	防止多任务调用
		os_time_dly(1);
	}
	
	i2c_work_flag	= TRUE;
	i2c_dev_addr 	= dev_addr;
	i2c_rate_delay 	= rate;
	i2c_gpio		= gpio;
	
	i2c_gpio->i2c_scl_init();	//I2C_SCL_INIT();
	i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
	i2c_gpio->i2c_sda_init();	//I2C_SDA_INIT();	
	i2c_gpio->i2c_sda_h();		//I2C_SDA_H();
}

void i2c_data_exit(void)
{
	i2c_work_flag = FALSE;
}

uint8 i2c_data_r(uint8 *value, uint16 length)
{
	uint16 i;
	
	i2c_start_sig();
	
	if(i2c_byte_w(i2c_dev_addr|0x01))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	for(i = 0; i < (length - 1); i++)
	{
		value[i] = i2c_byte_r(0);
	}
	
	value[i] = i2c_byte_r(1);
	
	i2c_stop_sig();
	
	return TRUE;
}

uint8 i2c_data_w(const uint8 *value, uint16 length)
{
	uint16 i;
	
	i2c_start_sig();
	
	if(i2c_byte_w(i2c_dev_addr))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	for(i = 0; i < length; i++)
	{
		if(i2c_byte_w(value[i]))
		{
			i2c_stop_sig();
			return FALSE;
		}
	}
	
	i2c_stop_sig();
	
	return TRUE;
}

#if defined(_SPECIAL_2313_CONTROL_)
void i2c_start_sig1(void)
{
	I2C_SDA_OUT1();
	i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
	i2c_gpio->i2c_sda_h();		//I2C_SDA_H();
	i2c_gpio->i2c_sda_l();		//I2C_SDA_L();
	i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
}

void i2c_stop_sig1(void)
{
	I2C_SDA_OUT1();
	i2c_gpio->i2c_sda_l();		//I2C_SDA_L();
	i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
	i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
	i2c_gpio->i2c_sda_h();		//I2C_SDA_H();
}

uint8 i2c_byte_w1(uint8 data)
{
	uint8 ack;
	uint8 i;
	
    I2C_SDA_OUT1();
	for(i = 0; i < 8; i++)
	{
		if(data & 0x80)
		{
			i2c_gpio->i2c_sda_h();	//I2C_SDA_H();
		}
		else
		{
			i2c_gpio->i2c_sda_l();	//I2C_SDA_L();
		}
		i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
		data <<= 1;
		i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
	}
	
	// Get ACK
//	i2c_gpio->i2c_sda_h();			//I2C_SDA_H();
    I2C_SDA_IN1();
	i2c_gpio->i2c_scl_h();			//I2C_SCL_H();
	ack = i2c_gpio->i2c_sda_get();	//I2C_SDA_GET()
	i2c_gpio->i2c_scl_l();			//I2C_SCL_L();
	
	return ack;
}

uint8 i2c_data_w1(const uint8 *value, uint16 length)
{
	uint16 i;
	
	i2c_start_sig1();
	
	if(i2c_byte_w1(i2c_dev_addr))
	{
		i2c_stop_sig1();
		return FALSE;
	}
	
	for(i = 0; i < length; i++)
	{
		if(i2c_byte_w1(value[i]))
		{
			i2c_stop_sig1();
			return FALSE;
		}
	}
	
	i2c_stop_sig1();
	
	return TRUE;
}
#endif

#if defined(_AUDIO_DRIVER_TDA7419_)
uint8 i2c_data_w3(const uint8 *value, uint16 length)
{
	uint16 i;
	
	i2c_start_sig();
	
	if(i2c_byte_w(i2c_dev_addr))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	if(i2c_byte_w(_B5_|0x00))
	{
		i2c_stop_sig();
		return FALSE;
	}

	for(i = 0; i < length; i++)
	{
		if(i2c_byte_w(value[i]))
		{
			i2c_stop_sig();
			return FALSE;
		}
	}
	
	i2c_stop_sig();
	
	return TRUE;
}
#endif

#if defined(_SUPPORT_EXT_MEMORY_)
uint8 i2c_byte_w2(uint8 data)
{
	uint8 ack;
	uint8 i;
	
	for(i = 0; i < 8; i++)
	{
		if(data & 0x80)
		{
			i2c_gpio->i2c_sda_h();	//I2C_SDA_H();
		}
		else
		{
			i2c_gpio->i2c_sda_l();	//I2C_SDA_L();
		}
		i2c_delay();
		i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
		i2c_delay();
		data <<= 1;
		i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
	}
	
	// Get ACK
	i2c_gpio->i2c_sda_h();			//I2C_SDA_H();
    //I2C_SDA_IN();

	i2c_delay();
	i2c_gpio->i2c_scl_h();			//I2C_SCL_H();
	i2c_delay();
	ack = i2c_gpio->i2c_sda_get();	//I2C_SDA_GET()
	i2c_gpio->i2c_scl_l();			//I2C_SCL_L();
	i2c_delay();
	
	return ack;
}

uint8 i2c_byte_r2(uint8 ack)
{
	uint8 i;
	uint8 data = 0;
	i2c_gpio->i2c_sda_h();		//I2C_SDA_H();
    //I2C_SDA_IN();	

	for(i = 0; i < 8; i++)
	{
		i2c_delay();
		i2c_gpio->i2c_scl_h();	//I2C_SCL_H();
		i2c_delay();
		data = (data << 1) | i2c_gpio->i2c_sda_get();
		i2c_gpio->i2c_scl_l();	//I2C_SCL_L();
	}
	
	// Set ACK !!
	if(ack)
	{
		i2c_gpio->i2c_sda_h();	//I2C_SDA_H();
	}
	else
	{
		i2c_gpio->i2c_sda_l();	//I2C_SDA_L();
	}
	i2c_delay();
	i2c_gpio->i2c_scl_h();		//I2C_SCL_H();
	i2c_delay();
	i2c_gpio->i2c_scl_l();		//I2C_SCL_L();
	i2c_delay();
	
	return data;
}
uint8 i2c_data_r2(uint8 *value, uint16 length)
{
	uint16 i;
	
	i2c_start_sig();
	
	if(i2c_byte_w2(i2c_dev_addr|0x01))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	for(i = 0; i < (length - 1); i++)
	{
		value[i] = i2c_byte_r2(0);
	}
	
	value[i] = i2c_byte_r2(1);
	
	i2c_stop_sig();
	
	return TRUE;
}

uint8 i2c_data_w2(const uint8 *value, uint16 length)
{
	uint16 i;
	
	i2c_start_sig();
	
	if(i2c_byte_w2(i2c_dev_addr))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	for(i = 0; i < length; i++)
	{
		if(i2c_byte_w2(value[i]))
		{
			i2c_stop_sig();
			return FALSE;
		}
	}
	
	i2c_stop_sig();
	
	return TRUE;
}
#endif

uint8 i2c_data_w_r(const uint8 *w_value, uint16 w_length, uint8 *r_value, uint16 r_length)
{
	uint16 i;
	
	i2c_start_sig();
	
	if(i2c_byte_w(i2c_dev_addr))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	for(i = 0; i < w_length; i++)
	{
		if(i2c_byte_w(w_value[i]))
		{
			i2c_stop_sig();
			return FALSE;
		}
	}
	
	if(r_length)
	{
		i2c_start_sig();
		
		if(i2c_byte_w(i2c_dev_addr|0x01))
		{
			i2c_stop_sig();
			return FALSE;
		}
		
		for(i = 0; i < (r_length - 1); i++)
		{
			r_value[i] = i2c_byte_r(0);
		}
		
		r_value[i] = i2c_byte_r(1);
	}
	
	i2c_stop_sig();

	return TRUE;
}

uint8 i2c_data_wr(uint16 *value, uint16 length)
{
	uint16 i;
	uint8 word_h, word_l;
	
	i2c_start_sig();
	
	if(i2c_byte_w(i2c_dev_addr|0x01))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	for(i = 0; i < (length - 1); i++)
	{
		word_h = i2c_byte_r(0);
		word_l = i2c_byte_r(0);
		value[i] = _u8tou16_(word_h, word_l);
	}
	
	word_h = i2c_byte_r(0);
	word_l = i2c_byte_r(1);
	value[i] = _u8tou16_(word_h, word_l);
	
	i2c_stop_sig();
	
	return TRUE;
}

uint8 i2c_data_ww(const uint16 *value, uint16 length)
{
	uint16 i;
	uint16 word;
	
	i2c_start_sig();
	
	if(i2c_byte_w(i2c_dev_addr))
	{
		i2c_stop_sig();
		return FALSE;
	}
	
	for(i = 0; i < length; i++)
	{
		word = value[i];
		
		if(i2c_byte_w(_u16to8h_(word)))
		{
			i2c_stop_sig();
			return FALSE;
		}
		
		if(i2c_byte_w(_u16to8l_(word)))
		{
			i2c_stop_sig();
			return FALSE;
		}
	}
	
	i2c_stop_sig();
	
	return TRUE;
}

#endif

