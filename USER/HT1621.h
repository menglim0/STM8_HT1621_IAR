#ifndef __HT1621_H
#define __HT1621_H


void HT1621B_SendBit(unsigned char data,unsigned char cnt);
void HT1621B_SendDataBit(unsigned char data,unsigned char bit);
void HT1621B_SendCmd(unsigned char command);
void HT1621B_WriteData(unsigned char addr,unsigned char data);
void DisplayNum(unsigned char *SegX,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
void HT1621B_Init(void);
void delayNMS(unsigned int n);






#endif


