#include "sdk_cfg.h"
#include "app_cfg.h"
#include "error.h"
#include "stdarg.h"
#include "disp.h"
#include "msg.h"
#include "setup.h"
#include "audio_drv/audio_drv.h"
#include "disc/disc.h"
#include "disc/disc_order.h"
#include "disc/disc_state.h"
#include "disc_drv/disc_drv.h"

#if defined(_SUPPORT_DISC_)

#define _CRT_VALUE_					0x8432

#define _CRC_BUILD_0_(x)			(x<<8)
#define _CRC_BUILD_1_(x)			((_CRC_BUILD_0_(x) << 1) ^ ((_CRC_BUILD_0_(x) & 0x8000) ? _CRT_VALUE_ : 0))
#define _CRC_BUILD_2_(x)			((_CRC_BUILD_1_(x) << 1) ^ ((_CRC_BUILD_1_(x) & 0x8000) ? _CRT_VALUE_ : 0))
#define _CRC_BUILD_3_(x)			((_CRC_BUILD_2_(x) << 1) ^ ((_CRC_BUILD_2_(x) & 0x8000) ? _CRT_VALUE_ : 0))
#define _CRC_BUILD_4_(x)			((_CRC_BUILD_3_(x) << 1) ^ ((_CRC_BUILD_3_(x) & 0x8000) ? _CRT_VALUE_ : 0))
#define _CRC_BUILD_5_(x)			((_CRC_BUILD_4_(x) << 1) ^ ((_CRC_BUILD_4_(x) & 0x8000) ? _CRT_VALUE_ : 0))
#define _CRC_BUILD_6_(x)			((_CRC_BUILD_5_(x) << 1) ^ ((_CRC_BUILD_5_(x) & 0x8000) ? _CRT_VALUE_ : 0))
#define _CRC_BUILD_7_(x)			((_CRC_BUILD_6_(x) << 1) ^ ((_CRC_BUILD_6_(x) & 0x8000) ? _CRT_VALUE_ : 0))
#define _CRC_BUILD_8_(x)			((_CRC_BUILD_7_(x) << 1) ^ ((_CRC_BUILD_7_(x) & 0x8000) ? _CRT_VALUE_ : 0))

#define _CRC_BUILD_(x)				_CRC_BUILD_8_(x)


