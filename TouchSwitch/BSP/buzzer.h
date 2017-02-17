/**
  ******************************************************************************
  * @file    buzzer.h
  * @author  DatLe
  * @version v2.00
  * @date    28-Aug-2014
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
#ifndef __BUZZER_H
#define __BUZZER_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   #include "stdbool.h" 
   #include "save.h"   

  /* Select some of configs here!!!!!!!!! */
    //#define BUZZER_ON_LOW_LEVEL
    #define BUZZER_OFF_LOW_LEVEL
  /* ----------------------------------------------------------------------- */
    
  #define BUZZER_PIN                              GPIO_Pin_0
  #define BUZZER_GPIO_PORT                        GPIOB
  #define BUZZER_GPIO_CLK                         RCC_APB2Periph_GPIOB
                                                
/**
@code  
 
@endcode
*/

/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
typedef struct 
{
	bool Enable;
	
	uint32_t Counter;
  uint32_t BipTime;
	uint32_t BipTimeTrue;
  uint32_t BipTimeFalse;
} Buzzer_TypeDef;

/**
  * @}
  */ 
  
/** @defgroup Exported_Constants
  * @{
  */
  #define BUZZER_BIP_TIME_TRUE								  2
  #define BUZZER_BIP_TIME_FALSE                 8

  #define BUZZER_FREQ                           450
/**
  * @}
  */ 

/** @defgroup Exported_Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup Exported_Functions
  * @{
  */
		void Buzzer_Init (void);
    
		void Buzzer_BipTrue(void);
    void Buzzer_BipFalse(void);
		void Buzzer_SetBipTimeTrue(uint16_t time);
    void Buzzer_SetBipTimeFalse(uint16_t time);
		void Buzzer_BipTrueDF(void);
    void Buzzer_BipFalseDF(void);

		 void Buzzer_Off(void);
		static void Buzzer_On(void);
		void Buzzer_ISR(void);
    void Set_BuzzerOn(void);
    void Set_BuzzerOff(void);

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
