#include "TimerTask.h"
#include "RN8302.h"
#include "BIT.h"
#include "GPRS.h"
#include "PLC.h"
#include "dacai_touch.h"
#include "Key.h"
#include "IOctl.h"
#include "wdg.h"
#include "LM75A.h"
#include "globaldata.h"
#include "ExtModule.h"
#include "DS1302.h"
#include "Lamp.h"
#include "Update.h"
/********************************************************************************************************** 
**********************************************************************************************************/
extern OS_EVENT * QMsg; 


/********************************************************************************************************** 
** Function name:      Task_2000ms_Timer
** Descriptions:      
** input parameters:   void *pdata
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
unsigned short m_2000ms_counter = 0;

unsigned char m_working_time_num;
unsigned char m_send_working_time_flag = 0;
unsigned short m_heart_counter = 0;
unsigned short m_send_PLC_sys_time_Counter = 0;

unsigned char b_send_bus_state = 0;
unsigned char m_show_working_time_couter;
unsigned char m_show_working_time_flag;
unsigned char m_extmodule_relay[3][4];
void Task_2000ms_Timer(void)
{		
	{ 	
		m_2000ms_counter++;
////////////////////////////////////////////////////////////////////////////////////
		//20�뷢һ����������
		if(g_GPRS_connect_OK == ONLINE)
		{			
			m_heart_counter++;
			if(m_heart_counter >= 10)  
			{
				send_gprs_heart_bdp();
				m_heart_counter = 0;
			}
		}
//////////////////////////////////////////////////////////////////////////////
		//��ȡ����ʱ���ʱ�� ֻ�е�����ʱ��Ϊ0:07��ʱ�򣬲Ŷ�ȡFLASHһ��
		//��ȡ��ϵͳʱ��Ż�ȡ����ʱ��
		if(g_first_get_TS_rtc_time_flag == 1)
		{
			get_working_time_and_hours();
			//��ʾ����ʱ��
			show_working_time();
		}
		//////////////////////////////////////////////////////////////////////////////
		//ԤԼ���ؿ���  ���� ���߶��� ���ؿ�
		//ֻ���ڷǵ���ģʽ��ʱ��,ͬʱ�������·�����ģʽΪ�ܻ�����ԤԼ����ģʽ��ʱ�򣬱��ؿ��Ʋ�������
		if ((g_mannual_debug_flag == 0) && ((g_working_mode == MONTH_MODE) || (g_working_mode == WEEK_MODE)) )    
		{
			//��ȡ��ϵͳʱ��Ž��б��ؿ���
			if(g_first_get_TS_rtc_time_flag == 1)
			{
				Loop_Lamp_Control_Func();	     //���ؿ���
			}
		}	

//////////////////////////////////////////////////////////////////////////////		
		m_send_PLC_sys_time_Counter++;
		if(m_send_PLC_sys_time_Counter > 120)     //4����ͬ��һ��PLCʱ��
		{
			b_send_bus_state = 1 - b_send_bus_state;
			if(b_send_bus_state == 1)
			{
				send_system_time_bdp();	          //ͬ������ϵͳʱ��
			}
			else
			{
				send_working_time_bdp();		 //���͹���ʱ�䵽����
			}
			
			m_send_PLC_sys_time_Counter = 0;
		}
//////////////////////////////////////////////////////////////////////////////			
	}			
}

unsigned short m_1000ms_counter = 0;
unsigned char m_show_damp_1000ms_counter = 0;
unsigned char m_e3000_relay_status = 0;
unsigned char bbb_2000ms_flag = 0;
unsigned char m_get_power_info_counter = 0;
unsigned char m_plc_cmd_resend_counter = 0;
unsigned char m_send_time_fault_flag = 0;
void Task_1000ms_Timer(void *pdata)
{	
	unsigned short value = 0;
	
#ifdef  DEBUG_MODE
	IWDG_Init(5,2000);    //��Ƶ��Ϊ128,����ֵ��11λ��Ч  С�� 2047,���ʱ��Ϊ8s	
#endif

	while(1)
	{	
		m_1000ms_counter++;
		//���е�1Sѭ������
		Set_RUN_LED_Status(m_1000ms_counter % 2);
		//��������״̬,�������������� 8bb
		if(g_update_flag != 1)
		{
			//�ز�ͨ��״ָ̬ʾ��
			Plc_Led_Ctl();
	
///////////////////////////////////////////////////////////////////////////////////////		
			//2S����
			bbb_2000ms_flag = 1 - bbb_2000ms_flag;
			if(bbb_2000ms_flag == 0)	
			{
				Task_2000ms_Timer();
			}			
///////////////////////////////////////////////////////////////////////////////////////	
			//��ȡ����֤RTC��TSʱ��	
			Check_RTC_Time();
			Check_TS_Time();
			//Ĭ��ʹ�ô�����ʱ��,
			//������ʱ�ӹ���,ʹ�ô�����ʱ��
			if(g_ts_time_fault == 0)
			{
				g_now_year = g_ts_year;
				g_now_month = g_ts_month;
				g_now_day = g_ts_day;
				g_now_hour = g_ts_hour;
				g_now_min = g_ts_min;
				g_now_sec = g_ts_sec;
				g_now_week = g_ts_week;
			}
			else
			{			
				g_now_year = g_rtc_year;
				g_now_month = g_rtc_month;
				g_now_day = g_rtc_day;
				g_now_hour = g_rtc_hour;
				g_now_min = g_rtc_min;
				g_now_sec = g_rtc_sec;
				g_now_week = g_rtc_week;
			}
///////////////////////////////////////////////////////////////////////////////////////			
			//RTC��TSʱ��ȫ���������ϱ�ʱ�ӹ��ϣ�������ʱ�ӹ���
			if( (g_rtc_time_fault == 1)&&(g_ts_time_fault == 1) )
			{
				if(m_send_time_fault_flag == 0)
				{
					send_concentrator_fault_bdp(CONCENTRATOR_FAULT_3,0);
					m_send_time_fault_flag = 1;
				}
			}
			else //���������һ��ʱ����������֮ǰ�ϱ������ϣ�����ʱ�ӹ���
			{
				if( m_send_time_fault_flag == 1)
				{
					send_concentrator_fault_release_bdp(CONCENTRATOR_FAULT_3,0);
					m_send_time_fault_flag = 0;
				}
			}
///////////////////////////////////////////////////////////////////////////////////////	
			//ÿ20�뱣��һ��ϵͳʱ�� �Ա�������ԭ״̬��
			if((m_1000ms_counter % 20) == 19)  
			{
				Write_CMD_Info();	//���濪�ص�����
			}
///////////////////////////////////////////////////////////////////////////////////////		
			//���2���ϱ���������ʾ
			value = Get_LM75A_Temp_Value();
			if( 20 <= abs(g_LM75A_temp - value) )  
			{		
				g_LM75A_temp = value;
				show_temperature();//��ʾ�¶�
				send_concentrator_temperature_bdp();
			}
///////////////////////////////////////////////////////////////////////////////////////		
			//·��ͨ�Ź����ж�
			g_PLC_timeout_1000ms_counter++;
			if(g_PLC_timeout_1000ms_counter > PLC_COMMUNICATION_OVERTIMR)	//��Ƭ����·��ͨ�ų�ʱ,5����
			{						
				g_router_fault_reboot_num++;	
				//�������θ�λ·����Ȼ��������ͨ��
				if(g_router_fault_reboot_num >= 3)
				{					
					g_router_fault_reboot_num = 0;						
					if( g_router_fault_flag != 1 )
					{
						send_concentrator_fault_bdp(CONCENTRATOR_FAULT_1,0);	//�ϱ���������·��ͨ�Ź���
						g_router_fault_flag = 1;
					}								
				}
				g_PLC_timeout_1000ms_counter = 0;
				send_hardware_init_bdp();	//·�ɸ�λ
			}	
			//·�ɳ�����ʱ�ж�
			if(g_lamp_total_num != 0)	//�ӽڵ㵵����Ϊ��
			{	
				g_plc_read_1000ms_couter++;
				if(g_plc_read_1000ms_couter > PLC_READ_OVERTIMR)
				{
					send_plc_restore_bdp();		//�ָ�����
					g_plc_read_1000ms_couter = 0;
				}
			}
			//·�ɳ�����ɣ�60S����������
			if(g_read_restart_flag == 1)
			{
				g_read_restart_counter++;
				if(g_read_restart_counter >= 60)
				{
					show_lamp_statistics();		//��ʾ����ͳ����Ϣ
					clear_lamp_read_flag();	//����ǰ��յ��Ʋ���
					send_plc_reboot_bdp();
					
					g_read_restart_flag = 0;
					g_read_restart_counter = 0;
				}
			}
			//��ⵥ�ƿ��غ͹���
			check_lamp_status();
//////////////////////////////////////////////////////////////////////////////////////////////////////
			//��鹩�緽ʽ ������
			check_power_supply();
			 //��չģ�鷢�����������
			Send_CMD_bdp_to_ExtModule();
			//��չģ�������ж�
			Ext_Module_online_judge();
			//�ֶ����Կ��ص�ģʽ����ʱ
			if(g_debug_countdown_counter > 0)
			{
			   g_debug_countdown_counter --;
			   //�˳�����������״̬��֮ǰ���ص�״̬������
			   if(g_debug_countdown_counter == 0)
			   	{
			   	   g_mannual_debug_flag = 0;    //�رյ���ģʽ
				   Set_DEBUG_LED_Status(CLOSE);  
			   	}
			}
//////////////////////////////////////////////////////////////////////////////////////////////////////
		

/////////////////////////////////////////////////////////////////////////////////////////////////////
			//��ʱ�ɼ���ѹ����״̬
			get_power_info();

			//�жϲ����Ƿ���Ҫ�ϱ���������
			//�ϱ�ʱ��  5�����ϱ�һ��  
			//���صƺ�ʱ10���Ӻ��ϱ�
			if(g_send_concentrator_parameter == 1)  //���ػ�·ʱ��
			{
				//�ж��Ƿ��л�·�ɿ����أ���Ҫ��λ8302
				judge_Loop_status();
				
				m_get_power_info_counter++;  //��ѹ��������ʱ
				//�صƺ�10S��λ8302
				if( (g_reset_E3000_8302_flag == 1)&&(m_get_power_info_counter == 10) )
				{
					Reset_8302_Module();
					RN8302_Init();
					g_reset_E3000_8302_flag = 0;
				}

				//��ȡ��������ѹ���������ݵ����䶯�����ϱ�
				if(m_get_power_info_counter >= 20)	//20S�ϱ�
				{			
					//�ϱ�����������
					send_concentrator_parameter_bdp();

					m_get_power_info_counter = 0;
					g_send_concentrator_parameter = 0;
				}
			}
			else  //�ǿ��ػ�·ʱ��
			{
				//5���Ӷ�ʱ�ϱ�
				if((m_1000ms_counter % 300) == 20) 
				{
					send_concentrator_parameter_bdp();
				}
			}
/////////////////////////////////////////////////////////////////////////////////////////////////////  					 	
			SendMessage(TIMER_1000MS_MESSAGE_TYPE,0,0);
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////		
		//��֤����״̬
		Check_Update_Status();
//////////////////////////////////////////////////////////////////////////////////////////////////////			
		IWDG_Feed();  //ι��
		
		OSTimeDly(500);
	}
}

void Task_Key_Scan(void *pdata)
{
	while(1)
	{
		Key_Scan();
		OSTimeDly(10);
	}		
}


unsigned char m_start_cmd[8] = {0};	//8�ι�����ʼʱ�俪�Ʊ�־
unsigned char m_end_cmd[8] = {0};	//8�ι���ֹͣʱ�俪�Ʊ�־
//��·�Լ�����  ���ؿ��� ��ʱִ��
void Loop_Lamp_Control_Func(void)
{
	unsigned char i = 0;
	unsigned short m_val = 0;	            //ʵʱʱ��

	//��ȡʵʱʱ��
	m_val = g_now_hour * 60 + g_now_min;

	//���μ��8�ι���ʱ��
	for(i = 0; i < 8; i++)
	{	
		if(t_v[i] == 1)                      //��ǰʱ�����Ч
		{ 
			if( m_val == t_s[i])	         //����ʱ�̵�  
			{			
				if(m_start_cmd[i] == 0)		 //��ִֻ֤��һ��
				{
				    m_start_cmd[i] = 1;
					/////////////////////////////////////////////////////////////////////////////////////
					open_time_stage(i);
					/////////////////////////////////////////////////////////////////////////////////////
					send_local_time_bdp(0);
				}
				
			}
			else
			{
			  //����t_s[i](����)ʱ���ѱ�־λ���� ͬʱ��Ϊ�Ѿ�����t_s[i]ʱ�̣����������Ѿ�����ȥ�ˣ���ֻ֤��һ�ν��벢ִ�п��ƴ���
			  m_start_cmd[i] = 0;
			}
           //////////////////////////////////////////////////////////////////////////////////////////
			if(m_val == t_e[i])   //�ص�ʱ�̵�
			{
				if(m_end_cmd[i] == 0)		   //��ִֻ֤��һ��
				{
                    m_end_cmd[i] = 1;	
					
					close_time_stage(i);
				/////////////////////////////////////////////////////////////////////////////////////
					send_local_time_bdp(0);
				}
			}
			else
			{
			   m_end_cmd[i] = 0;
			}
			//////////////////////////////////////////////////////////////////////////////////////
		}			
	}
}

unsigned char m_now_month = 0;
void get_working_time_and_hours(void)
{
  ////////////////////////////////////////////////////////////////////////
   //ֻ�е�0:07��ʱ�䵽������ֻ��һ��(g_get_at_update_time_flag = 0)�Ŷ�ȡFLASHһ��
   
	if((g_read_falsh_data_info == 1) && (g_get_at_update_time_flag == 0))
		{
		    get_working_time(g_working_mode);
			get_working_hours();
			g_read_falsh_data_info = 0;     // ȷ��ֻ��һ��
			g_get_at_update_time_flag = 1;  // ȷ��ֻ��һ��

		}
	/////////////////////////////////////////////////////////////////////////
	if((g_now_hour == 0) &&(g_now_min == 0x07)) // ÿ��0:07�� ����ʱ��
	{
		g_read_falsh_data_info = 1;
	}
	else
	{
		g_read_falsh_data_info = 0;     // ȷ��ֻ��һ��
		g_get_at_update_time_flag = 0;   //��ձ�־��Ϊ����һ�θ���
	}
}

void get_working_time(unsigned char working_mode)
{
	unsigned char i = 0;
	unsigned char index = 0;
	
	m_working_time_num = 0;
   //////////////////////////////////////////////////////////////////////////////////////////////////////


	if(working_mode == WEEK_MODE)	//���ܹ���ģʽ
	{
		Read_Week_Light_Time_Info();
	}
	else if(working_mode == MONTH_MODE)	//���¹���ģʽ
	{
		Read_Month_Light_Time_Info();
	}
	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
    for(i = 0;i < 8;i++)
	{
		//�жϹ���ʱ���Ƿ���Ч
		t_v[i] = check_time_valid(g_Light_Time_Info.s_time[i],g_Light_Time_Info.e_time[i]);
		if(t_v[i] == 1)
		{		
			//ƴװ����ʱ��
			sprintf((char*)g_showtime_buf[index],"%2d:%2d-%2d:%2d",g_Light_Time_Info.s_time[i]>>8,g_Light_Time_Info.s_time[i]&0xff,\
						g_Light_Time_Info.e_time[i]>>8,g_Light_Time_Info.e_time[i]&0xff);				
			index++;
			m_working_time_num++;
		}
	}
	//�����Ч����ʱ����ʾ����
	for(i = index;i < 8;i++)
	{
		memset(g_showtime_buf[i],0,32);
	}
	//////////////////////////////////////////////////////////////////////////
	//��ȡ�µĹ���ʱ��֮��     ��ʾ����ʱ���־λ��1
	m_show_working_time_flag = 1;
	//////////////////////////////////////////////////////////////////////////
	//�ϱ�����ʱ��
	send_local_time_bdp(0);	
	//�ϱ�����ʱ��
	send_local_working_time_bdp();
}

void get_working_hours(void)
{
	unsigned char i = 0;
	
	g_lamp_working_time_num = 0;
	
	for(i = 0;i < 8;i++)
		{
			t_s[i] = g_Light_Time_Info.s_time[i]/256*60+g_Light_Time_Info.s_time[i]%256;
			t_e[i] = g_Light_Time_Info.e_time[i]/256*60+g_Light_Time_Info.e_time[i]%256;
			t_g[i] = g_Light_Time_Info.group[i];
			t_l[i] = g_Light_Time_Info.loop[i];
			t_r[i][0] = g_Light_Time_Info.ratio[i][0];
			t_r[i][1] = g_Light_Time_Info.ratio[i][1];
			t_r[i][2] = g_Light_Time_Info.ratio[i][2];
			
			if(t_g[i] != 0)	//�Ե���
			{	
				if(g_lamp_working_time_num < 4)		//ֻȡ4��	
				{
					g_lamp_working_time_index[g_lamp_working_time_num] = i;
					g_lamp_working_time_num++;
				}
				
			}
		}
}

void open_time_stage(unsigned char satge_inedx)
{

	unsigned char contents[18] = {0};	    //������������
	unsigned short m_swicth = 0;	        //��·������ݴ�
	unsigned char m_open = 0;		        //�������ڿ��Ʊ�־
	unsigned char m = 0,n = 0;
	unsigned char addr[6] = {0};	        //���Ƶ�ַ

    if(t_g[satge_inedx] == 0)		     //��E3000����
	{
		m_swicth = t_l[satge_inedx];
		//E3000��·1�����ж�
		//��ǰҪ��
		if( (m_swicth & 0x01) == 0x01 )	
		{
			if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//E3000�̵�������
			{
				E3000_Relay_Ctl(OPEN);		//��E3000���м̵���
				g_Concentrator_Para.e3000_relay_enable[0] = 100;
			}
		}
		
		else
		{						
			if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//E3000�̵�������
			{
				E3000_Relay_Ctl(CLOSE);		//�ر�E3000���м̵���
				g_Concentrator_Para.e3000_relay_enable[0] = 0;
			}
		}
		//��չ��·�����ж�
		m_swicth = m_swicth >> 1;
		for(m = 0 ; m < MAX_EXT_MODULE_NUM ; m++)
		{
			for(n = 0 ; n < 4 ; n++)
			{
				if(g_Concentrator_Para.ext_relay_enable[m][n] != 0xFF)	//��չģ��̵�������
				{
					m_open = ( m_swicth >> (m*4+n) ) & 0x01;
					
					//���ͬʱ��չ���ߣ�ִ�п���
					if( (m_open == 1)&&(g_ext_online_flag[m] == ONLINE) )	
					{										
						g_extmodule_relay[m][n] = 1;
						g_Concentrator_Para.ext_relay_enable[m][n] = 100;
					}
					
					else
					{
						g_extmodule_relay[m][n] = 0;
						g_Concentrator_Para.ext_relay_enable[m][n] = 0;
					}
					
				}
			}
			
		}
		
		g_send_concentrator_parameter = 1;  //20S���ϱ�����
//////////////////////////////////////////////////////		
		send_loop_debuf_inf0_to_PC();
	}
    else				//�Ե�������
	{
		contents[0] = t_l[satge_inedx]/256;
		contents[1] = t_l[satge_inedx]%256;
		contents[15] = t_r[satge_inedx][2];
		contents[16] = t_r[satge_inedx][1];
		contents[17] = t_r[satge_inedx][0];
		send_plc_cmd_bdp(addr,t_g[satge_inedx],contents);

////////////////////////////////////////////////////////		
		send_lamp_debuf_inf0_to_PC(addr,t_g[satge_inedx],contents);
	}


}


void close_time_stage(unsigned char satge_inedx)
{
    unsigned char contents[18] = {0};	    //������������
	unsigned char m_open = 0;		        //�������ڿ��Ʊ�־
	unsigned char m = 0,n = 0 ,j = 0;
	unsigned char addr[6] = {0};	        //���Ƶ�ַ
	unsigned short m_group = 0;		        //������
	unsigned short m_g = 0;					//��ʱ��εĵ�����
	unsigned short m_other_g = 0;		//����ʱ��εĵ�����

	if(t_g[satge_inedx] == 0)		//��E3000����
	{	
		m_open = 0;	//Ĭ��ִ�йص�
		//����·������7�ι���ʱ��Ŀ���״̬
		for(j = 0;j < 8;j++)
		{
			//���뱾�ι���ʱ��Ƚ�
			if(j == satge_inedx)
			{
				continue;
			}
			if(t_g[j] == 0)	//��E3000����
			{
			    //�����2�������п��Ʋ�������ִ�йص�  ���п��������������
				if( abs(t_s[j]-t_e[satge_inedx]) < 2)
				{
					m_open = 1;
					break;
				}
			}							
		}
		if(m_open == 0)	//ִ�йص�
		{
			E3000_Relay_Ctl(CLOSE);		//�ر�E3000���м̵���
			if(g_Concentrator_Para.e3000_relay_enable[0] != 0xFF)	//��·����
			{															
				g_Concentrator_Para.e3000_relay_enable[0] = 0;
			}
						
			//�ر���չ���м̵���
			for(m = 0;m < MAX_EXT_MODULE_NUM;m++)
			{
				for(n = 0;n < 4;n++)
				{
					g_extmodule_relay[m][n] = 0;
					if(g_Concentrator_Para.ext_relay_enable[m][n] != 0xFF)	//��չ�̵�������
					{
						g_Concentrator_Para.ext_relay_enable[m][n] = 0;
					}								
				}
			}
			
			g_send_concentrator_parameter = 1;  //20S���ϱ�����
			
			send_loop_debuf_inf0_to_PC();
		}
	}
	else				//�Ե�������
	{
		m_group = t_g[satge_inedx];
		m_g = t_g[satge_inedx];
		for(m = 0;m < 16;m++)
		{							
			if( ((m_g>>m)&0x01) == 1 )
			{
				for(j = 0;j < 8;j++)
				{
					//���뱾�ι���ʱ��Ƚ�
					if(j == satge_inedx)
					{
						continue;
					}
					if(t_g[j] != 0) //�Ե�������
					{
						m_other_g = t_g[j];
						//����õ�������2�������п��Ʋ�������ִ�йص�
						if(((m_other_g>>m)&0x01) == 1)
						{
							if( abs(t_s[j]-t_e[satge_inedx]) < 2)
							{
								m_group &= (~(1<<m));	//������0������鲻ִ�йص�
								break;
							}
						}
					}
				}
			}
		}
		send_plc_cmd_bdp(addr,m_group,contents);
//////////////////////////////////////////////////	
		send_lamp_debuf_inf0_to_PC(addr,m_group,contents);		
	}
	
}

//��ʾ����ʱ��
void show_working_time(void)
{
	unsigned char i = 0;
	
	if(m_working_time_num >= 5)	//����4�ι���ʱ�䣬20Sѭ����ʾ
	{
		m_show_working_time_couter++;
		if(m_show_working_time_couter >= 10)	//20Sѭ��
		{
			if(m_show_working_time_flag == 1)	//��ʾǰ4�ι���ʱ��
			{
				for(i = 0;i < 4;i++)
				{
					Show_Light_Time(i,i);
				}
				m_show_working_time_flag = 0;
			}
			else	//��ʾ��4�ι���ʱ��
			{
				for(i = 4;i < 8;i++)
				{
					Show_Light_Time(i-4,i);
				}
				m_show_working_time_flag = 1;
			}
			m_show_working_time_couter = 0;
		}
	}
	else	//С��4�ι���ʱ��,ֻ��ʾ1��
	{
		if(m_show_working_time_flag == 1)
		{
			for(i = 0;i < 4;i++)
			{
				Show_Light_Time(i,i);
			}
			
			m_show_working_time_flag = 0;
		}
	}
}
//��֤����ʱ���Ƿ���Ч
unsigned char check_time_valid(unsigned short s_time,unsigned short e_time)
{
	if( (s_time == 0)&&(e_time == 0) )
	{
		return 0;
	}
	else if( (s_time == 0xFFFF)||(e_time == 0xFFFF) )
	{
		return 0;
	}
	return 1;
}

//��ȡ��������ѹ������Ϣ
void get_power_info(void)
{
	unsigned char i = 0;
	unsigned short m_amp[3] = {0};
	unsigned short m_vol[3] = {0};
	unsigned char buf[16] = {0};
	unsigned char len = 0;
	
	Read_PhasePower_Info();//��ȡ��ѹ����ֵ
	for(i = 0; i < 3;i++)
	{
		m_amp[i] = g_Phase_Amp[i];
		m_vol[i] = g_Phase_Voltage[i];
		
		g_Phase_Voltage[i] = G_PhasePower_Info[i].Ux * 10 * g_Voltage_Adjust_Ratio[i]/1000;
		g_Phase_Amp[i] = (G_PhasePower_Info[i].Ix / 10) * g_Current_Adjust_Ratio[i]/1000;  //IX�ķֱ���Ϊ*100	
		//���ΰ�����С��0.1A����
		if(g_Phase_Amp[i] <= 10)
		{
			g_Phase_Amp[i] = 0;
		}
		
		g_Concentrator_Para.Voltage[i] = g_Phase_Voltage[i];
		g_Concentrator_Para.e3000_current[i] = g_Phase_Amp[i];
	}
	for(i = 0; i < 3;i++)
	{
		//�����仯����1%���ұ仯ֵ����1A���ϴ���������ͬʱ���½�����ʾ
		//�������ӷ�0ֵ��Ϊ0ֵʱ�����½�����ʾ
		if( (((m_amp[i]*LARGE_CURRENT_VARIATION_RATIO) <= abs(m_amp[i]-g_Phase_Amp[i]))
			&&(CURRENT_VARIATION_THRESHOLD < abs(m_amp[i]-g_Phase_Amp[i])))
			||((m_amp[i] != 0)&&(g_Phase_Amp[i] == 0)) )
		{		
			len = data_conversion(buf,g_Phase_Amp[i],DECIMAL2);
			show_text(DGUS_STATE_SHOW_ADDR,16+i,buf,len);
		}
		//��ѹ�仯ֵ����5V�����½�����ʾ
		if(VOLTAGE_VARIATION_THRESHOLD <= abs(m_vol[i]-g_Phase_Voltage[i]) )
		{			
			len = data_conversion(buf,g_Phase_Voltage[i],DECIMAL1);
			show_text(DGUS_STATE_SHOW_ADDR,13+i,buf,len);
		}
	}
}

unsigned char m_extmodule_relay[3][4];
//����·״̬
void judge_Loop_status(void)
{
   unsigned char m = 0, n = 0;
	
	//�жϼ������̵���״̬�仯���ɿ�������λ8302
	if(m_e3000_relay_status != g_Concentrator_Para.e3000_relay_enable[0])
	{
		m_e3000_relay_status = g_Concentrator_Para.e3000_relay_enable[0];
		//�صƺ�λ8302
		if(g_Concentrator_Para.e3000_relay_enable[0] == 0)
		{
			g_reset_E3000_8302_flag = 1;
			g_reset_ext_8302_flag = 1;		
		}
	}
   //�ж���չ�̵���״̬�仯����1�̵����ɿ�������λ8302

	for(m = 0;m < MAX_EXT_MODULE_NUM;m++)
	{
			for(n = 0;n < 4;n++)
			{
				if(m_extmodule_relay[m][n] != g_extmodule_relay[m][n])	//��Ӧ��·״̬�仯
				{
					m_extmodule_relay[m][n] = g_extmodule_relay[m][n];
					if(g_extmodule_relay[m][n] == 0)	//�ɿ�����
					{
						g_reset_E3000_8302_flag = 1;
						g_reset_ext_8302_flag = 1;				
					}
				}								
			}
	} 
}

/////////////////////////////////////////////////////
unsigned char m_rtc_time_check_times = 0;	//RTCʱ����֤����
unsigned char m_rtc_time_correct_counter = 0;//RTCʱ����ȷ����
unsigned char m_rtc_sec = 0;	//RTC���ݴ�
//��ȡ����֤RTCʱ��
void Check_RTC_Time(void)
{
	//��ȡRTCʱ��
	Get_RTC_Time();	

	//�ȶ����βɼ��������ݣ����1��2SΪ����
	if( (abs(m_rtc_sec - g_rtc_sec) <= 3)&&(m_rtc_sec != g_rtc_sec) )
	{
		m_rtc_time_correct_counter++;
	}
	m_rtc_sec = g_rtc_sec;
	m_rtc_time_check_times++;
	
	//ÿ�ֽ���10�αȽϣ�����ʱ�ӹ����ж�
	if(m_rtc_time_check_times >= 10)
	{
		//�����������ڵ���8�Σ����ж�ʱ���������������
		if(m_rtc_time_correct_counter >= 8)
		{
			g_rtc_time_fault = 0;
		}
		else
		{
			g_rtc_time_fault = 1;
		}
		
		m_rtc_time_check_times = 0;
		m_rtc_time_correct_counter = 0;
	}
}

unsigned char m_ts_time_check_times = 0;	//TSʱ����֤����
unsigned char m_ts_time_correct_counter = 0;//TSʱ����ȷ����
unsigned char m_ts_sec = 0;	//TS���ݴ�
//��ȡ����֤TSʱ��
void Check_TS_Time(void)
{		
	Get_TS_Time();
	
	//�ȶ����βɼ��������ݣ����1��2SΪ����
	if( (abs(m_ts_sec - g_ts_sec) <= 3)&&(m_ts_sec != g_ts_sec) )
	{
		m_ts_time_correct_counter++;
	}
	m_ts_sec = g_ts_sec;
	m_ts_time_check_times++;
	//ÿ�ֽ���10�αȽϣ�����ʱ�ӹ����ж�
	if(m_ts_time_check_times >= 10)
	{
		//�����������ڵ���8�Σ����ж�ʱ���������������
		if(m_ts_time_correct_counter >= 8)
		{
			g_ts_time_fault = 0;
		}
		else
		{
			g_ts_time_fault = 1;
		}
		
		m_ts_time_check_times = 0;
		m_ts_time_correct_counter = 0;
	}
}
/////////////////////////////////////////////////////

