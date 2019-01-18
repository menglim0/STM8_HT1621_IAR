/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "FuelLevel.h"
#include "stm8s_gpio.h"
#include "HT1621.h"
#include "interpol.h"
#include "typedef.h"


//#include "STM8S103F.h"



bool Fuel_Read_1ST;

uint8_t i,IndexFilter;

uint16_t ADC_ConvertValueFilter[10],ADC_ConvertValueFilterFinalarray[10],ADC_SimpleConvertValue[1];
uint16_t FuelLevel_Percent,ADC_ConvertValueFilterFinal,ADC_ConvertValueFilterTemp,Fuel_Percent;




uint16_t FuelLevel_Percent_Calc(uint16_t ADC_Convert_Val)
{
	ADC_SimpleConvertValue[0]=ADC_Convert_Val;



	if(Fuel_Read_1ST==FALSE)
	{
		for(i=0;i<9;i++)
		{
			ADC_ConvertValueFilter[i]=ADC_SimpleConvertValue[0];
			ADC_ConvertValueFilterFinalarray[i]=ADC_SimpleConvertValue[0];
		}
		FuelLevel_Percent = ADC_SimpleConvertValue[0];
		ADC_ConvertValueFilterFinal= ADC_SimpleConvertValue[0];
		Fuel_Read_1ST = TRUE;
	}
	else
	{
		if(IndexFilter<=9)
		{
			
			IndexFilter++;
		}
		else
		{
			IndexFilter=0;
		}
		ADC_ConvertValueFilter[IndexFilter]=ADC_SimpleConvertValue[0];
		ADC_ConvertValueFilterTemp=0;
		for(i=0;i<9;i++)
		{
			ADC_ConvertValueFilterTemp= ADC_ConvertValueFilterTemp+ADC_ConvertValueFilter[i];
		}
		ADC_ConvertValueFilterTemp = ADC_ConvertValueFilterTemp/16;
	  ADC_ConvertValueFilterFinalarray[IndexFilter]=ADC_ConvertValueFilterTemp ;
		
		ADC_ConvertValueFilterTemp=0;
		for(i=0;i<9;i++)
		{
			ADC_ConvertValueFilterTemp= ADC_ConvertValueFilterTemp+ADC_ConvertValueFilterFinalarray[i];
		}
		ADC_ConvertValueFilterFinal = ADC_ConvertValueFilterTemp/16;
				
	}
	
		Fuel_Percent=Interpol_FindPoint(ADC_ConvertValueFilterFinal);
		FuelLevel_Percent =ADC_ConvertValueFilterFinal;

		return FuelLevel_Percent;


}








