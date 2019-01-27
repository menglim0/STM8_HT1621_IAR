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
u16 u16_adc1_value,u16_adc1_value_filter; 
u16 u16_adc1_value_old,DisplayFuelLevel_Unfilter,DisplayFuelLevel,DisplayFuelLevel_Percent,DisplayFuelLevel_Percent_old; 

u16 DisplayFuelLevel_Hund,DisplayFuelLevel_Dec,DisplayFuelLevel_Unit;
/* display data addre and data*/

#define DisplayFuelLevel_Data_Seg0 0
#define DisplayFuelLevel_Data_Seg1 1
#define DisplayFuelLevel_Data_Seg2 2
#define DisplayFuelLevel_Data_Seg3 3




unsigned char Percent_data_Data_Seg0[10]={0x0F,0x00,0x0B,0x09,0x04,0x0D,0x0F,0x08,0x0F,0x0D};
unsigned char Percent_data_Data_Seg1[10]={0x0A,0x0A,0x0C,0x0E,0x0E,0x06,0x06,0x0A,0x0E,0x0E};


#define DisplayFuelLevel_Data_Seg4 4
#define DisplayFuelLevel_Data_Seg5 5
#define DisplayFuelLevel_Data_Seg6 6
#define DisplayFuelLevel_Data_Seg7 7
#define DisplayFuelLevel_Data_Seg8 8

unsigned char DisplayFuelLevel_Data_Seg_Percent[5]={4,5,6,7,8};
unsigned char DisplayFuelLevel_Data_Data_Percent[4]={1,3,7,15};
unsigned char DisplayFuelLevel_Data_Data_Percent_N[5]={15,7,3,1};


unsigned char Percent_Data_Index,Percent_Seg_Index;

unsigned char Percent_Data_Index_AutoTest;


uint8_t ADC_Update_index;
u8 LCD_Display_Addr,LCD_Display_Data,LCD_Display_Data_Bit,LCD_Display_Data_Index;

u8 LCD_Display_Percent,LCD_Display_Percent_Add,LCD_Display_Percent_Sub,LCD_Display_Percent_Addr,LCD_Display_Percent_Data,LCD_Display_Percent_Data_Off;

unsigned char Percent_data_Addr[21]={9,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8};
unsigned char Percent_data_Addr_New[5]={4,5,6,7,8};
//

unsigned char Percent_data_Data[21]={13,8,12,14,15,1,3,7,15,8,12,14,15, 1,3,7,15,8,12,14,15};
unsigned char Percent_data_Data_Off[21]={4,8, 4, 2,1,1,2,4 ,8,8,4,2 ,1, 1, 2, 4, 8,8,4,2,1};

unsigned char Percent_Set_Data[21][5]={{0x00,0x00,0x00,0x00,0x00},
                                         {0x08,0x00,0x00,0x00,0x00},
                                         {0x0C,0x00,0x00,0x00,0x00},            
                                         {0x0E,0x00,0x00,0x00,0x00},
                                         {0x0F,0x00,0x00,0x00,0x00}, 
                                         {0x0F,0x01,0x00,0x00,0x00},
                                         {0x0F,0x03,0x00,0x00,0x00},                                         
                                         {0x0F,0x07,0x00,0x00,0x00}, 
                                         {0x0F,0x0F,0x00,0x00,0x00},
                                         {0x0F,0x0F,0x08,0x00,0x00}, 
                                         {0x0F,0x0F,0x0C,0x00,0x00},
                                         {0x0F,0x0F,0x0E,0x00,0x00}, 
                                         {0x0F,0x0F,0x0F,0x00,0x00},
                                         {0x0F,0x0F,0x0F,0x01,0x00}, 
                                         {0x0F,0x0F,0x0F,0x03,0x00},
                                         {0x0F,0x0F,0x0F,0x07,0x00},
                                         {0x0F,0x0F,0x0F,0x0F,0x00},
                                         {0x0F,0x0F,0x0F,0x0F,0x08}, 
                                         {0x0F,0x0F,0x0F,0x0F,0x0C},
                                         {0x0F,0x0F,0x0F,0x0F,0x0E},
                                         {0x0F,0x0F,0x0F,0x0F,0x0F}};

