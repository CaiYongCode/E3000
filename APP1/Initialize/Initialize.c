#include "Initialize.h"
#include "const.h"
#include "IOctl.h"
#include "PowerManager.h"

TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;
TIM_OCInitTypeDef        TIM4_OCInitStructure;
TIM_BDTRInitTypeDef      TIM4_BDTRInitStructure;

//#define RTCClockSource_LSI   /* Use the internal 32 KHz oscillator as RTC clock source */
#define RTCClockSource_LSE   /* Use the external 32.768 KHz oscillator as RTC clock source */
/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  USART1_Putchar((u8) ch);
  return ch;
}
int fgetc(FILE *f)
{
  /* Loop until received a char */
  while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET));
  /* Read a character from the USART and RETURN */
  return (USART_ReceiveData(USART1));
}
/*
********************************************************************************
** �������� �� RCC_Configuration(void)
** �������� �� ʱ�ӳ�ʼ��
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
void RCC_Configuration(void)
{   
	ErrorStatus HSEStartUpStatus;
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_1);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* PLLCLK = 8MHz * 10 = 80 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_10);
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource()!= 0x08);
  }  
}

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);  //PB13/14/15����


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                  //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		              //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                      //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	  //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);                                   //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	SPI2_ReadWriteByte(0xff);//��������	 
} 

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable SPI1 GPIOA clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	/* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//RN8302 CS1
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//NotSelect_RN8302();
	
	/* SPI1 configuration */ 
	/* SPI1 configuration */ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);   
}
/*******************************************************************************
* Function Name  : SPI_SetSpeed
* Description    : SPI1�����ٶ�
* Input          : u8 Speed 
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_SetSpeed(u8 Speed)
{
 
}
/*******************************************************************************
* Function Name  : SPI2_SetSpeed
* Description    : SPI2�����ٶ�
* Input          : u8 Speed 
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_SetSpeed(u8 Speed)
{
  
}

void USART1_Configuration(u32 BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
//USART_ClockInitTypeDef USART_ClockInitStruct;
  /* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
 }


void USART2_Configuration(u32 BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
//USART_ClockInitTypeDef USART_ClockInitStruct;
  /* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USART2 */
  USART_Init(USART2, &USART_InitStructure);
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
 // USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);
//	USART_DeInit(USART2);
 }

void USART3_Configuration(u32 BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
//USART_ClockInitTypeDef USART_ClockInitStruct;
  /* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USART3 */
  USART_Init(USART3, &USART_InitStructure);
  /* Enable USART3 Receive and Transmit interrupts */
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
 // USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
  /* Enable the USART3 */
  USART_Cmd(USART3, ENABLE);
//	USART_DeInit(USART3);
 }


void UART4_Configuration(u32 BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
//USART_ClockInitTypeDef USART_ClockInitStruct;
  /* USART4 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USART2 */
  USART_Init(UART4, &USART_InitStructure);
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
  /* Enable the USART2 */
  USART_Cmd(UART4, ENABLE);
//	USART_DeInit(USART2);
 }

void UART5_Configuration(u32 BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
//USART_ClockInitTypeDef USART_ClockInitStruct;
  /* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USART2 */
  USART_Init(UART5, &USART_InitStructure);
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
 // USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  /* Enable the USART2 */
  USART_Cmd(UART5, ENABLE);
//	USART_DeInit(USART2);
 }

/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
  /* Reset Backup Domain */
  BKP_DeInit();
  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
#ifdef RTCClockOutput_Enable  
  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                               functionality must be disabled */                             
  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCCalibrationClockOutputCmd(ENABLE);
#endif 
  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Enable the RTC Second */  
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32769); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}
/******************************************************************************************************
****GPIO��ʼ��
******************************************************************************************************/
 void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE); 
	
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4|RCC_APB1Periph_UART5 ,  ENABLE );//
 /**************************GPIOC/AD******************************/
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	

	
	  
// 	  /**************************GPIOA/ USART1******************************/	

	/* Configure USART1 Tx (PA9)as alternate function push-pull                */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           	//USART1 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		   		 	//A�˿� 
	
	/* Configure USART1 Rx (PA10) as input floating                             */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	//USART1 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//���ÿ�©����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
// 	  /**************************GPIOA/ USART2******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         	//USART2 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		//�����������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);		    		//A�˿� 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	//USART2 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //���ÿ�©����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


// 	  /**************************GPIOB/ USART3******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         //USART3 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);		    //B�˿� 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         //USART3 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //���ÿ�©����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	

// 	  /**************************GPIOB/ USART4******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         //USART4 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);		    //B�˿� 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         //USART4 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //���ÿ�©����
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	

// 	  /**************************GPIOB/ USART5******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	         //USART5 TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);		    //B�˿� 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         //USART5 RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //���ÿ�©����
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
}
/*
********************************************************************************
** �������� �� TIM1_Configuration(void)
** �������� �� ��ʱ��1��ʼ��
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
void TIM1_Configuration(void)
{
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE );
   /* Time Base configuration */
  TIM_DeInit(TIM1);
  TIM_TimeBaseStructure.TIM_Prescaler = 79;                   //����Ԥ��Ƶ����Ƶϵ��71����APB2=72M, TIM1_CLK=72/72=1MHz ��
                                                              //����ȡֵ������0x0000��0xFFFF֮��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ����0�ǵ�����ֵ�����
  TIM_TimeBaseStructure.TIM_Period =10000;	                   //10ms��ʱ,����ȡֵ������0x0000��0xFFFF֮��
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;			        //�����˶�ʱ��ʱ�ӷָ
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;		      //���������ڼ�����ֵ������ȡֵ������0x00��0xFF֮��
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);	            // ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);      //���жϣ�����һ�����жϺ����������ж�   
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); //ʹ��TIM1�ж�Դ 
   
  TIM_Cmd(TIM1, ENABLE); 		             //TIM1�ܿ��أ�����
 
}

