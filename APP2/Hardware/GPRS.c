#include "GPRS.h"
#include "struct.h"
#include "IOctl.h"
#include "dacai_touch.h"
#include "PLC.h"
#include "globaldata.h"
#include "DS1302.h"
#include "TCP_Reconnect.h"
#include "Lamp.h"
#include "Update.h"
#include "ExtModule.h"
unsigned char m_gprs_send_buf[256];
//////////////////////////////////////////////////////////////////////////////////////////
volatile unsigned char m_send_GPRS_index = 0;
volatile unsigned char m_add_GPRS_index = 0;
volatile unsigned char m_GPRS_RingBuf_Num = 0;
///////////////////////////////////////////////////////////////////////////
//����PLC����   ���ڵ���   ���б��Ļ��浽FIFO�У�ÿ��6�����ڵ���
void Send_GPRS_Bdp(void)
{ 
   if(m_GPRS_RingBuf_Num > 0 )
   	{
   	    if(g_Send_GPRS_bdp_FIFO[m_send_GPRS_index].send_counter > 0)
   	    	{
   	    	 
   	    	OSIntEnter();
   	    	    //���ͱ���
   	    	    g_Send_GPRS_bdp_FIFO[m_send_GPRS_index].send_counter--;
				gprs_data_send(g_Send_GPRS_bdp_FIFO[m_send_GPRS_index].buf,g_Send_GPRS_bdp_FIFO[m_send_GPRS_index].len);
				//
			    
				
				if(g_Send_GPRS_bdp_FIFO[m_send_GPRS_index].send_counter == 0)  //�������ķ������
					{
					    memset((char *)g_Send_GPRS_bdp_FIFO[m_send_GPRS_index].buf,0,72);//��ջ�����
					    m_send_GPRS_index++;
						if(m_send_GPRS_index == MAX_GPRS_FIFO_DEP )
					   	{
					   	   m_send_GPRS_index = 0;
					   	}
						
						m_GPRS_RingBuf_Num -- ;
					}
				OSIntExit();
   	    	}
   	}
   else
   	{
   	   //���������ޱ��ķ���
   	}

}


unsigned char Add_bpd_to_GPRS_RingBuf(unsigned char send_counter, unsigned char * pbuf, unsigned char len)
{
     unsigned char m_ret  = 0;
     if(m_GPRS_RingBuf_Num == MAX_GPRS_FIFO_DEP)  //�洢�ռ��Ѿ�����
     	{
     	   m_ret = 0;
     	}
	 else
	 	{
	 	OSIntEnter();
	 	    //��ֵ
	 	   g_Send_GPRS_bdp_FIFO[m_add_GPRS_index].send_counter = send_counter;
		   g_Send_GPRS_bdp_FIFO[m_add_GPRS_index].len = len;
		   memcpy((char *)g_Send_GPRS_bdp_FIFO[m_add_GPRS_index].buf,pbuf,len);
	 	  
	 	   m_add_GPRS_index++;
		   if(m_add_GPRS_index == MAX_GPRS_FIFO_DEP )
		   	{
		   	   m_add_GPRS_index = 0;
		   	}
		   m_GPRS_RingBuf_Num++;
		   m_ret = 1;
		OSIntExit();
	 	}
	 

	 return m_ret;
}


////////////////////////////////////////////////////////////////////////////////////////////////

