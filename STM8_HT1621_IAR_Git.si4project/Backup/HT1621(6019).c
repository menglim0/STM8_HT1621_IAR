/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "stm8s.h"
#include "stm8s_gpio.h"

//#include "STM8S103F.h"


#include "HT1621.h"

#define SYS_DIS    	0x00  //�ر�ϵͳ������LCDƫѹ������
#define SYS_EN   		0x02  //��ϵͳ����
#define LED_OFF  		0x04  //�ر�LCDƫѹ
#define LED_ON   		0x06  //��LCDƫѹ
//#define TIME_DIS 		0x08  //ʱ�����ʧЧ
#define WDT_DIS  		0x0A  //WDT�����־ʧЧ
//#define TIMER_EN 		0x0C  //ʱ�����ʹ��
//#define WDT_EN   		0x07  //WDT�����־�����Ч 
//#define TONE_OFF 		0x10  //�ر��������
//#define TONE_ON  		0x12  //���������
//#define CLR_TIMER		0x18  //ʱ������������
//#define CLR_WDT     0x1C  //���WDT״̬
//#define XTAL_32K    0x28  //ϵͳʱ��Դ������
#define RC_256K     0x30  //ϵͳʱ��Դ Ƭ��RC����
#define BIAS_COM 		0x52  //1/2ƫѹ 4��COM
//#define TONE_4K     0x80  //����Ƶ��4KHz 
//#define TONE_2K     0xC0  //����Ƶ��2KHz
//#define 

//_Bool CS @PD_ODR:6;   //Ƭѡ
//_Bool WR @PD_ODR:5;   //дʹ��
//_Bool DAT @PD_ODR:4; //���ݿ�


/***
**CS  PD6
**WR  PD5
**DAT  PD4
**/

#define CS_Port GPIOD
#define CS_Pin GPIO_PIN_6

#define WR_Port GPIOD
#define WR_Pin GPIO_PIN_5

#define DAT_Port GPIOD
#define DAT_Pin GPIO_PIN_4


#define CS_High() GPIO_WriteHigh(CS_Port, CS_Pin)
#define CS_Low() GPIO_WriteLow(CS_Port, CS_Pin)

#define WR_High() GPIO_WriteHigh(WR_Port, WR_Pin)
#define WR_Low() GPIO_WriteLow(WR_Port, WR_Pin)

#define DAT_High() GPIO_WriteHigh(DAT_Port, DAT_Pin)
#define DAT_Low() GPIO_WriteLow(DAT_Port, DAT_Pin)

//
//_Bool TEST_PORT @PA_ODR:3; //���ڲ���


void Port_Init(void)
{       
        //����LED�Ĺܽ�Ϊ���ģʽ
	GPIO_Init(CS_Port, CS_Pin, GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(WR_Port, WR_Pin, GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(DAT_Port, DAT_Pin, GPIO_MODE_OUT_PP_HIGH_FAST );	
        //GPIO_Init(LED_PORT, LED_4, GPIO_MODE_OUT_PP_HIGH_FAST );	
}


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


void delayNop(unsigned int n)
{
	//unsigned int i;
	unsigned int j;
	
	
	 for(j=0;j<=n;j++) 	
         {
           asm("nop");
         }

}



void HT1621B_SendBit(unsigned char data,unsigned char cnt) //һλ���ݸ�λ��ǰ
{
	unsigned char i;
  for(i=0;i<cnt;i++)
  {
	  if((data&0x80) == 0) 
	  	{
	  	DAT_Low();
	  	}	  
		else
		{
		DAT_High();
		}
		WR_Low();
		delayNop(1);
		WR_High();   //����һ��������
		data<<=1;   //����λ����
                delayNop(1);
	}
}

void HT1621B_SendDataBit(unsigned char data,unsigned char bit) //һλ���ݵ�λ��ǰ
{
	unsigned char i;
  for(i=0;i<bit;i++)
  {
	  if((data&0x01) == 0) DAT_Low();
		else DAT_High();
		WR_Low();
		delayNop(1);
		WR_High();   //����һ��������
		data>>=1;   //����λ����
                delayNop(1);
	}  
}

void HT1621B_SendCmd(unsigned char command)
{
  CS_Low();
	HT1621B_SendBit(0x80,4); //CMDģʽ
	HT1621B_SendBit(command,8);
	CS_High();
}


void HT1621B_WriteData(unsigned char addr,unsigned char data) //����д
{
	CS_Low(); //дģʽCS����Ϊ�͵�ƽ
  	HT1621B_SendBit(0xa0,3); //WRITEģʽ
	addr<<=2;
	HT1621B_SendBit(addr,6); //д6λ��ַ,��ѡ
	HT1621B_SendDataBit(data,4); //д4λ����,λѡ
	CS_High(); 
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
	CS_High();
	WR_High();;
	DAT_High();
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