void TIM2_Configuration(void)
{
		/* PWM�źŵ�ƽ����ֵ */  
  u16 CCR3= 36000;          
  u16 CCR4= 36000;  
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
	 
  /*GPIOA Configuration: TIM2 channel 3 and 4 as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // ����������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
	/* Time base configuration */                                            
	TIM_TimeBaseStructure.TIM_Period =PWM_Counter;  
	TIM_TimeBaseStructure.TIM_Prescaler = 2;                                    //����Ԥ��Ƶ��Ԥ��Ƶ=2����Ϊ72/3=24MHz  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //����ʱ�ӷ�Ƶϵ��������Ƶ  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //���ϼ������ģʽ  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  
	/* PWM1 Mode configuration: Channel3 */  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //����ΪPWMģʽ1  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
	TIM_OCInitStructure.TIM_Pulse = CCR3;                                       //��������ֵ�������������������ֵʱ����ƽ��������  
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //����ʱ������ֵС��CCR3ʱΪ�ߵ�ƽ  
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);                                    //ʹ��ͨ��3      
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	/* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	TIM_OCInitStructure.TIM_Pulse = CCR4;                                       //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                    //����ʱ������ֵС��CCR4ʱΪ�͵�ƽ 
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);                                    //ʹ��ͨ��4  
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_ARRPreloadConfig(TIM2, ENABLE);                                         //ʹ��TIM2���ؼĴ���ARR  
	/* TIM2 enable counter */  
	TIM_Cmd(TIM2, ENABLE);                                                      //ʹ��TIM2   
}
//PWM
void TIM_PWM_Init(void)
{  
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
  /* TIM4 Peripheral Configuration */ 
  TIM_DeInit(TIM4);
  /* Time Base configuration */
  TIM4_TimeBaseStructure.TIM_Prescaler = 1;	
  TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM4_TimeBaseStructure.TIM_Period = 0xffff;
  TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
  TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);
  /* Channel 1 Configuration in PWM mode */
  TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM4_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;                  
  TIM4_OCInitStructure.TIM_Pulse = 655*50; 
  TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
  TIM4_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;         
  TIM4_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM4_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;          
  TIM_OC1Init(TIM4,&TIM4_OCInitStructure); 
  /* Automatic Output enable, Break, dead time and lock configuration*/
  TIM4_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM4_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM4_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1; 
  TIM4_BDTRInitStructure.TIM_DeadTime = 0x75;
  TIM4_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
  TIM4_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM4_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(TIM4,&TIM4_BDTRInitStructure);
  /* TIM4 counter enable */
  TIM_Cmd(TIM4,ENABLE);
  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM4,ENABLE);
  /* NVIC configuration */
}

void NVIC_Configuration()
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//   /* Enable the EXTI2 Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
	
	
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        	//��Ӧ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	
//   /* Enable the USART2 Interrupt */

   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�1
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          	//��Ӧ���ȼ�1
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure); 


//   	/* Enable the UART3 Interrupt */ 
   	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
   	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//��Ӧ���ȼ�2
   	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
   	NVIC_Init(&NVIC_InitStructure);


 	/* Enable the UART4 Interrupt */ 
   	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn; 	
   	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//��Ӧ���ȼ�3
   	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
   	NVIC_Init(&NVIC_InitStructure);

	
 	/* Enable the UART5 Interrupt */ 
   	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn; 			
   	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//��Ӧ���ȼ�0
   	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
   	NVIC_Init(&NVIC_InitStructure);

   /* Enable the TIM1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;          //�����¼� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�2 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //��Ӧ���ȼ�1 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //�����ж� 
  NVIC_Init(&NVIC_InitStructure);  //д������

	 
}
u8 SPI1_ReadWriteByte(u8 byte)
{
  /* Send byte through the SPI2 peripheral */
  SPI1->DR=byte;
  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Return the byte read from the SPI bus */
  return SPI1->DR;
}
u8 SPI2_ReadWriteByte(u8 byte)
{
  /* Send byte through the SPI2 peripheral */
  SPI2->DR=byte;
  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  /* Return the byte read from the SPI bus */
  return SPI2->DR;
}

void Init_RN8302_XiaoBiao(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC ,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;     //D�����?��2a???��        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��?��-��?��?
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;       //D�����??��?��?      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
    GPIO_SetBits(GPIOC, GPIO_Pin_12); 	
}



u8 MCU_Init(void)//��Ƭ����ʼ��
{
// 	SCB->VTOR = FLASH_BASE | 0x10000; /* Vector Table Relocation in Internal FLASH. */
	RCC_Configuration(); 
	GPIO_Configuration();
	Init_GPIO_config();
	Init_powermanager_pin_config();
	
// 	RTC_Configuration();
	USART1_Configuration(115200);
	USART2_Configuration(9600);
	USART3_Configuration(9600);
    UART4_Configuration(9600);
	UART5_Configuration(9600);

	SPI1_Init();
	
 	TIM1_Configuration();
//    EXTI_Configuration();  
	NVIC_Configuration();
	return 0;
}

