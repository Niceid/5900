#include "sdk_cfg.h"
#include "id3/id3_conv.h"


#if defined(_SUPPORT_CHAR_RUSSIAN_)

#define UNICODE_RUSSIAN2_FLAG				(0x04)
#define UNICODE_RUSSIAN2(x)					((((uint16)UNICODE_RUSSIAN2_FLAG)<<8)|(x))

#define UNICODE_RUSSIAN2_START				UNICODE_RUSSIAN2(0x30)
#define UNICODE_RUSSIAN2_END				UNICODE_RUSSIAN2(0x4F)
#define UNICODE_RUSSIAN2_ESPECIALLY			UNICODE_RUSSIAN2(0x51)

#define UNICODE_CAPS_RUSSIAN2_START			UNICODE_RUSSIAN2(0x10)
#define UNICODE_CAPS_RUSSIAN2_END			UNICODE_RUSSIAN2(0x2F)
#define UNICODE_CAPS_RUSSIAN2_ESPECIALLY	UNICODE_RUSSIAN2(0x01)

#define CHAR_RUSSIAN_LENGTH2				(33)
#define CHAR_RUSSIAN_START2					(0xC0+0x00)//自编码映射西里尔字母到AscII区域
#define CHAR_RUSSIAN_ESPECIALLY2			(CHAR_RUSSIAN_START2+ CHAR_RUSSIAN_LENGTH2 - 1)
#define CHAR_RUSSIAN_END2					(CHAR_RUSSIAN_ESPECIALLY2)

static uint8 UnicodeRussianToAscii(uint16 c)
{
	uint8 Ascii;

	if(c >= UNICODE_RUSSIAN2_START && c <= UNICODE_RUSSIAN2_END)
	{
		return CHAR_RUSSIAN_START2 + (c - UNICODE_RUSSIAN2_START);
	}
	if(c >= UNICODE_CAPS_RUSSIAN2_START && c <= UNICODE_CAPS_RUSSIAN2_END)
	{
		return CHAR_RUSSIAN_START2 + (c - UNICODE_CAPS_RUSSIAN2_START);
	}
	if(c == UNICODE_RUSSIAN2_ESPECIALLY || c == UNICODE_CAPS_RUSSIAN2_ESPECIALLY)
	{
		return CHAR_RUSSIAN_ESPECIALLY2;
	}
	
	return 0;
}

#define ASCII_RUSSIAN2_START1				(0xC0)
#define ASCII_RUSSIAN2_END1					(ASCII_RUSSIAN2_START1+0x1F)
#define ASCII_RUSSIAN2_ESPECIALLY1			(0xA8)

#define ASCII_CAPS_RUSSIAN2_START1			(0xE0)
#define ASCII_CAPS_RUSSIAN2_END1			(ASCII_CAPS_RUSSIAN2_START1+0x1F)
#define ASCII_CAPS_RUSSIAN2_ESPECIALLY1	    (0xB8)

static uint8 AsciiRussianToAscii(uint8 c)
{
	if(c >= ASCII_RUSSIAN2_START1 && c <= ASCII_RUSSIAN2_END1)
	{
		return CHAR_RUSSIAN_START2 + (c-ASCII_RUSSIAN2_START1);
	}
	if(c >= ASCII_CAPS_RUSSIAN2_START1 && c <= ASCII_CAPS_RUSSIAN2_END1)
	{
		return CHAR_RUSSIAN_START2 + (c-ASCII_CAPS_RUSSIAN2_START1);
	}
	if(c== ASCII_RUSSIAN2_ESPECIALLY1 || c== ASCII_CAPS_RUSSIAN2_ESPECIALLY1)
	{
		return CHAR_RUSSIAN_ESPECIALLY2;
	}
	return 0;
}
#endif

