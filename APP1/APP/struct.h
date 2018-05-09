#ifndef _STRUCT_H
#define _STRUCT_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"
#include "Const.h"

typedef struct R_msg
{
	unsigned short msg_type;
	unsigned short msg_len;
	int msg_addr;
}R_msg;


typedef struct RTU_CONFIG_INFO
{
    unsigned short config_flag;  //0x0D  ��ע��
	unsigned char SN[8];
	unsigned char soft_ersion[4];
	unsigned char simcard_num[14];	
	unsigned char route_addr[6];
	unsigned char ip_set;	
	unsigned char ip_len;
	unsigned char ip[32];
	unsigned char port_set;
	unsigned char port_len;
	unsigned char port[5];
	unsigned char SQ;
	unsigned int longitude;			//����
	unsigned int latitude;			//γ��
}RTU_CONFIG_INFO;

typedef struct Concentrator_parameter
{
	unsigned short Voltage[3];
	unsigned short e3000_current[3];        //���G���i·�����ɼ�
	unsigned char ext_enable[3];             // ��չģ��״̬
	unsigned short ext_current[MAX_EXT_MODULE_NUM][12];      //��չģ�����
	unsigned char power_supply;				//���緽ʽ
	unsigned char e3000_enable;
	unsigned char e3000_config_flag;		//���������ñ�־λ
	unsigned char e3000_relay_enable[3];		//�������̵�������״̬��0xFFδ���ã�0~100ͨ��0xFF��
	unsigned char e3000_V_enable[3];		//������3���ѹ����״̬��0xFFδ����
	unsigned char e3000_A_enable[3];		//������3���������״̬��0xFFδ����
	unsigned char ext_relay_enable[MAX_EXT_MODULE_NUM][4];	//3����չģ���4���̵�������״̬��0xFFδ���ã�0~100ͨ��0xFF��
	unsigned char  ext_phase_enable[MAX_EXT_MODULE_NUM][12];   	//3����չģ���12������״̬��0xFFδ����
}CONCENTRATOR_PARAMETER;


////////////////////////////////////////////////////////////////////////

//PLC

typedef struct  Lamp_bit_status
{
	unsigned short send_para_bdp_flag:1;    //���͵��������������� 1 �ѷ���
	unsigned short send_online_bdp_flag:1;  //���͵������������߱��� 1 �ѷ���
	unsigned short send_status_bdp_flag:1;  //���͵����������ϱ��� 1 �ѷ���
	unsigned short read_flag:2;             //��ǰ�ڵ㳭����־λ:0δ����;1�ѳ���;2�����ɹ�
	unsigned short lamp_read_status:1;      //�ڵ��Ѿ������ɹ�����־
	unsigned short switch_flag:1;           //���ر�־λ
	unsigned short phase:2;                 //������λ
	unsigned short status:1;                //�洢״̬ ���޴洢�ڵ�
	unsigned short group:4;                 //�������
	unsigned short lamp_online_flag:2;      //��ǰ�ڵ����� ���ߣ�δ֪ ��־  ��GPRS ����������Ĭ�����е��ƶ������ߵ�	
}Lamp_bit_status;



typedef struct  Damp_Info
{
	unsigned char SN[6];	      
	unsigned short loop;			      //3��·״̬
	unsigned char ratio[3];         	  //����--0xFF��ʾ��·�Ͽ�			//
	unsigned short voltage;			      //��ѹ
	unsigned short current[3];		      //3��·����ֵ
	unsigned short active_power[3];	      //3��·�й�����
	unsigned short reactive_power[3];	  //3��·�޹�����         
	unsigned char fault_flag;	          //���ϱ�־λ 
	Lamp_bit_status m_bit_status;
}Damp_Info;


typedef struct  Send_bdp_FIFO
{
	unsigned char send_counter; //��ǰ���ķ��ʹ���
	unsigned char buf[156];   	//���淢�ͱ�������
	unsigned char len;          //���ͱ��ĳ���
	unsigned char type;			//��������
}Send_bdp_FIFO;