//ע����������ر��Ľ���
unsigned char Server_reg_bak_info(unsigned char* pbuf)
{
	unsigned char ret = 0xff;
	if(pbuf[0] == 0xA5)
	{
		ret = pbuf[5];
		switch(ret)
		{
			case 0x01:  //��֤ͨ��
				break;
			case 0x11:  //ID������
//				show_text(DGUS_STATE_SHOW_ADDR,9,"ID������  ",strlen("ID������  "));
				break;
			case 0x12:  // �ڲ�����
//				show_text(DGUS_STATE_SHOW_ADDR,9,"�ڲ�����  ",strlen("�ڲ�����  "));
				break;
			default:
				break;
		}
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////
//���������Ľ����봦��
////////////////////////////////////////////////////////////////////////////////////////////
//����GPRS�ı�����Ϣ

void ActOn_GPRS_BDP(unsigned char * pbuf,unsigned short len)
{
	unsigned char b_valid = 0;
	unsigned short data_len = 0;

	if(pbuf[0] == 0xA5)  //�Ƿ������������ı���
	{	
		//��ֹ����״̬GPRSģ������ն˷����ı��Ķ���������
		if((memcmp((char *)com1_send_buf,(char *)pbuf,com1_send_len) == 0) && (com1_send_len == len))  //���յ������ݺ��ϴη���ȥ������һ��������״̬GPRSģ������ն˷����ı���
		{
			return;
		}
// 		//���ճ����뱨�ĳ��Ȳ���
 		data_len = pbuf[1]*256+pbuf[2];
 		if(data_len != len)
		{
 			return;
 		}
		
		b_valid = bdp_valid(pbuf,len);
		
		if(b_valid == 1)
		{		
			g_GPRS_connect_OK = 1;  //�յ��������ı��� ֤��������
			Set_GPRS_LED_Status(OPEN);
			Set_GPRS_Status_to_TC(g_GPRS_signal_strength);  //��ʾGPRS����
			g_gprs_timeout_2000ms_counter = 0;  //�յ�GPRS���ģ��ѳ�ʱ����������			
			
			if((data_len == 5)&&(pbuf[3] == 0xFE))
			{
				//������������Ӧ���Ĳ�������ˮ��
//				g_recv_heart_num++;
// 				//��ӡ������������
// 				print_len = data_conversion(print_buf,g_recv_heart_num,DECIMAL0);
// 				show_text(DGUS_PRINT_SHOW_ADDR,7,print_buf,print_len);
			}
			else
			{
				g_gprs_msg_num = pbuf[3];
			}
			
		    switch(pbuf[4])  //��Ϣ����
			{
				case 0xCC:  //��Ӧ�����������������౨��
					act_on_cmd_bdp(pbuf,len);
					break;
				case 0x99:  //�ն��������� ����		
					ActOn_Update_BDP(pbuf,len);	
					break;
				default:
//					send_result_return_bdp(0xFD);	//����δʶ��
					break;
			}	
		}
		else
		{
			send_result_return_bdp(0xFC);	//����ͨ�ô���
		}
	}	
}

//������������Ч���ж�
unsigned char bdp_valid(unsigned char * pbuf,unsigned short len)
{
	unsigned char ret = 0,sum = 0;
	unsigned short i = 0;
	for(i = 0; i < len - 1; i++)
	{
		sum += pbuf[i];
	}
	if(sum == pbuf[len - 1])
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	return ret;
}



//��Ӧ�����������������౨��
void act_on_cmd_bdp(unsigned char* pbuf,unsigned short len)
{

	switch(pbuf[5])
	{
		case 0x01:	//���ƺ͵���
            cmd_01_message(pbuf,len);
			break;
		case 0x02:  //�ص�
		    cmd_02_message(pbuf,len);
			break;
		case 0x03:  //���ÿ��ص�ʱ��(��)
		    cmd_03_message(pbuf,len);
			break;
		case 0x04: 	//���ÿ���
		    cmd_04_message(pbuf,len);
			break;
		case 0x05:   //ʱ��ͬ������
		    cmd_05_message(pbuf,len);
			break;
		case 0x06:	//����
		    cmd_06_message(pbuf,len);
			break;
//		case 0x07:	//�·��¶ȹ���ʱ��
//		    cmd_07_message(pbuf,len);
//			break;
		case 0x08:	//���߹���ģʽ�趨
		    cmd_08_message(pbuf,len);
			break;
		case 0x09:	//�趨���Ʒ���
		    cmd_09_message(pbuf,len);
			break;
		case 0x0A:	//�·��¶ȹ���ʱ��8��
		    cmd_0A_message(pbuf,len);
			break;
		case 0x0B:	//��Ӵӽڵ�
		    cmd_0B_message(pbuf,len);
			break;
		case 0x0C:	//ɾ���ӽڵ�
		    cmd_0C_message(pbuf,len);
			break;
		case 0x0D:	//�ӽڵ��ʼ��
			break;
		case 0x0E:	//��ѯ�汾��
		    cmd_0E_message(pbuf,len);
			break;
		case 0x0F:	//���õ��ƻ�·˳��
		    cmd_0F_message(pbuf,len);
			break;
		case 0x10:	//�������ÿ��ƻ�·
			cmd_10_message(pbuf,len);
			break;
		case 0x11:	//�������òɼ���·
			cmd_11_message(pbuf,len);
			break;
		case 0x12:	//��������IP
			cmd_12_message(pbuf,len);
			break;
		case 0x13:	//������������
			cmd_13_message(pbuf,len);
			break;
		case 0x14:	//���ö˿ں�
			cmd_14_message(pbuf,len);
			break;
		case 0x15:	//����·��
			cmd_15_message(pbuf,len);
			break;
		case 0x16:	//����·��
			cmd_16_message(pbuf,len);
			break;
		case 0x17:	//���þ�γ��
			cmd_17_message(pbuf,len);
			break;
		case 0xFF:	//�����ϱ�
		    cmd_FF_message(pbuf,len);
			break;
		default:
			send_result_return_bdp(0xFD);
			break;		   
	}

}

//���ƺ͵���
void cmd_01_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short i = 0,j = 0;
	unsigned int addr = 0;
	unsigned char bcd_addr[6] = {0};
	unsigned char m_group = 0;
	unsigned short ctrl_group = 0;
	unsigned char addr_valid = 0;
	unsigned short m_swicth = 0;


	for(i = 10;i <= 13;i++)
	{
		addr = (addr << 8)|pbuf[i];
	}
	
	if(addr == 0x00000000)			//�Լ���������
	{
		m_swicth = pbuf[14]*256+pbuf[15];
		//�Լ�����1�ż̵���
		if( (m_swicth&0x01) == 1 )
		{
			if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//������1�ż̵�������
			{
				E3000_Relay_Ctl(OPEN);		//�򿪼������̵���
				g_Concentrator_Para.e3000_relay_enable[0] = 100;
			}
		}
		else if( (m_swicth&0x01) == 0 )
		{
			E3000_Relay_Ctl(CLOSE);		//�رռ������̵���
			if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//�������̵�������
			{				
				g_Concentrator_Para.e3000_relay_enable[0] = 0;
			}
		}
		//����չģ��̵���
		m_swicth = m_swicth>>1;
		for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
		{
			for(j = 0;j < 4;j++)
			{
				if(g_Concentrator_Para.ext_relay_enable[i][j] != 0xFF)	//��չģ��̵�������
				{
					g_extmodule_relay[i][j] = (m_swicth>>(i*4+j))&0x01;
					//�����չģ�鲻���ߣ����
					if(g_ext_online_flag[i] == OFFLINE)
					{
						g_extmodule_relay[i][j] = 0;
					}
					if(g_extmodule_relay[i][j] == 0)
					{
						g_Concentrator_Para.ext_relay_enable[i][j] = 0;
					}
					else
					{
						g_Concentrator_Para.ext_relay_enable[i][j] = 100;
					}
				}
				else
				{
					g_extmodule_relay[i][j] = 0;
				}
			}
			
		}
		
		g_send_concentrator_parameter = 1;  //20S���ϱ�����
		
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}	
	else if( addr == 0xFFFFFFFF) 	//�Ե���ȫ������
	{
		if(g_lamp_total_num == 0)	//�ӽڵ�Ϊ��
		{
			send_result_return_bdp(0x04);	//����ִ�н��
		}
		else
		{
			ctrl_group = 0xFFFF;
			for(i = 0;i < DAMP_MAX_NUM;i++)
			{
				if(g_Damp_Info[i].m_bit_status.status != 0)
				{
					g_Damp_Info[i].m_bit_status.switch_flag = 1;
				}
			}	
			send_plc_cmd_bdp(bcd_addr,ctrl_group,&pbuf[14]);
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
		}
	}
	else if( (addr >= 0xFFFFFF21)&&(addr <= 0xFFFFFF40) )	//�Ե��������
	{
		m_group = (addr&0xFF)-0x20;
		for(i = 0;i < DAMP_MAX_NUM;i++)
		{
			if(g_Damp_Info[i].m_bit_status.group == m_group)
			{
				g_Damp_Info[i].m_bit_status.switch_flag = 1;
				addr_valid = 1;
			}
		}
		if(addr_valid == 0)	//��𲻴���
		{
			send_result_return_bdp(0x03);	//����ִ�н��
		}
		else
		{
			ctrl_group = (1<<(m_group-1));
			send_plc_cmd_bdp(bcd_addr,ctrl_group,&pbuf[14]);
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
		}
	}
	else if( (addr >= 0x000001)&&(addr <= 0xFFFFFDFF) )	//�Ե��Ƶ��ڵ����
	{							
		for(i = 0;i < DAMP_MAX_NUM;i++)
		{
			if((g_Damp_Info[i].SN[0] == pbuf[10])&&(g_Damp_Info[i].SN[1] == pbuf[11])\
				&&(g_Damp_Info[i].SN[2] == pbuf[12])&&(g_Damp_Info[i].SN[3] == pbuf[13]))
			{	
				g_Damp_Info[i].m_bit_status.switch_flag = 1;
				addr_valid = 1;
				break;;
			}
		}
		if(addr_valid == 0)		//�ڵ��ַ��Ч
		{
			send_result_return_bdp(0x02);	//����ִ�н��
		}
		else
		{
			HEXaddr_to_BCDaddr(&pbuf[10],bcd_addr);
			send_plc_cmd_bdp(bcd_addr,ctrl_group,&pbuf[14]);
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
		}

		
	}
	else
	{
		send_result_return_bdp(0xFD);	//����δʶ��
	}

	g_send_concentrator_parameter = 1;  //10S���ϱ�����
	
	
}
//�ص�
void cmd_02_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short i = 0,j = 0;
	unsigned int addr = 0;
	unsigned char bcd_addr[6] = {0};
	unsigned char m_group = 0;
	unsigned short ctrl_group = 0;
	unsigned char addr_valid = 0;
	unsigned char contents[18] = {0};
	

	for(i = 10;i <= 13;i++)
	{
		addr = (addr << 8)|pbuf[i];
	}
	
	if(addr == 0x00000000)			//�Լ���������
	{
		for(i = 0;i < 3;i++)
		{
			for(j = 0;j < 4;j++)
			{
				g_extmodule_relay[i][j] = 0;
			}
		}
		E3000_Relay_Ctl(CLOSE);		//�رռ������̵���

		if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//�������̵�������
		{
			g_Concentrator_Para.e3000_relay_enable[0] = 0;
		}
		for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
		{
			for(j = 0;j < 4;j++)
			{
				if(g_Concentrator_Para.ext_relay_enable[i][j] != 0xFF)	//��չģ��̵�������
				{
					g_Concentrator_Para.ext_relay_enable[i][j] = 0;
				}
			}
			
		}
		
		g_send_concentrator_parameter = 1;  //20S���ϱ�����
		
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}	
	else if( addr == 0xFFFFFFFF) 	//�Ե���ȫ������
	{
		if(g_lamp_total_num == 0)	//�ӽڵ�Ϊ��
		{
			send_result_return_bdp(0x04);	//����ִ�н��
		}
		else
		{
			ctrl_group = 0xFFFF;
			for(i = 0;i < DAMP_MAX_NUM;i++)
			{
				if(g_Damp_Info[i].m_bit_status.status != 0)
				{
					g_Damp_Info[i].m_bit_status.switch_flag = 0;
				}
			}	
			send_plc_cmd_bdp(bcd_addr,ctrl_group,contents);
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
		}
	}
	else if( (addr >= 0xFFFFFF21)&&(addr <= 0xFFFFFF40) )	//�Ե��������
	{
		m_group = (addr&0xFF)-0x20;
		for(i = 0;i < DAMP_MAX_NUM;i++)
		{
			if(g_Damp_Info[i].m_bit_status.group == m_group)
			{
				g_Damp_Info[i].m_bit_status.switch_flag = 0;
				addr_valid = 1;
			}
		}
		if(addr_valid == 0)	//��𲻴���
		{
			send_result_return_bdp(0x03);	//����ִ�н��
		}
		else
		{
			ctrl_group = (1<<(m_group-1));
			send_plc_cmd_bdp(bcd_addr,ctrl_group,contents);
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
		}
	}
	else if( (addr >= 0x000001)&&(addr <= 0xFFFFFDFF) )	//�Ե��Ƶ��ڵ����
	{							
		for(i = 0;i < DAMP_MAX_NUM;i++)
		{
			if((g_Damp_Info[i].SN[0] == pbuf[10])&&(g_Damp_Info[i].SN[1] == pbuf[11])\
				&&(g_Damp_Info[i].SN[2] == pbuf[12])&&(g_Damp_Info[i].SN[3] == pbuf[13]))
			{	
				g_Damp_Info[i].m_bit_status.switch_flag = 0;
				addr_valid = 1;
				break;;
			}
		}
		if(addr_valid == 0)		//�ڵ��ַ��Ч
		{
			send_result_return_bdp(0x02);	//����ִ�н��
		}
		else
		{
			HEXaddr_to_BCDaddr(&pbuf[10],bcd_addr);
			send_plc_cmd_bdp(bcd_addr,ctrl_group,contents);
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
		}
	}
	else
	{
		send_result_return_bdp(0xFD);	//����δʶ��//��ַ��Ч
	}	
	
}
//�·��ܹ���ʱ��
void cmd_03_message(unsigned char* pbuf,unsigned short len)
{
    unsigned char i = 0;
	for(i = 0;i < 7;i++)
	{
		g_Week_Light_Time_Info[i].s_time[0]    = pbuf[14+i*96]*256+pbuf[15+i*96];
		g_Week_Light_Time_Info[i].e_time[0]    = pbuf[16+i*96]*256+pbuf[17+i*96];
		g_Week_Light_Time_Info[i].group[0]     = pbuf[18+i*96]*256+pbuf[19+i*96];
		g_Week_Light_Time_Info[i].loop[0]      = pbuf[20+i*96]*256+pbuf[21+i*96];
		g_Week_Light_Time_Info[i].ratio[0][2]  = pbuf[35+i*96];
		g_Week_Light_Time_Info[i].ratio[0][1]  = pbuf[36+i*96];
		g_Week_Light_Time_Info[i].ratio[0][0]  = pbuf[37+i*96];		
		
		g_Week_Light_Time_Info[i].s_time[1]    = pbuf[38+i*96]*256+pbuf[39+i*96];
		g_Week_Light_Time_Info[i].e_time[1]    = pbuf[40+i*96]*256+pbuf[41+i*96];
		g_Week_Light_Time_Info[i].group[1]     = pbuf[42+i*96]*256+pbuf[43+i*96];
		g_Week_Light_Time_Info[i].loop[1]      = pbuf[44+i*96]*256+pbuf[45+i*96];
		g_Week_Light_Time_Info[i].ratio[1][2]  = pbuf[59+i*96];
		g_Week_Light_Time_Info[i].ratio[1][1]  = pbuf[60+i*96];
		g_Week_Light_Time_Info[i].ratio[1][0]  = pbuf[61+i*96];
		
		g_Week_Light_Time_Info[i].s_time[2]    = pbuf[62+i*96]*256+pbuf[63+i*96];
		g_Week_Light_Time_Info[i].e_time[2]    = pbuf[64+i*96]*256+pbuf[65+i*96];
		g_Week_Light_Time_Info[i].group[2]     = pbuf[66+i*96]*256+pbuf[67+i*96];
		g_Week_Light_Time_Info[i].loop[2]      = pbuf[68+i*96]*256+pbuf[69+i*96];
		g_Week_Light_Time_Info[i].ratio[2][2]  = pbuf[83+i*96];
		g_Week_Light_Time_Info[i].ratio[2][1]  = pbuf[84+i*96];
		g_Week_Light_Time_Info[i].ratio[2][0]  = pbuf[85+i*96];
		
		g_Week_Light_Time_Info[i].s_time[3]    = pbuf[86+i*96]*256+pbuf[87+i*96];
		g_Week_Light_Time_Info[i].e_time[3]    = pbuf[88+i*96]*256+pbuf[89+i*96];
		g_Week_Light_Time_Info[i].group[3]     = pbuf[90+i*96]*256+pbuf[91+i*96];
		g_Week_Light_Time_Info[i].loop[3]      = pbuf[92+i*96]*256+pbuf[93+i*96];
		g_Week_Light_Time_Info[i].ratio[3][2]  = pbuf[107+i*96];
		g_Week_Light_Time_Info[i].ratio[3][1]  = pbuf[108+i*96];
		g_Week_Light_Time_Info[i].ratio[3][0]  = pbuf[109+i*96];
	}

	Write_Week_Light_Time_Info();
	
	send_result_return_bdp(0x01);	//ִ�гɹ�

}
//���ÿ���
void cmd_04_message(unsigned char* pbuf,unsigned short len)
{
	unsigned char num[13] = {0};
	unsigned char i,j;
    j = 1;
	num[0] = pbuf[14]%16;
	for(i = 1;i < 13;)
	{
		num[i] = pbuf[14+j]/16;
		num[i+1] = pbuf[14+j]%16;
		i = i+2;
		j++;
	}
	
	for(i = 1;i < 13;i++)	//����ÿһλ����Ϊ0~9
	{
		if(num[i] > 9)
		{
			send_result_return_bdp(0x08);	//���ݸ�ʽ����
			return;
		}
	}
	
	if(num[0] == 1)		//��λ����Ϊ1		
	{	
		for(i = 0;i < 13;i++)	
		{
			g_E3000_Info.simcard_num[i] = num[i];
		}			
		Write_RTU_info_to_Flash();
		show_simcard_num();
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}
	else
	{
		send_result_return_bdp(0x08);	//���ݸ�ʽ����
	}
	
}
//�·���ǰʱ��
unsigned char m_time_fault = 0;
void cmd_05_message(unsigned char* pbuf,unsigned short len)
{
	if((pbuf[16] <= 12)&&(pbuf[17] <= 31)&&(pbuf[18] <= 24)&&(pbuf[19] <= 60)&&(pbuf[20] <= 60))
	{
		g_set_year = pbuf[15];
		g_set_month = pbuf[16];
		g_set_day = pbuf[17];
		g_set_hour = pbuf[18];
		g_set_min  =  pbuf[19];
		g_set_sec  = pbuf[20];
		
		Set_TS_Time();					//����ϵͳʱ�䵽������
		Set_RTC_Time();					//����ϵͳʱ�䵽DS1302
		
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
		
		//�������һ��ͬ��ϵͳʱ�䣬���жϹ���
		if(g_judge_time_fault_flag == 1)
		{
			//�жϵ�ǰʱ���ͬ��ʱ���������30���ӣ��򱨾�
			if( (abs((g_set_min + g_set_hour*60) - (g_now_min + g_now_hour*60)) >= 30) || ( g_set_month != g_now_month ) || (g_set_day != g_now_day) )
			{
				if(m_time_fault == 0)
				{
					send_local_time_bdp(1);	
					m_time_fault = 1;
				}
			}
		}
		if(m_time_fault == 0)
		{
			send_local_time_bdp(0);	
		}
		
		g_judge_time_fault_flag = 1;
	}
	else
	{
		send_result_return_bdp(0x08);	//���ݸ�ʽ����
	}
}
//����
void cmd_06_message(unsigned char* pbuf,unsigned short len)
{
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	
	Close_Connect();
	Reset_GPRS_Module();
    NVIC_SystemReset();	 //ϵͳ��λ
}
// //�·��¶ȹ���ʱ��(4��)
// void cmd_07_message(unsigned char* pbuf,unsigned short len)
// {

