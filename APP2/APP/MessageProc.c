#include "MessageProc.h"
#include "struct.h"
#include "GPRS.h"
#include "dacai_touch.h"
#include "PLC.h"
#include "wdg.h"
#include "ExtModule.h"
#include "TCP_Reconnect.h"
#include "IOctl.h"
#include "TimerTask.h"
/********************************************************************************************************** 
** Function name:      Task_COM_PROC
** Descriptions:       ���ڴ���
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��h
**********************************************************************************************************/
extern OS_EVENT * QMsg; 

unsigned char m_touch_com_buf[USART_REC_LEN];
volatile unsigned char m_touch_com_len;

unsigned char m_gprs_com_buf[USART1_REC_LEN];
volatile unsigned short m_gprs_com_len;

unsigned char m_config_com_buf[USART_REC_LEN];
volatile unsigned char m_config_com_len;

unsigned char m_PLC_com_buf[USART_REC_LEN];
volatile unsigned char m_PLC_com_len;

unsigned char m_485_com_buf[USART_REC_LEN];
volatile unsigned char m_485_com_len;

unsigned char m_485_1_com_buf[USART_REC_LEN];
volatile unsigned char m_485_1_com_len;

void MsgProc_Task(void *pdata)
{
	R_msg  msg;
	INT8U  err;
	
	while(1)
	{ 	
		msg = *(R_msg *)OSQPend(QMsg,0,&err); // �ȴ���Ϣ���з�����Ϣ����
		if(err == OS_ERR_NONE)
		{
			switch (msg.msg_type)
			{
				case COM_1_MESSAGE_TYPE:   //GPRS ��Ϣ
					m_gprs_com_len = msg.msg_len;
					memset(m_gprs_com_buf,0,USART1_REC_LEN);
					memcpy(m_gprs_com_buf,(unsigned char *)msg.msg_addr, m_gprs_com_len);
					//��E3000����ģ��ʶ��TCP���ӵ���Connect_and_Init����
					if((g_identify_module_model_flag == 1) || (g_GPRS_in_Reg_stage == TRUE))
					{
						ActOn_TCP_Connect_Bdp(m_gprs_com_buf,m_gprs_com_len);
					}
					else  //���� ������ʱ�����GPRS����
					{
						ActOn_GPRS_BDP(m_gprs_com_buf,m_gprs_com_len);
					}			
					break;
				case COM_2_MESSAGE_TYPE:  //PLC
					m_PLC_com_len = msg.msg_len;
					memset(m_PLC_com_buf,0,USART_REC_LEN);
					memcpy(m_PLC_com_buf,(unsigned char *)msg.msg_addr, m_PLC_com_len);			
					
//					COM4_Send_BDP(m_PLC_com_buf,m_PLC_com_len);
					
					ActOn_RTB42_BDP(m_PLC_com_buf,m_PLC_com_len);  //��Ӧ�ز�·��ģ�鷢�͹����Ĵ�������
					plc_comm_flag = 2;		
					
					break;
				case COM_5_MESSAGE_TYPE:  //������
					m_touch_com_len = msg.msg_len;
					memset(m_touch_com_buf,0,USART_REC_LEN);
					memcpy(m_touch_com_buf,(unsigned char *)msg.msg_addr, m_touch_com_len);				
					ActOn_TouchScreen_BDP(m_touch_com_buf,m_touch_com_len);
					break;
				case COM_3_MESSAGE_TYPE:  //485_1	��չģ��	JP3
					m_485_1_com_len = msg.msg_len;
					memset(m_485_1_com_buf,0,USART_REC_LEN);
					memcpy(m_485_1_com_buf,(unsigned char *)msg.msg_addr, m_485_1_com_len);
					ActOn_ExtModule_BDP(m_485_1_com_buf,m_485_1_com_len);
					break;
				case COM_4_MESSAGE_TYPE:    //485	
					m_485_com_len = msg.msg_len;
					memset(m_485_com_buf,0,USART_REC_LEN);
					memcpy(m_485_com_buf,(unsigned char *)msg.msg_addr, m_485_com_len);
					ActOn_CONFIG_BDP(m_485_com_buf);	
					break;
				case TIMER_1000MS_MESSAGE_TYPE:	
					Send_GPRS_Bdp();
					Send_RTB42_Bdp();   //���ڶ�ʱ����PLC����
					break;
				case TIMER_10S_MESSAGE_TYPE:						
					break;
				case KEY_MESSAGE_TYPE:				
					ActOn_Key_BDP(msg.msg_addr);
					break;
				case FIRST_GET_TS_RTC_MESSAGE_TYPE:
                    sys_recovery_info();
					break;
				default:
					break;
			}
		}
		else
		{
						
		}
	}
}



// void ActOn_Timer_MSG(void)
// {		
//  
// }

