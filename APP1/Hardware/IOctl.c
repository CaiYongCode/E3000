#include "IOctl.h"
#include "globaldata.h"

void  Init_GPIO_config(void)
{
   //��ʼ��  ��ʼ��ʱ����
   // PC9--SWITCH CHECK    PA8--SWITCHCTL
    GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;     //       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;       //    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;   //RS485 ���ƹܽ�        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_3);	 //ʹ�ܽ��� 485

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   //RS485_1 ���ƹܽ�    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOA, GPIO_Pin_15);   //ʹ�ܽ��� RS485_1

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   //GPRS_RST      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_11);   //��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;   //8302_RST      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_14);   //�ø�
	////////////////////////////////////////////////////////////////////
	//KEY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //KEY 4   ȷ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;   //KEY 3   ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //KEY 2   ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;   //KEY 1	 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

////////////////////////////////////////////////////////////////////
	//LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   //LED7 	 ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);   //��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;   //LED6   ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);   //��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //LED5     ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);   //��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   //LED4    ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_15);   //��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;   //LED3  	GPS 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_14);   //��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;   //LED2    ͨ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);   //��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;   //LED1   ����   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_4);   //��
	
//////////////////////////////////////////////////////////////////////////////
	//�̵���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;   //C_K1   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);   //��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   //C_K2	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);   //��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //C_K3   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);   //��
}
//GPRS��λ
void Reset_GPRS_Module(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	OSTimeDly(200);
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	OSTimeDly(200);
}
//8302��λ
void Reset_8302_Module(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	OSTimeDly(200);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	OSTimeDly(100);
}

//����
void Set_RUN_LED_Status(unsigned char status)
{
	if(status == CLOSE)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	}
}

//ͨ��
void Set_GPRS_LED_Status(unsigned char status)
{
	if(status == CLOSE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
}

//PLC
void Set_PLC_LED_Status(unsigned char status)
{
	if(status == CLOSE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_14);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	}
}

//����	
void Set_DEBUG_LED_Status(unsigned char status)
{
	if(status == CLOSE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_15);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_15);
	}
}

//����	//����ȫ������ʱ��
void Set_NORMAL_LED_Status(unsigned char status)
{
	if(status == CLOSE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	}
}

//����  �е�����ʱ��
void Set_BRIGNT_LED_Status(unsigned char status)
{
	if(status == CLOSE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	}
}

//����	�е��ƹ���ʱ��
void Set_FAULT_LED_Status(unsigned char status)
{
	if(status == CLOSE)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_0);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	}
}

///////////////////////////////////////////////////////////////////////////////
// //�̵���1
// void Fhase_A_Ctl(unsigned char state)
// {
// 	if(state == CLOSE)
// 	{
// 		GPIO_SetBits(GPIOB, GPIO_Pin_8);   //�̵�����Ӧ		   
// 	}
// 	else
// 	{
// 		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
// 	}
// }

// //�̵���2
// void Fhase_B_Ctl(unsigned char state)
// {
// 	if(state == CLOSE)
// 	{
// 		GPIO_SetBits(GPIOB, GPIO_Pin_9);   //�̵�����Ӧ	   
// 	}
// 	else
// 	{
// 		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
// 	}
// }

// //�̵���3
// void Fhase_C_Ctl(unsigned char state)
// {
// 	if(state == CLOSE)
// 	{
// 		GPIO_SetBits(GPIOB, GPIO_Pin_5);    //�̵�����Ӧ
// 	}
// 	else
// 	{
// 		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
// 	}
// }
//E3000����3·�̵���ͳһ����
void E3000_Relay_Ctl(unsigned char state)
{
	if(state == CLOSE)	//��
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_8);	//A
		GPIO_SetBits(GPIOB, GPIO_Pin_9);	//B
		GPIO_SetBits(GPIOB, GPIO_Pin_5);    //C
	}
	else	//��
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);	//A
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);	//B
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);    //C
	}
}

// //��ȡ��λ������Ϣ  ע�⿪���ǳ��ջ��ǳ��� ��״̬��һ��
// unsigned char get_limit_switch_status(void)
// {
//     unsigned char val = 0;
//     val = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
//     return val;
// }

// //�������״̬ ������
// void check_box_door_status()
// {
//     unsigned char ret = 0;
// 	ret = get_limit_switch_status();
// 	if(OPEN == ret)  //�����Ѵ�
// 	{
// 	   
// 	}
// 	else
// 	{
// 		
// 	}
// }


/********************************************************************************************************** 
** Function name:      Plc_Led_Ctl
** Descriptions:       �ز�ͨ��ָʾ�� ��������, ����������  
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
unsigned char counter = 0;
void Plc_Led_Ctl(void)
{
	if(plc_comm_flag == 0)			//��ͨ��,��
	{
		Set_PLC_LED_Status(CLOSE);  
	}
	else if(plc_comm_flag == 1)			//����,��һ��
	{
		Set_PLC_LED_Status(OPEN);  
		plc_comm_flag = 0;
	}
	else if(plc_comm_flag == 2)		//����,������2��
	{		
		for(counter = 0;counter<2;counter++)
		{
			Set_PLC_LED_Status(OPEN);
			OSTimeDly(100);
			Set_PLC_LED_Status(CLOSE);
			OSTimeDly(100);
		}
		plc_comm_flag = 0;
	}
}