// }
//���߹���ģʽ�趨
void cmd_08_message(unsigned char* pbuf,unsigned short len)
{
	if( (pbuf[14] <= 3)&&(pbuf[14] != 0) )
	{
		g_working_mode = pbuf[14];

		Write_RTU_info_to_Flash();
		
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}
	else
	{
		send_result_return_bdp(0x08);	//���ݸ�ʽ����
	}
}
//�趨���Ʒ���
void cmd_09_message(unsigned char *pbuf,unsigned short len)
{
    unsigned short i = 0;
	unsigned char addr[6] = {0};

	for(i = 0;i < DAMP_MAX_NUM;i++)
	{
		if((g_Damp_Info[i].SN[0] == pbuf[10])&&(g_Damp_Info[i].SN[1] == pbuf[11])\
			&&(g_Damp_Info[i].SN[2] == pbuf[12])&&(g_Damp_Info[i].SN[3] == pbuf[13]))
		{	
			g_Damp_Info[i].m_bit_status.group = pbuf[14];
			HEXaddr_to_BCDaddr(&pbuf[10],addr);
			send_set_damp_group_bdp(addr,pbuf[14]);
			Write_Damp_Info();
			
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
			return;;
		}
	}
	
	send_result_return_bdp(0x02);	//�ظ��ڵ㲻����
}
//�·��¶ȹ���ʱ��(8��)
void cmd_0A_message(unsigned char* pbuf,unsigned short len)
{
	unsigned char  month = 0,days = 0;
	unsigned char i = 0,j = 0;
	
	month = pbuf[14];
	days = pbuf[15];
	//����
	for(i = 0;i < days;i++)
	{
		//8��
		for(j = 0;j < 8;j++)
		{			
			g_Month_Light_Time_Info[i].s_time[j]    = pbuf[16+j*24+i*192]*256+pbuf[17+j*24+i*192];
			g_Month_Light_Time_Info[i].e_time[j]    = pbuf[18+j*24+i*192]*256+pbuf[19+j*24+i*192];
			g_Month_Light_Time_Info[i].group[j]     = pbuf[20+j*24+i*192]*256+pbuf[21+j*24+i*192];
			g_Month_Light_Time_Info[i].loop[j]      = pbuf[22+j*24+i*192]*256+pbuf[23+j*24+i*192];
			g_Month_Light_Time_Info[i].ratio[j][2]  = pbuf[37+j*24+i*192];
			g_Month_Light_Time_Info[i].ratio[j][1]  = pbuf[38+j*24+i*192];
			g_Month_Light_Time_Info[i].ratio[j][0]  = pbuf[39+j*24+i*192];
		}
	}
	Write_Month_Light_Time_Info(month,days);
	
	
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	
}
//��Ӵӽڵ�
void cmd_0B_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short i = 0;
	unsigned short m_damp_num = 0;
	unsigned char addr[6] = {0};

    m_damp_num = pbuf[14];
	for(i = 0;i < m_damp_num;i++)
	{	
		g_add_damp_addr[0] = pbuf[15+4*i];
		g_add_damp_addr[1] = pbuf[15+4*i+1];
		g_add_damp_addr[2] = pbuf[15+4*i+2];
		g_add_damp_addr[3] = pbuf[15+4*i+3];
		
		HEXaddr_to_BCDaddr(g_add_damp_addr,addr);
		
		send_add_damp_bdp(addr);
	}
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	
}
//ɾ���ӽڵ�
void cmd_0C_message(unsigned char* pbuf,unsigned short len)
{
   	unsigned short i = 0;
	unsigned short m_damp_num = 0;
	unsigned char addr[6] = {0};

    m_damp_num = pbuf[14];
	for(i = 0;i < m_damp_num;i++)
	{
		g_del_damp_addr[0] = pbuf[15+4*i];
		g_del_damp_addr[1] = pbuf[15+4*i+1];
		g_del_damp_addr[2] = pbuf[15+4*i+2];
		g_del_damp_addr[3] = pbuf[15+4*i+3];
		
		HEXaddr_to_BCDaddr(g_del_damp_addr,addr);
		
		send_del_damp_bdp(addr);
	}
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	
}
//�ӽڵ��ʼ��
void cmd_0D_message(unsigned char* pbuf,unsigned short len)
{
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
}
//��ѯ�汾��
void cmd_0E_message(unsigned char* pbuf,unsigned short len)
{
	send_software_version_bdp();
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
}

