#include "Key.h"
#include "globaldata.h"
#include "globalfunc.h"
#include "IOctl.h"

unsigned char m_keydown_flag = 0;
unsigned char m_key_back = 0;
unsigned char	m_last_keyval = 0;
void Key_Scan(void)
{
	unsigned char m_keyval = 0,m_keyval2 = 0;

	if(OPEN == Get_Key_OK_Status())
	{
		m_keyval = KEY_OK_BTN; //��ȡKEY_OK_BTN ��ֵ 1
	}
	if(OPEN == Get_Key_RETURN_Status())
	{
		m_keyval = KEY_RETURN_BTN;  //KEY_RETURN_BTN ��ֵ 2
	}
	if(OPEN == Get_Key_SET_Status())
	{
		m_keyval = KEY_SET_BTN; //KEY_SET_BTN ��ֵ 3
	}
	if(OPEN == Get_Key_FUNCTION_Status())
	{
		m_keyval = KEY_FUNCTION_BTN; //��ȡKEY_FUNCTION_BTN ��ֵ 4
	}

	//��֮ǰ״̬�Ƚ�  ����������
	OSTimeDly(10);  //20ms��ʱ ������
	//ȷ�ϵ�ǰ�Ƿ��а�����Ӧ������ȡ����ֵ
   
	if(OPEN == Get_Key_OK_Status())
	{
		m_keyval2 = KEY_OK_BTN; //��ȡKEY_OK_BTN ��ֵ 1
	}
	if(OPEN == Get_Key_RETURN_Status())
	{
		m_keyval2 = KEY_RETURN_BTN;  //KEY_RETURN_BTN ��ֵ 2
	}
	if(OPEN == Get_Key_SET_Status())
	{
		m_keyval2 = KEY_SET_BTN; //KEY_SET_BTN ��ֵ 3
	}
	if(OPEN == Get_Key_FUNCTION_Status())
	{
		m_keyval2 = KEY_FUNCTION_BTN; //��ȡKEY_FUNCTION_BTN ��ֵ 4
	}
	if((m_keyval2 == m_keyval)&&(m_keyval > 0))
	{
		m_keydown_flag = 1; 
		m_last_keyval = m_keyval;   //��ȡȷʵ���µļ�ֵ
	}
   

	//�����µļ������ʱ��
	// m_key_back = 0;  //���������־
	if(m_keydown_flag == 1)
	{
		switch(m_last_keyval)
		{
			case KEY_OK_BTN:
				if(CLOSE == Get_Key_OK_Status())
				{	
					m_key_back = 1;
				}
				break;
			case KEY_RETURN_BTN:
				if(CLOSE == Get_Key_RETURN_Status())
				{	
					m_key_back = 1;
				}
				break;
			case KEY_SET_BTN:
				if(CLOSE == Get_Key_SET_Status())
				{	
					m_key_back = 1;
				}
				break;
			case KEY_FUNCTION_BTN:
				if(CLOSE == Get_Key_FUNCTION_Status())
				{	
					m_key_back = 1;
				}
				break;
			default:
				break;
		}
	}
   
	//���������Ӧ������Ϣ������Ϣ����
	if((m_last_keyval > 0) && (m_key_back  == 1))
 	{		
		SendMessage(KEY_MESSAGE_TYPE,1,(INT32U)m_last_keyval);
		m_last_keyval = 0;
		m_keyval2 = 0;
		m_keyval = 0;
		m_keydown_flag = 0;
		m_key_back = 0; 	  
 	}
}

/********************key IO�� ״̬��ȡ***************************/

//ȷ��
unsigned char Get_Key_OK_Status(void)
{
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
}
//����
unsigned char Get_Key_RETURN_Status(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);
}
//����
unsigned char Get_Key_SET_Status(void)
{
	return	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
}
//����
unsigned char Get_Key_FUNCTION_Status(void)
{
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
}


