/**
  ******************************************************************************
  * @file   :setting.h
  * @author :DatLe
  * @version:v1_00 
  * @date   :23/06/2015   
  * @brief  :Header file for .c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SETTING_H
#define __SETTING_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "common.h" 

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
typedef enum
{
  TOUCH_ENABLE_CMD=0,
  TOUCH_RESET_CMD,
  
  
  TOUCH_TOTAL_CMD
}Touch_SettingCmdTypeDef;

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
