#ifndef __MC30P6060_H__
#define __MC30P6060_H__

#if defined(_SUPPORT_30P60_LIGHT_)

#define SCL_CMD_POWER_OFF					0x00
#define SCL_CMD_SET_AUTO					0x01
#define SCL_CMD_SET_GREEN					0x02
#define SCL_CMD_SET_BLUE					0x03
#define SCL_CMD_SET_RED						0x04
#define SCL_CMD_SET_CYAN					0x05
#define SCL_CMD_SET_YELLOW				    0x06
#define SCL_CMD_SET_FUCHSINE				0x07
#define SCL_CMD_SET_WHITE					0x08
#define SCL_CMD_SET_ADJ 					0x09

extern void LightDriverIrCmd(uint8 cmd);

#endif

#endif /* __MC30P6060_H__ */

