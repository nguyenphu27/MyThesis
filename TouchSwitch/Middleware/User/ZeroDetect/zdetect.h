/**
  ******************************************************************************
  * @file    :zdetect.h
  * @author  :DatLe
  * @version :v1_00 
  * @date    :27/11/2014    
  * @brief   :Header file for zdetect.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ZDETECT_H
#define __ZDETECT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   #include "common.h"
   #include "adc.h" 
   #include "relay.h" 
/**
@code  
 
@endcode
*/
#define ZDETECT_DELAY_ON_DF       45700
#define ZDETECT_DELAY_OFF_DF      6500
	 
#define ZeroEXTI_IRQHandler		EXTI0_IRQHandler
#define ZEROEXTI_PORT					GPIO_PortSourceGPIOA
#define ZEROEXTI_GPIO_PORT		GPIOA
#define ZEROEXTI_PORT_PIN			GPIO_Pin_0
#define ZEROEXTI_PORT_LINE		EXTI_Line0
#define ZEROEXTI_PORT_IRQ			EXTI0_IRQn
/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
typedef struct
{
  uint8_t       Enable:1;
  uint8_t       ZeroPoint:1;
  uint8_t       NeedAction[RELAY_NUM];
  uint8_t       StatusAction[RELAY_NUM];
  
  uint32_t      DelayTimeOn;/*store delay time ON of relay*/
  uint32_t      DelayTimeOff;/*store delay time OFF of relay*/
}ZeroDetect_TypeDef;
/**
  * @}
  */ 
  
/** @defgroup Exported_Constants
  * @{
  */

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
  void ZDetect_Init(void);
  
  void Zdetect_Task(void);
  
  void Zdetect_On(Relay_TypeDef Relay);
  void Zdetect_Off(Relay_TypeDef Relay);
  
  void Zdetect_Delay(uint32_t time);
  void Zdetect_DelayOn(void);
  void Zdetect_DelayOff(void);
  void Zdetect_DelayOnOff(void);
	
	void ZDetect_test();
	void Zdetect_Handler();
	
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
