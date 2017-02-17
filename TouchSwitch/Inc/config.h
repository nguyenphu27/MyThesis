/**
  ******************************************************************************
  * @file   :config.h
  * @author :DatLe
  * @version:v1_00 
  * @date   :   
  * @brief  :Header file for setting.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   #include "common.h" 

/**
@code  
 
@endcode
*/
typedef enum
{
  SCRIPT_A=0,
  SCRIPT_B,
  
  SCRIPT_NUM
}ScriptEnum_TypeDef; 
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

/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
