#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.

//����0X08000000~0X0800FFFF�Ŀռ�ΪIAPʹ��
//������ر�־1ҳ
#define APP_START_FLAG_ADDR			0x807F800			//APP������־	1Ϊ1��,2Ϊ2��
#define APP_RUN_FLAG_ADDR			(0x807F800+2)		//APP���б�־,0xAAΪʧ��,0xBBΪ�ɹ�
#define APP_UPDATE_FLAG_ADDR		(0x807F800+4)			//������־��1��ʶ��Ҫ����

#define FLASH_APP1_ADDR		0x08005000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
											
#define FLASH_APP2_ADDR		0x0801F000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
																						
void iap_load_app(u32 appxaddr);			//ִ��flash�����app����

//void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//��ָ����ַ��ʼ,д��bin

#endif







































