#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"
#include "struct.h"
#include "const.h"
////////////////////////////////////////////////////////////////////////////////////////////////
#define USART1_REC_LEN	(6*1024)	//����1�������ݳ���6K
#define USART_REC_LEN  	128  	//���ڽ������ݳ���128
#define USART_SEND_LEN  	256  	
////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϣ�������Ͷ���

#define COM_1_MESSAGE_TYPE                  1        //����1��Ϣ
#define COM_2_MESSAGE_TYPE                  2        //����2��Ϣ
#define TIMER_1000MS_MESSAGE_TYPE           3        //1000ms��Ϣ
#define COM_3_MESSAGE_TYPE                  4        //����3��Ϣ
#define COM_4_MESSAGE_TYPE                  5        //����2��Ϣ
#define COM_5_MESSAGE_TYPE                  6        //����5��Ϣ
#define TIMER_2000MS_MESSAGE_TYPE           7        //2000ms��Ϣ
#define KEY_MESSAGE_TYPE                    8        //��尴����Ϣ
#define TIMER_10S_MESSAGE_TYPE              9        //10s��Ϣ
#define FIRST_GET_TS_RTC_MESSAGE_TYPE       20       // ��һ�λ�ȡTSʱ����Ϣ
////////////////////////////////////////////////////////////////////////////////////////
#define QSIZE     32

////////////////////////////////////////////////////////////////////////////////////////
#define PLC_COMMUNICATION_OVERTIMR			600	//ͨ�ų�ʱʱ��10����
#define PLC_READ_OVERTIMR					300	//������ʱʱ��5����
#define DAMP_MAX_NUM	400	//���ɴ洢�ĵ�������

///////////////////////////////////////////////////////////////////////////////////////
#define LARGE_CURRENT_VARIATION_RATIO	0.01	//������䶯����1% 
#define SMALL_CURRENT_VARIATION_RATIO	0.05	//С�����䶯����5% 

#define CURRENT_VARIATION_THRESHOLD		100		//�����䶯��ֵ1A
#define VOLTAGE_VARIATION_THRESHOLD		50		//��ѹ�䶯��ֵ5V
////////////////////////////////////////////////////////////////////////////////////////
#define CONCENTRATOR_FAULT_1	1	//��������·��ͨ��ʧ��
#define CONCENTRATOR_FAULT_2	2	//����������չģ��ͨ��ʧ��
#define CONCENTRATOR_FAULT_3	3	//�����澯
#define CONCENTRATOR_FAULT_4	4	//UPS����
#define CONCENTRATOR_FAULT_5	5	//ͬ��ʱ����RTCʱ�䳬�����

#define LAMP_FAULT_1	1			//����©��
////////////////////////////////////////////////////////////////////////////////////////
//PLC FIFO ���
#define MAX_PLC_FIFO_DEP  	6  

#define MAX_GPRS_FIFO_DEP  	10  
////////////////////////////////////////////////////////////////////////////////////////

extern  RTU_CONFIG_INFO  g_E3000_Info;
extern unsigned char g_read_falsh_data_info;
extern unsigned char g_get_at_update_time_flag;
extern Light_Time_Info g_Light_Time_Info;
extern CONCENTRATOR_PARAMETER g_Concentrator_Para;
extern Light_Time_Info g_Week_Light_Time_Info[7];
extern Light_Time_Info g_Month_Light_Time_Info[31];
/************************************************************************************************
**��Ϣ��������
************************************************************************************************/


extern OS_EVENT * QMsg; 
extern void * QMSG_ARRAY[QSIZE];
extern R_msg  R_msg_Array[QSIZE];
extern unsigned char msg_index;
/************************************************************************************************/

extern unsigned char com1_timeout_count;  //����1ͨ�ų�ʱ������
extern unsigned short com1_recvbyte_count; //����1ͨ�Ž������ݼ�����
extern unsigned char com1_buf[USART1_REC_LEN];       //����1������
extern unsigned char com1_send_buf[USART_SEND_LEN];
extern unsigned char com1_send_len;

