/*
*********************************************************************************************************
*                                             BC51
*
*                                             CODE
*
*                          (c) Copyright 2015-2016, ZHUHAI JIELI
*                                           All Rights Reserved
*
* File : *
* By   : jamin.li
* DATE : 11/11/2015 build this file
* junqian 20170329 add para inbuf_len,outbuf_len,kick_start_len,cbuf_len 
*********************************************************************************************************
*/
#ifndef _SRC_H_
#define _SRC_H_

#include "rtos/os_api.h"
#include "hw_cpu.h"
#include "cpu.h"
#include "cbuf/circular_buf.h"
#include "common/printf.h"
#include "mem/malloc.h"

typedef struct{
    u16 inbuf_len;      ///SRC�����жϷ���ʱ��SRCģ��һ�δ�cbuf���������������default is 128
    u16 outbuf_len;     ///SRC����жϷ���ʱ��SRCģ��һ��������������� default is 128
    u16 cbuf_len;       //���뻺�棬��С��inbuf_len����������default is 128*12
    u16 kick_start_len; //�����뻺���дﵽ��������ʱ���Զ���SRCת������С��inbuf_len��������. default is 128*4
}src_buf_param_t;    //������src_init_api����֮ǰ�����ô˺����ı�SRC�ڲ�BUF�����ã��������ô˺�������ʹ��Ĭ�����á�


typedef struct
{
    u8 nchannel;        //һ��ת����ͨ��������ȡ�᷶Χ(1 ~ 8)�����֧��8��ͨ��
    u8 reserver[3]; 
    u16 in_rate;        ///���������
    u16 out_rate;       ///���������
    u16 in_chinc;       ///���뷽��,��ͨ��ת��ʱ��ÿͨ�����ݵĵ�ַ����
    u16 in_spinc;       ///���뷽��,ͬһͨ����һ�������ǰһ���ݵĵ�ַ����
    u16 out_chinc;      ///�������,��ͨ��ת��ʱ��ÿͨ�����ݵĵ�ַ����
    u16 out_spinc;      ///�������,ͬһͨ����һ�������ǰһ���ݵĵ�ַ���� 
    void (*output_cbk)(u8* ,u16 ); ///һ��ת����ɺ�����жϻ���ô˺������ڽ���������ݣ���������С��outbuf_len����
} src_param_t;

int src_init_api(src_param_t *parm);  //��ʼ��SRC,����SRC��Ҫ��RAM�ȡ���ʼ���ɹ��򷵻�0,ʧ���򷵻�����ֵ��
u32 src_write_data_api(u8 *buf , u16 len);//�����뻺��cbuf��д����,����ֵΪʵ��д���������
void src_clear_data_api(void);//������뻺��cbuf�е����ݡ�
void src_exit_api(void);//�ر�SRC,�ͷ�SRCʹ�õ�RAM��
u32 get_srccbuf_total_len(void);  //input cbuf total len
u32 get_srccbuf_idle_len(void);   //input cbuf idle ram len
u32 get_srccbuf_data_len(void);   //input cbuf unread data len
int src_init_buf_param(src_buf_param_t *parm); //������src_init_api����֮ǰ�����ô˺����ı�SRC�ڲ�BUF�����ã��������ô˺�������ʹ��Ĭ�����á�
#endif
