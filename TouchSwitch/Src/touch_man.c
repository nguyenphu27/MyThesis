/**
  ******************************************************************************
  * @file    :touch_man.c
  * @author  :DatLe
  * @version :v1_00
  * @date    :23/06/2015
  * @brief   :
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "touch_man.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */
extern Button_DrvTypeDef  *Touch;
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
static TouchMan_CBTypeDef CB[BUTTON_NUM];
static TouchMan_ErrorTypeDef Error;
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
void TouchMan_Init(void)
{
  uint8_t i;
  Touch_Init();
  /* Clear All Register Callback */
  for(i=0;i<BUTTON_NUM;i++)
  {
    CB[i].DetectPress=0;
    CB[i].DetectRelease=0;
    CB[i].DetectHold=0;
  }
  Error.CB=&TouchMan_Error;
}
void TouchMan_Handle(void)
{
  uint8_t i;
  /* #1## Input task */
  Touch->InputTask();
  
  /* #2## Handle press/release & hold */
  for(i=0;i<BUTTON_NUM;i++)
  {
    if(Touch->DetectPress((Button_EnumTypeDef)i))
    {
      Touch->ClearPress((Button_EnumTypeDef)i);
      /* Call Callback for press function */
      if(CB[i].DetectPress)
      {
        CB[i].DetectPress();
        
        #ifdef __USING_DEBUG_KEILC
          printf(">>Button %d is pressed\n",i);
        #endif
      }
      else
      {
        #ifdef __USING_DEBUG_KEILC
          printf(">>Button %d is not register to press callback function\n",i);
        #endif
      };
    }
    if(Touch->DetectRelease((Button_EnumTypeDef)i))
    {
      Touch->ClearRelease((Button_EnumTypeDef)i);
      /* Call Callback for release function */
      if(CB[i].DetectRelease)
      {
        CB[i].DetectRelease();
        
        #ifdef __USING_DEBUG_KEILC
          printf(">>Button %d is released\n",i);
        #endif
      }
      else
      {
        #ifdef __USING_DEBUG_KEILC
          printf(">>Button %d is not register to release callback function\n",i);
        #endif
      };
    }
    if(Touch->DetectHold((Button_EnumTypeDef)i))
    {
      Touch->ClearHold((Button_EnumTypeDef)i);
      /* Call Callback for release function */
      if(CB[i].DetectHold)
      {
        CB[i].DetectHold();
        
        #ifdef __USING_DEBUG_KEILC
          printf(">>Button %d is hold\n",i);
        #endif
      }
      else
      {
        #ifdef __USING_DEBUG_KEILC
          printf(">>Button %d is not register to hold callback function\n",i);
        #endif
      };
    }
  }
}

void TouchMan_RegisterErrorCB(void (*Callback)(void))
{
  Error.CB=Callback;
}
void TouchMan_UnregisterErrorCB(void)
{
  Error.CB=0;
}

void TouchMan_RegisterPressCB(Button_EnumTypeDef Btn,void (*Callback)(void))
{
  if(Btn<BUTTON_NUM)
  {
    CB[Btn].DetectPress=Callback;
  }
  else
  {
    if(Error.CB)
    {
      Error.Code=TOUCHMAN_ERROR_OVERFLOW_BTN_NUM;
      Error.CB();
    }
  };
}
void TouchMan_UnregisterPressCB(Button_EnumTypeDef Btn)
{
  CB[Btn].DetectPress=0;
}

void TouchMan_RegisterReleaseCB(Button_EnumTypeDef Btn,void (*Callback)(void))
{
  if(Btn<BUTTON_NUM)
  {
    CB[Btn].DetectRelease=Callback;
  }
  else
  {
    if(Error.CB)
    {
      Error.Code=TOUCHMAN_ERROR_OVERFLOW_BTN_NUM;
      Error.CB();
    }
  };
}
void TouchMan_UnregisterReleaseCB(Button_EnumTypeDef Btn)
{
  CB[Btn].DetectRelease=0;
}

void TouchMan_RegisterHoldCB(Button_EnumTypeDef Btn,void (*Callback)(void))
{
  if(Btn<BUTTON_NUM)
  {
    CB[Btn].DetectHold=Callback;
  }
  else
  {
    if(Error.CB)
    {
      Error.Code=TOUCHMAN_ERROR_OVERFLOW_BTN_NUM;
      Error.CB();
    }
  };
}
void TouchMan_UnregisterHoldCB(Button_EnumTypeDef Btn)
{
  CB[Btn].DetectHold=0;
}

__weak void TouchMan_Error(void)
{
  /* Display error */
}
    
/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