//���õ��ƻ�·˳��
void cmd_0F_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short i = 0;
	unsigned char addr[6] = {0};
	unsigned char contents[9] = {0};

	for(i = 0;i < DAMP_MAX_NUM;i++)
	{
		if(((g_Damp_Info[i].SN[0] == pbuf[10])&&(g_Damp_Info[i].SN[1] == pbuf[11])\
			&&(g_Damp_Info[i].SN[2] == pbuf[12])&&(g_Damp_Info[i].SN[3] == pbuf[13])))
		{	
			HEXaddr_to_BCDaddr(&pbuf[10],addr);
			memcpy(contents,addr,6);
			memcpy(&contents[6],&pbuf[14],3);
			
			send_lamp_loop_order_bdp(contents);
					
			send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�

			return;;
		}
	}
}	

//�������ÿ��ƻ�·
void cmd_10_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short loop_enable = 0;
	unsigned short i = 0,j = 0;
	unsigned int addr = 0;
	
	loop_enable = pbuf[14]*256+pbuf[15];
	
	for(i = 10;i <= 13;i++)
	{
		addr = (addr << 8)|pbuf[i];
	}
	
	if(addr == 0x00000000)			//�Լ���������
	{
		//�Լ��������ƻ�·
		if( (loop_enable&0x01) == 1 )
		{
			g_Concentrator_Para.e3000_relay_enable[0] = 0;	//������1�ż̵�������
			
		}
		else
		{
			g_Concentrator_Para.e3000_relay_enable[0] = 0xFF;	//������1�ż̵�������
		}
		//����չ���ƻ�·		
		for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
		{
			for(j = 0;j < 4;j++)
			{
				loop_enable = (loop_enable>>1);
				if((loop_enable&0x01) == 1)
				{
					g_Concentrator_Para.ext_relay_enable[i][j] = 0;	//��չģ��̵�������
				}
				else
				{
					g_Concentrator_Para.ext_relay_enable[i][j] = 0xFF;	//��չģ��̵�������
				}
			}
		}
		
		g_Concentrator_Para.e3000_config_flag = 1;
		
		for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
		{
			//��֤��չ����
			Check_ext_enable(i);
			//��ʾ��չģ��״̬
			Update_Ext_Status(i);
		}
		//����������Ϣ
		
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}
				
}

