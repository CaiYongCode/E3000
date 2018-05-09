#ifndef _DS1302_H
#define _DS1302_H

#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"
#include "struct.h"
#include "const.h"

#define Sec_Add_W          0x80        //д���ַ
#define Sec_Add_R          0x81        //�����ַ
#define Min_Add_W          0x82        //д�ֵ�ַ
#define Min_Add_R          0x83        //���ֵ�ַ
#define Hour_Add_W          0x84        //дСʱ��ַ
#define Hour_Add_R          0x85        //��Сʱ��ַ
#define Data_Add_W          0x86        //д�յ�ַ
#define Data_Add_R          0x87        //���յ�ַ
#define Month_Add_W          0x88        //д�µ�ַ
#define Month_Add_R          0x89        //���µ�ַ
#define Week_Add_W          0x8a        //д���ڵ�ַ
#define Week_Add_R          0x8b        //�����ڵ�ַ
#define Year_Add_W          0x8c        //д���ַ
#define Year_Add_R          0x8d        //�����ַ
#define Protect_Add_W          0x8e        //дд������ַ
#define Protect_Add_R          0x8f        //��д������ַ
#define Charge_Add_W          0x90        //д����ַ
#define Charge_Add_R          0x91        //������ַ

#define BeiFen_Add_W           0xc4        //����RAM����������Ƿ������
#define BeiFen_Add_R           0xc5 

#define Open_Write_Protect               0x80        //��д����
#define Close_Write_Protect              0x00        //�ر�д����
#define Close_Charge                     0x00        //�رճ�繦��



#define Set_DS1302_RST_High()           GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define Set_DS1302_RST_Low()            GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define Set_DS1302_CLK_High()           GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define Set_DS1302_CLK_Low()            GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define Set_DS1302_SIO_High()           GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define Set_DS1302_SIO_Low()            GPIO_ResetBits(GPIOB, GPIO_Pin_15)

#define Read_DS1302_SIO()               GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)      



//extern Time_Typedef TimeValue_Init;     //���ڵ����ĳ�ʼ��

void DS1302_Init(void);
void DS1302_GetTime(Time_Typedef *PP);
void Set_RTC_Time(void);
void Get_RTC_Time(void);
unsigned char WeekDay(unsigned char year, unsigned char month, unsigned char day) ;
#endif






