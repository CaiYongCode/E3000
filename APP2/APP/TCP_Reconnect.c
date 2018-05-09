#include "TCP_Reconnect.h"
#include "RN8302.h"
#include "BIT.h"
#include "GPRS.h"
#include "PLC.h"
#include "dacai_touch.h"
#include "Key.h"
#include "IOctl.h"
#include "wdg.h"
#include "LM75A.h"
#include "globaldata.h"
#include "ExtModule.h"
#include "DS1302.h"
#include "Update.h"

#define DELAY  1000

//��ӡ��Ϣ
//SIM900A
unsigned char g_CGCLASS1[32]   = "YDT������óɹ�   "; 
unsigned char g_CGCLASS2[32]   = "YDT�������ʧ��   ";

unsigned char g_CIPMODE0[32]   = "��ʼ����ģʽ����  ";
unsigned char g_CIPMODE1[32]   = "����ģʽ���óɹ�  "; 
unsigned char g_CIPMODE2[32]   = "����ģʽ����ʧ��  ";
 
unsigned char g_CLPORT0[32]    = "��ʼ���ض˿�����  ";
unsigned char g_CLPORT1[32]    = "���ض˿����óɹ�  "; 
unsigned char g_CLPORT2[32]    = "���ض˿�����ʧ��  "; 

unsigned char g_CIPCCFG0[32]   = "��ʼ���û�����    "; 
unsigned char g_CIPCCFG1[32]   = "���������óɹ�    "; 
unsigned char g_CIPCCFG2[32]   = "����������ʧ��    "; 

unsigned char g_CIPSTART0[32]  = "��ʼ������������  ";
unsigned char g_CIPSTART1[32]  = "�����������óɹ�  "; 
unsigned char g_CIPSTART2[32]  = "������������ʧ��  ";

unsigned char g_REG0[32]       = "��ʼ������ע��    "; 
unsigned char g_REG1[32]       = "������ע��ɹ�    "; 
unsigned char g_REG2[32]       = "������ע��ʧ��    ";

//EC20
unsigned char g_QICSGP0[32]   = "��ʼ����TCP/IP      ";
unsigned char g_QICSGP1[32]   = "TCP/IP���óɹ�      "; 
unsigned char g_QICSGP2[32]   = "TCP/IP����ʧ��      ";
 
unsigned char g_CPIN0[32]     = "��ʼ��ѯPIN״̬       ";
unsigned char g_CPIN1[32]     = "PIN�ѽ���             "; 
unsigned char g_CPIN2[32]     = "PIN������             ";

unsigned char g_CREG0[32]     = "��ʼ��ѯCS����      "; 
unsigned char g_CREG1[32]     = "CS����ע��ɹ�      "; 
unsigned char g_CREG2[32]     = "CS����ע��ʧ��      ";

unsigned char g_CCID0[32]     = "��ʼ����CCID      	 "; 
unsigned char g_CCID1[32]     = "����CCID�ɹ�        "; 
unsigned char g_CCID2[32]     = "����CCIDʧ��        ";

unsigned char g_IMSI0[32]     = "��ʼ����IMSI        "; 
unsigned char g_IMSI1[32]     = "����IMSI�ɹ�        "; 
unsigned char g_IMSI2[32]     = "����IMSIʧ��        ";

unsigned char g_QIACT0[32]    = "��ʼ����PDP 		 ";
unsigned char g_QIACT1[32]    = "PDP����ɹ�         "; 
unsigned char g_QIACT2[32]    = "PDP����ʧ��         "; 

unsigned char g_QIOPEN0[32]   = "��ʼ��������        ";
unsigned char g_QIOPEN1[32]   = "�������ӳɹ�        "; 
unsigned char g_QIOPEN2[32]   = "��������ʧ��        ";
/////////////////////////////////////////////////////////////////////

unsigned char CIPSTART_timer = 0;

//unsigned char debug_buf1[128] = {0};
/********************************************************************************************************** 
**********************************************************************************************************/
extern OS_EVENT * QMsg; 


