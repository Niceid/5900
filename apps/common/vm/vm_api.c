#include "vm/vm_api.h"
#include "dac/dac_api.h"
#include "common/flash_cfg.h"
#include "common/app_cfg.h"
#include "peripherals/winbond_flash.h"



#define VM_DEBUG
#ifdef VM_DEBUG
#define vm_printf 		ew_printf
#define vm_puts 		ew_puts
#else
#define vm_printf(...)
#define vm_puts(...)
#endif


vm_hdl *vm_ptr = NULL;
extern void free_fun(void **ptr);
extern void *malloc_fun(void *ptr,u32 len,char *info);
/*----------------------------------------------------------------------------*/
/** @brief: 开辟的各个存储单元的长度
    @param: MAX_VM：开辟的存储单元数目
    @return:void
    @author:lj
    @note:  const u8 vm_index_len[MAX_VM]
*/
/*----------------------------------------------------------------------------*/
const u16 vm_index_len[VM_MAX_SIZE] =
{
    VM_SYS_VOL_LEN,/* VM_SYS_VOL = 0,  */
    VM_SYS_EQ_LEN,/*VM_SYS_EQ         */
    VM_DEV0_BK_LEN,/* VM_DEV_BREAKPOINT0, */
    VM_DEV1_BK_LEN,/* VM_DEV_BREAKPOINT1, */
    VM_DEV2_BK_LEN,/* VM_DEV_BREAKPOINT2, */
    VM_DEV3_BK_LEN,/* VM_DEV_BREAKPOINT3, */
    VM_DEV0_FLACBK_LEN,/* VM_DEV_BREAKPOINT0, */
    VM_DEV1_FLACBK_LEN,/* VM_DEV_BREAKPOINT1, */
    VM_DEV2_FLACBK_LEN,/* VM_DEV_BREAKPOINT2, */
    VM_DEV3_FLACBK_LEN,/* VM_DEV_BREAKPOINT3, */
    VM_MUSIC_DEVICE_LEN,
    VM_PC_VOL_LEN, /*PC VOL*/
    VM_FM_INFO_LEN,
	VM_PHONE_VOL_LEN,
	VM_STEREO_INFO_LEN,
	VM_OSC_INT_R_LEN,
	VM_OSC_INT_L_LEN,

	VM_SETUP_LEN,
	VM_FM_RDS_LEN,
	VM_FM_RDS_LEN,
	VM_FM_RDS_LEN,
	VM_FM_RDS_LEN,
#if defined(TD_ADD_NEW_APP)&&defined(_AUDIO_DRIVER_AP3768_)
    VM_AP3768_LEN,
#endif
#if defined(_ENABLE_MEMORY_PASSWORD_)
	VM_PWD_LEN,
#endif
#if defined(_UPDATA_CLEAN_MEMORY_)
	VM_UPDATA_LEN,
#endif
#if defined(_ENABLE_AUX_RESET_)
	VM_SOFT_RESET_LEN,
#endif
#if defined(_ENABLE_MEMORY_POWER_)
	VM_POWER_LEN,
#endif

    /* MAX_VM,          */
};

/*----------------------------------------------------------------------------*/
/** @brief: 初始化VM，并打开所有存储单元
    @param: void
    @return:void
    @author:lj
    @note:  void vm_open_all(void
*/
/*----------------------------------------------------------------------------*/
void vm_open_all(void)
{
    u32 i;
    vm_ptr = (vm_hdl *)malloc_fun(vm_ptr,sizeof(vm_hdl)*VM_MAX_SIZE,0);

    for(i = 0; i < VM_MAX_SIZE; i++)
    {
        if(vm_index_len[i])
        {
            vm_ptr[i] = vm_open(i + VM_START_INDEX,vm_index_len[i]);
        }
    }
}

/*----------------------------------------------------------------------------*/
/** @brief: 按照索引号写存储信息
    @param: index：存储单元索引
    @param: data_buf：需要存储的数据的指针
    @return:正确返回长度
    @author:lj
    @note:  vm_err vm_write_api(u8 index ,const void *data_buf)
*/
/*----------------------------------------------------------------------------*/
s32 vm_write_api(u8 index ,const void *data_buf)
{
	ew_puts("vm_write_api\n");
    return vm_write(vm_ptr[index - VM_START_INDEX],data_buf);
}
/*----------------------------------------------------------------------------*/
/** @brief: 按照索引号读存储信息
    @param: index：存储单元索引
    @param: data_buf：需要读取的数据的指针
    @return:正确返回长度
    @author:lj
    @note:  vm_err vm_read_api(u8 index, void * data_buf)
*/
/*----------------------------------------------------------------------------*/
s32 vm_read_api(u8 index, void * data_buf)
{
    return vm_read(vm_ptr[index - VM_START_INDEX],data_buf);
}

/*----------------------------------------------------------------------------*/
/** @brief: 初始化VM的存储模式
    @param: cfg - flash参数
    @return:void
    @author:
    @note:
*/
/*----------------------------------------------------------------------------*/
vm_err vm_init_api(struct flash_cfg *cfg)
{
    //u32 tmp;
    u8 vm_mode;

    if(((cfg->spi_run_mode & 3) == 0) || ((cfg->spi_run_mode & 3) == 1))
    {
        //spi2_parm = SPI_ODD_MODE;
        vm_mode = WINBOND_READ_DATA_MODE;
    }
    else if((cfg->spi_run_mode & 3) == 2)
    {
        //spi2_parm = SPI_DUAL_MODE;
        if(cfg->spi_run_mode & BIT(3))
        {
            vm_mode = WINBOND_FAST_READ_DUAL_OUTPUT_MODE;
        }
        else
        {
            if(cfg->spi_run_mode & BIT(2))
            {
                vm_mode = WINBOND_FAST_READ_DUAL_IO_CONTINUOUS_READ_MODE;
            }
            else
            {
                vm_mode = WINBOND_FAST_READ_DUAL_IO_NORMAL_READ_MODE;
            }
        }
    }
    else
    {
        if(cfg->spi_run_mode & BIT(3))
        {
            vm_mode = WINBOND_FAST_READ_QUAD_OUTPUT_MODE;
        }
        else
        {
            if(cfg->spi_run_mode & BIT(2))
            {
                vm_mode = WINBOND_FAST_READ_QUAD_IO_CONTINUOUS_READ_MODE;
            }
            else
            {
                vm_mode = WINBOND_FAST_READ_QUAD_IO_NORMAL_READ_MODE;
            }
        }
    }

    if(vm_init(app_use_flash_cfg.vmif_addr ,app_use_flash_cfg.vmif_len, cfg->flash_base, vm_mode, 1)!= VM_ERR_NONE)
    {
        puts("\n\n\n\n----WARN-vm_init_err----\n");
		return VM_ERR_INIT;
    }
    vm_puts("\n===vm_init_ok===\n");
	return VM_ERR_NONE;
}