static const uint16 CRC_TAB[]=
{
	(uint16)(_CRC_BUILD_(0)),
	(uint16)(_CRC_BUILD_(1)),
	(uint16)(_CRC_BUILD_(2)),
	(uint16)(_CRC_BUILD_(3)),
	(uint16)(_CRC_BUILD_(4)),
	(uint16)(_CRC_BUILD_(5)),
	(uint16)(_CRC_BUILD_(6)),
	(uint16)(_CRC_BUILD_(7)),
	(uint16)(_CRC_BUILD_(8)),
	(uint16)(_CRC_BUILD_(9)),

	(uint16)(_CRC_BUILD_(10)),
	(uint16)(_CRC_BUILD_(11)),
	(uint16)(_CRC_BUILD_(12)),
	(uint16)(_CRC_BUILD_(13)),
	(uint16)(_CRC_BUILD_(14)),
	(uint16)(_CRC_BUILD_(15)),
	(uint16)(_CRC_BUILD_(16)),
	(uint16)(_CRC_BUILD_(17)),
	(uint16)(_CRC_BUILD_(18)),
	(uint16)(_CRC_BUILD_(19)),

	(uint16)(_CRC_BUILD_(20)),
	(uint16)(_CRC_BUILD_(21)),
	(uint16)(_CRC_BUILD_(22)),
	(uint16)(_CRC_BUILD_(23)),
	(uint16)(_CRC_BUILD_(24)),
	(uint16)(_CRC_BUILD_(25)),
	(uint16)(_CRC_BUILD_(26)),
	(uint16)(_CRC_BUILD_(27)),
	(uint16)(_CRC_BUILD_(28)),
	(uint16)(_CRC_BUILD_(29)),

	(uint16)(_CRC_BUILD_(30)),
	(uint16)(_CRC_BUILD_(31)),
	(uint16)(_CRC_BUILD_(32)),
	(uint16)(_CRC_BUILD_(33)),
	(uint16)(_CRC_BUILD_(34)),
	(uint16)(_CRC_BUILD_(35)),
	(uint16)(_CRC_BUILD_(36)),
	(uint16)(_CRC_BUILD_(37)),
	(uint16)(_CRC_BUILD_(38)),
	(uint16)(_CRC_BUILD_(39)),

	(uint16)(_CRC_BUILD_(40)),
	(uint16)(_CRC_BUILD_(41)),
	(uint16)(_CRC_BUILD_(42)),
	(uint16)(_CRC_BUILD_(43)),
	(uint16)(_CRC_BUILD_(44)),
	(uint16)(_CRC_BUILD_(45)),
	(uint16)(_CRC_BUILD_(46)),
	(uint16)(_CRC_BUILD_(47)),
	(uint16)(_CRC_BUILD_(48)),
	(uint16)(_CRC_BUILD_(49)),

	(uint16)(_CRC_BUILD_(50)),
	(uint16)(_CRC_BUILD_(51)),
	(uint16)(_CRC_BUILD_(52)),
	(uint16)(_CRC_BUILD_(53)),
	(uint16)(_CRC_BUILD_(54)),
	(uint16)(_CRC_BUILD_(55)),
	(uint16)(_CRC_BUILD_(56)),
	(uint16)(_CRC_BUILD_(57)),
	(uint16)(_CRC_BUILD_(58)),
	(uint16)(_CRC_BUILD_(59)),

	(uint16)(_CRC_BUILD_(60)),
	(uint16)(_CRC_BUILD_(61)),
	(uint16)(_CRC_BUILD_(62)),
	(uint16)(_CRC_BUILD_(63)),
	(uint16)(_CRC_BUILD_(64)),
	(uint16)(_CRC_BUILD_(65)),
	(uint16)(_CRC_BUILD_(66)),
	(uint16)(_CRC_BUILD_(67)),
	(uint16)(_CRC_BUILD_(68)),
	(uint16)(_CRC_BUILD_(69)),

	(uint16)(_CRC_BUILD_(70)),
	(uint16)(_CRC_BUILD_(71)),
	(uint16)(_CRC_BUILD_(72)),
	(uint16)(_CRC_BUILD_(73)),
	(uint16)(_CRC_BUILD_(74)),
	(uint16)(_CRC_BUILD_(75)),
	(uint16)(_CRC_BUILD_(76)),
	(uint16)(_CRC_BUILD_(77)),
	(uint16)(_CRC_BUILD_(78)),
	(uint16)(_CRC_BUILD_(79)),

	(uint16)(_CRC_BUILD_(80)),
	(uint16)(_CRC_BUILD_(81)),
	(uint16)(_CRC_BUILD_(82)),
	(uint16)(_CRC_BUILD_(83)),
	(uint16)(_CRC_BUILD_(84)),
	(uint16)(_CRC_BUILD_(85)),
	(uint16)(_CRC_BUILD_(86)),
	(uint16)(_CRC_BUILD_(87)),
	(uint16)(_CRC_BUILD_(88)),
	(uint16)(_CRC_BUILD_(89)),

	(uint16)(_CRC_BUILD_(90)),
	(uint16)(_CRC_BUILD_(91)),
	(uint16)(_CRC_BUILD_(92)),
	(uint16)(_CRC_BUILD_(93)),
	(uint16)(_CRC_BUILD_(94)),
	(uint16)(_CRC_BUILD_(95)),
	(uint16)(_CRC_BUILD_(96)),
	(uint16)(_CRC_BUILD_(97)),
	(uint16)(_CRC_BUILD_(98)),
	(uint16)(_CRC_BUILD_(99)),

	(uint16)(_CRC_BUILD_(100)),
	(uint16)(_CRC_BUILD_(101)),
	(uint16)(_CRC_BUILD_(102)),
	(uint16)(_CRC_BUILD_(103)),
	(uint16)(_CRC_BUILD_(104)),
	(uint16)(_CRC_BUILD_(105)),
	(uint16)(_CRC_BUILD_(106)),
	(uint16)(_CRC_BUILD_(107)),
	(uint16)(_CRC_BUILD_(108)),
	(uint16)(_CRC_BUILD_(109)),

	(uint16)(_CRC_BUILD_(110)),
	(uint16)(_CRC_BUILD_(111)),
	(uint16)(_CRC_BUILD_(112)),
	(uint16)(_CRC_BUILD_(113)),
	(uint16)(_CRC_BUILD_(114)),
	(uint16)(_CRC_BUILD_(115)),
	(uint16)(_CRC_BUILD_(116)),
	(uint16)(_CRC_BUILD_(117)),
	(uint16)(_CRC_BUILD_(118)),
	(uint16)(_CRC_BUILD_(119)),

	(uint16)(_CRC_BUILD_(120)),
	(uint16)(_CRC_BUILD_(121)),
	(uint16)(_CRC_BUILD_(122)),
	(uint16)(_CRC_BUILD_(123)),
	(uint16)(_CRC_BUILD_(124)),
	(uint16)(_CRC_BUILD_(125)),
	(uint16)(_CRC_BUILD_(126)),
	(uint16)(_CRC_BUILD_(127)),
	(uint16)(_CRC_BUILD_(128)),
	(uint16)(_CRC_BUILD_(129)),

	(uint16)(_CRC_BUILD_(130)),
	(uint16)(_CRC_BUILD_(131)),
	(uint16)(_CRC_BUILD_(132)),
	(uint16)(_CRC_BUILD_(133)),
	(uint16)(_CRC_BUILD_(134)),
	(uint16)(_CRC_BUILD_(135)),
	(uint16)(_CRC_BUILD_(136)),
	(uint16)(_CRC_BUILD_(137)),
	(uint16)(_CRC_BUILD_(138)),
	(uint16)(_CRC_BUILD_(139)),

	(uint16)(_CRC_BUILD_(140)),
	(uint16)(_CRC_BUILD_(141)),
	(uint16)(_CRC_BUILD_(142)),
	(uint16)(_CRC_BUILD_(143)),
	(uint16)(_CRC_BUILD_(144)),
	(uint16)(_CRC_BUILD_(145)),
	(uint16)(_CRC_BUILD_(146)),
	(uint16)(_CRC_BUILD_(147)),
	(uint16)(_CRC_BUILD_(148)),
	(uint16)(_CRC_BUILD_(149)),

	(uint16)(_CRC_BUILD_(150)),
	(uint16)(_CRC_BUILD_(151)),
	(uint16)(_CRC_BUILD_(152)),
	(uint16)(_CRC_BUILD_(153)),
	(uint16)(_CRC_BUILD_(154)),
	(uint16)(_CRC_BUILD_(155)),
	(uint16)(_CRC_BUILD_(156)),
	(uint16)(_CRC_BUILD_(157)),
	(uint16)(_CRC_BUILD_(158)),
	(uint16)(_CRC_BUILD_(159)),

	(uint16)(_CRC_BUILD_(160)),
	(uint16)(_CRC_BUILD_(161)),
	(uint16)(_CRC_BUILD_(162)),
	(uint16)(_CRC_BUILD_(163)),
	(uint16)(_CRC_BUILD_(164)),
	(uint16)(_CRC_BUILD_(165)),
	(uint16)(_CRC_BUILD_(166)),
	(uint16)(_CRC_BUILD_(167)),
	(uint16)(_CRC_BUILD_(168)),
	(uint16)(_CRC_BUILD_(169)),

	(uint16)(_CRC_BUILD_(170)),
	(uint16)(_CRC_BUILD_(171)),
	(uint16)(_CRC_BUILD_(172)),
	(uint16)(_CRC_BUILD_(173)),
	(uint16)(_CRC_BUILD_(174)),
	(uint16)(_CRC_BUILD_(175)),
	(uint16)(_CRC_BUILD_(176)),
	(uint16)(_CRC_BUILD_(177)),
	(uint16)(_CRC_BUILD_(178)),
	(uint16)(_CRC_BUILD_(179)),

	(uint16)(_CRC_BUILD_(180)),
	(uint16)(_CRC_BUILD_(181)),
	(uint16)(_CRC_BUILD_(182)),
	(uint16)(_CRC_BUILD_(183)),
	(uint16)(_CRC_BUILD_(184)),
	(uint16)(_CRC_BUILD_(185)),
	(uint16)(_CRC_BUILD_(186)),
	(uint16)(_CRC_BUILD_(187)),
	(uint16)(_CRC_BUILD_(188)),
	(uint16)(_CRC_BUILD_(189)),

	(uint16)(_CRC_BUILD_(190)),
	(uint16)(_CRC_BUILD_(191)),
	(uint16)(_CRC_BUILD_(192)),
	(uint16)(_CRC_BUILD_(193)),
	(uint16)(_CRC_BUILD_(194)),
	(uint16)(_CRC_BUILD_(195)),
	(uint16)(_CRC_BUILD_(196)),
	(uint16)(_CRC_BUILD_(197)),
	(uint16)(_CRC_BUILD_(198)),
	(uint16)(_CRC_BUILD_(199)),

	(uint16)(_CRC_BUILD_(200)),
	(uint16)(_CRC_BUILD_(201)),
	(uint16)(_CRC_BUILD_(202)),
	(uint16)(_CRC_BUILD_(203)),
	(uint16)(_CRC_BUILD_(204)),
	(uint16)(_CRC_BUILD_(205)),
	(uint16)(_CRC_BUILD_(206)),
	(uint16)(_CRC_BUILD_(207)),
	(uint16)(_CRC_BUILD_(208)),
	(uint16)(_CRC_BUILD_(209)),

	(uint16)(_CRC_BUILD_(210)),
	(uint16)(_CRC_BUILD_(211)),
	(uint16)(_CRC_BUILD_(212)),
	(uint16)(_CRC_BUILD_(213)),
	(uint16)(_CRC_BUILD_(214)),
	(uint16)(_CRC_BUILD_(215)),
	(uint16)(_CRC_BUILD_(216)),
	(uint16)(_CRC_BUILD_(217)),
	(uint16)(_CRC_BUILD_(218)),
	(uint16)(_CRC_BUILD_(219)),

	(uint16)(_CRC_BUILD_(220)),
	(uint16)(_CRC_BUILD_(221)),
	(uint16)(_CRC_BUILD_(222)),
	(uint16)(_CRC_BUILD_(223)),
	(uint16)(_CRC_BUILD_(224)),
	(uint16)(_CRC_BUILD_(225)),
	(uint16)(_CRC_BUILD_(226)),
	(uint16)(_CRC_BUILD_(227)),
	(uint16)(_CRC_BUILD_(228)),
	(uint16)(_CRC_BUILD_(229)),

	(uint16)(_CRC_BUILD_(230)),
	(uint16)(_CRC_BUILD_(231)),
	(uint16)(_CRC_BUILD_(232)),
	(uint16)(_CRC_BUILD_(233)),
	(uint16)(_CRC_BUILD_(234)),
	(uint16)(_CRC_BUILD_(235)),
	(uint16)(_CRC_BUILD_(236)),
	(uint16)(_CRC_BUILD_(237)),
	(uint16)(_CRC_BUILD_(238)),
	(uint16)(_CRC_BUILD_(239)),

	(uint16)(_CRC_BUILD_(240)),
	(uint16)(_CRC_BUILD_(241)),
	(uint16)(_CRC_BUILD_(242)),
	(uint16)(_CRC_BUILD_(243)),
	(uint16)(_CRC_BUILD_(244)),
	(uint16)(_CRC_BUILD_(245)),
	(uint16)(_CRC_BUILD_(246)),
	(uint16)(_CRC_BUILD_(247)),
	(uint16)(_CRC_BUILD_(248)),
	(uint16)(_CRC_BUILD_(249)),

	(uint16)(_CRC_BUILD_(250)),
	(uint16)(_CRC_BUILD_(251)),
	(uint16)(_CRC_BUILD_(252)),
	(uint16)(_CRC_BUILD_(253)),
	(uint16)(_CRC_BUILD_(254)),
	(uint16)(_CRC_BUILD_(255)),
};

