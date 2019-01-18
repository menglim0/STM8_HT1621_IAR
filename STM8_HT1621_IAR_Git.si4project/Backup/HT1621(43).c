/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "stm8s.h"
//#include "STM8S103F.h"


#include "HT1621.h"

#define SYS_DIS    	0x00  //关闭系统振荡器和LCD偏压发生器
#define SYS_EN   		0x02  //打开系统振荡器
#define LED_OFF  		0x04  //关闭LCD偏压
#define LED_ON   		0x06  //打开LCD偏压
//#define TIME_DIS 		0x08  //时基输出失效
#define WDT_DIS  		0x0A  //WDT溢出标志失效
//#define TIMER_EN 		0x0C  //时基输出使能
//#define WDT_EN   		0x07  //WDT溢出标志输出有效 
//#define TONE_OFF 		0x10  //关闭声音输出
//#define TONE_ON  		0x12  //打开声音输出
//#define CLR_TIMER		0x18  //时基发生器清零
//#define CLR_WDT     0x1C  //清除WDT状态
//#define XTAL_32K    0x28  //系统时钟源，晶振
#define RC_256K     0x30  //系统时钟源 片内RC振荡器
#define BIAS_COM 		0x52  //1/2偏压 4个COM
//#define TONE_4K     0x80  //声音频率4KHz 
//#define TONE_2K     0xC0  //声音频率2KHz
//#define 

//_Bool CS @PD_ODR:6;   //片选
//_Bool WR @PD_ODR:5;   //写使能
//_Bool DAT @PD_ODR:4; //数据口
#define CS_Port GPIOD
#define CS_Pin GPIO_PIN_6

#define WR_Port GPIOD
#define WR_Pin GPIO_PIN_5

#define DAT_Port GPIOD
#define DAT_Pin GPIO_PIN_5


#define CS_High() GPIO_WriteHigh(CS_Port, CS_Pin)
#define CS_Low() GPIO_WriteLow(CS_Port, CS_Pin)

#define WR_High() GPIO_WriteHigh(WR_Port, WR_Pin)
#define WR_Low() GPIO_WriteLow(WR_Port, WR_Pin)

#define DAT_High() GPIO_WriteHigh(DAT_Port, DAT_Pin)
#define DAT_Low() GPIO_WriteLow(DAT_Port, DAT_Pin)

//
//_Bool TEST_PORT @PA_ODR:3; //用于测试





void delayNMS(unsigned int n)
{
	unsigned int i;
	unsigned int j;
	
	for(i=0;i<n;i++)
	{
	 for(j=0;j<20;j++) 	
         {
           asm("nop");
         }
	}
}

void HT1621B_SendBit(unsigned char data,unsigned char cnt) //一位数据高位在前
{
	unsigned char i;
  for(i=0;i<cnt;i++)
  {
	  if((data&0x80) == 0) DAT = 0;
		else DAT = 1;
		WR = 0;
		
		WR = 1;    //产生一个上升沿
		data<<=1;   //数据位左移
	}
}

void HT1621B_SendDataBit(unsigned char data,unsigned char bit) //一位数据低位在前
{
	unsigned char i;
  for(i=0;i<bit;i++)
  {
	  if((data&0x01) == 0) DAT = 0;
		else DAT = 1;
		WR = 0;
		
		WR = 1;    //产生一个上升沿
		data>>=1;   //数据位左移
	}  
}

void HT1621B_SendCmd(unsigned char command)
{
  CS = 0;
	HT1621B_SendBit(0x80,4); //CMD模式
	HT1621B_SendBit(command,8);
	CS = 1;
}


void HT1621B_WriteData(unsigned char addr,unsigned char data) //连续写
{
	CS = 0; //写模式CS需置为低电平
  	HT1621B_SendBit(0xa0,3); //WRITE模式
	addr<<=2;
	HT1621B_SendBit(addr,6); //写6位地址,段选
	HT1621B_SendDataBit(data,4); //写4位数据,位选
	CS = 1; 
}

                         // seg           1              2              4                 8
void DisplayNum(unsigned char *SegX,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
	unsigned char i,bit=0; 
	for(i=0;i<8;i++)
  {
		if(SegX[a]&(1<<i)) bit+=1;
		if((SegX[b]+0x80)&(1<<i)) bit+=2;	
		if(SegX[c]&(1<<i)) bit+=4;
		if((SegX[d]+0x80)&(1<<i)) bit+=8;	
	    HT1621B_WriteData(i,bit);
	    bit=0;		
	}
}

void HT1621B_Init(void)
{
	unsigned char i;
	CS = 1;
	WR = 1;
	DAT = 1;
	delayNMS(100);
  	HT1621B_SendCmd(BIAS_COM);
	HT1621B_SendCmd(RC_256K);
	HT1621B_SendCmd(SYS_DIS);
	HT1621B_SendCmd(WDT_DIS);
	HT1621B_SendCmd(SYS_EN);
	HT1621B_SendCmd(LED_ON);
	for(i=0;i<=8;i++)
        HT1621B_WriteData(i,0x00);
}




