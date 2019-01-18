/*************** (C) COPYRIGHT  EW������ ***************************************
 * �ļ���  ��main.c
 * ����    ��ADCʵ�飬���ռ�⣬�����������ADת����ĵ�ѹֵ��
 * ʵ��ƽ̨��EW STM8���Ű� V1.0
 * ��汾  ��V2.1.0
 * QQȺ	   ��261825684
 * �޸�ʱ�䣺2013-08-20
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "sysclock.h"
#include "adc1.h"
#include "uart.h"
#include "HT1621.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
    u16 u16_adc1_value;  

void Delay(unsigned int time)
{
	volatile unsigned int i;
	while(time--){
		i=300;
		while(i--); 
	}
}

int main(void)
{

  /* ϵͳʱ�ӳ�ʼ�� */
  SystemClock_Init();
  ADC_Init();
  Uart2_Init();
  HT1621B_Init();
  while (1)
  {
    /*������Ĵ���*/
  HT1621B_WriteData(0x06,0x88);
  
    Delay(3000);
      u16_adc1_value=u16_adc1_value;
    u16_adc1_value = ADC1_GetConversionValue();
   // UART2_printf("adc=%d\n",u16_adc1_value);
    //printf_str("\nadc=");
    //putascShort(u16_adc1_value);

  }
  
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/******************* (C) COPYRIGHT EW������ *****END OF FILE******************/