static uint16 GetCrcCode(const uint8 *data, uint8 length)
{
	uint16 crc = 0;
	uint8  ptr = 0;

	while(length --)
	{
		ptr = _u16to8h_(crc);
		crc <<= 8;
		crc ^= CRC_TAB[ptr ^ (*data)];
		data ++;
	}
	return crc;
}

static uint8 DecodeCrc(const uint8 *data, uint8 length)
{
	uint16 crc;

	if(length < 2)
	{
		return 0;
	}
	length -= 2;
	crc = GetCrcCode(data, length);
	if(crc != _u8tou16_(data[length], data[length+1]))length = 0;
	return length;
}

static uint8 EncodeCrc(uint8 *data, uint8 length)
{
	uint16 crc = GetCrcCode(data, length);

	data[length] = _u16to8h_(crc);
	data[length+1] = _u16to8l_(crc);
	length += 2;
	return length;
}

static uint8 DecodeXorSum(uint8 *data, uint8 length)
{
	uint8 XorSum;
	uint8 i;

	if(length)
	{
		length --;
		XorSum = data[length];
		for(i = 0; i < length; i++)
		{
			data[i] ^= XorSum;
		}
	}

	return length;
}

static uint8 EncodeXorSum(uint8 *data, uint8 length)
{
	uint8 XorSum = 0;
	uint8 i;

	for(i = 0; i < length; i++)
	{
		XorSum += data[i];
	}
	for(i = 0; i < length; i++)
	{
		data[i] ^= XorSum;
	}

	data[i] = XorSum;
	length++;
	return length;
}

