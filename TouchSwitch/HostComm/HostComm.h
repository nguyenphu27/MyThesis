/**
  ******************************************************************************
  * @file   : HostComm.h
  * @author : DAI VE
  * @version: v1_00 
  * @date   :  2015
  * @brief  :Header file for .c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HostComm_H
#define __HostComm_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   #include "common.h"
   #include "string.h"
   #include "rf.h"

/**
@code  
 
@endcode
*/ 


 /* Define Event list Receive */
/* Diagnostic and control */
#define   CMD_SCAN_MASTER                 0x0040
#define   CMD_REQUEST_ID                  0x0041
#define   CMD_SET_ID                      0x0042
#define   CMD_SET_ID_OK                   0x0043
#define   CMD_SYNC_DATABASE               0x0044
#define   CMD_CHECK_CONNECTION            0x0045
#define   CMD_GET_VERSION                 0x0046
#define   CMD_RESET_DEVICE                0x0047
#define   CMD_RESET_TOUCH                 0x0048

/* Cmd for config */
#define   CMD_SET_SYNC_NAME_DEVICE        0x0080
#define   CMD_GET_SYNC_NAME_DEVICE        0x0081
#define   CMD_SET_SYNC_NAME_SCRIPT        0x0082
#define   CMD_GET_SYNC_NAME_SCRIPT        0x0083
#define   CMD_SET_TIME                    0x0084
#define   CMD_GET_TIME                    0x0085
#define   CMD_SET_RESET_TIME              0x0086
#define   CMD_GET_RESET_TIME              0x0087
#define   CMD_SET_RELAY_TIME              0x0088
#define   CMD_GET_RELAY_TIME              0x0089
#define   CMD_SET_UPDATE_TIME             0x008A
#define   CMD_GET_UPDATE_TIME             0x008B

/* Cmd for config RF */
#define   CMD_SET_RF_POWER                0x00C0
#define   CMD_GET_RF_POWER                0x00C1
#define   CMD_SET_RF_REPEATER             0x00C2
#define   CMD_GET_RF_REPEATER             0x00C3
#define   CMD_SET_RF_CONFIG               0x00C4
#define   CMD_GET_RF_CONFIG               0x00C5


/* Cmd for config touch */
#define   CMD_SET_TOUCH_SAMPLING          0x0100
#define   CMD_GET_TOUCH_SAMPLING          0x0101
#define   CMD_SET_BUTTON_ENABLE_DISABLE   0x0102
#define   CMD_GET_BUTTON_ENABLE_DISABLE   0x0103
#define   CMD_SET_HOLD_STATUS             0x0104
#define   CMD_GET_HOLD_STATUS             0x0105
#define   CMD_SET_TOUCH_QUEUE             0x0106 
#define   CMD_GET_TOUCH_QUEUE             0x0107
#define   CMD_SET_TOUCH_SENSE             0x0108
#define   CMD_GET_TOUCH_SENSE             0x0109
#define   CMD_SET_TOUCH_PROX_SENSE        0x010A
#define   CMD_GET_TOUCH_PROX_SENSE        0x010B
#define   CMD_SET_TOUCH_PROX_AVERAGE      0x010C
#define   CMD_GET_TOUCH_PROX_AVERAGE      0x010D
#define   CMD_SET_TOUCH_MUTI              0x010E
#define   CMD_GET_TOUCH_MUTI              0x010F 
#define   CMD_SET_PROXIMITY               0x0110
#define   CMD_GET_PROXIMITY               0x0111
#define   CMD_SET_TOUCH_CONFIG            0x0112
#define   CMD_GET_TOUCH_CONFIG            0x0113

/* Cmd for Error handle */
#define   CMD_SET_ERROR_PVD                0x0140
#define   CMD_GET_ERROR_PVD                0x0141
#define   CMD_SET_ERROR_RF                 0x0142
#define   CMD_GET_ERROR_RF                 0x0143
#define   CMD_SET_ERROR_TOUCH              0x0144
#define   CMD_GET_ERROR_TOUCH              0x0145
#define   CMD_SET_ERROR_ZERODETECT         0x0146
#define   CMD_GET_ERROR_ZERODETECT         0x0147
#define   CMD_SET_ERROR                    0x0148
#define   CMD_GET_ERROR                    0x0149

/* Define Event list Transmit */
#define CMD_CONTROL_CHANNEL                0x0180
#define CMD_UPDATE_CHANNEL                 0x0181
#define CMD_CONTROL_SCRIPT                 0x0182
#define CMD_UPDATE_SCRIPT                  0x0183
#define CMD_CONTROL_REMOTE_TOGGLE          0x0184
#define CMD_CONTROL_REMOTE_ON_OFF          0x0185