unsigned char Percent_Set_Data_Display[4];

unsigned char Percent_data_Data_Off_result;

#define Fliter_Length 100
uint16_t filter(uint8_t Length,uint16_t ADC_Result);
void DisplayFuelLevel_Percent_Func(uint8_t dat);


void Delay(unsigned int time)
{
	volatile unsigned int i;
	while(time--){
		i=100;
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
  HT1621B_WriteData(0x09,0x0D);
     // HT1621B_WriteData(0x09,0x09);
  while (1)
  {
    /*放置你的代码*/
       
  
        Delay(2000);
		u16_adc1_value_old=u16_adc1_value;
        u16_adc1_value = ADC1_GetConversionValue();

		u16_adc1_value_filter=filter(Fliter_Length,u16_adc1_value);
 
        ADC_Update_index++;
        
        if(ADC_Update_index>=32)
        {
          ADC_Update_index=0;
        }
    
        DisplayFuelLevel_Unfilter = FuelLevel_Percent_Calc(u16_adc1_value_filter,ADC_Update_index);

		
		DisplayFuelLevel=filter(Fliter_Length,DisplayFuelLevel_Unfilter);

		

		/* dispaly the percent fule by Data*/
        
		DisplayFuelLevel_Percent = DisplayFuelLevel/5+1;
        DisplayFuelLevel_Hund=DisplayFuelLevel/100;
		DisplayFuelLevel_Dec=DisplayFuelLevel%100/10;
		DisplayFuelLevel_Unit=DisplayFuelLevel%10;
		
//if(Fuellevel_FaultState==1)
//else if(Fuellevel_FaultState==2)
//else
{
		 HT1621B_WriteData(DisplayFuelLevel_Data_Seg0,Percent_data_Data_Seg0[DisplayFuelLevel_Dec]);
		 HT1621B_WriteData(DisplayFuelLevel_Data_Seg1,Percent_data_Data_Seg1[DisplayFuelLevel_Dec]|DisplayFuelLevel_Hund);
		 HT1621B_WriteData(DisplayFuelLevel_Data_Seg2,Percent_data_Data_Seg0[DisplayFuelLevel_Unit]);
                 HT1621B_WriteData(DisplayFuelLevel_Data_Seg3,Percent_data_Data_Seg1[DisplayFuelLevel_Unit]|0x01);
}
/*
for(LCD_Display_Data_Index=0;LCD_Display_Data_Index<10;LCD_Display_Data_Index++)
{
		HT1621B_WriteData(DisplayFuelLevel_Data_Seg0,Percent_data_Data_Seg0[LCD_Display_Data_Index]);
		 HT1621B_WriteData(DisplayFuelLevel_Data_Seg1,Percent_data_Data_Seg1[LCD_Display_Data_Index]);
		  HT1621B_WriteData(DisplayFuelLevel_Data_Seg2,Percent_data_Data_Seg0[LCD_Display_Data_Index]);
		   HT1621B_WriteData(DisplayFuelLevel_Data_Seg3,Percent_data_Data_Seg1[LCD_Display_Data_Index]);
                     Delay(3000);
}*/
		/*Test code for LCD display

		
			for(LCD_Display_Data=0;LCD_Display_Data<=4;LCD_Display_Data++)
				{
                                  for(LCD_Display_Addr=0;LCD_Display_Addr<10;LCD_Display_Addr++)
			{
                              
                                LCD_Display_Data_Bit =1<<LCD_Display_Data;
                                HT1621B_WriteData(LCD_Display_Addr,LCD_Display_Data_Bit);
                                Delay(1000);
				}
			}
  		*/
        //LCD_Display_Percent_Addr=0;
        //LCD_Display_Percent_Data=0;
        if(DisplayFuelLevel<=2)
        {
          
           HT1621B_WriteData(0x09,0x09);
            Delay(10);
	}
        else if(DisplayFuelLevel<=5)
        {
           HT1621B_WriteData(0x09,0x0D);
           HT1621B_WriteData(Percent_data_Addr_New[0],Percent_Set_Data[0][0]);
        }
       else if(DisplayFuelLevel_Percent_old!=DisplayFuelLevel_Percent)
        {
                        /*Percent_Seg_Index=(DisplayFuelLevel_Percent-1)/5;
                        Percent_Data_Index=(DisplayFuelLevel_Percent-1)/4;

                        HT1621B_WriteData(DisplayFuelLevel_Data_Seg_Percent[Percent_Seg_Index],DisplayFuelLevel_Data_Data_Percent[Percent_Data_Index]);
                        

                
                        for(LCD_Display_Percent=0;LCD_Display_Percent<21;LCD_Display_Percent++)
                        {
                          

                            for(LCD_Display_Percent_Sub=DisplayFuelLevel_Percent;LCD_Display_Percent_Sub<21;LCD_Display_Percent_Sub++)    
                                {
                                  
                                  Percent_data_Data_Off_result=Percent_data_Data[LCD_Display_Percent_Sub]&(~Percent_data_Data_Off[LCD_Display_Percent_Sub]);
                                   HT1621B_WriteData(Percent_data_Addr[LCD_Display_Percent_Sub],Percent_data_Data_Off_result);
                                   //   HT1621B_WriteData(Percent_data_Addr[LCD_Display_Percent_Sub],0);                                      
                                }
                            
                                for(LCD_Display_Percent_Add=0;LCD_Display_Percent_Add<DisplayFuelLevel_Percent;LCD_Display_Percent_Add++)
                                {
                                  
                                    HT1621B_WriteData(Percent_data_Addr[LCD_Display_Percent_Add],Percent_data_Data[LCD_Display_Percent_Add]);
                                    Delay(1);
                                }
                                
                                
                                
                              
                        }*/
                        // for(LCD_Display_Percent=0;LCD_Display_Percent<DisplayFuelLevel_Percent;LCD_Display_Percent++)
                        {
                          
                            for(Percent_Data_Index_AutoTest=0;Percent_Data_Index_AutoTest<5;Percent_Data_Index_AutoTest++)
                           	{
                               //    HT1621B_WriteData(Percent_data_Addr_New[Percent_Data_Index_AutoTest],0x00);
                                   //   HT1621B_WriteData(Percent_data_Addr[LCD_Display_Percent_Sub],0);              

                           	}
                           for(Percent_Data_Index_AutoTest=0;Percent_Data_Index_AutoTest<5;Percent_Data_Index_AutoTest++)
                           	
                                   HT1621B_WriteData(Percent_data_Addr_New[Percent_Data_Index_AutoTest],Percent_Set_Data[DisplayFuelLevel_Percent -1][Percent_Data_Index_AutoTest]);
                                   //   HT1621B_WriteData(Percent_data_Addr[LCD_Display_Percent_Sub],0);              

                           	}
                    
                                
                                
                                
                              
                        }
                        
                        
                        
        }
        //Percent_Data_Index_AutoTest++;
        //DisplayFuelLevel_Percent_Func(Percent_Data_Index_AutoTest);

  //DisplayFuelLevel_Percent_old=DisplayFuelLevel_Percent;



  }


  

void DisplayFuelLevel_Percent_Func(uint8_t dat)
{

     
      HT1621B_WriteData(Percent_data_Addr[1],Percent_Set_Data_Display[0]);
}
  uint16_t filter(uint8_t Length,uint16_t ADC_Result)
  {
  unsigned int sum = 0;
  unsigned char i;
  
  for (i=0;i<Length;i++)
  {
  sum += ADC_Result;
    }
  return(uint16_t)(sum/Length);
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
