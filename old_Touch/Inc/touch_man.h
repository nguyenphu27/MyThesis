/**
  ******************************************************************************
  * @file   :touch_man.h
  * @author :DatLe
  * @version:v1_00 
  * @date   :23/06/2015   
  * @brief  :Header file for .c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCH_MAN_H
#define __TOUCH_MAN_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "common.h" 
   #include "touch.h"
   #include "setting.h"

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
typedef struct
{
  void (*DetectPress)(void);  
  void (*DetectRelease)(void);  
  void (*DetectHold)(void);
}TouchMan_CBTypeDef;

typedef struct
{
  uint8_t Code; 
  void    (*CB)(void);  
}TouchMan_ErrorTypeDef;

typedef enum
{
  TOUCHMAN_ERROR_NONE=0,
  TOUCHMAN_ERROR_OVERFLOW_BTN_NUM,
  
  
  TOUCHMAN_ERROR_TOTAL_CODE
}TouchMan_ErrorCode;
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
void TouchMan_Init(void);
void TouchMan_Handle(void);

void TouchMan_RegisterErrorCB(void (*Callback)(void));
void TouchMan_UnregisterErrorCB(void);

void TouchMan_RegisterPressCB(Button_EnumTypeDef Btn,void (*Callback)(void));
void TouchMan_UnregisterPressCB(Button_EnumTypeDef Btn);

void TouchMan_RegisterReleaseCB(Button_EnumTypeDef Btn,void (*Callback)(void));
void TouchMan_UnregisterReleaseCB(Button_EnumTypeDef Btn);

void TouchMan_RegisterHoldCB(Button_EnumTypeDef Btn,void (*Callback)(void));
void TouchMan_UnregisterHoldCB(Button_EnumTypeDef Btn);

void TouchMan_Error(void);



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
