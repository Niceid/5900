#include "sdk_cfg.h"
#include "dec/music_api.h"
#include "id3/id3.h"
#include "id3/id3_conv.h"

/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------*/

static const uint16 TabAddr[] =
{ 0, 5, 16, 23, 28, 33, 37, 43, 50, 54, 59, 65, 71, 76, 79, 82, 85, 91, 95, 101, 111, 122, 125, 135, 141, 151, 162,
        169, 177, 182, 191, 197, 203, 212, 224, 228, 233, 237, 246, 252, 257, 267, 271, 275, 279, 284, 294, 309, 325,
        331, 337, 345, 362, 372, 380, 389, 394, 406, 412, 416, 423, 428, 440, 448, 454, 468, 475, 482, 493, 497, 506,
        513, 518, 524, 532, 540, 545, 550, 557, 566, 574, 578, 587, 599, 604, 614, 619, 624, 631, 637, 646, 656, 666,
        680, 695, 708, 716, 723, 729, 742, 750, 756, 762, 769, 774, 786, 792, 800, 809, 815, 825, 831, 838, 842, 847,
        852, 860, 866, 877, 889, 898, 902, 910, 918, 926, 936, 945, 948, 967, 975, 981, 986, 993, 1002, 1011, 1015,
        1034, 1044, 1054, 1063, 1084, 1097, 1105, 1110, 1120, 1125, 1129, 1137 };

static const uint8 genre_tab[] = \
"BluesClassicRockCountryDanceDiscoFunkGrungeHip-HopJazz\
MetalNewAgeOldiesOtherPopR&BRapReggaeRockTechno\
IndustrialAlternativeSkaDeathMetalPranksSoundtrackEuro-TechnoAmbientTrip-HopVocal\
Jazz+FunkFusionTranceClassicalInstrumentalAcidHouseGameSoundClipGospel\
NoiseAlternRockBassSoulPunkSpaceMeditativeInstrumentalPopInstrumentalRockEthnic\
GothicDarkwaveTechno-IndustrialElectronicPop-FolkEurodanceDreamSouthernRockComedyCult\
GangstaTop40ChristianRapPop/FunkJungleNativeAmericanCabaretNewWavePsychadelicRave\
ShowtunesTrailerLo-FiTribalAcidPunkAcidJazzPolkaRetroMusicalRock&Roll\
HardRockFolkFolk-RockNationalFolkSwingFastFusionBebobLatinRevivalCeltic\
BluegrassAvantgardeGothicRockProgessiveRockPsychedelicRockSymphonicRockSlowRockBigBandChorusEasyListening\
AcousticHumourSpeechChansonOperaChamberMusicSonataSymphonyBootyBassPrimus\
PornGrooveSatireSlowJamClubTangoSambaFolkloreBalladPowerBalladRhythmicSoul\
FreestyleDuetPunkRockDrumSoloAcapellaEuro-HouseDanceHallGoaDrum&BassClub-HouseHardcore\
TerrorIndieBritPopNegerpunkPolskPunkBeatChristianGangstaRapHeavyMetalBlackMetalCrossover\
ContemporaryChristianChristianRockMerengueSalsaTrashMetalAnimeJPopSynthpop";

// ID3V2.2��Ҫ���ҵ�FrameID Table
#define ID3_COUNT1 		7
static const char id3_tab_v2_2[] = "TT2TP1TALXXXTENTRKXXX";

// ID3V2.3��Ҫ���ҵ�FrameID Table
#define ID3_COUNT2 		7
static const char id3_tab_v2_3[] = "TIT2TPE1TALBTCONTENCTRCKAPIC";