//�������òɼ���·
void cmd_11_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short i = 0,j = 0,m = 0;
	unsigned int addr = 0;
	unsigned char loop_enable[12] = {0};

	for(i = 10;i <= 13;i++)
	{
		addr = (addr << 8)|pbuf[i];
	}
	
	if(addr == 0x00000000)			//�Լ���������
	{
		loop_enable[0] = (pbuf[21]>>4);
		loop_enable[1] = (pbuf[20]&0xFF);
		loop_enable[2] = (pbuf[20]>>4);
		loop_enable[3] = (pbuf[19]&0xFF);
		loop_enable[4] = (pbuf[19]>>4);
		loop_enable[5] = (pbuf[18]&0xFF);
		loop_enable[6] = (pbuf[18]>>4);
		loop_enable[7] = (pbuf[17]&0xFF);
		loop_enable[8] = (pbuf[17]>>4);
		loop_enable[9] = (pbuf[16]&0xFF);
		loop_enable[10] = (pbuf[16]>>4);
		loop_enable[11] = (pbuf[15]&0xFF);
		
		//�Լ������ɼ���·
		for(i = 0;i < 3;i++)
		{
			if( ( (pbuf[21]>>i)&0x01 ) == 1 )
			{
				g_Concentrator_Para.e3000_A_enable[i] = 0;	//�����������������	
				g_Concentrator_Para.e3000_V_enable[i] = 0;	//��������ѹ�������	
			}
			else
			{
				g_Concentrator_Para.e3000_A_enable[i] = 0xFF;	//����������������
				g_Concentrator_Para.e3000_V_enable[i] = 0xFF;	//��������ѹ�������	
			}
		}
		
		//����չ���ƻ�·	
		for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
		{
			for(j = 0;j < 4;j++)
			{
				for(m = 0;m < 3;m++)
				{
					if( ( (loop_enable[i*4+j]>>m)&0x01 ) == 1 )
					{ 
						g_Concentrator_Para.ext_phase_enable[i][j*3+m] = 0;	//��չģ��ɼ���·����
					}
					else
					{
						g_Concentrator_Para.ext_phase_enable[i][j*3+m] = 0xFF;	//��չģ��ɼ���·����
					}
				}
			}
		}
		g_Concentrator_Para.e3000_config_flag = 1;

		for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
		{
			//��֤��չ����
			Check_ext_enable(i);
			//������չģ��״̬
			Update_Ext_Status(i);
		}
		//����������Ϣ
		Write_Concentrator_Config_Info();
		
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}
}

//��������IP
void cmd_12_message(unsigned char* pbuf,unsigned short len)
{
	unsigned char lengh1 = 0,lengh2 = 0,lengh3 = 0,lengh4 = 0;
	
	lengh1 = data_conversion(g_E3000_Info.ip,pbuf[14],DECIMAL0);
	g_E3000_Info.ip[lengh1] = 0x2E;
	lengh2 = data_conversion(&g_E3000_Info.ip[lengh1+1],pbuf[15],DECIMAL0);
	g_E3000_Info.ip[lengh1+lengh2+1] = 0x2E;
	lengh3 = data_conversion(&g_E3000_Info.ip[lengh1+lengh2+2],pbuf[16],DECIMAL0);
	g_E3000_Info.ip[lengh1+lengh2+lengh3+2] = 0x2E;
	lengh4 = data_conversion(&g_E3000_Info.ip[lengh1+lengh2+lengh3+3],pbuf[17],DECIMAL0);
	
	g_E3000_Info.ip_len = lengh1+lengh2+lengh3+lengh4+3;
	
	g_E3000_Info.ip_set = 1;
	
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
}

//������������
void cmd_13_message(unsigned char* pbuf,unsigned short len)
{
	g_E3000_Info.ip_len = len-15;
	memcpy(g_E3000_Info.ip,&pbuf[14],g_E3000_Info.ip_len);
	g_E3000_Info.ip_set = 1;
	
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
}

//���ö˿ں�
void cmd_14_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short port = 0;
	
	port = pbuf[14]*256+pbuf[15];
	g_E3000_Info.port_len = data_conversion(g_E3000_Info.port,port,DECIMAL0);
	g_E3000_Info.port_set = 1;
	
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
}

//����·��
void cmd_15_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short i = 0,j = 0;
	unsigned int addr = 0;

	for(i = 10;i <= 13;i++)
	{
		addr = (addr << 8)|pbuf[i];
	}
	
	if(addr == 0x00000000)			//�Լ���������
	{
		if(pbuf[14] == 0x01)
		{
			if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//�������̵�������
			{
				E3000_Relay_Ctl(OPEN);		//�򿪼������̵���
				g_Concentrator_Para.e3000_relay_enable[0] = 100;
			}
		}
		else if(pbuf[14] <= 13)
		{
			i = (pbuf[14]-2)/4;
			j = (pbuf[14]-2)%4;
			if(g_Concentrator_Para.ext_relay_enable[i][j] != 0xFF)	//��չģ��̵�������
			{
				if(g_ext_online_flag[i] == ONLINE)	//��չģ������
				{
					g_extmodule_relay[i][j] = 1;
					g_Concentrator_Para.ext_relay_enable[i][j] = 100;
				}
			}
		}
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}
}

//����·��
void cmd_16_message(unsigned char* pbuf,unsigned short len)
{
	unsigned short i = 0,j = 0;
	unsigned int addr = 0;

	for(i = 10;i <= 13;i++)
	{
		addr = (addr << 8)|pbuf[i];
	}
	
	if(addr == 0x00000000)			//�Լ���������
	{
		if(pbuf[14] == 0x01)
		{
			E3000_Relay_Ctl(CLOSE);		//�򿪼������̵���
			if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//�������̵�������
			{				
				g_Concentrator_Para.e3000_relay_enable[0] = 0;
			}
		}
		else if(pbuf[14] <= 13)
		{
			i = (pbuf[14]-2)/4;
			j = (pbuf[14]-2)%4;
			
			g_extmodule_relay[i][j] = 0;
			if(g_Concentrator_Para.ext_relay_enable[i][j] != 0xFF)	//��չģ��̵�������
			{									
				g_Concentrator_Para.ext_relay_enable[i][j] = 0;
			}
		}
		send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
	}
}

//���þ�γ��
void cmd_17_message(unsigned char* pbuf,unsigned short len)
{
	g_E3000_Info.longitude = ( (pbuf[14]<<24)|(pbuf[15]<<16)|(pbuf[16]<<8)|pbuf[17] );
	g_E3000_Info.latitude = ( (pbuf[18]<<24)|(pbuf[19]<<16)|(pbuf[20]<<8)|pbuf[21] );
	
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
}

//�ϱ�����
void cmd_FF_message(unsigned char* pbuf,unsigned short len)
{
	g_report_state_flag = 1;		
	
	send_result_return_bdp(0x01);	//�ظ�������ִ�гɹ�
}
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////

