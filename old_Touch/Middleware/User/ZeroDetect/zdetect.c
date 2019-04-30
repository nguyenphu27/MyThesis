/**
  ******************************************************************************
  * @file    :zdetect.c
  * @author  :DatLe
  * @version :v1_00
  * @date    :27/11/2014
  * @brief   :zero detect functions
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "zdetect.h"
#include "function.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */

/** @defgroup Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup Private_Variables
  * @{
  */
ZeroDetect_TypeDef ZeroDetect;
Error_TypeDef ZeroError;
/**
  * @}
  */ 
void ZeroEXTI_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
		/* Enable AFIO clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	
	  /* Configure the EN pin */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  /* Connect EXTIx Line to INT pin */
  GPIO_EXTILineConfig(ZEROEXTI_PORT, ZEROEXTI_PORT_PIN	);

  /* Configure EXTIx line */			
  EXTI_InitStructure.EXTI_Line = ZEROEXTI_PORT_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* Enable and set INT Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = ZEROEXTI_PORT_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0E;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void ZDetect_test()
{
	EXTI_GenerateSWInterrupt(ZEROEXTI_PORT_LINE);
}

/** @defgroup FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */
  
void ZDetect_Init(void)
{
  //ADC_ZeroDetectInit();
	ZeroEXTI_Init();
  //ADC_Config();
  /* Load delay value */
  
  /* Load default data for module */
  ZeroDetect.DelayTimeOn=ZDETECT_DELAY_ON_DF;
  ZeroDetect.DelayTimeOff=ZDETECT_DELAY_OFF_DF;
  
}

void Zdetect_Handler()
{
	Zero_UpdateLedState(GPIO_ReadInputDataBit(ZEROEXTI_GPIO_PORT, ZEROEXTI_PORT_PIN));
}
  
/**
  * @brief : control relay by zero detect
  * @param : none  
  * @retval : none
  */
void Zdetect_On(Relay_TypeDef Relay)
{
  /* Need to run zerodetect function */
  ZeroDetect.NeedAction[Relay]=1;
  ZeroDetect.Enable=1;
  /* Action is on */
  ZeroDetect.StatusAction[Relay]=1; 
}
void Zdetect_Off(Relay_TypeDef Relay)
{
  /* Need to run zerodetect function */
  ZeroDetect.NeedAction[Relay]=1;
  ZeroDetect.Enable=1;
  /* Action is off */
  ZeroDetect.StatusAction[Relay]=0;
}
/**
  * @brief : Zerodetect handle task
  * @param : none  
  * @retval : none
  */  
void Zdetect_Task(void)
{
  #define ZERO_DETECT_TO                  200000//10000
  #define ZERO_DETECT_ERROR_THRESHOLD     2
  #define ZERO_DETECT_OFFSET_HIGH         3300
  #define ZERO_DETECT_OFFSET_LOW          700

  uint8_t haveOn;
  uint8_t haveOff;
  uint8_t i;
  uint16_t temp;
  uint32_t cnt=0;
  if(ZeroDetect.Enable)
  {
    temp=ADC_ZeroDetectRead();
    while((temp<ZERO_DETECT_OFFSET_HIGH)&&(cnt<ZERO_DETECT_TO))
    {
      temp=ADC_ZeroDetectRead();
      cnt++;
    }
    if(cnt>=ZERO_DETECT_TO)
    {
      ZeroError.Cnt++;
      if(ZeroError.Cnt>ZERO_DETECT_ERROR_THRESHOLD)
      {
        ZeroError.Cnt=ZERO_DETECT_ERROR_THRESHOLD+1;
        ZeroError.Error=true;
        ZeroError.ErrorFlag=true;
      }
    }
    else
    {
      ZeroError.Cnt=0;
      ZeroError.Error=false;
      ZeroError.ErrorFlag=false;
    }
    cnt=0;
    while((temp>ZERO_DETECT_OFFSET_LOW)&&(cnt<ZERO_DETECT_TO))
    {
      temp=ADC_ZeroDetectRead();
      cnt++;
    }
    cnt=0;
    haveOff=0;
    haveOn=0;
    for(i=0;i<RELAY_NUM;i++)
    {
      if((ZeroDetect.NeedAction[i])&&(ZeroDetect.StatusAction[i]))
      {
        haveOn=1;
      };
      if((ZeroDetect.NeedAction[i])&&(!ZeroDetect.StatusAction[i]))
      {
        haveOff=1;
      };
    }
    if(haveOn&&haveOff)
    {
      if(ZeroDetect.DelayTimeOff<ZeroDetect.DelayTimeOn)/*if delay off is smaller than delay on*/
      {
        Zdetect_DelayOff();
        for(i=0;i<RELAY_NUM;i++)
        {
          if(!ZeroDetect.StatusAction[i])
          {
            RELAY_Off((Relay_TypeDef)i);
            ZeroDetect.NeedAction[i]=0;
          };
        }
        Zdetect_DelayOnOff();//delay on time - off time
        for(i=0;i<RELAY_NUM;i++)
        {
          if(ZeroDetect.StatusAction[i])
          {
            RELAY_On((Relay_TypeDef)i);
            ZeroDetect.NeedAction[i]=0;
          };
        }  
      }
      else
      {
        Zdetect_DelayOn();
        for(i=0;i<RELAY_NUM;i++)
        {
          if(ZeroDetect.StatusAction[i])
          {
            RELAY_On((Relay_TypeDef)i);
            ZeroDetect.NeedAction[i]=0;
          };
        }
        Zdetect_DelayOnOff();//delay off time - on time
        for(i=0;i<RELAY_NUM;i++)
        {
          if(!ZeroDetect.StatusAction[i])
          {
            RELAY_Off((Relay_TypeDef)i);
            ZeroDetect.NeedAction[i]=0;
          };
        }
      }
    }
    else if(haveOn&&!haveOff)
    {
      Zdetect_DelayOn();
      for(i=0;i<RELAY_NUM;i++)
      {
        if(ZeroDetect.StatusAction[i])
        {
          RELAY_On((Relay_TypeDef)i);
          ZeroDetect.NeedAction[i]=0;
        };
      }
    }
    else if(!haveOn&&haveOff)
    {
      Zdetect_DelayOff();
      for(i=0;i<RELAY_NUM;i++)
      {
        if(!ZeroDetect.StatusAction[i])
        {
          RELAY_Off((Relay_TypeDef)i);
          ZeroDetect.NeedAction[i]=0;
        };
      }
    }
    else{;};
    ZeroDetect.Enable=0;  
  }
}

/**
  * @brief : Delay function for zerodetect 
  * @param : none  
  * @retval : none
  */
void Zdetect_Delay(uint32_t time)
{
  __IO uint32_t i;
  for(i=0;i<time;i++)
  {;}
}
void Zdetect_DelayOn(void)
{
  Zdetect_Delay(ZeroDetect.DelayTimeOn);
}

void Zdetect_DelayOff(void)
{
  Zdetect_Delay(ZeroDetect.DelayTimeOff);
}

void Zdetect_DelayOnOff(void)
{
  Zdetect_Delay(MAX(ZeroDetect.DelayTimeOn,ZeroDetect.DelayTimeOff)-MIN(ZeroDetect.DelayTimeOn,ZeroDetect.DelayTimeOff));
}

/** @defgroup Private_Functions
  * @{
  */ 
                                 
    
/******************* (C) COPYRIGHT 2014 ACIS *****END OF FILE****/
