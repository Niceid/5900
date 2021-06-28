#ifndef __APT8L16_H__
#define __APT8L16_H__

#if defined(_SUPPORT_TOUCH_KEY_)

#define APT8L16_I2C_WRITE_ADDR						0xAC
#define APT8L16_I2C_READ_ADDR						0xAD


extern void touch_key_init(void);
extern uint8 Read_Touch_Value(uint8 Reg);
extern void Write_Touch_Value(uint8 Reg,uint8 Value);
#endif

#endif /* __APT8L16_H__ */

