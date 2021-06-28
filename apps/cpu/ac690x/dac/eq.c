#include "sdk_cfg.h"
#include "dac/dac_api.h"
#include "dac/eq_api.h"
#include "dac/eq.h"
#include "crc_api.h"
#include "common/flash_cfg.h"
#include "file_operate/logic_dev_list.h"
#include "dec/decoder_phy.h"
#include "setup.h"

#if EQ_EN

#define USE_CONFIG_BIN_MODE  6
#define USE_EQ_DBG_MALLOC 1

extern void * zebra_malloc(u32 size);
extern void zebra_free(void * P);
#define eq_malloc	zebra_malloc
#define	eq_free		zebra_free

#if USE_EQ_DBG_MALLOC
static eq_dbg_online_t *eq_dbg_online;
#else
static eq_dbg_online_t __eq_dbg_online;
#define eq_dbg_online  (&__eq_dbg_online)
#endif // USE_EQ_DBG_MALLOC

static u8 eq_switch_state = 0;
static u8 eq_switch_type = 0xff;

#if 1
int user_eq_seg_gain[6+1][10] =
{
	{ 0, -3, 0, 0, 0, 0, 0, 0, -5, 0}, ///<Normal
	{ 7, 8, 8, 4, 1, 1, 8, 8, 8, 8}, ///<Rock
	{ 1, 1, 1, 4,2,2, 0, 1, 1, 1}, ///<Pop
	{ 7, 8, 8,4,2,1,1, 1, 1, 1}, ///<Classic
	{ 0, 0, 0, 4, 4, 4, 0, 2, 3, 4}, ///<Jazz
	{-2, 0, 0, 2, 2, 0, 0, 0, 6, 4}, ///<Country
	
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

#if 0
	{-12, -12, -12, 0, 0, 0, 0, -12, -12, -12},
	{-10, -10, -10, 0, 0, 0, 0, -10, -10, -10},
	{-9, -9, -9, 0, 0, 0, 0, -9, -9, -9},
	{-8, -8, -8, 0, 0, 0, 0, -8, -8, -8},
	{-6, -6, -6, 0, 0, 0, 0, -6, -6, -6},
	{-4, -4, -4, 0, 0, 0, 0, -4, -4, -4},
	{-2, -2, -2, 0, 0, 0, 0, -2, -2, -2},
	
	{2, 2, 2, 0, 0, 0, 0, 2, 2, 2},
	{4, 4, 4, 0, 0, 0, 0, 4, 4, 4},
	{6, 6, 6, 0, 0, 0, 0, 6, 6, 6},
	{8, 8, 8, 0, 0, 0, 0, 8, 8, 8},
	{9, 9, 9, 0, 0, 0, 0, 9, 9, 9},
	{10, 10, 10, 0, 0, 0, 0, 10, 10, 10},
	{12, 12, 12, 0, 0, 0, 0, 12, 12, 12},
#endif
};

#else
int user_eq_seg_gain[6+14][10] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, ///<Normal
	{ 0, 8, 8, 4, 0, 0, 0, 0, 2, 2}, ///<Rock
	{-2, 0, 2, 4,-2,-2, 0, 0, 4, 4}, ///<Pop
	{ 4, 2, 0,-3,-6,-6,-3, 0, 3, 5}, ///<Classic
	{ 0, 0, 0, 4, 4, 4, 0, 2, 3, 4}, ///<Jazz
	{-2, 0, 0, 2, 2, 0, 0, 0, 4, 4}, ///<Country

	{-8, -7, -6, -5, -5, -4, 0, 0, 0, 0},
	{-7, -6, -6, -5, -5, -3, 0, 0, 0, 0},
	{-6, -5, -5, -4, -3, -2, 0, 0, 0, 0},
	{-5, -4, -4, -3, -2, -1, 0, 0, 0, 0},
	{-4, -3, -3, -2, -1, 0, 0, 0, 0, 0},
	{-3, -3, -2, -2, 0, 0, 0, 0, 0, 0},
	{-2, -2, -2, -1, -1, 1, 0, 0, 0, 0},
	
	{1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{3, 2, 1, 2, 0, 0, 0, 0, 0, 0},
	{4, 2, 1, 2, 2, 1, 0, 0, 0, 0},
	{5, 3, 2, 2, 2, 1, 0, 0, 0, 0},
	{6, 4, 3, 2, 1, 1, 0, 0, 0, 0},
	{7, 5, 4, 3, 3, 1, 0, 0, 0, 0},
	{8, 6, 5, 4, 3, 4, 0, 0, 0, 0},

};
#endif