typedef enum
{
  /* Diagnostic and control */
  EVENT_SCAN_MASTER=0,                
  EVENT_REQUEST_ID,                 
  EVENT_SET_ID, 
  EVENT_SET_ID_OK,           
  EVENT_SYNC_DATABASE,             
  EVENT_CHECK_CONNECTION,                  
  EVENT_GET_VERSION,                
  EVENT_RESET_DEVICE,               
  EVENT_RESET_TOUCH ,               

  /* Cmd for config */
  EVENT_SET_SYNC_NAME_DEVICE,       
  EVENT_GET_SYNC_NAME_DEVICE,       
  EVENT_SET_SYNC_NAME_SCRIPT,       
  EVENT_GET_SYNC_NAME_SCRIPT,       
  EVENT_SET_TIME,                   
  EVENT_GET_TIME,                   
  EVENT_SET_RESET_TIME,             
  EVENT_GET_RESET_TIME,             
  EVENT_SET_RELAY_TIME,             
  EVENT_GET_RELAY_TIME,             
  EVENT_SET_UPDATE_TIME,            
  EVENT_GET_UPDATE_TIME,            

  /* Cmd for config RF */
  EVENT_SET_RF_POWER,               
  EVENT_GET_RF_POWER,               
  EVENT_SET_RF_REPEATER,            
  EVENT_GET_RF_REPEATER,            
  EVENT_SET_RF_CONFIG,              
  EVENT_GET_RF_CONFIG,              


  /* Cmd for config touch */
  EVENT_SET_TOUCH_SAMPLING,         
  EVENT_GET_TOUCH_SAMPLING,         
  EVENT_SET_BUTTON_ENABLE_DISABLE,  
  EVENT_GET_BUTTON_ENABLE_DISABLE,  
  EVENT_SET_HOLD_STATUS,         
  EVENT_GET_HOLD_STATUS,         
  EVENT_SET_TOUCH_QUEUE,          
  EVENT_GET_TOUCH_QUEUE,         
  EVENT_SET_TOUCH_SENSE,         
  EVENT_GET_TOUCH_SENSE,         
  EVENT_SET_TOUCH_PROX_SENSE,    
  EVENT_GET_TOUCH_PROX_SENSE,    
  EVENT_SET_TOUCH_PROX_AVERAGE,  
  EVENT_GET_TOUCH_PROX_AVERAGE,  
  EVENT_SET_TOUCH_MUTI,          
  EVENT_GET_TOUCH_MUTI,           
  EVENT_SET_PROXIMITY,          
  EVENT_GET_PROXIMITY,           
  EVENT_SET_TOUCH_CONFIG,        
  EVENT_GET_TOUCH_CONFIG,        

  /* Cmd for Error handle */
  EVENT_SET_ERROR_PVD,             
  EVENT_GET_ERROR_PVD,             
  EVENT_SET_ERROR_RF,              
  EVENT_GET_ERROR_RF,              
  EVENT_SET_ERROR_TOUCH,           
  EVENT_GET_ERROR_TOUCH,           
  EVENT_SET_ERROR_ZERODETECT,      
  EVENT_GET_ERROR_ZERODETECT,      
  EVENT_SET_ERROR,                 
  EVENT_GET_ERROR,                 

  /* Define Event list Transmit */
  EVENT_CONTROL_CHANNEL,             
  EVENT_UPDATE_CHANNEL,              
  EVENT_CONTROL_SCRIPT,              
  EVENT_UPDATE_SCRIPT,               
  EVENT_CONTROL_REMOTE_TOGGLE,       
  EVENT_CONTROL_REMOTE_ON_OFF,  

  EVENT_TOTAL
}HostComm_EventTypeDef;


/**
  * @}
  */
typedef struct
{
  HostComm_EventTypeDef Event;
  uint16_t Cmd;
  void (*Cb)(uint8_t* u8_data,uint16_t u16_len); /* function Callback */
}HostComm_CbTypeDef;   

/** @defgroup Exported_Types
  * @{
  */
void v_HostComm_Get(void);
void v_HostComm_Send(HostComm_EventTypeDef enm_event, uint8_t u8_name_des, uint16_t u16_sub_name_des, uint8_t* u8_data, uint16_t u16_len);
void v_HostComm_RegisterCallback(HostComm_EventTypeDef enm_event, void (*cb)(uint8_t* u8_data,uint16_t u16_len));
uint32_t u32_HostComm_GetEventID(void);


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
