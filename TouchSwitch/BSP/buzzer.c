/**
  ******************************************************************************
  * @file    buzzer.c
  * @author  DatLe
  * @version V1.00
  * @date    23-May-2013
  * @brief   STM32xx-EVAL abstraction layer. 
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
#include "buzzer.h"
#include "led.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
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
//extern Setting_TypeDef Setting;  
static Buzzer_TypeDef Buzzer;

/**
  * @}
  */ 


/** @defgroup FunctionPrototypes
  * @{
  */
static void PWM_LL_Config(void); 
static uint8_t BuzzerOn=1;
/**
  * @}
  */ 


/** @defgroup Private_Functions
  * @{
  */ 
void Set_BuzzerOn(void)
{
  BuzzerOn=1;
}
void Set_BuzzerOff(void)
{
  BuzzerOn=0;
}
                              
/**
  * @brief  Configures Buzzer GPIO and load default value.
  * @param   
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */
void Buzzer_Init (void)
{
  /* Enable the GPIO_BUZZER Clock */
  RCC_APB2PeriphClockCmd(BUZZER_GPIO_CLK, ENABLE);

  PWM_LL_Config();
  Buzzer_Off();
  
  Buzzer.Enable = false;
	Buzzer.Counter = 0;
	Buzzer.BipTimeTrue = BUZZER_BIP_TIME_TRUE;
  Buzzer.BipTimeFalse = BUZZER_BIP_TIME_FALSE;
  Buzzer.BipTime = Buzzer.BipTimeTrue;
}

/**
  * @brief : LL config for PWM Buzzer
  * @param : none  
  * @retval : none
  */
static void PWM_LL_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  uint16_t PrescalerValue = 0;
  
  RCC_APB2PeriphClockCmd(BUZZER_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  /* DIMMER_PIN Configuration: Channel 1, 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
  
  /* TIM3 Full remapping pins */
  //GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); 
  
  /*PWM config base TIMER2: Using TIM3_CHANNEL3, TIM3_CH4 channel*/
  /* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
    clock at 24 MHz the Prescaler is computed as following:
     - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 24 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                  = 2 MHz / BUZZER_FREQ = ??? KHz
  ----------------------------------------------------------------------- */
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 2000000) - 1;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = BUZZER_FREQ;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief : Turn ON buzzer 
  * @param : none  
  * @retval : none
  */
static void Buzzer_On(void)
{
  TIM_SetCompare3(TIM3,BUZZER_FREQ/2);
}

/**
  * @brief : Turn OFF buzzer
  * @param : none  
  * @retval : none
  */
void Buzzer_Off(void)
{
  TIM_SetCompare3(TIM3,0);
}

/**
  * @brief : Set buzzer bip true time
  * @param : time to set (in 10ms unit)  
  * @retval : none
  */
void Buzzer_SetBipTimeTrue(uint16_t time)
{
  Buzzer.BipTimeTrue= time;
}

/**
  * @brief : Set buzzer bip false time
  * @param : time to set (in 10ms unit)  
  * @retval : none
  */
void Buzzer_SetBipTimeFalse(uint16_t time)
{
  Buzzer.BipTimeFalse= time;
}

/**
  * @brief : Bip the buzzer with duration time is bip true time
  * @param : none  
  * @retval : none
  */
void Buzzer_BipTrue(void)
{
  if(BuzzerOn)
  {
    Buzzer.Enable= true;
    Buzzer.Counter= 0;
    Buzzer.BipTime= Buzzer.BipTimeTrue;
    Buzzer_On(); 
  }
}

/**
  * @brief : Bip the buzzer with duration time is bip false time
  * @param : none  
  * @retval : none
  */
void Buzzer_BipFalse(void)
{
    Buzzer.Enable= true;
    Buzzer.Counter= 0;
    Buzzer.BipTime= Buzzer.BipTimeFalse;
    Buzzer_On();
}


/**
  * @brief : Bip the buzzer with duration time is bip true default time
  * @param : none  
  * @retval : none
  */
void Buzzer_BipTrueDF(void)
{
  Buzzer.Enable= true;
  Buzzer.Counter= 0;
  Buzzer.BipTime= BUZZER_BIP_TIME_TRUE;
  Buzzer_On();
}

/**
  * @brief : Bip the buzzer with duration time is bip false default time
  * @param : none  
  * @retval : none
  */
void Buzzer_BipFasleDF(void)
{
  Buzzer.Enable= true;
  Buzzer.Counter= 0;
  Buzzer.BipTime= BUZZER_BIP_TIME_FALSE;
  Buzzer_On();
}


/**
  * @brief : Buzzer ISR handle
  * @param : none  
  * @retval : none
  */
void Buzzer_ISR(void)
{
  if(Buzzer.Enable)
	{
		Buzzer.Counter++;
		if(Buzzer.Counter>Buzzer.BipTime)
		{
			Buzzer.Enable=false;
			Buzzer.Counter=0;
			Buzzer_Off();
		}
	}
}


/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 


/**
  * @}
  */ 
    
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