static uint8 ascii_to_ascii(uint8 Ascii)
{
	uint8 Name;
	Name = 0;

#if defined(_SUPPORT_CHAR_RUSSIAN_)
	if(Name = AsciiRussianToAscii(Ascii))return Name;
#endif

	if((Ascii>('a'-1)) && (Ascii<('z'+1)))
	{
		Ascii = Ascii -'a'+'A';
	}

	return Ascii;
}

static uint8 unicode_to_ascii(uint16 Unicode)
{
	uint8 Name;
	Name = 0;

#if defined(_SUPPORT_CHAR_RUSSIAN_)
	if(Name = UnicodeRussianToAscii(Unicode))return Name;
#endif

	if(!_u16to8h_(Unicode))
	{
	#if defined(UNDISP_CHAR_USE_START_REPLACE)
		if((_u16to8l_(Unicode)<0x20)||(_u16to8l_(Unicode)>0x7F))
			Name='*';
	#else
		if((_u16to8l_(Unicode)<0x20)||(_u16to8l_(Unicode)>0x7F))
			Name='-';
	#endif
		else
			Name=_u16to8l_(Unicode);
	}
	else
	{
    #if defined(UNDISP_CHAR_USE_START_REPLACE)
    	if(_u16to8l_(Unicode))
    		Name='*';
    #else
    	if(_u16to8l_(Unicode))
    		Name='-';
    #endif
		else
		{
			if((_u16to8h_(Unicode)<0x20)||(_u16to8h_(Unicode)>0x7F))
	    #if defined(UNDISP_CHAR_USE_START_REPLACE)
	    	Name='*'; 
	    #else
	    	Name='-';
	    #endif
			else
				Name=_u16to8h_(Unicode);
		}
	}

	if((Name>('a'-1)) && (Name<('z'+1)))
	{
		Name = Name-'a'+'A';
	}
	return Name;
}

