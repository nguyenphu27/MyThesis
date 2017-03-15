/**
  ******************************************************************************
  * @file   :function.h
  * @author :DatLe
  * @version:v1_00 
  * @date   :   
  * @brief  :Header file for function.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUNCTION_H
#define __FUNCTION_H

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
void v_Run(void);

void RF_Setup(void);
void RF_MainControl(void);
void RF_RemoteControl(void);
 
void v_SetupMenu(void);
void v_SetupID(void);
void v_SetupRemote(void);
void v_SetupScript(void);
void v_SetupReset(void);
 
void v_OtherTask(void);

void Zero_UpdateLedState(u8 new_state);

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