//SN���ù��ܺ���
void ActOn_CONFIG_BDP(unsigned char* pbuf)
{	
	unsigned char i = 0;
	
	if(pbuf[0] == 0x7E)
	{
		switch(pbuf[1])
		{
		  case 0x50:    //��ȡSN
			Get_RTU_Info_to_PC();
			break;
		  case 0x51:   //����SN			 
			for(i = 0; i < 8 ; i++)
			{
			   g_E3000_Info.SN[i] =  pbuf[i + 3];
			}	
			Write_RTU_info_to_Flash();
			show_Concentrator_SN();			
			break;  				
		  default:
			break;
		}
	}
		
}
//����SN��PC
 void Get_RTU_Info_to_PC(void)
 {
	unsigned short i = 0;
	
	Read_Flash_RTU_Info();
	
	com4_send_buf[0] = 0x7E;
	com4_send_buf[1] = 0x52;
	com4_send_buf[2] = 40;
	for(i = 0; i < 8 ; i++)
	{
	   com4_send_buf[3 + i] =  g_E3000_Info.SN[i];
	}	
	com4_send_buf[39] = 0xE7;

	COM4_Send_BDP(com4_send_buf, 40);
 }

 //�������ܺ���
void ActOn_Key_BDP(unsigned char val)
{
	unsigned char m = 0,n = 0;
	switch(val)
	{
		case  KEY_OK_BTN:	//ȷ��
			//����ģʽ�£���ȷ��������·ȫ��
			if(g_mannual_debug_flag == 1)
			{ 
				if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//E3000�̵�������
				{
					E3000_Relay_Ctl(OPEN);		//��E3000���м̵���
					g_Concentrator_Para.e3000_relay_enable[0] = 100;
				}
						
				for(m = 0;m < MAX_EXT_MODULE_NUM;m++)
				{
					for(n = 0;n < 4;n++)
					{
						if(g_Concentrator_Para.ext_relay_enable[m][n] != 0xFF)	//��չģ��̵�������
						{	
							if( g_ext_online_flag[m] == ONLINE )	//��չ����						
							{
								g_extmodule_relay[m][n] = 1;
								g_Concentrator_Para.ext_relay_enable[m][n] = 100;
							}
						}
					}
				}
				g_send_concentrator_parameter = 1;  //20S���ϱ�����  ����Ի�·
			}
			break;
		case  KEY_RETURN_BTN:  //���� 
			//����ģʽ�£������ؼ�����·ȫ��
			if(g_mannual_debug_flag == 1)
			{
				if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//E3000�̵�������
				{
					E3000_Relay_Ctl(CLOSE);		//�ر�E3000���м̵���
					g_Concentrator_Para.e3000_relay_enable[0] = 0;
				}
				for(m = 0;m < MAX_EXT_MODULE_NUM;m++)
				{
					for(n = 0;n < 4;n++)
					{
						if(g_Concentrator_Para.ext_relay_enable[m][n] != 0xFF)	//��չģ��̵�������
						{			
							g_extmodule_relay[m][n] = 0;
							g_Concentrator_Para.ext_relay_enable[m][n] = 0;
						}
					}
				}
				g_send_concentrator_parameter = 1;  //20S���ϱ�����  ����Ի�·
			}
			break;
		case  KEY_SET_BTN:    //���ü�			
			break;
		case  KEY_FUNCTION_BTN:   //���ܼ�,����/�رյ���ģʽ
			g_mannual_debug_flag = 1 - g_mannual_debug_flag;
			if(g_mannual_debug_flag == 1)
			{
				Set_DEBUG_LED_Status(OPEN);
				g_debug_countdown_counter = 600;  //Ĭ�ϵ��Կ�ʼ�� 600����Զ���ԭΪ����ģʽ
			}
			else
			{
				Set_DEBUG_LED_Status(CLOSE);
				g_debug_countdown_counter = 0;
			}
			break;
		default:
			break;   
	}
}

void sys_recovery_info(void)
{
	unsigned char i = 0, j = 0;
	unsigned char m_stage_index = 0;
	
	g_now_year = g_ts_year;
	g_now_month = g_ts_month;
	g_now_day = g_ts_day;
	g_now_hour = g_ts_hour;
	g_now_min = g_ts_min;
	g_now_sec = g_ts_sec;
	g_now_week = g_ts_week;
	//�ϵ�ʱ�������ʱ���С��60s����ָ�����ǰ״̬
	if( Read_CMD_Info(g_now_hour,g_now_min,g_now_sec) == 1 )
	{
		if(g_Concentrator_Para.e3000_relay_enable[0] == 100)
		{
			E3000_Relay_Ctl(OPEN);		//��E3000���м̵���
		}
		else
		{
			E3000_Relay_Ctl(CLOSE);		//�ر�E3000���м̵���
		}
		//��չ��·������������������100
		for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
		{
			for(j = 0;j< 4;j++)
			{
				if( (g_extmodule_relay[i][j] == 1)
					&&(g_Concentrator_Para.ext_relay_enable[i][j] != 0xFF) )	//��չ�̵�������
				{
					g_Concentrator_Para.ext_relay_enable[i][j] = 100;
				}
			}
		}
	}
	else
	{
	    //�Զ�������Ĭ�ϵ�ǰ����ʲô�׶�
	     get_working_time(g_working_mode);
		 get_working_hours();
			
	    m_stage_index = judge_light_time_stage(g_now_hour,g_now_min);
		if(m_stage_index > 0)  //��ǰ���ڵ�m_stage_indexʱ�����
			{
			    open_time_stage(m_stage_index - 1);  //ִ����Ӧʱ����ڵĿ���·�͵��ƶ���
			}
	}
}
