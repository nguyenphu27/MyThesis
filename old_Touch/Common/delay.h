/**
  ******************************************************************************
  * @file     delay.h
  * @author:  DatLe
  * @version: v1_00
  * @date:    
  * @brief:   Header file for delay.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   
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
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(__IO uint32_t Delay);
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