typedef struct  Send_GPRS_bdp_FIFO
{
   unsigned char send_counter;  //��ǰ���ķ��ʹ���
   unsigned char buf[128];   //���淢�ͱ�������
   unsigned char len;           //���ͱ��ĳ���
}Send_GPRS_bdp_FIFO;

///////////////////////////////////////////////

typedef struct Light_Time_Info
{
	unsigned short s_time[8];
	unsigned short e_time[8];
	unsigned short group[8];
	unsigned short loop[8];  
	unsigned char ratio[8][3];
}Light_Time_Info;





//RN8302 

typedef struct  EffectParSetToSave
{

    uint16_t adjust_valid_flag;       //У��ϵ����Ч��־   1��Ч
	uint16_t voltage_adjust_ratio[3]; //�����ѹУ��ϵ��
	uint16_t current_adjust_ratio[3]; //�������У��ϵ��

	
	uint16_t CosH;			//��������
	uint16_t CosL;			//��������
	uint16_t U_Max;			//��ѹ�������
	uint16_t U_Mix;			//��ѹ��С����
	uint16_t I_Mix;			//������С����
	uint16_t THDu;          //��ѹг���ܻ���������
	uint16_t THDi;          //����г���ܻ���������
	uint16_t CT;            //�������
	uint16_t DelayTime;		//��ʱʱ��
	uint16_t SystemPassword; //����		
	uint16_t Addr;       //�ն˵�ַ��
	uint16_t PW;          //�ն�����
	uint16_t BAUD ;       //ͨ�Ų�����
	uint16_t IP1[4];     //��IP   IP1[0].IP1[1].IP1[2].IP1[3]
	uint16_t Port1;      //���˿�
	uint16_t IP2[4];     //��IP   IP2[0].IP2[1].IP2[2].IP2[3]
	uint16_t Port2;      //���˿�
	uint16_t GSUA;			//��ѹͨ���͵���ͨ�����������
	uint16_t GSUB;
	uint16_t GSUC;
	uint16_t GSIA;
	uint16_t GSIB;
	uint16_t GSIC;	
	uint16_t PHSUA;
	uint16_t PHSUB;
	uint16_t PHSUC;
	uint16_t Work_Mode;   //����ģʽ 0Ϊ�첹��1Ϊ����

	
}EffectParSetToSave;

typedef struct typPHASE_POWER_ITEM
{
	uint16_t Ux;		//��ѹֵUa,Ub,Uc
	int16_t Cosx;      //��������--����
	uint32_t Ix;		//����ֵIa,Ib,Ic	
	uint32_t Px;        //�й����� >   1L<<31   Ϊ����
	uint32_t Qx;        //�޹����� >   1L<<31   Ϊ����
	uint32_t Px_WP_Pos;   //����
	uint32_t Px_WP_Neg;   //����
	uint32_t Qx_WP_Pos;
	uint32_t Qx_WP_Neg;
	uint8_t  Ux_Over;   //��ѹ���
	uint8_t  Ix_Over;   //�������
	uint8_t  THDu_Over; //��ѹ�ܻ��������
	uint8_t  THDi_Over; //�����ܻ��������
}PHASE_POWER_ITEM;

typedef struct Time_Typedef //  bit7        bit6          bit5          bit4          bit3          bit2          bit1          bit0        
{		
	uint8_t second; //���� ,  =0,������            bit6-bit4  10second                               bit3-bit0  second
	uint8_t minute; //����       ��Ч                bit6-bit4  10minter                               bit3-bit0  minter
	uint8_t hour;   //Сʱ   =0 24Сʱģʽ    ��Ч  =1ѡ��20-23Сʱ�� =1ѡ��10-20Сʱ��                bit3-bit0  hour
	uint8_t date;   //��         ��Ч         ��Ч           bit5-bit4  10data                         bit3-bit0  data
	uint8_t month;  //��         ��Ч         ��Ч          ��Ч         10month                       bit3-bit0  month
	uint8_t week;   //����       ��Ч         ��Ч          ��Ч          ��Ч          ��Ч                    bit2-bit0  month 
	uint8_t  year;   //��	                     bit7-bit4  10year                                        bit3-bit0  year 
}Time_Typedef;


#endif
