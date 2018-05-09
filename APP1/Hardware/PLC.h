#ifndef _PLC_H
#define _PLC_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"
#include "struct.h"
#include "const.h"
////////////////////////////////////////////////////////////////////////////////////////////////
//SSC1642�����ز�ͨ��оƬͨ��Э��
////////////////////////////////////////////////////////////////////////////////////////////////

//����PLC����   ���ڵ���   ���б��Ļ��浽FIFO�У�ÿ��6�����ڵ���
void Send_RTB42_Bdp(void);

//�ѷ��ͱ��Ĵ򵽻��λ�����ȥ  �����Ƿ���ӳɹ�
unsigned char Add_bpd_to_PLC_RingBuf(unsigned char send_counter, unsigned char * pbuf, unsigned char len,unsigned char type);

////////////////////////////////////////////////////////////////////////////////////


void ActOn_RTB42_BDP(unsigned char * pbuf,unsigned char len);

void confirm_message_handle_func(void);
void deny_message_handle_func(unsigned char error);
void request_read_handle_func(unsigned char *pbuf);
void route_report_handle_func(unsigned char *pbuf);

void send_hardware_init_bdp(void);
void send_parameter_init_bdp(void);
void send_plc_reboot_bdp(void);
void send_plc_stop_bdp(void);
void send_plc_restore_bdp(void);
void send_set_route_addr_bdp(void);
void send_add_damp_bdp(unsigned char * pbuf);
void send_del_damp_bdp(unsigned char * pbuf);
void send_query_damp_info_bdp(void);
void send_monitor_damp_bdp(unsigned char *damp_addr);
void send_read_need_bdp(unsigned char * pbuf);
void send_read_reply_bdp(unsigned char read_flag);
void send_plc_ack_bdp(void);

void send_set_damp_addr_bdp(unsigned char* dest_addr,unsigned char* new_addr,unsigned char loops);
void send_set_damp_group_bdp(unsigned char *pbuf,unsigned char group);
void send_plc_cmd_bdp(unsigned char *addr,unsigned short group,unsigned char *contents);
void send_system_time_bdp(void);
void send_working_time_bdp(void);
void send_working_mode_bdp(void);
void send_lamp_loop_order_bdp(unsigned char *contents);

void data_handle_func(unsigned char *pbuf);

void read_fail_judge_func(unsigned char *last_addr);

////////////////////////////////////////////////////////////////////////////////////
#endif
