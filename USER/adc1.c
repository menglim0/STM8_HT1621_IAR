
/*************** (C) COPYRIGHT  EW工作室 ***************************************
 * 文件名  ：adc1.c
 * 描述    ：ADC配置函数    
 * 实验平台：EW STM8入门板 V1.0
 * 库版本  ：V2.1.0
 * QQ群	   ：261825684
 * 修改时间：2013-08-20
*******************************************************************************/

/* 包含系统头文件 */

/* 包含自定义头文件 */
#include "adc1.h"

/* 自定义新类型 */

/* 自定义宏 */

/* 自定义变量 */

/*实现函数部分*/

/*******************************************************************************
 * 名称: void ADC_Init(void)
 * 功能: ADC初始化
 * 形参: 无
 * 返回: 无
 * 说明: 无

 ******************************************************************************/




void ADC_Init(void)
{
    /**< 连续转换模式 */
    /**< 使能通道 */
    /**< ADC时钟：fADC2 = fcpu/18 */
    /**< 这里设置了从TIM TRGO 启动转换，但实际是没有用到的*/
    /**  不使能 ADC2_ExtTriggerState**/
    /**< 转换数据左对齐 */
    /**< 不使能通道10的斯密特触发器 */
    /**  不使能通道10的斯密特触发器状态 */
    ADC1_DeInit();
    ADC1_Cmd(ENABLE);
    //ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_3, ADC1_ALIGN_LEFT);
    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D2, \
      ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);

    ADC1_Cmd(ENABLE);
    ADC1_StartConversion();


  
}






/******************* (C) COPYRIGHT EW工作室 *****END OF FILE******************/