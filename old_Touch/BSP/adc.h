/**
  ******************************************************************************
  * @file    :adc.h
  * @author  :DatLe
  * @version :v1_00 
  * @date    :27/11/2014    
  * @brief   :Header file for adc.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   
/**
@code  
 
@endcode
*/
  #define _USING_DMA__                     
   
   
  #define ZERO_DETECT_PIN 									GPIO_Pin_0
  #define ZERO_DETECT_PORT									GPIOA
  #define ZERO_DETECT_CLK_ENABLE() 	        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
   
  #define CURRENT_DETECT_PIN 							  GPIO_Pin_1
  #define CURRENT_DETECT_PORT								GPIOA
  #define CURRENT_DETECT_CLK_ENABLE() 	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup Exported_Constants
  * @{
  */
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
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
  void ADC_ZeroDetectInit(void);
  uint16_t ADC_ZeroDetectRead(void);
  uint16_t ADC_CurrentRead(void);
  
  void ADC_Config(void);
  
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* ___H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */   

/******************* (C) COPYRIGHT 2014 ACIS *****END OF FILE****/
