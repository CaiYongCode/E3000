#include "ExtModule.h"
#include "BIT.h"
#include "struct.h"
#include "IOctl.h"
#include "dacai_touch.h"
#include "GPRS.h"
////////////////////////////////////////////////////////////
//��Ӧ�ⲿ��չģ��ظ汨�� 
unsigned char m_flag[3] = {1,1,1};

void ActOn_ExtModule_BDP(unsigned char * pbuf,unsigned char len)
{
	unsigned char m_addr = 0;
	unsigned char i = 0;
	m_addr = pbuf[1];
	if((pbuf[0]== 0x7a) && (pbuf[2]== 31) && (pbuf[3] == 0x11))
	{
		if((m_addr > 0) &&( m_addr < 4))
		{
			g_ext_offline_counter[m_addr - 1] = 0;
			m_flag[m_addr - 1] = ONLINE;
			for(i = 0; i < 12; i++)
			{
				g_Concentrator_Para.ext_current[m_addr - 1][i] = pbuf[4 + 2*i]*256 + pbuf[4 + 2*i + 1];
			}
// 			if(pbuf[28] != 0)	//��չģ������䶯���ϱ�����������
// 			{
// 			}
		}
	}
}


////////////////////////////////////////////////////////////////////
unsigned short m_send_counter = 0;
unsigned char m_send_ext_buf[16];
unsigned char m_send_rst_8302_counter = 0;
void Send_CMD_bdp_to_ExtModule()
{
	unsigned char m_val = 0;
	
	if(g_reset_ext_8302_flag == 1)
	{
		m_send_rst_8302_counter++;		
	}
	if(m_send_rst_8302_counter >= 4)
	{
		m_send_rst_8302_counter = 0;
		g_reset_ext_8302_flag = 0;
	}
	
	m_val = m_send_counter % 3;
    m_send_counter ++;
    m_send_ext_buf[0] = 0x7A;
	m_send_ext_buf[1] = m_val + 1;  //��ַ
	m_send_ext_buf[2] = 10;
	m_send_ext_buf[3] = 0x21;
	switch(m_val)
		{
		   case 0:
			   	m_send_ext_buf[4] = g_extmodule_relay[0][0];
			    m_send_ext_buf[5] = g_extmodule_relay[0][1];
				m_send_ext_buf[6] = g_extmodule_relay[0][2];
				m_send_ext_buf[7] = g_extmodule_relay[0][3];
				
		   	break;
		   case 1:
			   	m_send_ext_buf[4] = g_extmodule_relay[1][0];
			    m_send_ext_buf[5] = g_extmodule_relay[1][1];
				m_send_ext_buf[6] = g_extmodule_relay[1][2];
				m_send_ext_buf[7] = g_extmodule_relay[1][3];
		   	break;
           case 2:
			   	m_send_ext_buf[4] = g_extmodule_relay[2][0];
			    m_send_ext_buf[5] = g_extmodule_relay[2][1];
				m_send_ext_buf[6] = g_extmodule_relay[2][2];
				m_send_ext_buf[7] = g_extmodule_relay[2][3];
		   	break;
		   default:
		   	break;
		}
	m_send_ext_buf[8] = g_reset_ext_8302_flag;
	m_send_ext_buf[9] = 0xA7;
	COM3_Send_BDP(m_send_ext_buf,10);
}


