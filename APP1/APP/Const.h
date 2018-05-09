#ifndef _CONST_H
#define _CONST_H



//ע�� DEBUG_MODE  ��ִ��
//�������ģʽ ��Ҫ�ͷ�
//#define DEBUG_MODE  1


//ʹ�ù���������  ��������� ��ʹ�õ��Է����� 
//#define WORKING_SERVER_IP

#define  TRUE     1
#define  FALSE    0

#define  OPEN     1
#define  CLOSE    0

#define  SIM900   1
#define  EC20     2

#define  ONLINE      1
#define  OFFLINE     0

#define  LAMP_STATE_ONLINE      2
#define  LAMP_STATE_OFFLINE     1
#define  LAMP_STATE_UNKNOW      0

//С��λ��
#define	 DECIMAL0	0
#define	 DECIMAL1	1
#define	 DECIMAL2	2
///////////////////////////////////////////////
#define	SOFTWARE_VERSION	0x180209		//����汾
///////////////////////////////////////////////
#define  AC_POWER_SUPPLY          0
#define  BATTERY_POWER_SUPPLY     1
///////////////////////////////////////////////
#define VOLTAGE_ALW  20     //��ѹ�ݲΧ �����ж���

#define VOLTAGE_THRESHOLD  36     //��ѹ�����ж��ż�
#define CURRENT_THRESHOLD  10     //���������ж��ż�
/////////////////////////////////////////////////////////////////
//����ģʽ
#define WEEK_MODE	    1
#define MONTH_MODE	    2
#define OFFLINE_MODE	3   //�������·����߹���ģʽ
////////////////////////////////////////////////////////////////
#define  KEY_OK_BTN          1  //�������
#define  KEY_RETURN_BTN      2
#define  KEY_SET_BTN         3
#define  KEY_FUNCTION_BTN    4
////////////////////////////////////////////////////////////////
#define DGUS_unregistered_SHOW_ADDR     1	//δע�����
#define DGUS_STATE_SHOW_ADDR          	2 	//״̬��ʾ����id
#define DGUS_DAMP_MANAGE_SHOW_ADDR      3 	//�ڵ�������id
#define DGUS_FUNCTION_SET_SHOW_ADDR		4	//�������ý���id
#define DGUS_TIME_SET_SHOW_ADDR			5	//����ʱ�����id
#define DGUS_LAMP_SET_SHOW_ADDR      	6	//�������ý���id
#define DGUS_DAMP_READ_SHOW_ADDR      	7	//�ڵ㳭������id
#define DGUS_MANUAL_CONTROL_SHOW_ADDR   8 	//�ֶ����ƽ���id
#define DGUS_EXPANSION_SET_SHOW_ADDR    9	//��չ���ý���id
#define DGUS_EXPANSION1_SET_SHOW_ADDR   10	//��չ1���ý���id
#define DGUS_EXPANSION2_SET_SHOW_ADDR   11	//��չ2���ý���id
#define DGUS_EXPANSION3_SET_SHOW_ADDR   12	//��չ3���ý���id
#define DGUS_DAMP_SHOW_ADDR         	13	//�ڵ���ʾ����id
#define DGUS_PROMPT_SHOW_ADDR         	14	//��ʾ����id
#define DGUS_CONCENTRATOR_SET_SHOW_ADDR 15	//���������ý���id
#define DGUS_PRINT_SHOW_ADDR   			16	//��ӡ����id
#define DGUS_SET_DAMP_ADDR_SHOW_ADDR    17	//���õ��Ƶ�ַ����id
#define DGUS_PASSWORD_INPUT_SHOW_ADDR	19	//�����������id
#define DGUS_LOOP_SHOW_ADDR				20  //��·��Ϣ��ʾ����
///////////////////////////////////////////////////////////////
//���� �ز����
#define INVALID_LAMP_ADDR    1
#define LAMP_ADDR_FULL       2
#define ADD_LAMP_ADDR_SUCESS 3
#define DEL_LAMP_ADDR_SUCESS 4

