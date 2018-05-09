#ifndef _RTB42_ROUTER_H
#define _RTB42_ROUTER_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"
#include "struct.h"
#include "const.h"
////////////////////////////////////////////////////////////////////////////////////////////////
//SSC1642�����ز�ͨ��оƬͨ��Э��
////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��RTB42
void Init_RTB42(void);

//����PLC����   ���ڵ���   ���б��Ļ��浽FIFO�У�ÿ��6�����ڵ���
void Send_RTB42_Bdp(void);

//�ѷ��ͱ��Ĵ򵽻��λ�����ȥ  �����Ƿ���ӳɹ�
unsigned char Add_bpd_to_PLC_RingBuf(unsigned char send_counter, unsigned char * pbuf, unsigned char len);

////////////////////////////////////////////////////////////////////////////////////
#endif
