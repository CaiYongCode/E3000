#include "Update.h"
#include "globaldata.h"
#include "globalfunc.h"
#include "dacai_touch.h"
#include "TCP_Reconnect.h"
#include "PLC.h"
#include "IOctl.h"

void ActOn_Update_BDP(unsigned char * pbuf,unsigned short len)
{
	switch(pbuf[5])
	{
		case 0x09:	//ֹͣ����
			Update_Stop();
			break;
		case 0x0B:	//�·�������
			g_update_timeout_1000ms_counter = 0;
			
			Write_Update_Program_to_Flash(pbuf,len);
			Show_Update_Progress(pbuf[6],pbuf[7]);
			break;
		case 0x0D:	//֪ͨ����
			Update_Ready();
			break;
		
	}
	
	
}



void Update_Ready(void)
{
// 	//�����ǰʱ�䴦�ڹ���ʱ����,����������
// 	if(judge_light_time_stage(g_now_hour,g_now_min) > 0)
// 	{
// 		send_update_ready_bdp(0);
// 	}
// 	else
	{		
		g_update_flag = 1;
		Write_APP_Flag_to_Flash(SPACE1);
		
		send_plc_stop_bdp();	//��ͣ����
		
		send_update_ready_bdp(0);
		show_text(DGUS_STATE_SHOW_ADDR,9,"�汾������......  ",strlen("�汾������......  "));
	}
}

void Update_Stop(void)
{
	g_update_flag = 0;

	Write_APP_Flag_to_Flash(SPACE1);

	show_text(DGUS_STATE_SHOW_ADDR,9,"�汾������......  ",strlen("�汾������......  "));
}

//������������
void send_update_request_bdp(void)
{
	unsigned char buf[10] = {0}; 
	
	g_gprs_msg_num++;

	buf[0] = 0xA5;
	buf[1] = 0x00;
	buf[2] = 0x0A;
	buf[3] = g_gprs_msg_num;
	buf[4] = 0x99;
	buf[5] = 0x0A;
	buf[6] = 0x00;
	buf[7] = 0x00;
	buf[8] = 0x00;
	
	buf[9] = checksum(buf,9);
	gprs_data_send(buf,10);
}

//�ظ�����׼��״̬
void send_update_ready_bdp(unsigned char status)
{
	unsigned char buf[8] = {0}; 

	buf[0] = 0xA5;
	buf[1] = 0x00;
	buf[2] = 0x08;
	buf[3] = g_gprs_msg_num;
	buf[4] = 0x99;
	buf[5] = 0x0E;
	buf[6] = status;
	
	buf[7] = checksum(buf,7);
	gprs_data_send(buf,8);
}

//�ظ�����������
void send_update_package_request_bdp(unsigned char num)
{
	unsigned char buf[8] = {0}; 
	
	buf[0] = 0xA5;
	buf[1] = 0x00;
	buf[2] = 0x08;
	buf[3] = g_gprs_msg_num;
	buf[4] = 0x99;
	buf[5] = 0x0C;
	buf[6] = num;
	
	buf[7] = checksum(buf,8);
	gprs_data_send(buf,8);
}

//�ظ��������
void send_update_result_bdp(unsigned char result)
{
	unsigned char buf[8] = {0};; 

	buf[0] = 0xA5;
	buf[1] = 0x00;
	buf[2] = 0x08;
	buf[3] = g_gprs_msg_num;
	buf[4] = 0x99;
	buf[5] = 0x0F;
	buf[6] = result;
	
	buf[7] = checksum(buf,7);
	gprs_data_send(buf,8);
}