#define PLC_MSG_TYPE_1		1	//�ز���������:	Ӳ����ʼ��
#define PLC_MSG_TYPE_2		2	//�ز���������:	��������ʼ��
#define PLC_MSG_TYPE_3		3	//�ز���������: ��������
#define PLC_MSG_TYPE_4		4	//�ز���������:	��ͣ����
#define PLC_MSG_TYPE_5		5	//�ز���������: �ָ�����
#define PLC_MSG_TYPE_6		6	//�ز���������: �������ڵ��ַ
#define PLC_MSG_TYPE_7		7	//�ز��������ͣ���Ӵӽڵ�
#define PLC_MSG_TYPE_8		8	//�ز��������ͣ�ɾ���ӽڵ�
#define PLC_MSG_TYPE_9		9	//�ز���������:	��شӽڵ�
#define PLC_MSG_TYPE_10		10	//�ز���������: ���õ��Ƶ�ַ
#define PLC_MSG_TYPE_11		11	//�ز���������: ���õ������
#define PLC_MSG_TYPE_12		12	//�ز���������: ���ƿ���
#define PLC_MSG_TYPE_13		13	//�ز���������: ͬ��ϵͳʱ��
#define PLC_MSG_TYPE_14		14	//�ز���������: ͬ������ʱ��
#define PLC_MSG_TYPE_15		15	//�ز���������: ���õ��ƹ���ģʽ
#define PLC_MSG_TYPE_16		16	//�ز���������: ���õ��ƻ�·˳��
///////////////////////////////////////////////////////////////
//GPRS�Զ��屨������
#define CUSTOM_MSG_TYPE_1	1	//�ϱ���·��������
#define CUSTOM_MSG_TYPE_2	2	//�ϱ����ƿ�������
#define CUSTOM_MSG_TYPE_3	3	//ʱ�ӳ������ϵͳʱ�䵽������
#define CUSTOM_MSG_TYPE_4   4   //�ϱ�����ʱ��
///////////////////////////////////////////////////////////////
#define MAX_EXT_MODULE_NUM    3   //e3000���������չģ�����
///////////////////////////////////////////////////////////////
#define PWM_Counter     59999


#define RTU_ALREADY_CONFIG_FLAG        0xD5   //RTU��ע���־

///////////////////////////////////////////////////////////////
//A�������ַ��Χ0x08010000~0x08037FFF,��160K
//B�������ַ��Χ0x08038000~0x0805FFFF,��160K
#define SPACE1			1
#define SPACE2			2
#define FLASH_APP1_ADDR		0x08005000  	//��1��Ӧ�ó�����ʼ��ַ(�����FLASH)
											
#define FLASH_APP2_ADDR		0x0801F000  	//��2��Ӧ�ó�����ʼ��ַ(�����FLASH)
///////////////////////////////////////////////////////////////

//��������ַ��Χ0x08060000~0x0807FFFF,�Ӻ���ǰ�洢

//������ر�־1ҳ
#define APP_START_FLAG_ADDR			0x807F800			//APP������־	1Ϊ1��,2Ϊ2��
#define APP_RUN_FLAG_ADDR			(0x807F800+2)		//APP���б�־,0xAAΪʧ��,0xBBΪ�ɹ�
#define APP_UPDATE_FLAG_ADDR		(0x807F800+4)			//������־��1��ʶ��Ҫ����                                                                                                                 

//8302��Ϣ1ҳ
#define RTU_RN8302_INFO_START_ADDR  			0x807F000
//E3000���ò���1ҳ
#define RTU_CONFIG_INFO_START_ADDR				0x807E800
#define RTU_CONFIG_FLAG_ADDR					0x807E800
#define RTU_SN_START_ADDR						(0x807E800+2)
#define RTU_SOFTWARE_VERSION_START_ADDR			(0x807E800+18)
#define RTU_SIMCARD_NUM_START_ADDR				(0x807E800+26)
#define RTU_ROUTE_ADDR_START_ADDR				(0x807E800+54)
#define RTU_IP_SET_ADDR_START_ADDR				(0x807E800+66)
#define RTU_IP_LEN_ADDR_START_ADDR				(0x807E800+68)
#define RTU_IP_ADDR_START_ADDR					(0x807E800+70)
#define RTU_PORT_SET_ADDR_START_ADDR			(0x807E800+134)
#define RTU_PORT_LEN_ADDR_START_ADDR			(0x807E800+136)
#define RTU_PORT_ADDR_START_ADDR				(0x807E800+138)
#define RTU_WORKING_MODE_ADDR					(0x807E800+148)
#define DAMP_WORKING_MODE_ADDR					(0x807E800+150)
//��·����������Ϣ1ҳ
#define RTU_CONCENTRATOR_PARA_START_ADDR		0x807E000
//�ܹ�������1ҳ
#define RTU_WEEK_LIGHT_TIME_START_ADDR          0x807D800
//�¹�������24ҳ
#define RTU_MONTH_LIGHT_TIME_START_ADDR       	0x807D000
//������Ϣ2ҳ
#define DAMP_NUM_ADDR                      		0x807C800
#define DAMP_INFO_DATA_START_ADDR          		(0x807C800+2)
#define DAMP_INFO_DATA_START_ADDR2          	0x807C000	

//������ַ1ҳ
#define CMD_SAVE_ADDR							0x807B8000



#endif
