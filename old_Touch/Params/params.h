/**
  ******************************************************************************
  * @file   :params.h
  * @author :DatLe
  * @version:v1_00 
  * @date   :09/07/2015   
  * @brief  :Header file for params.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PARAMS_H
#define __PARAMS_H

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
 
/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
typedef enum
{
  /* System infor params */
  PARAMS_ID_SYSTEM_FW_VERSION=0,
  PARAMS_ID_SYSTEM_HW_VERSION,
  PARAMS_ID_SYSTEM_RF_CODE,
  PARAMS_ID_SYSTEM_TOUCH_CODE,
  PARAMS_ID_SYSTEM_DATE_RELEASE,
  PARAMS_ID_SYSTEM_MANUFACTURING_CODE,
  PARAMS_ID_SYSTEM_RESERVED,

  /* Device infor params */
  PARAMS_ID_DEVICE_ID,
  PARAMS_ID_DEVICE_TOTAL,
  PARAMS_ID_DEVICE_NAME,
  PARAMS_ID_DEVICE_SUBNAME,
  PARAMS_ID_DEVICE_RESERVED,
  
  /* Device configuration */
  /* RF config */
  PARAMS_ID_RF_EN,
  PARAMS_ID_RF_LEN_TYPE,
  PARAMS_ID_RF_LEN,
  PARAMS_ID_RF_POWER,
  PARAMS_ID_RF_REPEATER,
  PARAMS_ID_RF_INTERVAL,
  PARAMS_ID_RF_RESERVED,
  
  /* Touch config */
  PARAMS_ID_TOUCH_EN,
  PARAMS_ID_TOUCH_AUTO_CALIBRE,
  PARAMS_ID_TOUCH_QUEUE,
  PARAMS_ID_TOUCH_SAMPLING,
  PARAMS_ID_TOUCH_,
  PARAMS_ID_TOUCH_ENABLE1,
  PARAMS_ID_TOUCH_ENABLE2,
  PARAMS_ID_TOUCH_ENABLE3,
  PARAMS_ID_TOUCH_ENABLE4,
  PARAMS_ID_TOUCH_ENABLE5,
  PARAMS_ID_TOUCH_ENABLE6,
  PARAMS_ID_TOUCH_ENABLE7,
  PARAMS_ID_TOUCH_ENABLE8,
 
  /* License and active/inactive */
  PARAMS_ID_RESET_FACTORY_NUM,
  PARAMS_ID_LICENSE_NUM,
  
  
  /* CRC and checksum */
  PARAMS_ID_CHECKSUM,
  
  PARAMS_ID_NUM_OF_ITEMS
} ENM_PARAMS_ITEMS_T;
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


uint8_t u8_PARAMS_ID_Set(ENM_PARAMS_ITEMS_T stru_id,uint8_t *u8_data);
uint8_t u8_PARAMS_ID_Get(ENM_PARAMS_ITEMS_T stru_id,uint8_t *u8_data);
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
