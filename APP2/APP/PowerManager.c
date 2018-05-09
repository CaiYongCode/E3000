#include "BIT.h"
#include "struct.h"
#include "IOctl.h"
#include "PowerManager.h"
#include "dacai_touch.h"
#include "GPRS.h"
/////////////////////////////////////////////////////////////////////
//��ʼ�� ��Դ����ܽ�����  ��ʼ��ʱ����
//PC6--UPS SUPPLY     PC8--UPS CHARGE 
void Init_powermanager_pin_config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA ,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 ;     //       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //
	GPIO_Init(GPIOC, &GPIO_InitStructure);   

	
}
/////////////////////////////////////////////////////////////////////
//���״̬  ��������   ����
unsigned char get_battery_status(void)
{
     //
	return 0;
}

/////////////////////////////////////////////////////////////////////
//�õ�����ͨ����Ϣ   ��������   ��ع���  ���ڵ��� �����
//1----��������   0---��ع���
unsigned char get_power_supply_channel(void)
{
	unsigned char m_Val = 0;
	m_Val = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
	return m_Val;
}
//////////////////////////////////////////////////////////////////////
//��ع������������
void set_battery_supply_mode()
{
}
//////////////////////////////////////////////////////////////////////
//��������ģʽ��������� 
void set_AC_supply_mode()
{
}


//��鹩�緽ʽ ������
void check_power_supply()
{
  unsigned char ret = 0;  	
	ret = get_power_supply_channel();
	if(AC_POWER_SUPPLY == ret)
	{
		if(g_Concentrator_Para.power_supply == BATTERY_POWER_SUPPLY)
		{
			show_power_supply(0);		//����ͼ����ʾ����
			adjust_TS_backlight(0);//�����ı��� 
			send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_4,0);	//�ϱ�������
			g_Concentrator_Para.power_supply = AC_POWER_SUPPLY;
		}
	}
	else if(BATTERY_POWER_SUPPLY == ret)
	{
	//��ع���  �رռ̵���(������չģ��)   �����ı���ر�  
 		if(g_Concentrator_Para.power_supply == AC_POWER_SUPPLY)
		{
		    show_power_supply(1);			//����ͼ����ʾ���
		    
#ifdef  DEBUG_MODE		
			adjust_TS_backlight(0xFF);
#endif				
			send_concentrator_fault_bdp(CONCENTRATOR_FAULT_4,0);	//�ϱ�������
			g_Concentrator_Para.power_supply = BATTERY_POWER_SUPPLY;
		}
	}
}
