#ifndef _GPRS_H
#define _GPRS_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"

#include "globaldata.h"
#include "globalfunc.h"

void Send_GPRS_Bdp(void);

unsigned char Add_bpd_to_GPRS_RingBuf(unsigned char send_counter, unsigned char * pbuf, unsigned char len);


void send_registered(void);
unsigned char Server_reg_bak_info(unsigned char* pbuf); //����������ע����Ϣ


void assemble_gprs_send_bdp(unsigned char message_type, unsigned char * databuf,unsigned char datalen);
///////////////////////////////////////////////////////////////////////////
//�������������
///////////////////////////////////////////////////////////////////////////
void ActOn_GPRS_BDP(unsigned char* pbuf,unsigned short len);
//������������Ч���ж�
unsigned char bdp_valid(unsigned char * pbuf,unsigned short len);
//�������·����������
void act_on_update_proc(unsigned char* pbuf,unsigned short len);
//�������·����úͿ��������
void act_on_cmd_bdp(unsigned char* pbuf,unsigned short len);

///////////////////////////////////////////////////////////////////////////
//���ƺ͵���
void cmd_01_message(unsigned char* pbuf,unsigned short len);
//�ص�
void cmd_02_message(unsigned char* pbuf,unsigned short len);
//�·��ܹ���ʱ��
void cmd_03_message(unsigned char* pbuf,unsigned short len);
//���ÿ���
void cmd_04_message(unsigned char* pbuf,unsigned short len);
//�·���ǰʱ��
void cmd_05_message(unsigned char* pbuf,unsigned short len);
//����
void cmd_06_message(unsigned char* pbuf,unsigned short len);
//�·��¶ȹ���ʱ��
//void cmd_07_message(unsigned char* pbuf,unsigned short len);
//���߹���ģʽ�趨
void cmd_08_message(unsigned char* pbuf,unsigned short len);
//�趨���Ʒ���
void cmd_09_message(unsigned char* pbuf,unsigned short len);
//��������ϴ�
void cmd_0A_message(unsigned char* pbuf,unsigned short len);
//��Ӵӽڵ�
void cmd_0B_message(unsigned char* pbuf,unsigned short len);
//ɾ���ӽڵ�
void cmd_0C_message(unsigned char* pbuf,unsigned short len);
//��ѯ�汾��
void cmd_0E_message(unsigned char* pbuf,unsigned short len);
//���õ��ƻ�·˳��
void cmd_0F_message(unsigned char* pbuf,unsigned short len);
//�������ÿ��ƻ�·
void cmd_10_message(unsigned char* pbuf,unsigned short len);
//�������òɼ���·
void cmd_11_message(unsigned char* pbuf,unsigned short len);
//��������IP
void cmd_12_message(unsigned char* pbuf,unsigned short len);
//������������
void cmd_13_message(unsigned char* pbuf,unsigned short len);
//���ö˿ں�
void cmd_14_message(unsigned char* pbuf,unsigned short len);
//����·��
void cmd_15_message(unsigned char* pbuf,unsigned short len);
//����·��
void cmd_16_message(unsigned char* pbuf,unsigned short len);
//���þ�γ��
void cmd_17_message(unsigned char* pbuf,unsigned short len);
//�ϱ�����
void cmd_FF_message(unsigned char* pbuf,unsigned short len);
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//����������
void send_gprs_heart_bdp(void);
//���ط���������ִ�н������
void send_result_return_bdp(unsigned char result);

///////////////////////////////////////////////////////////////////////////
//�����ϴ�
///////////////////////////////////////////////////////////////////////////
//�ϱ�����������
void send_concentrator_parameter_bdp(void);
//�ϱ��汾��
void send_software_version_bdp(void);
//�ϴ�CCID��IMSI
void send_CCID_and_IMSI_bdp(void);
//�ϴ������ڵ����
//void send_single_damp_info_bdp(unsigned char index);
//�ϱ����������õ���
//void send_concentrator_Electricity_bdp(unsigned char *pbuf);
//�ϱ�STM32оƬID
void send_STM32_CHIP_ID_bdp(void);
//�ϱ��������¶�
void send_concentrator_temperature_bdp(void);
//�ϴ������ڵ����(��������)
void send_single_lamp_para_bdp(unsigned char index);
//�ϱ��ź�����
void send_singnal_quality_bdp(void);

//���͵�����Ϣ����
void send_debug_bdp(unsigned char *contents, unsigned char len);
//�ϱ���·��������
void send_loop_debuf_inf0_to_PC(void);
//�ϱ����ƿ�������
void send_lamp_debuf_inf0_to_PC(unsigned char *addr,unsigned short group,unsigned char *contents);
//ʱ�ӳ������ϵͳʱ�䵽������
void send_local_time_bdp(unsigned char alarm);
//�ϱ�����ʱ��
void send_local_working_time_bdp(void);
///////////////////////////////////////////////////////////////////////////
//�澯�ϴ�
///////////////////////////////////////////////////////////////////////////
//�ϱ�����������
void send_concentrator_fault_bdp(unsigned char fault,unsigned char num);
//�ϱ����ƹ���
void send_lamp_fault_bdp(unsigned char index,unsigned char *contents);

///////////////////////////////////////////////////////////////////////////
//����澯
///////////////////////////////////////////////////////////////////////////
//������������ϱ���
void send_concentrator_fault_release_bdp(unsigned char fault,unsigned char num);
//������ƹ��ϱ���
void send_lamp_fault_release_bdp(unsigned char index,unsigned char fault);

///////////////////////////////////////////////////////////////////////////
//�¼��ϴ�
///////////////////////////////////////////////////////////////////////////
//�ϱ��ն�����
void send_terminal_reboot_bdp(void);
//��������
void send_lamp_online_bdp(unsigned char index);
//��������
void send_lamp_offline_bdp(unsigned char index);
//���Ŵ�
void send_Boxdoor_open_bdp(unsigned char index);
//���Źر�
void send_Boxdoor_close_bdp(unsigned char index);

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
#endif
