#include "sdk_cfg.h"
#include "iic.h"
#include "includes.h"
#include "e2prom/at24cxx.h"


#if defined(_E2PROM_DRIVER_AT24CXX_)

static void at24cxx_scl_init(void)		{	E2PROM_I2C_SCL_INIT();				};
static void at24cxx_scl_h(void)			{	E2PROM_I2C_SCL_H();					};
static void at24cxx_scl_l(void)			{	E2PROM_I2C_SCL_L();					};
static void at24cxx_sda_init(void)		{	E2PROM_I2C_SDA_INIT();				};
static void at24cxx_sda_h(void)			{	E2PROM_I2C_SDA_H();					};
static void at24cxx_sda_l(void)			{	E2PROM_I2C_SDA_L();					};
static uint8 at24cxx_sda_get(void)		{	return (E2PROM_I2C_SDA_GET()?1:0);	};

static const i2c_gpio_t at24cxx_gpio =
{
	at24cxx_scl_init,
	at24cxx_scl_h,
	at24cxx_scl_l,
	at24cxx_sda_init,
	at24cxx_sda_h,
	at24cxx_sda_l,
	at24cxx_sda_get,
};

static uint8 write_to_at24cxx(uint16 addr, uint8 data, uint8 flag)
{
	uint8 count;
	uint8 result = FALSE;
	uint8 start_addr = AT24CXX_I2C_ADDR | (_u16to8h_(addr)<<1);
	uint8 transfer[2];

	transfer[0] = _u16to8l_(addr);
	transfer[1] = data;

	for(count = 0; (result == FALSE) && (count < EEPROM_WRITE_ERRO_MAX); count++)
	{
		i2c_data_init(start_addr, AT24CXX_I2C_RATE, &at24cxx_gpio);
		result = i2c_data_w2(transfer, flag ? 2 : 1);
		i2c_data_exit();
	}

	return result;
}

static uint8 read_for_at24cxx(uint16 addr, uint8 *buffer, uint16 length)
{
	uint8 result = FALSE;
	uint8 start_addr = AT24CXX_I2C_ADDR | (_u16to8h_(addr)<<1);
	
	if(write_to_at24cxx(addr, 0, FALSE))
	{
		i2c_data_init(start_addr, AT24CXX_I2C_RATE, &at24cxx_gpio);
		result = i2c_data_r2(buffer, length);
		i2c_data_exit();
	}
	
	return result;
}

uint8 e2prom_write_data(const uint8 *value, uint16 addr,uint16 length)
{
	uint16 index;

	if((addr + length) > EEPROM_SPACE_MAX)return FALSE;

	for(index = 0; index < length; index++)
	{
		if(!write_to_at24cxx(addr++, value[index], TRUE))return FALSE;
	}

	return TRUE;
}

uint8 e2prom_read_data(uint8 *value, uint16 addr, uint16 length)
{
	if((addr + length) > EEPROM_SPACE_MAX)return FALSE;
	
	return read_for_at24cxx(addr, value, length);
}

#endif