u32 appxaddr = FLASH_APP2_ADDR;	//��������д��FLASH���׵�ַ
void Write_Update_Program_to_Flash(u8 *appbuf,u32 appsize)
{
	u16 t = 0;
	u16 i = 0;
	u16 j = 0;
	u16 temp;
	u16 temp_buf[1024] = {0};
	u8 *dfu=NULL;
	u8 xorcheck = 0;
	u8 packNum = 0;

	packNum = appbuf[6];
	
// 	if(	(appsize != (appbuf[1]*256+appbuf[2])) )	//���������������ݳ��Ȳ�һ�£�ȡ�������ݳ���
// 		appsize = (appbuf[1]*256+appbuf[2]);
	
	for(j = 8;j < (appsize-2);j++)	//���У��
	{
		xorcheck ^= appbuf[j];
	}
	if(xorcheck == appbuf[appsize-2])		//У��ɹ�
	{
//		show_text(DGUS_STATE_SHOW_ADDR,9,"У��ɹ�......  ",strlen("У��ɹ�......  "));
		
		dfu=&appbuf[8];
		appsize = appsize-10;
		
		for(t=0;t<appsize;t+=2)
		{						    
			temp=(u16)dfu[1]<<8;
			temp+=(u16)dfu[0];	  
			dfu+=2;//ƫ��2���ֽ�
			temp_buf[i++]=temp;	    
			if(i==1024)
			{
				i=0;
				FLASH_HALFWORD_Write(appxaddr,temp_buf,1024);	
				appxaddr+=2048;//ƫ��2048  
			}
		}
		if(i)
		{
			FLASH_HALFWORD_Write(appxaddr,temp_buf,i);	//������һЩ�����ֽ�д��ȥ. 
		}
		
//		show_text(DGUS_STATE_SHOW_ADDR,9,"д��FLASH���......  ",strlen("д��FLASH���......  "));
		
		if(appbuf[6] == appbuf[7])		//��ǰ����������ܶ�����ȣ������������
		{
			send_update_result_bdp(0);		//�ظ��������			
						
			g_update_flag = 0;
			Write_APP_Flag_to_Flash(SPACE2);	
			
			show_text(DGUS_STATE_SHOW_ADDR,9,"�汾�������      ",strlen("�汾�������      "));
			
			Delay_s(5);
			NVIC_SystemReset();	//ϵͳ��λ
		}
		else
		{
			packNum++;
			send_update_package_request_bdp(packNum);		//������һ��������
		}
	}
	else
	{
		show_text(DGUS_STATE_SHOW_ADDR,9,"У��ʧ��,���½���......  ",strlen("У��ʧ��,���½���......  "));
		send_update_package_request_bdp(packNum);	//У��ʧ�ܣ������1֡������
	}
}

//��ʾ��������
void Show_Update_Progress(unsigned char packNum,unsigned char totalNum)
{
	unsigned char buf[32] = {0xC9,0xFD,0xBC,0xB6,0xCA,0xFD,0xBE,0xDD,0xBD,0xD3,0xCA,0xD5,0xD6,0xD0,0x2E,0x2E,0x2E,0,0,0,0x25};
	unsigned char buf1[3] = {0};	
	unsigned char ratio = 0;
	unsigned char len = 0;
	
	ratio = packNum*100/totalNum;
	len = data_conversion(buf1,ratio,0);
	
	memcpy(&buf[17],buf1,len);
	buf[17+len] = 0x25;
	
	show_text(DGUS_STATE_SHOW_ADDR,9,buf,17+len+1);
	
}

void Check_Update_Status(void)
{
	if((g_update_flag == 1)&&(g_GPRS_connect_OK == 1))
	{
		g_update_timeout_1000ms_counter++;
		if(g_update_timeout_1000ms_counter > 120)	//2����δ�յ����������ٴ�����ö�������
		{				
			g_update_timeout_1000ms_counter = 0;
			send_update_package_request_bdp(1);
			show_text(DGUS_STATE_SHOW_ADDR,9,"���½�������  ",strlen("���½�������  "));
		}
	}
}

void Delay_s(unsigned char s)
{
	unsigned short i = 0,j = 0;
	
	j = s*1000;
	while(j--)
	{
		i=10000;
		while(i--);
	}
}