#if defined(_HY1037_LOUD_FUNTION_)
int user_loud_seg_gain[1][10] =
{
	//{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//LOUD
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//LOUD
};
#else
int user_loud_seg_gain[1][10] =
{
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 2},//LOUD
};
#endif

#if defined(YS_ADJ_EQ_10K_LOW)
int user_bas_seg_gain[15][5] =
{
	{0, -12, 0, 0, 0},
	{0, -10, 0, 0, 0},
	{0, -8, 0, 0, 0},
	{0, -6, 0, 0, 0},
	{0, -4, 0, 0, 0},
	{0, -2, 0, 0, 0},
	{0, -1, 0, 0, 0},
	
	{0, 0, 0, 0, 0},
	
	{0, 1, 0, 0, 0},
	{0, 2, 0, 0, 0},
	{0, 4, 0, 0, 0},
	{0, 6, 0, 0, 0},
	{0, 8, 0, 0, 0},
	{0, 9, 0, 0, 0},
	{0, 10, 0, 0, 0},
};

int user_tre_seg_gain[15][5] =
{
	{0, 0, 0, -12, 0},
	{0, 0, 0, -10, 0},
	{0, 0, 0, -8, 0},
	{0, 0, 0, -6, 0},
	{0, 0, 0, -4, 0},
    {0, 0, 0, -2, 0},
	{0, 0, 0, -1, 0},
	
	{0, 0, 0, 0, 0},
	
	{0, 0, 0, 1, 0},
	{0, 0, 0, 2, 0},
	{0, 0, 0, 4, 0},
	{0, 0, 0, 6, 0},
	{0, 0, 0, 8, 0},
	{0, 0, 0, 9, 0},
	{0, 0, 0, 10, 0},
};

#else
int user_bas_seg_gain[15][5] =
{
	{0, -12, 0, 0, 0},
	{0, -10, 0, 0, 0},
	{0, -8, 0, 0, 0},
	{0, -7, 0, 0, 0},
	{0, -6, 0, 0, 0},
	{0, -5, 0, 0, 0},
	{0, -4, 0, 0, 0},
	
	{0, -3, 0, 0, 0},
	
	{0, -2, 0, 0, 0},
	{0, -1, 0, 0, 0},
	{0, 0, 0, 0, 0},
	{0, 3, 0, 0, 0},
	{0, 7, 0, 0, 0},
	{0, 9, 0, 0, 0},
	{0, 10, 0, 0, 0},
};

int user_tre_seg_gain[15][5] =
{
	{0, 0, 0, -12, 0},
	{0, 0, 0, -11, 0},
	{0, 0, 0, -10, 0},
	{0, 0, 0, -9, 0},
	{0, 0, 0, -8, 0},
	{0, 0, 0, -7, 0},
	{0, 0, 0, -6, 0},

	{0, 0, 0, -5, 0},
	
	{0, 0, 0, -4, 0},
	{0, 0, 0, -3, 0},
	{0, 0, 0, -2, 0},
	{0, 0, 0, -1, 0},
	{0, 0, 0, 0, 0},
	{0, 0, 0, 5, 0},
	{0, 0, 0, 10, 0},
};
#endif


int user_eq_global_gain[6+1] =
{
	0,-6,-3,-6,-6,-3,
	0
};

