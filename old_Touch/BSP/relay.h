/**
  ******************************************************************************
  * @file    relay.h
  * @author  Dai Ve
  * @version V1.00
  * @date    23-May-2013
  * @brief   Header file for relay.c module.
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
#ifndef __RELAY_H
#define __RELAY_H

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
  //#define RELAY_ON_LOW_LEVEL
  #define RELAY_OFF_LOW_LEVEL
    	 
	#define RELAY1_PIN 									      GPIO_Pin_13
  #define RELAY1_GPIO_PORT									GPIOB
  #define RELAY1_GPIO_CLK 				          RCC_APB2Periph_GPIOB
  
  #define RELAY2_PIN 									      GPIO_Pin_12
  #define RELAY2_GPIO_PORT									GPIOB
  #define RELAY2_GPIO_CLK 				          RCC_APB2Periph_GPIOB
  
  #define RELAY3_PIN 									      GPIO_Pin_11
  #define RELAY3_GPIO_PORT									GPIOB
  #define RELAY3_GPIO_CLK 				          RCC_APB2Periph_GPIOB
  
  #define RELAY4_PIN 									      GPIO_Pin_10
  #define RELAY4_GPIO_PORT									GPIOB
  #define RELAY4_GPIO_CLK 				          RCC_APB2Periph_GPIOB

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
  RELAY1 = 0,
  RELAY2,
  RELAY3,
  RELAY4,
  
  RELAY_NUM
}Relay_TypeDef;

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
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Functions
  * @{
  */
		void RELAY_Init (void);
    uint8_t RELAY_GetStatus(Relay_TypeDef Relay);
    void RELAY_SetStatus(Relay_TypeDef Relay,uint8_t RelayStatus);
		void RELAY_Off(Relay_TypeDef Relay);
		void RELAY_On(Relay_TypeDef Relay);
		void RELAY_Toggle(Relay_TypeDef Relay);
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* __RELAY_H */

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
