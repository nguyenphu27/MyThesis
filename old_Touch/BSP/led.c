/**
  ******************************************************************************
  * @file    led.c
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
#include "led.h"

/** @addtogroup Utilities
  * @{
  */ 
  
/** @defgroup STM32_EVAL 
  * @brief This file provides firmware functions to manage Leds, push-buttons, 
  *        COM ports and low level HW resources initialization of the different 
  *        modules available on STM32 Evaluation Boards from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32_EVAL_Abstraction_Layer
  * @{
  */ 
  


/** @defgroup STM32_EVAL_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Variables
  * @{
  */
GPIO_TypeDef* LED_PORT[] =     { LED1_GPIO_PORT, 
                                 LED2_GPIO_PORT, 
                                 LED3_GPIO_PORT,
                                 LED4_GPIO_PORT,
                                 LEDBL_GPIO_PORT};

const uint16_t LED_PIN[] =     { LED1_PIN, 
                                 LED2_PIN, 
                                 LED3_PIN,
                                 LED4_PIN,
                                 LEDBL_PIN};
const uint32_t  LED_CLK[]  = { LED1_GPIO_CLK,
                               LED2_GPIO_CLK,
                               LED3_GPIO_CLK,
                               LED4_GPIO_CLK,
                               LEDBL_GPIO_CLK};
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Functions
  * @{
  */ 
	/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/

                               /**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */
void LED_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t i;
	for(i=0;i<3;i++) //(sizeof(LED_PIN)/sizeof(LED_PIN[0]))
  {
    /* Enable the GPIO_LED Clock */
    RCC_APB2PeriphClockCmd(LED_CLK[i], ENABLE);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = LED_PIN[i] ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_PORT[i], &GPIO_InitStructure);
    LED_Off((Led_TypeDef)i);
  }
}

void LED_On(Led_TypeDef Led)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  /* Config LED as Output */
  GPIO_InitStructure.GPIO_Pin = LED_PIN[Led] ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LED_PORT[Led], &GPIO_InitStructure);
  #ifdef LED_ON_LOW_LEVEL
    GPIO_ResetBits(LED_PORT[Led],LED_PIN[Led] );
  #else
    GPIO_SetBits(LED_PORT[Led],LED_PIN[Led] );
  #endif
}

void LED_Off(Led_TypeDef Led)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Config LED as Output */
  GPIO_InitStructure.GPIO_Pin = LED_PIN[Led] ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LED_PORT[Led], &GPIO_InitStructure);
  #ifdef LED_ON_LOW_LEVEL
    GPIO_SetBits(LED_PORT[Led],LED_PIN[Led] );
  #else
    GPIO_ResetBits(LED_PORT[Led],LED_PIN[Led] );
  #endif
}

void LED_Toggle(Led_TypeDef Led)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Config LED as Output */
  GPIO_InitStructure.GPIO_Pin = LED_PIN[Led] ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LED_PORT[Led], &GPIO_InitStructure);
  if(GPIO_ReadOutputDataBit(LED_PORT[Led],LED_PIN[Led]))
  {
    GPIO_ResetBits(LED_PORT[Led],LED_PIN[Led] );
  }
  else
  {
    GPIO_SetBits(LED_PORT[Led],LED_PIN[Led] );
  };
}

void LED_Blur(Led_TypeDef Led)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    /* Config LED as input */
    GPIO_InitStructure.GPIO_Pin = LED_PIN[Led] ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(LED_PORT[Led], &GPIO_InitStructure);
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