void eq_init(void)
{
	soft_eq_init(USE_SOFTWARE_EQ);
    eq_init_api(3);

#if EQ_UART_BDG
    EQ_uart_init();
#endif // EQ_UART_BDG

}
#if USE_SOFTWARE_EQ
#define EQ_CFG_FILE            "cfg_eq.bin"
#else
#define EQ_CFG_FILE            "eq_cfg.bin"
#endif
void eq_enable(void)
{
    tbool ret;
	
	if(1 == eq_switch_state)
	{
		return;
	}
	reg_get_decoder_change(soft_eq_reset_status);
	eq_switch_state = 1;

#if USE_EQ_DBG_MALLOC
    eq_dbg_online = (eq_dbg_online_t *)eq_malloc(sizeof(eq_dbg_online_t));
    if(eq_dbg_online == NULL)
    {
        puts("eq_dbg malloc error\n");
    }
    memset(eq_dbg_online , 0x0 , sizeof(eq_dbg_online_t));
#endif // USE_EQ_DBG_MALLOC

    ret = read_cfg_file((u8*)eq_dbg_online,(u8 *)EQ_CFG_FILE,sizeof(eq_dbg_online_t));

    //read eq config file ok then update the config
  
    if(ret==0)
    {
        puts("default EQ config\n");

        memcpy(eq_dbg_online->eq_gain , user_eq_global_gain , sizeof(user_eq_global_gain));
        memcpy(eq_dbg_online->eq_freq_gain , user_eq_seg_gain , sizeof(user_eq_seg_gain));
        eq_config_api( eq_dbg_online->eq_filt_0 ,
                                eq_dbg_online->eq_filt_1 ,
                                eq_dbg_online->eq_filt_2 ,
                                eq_dbg_online->eq_filt_3 ,
                                eq_dbg_online->eq_filt_4 ,
                                eq_dbg_online->eq_filt_5 ,
                                eq_dbg_online->eq_filt_6 ,
                                eq_dbg_online->eq_filt_7 ,
                                eq_dbg_online->eq_filt_8 ,
                                (int (*)[10])eq_dbg_online->eq_freq_gain ,
                                eq_dbg_online->eq_gain ,
								!USE_SOFTWARE_EQ);
		soft_eq_set_segment_num(eq_dbg_online->eq_cnt);
        eq_mode_sw(eq_dbg_online->eq_type );
    }
	else
	{
        printf("EQ config file,eq_type:%d\n",eq_dbg_online->eq_type);
		eq_dbg_online->eq_gain[USE_CONFIG_BIN_MODE] = user_eq_global_gain[0]; 
		
		//eq_dbg_online->eq_gain[6] = user_eq_global_gain[eq_dbg_online->eq_type]; 
		memcpy(&eq_dbg_online->eq_freq_gain[USE_CONFIG_BIN_MODE] , &user_eq_seg_gain[0] , sizeof(user_eq_seg_gain[0]));
        eq_config_api( eq_dbg_online->eq_filt_0 ,
                       eq_dbg_online->eq_filt_1 ,
                       eq_dbg_online->eq_filt_2 ,
                       eq_dbg_online->eq_filt_3 ,
                       eq_dbg_online->eq_filt_4 ,
                       eq_dbg_online->eq_filt_5 ,
                       eq_dbg_online->eq_filt_6 ,
                       eq_dbg_online->eq_filt_7 ,
                       eq_dbg_online->eq_filt_8 ,
                       (int (*)[10])eq_dbg_online->eq_freq_gain ,
                       eq_dbg_online->eq_gain ,
					!USE_SOFTWARE_EQ);
		soft_eq_set_segment_num(eq_dbg_online->eq_cnt);
        eq_mode_sw(USE_CONFIG_BIN_MODE);
    }
}

void eq_disable(void)
{
	if(0 == eq_switch_state)
	{
		return;
	}
	eq_dis_api();
#if USE_EQ_DBG_MALLOC
    if(eq_dbg_online != NULL)
    {
        puts("eq_dbg_online free\n");
        eq_free(eq_dbg_online);
        eq_dbg_online = NULL;
    }
#endif
	eq_switch_state = 0;
}

void eq_run(short *in,short *out,int npoint)
{
	eq_run_api(in,out,npoint);
}

