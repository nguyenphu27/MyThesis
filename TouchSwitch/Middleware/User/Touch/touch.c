/**
  ******************************************************************************
  * @file    touch.c
  * @author  Dai Ve
  * @version v1.0.0
  * @date    06-Sep-2014
  * @brief   This file provides a set of functions needed to manage Touch 
  *          button available with cap1214 device mounted on 
  *          MCP.
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
  
/* Includes ------------------------------------------------------------------*/
#include "touch.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */ 
  
/** @defgroup STM32F429I_DISCOVERY_TS
  * @{
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Types_Definitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Variables
  * @{
  */
Button_DrvTypeDef  *Touch;
extern Button_DrvTypeDef Cap1214_Drv;
static Touch_SettingTypeDef Setting;

static Cap1214_TypeDef Button;
static uint16_t Mask[BUTTON_NUM]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0100};

/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Function_Prototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_TS_Private_Functions
  * @{
  */

/**
  * @brief  Initializes and configures the touch button functionalities and 
  *         configures all necessary hardware resources (GPIOs, clocks..).
  * @param  none
  * @param    
  * @retval none
  */
void Touch_Init(void)
{
  /* Driver Linker */
  Touch=&Cap1214_Drv;
  /* Init RF module */
  Touch->Init();
  /* Retarget some of functions */
  Touch->InputTask=Touch_InputTaskEx1;//
  Touch->Reset=Touch_Reset;

  Touch->DetectPress=Touch_DetectPress;
  Touch->DetectRelease=Touch_DetectRelease;
  Touch->DetectHold=Touch_DetectHold;
  Touch->DetectDoublePress=Touch_DetectDoublePress;
  Touch->ClearPress=Touch_ClearPress;
  Touch->ClearRelease=Touch_ClearRelease;
  Touch->ClearHold=Touch_ClearHold;
  Touch->ClearDoublePress=Touch_ClearDoublePress;
  
  Touch->Clear=Touch_Clear;
  
  Touch->LoadDFConfig=Touch_LoadDFConfig;
  Touch->SetHoldTime=Touch_SetHoldTime;
  Touch->SetDoubleClickTimeAll=Touch_SetDoubleClickTimeAll;
  Touch->SetHoldTime=Touch_SetHoldTime;
  Touch->SetDoubleClickTime=Touch_SetDoubleClickTime;
  
  /* Run load default config and next is update to store config*/
  Touch->LoadDFConfig();

}

/**
  * @brief : Cap1214 Reset (I2C, GPIO, ext int)
  * @param : none  
  * @retval : none
  */
void Touch_Reset(void)
{
  Touch->DeInit();
  Touch->Init();
  Touch->LoadDFConfig();
}

/**
  * @brief : Cap1214_Buffer input task handle
  *           Get data from buffer and analyse which button is pressed, hold, and released...
  * @param :   
  * @retval :
  */
void Touch_InputTask (void)
{
  static Num32 Data;
  uint16_t i;
  static Num32 Pre;
  /* 1## Read data from buffer */
  /* checking buffer */
  if(Touch->DetectData())
  {
    Data.n16.n[0]=Touch->GetData();
    for(i=0;i<BUTTON_NUM;i++)
    {
      if(Data.n16.n[0]&Mask[i])
      {
        Button.Button[i].Press=true;
        Button.Button[i].PressFlag=true;
        TO_Start(&Button.HoldTO[i],Button.HoldTime[i]);/* Start timeout to calculate hold */
      }
      else
      {
        if(Pre.n16.n[0]&Mask[i])
        {
          Button.Button[i].Press=false;
          Button.Button[i].ReleaseFlag=true;
          TO_Stop(&Button.HoldTO[i]);/* Start timeout to calculate hold */
        }            
      };
    }
    Pre=Data;
  }

  /* Timeout task for hold status process */
  for(i=0;i<BUTTON_NUM;i++)
  {
    if(TO_ReadStatus(&Button.HoldTO[i]))
    {
      TO_ClearStatus(&Button.HoldTO[i]);
      Button.Button[i].HoldFlag=true;
    }
    TO_Task(&Button.HoldTO[i]);
  }
  
}


void Touch_InputTaskEx (void)
{
  static Num32 Data,DataPre;
  uint16_t i,j;
  static Num32 Pre;
  /* 1## Read data from buffer */
  /* checking buffer */
  if(Touch->DetectData())
  {
    Data.n16.n[0]=Touch->GetData();
    for(i=0;i<BUTTON_NUM;i++)
    {
//      if(Data.n16.n[0]!=0)
//      {  
        if((Data.n16.n[0]&Mask[i])&&((Data.n16.n[0]&Mask[i])!=(DataPre.n16.n[0]&Mask[i])))
        {
          Button.Button[i].Press=true;
          Button.Button[i].PressFlag=true;
          TO_Start(&Button.HoldTO[i],Button.HoldTime[i]);/* Start timeout to calculate hold */
        }
        else
        {
          if(DataPre.n16.n[0]&Mask[i])
          {
            Button.Button[i].Press=false;
            Button.Button[i].HoldFlag=false;
            Button.Button[i].ReleaseFlag=true;
            TO_Stop(&Button.HoldTO[i]);/* Start timeout to calculate hold */
          }
        }
//      }
//      else
//      {
//        for(j=0;j<BUTTON_NUM;j++)
//        {
//          TO_Stop(&Button.HoldTO[j]);
//        }
//      };
    }
    DataPre=Data;
  }
  if(Data.n16.n[0]==0)
  {
    for(j=0;j<BUTTON_NUM;j++)
    {
      TO_Stop(&Button.HoldTO[j]);
      Button.Button[j].HoldFlag=false;
    }
  }
  for(i=0;i<BUTTON_NUM;i++)
  {
    if(TO_ReadStatus(&Button.HoldTO[i]))
    {
      TO_ClearStatus(&Button.HoldTO[i]);
      Button.Button[i].HoldFlag=true;
    }
    TO_Task(&Button.HoldTO[i]);
  }
}

void Touch_InputTaskEx1 (void)
{
  static Num32 Data,DataPre;
  uint16_t i,j;
  static Num32 Pre;
  /* 1## Read data from buffer */
  /* checking buffer */
  if(Touch->DetectData())
  {
    Data.n16.n[0]=Touch->GetData();
    for(i=0;i<BUTTON_NUM;i++)
    { 
      if((Data.n16.n[0]&Mask[i])!=(DataPre.n16.n[0]&Mask[i]))
      {
        if(Data.n16.n[0]&Mask[i])
        {
          Button.Button[i].Press=true;
          Button.Button[i].PressFlag=true;
          TO_Start(&Button.HoldTO[i],Button.HoldTime[i]);/* Start timeout to calculate hold */
        }
        else
        {
          Button.Button[i].Press=false;
          Button.Button[i].HoldFlag=false;
          Button.Button[i].ReleaseFlag=true;
          TO_Stop(&Button.HoldTO[i]);/* Start timeout to calculate hold */
        }
      }
    }
    DataPre=Data;
  }
  if(Data.n16.n[0]==0)
  {
    for(j=0;j<BUTTON_NUM;j++)
    {
      TO_Stop(&Button.HoldTO[j]);
      Button.Button[j].HoldFlag=false;
    }
  }
  for(i=0;i<BUTTON_NUM;i++)
  {
    if(TO_ReadStatus(&Button.HoldTO[i]))
    {
      TO_ClearStatus(&Button.HoldTO[i]);
      Button.Button[i].HoldFlag=true;
    }
    TO_Task(&Button.HoldTO[i]);
  }
}

/**
  * @brief : Clear buffer and flag data
  * @param : none  
  * @retval : none
  */
void Touch_Clear(void)
{
  uint8_t i;
  Touch->ClearData();
  for(i=0;i<BUTTON_NUM;i++)
  {
    Button.Button[i].HoldFlag=false;
    Button.Button[i].PressFlag=false;
    Button.Button[i].ReleaseFlag=false;
  }
}
/**
  * @brief : Load default value for button config (HoldTime,doublePress TIme....)
  * @param : none  
  * @retval : none
  */
void     Touch_LoadDFConfig(void)
{
  uint16_t i;
  for(i=0;i<BUTTON_NUM;i++)
  {
    Button.HoldTime[i]=BUTTON_HOLD_TIME_DF;
    Button.DoublePressTime[i]=BUTTON_DOUBLE_PRESS_TIME_DF;
  }
}

/**
  * @brief : Checking Touch is pressed or not?
  * @param : none  
* @retval : true:Pressed, false: not press
  */
bool Touch_DetectPress(Button_EnumTypeDef Btn)
{
  return Button.Button[Btn].PressFlag;
}

/**
  * @brief : Checking Touch is released or not?
  * @param : none  
* @retval : true:Released, false: not release
  */
bool Touch_DetectRelease(Button_EnumTypeDef Btn)
{
  return Button.Button[Btn].ReleaseFlag;
}

/**
  * @brief : Checking Touch is double press or not?
  * @param : none  
* @retval : true:Double press, false: not double press
  */
bool Touch_DetectDoublePress(Button_EnumTypeDef Btn)
{
  return Button.Button[Btn].DoublePressFlag;
}

/**
  * @brief : Checking Touch is hold or not?
  * @param : none  
* @retval : true:hold, false: not hold
  */
bool Touch_DetectHold(Button_EnumTypeDef Btn)
{
  return Button.Button[Btn].HoldFlag;
}

/**
  * @brief : Checking Touch is pressed or not?
  * @param : none  
* @retval : true:Pressed, false: not press
  */
void Touch_ClearPress(Button_EnumTypeDef Btn)
{
  Button.Button[Btn].PressFlag=false;
}

/**
  * @brief : Checking Touch is released or not?
  * @param : none  
* @retval : true:Released, false: not release
  */
void Touch_ClearRelease(Button_EnumTypeDef Btn)
{
  Button.Button[Btn].ReleaseFlag=false;
}

/**
  * @brief : Checking Touch is double press or not?
  * @param : none  
* @retval : true:Double press, false: not double press
  */
void Touch_ClearDoublePress(Button_EnumTypeDef Btn)
{
  Button.Button[Btn].DoublePressFlag=false;
}

/**
  * @brief : Checking Touch is hold or not?
  * @param : none  
* @retval : true:hold, false: not hold
  */
void Touch_ClearHold(Button_EnumTypeDef Btn)
{
  Button.Button[Btn].HoldFlag=false;
}


void Touch_SetHoldTimeAll(uint16_t Time)
{
  uint16_t i;
  for(i=0;i<BUTTON_NUM;i++)
  {
    Button.HoldTime[i]=Time;
  }
}
void Touch_SetDoubleClickTimeAll(uint16_t Time)
{
  uint16_t i;
  for(i=0;i<BUTTON_NUM;i++)
  {
    Button.DoublePressTime[i]=Time;
  }
}


void Touch_SetHoldTime(Button_EnumTypeDef Btn,uint16_t Time)
{
  Button.HoldTime[Btn]=Time;
}

void Touch_SetDoubleClickTime(Button_EnumTypeDef Btn,uint16_t Time)
{
  Button.DoublePressTime[Btn]=Time;
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
