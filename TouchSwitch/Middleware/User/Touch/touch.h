/**
  ******************************************************************************
  * @file    touch.h
  * @author  MCD Application Team
  * @version v1.1.0
  * @date    05-Sep-2014
  * @brief   This file contains all the functions prototypes for the Touch Panel driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCH_H
#define __TOUCH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stdbool.h"
#include "common.h"
#include "timeout.h"    

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup Exported_Types
  * @{
  */
#define BUTTON_HOLD_TIME_DF             1000
#define BUTTON_DOUBLE_PRESS_TIME_DF     170
typedef enum
{
  PROX,
  BUTTON2,
  BUTTON3,
  BUTTON4,
  BUTTON5,
  BUTTON6,
  BUTTON7,
  BUTTON_NUM
}Button_EnumTypeDef;

typedef struct
{
  uint8_t Enable;
  uint8_t ProximityEnable;
  uint8_t Sampling;
  uint8_t Queue;
  uint8_t Sense;
  uint8_t ProxSense;
  uint8_t ProxAverage;
  uint8_t Multi;
  
  /* Checksum for this setting */
  uint8_t Checksum;
}Touch_SettingTypeDef;

/* Re-define button */
#define BTN1    BUTTON5
#define BTN2    BUTTON6
#define BTN3    BUTTON4
#define BTN4    BUTTON7
#define BTN5    BUTTON3
#define BTN6    BUTTON2


typedef struct
{
  bool      Enable;/*enable or disable Error checking module*/
  bool      Display;/*enable or disable error display*/
  bool      Error;/*indicate the error status*/
  bool      ErrorFlag;/*time to error occur*/
  uint16_t  Cnt;/*error conter*/
}Cap1214_ErrorTypeDef;
typedef struct 
{
  bool                        PressFlag;
  bool                        ReleaseFlag;
  bool                        HoldFlag;
  bool                        DoublePressFlag;
  bool                        DoublePressStart;
  bool                        Press; 
}Cap1214_TypeDefButton;

typedef struct
{
  uint8_t                     Sampling;
  uint8_t                     Queue;
  uint8_t                     Sense;
  uint8_t                     Multi;
}Cap1214_TypeDefTouchConfig;


typedef struct
{
  uint8_t                     Enable;
  uint8_t                     Average;
  uint8_t                     Sense;
}Cap1214_TypeDefProxConfig;

typedef struct 
{
  Cap1214_TypeDefButton       Button[BUTTON_NUM];
  Cap1214_TypeDefButton       ButtonPre[BUTTON_NUM];
  TO_TypeDef                  HoldTO[BUTTON_NUM];
  uint16_t                    HoldTime[BUTTON_NUM];
  uint16_t                    DoublePressTime[BUTTON_NUM];
  Cap1214_ErrorTypeDef        Error;

  /*Some of Configuration*/
  volatile bool               EnaProx;/*enable or disable using proximity function*/
  volatile bool               BLOn;/*Indicate BL ON/OFF*/
  volatile bool               Button1Lock;/*Enable or disable lock of button1*/
  volatile bool               Button2Lock;/*Enable or disable lock of button2*/
  volatile bool               Button3Lock;/*Enable or disable lock of button3*/
  volatile bool               Button4Lock;/*Enable or disable lock of button4*/
  Cap1214_TypeDefTouchConfig  TouchConf;
  Cap1214_TypeDefProxConfig   ProxConf;  
}Cap1214_TypeDef;
/** 
  * @brief  driver structure definition
  */
typedef struct
{  
  /* Interface */
    void                  (*Init)(void);
    void                  (*DeInit)(void);
    uint8_t               (*ReadID)(void);
    void                  (*Reset)(void);
    void                  (*InputTask)(void);
    /* Buffer handle */
    bool                  (*DetectData)(void);
    uint16_t              (*GetData)(void);/* Get data from buffer touch */
    void                  (*ClearData)(void);/* Clear alldata in buffer*/
  
    bool                  (*DetectPress)(Button_EnumTypeDef Btn);
    bool                  (*DetectRelease)(Button_EnumTypeDef Btn);
    bool                  (*DetectHold)(Button_EnumTypeDef Btn);
    bool                  (*DetectDoublePress)(Button_EnumTypeDef Btn);
    void                  (*ClearPress)(Button_EnumTypeDef Btn);
    void                  (*ClearRelease)(Button_EnumTypeDef Btn);
    void                  (*ClearHold)(Button_EnumTypeDef Btn);
    void                  (*ClearDoublePress)(Button_EnumTypeDef Btn);
  
    void                  (*Clear)(void);

    void                  (*ISR)(void);
    
    /* Config */
    void                  (*LoadDFConfig)(void);
    void                  (*SetHoldTimeAll)(uint16_t Time);
    void                  (*SetDoubleClickTimeAll)(uint16_t Time);
    void                  (*SetHoldTime)(Button_EnumTypeDef Btn,uint16_t Time);
    void                  (*SetDoubleClickTime)(Button_EnumTypeDef Btn,uint16_t Time);
}Button_DrvTypeDef;



void                  Touch_Init(void);

void                  Touch_InputTask (void);
void                  Touch_InputTaskEx (void);
void                  Touch_InputTaskEx1 (void);
void                  Touch_Clear(void);
void                  Touch_Reset(void);

bool                  Touch_DetectPress(Button_EnumTypeDef Btn);
bool                  Touch_DetectRelease(Button_EnumTypeDef Btn);
bool                  Touch_DetectHold(Button_EnumTypeDef Btn);
bool                  Touch_DetectDoublePress(Button_EnumTypeDef Btn);
void                  Touch_ClearPress(Button_EnumTypeDef Btn);
void                  Touch_ClearRelease(Button_EnumTypeDef Btn);
void                  Touch_ClearHold(Button_EnumTypeDef Btn);
void                  Touch_ClearDoublePress(Button_EnumTypeDef Btn);
/* Config */
void                  Touch_LoadDFConfig(void);
void                  Touch_SetHoldTimeAll(uint16_t Time);
void                  Touch_SetDoubleClickTimeAll(uint16_t Time);
void                  Touch_SetHoldTime(Button_EnumTypeDef Btn,uint16_t Time);
void                  Touch_SetDoubleClickTime(Button_EnumTypeDef Btn,uint16_t Time);

/* Setting config */
void Touch_Setting_Set_Status(uint8_t Enable);
void Touch_Setting_Set_Prox_Status(uint8_t Enable);
void Touch_Setting_Set_Sampling(uint8_t Sampling);
void Touch_Setting_Set_Queue(uint8_t Queue);
void Touch_Setting_Set_Sense(uint8_t Sense);
void Touch_Setting_Set_Prox_Sense(uint8_t Sense);
void Touch_Setting_Set_Prox_Average(uint8_t Average);
void Touch_Setting_Set_Multi(uint8_t Multi);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __TOUCH_H */

/************************ (C) COPYRIGHT ACIS *****END OF FILE****/
