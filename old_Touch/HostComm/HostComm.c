/**
  ******************************************************************************
  * @file    :HostComm.c
  * @author  : DAI VE
  * @version :
  * @date    :  2015
  * @brief   :
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "HostComm.h"
#include "save.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */

extern RF_DrvTypeDef *RF;
extern Setting_TypeDef Setting;

/** @defgroup Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup Private_Defines
  * @{
  */ 

/* Get Array callback and Command */


static HostComm_CbTypeDef HOSTCOMM_CB []= { /* Diagnostic and control */
                                      {EVENT_SCAN_MASTER,CMD_SCAN_MASTER,NULL},
                                      {EVENT_REQUEST_ID,CMD_REQUEST_ID,NULL},
                                      {EVENT_SET_ID,CMD_SET_ID,NULL},
                                      {EVENT_SET_ID_OK,CMD_SET_ID_OK,NULL},
                                      {EVENT_SYNC_DATABASE,CMD_SYNC_DATABASE,NULL},
                                      {EVENT_CHECK_CONNECTION,CMD_CHECK_CONNECTION,NULL},
                                      {EVENT_GET_VERSION,CMD_GET_VERSION,NULL},
                                      {EVENT_RESET_DEVICE,CMD_RESET_DEVICE,NULL},
                                      {EVENT_RESET_TOUCH,CMD_RESET_TOUCH,NULL},
                                      /* Cmd for config */
                                      {EVENT_SET_SYNC_NAME_DEVICE,CMD_SET_SYNC_NAME_DEVICE,NULL},
                                      {EVENT_GET_SYNC_NAME_DEVICE,CMD_GET_SYNC_NAME_DEVICE,NULL},
                                      {EVENT_SET_SYNC_NAME_SCRIPT,CMD_SET_SYNC_NAME_SCRIPT,NULL},
                                      {EVENT_GET_SYNC_NAME_SCRIPT,CMD_GET_SYNC_NAME_SCRIPT,NULL},
                                      {EVENT_SET_TIME,CMD_SET_TIME,NULL},
                                      {EVENT_GET_TIME,CMD_GET_TIME,NULL},
                                      {EVENT_SET_RESET_TIME,CMD_SET_RESET_TIME,NULL},
                                      {EVENT_GET_RESET_TIME,CMD_GET_RESET_TIME,NULL},
                                      {EVENT_SET_RELAY_TIME,CMD_SET_RELAY_TIME,NULL},
                                      {EVENT_GET_RELAY_TIME,CMD_GET_RELAY_TIME,NULL},
                                      {EVENT_SET_UPDATE_TIME,CMD_SET_UPDATE_TIME,NULL},
                                      {EVENT_GET_UPDATE_TIME,CMD_GET_UPDATE_TIME,NULL},
                                      /* Cmd for config RF */
                                      {EVENT_SET_RF_POWER,CMD_SET_RF_POWER,NULL},
                                      {EVENT_GET_RF_POWER,CMD_GET_RF_POWER,NULL},
                                      {EVENT_SET_RF_REPEATER,CMD_SET_RF_REPEATER,NULL},
                                      {EVENT_GET_RF_REPEATER,CMD_GET_RF_REPEATER,NULL},
                                      {EVENT_SET_RF_CONFIG,CMD_SET_RF_CONFIG,NULL},
                                      {EVENT_GET_RF_CONFIG,CMD_GET_RF_CONFIG,NULL},
                                      /* Cmd for config touch */
                                      {EVENT_SET_TOUCH_SAMPLING,CMD_SET_TOUCH_SAMPLING,NULL},
                                      {EVENT_GET_TOUCH_SAMPLING,CMD_GET_TOUCH_SAMPLING,NULL},
                                      {EVENT_SET_BUTTON_ENABLE_DISABLE,CMD_SET_BUTTON_ENABLE_DISABLE,NULL},
                                      {EVENT_GET_BUTTON_ENABLE_DISABLE,CMD_GET_BUTTON_ENABLE_DISABLE,NULL},
                                      {EVENT_SET_HOLD_STATUS,CMD_SET_HOLD_STATUS,NULL},
                                      {EVENT_GET_HOLD_STATUS,CMD_GET_HOLD_STATUS,NULL},
                                      {EVENT_SET_TOUCH_QUEUE,CMD_SET_TOUCH_QUEUE,NULL},
                                      {EVENT_GET_TOUCH_QUEUE,CMD_GET_TOUCH_QUEUE,NULL},
                                      {EVENT_SET_TOUCH_SENSE,CMD_SET_TOUCH_SENSE,NULL},
                                      {EVENT_GET_TOUCH_SENSE,CMD_GET_TOUCH_SENSE,NULL},
                                      {EVENT_SET_TOUCH_PROX_AVERAGE,CMD_SET_TOUCH_PROX_AVERAGE,NULL},
                                      {EVENT_GET_TOUCH_PROX_AVERAGE,CMD_GET_TOUCH_PROX_AVERAGE,NULL},
                                      {EVENT_SET_TOUCH_MUTI,CMD_SET_TOUCH_MUTI,NULL},
                                      {EVENT_GET_TOUCH_MUTI,CMD_GET_TOUCH_MUTI,NULL},
                                      {EVENT_SET_PROXIMITY,CMD_SET_PROXIMITY,NULL},
                                      {EVENT_GET_PROXIMITY,CMD_GET_PROXIMITY,NULL},
                                      {EVENT_SET_TOUCH_CONFIG,CMD_SET_TOUCH_CONFIG,NULL},
                                      {EVENT_GET_TOUCH_CONFIG,CMD_GET_TOUCH_CONFIG,NULL},
                                      /* Control */
                                      {EVENT_CONTROL_CHANNEL,CMD_CONTROL_CHANNEL,NULL},
                                      {EVENT_UPDATE_CHANNEL,CMD_UPDATE_CHANNEL,NULL},
                                      {EVENT_CONTROL_SCRIPT,CMD_CONTROL_SCRIPT,NULL},
                                      {EVENT_UPDATE_SCRIPT,CMD_UPDATE_SCRIPT,NULL},
                                      {EVENT_CONTROL_REMOTE_TOGGLE,CMD_CONTROL_REMOTE_TOGGLE,NULL},
                                      {EVENT_CONTROL_REMOTE_ON_OFF,CMD_CONTROL_REMOTE_ON_OFF,NULL}};
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
static uint32_t EventID=0;
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
  
