/**
  ******************************************************************************
  * @file    :hostcomm_man.c
  * @author  :DatLe
  * @version :
  * @date    :
  * @brief   :
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "hostcomm_man.h"
#include "function.h"
#include "debug.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */
RF_DataTypeDef RFData;

/** @defgroup Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup Private_Variables
  * @{
  */

/**
  * @}
  */ 


/** @defgroup FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup Private_Functions
  * @{
  */
  
void v_HostComm_ManInit(void)
{
  /* Register call back */
  v_HostComm_RegisterCallback(EVENT_SCAN_MASTER,&v_HostComm_ScanMasterCb);
  v_HostComm_RegisterCallback(EVENT_REQUEST_ID,&v_HostComm_SetIdRequestCb);
  v_HostComm_RegisterCallback(EVENT_SET_ID,&v_HostComm_SetIdCb);
  v_HostComm_RegisterCallback(EVENT_SET_ID_OK,&v_HostComm_SetIdOkCb);
  v_HostComm_RegisterCallback(EVENT_SYNC_DATABASE,&v_HostComm_SyncDatabaseCb);
  
  v_HostComm_RegisterCallback(EVENT_CONTROL_REMOTE_TOGGLE,&v_HostComm_RemoteControlToggleCb);
  v_HostComm_RegisterCallback(EVENT_UPDATE_CHANNEL,&v_HostComm_REControlChannelCb);
  v_HostComm_RegisterCallback(EVENT_CONTROL_SCRIPT,&v_HostComm_ControlScriptCb);
}

void v_HostComm_ManProc(void)
{
  /* Call host comm core */
  v_HostComm_Get();
}




/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
