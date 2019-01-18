/*************** (C) COPYRIGHT  EW工作室 ***************************************
 * 文件名  ：main.c
 * 描述    ：ADC实验，光照检测，串口输出经过AD转换后的电压值。
 * 实验平台：EW STM8入门板 V1.0
 * 库版本  ：V2.1.0
 * QQ群	   ：261825684
 * 修改时间：2013-08-20
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "sysclock.h"
#include "adc1.h"
//#include "uart.h"
#include "HT1621.h"
#include "FuelLevel.h"
#include "interpol.h"
#include "typedef.h"


/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
u16 u16_adc1_value; 
u16 u16_adc1_value_old,DisplayFuelLevel; 
uint8_t ADC_Update_index;



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

  /* 系统时钟初始化 */
  SystemClock_Init();
  Port_Init();
  ADC_Init();
  //Uart2_Init();
  HT1621B_Init();
  while (1)
  {
    /*放置你的代码*/
       
  
        Delay(1000);
	u16_adc1_value_old=u16_adc1_value;
        u16_adc1_value = ADC1_GetConversionValue();
        
        ADC_Update_index++;
        
        if(ADC_Update_index>=32)
        {
          ADC_Update_index=0;
        }
    
        DisplayFuelLevel = FuelLevel_Percent_Calc(u16_adc1_value,ADC_Update_index);
         HT1621B_WriteData(0xaa,0x07);


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


/******************* (C) COPYRIGHT EW工作室 *****END OF FILE******************/
