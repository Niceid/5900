#ifndef __ID3_CONV_H__
#define __ID3_CONV_H__


#define UNICODE_FLAG_H  	(0xFF)
#define UNICODE_FLAG_L  	(0xFE)
#define UTF8_FLAG_0   		(0xEF)
#define UTF8_FLAG_1   		(0xBB)
#define UTF8_FLAG_2   		(0xBF)


enum{
	eNAME_TYPE_NULLL = 0,
	eNAME_TYPE_ASCII,
	eNAME_TYPE_UNICODE,
	eNAME_TYPE_UTF_8,
};

extern uint32 decode_fs_name(const void *, uint32, void *, uint8);
extern uint32 filtrate_char_sub(void *, uint8);

#endif //__ID3_CONV_H__

