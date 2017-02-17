/**
  ******************************************************************************
  * @file    led.h
  * @author  DatLe
  * @version V1.00
  * @date    23-May-2013
  * @brief   Header file for led.c module.
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/** @addtogroup hardware_define
  * @{
  */ 
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @defgroup STM32_EVAL_Abstraction_Layer
  * @{
  */
  
/** @defgroup STM32_EVAL_HARDWARE_RESOURCES
  * @{
  */
  #define LED_ON_LOW_LEVEL
  //#define LED_OFF_LOW_LEVEL
    	 
	#define LED1_PIN 									      GPIO_Pin_9
  #define LED1_GPIO_PORT									GPIOA
  #define LED1_GPIO_CLK 				          RCC_APB2Periph_GPIOA
  
  #define LED2_PIN 									      GPIO_Pin_10
  #define LED2_GPIO_PORT									GPIOA
  #define LED2_GPIO_CLK 				          RCC_APB2Periph_GPIOA
  
  #define LED3_PIN 									      GPIO_Pin_14
  #define LED3_GPIO_PORT									GPIOB
  #define LED3_GPIO_CLK 				          RCC_APB2Periph_GPIOB
  
  #define LED4_PIN 									      GPIO_Pin_15
  #define LED4_GPIO_PORT									GPIOB
  #define LED4_GPIO_CLK 				          RCC_APB2Periph_GPIOB

  #define LEDBL_PIN 									    GPIO_Pin_9
  #define LEDBL_GPIO_PORT									GPIOB
  #define LEDBL_GPIO_CLK 				          RCC_APB2Periph_GPIOB


/**
@code  
 
@endcode
*/

/**
  * @}
  */
  
/** @defgroup STM32_EVAL_Exported_Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2,
  LED3,
  LED4,
  LEDBL
}Led_TypeDef;

/**
  * @}
  */ 
  
/** @defgroup STM32_EVAL_Exported_Constants
  * @{
  */

/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Macros
  * @{
  */
#define LEDBL_OFF     LED_Off(LEDBL)
#define LEDBL_ON      LED_On(LEDBL)
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Functions
  * @{
  */
		void LED_Init (void);
		void LED_Off(Led_TypeDef Led);
		void LED_On(Led_TypeDef Led);
		void LED_Toggle(Led_TypeDef Led);
    void LED_Blur(Led_TypeDef Led);
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* __LED_H */

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