void assemble_gprs_send_bdp(unsigned char message_type, unsigned char * databuf,unsigned char datalen)
{
	unsigned short bdp_len = 0;
	bdp_len = 1 + 2 + 1 + 1 + datalen + 1; //����ͷ + ���ĳ��� +����Ϣ��ˮ + ��Ϣ���� + ��Ϣ���� + У���
	
	m_gprs_send_buf[0] = 0xA5;
	m_gprs_send_buf[1] = bdp_len / 256;
	m_gprs_send_buf[2] = bdp_len % 256;
	g_gprs_msg_num++;
	m_gprs_send_buf[3] = g_gprs_msg_num;
	m_gprs_send_buf[4] = message_type;
	memcpy((char * )&m_gprs_send_buf[5],(char *)databuf,datalen);
	m_gprs_send_buf[bdp_len - 1] = checksum(m_gprs_send_buf,bdp_len - 1);
//	gprs_data_send(m_gprs_send_buf,bdp_len);
    Add_bpd_to_GPRS_RingBuf(1,m_gprs_send_buf,bdp_len);
	
}
//����ע�����������
void send_registered(void)
{
	unsigned char buf[4];
	unsigned char data_len = 0;
    
    buf[0] = a_to_i(g_E3000_Info.SN[0])*16 + a_to_i(g_E3000_Info.SN[1]);
 	buf[1] = a_to_i(g_E3000_Info.SN[2])*16 + a_to_i(g_E3000_Info.SN[3]);
 	buf[2] = a_to_i(g_E3000_Info.SN[4])*16 + a_to_i(g_E3000_Info.SN[5]);
 	buf[3] = a_to_i(g_E3000_Info.SN[6])*16 + a_to_i(g_E3000_Info.SN[7]);
	data_len = 4;
	assemble_gprs_send_bdp(0xAA,buf,data_len);  
	
}


//����������
//���ⱨ��
void send_gprs_heart_bdp(void)
{
	unsigned char buf[5]; 
	
	buf[0] = 0xA5;
	buf[1] = 0x00;
	buf[2] = 0x05;
	buf[3] = 0xFF;
	buf[4] = checksum(buf,4);
	
//	gprs_data_send(buf,5);	
	Add_bpd_to_GPRS_RingBuf(1,buf,5);
}

//����������ִ�н���ظ�
void send_result_return_bdp(unsigned char result)
{
	unsigned char buf[8] = {0};
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		buf[0] = 0xA5;
		buf[1] = 0x00;
		buf[2] = 0x07;
		buf[3] = g_gprs_msg_num;
		buf[4] = 0xEE;
		buf[5] = result;
		buf[6] = checksum(buf,6);

		gprs_data_send(buf,7);
//		Add_bpd_to_GPRS_RingBuf(1,buf,7);
	}
}

///////////////////////////////////////////////////////////////////////////
//�����ϴ�
///////////////////////////////////////////////////////////////////////////
//�ϱ�����������

void send_concentrator_parameter_bdp(void)
{
	unsigned char i = 0,j = 0,m = 0;
	unsigned char buf[128] = {0};
	unsigned char bits = 0;			//
	unsigned short control_loop_enable = 0;	//���û�·
	unsigned short detect_loop_enable = 0;	//���û�·
	unsigned char m_ext_use_index = 0;  //�ⲿ��չģ��ɼ���·�ܸ���
	unsigned char data_len = 0;
	unsigned char m_index = 0;
	unsigned char e3000_detect_enable = 0xFF;
	unsigned char ext_detect_enable[3][4] = {0};
	
	if(g_GPRS_connect_OK == 0)	//���߲�����
	{
		return;
	}
	//ͳ�����õĿ��ƻ�·
	if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//���������ƻ�·����
	{
		control_loop_enable = control_loop_enable|0x01;
	}
	for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
	{
		for(j = 0;j < 4;j++)
		{
			if(g_Concentrator_Para.ext_relay_enable[i][j] != 0xFF)	//��չ���ƻ�·����
			{
				bits = 1+i*4+j;
				control_loop_enable = control_loop_enable | (0x0001<<bits);
			}
		}
	}
	//ͳ�����õĲɼ���·
	for(i = 0;i < 3;i++)
	{
		if(g_Concentrator_Para.e3000_A_enable[i] != 0xFF)	//�������ɼ���·����
		{
			detect_loop_enable = detect_loop_enable|0x01;
		}
	}
	for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
	{
		for(j = 0;j < 4;j++)
		{
			for(m = 0;m < 3;m++)
			{
				if(g_Concentrator_Para.ext_phase_enable[i][j*3+m] != 0xFF)	//��չģ��ɼ���·����
				{
					ext_detect_enable[i][j] = 1;
					bits = 1+i*4+j;
					detect_loop_enable = detect_loop_enable | (0x0001<<bits);
					break;
				}
			}
		}
	}
	
	for(i = 0;i < 3;i++)	
	{
		if(g_Concentrator_Para.e3000_V_enable[i] == 0xFF)		//��������ѹ������
		{
			g_Concentrator_Para.Voltage[i] = 0xFFFF;
		}
		if(g_Concentrator_Para.e3000_A_enable[i] == 0xFF)		//����������������
		{
			g_Concentrator_Para.e3000_current[i] = 0xFFFF;
		}	
		else
		{
			e3000_detect_enable = 0;
		}
	}
	for(i = 0;i < MAX_EXT_MODULE_NUM;i++) //��չģ����λ����
	{
		for(j = 0;j < 12;j++)
		{
			if(0xFF == g_Concentrator_Para.ext_phase_enable[i][j])	
			{
				g_Concentrator_Para.ext_current[i][j] = 0xFFFF;
			}
		}
	}
	//ƴװ������
	
	buf[0] = 0x0A;  //��������

	buf[1] = g_Concentrator_Para.Voltage[0]/256;
	buf[2] = g_Concentrator_Para.Voltage[0]%256;
	buf[3] = g_Concentrator_Para.Voltage[1]/256;
	buf[4] = g_Concentrator_Para.Voltage[1]%256;
	buf[5] = g_Concentrator_Para.Voltage[2]/256;
	buf[6] = g_Concentrator_Para.Voltage[2]%256;
	
	buf[7] = control_loop_enable/256;
	buf[8] = control_loop_enable%256;
	buf[9] = detect_loop_enable/256;
	buf[10] = detect_loop_enable%256;
	m_index = 11;
	//�ϱ����õĿ��ƻ�·����ֵ
	if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)//E3000���ƻ�·����	
	{
		if(g_Concentrator_Para.e3000_relay_enable[0] == 0)
		{
			buf[m_index] = 0xFF;		//��
		}
		else
		{
			buf[m_index] = 100;		//����ֵ
		}
		m_index++;		
	}		
	for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
	{
		for(j = 0;j < 4;j++)
		{
			if(g_Concentrator_Para.ext_relay_enable[i][j] != 0xFF)	//��չ���ƻ�·����			
			{
				if(g_Concentrator_Para.ext_relay_enable[i][j] == 0)
				{
					buf[m_index] = 0xFF;		//��
				}
				else
				{
					buf[m_index] = 100;		//����ֵ
				}
				m_index++;
			}
		}
	}
	//�ϱ����õĲɼ���·����ֵ
	if(e3000_detect_enable != 0xFF) //E3000 �Դ�����
	{		
		buf[m_index++] = g_Concentrator_Para.e3000_current[0]/256;
		buf[m_index++] = g_Concentrator_Para.e3000_current[0]%256;
		buf[m_index++] = g_Concentrator_Para.e3000_current[1]/256;
		buf[m_index++] = g_Concentrator_Para.e3000_current[1]%256;
		buf[m_index++] = g_Concentrator_Para.e3000_current[2]/256;
		buf[m_index++] = g_Concentrator_Para.e3000_current[2]%256;
	}
	
	for(i = 0;i < MAX_EXT_MODULE_NUM;i++)
	{
		for(j = 0;j < 4;j++)
		{
			if(ext_detect_enable[i][j] == 1)	//��չ��·�������			
			{				
				buf[m_index + 6 * m_ext_use_index + 0] = g_Concentrator_Para.ext_current[i][0+3*j]/256;
				buf[m_index + 6 * m_ext_use_index + 1] = g_Concentrator_Para.ext_current[i][0+3*j]%256;
				buf[m_index + 6 * m_ext_use_index + 2] = g_Concentrator_Para.ext_current[i][1+3*j]/256;
				buf[m_index + 6 * m_ext_use_index + 3] = g_Concentrator_Para.ext_current[i][1+3*j]%256;
				buf[m_index + 6 * m_ext_use_index + 4] = g_Concentrator_Para.ext_current[i][2+3*j]/256;
				buf[m_index + 6 * m_ext_use_index + 5] = g_Concentrator_Para.ext_current[i][2+3*j]%256;
				m_ext_use_index++;
			}
		}
	}
	data_len = m_index + 6 * m_ext_use_index;
	assemble_gprs_send_bdp(0xD1,buf,data_len);

}
//�ϱ��汾��
void send_software_version_bdp(void)
{
	unsigned char buf[4] = {0};
	unsigned char data_len = 0;

	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{

		buf[0] = 0x02;  //��������
		buf[1] = (SOFTWARE_VERSION>>16)&0xFF;
		buf[2] = (SOFTWARE_VERSION>>8)&0xFF;
		buf[3] = SOFTWARE_VERSION&0xFF;
        data_len = 4;
        assemble_gprs_send_bdp(0xD1,buf,data_len);
	}
	
	
}
//�ϴ�CCID��IMSI
void send_CCID_and_IMSI_bdp(void)
{
	unsigned char i = 0;
	unsigned char buf[18] = {0};
	unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		buf[0] = 0x03;  //��������
		for(i = 0;i < 18;i++)
		{
			buf[1+i] = ICCID_and_IMSI_buf[i];
		}
		data_len = 19;
		assemble_gprs_send_bdp(0xD1,buf,data_len);	
	}
	

}
// //�ϴ������ڵ����
// void send_single_damp_info_bdp(unsigned char index)
// {
// 	
// }
//�ϱ����������õ���
// void send_concentrator_Electricity_bdp(unsigned char *pbuf)
// {
// 	
// }

