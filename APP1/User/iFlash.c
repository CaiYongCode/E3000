#include "iFlash.h"


/********************************************************************************************************** 
** Function name:      FLASH_ReadHalfWord
** Descriptions:       ��ȡָ����ַ�İ�������
** input parameters:   u32 addr��Ҫ���ĵ�ַ
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
u16 FLASH_ReadHalfWord(u32 addr)
{
	return *(vu16*)addr; 
}

/********************************************************************************************************** 
** Function name:      FLASH_Write
** Descriptions:       Flashд
** input parameters:   u32 Addr����ʼ��ַ
                       u16 *p �� Ҫд�����ݣ�����Ϊ��λ��
                       u16 Num�� Ҫд�İ�����Ŀ 
** output parameters:  ��
** Returned value:     ��
**********************************************************************************************************/
void FLASH_HALFWORD_Write(u32 Addr,u16 *p,u16 Num)
{	
	u16 i;
	uint16_t* RamSource;
	RamSource = p;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(Addr);
	for(i = 0;i < Num;i++)
	{
		FLASH_ProgramHalfWord(Addr,*(uint16_t*)RamSource);
	  
		if((*(uint16_t *)Addr != *(uint16_t *)RamSource))
		{
			return;
		}
		Addr += 2;
		RamSource += 1;
	} 
	FLASH_Lock();
}

////////////////////////////////////////////////////////////////////
void ReadData_From_Flash(uint32_t address,uint8_t num,uint16_t *Data)
{
	uint16_t *PthFlash;
	uint8_t count;
	PthFlash=(uint16_t *)(address );
	for(count=0;count<num;count++)
	{
		*(Data+count)=*(PthFlash+count);
  }
}