void eq_mode_sw(int mode)
{
    //u8 i;
	if(0 == eq_switch_state)
	{
		return;		
	}
	
    printf("NEW+++++EQ config file,eq_type:%d\n",mode);
	if(mode <6)
	{
	    eq_dbg_online->eq_gain[USE_CONFIG_BIN_MODE] = user_eq_global_gain[mode]; 
	}
	else
	{
		eq_dbg_online->eq_gain[USE_CONFIG_BIN_MODE] = user_eq_global_gain[6]; 
	}
	
	if(mode <6)//EQ
	{
		memcpy(&eq_dbg_online->eq_freq_gain[USE_CONFIG_BIN_MODE] , &user_eq_seg_gain[mode] , sizeof(user_eq_seg_gain[0]));
	}
	else if(mode == 37)//LOUD
	{
		memcpy(&eq_dbg_online->eq_freq_gain[USE_CONFIG_BIN_MODE] , &user_loud_seg_gain[0] , sizeof(user_loud_seg_gain[0]));
	}
	else
	{
		if(mode < 22)//����
		{
		    memcpy(&user_eq_seg_gain[6], &user_bas_seg_gain[mode - 7],sizeof(user_bas_seg_gain[0]));
		}
		else if(mode < 37)//����
		{
           memcpy(&user_eq_seg_gain[6][5], &user_tre_seg_gain[mode - 22],sizeof(user_tre_seg_gain[0]));
		}
		memcpy(&eq_dbg_online->eq_freq_gain[USE_CONFIG_BIN_MODE] , &user_eq_seg_gain[6] , sizeof(user_eq_seg_gain[0]));
	}

	/*#if defined(_YS_SEL_SETTING_)
    for(i=0;i<10;i++)
    {
        	printf("%d ",eq_dbg_online->eq_freq_gain[6][i]);
			
		if((i == 2)&&(mode == 0))
		{
			if(eq_dbg_online->eq_freq_gain[6][i] !=0)
			{
				eq_dbg_online->eq_freq_gain[6][i] = 0 ;
			}
		}
    }
	#else
    for(i=0;i<10;i++)
    {
        printf("%d ",eq_dbg_online->eq_freq_gain[6][i]);
		
    }
	#endif*/
	
    eq_config_api( eq_dbg_online->eq_filt_0 ,
                   eq_dbg_online->eq_filt_1 ,
                   eq_dbg_online->eq_filt_2 ,
                   eq_dbg_online->eq_filt_3 ,
                   eq_dbg_online->eq_filt_4 ,
                   eq_dbg_online->eq_filt_5 ,
                   eq_dbg_online->eq_filt_6 ,
                   eq_dbg_online->eq_filt_7 ,
                   eq_dbg_online->eq_filt_8 ,
                   (int (*)[10])eq_dbg_online->eq_freq_gain ,
                   eq_dbg_online->eq_gain ,
                   !USE_SOFTWARE_EQ);
	// soft_eq_set_segment_num(eq_dbg_online->eq_cnt);
	eq_mode_api(USE_CONFIG_BIN_MODE);
}

void eq_samplerate_sw(u16 sr)
{
	eq_samplerate_api(sr);
}