//�ϱ�STM32оƬID
void send_STM32_CHIP_ID_bdp(void)
{
	unsigned char i = 0;
	unsigned char buf[24] = {0};
	unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x06;  //��������
		
		for(i = 0;i < 12;i++)
		{
			buf[1+2*i] = i_to_a(STM32_CHIP_ID[i]&0x0F);
			buf[2+2*i] = i_to_a((STM32_CHIP_ID[i]>>4)&0x0F);
		}
		
		data_len = 25;
		assemble_gprs_send_bdp(0xD1,buf,data_len);	
	}
	

}
//�ϱ��������¶�
void send_concentrator_temperature_bdp(void)
{		
	unsigned char buf[3] = {0};
	unsigned char data_len = 0;

	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x07;  //��������
		
		buf[1] = g_LM75A_temp/256;
		buf[2] = g_LM75A_temp%256;
			
		data_len = 3;
		assemble_gprs_send_bdp(0xD1,buf,data_len);	
	}	
}

//�ϴ������ڵ����(��������)
void send_single_lamp_para_bdp(unsigned char index)
{
	unsigned char i = 0,j = 0;
	unsigned char buf[40] = {0};
	unsigned char loop[3] = {0};
	unsigned char data_len = 0;
	
	if(g_Damp_Info[index].loop == 0)	//�����û�·���ϴ�
	{
		return;
	}
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
        //ƴװ����������
		buf[0] = 0x08;  //��������
		buf[1] = g_Damp_Info[index].SN[0];
		buf[2] = g_Damp_Info[index].SN[1];
		buf[3] = g_Damp_Info[index].SN[2];
		buf[4] = g_Damp_Info[index].SN[3];
		buf[5] = g_Damp_Info[index].m_bit_status.group;
		buf[6] = g_Damp_Info[index].m_bit_status.phase;
		buf[7] = g_Damp_Info[index].voltage/256;
		buf[8] = g_Damp_Info[index].voltage%256;
		buf[9] = (g_Damp_Info[index].loop & 0x07)/256;  //ȷ��ֻ����·
		buf[10] = (g_Damp_Info[index].loop & 0x07)%256;
		
		for(i = 0;i < 3;i++)
		{
		    loop[i] = (g_Damp_Info[index].loop>>i)&0x01;
			if(loop[i] == 1)
			{
				buf[11 + 7*j] = g_Damp_Info[index].ratio[i];
				buf[12 + 7*j] = g_Damp_Info[index].current[i]/256;
				buf[13 + 7*j] = g_Damp_Info[index].current[i]%256;
				buf[14 + 7*j] = g_Damp_Info[index].active_power[i]/256;
				buf[15 + 7*j] = g_Damp_Info[index].active_power[i]%256;
				buf[16 + 7*j] = g_Damp_Info[index].reactive_power[i]/256;
				buf[17 + 7*j] = g_Damp_Info[index].reactive_power[i]%256;
				j++;
			}
		}
        data_len = 11 + 7*j;
		assemble_gprs_send_bdp(0xD1,buf,data_len);
	}
}
//�ϱ��ź�����
void send_singnal_quality_bdp(void)
{
	unsigned char buf[2] = {0};
	
	buf[0] = 0x0B;
	buf[1] = g_E3000_Info.SQ;
	assemble_gprs_send_bdp(0xD1,buf,2);
}
//���͵�����Ϣ,�Զ��屨��
void send_debug_bdp(unsigned char *contents, unsigned char len)
{
    unsigned char buf[96] = {0};
	unsigned char data_len = 0;
    buf[0] = 0xFF;  
	if(len > 96)
	{
	   len  = 96;
	}
	memcpy((char *)&(buf[1]),contents,len);
	data_len = len + 1;
    assemble_gprs_send_bdp(0xD1,buf,data_len);
}
//�ϱ���·��������
void send_loop_debuf_inf0_to_PC()
{
	unsigned char m = 0,n = 0;
	unsigned char debug_buf[15] = {0};
	
	debug_buf[0] = CUSTOM_MSG_TYPE_1;	//��·��Ϣ
	debug_buf[1] = g_Concentrator_Para.e3000_relay_enable[0];
	for(m = 0 ; m < MAX_EXT_MODULE_NUM ; m++)
		{
			for(n = 0 ; n < 4 ; n++)
			{		
				debug_buf[2 + m*4 + n] = g_extmodule_relay[m][n];
			}		
		}
		
	send_debug_bdp(debug_buf,14);
}
//�ϱ����ƿ�������
void send_lamp_debuf_inf0_to_PC(unsigned char *addr,unsigned short group,unsigned char *contents)
{
	unsigned char debug_buf[30] = {0};
	
	debug_buf[0] = CUSTOM_MSG_TYPE_2;	//������Ϣ
	debug_buf[1] = group/256;
	debug_buf[2] = group%256;
	memcpy((char *)&debug_buf[3],addr,6);
	memcpy((char *)&debug_buf[9],contents,18);

	send_debug_bdp(debug_buf,27);
}
//ʱ�ӳ������ϵͳʱ�䵽������
void send_local_time_bdp(unsigned char alarm)
{
	unsigned char debug_buf[22] = {0};
	
	debug_buf[0] = CUSTOM_MSG_TYPE_3;
	debug_buf[1] = alarm;
	debug_buf[2] = g_ts_time_fault;
	debug_buf[3] = g_rtc_time_fault;
	debug_buf[4] = g_now_year;
	debug_buf[5] = g_now_month;
	debug_buf[6] = g_now_day;
	debug_buf[7] = g_now_hour;
	debug_buf[8] = g_now_min;
	debug_buf[9] = g_now_sec;
	
	debug_buf[10] = g_ts_year;
	debug_buf[11] = g_ts_month;
	debug_buf[12] = g_ts_day;
	debug_buf[13] = g_ts_hour;
	debug_buf[14] = g_ts_min;
	debug_buf[15] = g_ts_sec;

	debug_buf[16] = g_rtc_year;
	debug_buf[17] = g_rtc_month;
	debug_buf[18] = g_rtc_day;
	debug_buf[19] = g_rtc_hour;
	debug_buf[20] = g_rtc_min;
	debug_buf[21] = g_rtc_sec;
	send_debug_bdp(debug_buf,22);
}

