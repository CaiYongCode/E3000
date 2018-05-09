#ifndef _TIMERTASK_H
#define _TIMERTASK_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"

#include "globaldata.h"
#include "globalfunc.h"

void Task_2000ms_Timer(void);//(void *pdata);                  //2000ms��ʱ����

void Task_1000ms_Timer(void *pdata);                  //1000ms��ʱ����

void Task_Key_Scan(void *pdata);

void Loop_Lamp_Control_Func(void);
void get_working_time_and_hours(void);
void get_working_time(unsigned char working_mode);
void get_working_hours(void);
void show_working_time(void);
unsigned char check_time_valid(unsigned short s_time,unsigned short e_time);

void check_power_supply(void);


void get_power_info(void);

//�ж����л�·״̬
void judge_Loop_status(void);

//��ʼʱ��α�־
void open_time_stage(unsigned char satge_inedx);

//��ʱ��α�־
void close_time_stage(unsigned char satge_inedx);

void Check_RTC_Time(void);
void Check_TS_Time(void);



#endif
