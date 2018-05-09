#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h"
#include "wdg.h"


int main(void)
{	
	unsigned short update_flag = 0;
	unsigned short app_start_flag = 0;
	unsigned short app_run_flag = 0;
	unsigned short flag[3] = {0};
	
// 	uart_init(9600);	//���ڳ�ʼ��Ϊ9600
// 	delay_init();	   	 	//��ʱ��ʼ�� 
	
	STMFLASH_Read(APP_UPDATE_FLAG_ADDR,&update_flag,1);

	STMFLASH_Read(APP_START_FLAG_ADDR,&app_start_flag,1);
	STMFLASH_Read(APP_RUN_FLAG_ADDR,&app_run_flag,1);


	if(app_run_flag != 0xBB)		//Ӧ�ó�������ʧ�ܻ��߳�ʼ״̬
	{
		if(app_start_flag != 1)		//2��Ӧ�ó�������ʧ�ܻ��߳�ʼ״̬
		{
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	
				flag[0] = 0x01;
				flag[1] = 0xAA;
				flag[2] = update_flag;
				STMFLASH_Write(APP_START_FLAG_ADDR,flag,3);					
				
				iap_load_app(FLASH_APP1_ADDR);//ִ��APP1����			
			}
		}
		else		//1��Ӧ�ó�������ʧ��
		{
			if(((*(vu32*)(FLASH_APP2_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				flag[0] = 0x02;
				flag[1] = 0xAA;
				flag[2] = update_flag;
				STMFLASH_Write(APP_START_FLAG_ADDR,flag,3);
				
				iap_load_app(FLASH_APP2_ADDR);//ִ��APP2����			
			}
		}
	}
	else
	{	
		if(app_start_flag == 1)	//����1������
		{
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				flag[0] = 0x01;
				flag[1] = 0xAA;
				flag[2] = update_flag;
				STMFLASH_Write(APP_START_FLAG_ADDR,flag,3);
				
				iap_load_app(FLASH_APP1_ADDR);//ִ��APP1����					
			}
		}
		else if(app_start_flag == 2)	//����2������
		{
			if(((*(vu32*)(FLASH_APP2_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				flag[0] = 0x02;
				flag[1] = 0xAA;
				flag[2] = update_flag;
				STMFLASH_Write(APP_START_FLAG_ADDR,flag,3);			
				
				iap_load_app(FLASH_APP2_ADDR);//ִ��APP2����	
			}
		}
	}
}













