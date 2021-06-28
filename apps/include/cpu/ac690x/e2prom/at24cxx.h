#ifndef __AT24CXX_H__
#define __AT24CXX_H__

#if defined(_E2PROM_DRIVER_AT24CXX_)

#define AT24CXX_I2C_ADDR							0xA0

#if defined(_EVB_I2C_RATE_)
#define AT24CXX_I2C_RATE							55//65ÊÇ130K
#elif defined(_YH1701_I2C_RATE_)
#define AT24CXX_I2C_RATE							65//65ÊÇ130K
#else
#define AT24CXX_I2C_RATE							20
#endif

#define EEPROM_WRITE_ERRO_MAX				    	(50)
#define EEPROM_SPACE_MAX							(1024)


extern uint8 e2prom_read_data(uint8 *,uint16,uint16);
extern uint8 e2prom_write_data(const uint8 *,uint16,uint16);


#endif

#endif /* __AT24CXX_H__ */

