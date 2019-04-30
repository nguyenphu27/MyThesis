/**
  ******************************************************************************
  * @file   :hostcomm_man.h
  * @author : DAI VE
  * @version: v1_00 
  * @date   :  2015
  * @brief  :Header file for .c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __hostcomm_man_H
#define __hostcomm_man_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   #include "common.h" 
   #include "HostComm.h"
   #include "led.h"

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

void v_HostComm_ManInit(void);
void v_HostComm_ManProc(void);

void v_HostComm_ScanMasterCb(uint8_t *u8_data, uint16_t u16_len);
void v_HostComm_SetIdRequestCb(uint8_t* u8_data,uint16_t u16_len);
void v_HostComm_SetIdCb(uint8_t* u8_data,uint16_t u16_len);
void v_HostComm_SetIdOkCb(uint8_t* u8_data,uint16_t u16_len);
void v_HostComm_RFCheckingConnectionCb(uint8_t* u8_data,uint16_t u16_len);
void v_HostComm_SyncDatabaseCb(uint8_t* u8_data,uint16_t u16_len);

void v_HostComm_REControlChannelCb(uint8_t* u8_data,uint16_t u16_len);
void v_HostComm_ControlScriptCb(uint8_t* u8_data,uint16_t u16_len);
void v_HostComm_SBControlScriptCb(uint8_t* u8_data,uint16_t u16_len);
void v_HostComm_RemoteControlToggleCb(uint8_t* u8_data,uint16_t u16_len);
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
