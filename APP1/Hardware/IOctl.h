#ifndef _IOCTL_H
#define _IOCTL_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"
#include "struct.h"
#include "const.h"

void  Init_GPIO_config(void);

void Reset_GPRS_Module(void);
void Reset_8302_Module(void);

void Set_RUN_LED_Status(unsigned char status);
void Set_GPRS_LED_Status(unsigned char status);
void Set_PLC_LED_Status(unsigned char status);
void Set_DEBUG_LED_Status(unsigned char status);
void Set_NORMAL_LED_Status(unsigned char status);
void Set_BRIGNT_LED_Status(unsigned char status);
void Set_FAULT_LED_Status(unsigned char status);

// void Fhase_A_Ctl(unsigned char state);
// void Fhase_B_Ctl(unsigned char state);
// void Fhase_C_Ctl(unsigned char state);
void E3000_Relay_Ctl(unsigned char state);

//��ȡ��λ������Ϣ
unsigned char get_limit_switch_status(void);
//�������״̬ ������
void check_box_door_status(void);

void Plc_Led_Ctl(void);

#endif
