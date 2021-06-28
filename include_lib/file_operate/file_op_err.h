#ifndef __FILE_OP_ERR_H__
#define __FILE_OP_ERR_H__

enum
{
///---------文件操作器错误
    FILE_OP_NO_ERR =0,
    FILE_OP_ERR_NOT_INIT,       ///<文件操作器没有初始化  1
    FILE_OP_ERR_INIT,           ///<文件操作器初始化错误	2
    FILE_OP_ERR_OP_HDL,         ///<文件操作器指针错误	3
    FILE_OP_ERR_NO_MEM,         ///<文件操作器申请内存失败	4
    FILE_OP_ERR_CMD,            ///<不能识别的操作命令	5

    FILE_OP_ERR_NO_FILE_ALLDEV, ///<所有设备都没有文件	6
    FILE_OP_ERR_NO_FILE_ONEDEV, ///<当前设备没有文件	7
    FILE_OP_ERR_OPEN_FILE,      ///<打开文件失败	8
    FILE_OP_ERR_OPEN_BPFILE,    ///<打开断点文件失败	9
    FILE_OP_ERR_NUM,            ///<打开文件序号错误	10
    FILE_OP_ERR_END_FILE,       ///<超过当前设备最大文件数	11
    FILE_OP_ERR_PRE_FILE,       ///<上一曲==0；	12

    FILE_OP_ERR_PTR,	//13
    FILE_OP_ERR_LGDEV_NULL,      ///<没有设备	14
    FILE_OP_ERR_LGDEV_NO_FIND,   ///<没找到指定的逻辑设备	15
    FILE_OP_ERR_LGDEV_FULL,	//16
    FILE_OP_ERR_LGDEV_MOUNT,	//17
    FILE_OP_ERR_FS,//18


    ERR_MUSIC_API_NULL  = 0x20,//32
    ERR_MUSIC_NULL_OPT,
    ERR_MUSIC_START_DEC,
    SUCC_MUSIC_START_DEC
};

#define SUCC_MUSIC_DEC  FILE_OP_NO_ERR


#endif