extern unsigned char com2_timeout_count;  //����2ͨ�ų�ʱ������
extern unsigned char com2_recvbyte_count; //����2ͨ�Ž������ݼ�����
extern unsigned char com2_buf[USART_REC_LEN];
extern unsigned char com2_send_buf[USART_SEND_LEN];

extern unsigned char com3_timeout_count;  //����3ͨ�ų�ʱ������
extern unsigned char com3_recvbyte_count; //����3ͨ�Ž������ݼ�����
extern unsigned char com3_buf[USART_REC_LEN];
extern unsigned char com3_send_buf[USART_SEND_LEN];

extern unsigned char com4_timeout_count;  //����4ͨ�ų�ʱ������
extern unsigned char com4_recvbyte_count; //����4ͨ�Ž������ݼ�����
extern unsigned char com4_buf[USART_REC_LEN];
extern unsigned char com4_send_buf[USART_SEND_LEN];

extern unsigned char com5_timeout_count;  //����5ͨ�ų�ʱ������
extern unsigned char com5_recvbyte_count; //����5ͨ�Ž������ݼ�����
extern unsigned char com5_buf[USART_REC_LEN];
extern unsigned char com5_send_buf[USART_SEND_LEN];
////////////////////////////////////////////////////////////////////////////////////////
extern unsigned char STM32_CHIP_ID[12];
///////////////////////////////////////////////////
//RN8302 ���
extern uint32_t G_CastProtect;//Ͷ�б�����־,��Ӧλ����������Ͷ�б���λ
extern EffectParSetToSave   G_EffectPar_Info; 

extern PHASE_POWER_ITEM G_PhasePower_Info[4]; 

extern unsigned short g_Voltage_Adjust_Ratio[3];
extern unsigned short g_Current_Adjust_Ratio[3];

extern unsigned short g_Phase_Voltage[3];
extern unsigned short g_Phase_Amp[3];
///////////////////////////////////////////////////
extern unsigned char m_SimCard_num[13];
extern unsigned char g_GPRS_connect_OK;
extern unsigned char g_get_SimCard_flag;
extern unsigned char g_GPRS_in_Reg_stage;
extern unsigned char g_connect_step;
extern unsigned char m_err_counter;
extern unsigned short g_gprs_timeout_2000ms_counter;
extern unsigned char ICCID_and_IMSI_buf[20];	//���ICCID��IMSI
extern unsigned char g_gprs_msg_num;
extern unsigned char g_GPRS_signal_strength;
extern unsigned char g_gprs_module_model;
extern unsigned char g_identify_module_model_flag;


//////////////////////////////////////////////////
extern unsigned char g_report_state_flag;  //��ǰ�Ƿ����ϱ�����״̬
extern unsigned char g_report_step;
//////////////////////////////////////////////////////////
extern unsigned char g_TouchScreen_Connect_OK;
extern unsigned short g_touchscreen_timeout_2000ms_counter;
extern unsigned char g_TouchScreen_Password_buf[12];
extern unsigned char g_TouchScreen_Send_Cmd_Flag;
extern unsigned char g_prompt_flag;
extern unsigned short g_ctl_group;
extern unsigned char g_ctl_phase;
extern unsigned char g_now_screen_id;
extern unsigned char g_password_target_screen_id;
extern unsigned char g_password_judge;
extern unsigned short g_show_damp_index;
extern unsigned short g_show_damp_last_index;
extern unsigned char g_showtime_buf[8][32];
extern unsigned char g_print_buf[64]; 
extern unsigned char g_first_get_TS_rtc_time_flag;
//////////////////////////////////////////////////