/********************************************************************************************************** 
** Function name:      Task_2000ms_Timer
** Descriptions:       TCP ���ߺ�ʵ����������
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
unsigned char m_timeout_counter = 0;
void Tcp_Reconnect_proc(void *pdata)
{
   
	while(1)
	{ 	
		g_gprs_timeout_2000ms_counter++;
// 		//��ӡGPRS��ʱ����
// 		print_len = data_conversion(print_buf,g_gprs_timeout_2000ms_counter,DECIMAL0);
// 		show_text(DGUS_PRINT_SHOW_ADDR,9,print_buf,print_len);
        //////////////////////////////////////////////////////////////////////////////
        //ʶ�� ����
		if(g_identify_module_model_flag == 1)  //ʶ��GPRSģ��
		{
			identify_gprs_module_model();
		}
		//////////////////////////////////////////////////////////////////////////////
		//TCP ���� ����   ��� TCP ���� �Ӵ˹��� ��ʼ ���� g_GPRS_in_Reg_stage = 1
	    if( g_GPRS_in_Reg_stage == 1)
		{
		   
			if(g_gprs_module_model == SIM900)	//SIM900A
			{

				SIM900A_connect_Step_by_Step();
			}
			else if(g_gprs_module_model == EC20)	//EC20
			{
				EC20_connect_Step_by_Step();
			} 
							
		}
      //////////////////////////////////////////////////////////////////////////////////////////
      //ע��    ������ʼ�ϱ���ʱ���ڴ˺�����ʵ��

		//������ɽ�����Ϣ�ϱ�
		if(g_report_state_flag == 1) //��ǰ���ڲ����ϱ�״̬
		{
			Report_Func();	
		}

       //////////////////////////////////////////////////////////////////////////////////////////
	   //TCP �����ж�
	   //���ݵ�ǰGPRS���ڵ�״̬(ע��/����) ���������ж�ʱ��
		
		if(g_GPRS_in_Reg_stage == 0)  //����ע��״̬
		{
			//����ע��״̬ 60�������
			m_timeout_counter = 30;
		}
		else
		{
			//��ע��״̬ 180�������  ��ֹ������ע������з���һ�����Ķ��ղ����ظ�Ӷ����������ܳ�������
			m_timeout_counter = 90;
		}
		//������߼�����(g_gprs_timeout_2000ms_counter)�ļ���ֵ�����趨��ʱ�䣬���ж�Ϊ����  
		//��TCP������������  ״̬�� TCP ���� ����  g_GPRS_in_Reg_stage = 1  
		if(g_gprs_timeout_2000ms_counter >= m_timeout_counter)  //����
		{				    
			show_text(DGUS_STATE_SHOW_ADDR,9,"��ʱ����          ",strlen("��ʱ����          "));
			Reset_parameter_to_Reconnect();				
		}

		
		OSTimeDly(1000);		
	}			
}

void Reset_parameter_to_Reconnect(void)
{
	g_update_flag = 0;				//�˳�����״̬
	
	Close_Connect();                  //�ر�TCP����
	Reset_GPRS_Module();              //���¸�λģ��
	g_connect_step = 0;               //���Ӳ�������
	g_GPRS_connect_OK = 0;            //GPRS����
	Set_GPRS_LED_Status(CLOSE);       //���LED��ʾGPRS����
	Set_GPRS_Status_to_TC(OFFLINE);   //��������ʾGPRS����   
	m_err_counter = 0;                //֮ǰ��������m_err_counter=16,������߱�����0,���򲻻�ִ��Connect_Server
	g_identify_module_model_flag = 1; //���õ�ʶ��״̬
	g_GPRS_in_Reg_stage = 0;          //����ע��״̬
	g_gprs_timeout_2000ms_counter = 0;//�������߼����� ��ִֻ֤��һ��	
		
// 	//�������ͽ��ռ�������
// 	g_send_heart_num = 0;
// 	print_len = data_conversion(print_buf,g_send_heart_num,DECIMAL0);
// 	show_text(DGUS_PRINT_SHOW_ADDR,5,print_buf,print_len);
// 	g_recv_heart_num = 0;
// 	print_len = data_conversion(print_buf,g_recv_heart_num,DECIMAL0);
// 	show_text(DGUS_PRINT_SHOW_ADDR,7,print_buf,print_len);
}
//���ӷ�����
void Connect_Server(void)
{
	unsigned char buf[96] = "AT+QIOPEN=1,1,\"TCP\",\"";  
	
	memcpy(&buf[21],g_E3000_Info.ip,g_E3000_Info.ip_len);
	memcpy(&buf[21+g_E3000_Info.ip_len],"\",",2);
	memcpy(&buf[21+g_E3000_Info.ip_len+2],g_E3000_Info.port,g_E3000_Info.port_len);
	memcpy(&buf[21+g_E3000_Info.ip_len+2+g_E3000_Info.port_len],",0,2\r\n",6);
	
	uart_send_char(buf);
}
//��һ�� ʶ����EC20 ����SIM900A  g_identify_module_model_flag Ϊ��־
//�ڶ��� TCP���ӳ�ʼ��   g_GPRS_in_Reg_stage Ϊ��־
void ActOn_TCP_Connect_Bdp(unsigned char * pbuf, unsigned char len)
{

    g_gprs_timeout_2000ms_counter = 0;  //�յ�GPRS���ģ��ѳ�ʱ����������

   //��һ�� ʶ�����
	if(g_identify_module_model_flag == 1)  //��ȡGPRSģ���ͺ�
	{
		if(strstr(pbuf,"SIM900") != NULL )
		{
			g_gprs_module_model = SIM900;
			g_identify_module_model_flag = 0;
			g_GPRS_in_Reg_stage = 1;  //������״̬  ע��״̬ 
		}
		else if(strstr(pbuf,"EC20") != NULL )
		{
			g_gprs_module_model = EC20;
			g_identify_module_model_flag = 0;
			g_GPRS_in_Reg_stage = 1;  //������״̬  ע��״̬ 
		}
		else
		{
			show_text(DGUS_STATE_SHOW_ADDR,9,"�汾180209 ģ��ʶ����...      ",strlen("�汾180209 ģ��ʶ����...     "));
		}
		
	}
	//�ڶ��� ��ӦTCP���ӹ���
	if(g_GPRS_in_Reg_stage == 1)  //GPRS��ע��׶�
	{
		if(g_gprs_module_model == SIM900)	//SIM900A
		{
			SIM900A_connect_to_gprs_net_proc(pbuf,len);
		}
		else if(g_gprs_module_model == EC20)	//EC20
		{
			EC20_connect_to_gprs_net_proc(pbuf);
		} 
	}
	
}

//�ر�GPRS����
void Close_Connect(void)
{
	OSTimeDly(DELAY);
	uart_send_char("+++");            //�˳�͸��ģʽ
	OSTimeDly(DELAY);
	if(g_gprs_module_model == SIM900)
	{
		uart_send_char("AT+CIPSHUT\r\n");	
		OSTimeDly(DELAY);
		uart_send_char("AT+CIPCLOSE=1\r\n");  //�ر�TCP����
	}
	else if(g_gprs_module_model == EC20)
	{
		uart_send_char("AT+QICLOSE=1\r\n");  //�ر�TCP����
		OSTimeDly(DELAY);
		uart_send_char("AT+QIDEACT=1\r\n");    //ͣ��PDP
	} 
	OSTimeDly(DELAY);
}

//ʶ��ģ���ͺ�
void identify_gprs_module_model(void)
{
// 	uart_send_char("ATE0\r\n");	//�ر�GPRSģ�����
// 	OSTimeDly(DELAY);
	uart_send_char("ATI\r\n");	//ʶ��ģ���ͺ�
}

//SIM900Aע�����������
void SIM900A_connect_Step_by_Step(void)
{
	if(m_err_counter < 10)  //���δ������Ӵ���������10��
	{
		switch(g_connect_step)
		{
			case 0:
				uart_send_char("AT+CSQ\r\n");
				break;
			case 1:
				uart_send_char("AT+CGCLASS=\"B\"\r\n");
				break;
			case 2:
				uart_send_char("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");  //
				break;
			case 3:
				uart_send_char("AT+CIPCSGP=1,\"CMNET\"\r\n"); 
				break;
			case 4:
				uart_send_char("AT+CCID\r\n");
				break;
			case 5:
				uart_send_char("AT+CIMI\r\n");
				break;
			case 6:
				uart_send_char("AT+CIPMODE=1\r\n");    //͸��ģʽ����
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPMODE0,strlen(g_CIPMODE0));
				break;
			case 7:
				uart_send_char("AT+CLPORT=\"TCP\",\"20002\"\r\n");  //TCP  ���ض˿ں����óɹ�
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CLPORT0,strlen(g_CLPORT0));
				break;
			case 8:
				uart_send_char("AT+CGATT=1\r\n");  //���Ų���
				break;
			case 9:
				uart_send_char("AT+CIPCCFG=5,2,1024,1\r\n");  //������
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPCCFG0,strlen(g_CIPCCFG0));
				CIPSTART_timer = 0;
				break;
			case 10:
				if(CIPSTART_timer == 0)
				{
#ifdef  WORKING_SERVER_IP
					if(g_E3000_Info.ip_set == 0)
					{
						uart_send_char("AT+CIPSTART=\"TCP\",\"kuitun.daominglight.com\",\"24680\"\r\n");  //��������	��ʽ������
					}
					else
					{
						Connect_Server();
					}
#else
					uart_send_char("AT+CIPSTART=\"TCP\",\"dev.daominglight.com\",\"24680\"\r\n");  //��������	���Է�����
#endif
					show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPSTART0,strlen(g_CIPSTART0));
				}
				CIPSTART_timer++;
				break;
			case 11:  //����ע�ᱨ�� 
				send_registered();
				show_text(DGUS_STATE_SHOW_ADDR,9,g_REG0,strlen(g_REG0));
				break;
			default:
				break;	  	
		}
	}
	else     //���δ������Ӵ�������10��
	{
		show_text(DGUS_STATE_SHOW_ADDR,9,"����������--ֹͣ  ",strlen("����������--ֹͣ  "));
		g_gprs_timeout_2000ms_counter = 0;  //����10��,��ѳ�ʱ����������
		g_GPRS_in_Reg_stage = 0; //�˳�����״̬   �˳�ע��״̬
	}
}

//EC20ע�����������
void EC20_connect_Step_by_Step(void)
{
	if(m_err_counter < 10)  //���δ������Ӵ���������10��
   	{
		switch(g_connect_step)
		{
			case 0:
				uart_send_char("AT+CSQ\r\n");
				break;
			case 1:	
				uart_send_char("AT+QCCID\r\n");	//��ѯCCID
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CCID0,strlen(g_CCID0));
				break;
			case 2:
				uart_send_char("AT+CIMI\r\n");	//��ѯIMSI
				show_text(DGUS_STATE_SHOW_ADDR,9,g_IMSI0,strlen(g_IMSI0));
				break;	
			case 3:
				uart_send_char("AT+QICSGP=1,1,\"CMIOT\",\"\",\"\",1\r\n");	//����TCP/IP
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QICSGP0,strlen(g_QICSGP0));
				break;
			case 4: 
				uart_send_char("AT+CREG?\r\n");  //��ѯCS����
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CREG0,strlen(g_CREG0));
				break;
			case 5:				
				uart_send_char("AT+QIACT=1\r\n");    //����PDP
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QIACT0,strlen(g_QIACT0));
				break;
			case 6:	
#ifdef  WORKING_SERVER_IP

//				uart_send_char("AT+CIPSTART=\"TCP\",\"kuitun.daominglight.com\",\"24680\"\r\n");  //��������	��ʽ������

				Connect_Server();
#else
//				uart_send_char("AT+QIOPEN=1,1,\"TCP\",\"dev.daominglight.com\",24680,0,2\r\n");  //���ӷ�����			//���Է�����
				Connect_Server();
#endif
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QIOPEN0,strlen(g_QIOPEN0));	
				break;
			case 7:  //����ע�ᱨ�� 
				send_registered();
				show_text(DGUS_STATE_SHOW_ADDR,9,g_REG0,strlen(g_REG0));
				break;
			default:
				break;	  	
		}
   	}
	else     //���δ������Ӵ�������10��
   	{
   	    show_text(DGUS_STATE_SHOW_ADDR,9,"����������--ֹͣ  ",strlen("����������--ֹͣ  "));
		g_gprs_timeout_2000ms_counter = 0;  //����10��,��ѳ�ʱ����������
		g_GPRS_in_Reg_stage = 0; //�˳�����״̬   �˳�ע��״̬
		g_connect_step = 0;
   	}
}

//SIM900Aע����������践�ر��Ĵ�����
void SIM900A_connect_to_gprs_net_proc(unsigned char* pbuf,unsigned short len)
{
    unsigned char ret;
	char *buf = NULL;
	unsigned char i = 0,j = 0;
	unsigned char CSQ = 0;
	
	switch(g_connect_step)  //��ǰ�ظ汨������ʲô����
	{
		case 0:
			if( strstr(pbuf,"CSQ") == NULL )  //��ȡ�ź�ǿ��
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,"�汾E3000 V1.2      ",strlen("�汾E3000 V1.2     "));
			}
			else
			{
				if((pbuf[32]>='1')&&(pbuf[32]<='3')&&(pbuf[33]>='0')&&(pbuf[33]<='9'))
				{
					CSQ = a_to_i(pbuf[32])*10+a_to_i(pbuf[33]);
					if(CSQ >= 20)
					{
						g_GPRS_signal_strength = 3;
					}
					else
					{
						g_GPRS_signal_strength = 2;
					}
				}
				else
				{
					g_GPRS_signal_strength = 1;
				}
				g_E3000_Info.SQ = CSQ;
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 1:
			if( strstr(pbuf,"OK") == NULL )  
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CGCLASS2,strlen(g_CGCLASS2));
			}
			else    //�ƶ�̨������óɹ�
			{
				g_connect_step ++;
				m_err_counter = 0;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CGCLASS1,strlen(g_CGCLASS1));
			}
			break;
		case 2:
			if( strstr(pbuf,"OK") == NULL )
			{
				m_err_counter++;
			}
			else
			{
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 3:
			if( strstr(pbuf,"OK") == NULL )
			{
				m_err_counter++;
			}
			else
			{
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 4:
			if(strstr(pbuf,"8986") == NULL )
			{
				m_err_counter++;
			}
			else
			{
				j = 0;
				buf = strstr(pbuf,"8986");
				for( i = 0;i <= 9;i++)
				{
					ICCID_and_IMSI_buf[i] = a_to_i( buf[j] )*16+a_to_i( buf[j+1] );
					j = j+2;
				}
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 5:
			if(strstr(pbuf,"460") == NULL )
			{
				m_err_counter++;
			}
			else
			{
				j = 1;					
				buf = strstr(pbuf,"460");
				ICCID_and_IMSI_buf[10] = a_to_i(buf[0]);
				for(i = 11;i <= 17;i++)
				{
					ICCID_and_IMSI_buf[i] = a_to_i( buf[j] )*16+a_to_i( buf[j+1] );
					j = j+2;
				}
				g_connect_step ++;
				m_err_counter = 0;
			}	
			break;
		case 6:    //͸��ģʽ����
			if( strstr(pbuf,"OK") == NULL )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPMODE2,strlen(g_CIPMODE2));
			}
			else
			{
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPMODE1,strlen(g_CIPMODE1));
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 7:  //���ض˿ں����óɹ�
			if( strstr(pbuf,"OK") == NULL )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CLPORT2,strlen(g_CLPORT2));
			}
			else
			{
				g_connect_step ++;
				m_err_counter = 0;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CLPORT1,strlen(g_CLPORT1));
			}
			break;
		case 8:  ////���Ų���
			if( strstr(pbuf,"OK") == NULL )
			{
				m_err_counter++;
			}
			else
			{
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 9:    //������
			if( strstr(pbuf,"OK") == NULL )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPCCFG2,strlen(g_CIPCCFG2));
			}
			else
			{
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPCCFG1,strlen(g_CIPCCFG1));
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 10:  //��������
			if( strstr(pbuf,"CONNECT") == NULL )
			{	 
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPSTART2,strlen(g_CIPSTART2));
			}
			else
			{
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CIPSTART1,strlen(g_CIPSTART1));	
				g_connect_step ++;
				m_err_counter = 0;
			}
			if(CIPSTART_timer >= 10)
			{
				m_err_counter++;   
				CIPSTART_timer = 0;
			}			
			break;
		case 11:   //ע�����������
			ret = Server_reg_bak_info(pbuf);
			if(ret == 1)  //�������ɹ���֤
			{
				g_connect_step ++;
				m_err_counter = 0;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_REG1,strlen(g_REG1));					
			}
			else
			{
				m_err_counter++;
			}
			break;
		case 12:
			g_GPRS_in_Reg_stage = 0;  //ע����� ������������ģʽ ���շ���������
			send_gprs_heart_bdp();     //����֮������һ֡����			
			g_connect_step ++;
		
// 			g_send_heart_num++;
// 			//��ӡ�����������������Ӳ���				
// 			print_len = data_conversion(print_buf,g_GPRS_reconnect_num,DECIMAL0);
// 			show_text(DGUS_PRINT_SHOW_ADDR,15,print_buf,print_len);
// 			g_GPRS_reconnect_num++;
		
		   //�����ϱ� ��ز�������
			g_report_state_flag = 1; //��������ϱ�״̬	
			g_report_step = 0;
			show_text(DGUS_STATE_SHOW_ADDR,9,"��������ϱ�����    ",strlen("��������ϱ�����    "));
			break;
		default:
			break;		
	}
}

//EC20ע����������践�ر��Ĵ�����
void EC20_connect_to_gprs_net_proc(unsigned char *pbuf)
{
	unsigned char ret = 0;
	unsigned char *buf = NULL;
	unsigned char i = 0,j = 0;
	unsigned char CSQ = 0;	//�ź�ǿ��
	
	switch(g_connect_step)  
	{
		case 0:		//��ѯ�ź�ǿ��
			if( strstr(pbuf,"OK") == NULL )
			{
			  show_text(DGUS_STATE_SHOW_ADDR,9,"�汾E3000 V1.3      ",strlen("�汾CE3000 V1.3     "));
			}
			else
			{
				if(pbuf[16] == 0x2C) //�ַ�','
				{
					CSQ = a_to_i(pbuf[15]);
					g_GPRS_signal_strength = 1;
				}
				else
				{
					CSQ = a_to_i(pbuf[15])*10+a_to_i(pbuf[16]);
					g_GPRS_signal_strength = 2;
				}							
				if(CSQ == 99)  //���һֱ��99 �����ⲻ��ǿ��  ��Ҫһֱ���
				{
					show_text(DGUS_STATE_SHOW_ADDR,9,"�ź���ǿ��",strlen("�ź���ǿ��"));
				}
				g_E3000_Info.SQ = CSQ;
				
				g_connect_step++;
			}						
			break;
		case 1:		//��ѯCCID
			if(strstr(pbuf,"8986") == NULL )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CCID2,strlen(g_CCID2));
			}
			else
			{
				buf = strstr(pbuf,"8986");
				for( i = 0;i <= 9;i++)
				{
					ICCID_and_IMSI_buf[i] = a_to_i( buf[j] )*16+a_to_i( buf[j+1] );
					j = j+2;
				}
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CCID1,strlen(g_CCID1));
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 2:		//��ѯIMSI
			if(strstr(pbuf,"460") == NULL )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_IMSI2,strlen(g_IMSI2));
			}
			else
			{
				j = 1;					
				buf = strstr(pbuf,"460");
				ICCID_and_IMSI_buf[10] = a_to_i(buf[0]);
				for(i = 11;i <= 17;i++)
				{
					ICCID_and_IMSI_buf[i] = a_to_i( buf[j] )*16+a_to_i( buf[j+1] );
					j = j+2;
				}					
				show_text(DGUS_STATE_SHOW_ADDR,9,g_IMSI1,strlen(g_IMSI1));
				g_connect_step ++;
				m_err_counter = 0;
			}			
			break;
		case 3:		//����TCP/IP
			if( strstr(pbuf,"OK") == NULL )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QICSGP2,strlen(g_QICSGP2));
			}
			else
			{				
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QICSGP1,strlen(g_QICSGP1));
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 4:		//��ѯCS����
			buf = strstr(pbuf,"CREG:");
			if( (buf == NULL)||((buf[8] != '1')&&(buf[8] != '5')) )  
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CREG2,strlen(g_CREG2));
			}
			else    
			{
				show_text(DGUS_STATE_SHOW_ADDR,9,g_CREG1,strlen(g_CREG1));
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 5:		//����PDP
			if( (strstr(pbuf,"OK") == NULL)||(strstr(pbuf,"ERROR") != NULL) )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QIACT2,strlen(g_QIACT2));
			}
			else
			{
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QIACT1,strlen(g_QIACT1));
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 6:  //��������
			if( strstr(pbuf,"CONNECT") == NULL )
			{
				m_err_counter++;
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QIOPEN2,strlen(g_QIOPEN2));
			}
			else
			{
				show_text(DGUS_STATE_SHOW_ADDR,9,g_QIOPEN1,strlen(g_QIOPEN1));
				g_connect_step ++;
				m_err_counter = 0;
			}
			break;
		case 7:   //ע�����������
			ret = Server_reg_bak_info(pbuf);
			if(ret == 1)  //�������ɹ���֤
			{
			   g_GPRS_connect_OK = 1;  //�յ��������ı��� ֤��������
			   Set_GPRS_LED_Status(OPEN);
			   Set_GPRS_Status_to_TC(g_GPRS_signal_strength);  //��ʾGPRS����
			   g_gprs_timeout_2000ms_counter = 0;  //�յ�GPRS���ģ��ѳ�ʱ����������
		
			   g_connect_step ++;
			   m_err_counter = 0;
			   show_text(DGUS_STATE_SHOW_ADDR,9,g_REG1,strlen(g_REG1));
			}
			else
			{
			   m_err_counter++;
			   show_text(DGUS_STATE_SHOW_ADDR,9,"��������֤ʧ��",strlen("��������֤ʧ��"));
			}
			break;
		case 8:
			g_GPRS_in_Reg_stage = 0;  //ע����� ��������ϱ�����  ���շ���������
			send_gprs_heart_bdp();     //����֮������һ֡����			
			g_connect_step ++;
		
// 			g_send_heart_num++;
// 			//��ӡ�����������������Ӳ���				
// 			print_len = data_conversion(print_buf,g_GPRS_reconnect_num,DECIMAL0);
// 			show_text(DGUS_PRINT_SHOW_ADDR,15,print_buf,print_len);
// 			g_GPRS_reconnect_num++;
		
		   //�����ϱ� ��ز�������
			g_report_state_flag = 1; //��������ϱ�״̬	
			g_report_step = 0;
			show_text(DGUS_STATE_SHOW_ADDR,9,"��������ϱ�����    ",strlen("��������ϱ�����    "));
			break;
		default:
			break;				
	}
}

//�ϱ���Ϣ���ܺ���
void Report_Func(void)
{
	unsigned short i = 0;
	
	switch(g_report_step)
		{
			case 0:
				if(g_terminal_reboot_flag == 1)
				{
					send_terminal_reboot_bdp();	//�����ն�������Ϣ
					g_terminal_reboot_flag = 0;
				}					
				g_report_step++;
				break;
			case 1:		//�ϱ�����������
				send_concentrator_parameter_bdp();
				g_report_step++;	
				show_text(DGUS_STATE_SHOW_ADDR,9,"����ϱ�����������    ",strlen("����ϱ�����������    "));
				break;
			case 2:		//�ϱ��汾��
				send_software_version_bdp();
				g_report_step++;
				show_text(DGUS_STATE_SHOW_ADDR,9,"����ϱ��汾��    ",strlen("����ϱ��汾��    "));
				break;
			case 3:		//�ϱ�CCID��IMSI
				send_CCID_and_IMSI_bdp();		
				g_report_step++;	
				show_text(DGUS_STATE_SHOW_ADDR,9,"����ϱ�CCID��IMSI    ",strlen("����ϱ�CCID��IMSI    "));
				break;
			case 4:		//�ϱ����������õ���
				g_report_step++;
				break;
			case 5:		//�ϱ�STM���к�
				send_STM32_CHIP_ID_bdp();
				show_text(DGUS_STATE_SHOW_ADDR,9,"����ϱ�STM���к�  ",strlen("����ϱ�STM���к�  "));
				g_report_step++;	
				break;
			case 6:		//�ϱ��������¶�
				send_concentrator_temperature_bdp();
				show_text(DGUS_STATE_SHOW_ADDR,9,"����ϱ�E3000�¶�  ",strlen("����ϱ�E3000�¶�  "));
				g_report_step++;
				break;
			case 7:		//�ϱ�·��״̬
				if(g_router_fault_flag == 0)
				{
					send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_1,0);//���·��ͨ�Ź���
					show_text(DGUS_STATE_SHOW_ADDR,9,"�ز�ͨ������  ",strlen("�ز�ͨ������  "));
				}
				else
				{
					send_concentrator_fault_bdp(CONCENTRATOR_FAULT_1,0);	//�ϱ���������·��ͨ�Ź���
					show_text(DGUS_STATE_SHOW_ADDR,9,"�ز�ͨ�Ź���  ",strlen("�ز�ͨ�Ź���  "));
				}
				g_report_step++;	
				break;
			case 8:		//�ϱ���չģ��״̬
				for(i = 0;i < 3;i++)
				{
					if(g_Concentrator_Para.ext_enable[i] != 0xFF)		//��չģ��δ����
					{
						if(g_ext_online_flag[i] == ONLINE)
						{
							send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_2,i+1);	//�����չģ��ͨ��ʧ��
						}
						else
						{
							send_concentrator_fault_bdp(CONCENTRATOR_FAULT_2,i+1);			//�ϱ���չģ��ͨ��ʧ��
						}
					}
				}				
				g_report_step++;
				show_text(DGUS_STATE_SHOW_ADDR,9,"�����չģ��״̬�ϱ� ",strlen("�����չģ��״̬�ϱ� "));
				break;
			case 9:		//�ϱ����緽ʽ
				if(g_Concentrator_Para.power_supply == AC_POWER_SUPPLY)
				{
					send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_4,0);	//�ϱ��������е繩��
				}
				else if(g_Concentrator_Para.power_supply == BATTERY_POWER_SUPPLY)
				{	
					send_concentrator_fault_bdp(CONCENTRATOR_FAULT_4,0);	//�ϱ���������ع���
				}
				g_report_step++;
				show_text(DGUS_STATE_SHOW_ADDR,9,"��ɹ��緽ʽ�ϱ� ",strlen("��ɹ��緽ʽ�ϱ� "));
				break;
			case 10:												
				send_local_time_bdp(0);					//�ϱ�����ʱ��
				g_report_step++;
				show_text(DGUS_STATE_SHOW_ADDR,9,"��ɱ���ʱ���ϱ� ",strlen("��ɱ���ʱ���ϱ� "));
				break;
			case 11:			
				send_local_working_time_bdp();		//�ϱ�����ʱ��			 
                g_report_step++;
				show_text(DGUS_STATE_SHOW_ADDR,9,"��ɹ���ʱ���ϱ� ",strlen("��ɹ���ʱ���ϱ� "));
				break;
             case 12:	
				g_report_step++;				 
				break;
			case 13:
				send_singnal_quality_bdp();
				show_text(DGUS_STATE_SHOW_ADDR,9,"����ź�ǿ���ϱ� ",strlen("����ź�ǿ���ϱ� "));
				g_report_step++;
				break;
			case 14:
				//��ʼ������״̬
				for(i = 0 ; i < DAMP_MAX_NUM ; i++ )
				{
					if(g_Damp_Info[i].m_bit_status.status == 1)
					{
						g_Damp_Info[i].m_bit_status.send_para_bdp_flag = 0;
						g_Damp_Info[i].m_bit_status.send_online_bdp_flag = 0;
						g_Damp_Info[i].m_bit_status.send_status_bdp_flag = 0;		
					}
				}
				//�����ϱ����̽���
				g_report_state_flag = 0;
			    g_report_step = 0;	
				
				show_text(DGUS_STATE_SHOW_ADDR,9,"��ɳ�ʼ����Ϣ�ϱ� ",strlen("��ɳ�ʼ����Ϣ�ϱ� "));
				break;
			default:
				break;
		}
// 	//��ӡ�����ϱ�����
// 	print_len = data_conversion(print_buf,g_report_step,DECIMAL0);
// 	show_text(DGUS_PRINT_SHOW_ADDR,11,print_buf,print_len);
}
