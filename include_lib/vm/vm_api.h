#ifndef __VM_API_H__
#define __VM_API_H__

#include "typedef.h"
#include "vm.h"
#include "common/flash_cfg.h"
#include "sdk_cfg.h"

//_VM_H_

//
enum
{
    //SYSCFG_BTSTACK = 0 ,
    //SYSCFG_FILE_BREAKPOINT,
    SYSCFG_REMOTE_DB = 0,
    SYSCFG_REMOTE_DB_END = (SYSCFG_REMOTE_DB + 20),
    SYSCFG_REMOTE_DB_HID,
    SYSCFG_REMOTE_DB_HID_END = (SYSCFG_REMOTE_DB_HID + 20),
	SYSCFG_REMOTE_DB_STEREO,
    SYSCFG_REMOTE_DB_STEREO_HID,

    VM_SYS_VOL,
    VM_SYS_EQ,
    VM_DEV0_BREAKPOINT,
    VM_DEV1_BREAKPOINT,
    VM_DEV2_BREAKPOINT,
    VM_DEV3_BREAKPOINT,
    VM_DEV0_FLACBREAKPOINT,
    VM_DEV1_FLACBREAKPOINT,
    VM_DEV2_FLACBREAKPOINT,
    VM_DEV3_FLACBREAKPOINT,
    VM_MUSIC_DEVICE,
    VM_PC_VOL,
    VM_FM_INFO,
	VM_PHONE_VOL,
	VM_BT_STEREO_INFO,
    VM_BT_OSC_INT_R,
	VM_BT_OSC_INT_L,
//------VM_PHONE_VOL此项前禁止修�?----//

	VM_SETUP,
	VM_FM_RDS1,
	VM_FM_RDS2,
	VM_FM_RDS3,
	VM_FM_RDS4,
#if defined(TD_ADD_NEW_APP)&&defined(_AUDIO_DRIVER_AP3768_)
	VM_AP3768_INFO,
#endif
#if defined(_ENABLE_MEMORY_PASSWORD_)
	VM_PWD_SET,
#endif
#if defined(_UPDATA_CLEAN_MEMORY_)
	VM_USB_UPDATA,
#endif
#if defined(_ENABLE_AUX_RESET_)
	VM_SOFT_RESET,
#endif
#if defined(_ENABLE_MEMORY_POWER_)
    VM_POWER_SET,
#endif
    VM_MAX_INDEX,
};

#define VM_START_INDEX     	 	VM_SYS_VOL
#define VM_MAX_SIZE				(VM_MAX_INDEX - VM_SYS_VOL)

#define VM_FILE_BK_LEN        	20
#define VM_FILE_FLACBK_LEN    	548


#define VM_SYS_VOL_LEN          1
#define VM_SYS_EQ_LEN           1

#define VM_DEV0_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV1_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV2_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV3_BK_LEN          VM_FILE_BK_LEN///(VM_FILE_BK_LEN+8)

#define VM_DEV0_FLACBK_LEN		VM_FILE_FLACBK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV1_FLACBK_LEN		VM_FILE_FLACBK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV2_FLACBK_LEN		VM_FILE_FLACBK_LEN///(VM_FILE_BK_LEN+8)
#define VM_DEV3_FLACBK_LEN		VM_FILE_FLACBK_LEN///(VM_FILE_BK_LEN+8)
#define VM_MUSIC_DEVICE_LEN     1

#define VM_PC_VOL_LEN           1
#define VM_PHONE_VOL_LEN		1
#define VM_STEREO_INFO_LEN		1
#define VM_OSC_INT_R_LEN        1
#define VM_OSC_INT_L_LEN        1


#if defined(_SPECIAL_NUMBER_SAVE_)
#define VM_FM_INFO_LEN          140
#elif defined(_ENABLE_RADIO_WB_)
#define VM_FM_INFO_LEN          130
#else
#define VM_FM_INFO_LEN          120
#endif

#define VM_FM_RDS_LEN			250
#define VM_FM_RDS_BLOCK			4

#if defined(TD_ADD_NEW_APP)&&defined(_AUDIO_DRIVER_AP3768_)
#define VM_AP3768_LEN			122
#endif

#if defined(_ENABLE_MEMORY_PASSWORD_)
#define VM_PWD_LEN				4
#endif

#if defined(_UPDATA_CLEAN_MEMORY_)
#define VM_UPDATA_LEN			1
#endif

#if defined(_ENABLE_AUX_RESET_)
#define VM_SOFT_RESET_LEN		1
#endif

#if defined(_ENABLE_MEMORY_POWER_)
#define VM_POWER_LEN		    1
#endif

#if defined(_ENABLE_BT_POWER_)||defined(_WLGK_SETUP_LENGH_)
#define VM_SETUP_LEN			70
#elif defined(_ENABLE_SETUP_LENGH_)
#define VM_SETUP_LEN			85
#else
#define VM_SETUP_LEN			60
#endif

typedef struct __VM_CACHE
{
    void *buff;
    u8 index;
    u8 dat_len;
    s16 cnt;
} _VM_CACHE;

void vm_cache_start(void);
void vm_open_all(void);
s32 vm_write_api(u8 index ,const void *data_buf);
s32 vm_read_api(u8 index, void * data_buf);
vm_err vm_cache_write(u8 index ,const void *data_buf,s16 cnt);
vm_err vm_cache_submit(void);
vm_err vm_init_api(struct flash_cfg *cfg);
#endif