uint32 decode_fs_name(const void *sptr, uint32 src_len, void *dptr, uint8 type)
{
	uint32 i;
	uint32 len = 0;
	const uint8 *src_ptr = (uint8 *)sptr;
    uint8 *dec_ptr = (uint8 *)dptr;

	if(type == eNAME_TYPE_UNICODE)
	{
		uint16 unicode;
		src_len = src_len / 2;

		for(i = 0;i < src_len; i++)
		{
		
			unicode = _au16_(src_ptr,i);

			if((unicode == 0)||(unicode == 0xFEFF))break;
			dec_ptr[len++] = unicode_to_ascii(unicode);
		}
	}
	else if(type == eNAME_TYPE_ASCII)
	{
		uint8 ascii;

		for(i = 0; i < src_len; i++)
		{
			ascii = src_ptr[i];
		
			if(ascii == '\0')
			break;
		
			dec_ptr[len++] = ascii_to_ascii(ascii);
		}
	}
	else if(type == eNAME_TYPE_UTF_8)
	{
		uint32 offset = 0;
		uint16 unicode;
		uint8 heard;

		while(src_len > offset)
		{
			heard = src_ptr[offset];

			if(heard == '\0')
			{
				break;
			}
			else
			{
				unicode = 0;

				if(!(heard & _B7_))
				{
					unicode |= ((src_ptr[offset++])&(_B6_|_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				}
				else if(!(heard & _B5_))
				{
					unicode |= ((src_ptr[offset++])&(_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				}
				else if(!(heard & _B4_))
				{
					unicode |= ((src_ptr[offset++])&(_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				}
				else if(!(heard & _B3_))
				{
					unicode |= ((src_ptr[offset++])&(_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				}
				else if(!(heard & _B2_))
				{
					unicode |= ((src_ptr[offset++])&(_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				}
				else if(!(heard & _B1_))
				{
					unicode |= ((src_ptr[offset++])&(_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
					unicode <<= 6;
					unicode |= ((src_ptr[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				}
				else break;

				dec_ptr[len++] = unicode_to_ascii(unicode);
			}
		}
	}

	dec_ptr[len] = '\0';

	return len;
}

uint32 filtrate_char_sub(void *buf_pt, uint8 type)
{
    uint8 start_flag = 0;
	uint8 end_flag = 0;
	uint8 *buf_start = (uint8 *)buf_pt;
    uint8 *start = buf_start;
	uint8 *end = buf_start;

    if(type == eNAME_TYPE_ASCII)
    {
        uint8 *pt8 = (uint8*)buf_start;

        while(*pt8 != 0x00)
        {
            if(*pt8 == 0x20)
            {
            	if(start_flag)
            	{
	                if(end_flag == 0)
	                {
	                	end = pt8;
						end_flag = 1;
	                }
            	}
            }
            else
            {
            	if(start_flag == 0)
            	{
					start = pt8;
					start_flag = 1;
				}
                end_flag = 0;
                end = pt8 + 1;
            }
            pt8++;
        }
    }
	else if(type == eNAME_TYPE_UNICODE)
    {
   
       	uint16 *pt16 = (uint16*)buf_start;
 

        while(*pt16 != 0x0000)
        {
            if(*pt16 == 0x0020)
            {
            	if(start_flag)
            	{
	                if(end_flag == 0)
	                {
	                	end = (uint8*)pt16;
						end_flag = 1;
	                }
            	}
            }
            else
            {
            	if(start_flag == 0)
            	{
					start = (uint8 *)pt16;
					start_flag = 1;
				}
                end_flag = 0;
                end = (uint8 *)(pt16 + 1);
            }
            pt16++;
        }
    }
	else if(type == eNAME_TYPE_UTF_8)
    {
    	uint32 offset = 0;
		uint16 unicode;
		uint8 heard;
		uint8 *pt8 = (uint8 *)buf_start;
		uint8 *ptu = (uint8 *)buf_start;

		while((heard = pt8[offset]) != '\0')
		{
			unicode = 0;
			ptu = &pt8[offset];

			if(!(heard & _B7_))
			{
				unicode |= ((pt8[offset++])&(_B6_|_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
			}
			else if(!(heard & _B5_))
			{
				unicode |= ((pt8[offset++])&(_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
			}
			else if(!(heard & _B4_))
			{
				unicode |= ((pt8[offset++])&(_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
			}
			else if(!(heard & _B3_))
			{
				unicode |= ((pt8[offset++])&(_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
			}
			else if(!(heard & _B2_))
			{
				unicode |= ((pt8[offset++])&(_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
			}
			else if(!(heard & _B1_))
			{
				unicode |= ((pt8[offset++])&(_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
				unicode <<= 6;
				unicode |= ((pt8[offset++])&(_B5_|_B4_|_B3_|_B2_|_B1_|_B0_));
			}
			else break;

			if(unicode == 0x0020)
			{
				if(start_flag)
            	{
	                if(end_flag == 0)
	                {
	                	end = ptu;
						end_flag = 1;
	                }
            	}
			}
			else
			{
				if(start_flag == 0)
            	{
					start = ptu;
					start_flag = 1;
				}
                end_flag = 0;
                end = &pt8[offset];
			}
		}
    }

    if((start_flag | end_flag) == 0)
    {
        //全是空格
        buf_start[0] = 0x00;
        if(type == eNAME_TYPE_UNICODE)
        {
			buf_start[1] = 0x00;
		}
        return 0;
    }
    else if(end_flag == 1)
    {
        //后面有空格
    	end[0] = 0x00;
		if(type == eNAME_TYPE_UNICODE)
    	{
    		end[1] = 0x00;
		}
		return (uint32)(end - start);
    }
	else
	{
		uint32 cpy_count = (uint32)(end - start);

		if(start != buf_start)
		{
	        memcpy(buf_start, start, cpy_count);

			buf_start[cpy_count] = 0x00;
	        if(type == eNAME_TYPE_UNICODE)
	        {
				buf_start[cpy_count + 1] = 0x00;
			}
		}

		return cpy_count;
	}
}

