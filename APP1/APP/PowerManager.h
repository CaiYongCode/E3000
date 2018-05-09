#ifndef _POWERMANGER_H
#define _POWERMANGER_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"

#include "globaldata.h"
#include "globalfunc.h"



//��ʼ�� ��Դ����ܽ�����
void Init_powermanager_pin_config(void);
//���״̬  ��������   ����

unsigned char get_battery_status(void);

//�õ�����ͨ����Ϣ   ��������   ��ع���  ���ڵ��� �����
unsigned char get_power_supply_channel(void);

//��ع������������
void set_battery_supply_mode(void);

//��������ģʽ��������� 
void set_AC_supply_mode(void);

//��鹩�緽ʽ ������
void check_power_supply(void);

#endif