//�ϱ�����ʱ��
void send_local_working_time_bdp(void)
{
	unsigned char i = 0;
	unsigned char debug_buf[90] = {0};
	
	debug_buf[0] = CUSTOM_MSG_TYPE_4;

	for(i = 0;i < 8;i++)
	{
		debug_buf[1+11*i] = g_Light_Time_Info.s_time[i]/256;
		debug_buf[2+11*i] = g_Light_Time_Info.s_time[i]%256;
		debug_buf[3+11*i] = g_Light_Time_Info.e_time[i]/256;
		debug_buf[4+11*i] = g_Light_Time_Info.e_time[i]%256;
		debug_buf[5+11*i] = g_Light_Time_Info.group[i]/256;
		debug_buf[6+11*i] = g_Light_Time_Info.group[i]%256;
		debug_buf[7+11*i] = g_Light_Time_Info.loop[i]/256;
		debug_buf[8+11*i] = g_Light_Time_Info.loop[i]%256;
		debug_buf[9+11*i] = g_Light_Time_Info.ratio[i][2];
		debug_buf[10+11*i] = g_Light_Time_Info.ratio[i][1];
		debug_buf[11+11*i] = g_Light_Time_Info.ratio[i][0];
	}
	send_debug_bdp(debug_buf,89);
}

///////////////////////////////////////////////////////////////////////////
//�澯�ϴ�
///////////////////////////////////////////////////////////////////////////
//�ϱ�����������

//a)0x01��Ƭ����·��ͨ��ʧ��,����00
//b)0x02 ��չģ��ͨ��ʧ�ܣ�Ԥ��ֵ����0��ʵ��ֵ1��ʧ�ܵ���չ���
//c)0x03 �����澯��Ԥ��ֵ����0��ʵ��ֵ����1���������ĵ��»�·���
//0x04 �е�Ͽ���UPS����
///////////////////////////////////////////////////////////////////////////
void send_concentrator_fault_bdp(unsigned char fault,unsigned char num)
{
	unsigned char buf[4] = {0};
	unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x01;  //��������
		buf[1] = fault;	
		switch(fault)
			{
			   case CONCENTRATOR_FAULT_1:  //��·��ͨ��ʧ��
			   case CONCENTRATOR_FAULT_4:  //�е��л�USP����
			   	   buf[2] = 0;		       //Ԥ��ֵ��ʵ��ֵ����
			   	   data_len = 3;
			   	break;
			   case CONCENTRATOR_FAULT_2:  //����չģ��ͨ��ʧ��
			   	   buf[2] = 0x01;		   //Ԥ��ֵ��ʵ��ֵ����
			       buf[3] = num;		   //��չģ����
			       data_len = 4;
			   	break;
			   case CONCENTRATOR_FAULT_5:
			       buf[2] = 0;		       //Ԥ��ֵ��ʵ��ֵ����
			   	   data_len = 3;
			   	break;
			   default:
			   	break;
			}
		assemble_gprs_send_bdp(0xD2,buf,data_len);	
		
	}
	

	
}
//�ϱ����ƹ���
void send_lamp_fault_bdp(unsigned char index,unsigned char *contents)
{
	unsigned i = 0;
	unsigned char buf[17] = {0};
	unsigned char data_len = 0;

	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x02;  //��������
		buf[1] = g_Damp_Info[index].SN[0];
		buf[2] = g_Damp_Info[index].SN[1];
		buf[3] = g_Damp_Info[index].SN[2];
		buf[4] = g_Damp_Info[index].SN[3];

		for(i = 0;i < 6;i++)
		{
			buf[5 + i] = contents[i];		
		}
		data_len = 11;
	 	assemble_gprs_send_bdp(0xD2,buf,data_len);
	}
	

}


///////////////////////////////////////////////////////////////////////////
//����澯
///////////////////////////////////////////////////////////////////////////
//������������ϱ���
void send_concentrator_fault_release_bdp(unsigned char fault,unsigned char num)
{
	unsigned char buf[10] = {0};
	unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x01;  //��������
		buf[1] = fault;	
		switch(fault)
			{
			   case CONCENTRATOR_FAULT_1:  //��·��ͨ��ʧ��
			   case CONCENTRATOR_FAULT_4:  //�е��л�USP����		       
			   	   data_len = 2;
			   	break;
			   case CONCENTRATOR_FAULT_2:  //����չģ��ͨ��ʧ��
					data_len = 2;
			   	break;
			   default:
			   	break;
			}
		assemble_gprs_send_bdp(0xD3,buf,data_len);
	}
}
//������ƹ��ϱ���
void send_lamp_fault_release_bdp(unsigned char index,unsigned char fault)
{
	unsigned char buf[12] = {0};
    unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{;
		//ƴװ����������
		buf[0] = 0x02;  //��������		
		buf[1] = g_Damp_Info[index].SN[0];
		buf[2] = g_Damp_Info[index].SN[1];
		buf[3] = g_Damp_Info[index].SN[2];
		buf[4] = g_Damp_Info[index].SN[3];
		buf[5] = fault;		
		data_len = 6;
		assemble_gprs_send_bdp(0xD3,buf,data_len);
	}
	

}


///////////////////////////////////////////////////////////////////////////
//�¼��ϴ�
///////////////////////////////////////////////////////////////////////////
//�ϱ��ն�����
void send_terminal_reboot_bdp(void)
{
	unsigned char buf[7] = {0};
	unsigned char data_len = 0;

	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x01;  //�¼�����
		data_len = 1;
		assemble_gprs_send_bdp(0xD4,buf,data_len);
	}
}

//��������
void send_lamp_online_bdp(unsigned char index)
{
	unsigned char buf[6] = {0};
	unsigned char data_len = 0;

	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x02;  //�¼�����
		buf[1] = g_Damp_Info[index].SN[0];
		buf[2] = g_Damp_Info[index].SN[1];
		buf[3] = g_Damp_Info[index].SN[2];
		buf[4] = g_Damp_Info[index].SN[3];

		
		data_len = 5;
		assemble_gprs_send_bdp(0xD4,buf,data_len);
	}
	

}
//��������
void send_lamp_offline_bdp(unsigned char index)
{
	unsigned char buf[6] = {0};
    unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x03;  //�¼�����	
		buf[1] = g_Damp_Info[index].SN[0];
		buf[2] = g_Damp_Info[index].SN[1];
		buf[3] = g_Damp_Info[index].SN[2];
		buf[4] = g_Damp_Info[index].SN[3];
        data_len = 5;
		assemble_gprs_send_bdp(0xD4,buf,data_len);
	}
	

}

//���Ŵ�
void send_Boxdoor_open_bdp(unsigned char index)
{
	unsigned char buf[1] = {0};
    unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x04;  //�¼�����
        data_len = 1;
		assemble_gprs_send_bdp(0xD4,buf,data_len);
	}
	

}

//���Źر�
void send_Boxdoor_close_bdp(unsigned char index)
{
	unsigned char buf[1] = {0};
    unsigned char data_len = 0;
	
	if(g_GPRS_connect_OK == ONLINE)	//���߲�����
	{
		//ƴװ����������
		buf[0] = 0x05;  //�¼�����	
        data_len = 1;
		assemble_gprs_send_bdp(0xD4,buf,data_len);
	}	
}