//PLC���
extern  unsigned short g_damp_init_num;
extern  unsigned short g_lamp_total_num;
extern unsigned short g_damp_fault_num;
extern unsigned short g_lamp_read_num;
extern  Damp_Info g_Damp_Info[DAMP_MAX_NUM];
extern unsigned char g_router_addr[6];
extern unsigned char g_add_damp_addr[6];
extern unsigned char g_del_damp_addr[6];
extern unsigned char g_ctl_damp_addr[6];
extern unsigned char g_set_damp_addr[6];
extern unsigned char g_read_damp_addr[6];
extern unsigned short g_Regular_Read_time_1000ms_counter;
extern unsigned short g_PLC_timeout_1000ms_counter;
extern unsigned short g_plc_read_1000ms_couter;
extern unsigned short g_router_fault_reboot_num;
extern unsigned char g_router_fault_flag;
extern unsigned char g_plc_msg_num;
extern unsigned char plc_comm_flag;	
extern unsigned char g_lamp_working_time_num;	//���ƹ���ʱ�����
extern unsigned char g_lamp_working_time_index[4];
extern unsigned char g_plc_msg_type;
extern unsigned char g_read_restart_flag;
extern unsigned char g_read_restart_counter;
/////////////////////////////////////////
extern Send_bdp_FIFO      g_Send_bdp_FIFO[MAX_PLC_FIFO_DEP];
extern Send_GPRS_bdp_FIFO g_Send_GPRS_bdp_FIFO[MAX_GPRS_FIFO_DEP];

/////////////////////////////////////////
extern unsigned char g_key_set_flag;
extern unsigned char g_key_function_flag;
extern unsigned short g_debug_countdown_counter;

extern unsigned char g_working_mode;
extern unsigned short t_s[8],t_e[8],t_g[8],t_l[8],t_r[8][3],t_v[8];

extern unsigned char g_rtc_year,g_rtc_month,g_rtc_week,g_rtc_day,g_rtc_hour,g_rtc_min,g_rtc_sec; 
extern unsigned char g_ts_year,g_ts_month,g_ts_week,g_ts_day,g_ts_hour,g_ts_min,g_ts_sec; 
extern unsigned char g_now_year,g_now_month,g_now_week,g_now_day,g_now_hour,g_now_min,g_now_sec; 
extern unsigned char g_set_year,g_set_month,g_set_week,g_set_day,g_set_hour,g_set_min,g_set_sec; 

///////////////////////////////////////////////////////////
extern unsigned short g_LM75A_temp;

////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
extern unsigned char g_extmodule_relay[3][4];
extern CONCENTRATOR_PARAMETER g_Concentrator_Para;
extern unsigned char g_ext_online_flag[3];  //��չģ�����߱�־
extern unsigned char g_ext_offline_counter[3];  //��չģ�����߼�����

extern unsigned char g_terminal_reboot_flag;

/////////////////////////////////////////////////////////////
extern Time_Typedef g_DS1302_TimeValue;   //year ��Ч8bit ��Χ0--256
/////////////////////////////////////////////////////////////
extern unsigned char g_lamp_working_mode;
/////////////////////////////////////////////////////////////
extern unsigned char g_mannual_debug_flag;


extern unsigned char g_send_concentrator_parameter;
extern unsigned char g_rtc_time_fault;
extern unsigned char g_ts_time_fault;
extern unsigned char g_judge_time_fault_flag;

extern unsigned char g_reset_E3000_8302_flag;
extern unsigned char g_reset_ext_8302_flag;
/////////////////////////////////////////////////////////////
extern unsigned char g_loop_total_num;		//�ܻ�·��
extern unsigned char g_loop_OK_num;		//���ϻ�·��
/////////////////////////////////////////////////////////////
// extern unsigned char print_buf[5];
// extern unsigned char print_len;
// extern unsigned short g_terminal_reboot_num;
// extern unsigned short g_GPRS_reconnect_num;
// extern unsigned short g_recv_heart_num;
// extern unsigned short g_send_heart_num;

extern unsigned short g_update_flag;
extern unsigned short g_update_pack_num;
extern unsigned short g_update_timeout_1000ms_counter;


#endif