/**
  * @brief : Host Com
  * @param :   
  * @retval : 
  */
void v_HostComm_Get(void)
{
  static RF_DataTypeDef RFData;
  uint16_t i;

  if(RF->CheckDataOK())
  {
    RFData=RF->CurrentRxData();
//    RF->ClearDataOK();
    for(i=0;i<sizeof(HOSTCOMM_CB)/sizeof(HOSTCOMM_CB[0]);i++)
    {
      if((RFData.Cmd!=CMD_SCAN_MASTER)&&(RFData.Cmd!=CMD_REQUEST_ID)&&(RFData.Cmd!=CMD_SET_ID))
      {
        if(RFData.Cmd==HOSTCOMM_CB[i].Cmd)
        {
          if(HOSTCOMM_CB[i].Cb)
          {
            HOSTCOMM_CB[i].Cb(RFData.Data,RFData.Len);
          }
          break;
        }
      }
    }
  }
  /* exception */
  if(RF->CheckDataFlag())
  {
    RFData=RF->CurrentRxData();
    RF->ClearDataFlag();
    for(i=0;i<sizeof(HOSTCOMM_CB)/sizeof(HOSTCOMM_CB[0]);i++)
    {
      if((RFData.Cmd==CMD_SCAN_MASTER)||(RFData.Cmd==CMD_REQUEST_ID)||(RFData.Cmd==CMD_SET_ID))
      {
        if(RFData.Cmd==HOSTCOMM_CB[i].Cmd)
        {
          if(HOSTCOMM_CB[i].Cb)
          {
            HOSTCOMM_CB[i].Cb(RFData.Data,RFData.Len);
          }
          break;
        }
      }
    }
    
    
  }
}

void v_HostComm_Send(HostComm_EventTypeDef enm_event, uint8_t u8_name_des, uint16_t u16_sub_name_des, uint8_t* u8_data, uint16_t u16_len)
{
  static RF_DataTypeDef RFData;
  uint16_t i=0;
  for(i=0;i<sizeof(HOSTCOMM_CB)/sizeof(HOSTCOMM_CB[0]);i++)
  {
    if(HOSTCOMM_CB[i].Event==enm_event)
    {
      EventID++;
      RFData.EventID=EventID;
      /* Save to Backup RAM */
      
      RFData.ID=Setting.DeviceID;
      /* Capture time */
      //RFData.Time;
      /* Source and des */
      RFData.NameSrc=Setting.Name;
      RFData.SubNameSrc=Setting.SubName;
      RFData.NameDes=u8_name_des;
      RFData.SubNameDes=u16_sub_name_des;
      
      RFData.Cmd=HOSTCOMM_CB[i].Cmd;
      RFData.Len=MIN(u16_len,sizeof(RFData.Data));
      
      memset(RFData.Data,0,sizeof(RFData.Data));
      memcpy(RFData.Data,u8_data,MIN(u16_len,sizeof(RFData.Data)));
      
      /* Push to LL */
      RF->TxData(RFData);
      break;
    }
  } 
}
 
void v_HostComm_RegisterCallback(HostComm_EventTypeDef enm_event, void (*cb)(uint8_t* u8_data,uint16_t u16_len))
{
  uint16_t i=0;
  for(i=0;i<sizeof(HOSTCOMM_CB)/sizeof(HOSTCOMM_CB[0]);i++)
  {
    if(HOSTCOMM_CB[i].Event == enm_event)
    {
      HOSTCOMM_CB[i].Cb=cb;
      break;
    }
  }
} 
  

uint32_t u32_HostComm_GetEventID(void)
{
  return EventID;
}

/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
