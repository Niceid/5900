#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


#define DAC_MUX      2
#define INDATA_MUX   3

#define  TASK_STK_SIZE                0x300       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        5       /* Number of identical tasks                          */


//??2-10?????????
/*????*/

#define TaskIdlePrio                    12  //Idle??
#define TaskLineinPrio                  12  //LINE IN??
#define TaskLineinEqPrio                13  //LINE IN sofe eq??
#define TaskPcPrio                      12  //PC??
#define TaskRTCPrio                     12  //RTC??
#define TaskPcCardPrio                  11  //PC???
#define TaskFMradioPrio                 12	//FM
#define TaskFMEngPrio                   13	//FM_Engine
#define TaskMusicPrio                   12

#if !defined(_ENABLE_USB_PRIO_)
#define TaskMusicPhyDecPrio             11
#endif

#define TaskEFOutPrio                   13
#if defined(_SUPPORT_DISC_)
#define TaskDiscPrio					12
#endif
#if defined(_SUPPORT_TAPE_)
#define TaskTapePrio					12
#endif
#if defined(_SUPPORT_MIC_OK_)
#define TaskMicPrio						12
#endif
#if defined(_SUPPORT_CAN_MODEL_)
#define TaskCanPrio						12
#endif

#if defined(_SUPPORT_REVERSE_MODEL_)
#define TaskReversePrio					12
#endif

#if defined(_SUPPORT_AUX1_)
#define TaskLinein2Prio					12
#endif

#define TaskBtPrio						12	//BlueTooth

#define TaskEncodePrio                  12  //ENCODE APP
#define TaskEncRunPrio                  11  //ENCODE RUN
#define TaskEncWFPrio                   10  //ENCODE WRITE FILE

#define TaskEchoMicPrio                 12  //????
#define TaskEchoRunPrio                 9  //????
#define TaskBleFsComPrio                16 //BLE Finger Spinner数据处理 //和TaskRcspPrio同一优先级，两者互斥


/*???????? ??????*/
typedef enum
{
   SecondPrio   =  14,
   TaskUIPrio   =  15,
   TaskRcspPrio      ,
   TaskHtkPrio,
   TaskDevMountPrio = 18,
   TaskSbcPrio      ,
   TaskBtEscoPrio   ,
   TaskBtAecPrio    ,
   TaskBtStackPrio  ,
   TaskBTMSGPrio    ,
   TaskBtLmpPrio    ,
   TaskResoursrPrio ,
   TaskPselPhyDecPrio,
   TaskPselPrio     ,
   TaskBtAecSDPrio  ,
#if defined(_ENABLE_USB_PRIO_)
   TaskMusicPhyDecPrio  ,
#else
   TaskDispPrio     ,
#endif
   TaskMainPrio     ,
   TaskDevCheckPrio , //31   31????
}SYS_TASK_PRIO;

/************脡猫脰脙脮禄麓贸脨隆拢篓碌楼脦禄脦陋 OS_STK 拢漏************/
#define START_TASK_STK_SIZE   1024
#define SECOND_TASK_STK_SIZE   512
#define MP3_TASK_STK_SIZE   0x4000
#define DAC_TASK_STK_SIZE   0x400

#define FILL_TASK_STK_SIZE   0x2000

#define THIRD_TASK_STK_SIZE   0x200
#define UI_TASK_STK_SIZE     0x1000
/*
struct task_req;

struct task_info {
	const char *name;
	void (*init)(void *priv);
	void (*exit)();
};

struct task_req {
	void (*exit_req)(const struct task_info *task);
	int (*switch_req)(const struct task_info *task, const char *name);
};

#define  TASK_REGISTER(task) \
	const struct task_info task __attribute__((section(".task_info")))
*/


#endif

