#ifndef	__IIC_H__
#define __IIC_H__


#if defined(_ENABLE_I2C_DRV_)

typedef struct{
//scl
	V_FUNC_V i2c_scl_init;
	V_FUNC_V i2c_scl_h;
	V_FUNC_V i2c_scl_l;
//sda
	V_FUNC_V i2c_sda_init;
	V_FUNC_V i2c_sda_h;
	V_FUNC_V i2c_sda_l;
	U_FUNC_V i2c_sda_get;
}i2c_gpio_t;


extern void i2c_start_sig(void);
extern void i2c_stop_sig(void);

extern uint8 i2c_byte_w(uint8);
extern uint8 i2c_byte_r(uint8);

extern void i2c_data_init(uint8, uint16, const i2c_gpio_t *);
extern void i2c_data_exit(void);

extern uint8 i2c_data_r(uint8 *, uint16);
extern uint8 i2c_data_w(const uint8 *, uint16);

#if defined(_SPECIAL_2313_CONTROL_)
extern uint8 i2c_data_w1(const uint8 *, uint16);
#endif

#if defined(_SUPPORT_EXT_MEMORY_)
extern uint8 i2c_data_r2(uint8 *, uint16);
extern uint8 i2c_data_w2(const uint8 *, uint16);
#endif

extern uint8 i2c_data_w3(const uint8 *, uint16);

extern uint8 i2c_data_wr(uint16 *, uint16);
extern uint8 i2c_data_ww(const uint16 *, uint16);

extern uint8 i2c_data_w_r(const uint8 *, uint16, uint8 *, uint16);

#endif


#endif /* __IIC_H__ */

