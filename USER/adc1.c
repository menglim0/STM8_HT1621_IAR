
/*************** (C) COPYRIGHT  EW������ ***************************************
 * �ļ���  ��adc1.c
 * ����    ��ADC���ú���    
 * ʵ��ƽ̨��EW STM8���Ű� V1.0
 * ��汾  ��V2.1.0
 * QQȺ	   ��261825684
 * �޸�ʱ�䣺2013-08-20
*******************************************************************************/

/* ����ϵͳͷ�ļ� */

/* �����Զ���ͷ�ļ� */
#include "adc1.h"

/* �Զ��������� */

/* �Զ���� */

/* �Զ������ */

/*ʵ�ֺ�������*/

/*******************************************************************************
 * ����: void ADC_Init(void)
 * ����: ADC��ʼ��
 * �β�: ��
 * ����: ��
 * ˵��: ��

 ******************************************************************************/




void ADC_Init(void)
{
    /**< ����ת��ģʽ */
    /**< ʹ��ͨ�� */
    /**< ADCʱ�ӣ�fADC2 = fcpu/18 */
    /**< ���������˴�TIM TRGO ����ת������ʵ����û���õ���*/
    /**  ��ʹ�� ADC2_ExtTriggerState**/
    /**< ת����������� */
    /**< ��ʹ��ͨ��10��˹���ش����� */
    /**  ��ʹ��ͨ��10��˹���ش�����״̬ */
    ADC1_DeInit();
    ADC1_Cmd(ENABLE);
    //ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_3, ADC1_ALIGN_LEFT);
    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D2, \
      ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);

    ADC1_Cmd(ENABLE);
    ADC1_StartConversion();


  
}






/******************* (C) COPYRIGHT EW������ *****END OF FILE******************/