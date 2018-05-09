#ifndef _APP_H
#define _APP_H
#include "stm32f10x.h"
#include "includes.h"
#include "Initialize.h"
#include "globaldata.h"
/************************************************************************************************
**��������ȼ�����
************************************************************************************************/
#define START_TASK_Prio              18       //��ʼ����
#define RTU_DEMO_TASK_Prio           50       //RTU����


#define Timer_1000ms_TASK_Prio       20       	//1000ms��ʱ����
#define Timer_2000ms_TASK_Prio       21       	//2000ms��ʱ����
#define MsgProc_TASK_Prio            22       	//��Ϣ������Ӧ����
#define TCP_RECONNETC_TASK_Prio      23     	//GPRSע������
#define Key_Scan_TASK_Prio           24     	//����ɨ������
/************************************************************************************************
**�����ջ�����������ֽڣ�
************************************************************************************************/
#define TIMER_2000MS_STK_SIZE        256      //2000MS��ʱ����
#define TIMER_1000MS_STK_SIZE        512      //1000MS��ʱ����
#define START_STK_SIZE               256       //��ʼ����
#define MSG_PROC_SIZE             	 512        //��Ϣ��������
#define KEY_SCAN_STK_SIZE            128      //����ɨ�趨ʱ����
#define TCP_RECONNECT_STK_SIZE       256      //tcp��ʱ�����������
////////////////////////////////////////////////////////////////////////////////////////


/************************************************************************************************
**��������
************************************************************************************************/
void TaskStart(void * pdata);                 //��ʼ����


void Task_COM_PROC(void *pdata);              //���ڴ�������


void System_App_Init(void);



#endif