/*----------------------------------------------------------------------------*/
/**@brief  update eq info online
   @param  ef_info_addr
   @param  size
   @param  cnt
   @return 1:receive data ok; 0:receive data err
   @note
*/
/*----------------------------------------------------------------------------*/
u8 update_ef_info(void *ef_info_addr,u16 size,u8 cnt)
{
    eq_printf("update_ef_info,size=%d,cnt=%d\n",size,cnt);

    if(eq_dbg_online == NULL)
        return 0;

    if(!cnt)
       my_memcpy((u8*)eq_dbg_online,ef_info_addr,size);
    else
       my_memcpy(((u8*)eq_dbg_online)+(512-4)*cnt,ef_info_addr,size);

    if(cnt == 4)
    {
        if(eq_dbg_online->crc16 == CRC16(eq_dbg_online,sizeof(eq_dbg_online_t)-4))
        {
            puts("eq_info_crc_OK\n");
            eq_config_api( eq_dbg_online->eq_filt_0 ,
                           eq_dbg_online->eq_filt_1 ,
                           eq_dbg_online->eq_filt_2 ,
                           eq_dbg_online->eq_filt_3 ,
                           eq_dbg_online->eq_filt_4 ,
                           eq_dbg_online->eq_filt_5 ,
                           eq_dbg_online->eq_filt_6 ,
                           eq_dbg_online->eq_filt_7 ,
                           eq_dbg_online->eq_filt_8 ,
                           (int (*)[10])eq_dbg_online->eq_freq_gain ,
                           eq_dbg_online->eq_gain ,
                           0);
		 	soft_eq_set_segment_num(eq_dbg_online->eq_cnt);
            eq_mode_sw(eq_dbg_online->eq_type);
            return 1;
        }
        else
        {
            puts("\neq_info_crc_ERR!!!!\n\n");
            return 0;
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/**@brief  配置文件读取函数
   @param  buff 存放配置文件的buff
           cfg_file_num 配置文件号
           cfg_len 配置文件的总长度
   @return ture配置文件读取成功  false配置文件读取失败
   @note   bool read_cfg_file(u8 *buff,u8 cfg_file_num,u16 cfg_len)
*/
/*----------------------------------------------------------------------------*/

extern lg_dev_list * lg_dev_find(u32 drv);
extern tbool syd_drive_open(void **p_fs_hdl,void *p_hdev,u32 drive_base);
extern tbool syd_drive_close(void **p_fs_hdl);
extern bool syd_get_file_byindex(void *p_fs_hdl, void **p_f_hdl, u32 file_number, char *lfn);
extern bool syd_file_close(void *p_fs_hdl, void **p_f_hdl);
extern u16 syd_read(void *p_f_hdl ,u8 _xdata *buff , u16 len);
extern bool syd_get_file_bypath(void *p_fs_hdl,void **p_f_hdl,u8 *path,char*lfn);

bool read_cfg_file(u8 *buff,u8* cfg_file_name,u16 cfg_len)
{
    lg_dev_list * pnode;
    void *fs_hdl = NULL;  //文件系统句柄
    void *file_hdl = NULL;//文件句柄
    eq_dbg_online_t *tmp_eq_dbg_buff;

    pnode = lg_dev_find('A');

    if(!syd_drive_open(&fs_hdl,pnode->cur_dev.lg_hdl->phydev_item,0))//打开文件系统
    {
        eq_puts("syd_drive_open err!\n");
        return FALSE;
    }

    if(!syd_get_file_bypath(fs_hdl ,&file_hdl, cfg_file_name,0))
    {
        eq_puts("syd_get_file_byindex err!\n");
        //以下两部顺序不能变
        syd_file_close(fs_hdl,&file_hdl);//失败的情况下关闭文件
        syd_drive_close(&fs_hdl);//失败的情况下关闭文件系统
        return FALSE;
    }

    syd_read(file_hdl,buff,cfg_len);//读取文件
    tmp_eq_dbg_buff = (eq_dbg_online_t*)buff;
    if(tmp_eq_dbg_buff->crc16 == CRC16((u8*)buff,cfg_len-4))
    {
        eq_puts("eq_cfg_file_crc_succ\n");
        //以下两部顺序不能变
        syd_file_close(fs_hdl,&file_hdl);//完成操作后，关闭文件
        syd_drive_close(&fs_hdl);  //完成操作后，关闭文件系统
        return true;
    }
    else
    {
        eq_puts("eq_cfg_file_crc_err\n");
        //以下两部顺序不能变
        syd_file_close(fs_hdl,&file_hdl);//完成操作后，关闭文件
        syd_drive_close(&fs_hdl);  //完成操作后，关闭文件系统
        return false;
    }
}

u8 get_eq_switch_state(void)
{
	return eq_switch_state;
}

u8 get_eq_default_mode(void)
{
	if(eq_dbg_online == NULL)
	{
		return 0;
	}
	else
	{
		return (u8)eq_dbg_online->eq_type; 
	}
}


void update_eq_table_data(s8 *eq_table,u16 offset,u16 eq_size,s8 *eq_global_gain)
{
   u16 cnt;
   int *eq_gain_p; 

	if(eq_dbg_online == NULL)
	{
		puts("eq not runing!!!\n");
		return;
	}

   if(eq_table != NULL)
   {	   
   		eq_gain_p = (int*)eq_dbg_online->eq_freq_gain;
   		eq_gain_p += offset;
   		for(cnt = 0; cnt < eq_size;cnt++)
   		{
   			eq_gain_p[cnt] = (int)eq_table[cnt];
   		}
   		memcpy(user_eq_seg_gain,eq_dbg_online->eq_freq_gain,sizeof(user_eq_seg_gain));
   }

   if(eq_global_gain != NULL)
   {	   
   		eq_gain_p  = eq_dbg_online->eq_gain;
   		for(cnt = 0; cnt < 6;cnt++)
   		{
   			eq_gain_p[cnt] = (int)eq_global_gain[cnt];
   		}
        memcpy(user_eq_global_gain, eq_dbg_online->eq_gain, sizeof(user_eq_global_gain));
   }
   
   /* puts("---update_eq---:"); */
   /* put_buf(eq_table,eq_size); */
   /* put_buf(eq_global_gain,6); */

}


void set_sys_eq(u8 type)
{
	if((0 == eq_switch_state)||(eq_switch_type == type))
	{
		return;
	}

	if(type)
	{
		eq_mode_sw(eq_dbg_online->eq_type );
	}
	else
	{
		eq_mode_sw(0);
	}
	eq_switch_type = type;
}

#endif