typedef struct
{
    uint8 header[3];	/*����Ϊ"ID3"������Ϊ��ǩ������*/
    uint8 ver;			/*�汾��;ID3V2.3�ͼ�¼03,ID3V2.4�ͼ�¼04*/
    uint8 revision;		/*���汾��;�˰汾��¼Ϊ00*/
    uint8 flag;			/*��ű�־���ֽڣ�����汾ֻ��������λ*/
    uint8 size[4];		/*��ǩ��С��������ǩͷ��10���ֽں����еı�ǩ֡�Ĵ�С*/
}__attribute__((packed)) id3_10_header;		//ID3V2.*��ǩͷ(ǰ10byte)

typedef struct
{
    uint8 frame_id[3]; 	/*���ĸ��ַ���ʶһ��֡��˵��������*/
    uint8 size[3]; 		/*֡���ݵĴ�С��������֡ͷ������С��1*/
    uint8 encode_type; 	//frame  �������
}__attribute__((packed)) id3_2_2_header;

typedef struct
{
    uint8 frame_id[4]; 	/*���ĸ��ַ���ʶһ��֡��˵�������ݣ��Ժ��г��õı�ʶ���ձ�*/
    uint8 size[4]; 		/*֡���ݵĴ�С��������֡ͷ������С��1*/
    uint8 flags[2];		/*��ű�־��ֻ������6λ*/
    uint8 encode_type;	//frame  �������
}__attribute__((packed)) id3_2_3_header;

/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************/
/*
 * \par  Description: ���������ַ���
 *
 * \param[in]    id3_info_r--id3��Ϣ
 genre_index--������
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static void mp3_data_copy(id3_ctrl_t *id3_ctrl, uint8 result, uint32 str_len, uint8 encode_type)
{
	uint8 *save_addr = id3_ctrl->id3_info->buf[result];
	uint32 *save_len = &id3_ctrl->id3_info->len[result];
	uint8 *id3_type	= &id3_ctrl->id3_type[result];
	const uint8 *id3_data = &id3_ctrl->id3_data[id3_ctrl->id3_pos];
	uint32 index;

    if(encode_type == 0)
    {//ascii
    	*id3_type = eNAME_TYPE_ASCII;
		
        if(str_len > (*save_len - 1))
        {
            str_len = *save_len - 1;
        }
		
		for(index = 0; index < str_len; index++)
		{
			if(id3_data[index])
			{
				save_addr[index] = id3_data[index];
			}
		}
		
		save_addr[index] = 0;
		*save_len = index;
    }
    else if(encode_type == 1)
    {//unicode
    	*id3_type = eNAME_TYPE_UNICODE;
		
		if(str_len > (*save_len - 2))
        {
            str_len = *save_len - 2;
        }
		
		if((str_len > 2) && (id3_data[0] == UNICODE_FLAG_H) && (id3_data[1] == UNICODE_FLAG_L))
		{
			str_len -= 2;
			str_len /= 2;
			str_len *= 2;
			
			for(index = 0; index < str_len; index += 2)
			{
				if((id3_data[index + 2] == 0) && (id3_data[index + 3] == 0))break;
				else
				{
					save_addr[index + 0] = id3_data[index + 2];
					save_addr[index + 1] = id3_data[index + 3];
				}
			}
			
			save_addr[index + 0] = save_addr[index + 1] = 0;
			*save_len = index;
		}
		else
		{//�쳣��������
			save_addr[0] = save_addr[1] = 0;
			*save_len = 0;
		}
    }
    else if(encode_type == 3)
    {//utf8
    	*id3_type = eNAME_TYPE_UTF_8;

		if(str_len > (*save_len - 1))
        {
            str_len = *save_len - 1;
        }

		uint32 len = 0;
		uint8 heard;
		while(TRUE)
		{
			heard = id3_data[len];

			if(heard == '\0')
			{
				break;
			}
			else
			{
				uint8 i = 0;

				if(!(heard & _B7_))
				{
					i = 1;
				}
				else if(!(heard & _B5_))
				{
					i = 2;
				}
				else if(!(heard & _B4_))
				{
					i = 3;
				}
				else if(!(heard & _B3_))
				{
					i = 4;
				}
				else if(!(heard & _B2_))
				{
					i = 5;
				}
				else if(!(heard & _B1_))
				{
					i = 6;
				}
				else break;

				if((len + i) > str_len)break;

				while(i--)
				{
					save_addr[len++] = *id3_data++;
				}
			}
		}

		save_addr[len] = 0;
		*save_len = len;
    }
    else
    {//�������벻֧�֣����
		*id3_type = eNAME_TYPE_NULLL;

		save_addr[0] = 0;
		*save_len = 0;
    }
}

/******************************************************************************/
/*
 * \par  Description: ƥ���ַ���
 *
 * \param[in]    check_str
 tab_str
 size_str--tab_str�ַ����ĳ���
 tab_count--tab_str�ַ�����ĸ���
 * \param[out]

 * \return        �ɹ�>0,ʧ��<0

 * \note
 *******************************************************************************/