void Ext_Module_online_judge()
{
	
	unsigned char i = 0;

	g_ext_offline_counter[0]++;
	g_ext_offline_counter[1]++;
	g_ext_offline_counter[2]++;
	//�����ж�
	if(g_ext_offline_counter[0] > 10)
	{
		g_ext_offline_counter[0] = 10;
		m_flag[0] = OFFLINE;
	}
	if(g_ext_offline_counter[1] > 10)
	{
		g_ext_offline_counter[1] = 10;
		m_flag[1] = OFFLINE;
	}
	if(g_ext_offline_counter[2] > 10)
	{
		g_ext_offline_counter[2] = 10;
		m_flag[2] = OFFLINE;
	}
	//��������״̬����ʾ��
	if(m_flag[0] != g_ext_online_flag[0]) 
	{
		show_ext_status(0,1-m_flag[0]);
		if(g_Concentrator_Para.ext_enable[0] != 0xFF)		//��չģ������
		{
			if(OFFLINE == m_flag[0])	//�ϱ���չģ��1ͨ��ʧ��
			{
				send_concentrator_fault_bdp(CONCENTRATOR_FAULT_2,1);
				for(i = 0;i < 12;i++)	//��չģ��1��������
				{
					g_Concentrator_Para.ext_current[0][i] = 0;
				}
			}
			else if(ONLINE == m_flag[0])	//�����չģ��1ͨ��ʧ��
			{
				send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_2,1);
			}
		}
		g_ext_online_flag[0] = m_flag[0];
		
	}
	if(m_flag[1] != g_ext_online_flag[1])
	{
		show_ext_status(1,1-m_flag[1]);
		if(g_Concentrator_Para.ext_enable[1] != 0xFF)		//��չģ������
		{
			if(OFFLINE == m_flag[1])	//�ϱ���չģ��2ͨ��ʧ��
			{
				send_concentrator_fault_bdp(CONCENTRATOR_FAULT_2,2);
				for(i = 0;i < 12;i++)	//��չģ��2��������
				{
					g_Concentrator_Para.ext_current[1][i] = 0;
				}
			}
			else if(ONLINE == m_flag[1])	//�����չģ��2ͨ��ʧ��
			{
				send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_2,2);			
			}
		}
		g_ext_online_flag[1] = m_flag[1];
	}
	if(m_flag[2] != g_ext_online_flag[2])
	{
		show_ext_status(2,1-m_flag[2]);
		if(g_Concentrator_Para.ext_enable[2] != 0xFF)		//��չģ������
		{
			if(OFFLINE == m_flag[2])	//�ϱ���չģ��3ͨ��ʧ��
			{
				send_concentrator_fault_bdp(CONCENTRATOR_FAULT_2,3);
				for(i = 0;i < 12;i++)	//��չģ��3��������
				{
					g_Concentrator_Para.ext_current[2][i] = 0;
				}
			}
			else if(ONLINE == m_flag[2])	//�����չģ��3ͨ��ʧ��
			{
				send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_2,3);
			}
		}
		g_ext_online_flag[2] = m_flag[2];
	}	
}
//��֤��չģ���Ƿ�����
void Check_ext_enable(unsigned char num)
{
	unsigned char i = 0;
	
	g_Concentrator_Para.ext_enable[num] = 0xFF;	//Ĭ��δ����
	for(i = 0;i < 4;i++)
	{
		if(g_Concentrator_Para.ext_relay_enable[num][i] != 0xFF)	//��1��·�̵�������
		{
			g_Concentrator_Para.ext_enable[num] = 0;	//��չģ������
			break;
		}
	}
	for(i = 0;i < 12;i++)
	{
		if(g_Concentrator_Para.ext_phase_enable[num][i] != 0xFF)	//��1��λ�����������
		{
			g_Concentrator_Para.ext_enable[num] = 0;	//��չģ������
			break;
		}
	}

}

//������չ״̬
void Update_Ext_Status(unsigned char num)
{
	
	if( g_Concentrator_Para.ext_enable[num] == 0xFF)	//��չģ��3δ����
	{				
		show_ext_status(num,2);
	}
	else	//��չģ��3����
	{				
		if(g_ext_online_flag[num] == ONLINE)
		{
			show_ext_status(num,0);	//��ʾ����
		}
		else if(g_ext_online_flag[num] == OFFLINE)
		{
			show_ext_status(num,1);	//��ʾ����
		}
	}
}
