#include "interpol.h"
#include "typedef.h"


//#include "stm32f10x.h"
//#include "stm32lib.h"

//u16 FuelLevel_Reading_AD_InputValue_PullDown[13]={1170,1328,1643,1894,2086,2217,2359,2458,2533,2633,2684,2731,2900}; //pull down 100 ohm
//u16 FuelLevel_Reading_AD_InputValue_PullUp[13]={2730,1328,1643,1894,2086,2217,2359,2458,2533,2633,2684,2731,2900}; //pull down 100 ohm
//u16 FuelLevel_Reading_Percent_OutputValue[14]={255,255,242,222,198,163,129,92,59,39,20,10,0,0};
//u16 FuelLevel_Reading_AD_InputValue[14]={3300,2926,2768,2453,2202,2010,1879,1737,1638,1563,1463,1412,1365,1000};
u16 FuelLevel_ControlPoint_Filter_Raw[7]  = {744,731,719,682,625,546,399};
u16 FuelLevel_ControlPoint_Percent[7]     = {0,5,10,25,50,75,100};


#define Full_Level 744
#define Empty_Level 399

u16 Interpol_Percent_OutputValue, Sum_Total,Level_Range,Current_Range;
int interpol_x1,interpol_x2,interpol_y1,interpol_y2;

	int y;
	int Temp_ratio;
	int Temp_ratio1;
	int Temp_ratio2;
	int Temp_ratio3;

enum FaultState
{ NoFault,ShortLow,ShortHigh,Otherfault };

enum FaultState Fuellevel_FaultState;
char i_index;
int Interpol_FindPoint(int x)
{

	
	//char interpol_index;
	
	
	//u16 Interpol_Percent_OutputValue, Sum_Total,Level_Range,Current_Range;
	
	Level_Range = Full_Level-Empty_Level;

	Current_Range = 744 - x;

	
	
	
	
	if(x>744)
	{
          if(x>800)
          {
	Fuellevel_FaultState = ShortHigh;
          }
          else
        {
          Fuellevel_FaultState = NoFault;
        }
        Interpol_Percent_OutputValue =0;
	}
	else if(x<399)
	{
	  if(x<300)
          {
	Fuellevel_FaultState = ShortLow;
          }
        else
        {
          Fuellevel_FaultState = NoFault;
        }
        Interpol_Percent_OutputValue = 100;
	}
	else
	{
		
		Fuellevel_FaultState = NoFault;
			
		for(i_index=0;i_index<7;i_index++)
		{
			if(FuelLevel_ControlPoint_Filter_Raw[i_index]==x)
			{
				Interpol_Percent_OutputValue=FuelLevel_ControlPoint_Percent[i_index];
				return Interpol_Percent_OutputValue;
			}
                      	else if(x<=FuelLevel_ControlPoint_Filter_Raw[i_index]&&x>FuelLevel_ControlPoint_Filter_Raw[i_index+1])
			{
				interpol_x1 = FuelLevel_ControlPoint_Filter_Raw[i_index];
				interpol_x2 = FuelLevel_ControlPoint_Filter_Raw[i_index+1];
				interpol_y1 =FuelLevel_ControlPoint_Percent[i_index];
				interpol_y2 =FuelLevel_ControlPoint_Percent[i_index+1];
				Interpol_Percent_OutputValue = Interpol_Calculate(interpol_x1,interpol_y1,interpol_x2,interpol_y2, x);
				return Interpol_Percent_OutputValue;
			}
			
		}
		
	}

	
	//Interpol_Percent_OutputValue = Current_Range*100/Level_Range;
	
	
		return Interpol_Percent_OutputValue;

}

int Interpol_Calculate(int x1,int y1,int x2,int y2, int x)
{

	Temp_ratio1=y2-y1;
	Temp_ratio2=x1-x2;
	Temp_ratio3=x-x2;
	Temp_ratio=(Temp_ratio1*Temp_ratio3)/Temp_ratio2;
	//Temp_ratio = 0;
	y=y2-(int)Temp_ratio;
	return y;
}


char Get_FuelLevelFaule_State()
{return Fuellevel_FaultState;}