static int8 mp3_check_tag(	uint16 *id3_check,
								const uint8 *check_str,
								const char *tab_str,
								uint8 size_str,
								uint8 tab_count)
{
    uint8 i;

    for(i = 0; i < tab_count; i++)
    {
        if((*id3_check) & (1<<i))
        {
            if(memcmp(check_str, tab_str, size_str) == 0)
            {
                (*id3_check) &= (~(1 << i));
                break;
            }
        }
        tab_str = tab_str + size_str;
    }

    if(i == tab_count)
    {
        return -1;
    }

    return i;
}

/******************************************************************************/
/*
 * \par  Description:����mp3���͵�V1�汾id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static uint8 mp3_v1_parse(id3_ctrl_t *id3_ctrl)
{
	id3_ctrl->id3_pos += 384;
	const uint8 *id3_data = &id3_ctrl->id3_data[id3_ctrl->id3_pos];
	id3_ctrl->id3_pos += 128;

	id3_printf("mp3_v1_parse:%c%c%c\n",id3_data[0],id3_data[1],id3_data[2]);

    if((id3_data[0] == 'T') && (id3_data[1] == 'A') && (id3_data[2] == 'G'))
    {
		//	title
    	if(id3_ctrl->id3_check & (1<<ID3_TYPE_TITLE))
    	{
    		uint8 *save_addr	= id3_ctrl->id3_info->buf[ID3_TYPE_TITLE];
			uint32 save_len		= id3_ctrl->id3_info->len[ID3_TYPE_TITLE];
			uint8 *id3_type		= &id3_ctrl->id3_type[ID3_TYPE_TITLE];
			uint32 str_len 		= 30;

        	*id3_type = eNAME_TYPE_ASCII;

            if(str_len > (save_len - 1))
            {
                str_len = save_len - 1;
            }

			memcpy(save_addr, &id3_data[3], str_len);
			save_addr[str_len] = 0;	//���ﲹ�ַ�������

			id3_ctrl->id3_check &= (~(1<<ID3_TYPE_TITLE));
		}
		id3_puts("id3_v1----->111\n");

		//	artist
        if(id3_ctrl->id3_check & (1<<ID3_TYPE_ARTIST))
        {
            uint8 *save_addr	= id3_ctrl->id3_info->buf[ID3_TYPE_ARTIST];
			uint32 save_len		= id3_ctrl->id3_info->len[ID3_TYPE_ARTIST];
			uint8 *id3_type		= &id3_ctrl->id3_type[ID3_TYPE_ARTIST];
			uint32 str_len 		= 30;

        	*id3_type = eNAME_TYPE_ASCII;

            if(str_len > (save_len - 1))
            {
                str_len = save_len - 1;
            }

			memcpy(save_addr, &id3_data[33], str_len);
			save_addr[str_len] = 0;	//���ﲹ�ַ�������

			id3_ctrl->id3_check &= (~(1<<ID3_TYPE_ARTIST));
        }
		id3_puts("id3_v1----->222\n");

		//	album
        if(id3_ctrl->id3_check & (1<<ID3_TYPE_ALBUM))
        {
            uint8 *save_addr	= id3_ctrl->id3_info->buf[ID3_TYPE_ALBUM];
			uint32 save_len		= id3_ctrl->id3_info->len[ID3_TYPE_ALBUM];
			uint8 *id3_type		= &id3_ctrl->id3_type[ID3_TYPE_ALBUM];
			uint32 str_len 		= 30;

        	*id3_type = eNAME_TYPE_ASCII;

            if(str_len > (save_len - 1))
            {
                str_len = save_len - 1;
            }

			memcpy(save_addr, &id3_data[66], str_len);
			save_addr[str_len] = 0;	//���ﲹ�ַ�������

			id3_ctrl->id3_check &= (~(1<<ID3_TYPE_ALBUM));
        }
		id3_puts("id3_v1----->333\n");
		
        return TRUE;
    }

	return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ����V2.2�汾
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static uint8 mp3_v2_2_parse(id3_ctrl_t *id3_ctrl, uint32 id3_size)
{
    uint8 num = 0;

    while((id3_ctrl->id3_pos < id3_ctrl->id3_len) && (id3_ctrl->id3_pos < id3_size) && (num < ID3_COUNT1))
    {
		const id3_2_2_header *frame_h = (id3_2_2_header *)&id3_ctrl->id3_data[id3_ctrl->id3_pos];
		id3_ctrl->id3_pos += sizeof(id3_2_2_header);

        uint32 frame_len = frame_h->size[0] * 0x10000 + frame_h->size[1] * 0x100 + frame_h->size[2];
		id3_printf("frame_id[0]:%c, frame_id[1]:%c, frame_id[2]:%c, frame_len:%d\n", frame_h->frame_id[0], frame_h->frame_id[1], frame_h->frame_id[2], frame_len);

        if(frame_len == 0)
        {
        	id3_puts("---v2_2-->id3_err--->111\n");
        	if(frame_h->frame_id[0] || frame_h->frame_id[1] || frame_h->frame_id[2])
        	{
        		id3_puts("---v2_2-->id3_err--->222\n");
				//����Ϊ�գ�������Ƚ�����
            	//֮ǰ�����һ���ֽڣ����µ���ָ��
            	id3_ctrl->id3_pos--;
            	continue;
			}
			else
            {//padding data
            	id3_puts("---v2_2-->id3_err--->333\n");
                break;
            }
        }
        else
        {
            frame_len--;//��ȥ��������1���ֽڳ���
        }

        if(frame_h->frame_id[0] == 'T')
        {
            int8 result = mp3_check_tag(&id3_ctrl->id3_check, frame_h->frame_id, id3_tab_v2_2, 3, ID3_COUNT1);
			
			id3_printf("id3_result:%d, encode_type:%d\n", result, frame_h->encode_type);
			
			if(result != - 1)
            {
            	mp3_data_copy(id3_ctrl, result, frame_len, frame_h->encode_type);
				num++;
            }
        }
		id3_ctrl->id3_pos += frame_len;
    }
	
    if(num == 0)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:����V2.3��V2.4�汾
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static uint8 mp3_v2_x_parse(id3_ctrl_t *id3_ctrl, uint32 id3_size, uint8 version)
{
	uint8 num = 0;

    while((id3_ctrl->id3_pos < id3_ctrl->id3_len) && (id3_ctrl->id3_pos < id3_size) && (num < ID3_COUNT2))
    {
    	const id3_2_3_header *frame_h = (id3_2_3_header *)&id3_ctrl->id3_data[id3_ctrl->id3_pos];
		id3_ctrl->id3_pos += sizeof(id3_2_3_header);
		
		uint32 frame_len;
        if(4 == version) // v2.4.0
        {
            frame_len = (frame_h->size[0] & 0x7F) * 0x200000 + (frame_h->size[1] & 0x7F) * 0x4000 + (frame_h->size[2]
                    & 0x7F) * 0x80 + (frame_h->size[3] & 0x7F);
        }
        else
        {
            frame_len = frame_h->size[0] * 0x1000000 + frame_h->size[1] * 0x10000 + frame_h->size[2] * 0x100
                    + frame_h->size[3];
        }
		
		id3_printf("frame_id[0]:%c, frame_id[1]:%c, frame_id[2]:%c, frame_id[3]:%c, frame_len:%d\n",frame_h->frame_id[0],frame_h->frame_id[1],frame_h->frame_id[2],frame_h->frame_id[3],frame_len);
		
        if(frame_len == 0)
        {
        	id3_puts("---v2_x-->id3_err--->111\n");
            if(frame_h->frame_id[0] || frame_h->frame_id[1] || frame_h->frame_id[2] || frame_h->frame_id[3])
            {
            	id3_puts("---v2_x-->id3_err--->222\n");
            	//����Ϊ�գ�������Ƚ�����
            	//֮ǰ�����һ���ֽڣ����µ���ָ��
            	id3_ctrl->id3_pos--;
            	continue;
            }
			else
			{//padding data
            	id3_puts("---v2_x-->id3_err--->333\n");
                break;
			}
        }
        else
        {
            frame_len--;//��ȥ��������1���ֽڳ���
        }
		
        if(frame_h->frame_id[0] == 'T')
        {
        	int8 result = mp3_check_tag(&id3_ctrl->id3_check, frame_h->frame_id, id3_tab_v2_3, 4, ID3_COUNT2 - 1);
			
			id3_printf("---111---id3_result:%d, encode_type:%d\n", result, frame_h->encode_type);
			
			if(result != - 1)
            {
            	mp3_data_copy(id3_ctrl, result, frame_len, frame_h->encode_type);
				num++;
            }
        }
        else if(frame_h->frame_id[0] == 'A')
        {//	����
        	int8 result = mp3_check_tag(&id3_ctrl->id3_check, frame_h->frame_id, id3_tab_v2_3, 4, ID3_COUNT2);
			
			id3_printf("---222---id3_result:%d, encode_type:%d\n", result, frame_h->encode_type);
			
			if(result != - 1)
            {
            	num++;
            }
        }
        id3_ctrl->id3_pos += frame_len;
    }
	
    if(num == 0)
    {
        return FALSE;
    }
	
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:����mp3���͵�V2�汾id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/

static uint8 mp3_v2_parse(id3_ctrl_t *id3_ctrl)
{
    uint8 ret_val;

	id3_ctrl->id3_pos = 0;
    const id3_10_header *id3h = (id3_10_header *)id3_ctrl->id3_data;
	id3_ctrl->id3_pos += sizeof(id3_10_header);

	id3_printf("id3h_header:%c%c%c\n", id3h->header[0], id3h->header[1], id3h->header[2]);
    if((id3h->header[0] != 'I') || (id3h->header[1] != 'D') || (id3h->header[2] != '3'))
    {
        return FALSE;
    }
	
   	uint8 id3_version = id3h->ver;
    //ID3Size������ID3V2�ĳ���
    uint32 id3_size = sizeof(id3_10_header) + (id3h->size[0] & 0x7F) * 0x200000 + (id3h->size[1] & 0x7F) * 0x4000
            + (id3h->size[2] & 0x7F) * 0x80 + (id3h->size[3] & 0x7F);
	
	id3_printf("id3_version:%d, id3_size:%d\n", id3_version, id3_size);
	
    if(id3_version < 3)
    {
        ret_val = mp3_v2_2_parse(id3_ctrl, id3_size);
    }
    else
    {
        ret_val = mp3_v2_x_parse(id3_ctrl, id3_size, id3_version);
    }
	id3_printf("id3_ret_val:%d\n", ret_val);
	
    return ret_val;
}

uint8 get_id3_mp3(id3_ctrl_t *id3_ctrl)
{
	if(mp3_v2_parse(id3_ctrl))return TRUE;
	if(mp3_v1_parse(id3_ctrl))return TRUE;
    return FALSE;
}

