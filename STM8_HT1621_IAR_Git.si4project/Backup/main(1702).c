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
u8 LCD_Display_Addr,LCD_Display_Data,LCD_Display_Data_Bit;




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
        
        			for(LCD_Display_Data=0;LCD_Display_Data<=4;LCD_Display_Data++)
				{
                                  for(LCD_Display_Addr=0;LCD_Display_Addr<=10;LCD_Display_Addr++)
			{
			
         			HT1621B_WriteData(0x00,0x00);
                                //Delay(1000);
				}
			}

        
        ADC_Update_index++;
        
        if(ADC_Update_index>=32)
        {
          ADC_Update_index=0;
        }
    
        DisplayFuelLevel = FuelLevel_Percent_Calc(u16_adc1_value,ADC_Update_index);

		/*Test code for LCD display*/

		
			for(LCD_Display_Data=0;LCD_Display_Data<=4;LCD_Display_Data++)
				{
                                  for(LCD_Display_Addr=0;LCD_Display_Addr<10;LCD_Display_Addr++)
			{
                              
                                LCD_Display_Data_Bit =1<<LCD_Display_Data;
                                HT1621B_WriteData(LCD_Display_Addr,LCD_Display_Data_Bit);
                                Delay(1000);
				}
			}


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