uint8 DecodeData(uint8 *data, uint8 length)
{
	length = DecodeXorSum(data, length);
	if(!length)return 0;
	length = DecodeCrc(data, length);
	if(!length)return 0;
	return length;
}

uint8 EncodeData(uint8 *data, uint8 length)
{
	length = EncodeCrc(data, length);
	if(!length)return 0;
	length = EncodeXorSum(data, length);
	if(!length)return 0;
	return length;
}

uint8 disc_order_send_data(SERVO_ORDER_e order, const uint8 *data, uint8 length)
{
	if(!player_get_normal())return FALSE;
	
	uint8 buffer[2 + 3 + length];
	uint8 size = 0;
	uint8 result = FALSE;
	uint8 i;

	if(order == SERVO_ORDER_DISC)
	{
		puts("---save_disc_first---\n");
	}

	if(order == SERVO_ORDER_DISC_RESUME)
	{
		puts("---save_disc_resume---\n");
	}

	
	if((order > SERVO_ORDER_NULL) && (order < SERVO_ORDER_END))
	{
		buffer[size++] = SERVO_TYPE_ORDER;
		buffer[size++] = order;
		for(i = 0; i < length; i++)
		{
			buffer[size++] = data[i];
		}

		size = EncodeData(buffer, size);
		if(size)
		{
			result = disc_servo_send(buffer, size);
		}
	}

	//disc_printf("%s, order:%d, result:%d\n", __FUNCTION__, order, result);
	return result;
}

uint8 disc_order_send(SERVO_ORDER_e order)
{
	return disc_order_send_data(order, NULL, 0);
}

void disc_audio_init(void)
{
    audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DISC_ON, TRUE);
	sys_mute_ctrl(ENABLE, 500);	//DISABLE
#if defined(_WLS168_CHANNEL_PLAY_GATE_)||defined(_ENABLE_CHANNEL_GAIN_)
	audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_7_5, TRUE);
#else
    audio_drv_ctrl(AUD_CTRL_GATE, AUDIO_GATE_PLUS_11_25, TRUE);
#endif
}

void disc_audio_exit(void)
{
	sys_mute_ctrl(ENABLE, 0);
#if defined(_ENABLE_BT_FIRST_)
#if defined(_AUDIO_DRIVER_AP3768_)
	if(global.rds_ta)
	{
		audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_RDS_OFF, TRUE);
		OSTimeDly(10);
	}
#endif
#endif
	audio_drv_ctrl(AUD_CTRL_CHANNEL, AUDIO_CHANNEL_DISC_OFF, TRUE);
}

#endif

