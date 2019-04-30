/**
  ******************************************************************************
  * @file    rf.h
  * @author  MCD Application Team
  * @version v1.1.0
  * @date    08-Sep-2014
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
#ifndef __RF_H
#define __RF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stdbool.h"
#include "repeater.h"
#include "common.h" 

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup Exported_Types
  * @{
  */
   #define RF_DATA_LEN                          48
   #define RF_MAX_BUF_RX												50
   #define RF_MAX_BUF_TX												120
   #define RF_ID_LEN                            4
   #define RF_USER_DATA_LEN                     24
   
   #define RF_TX_INTERVAL_DEFAULT               320
   
   /***************************************************/
  #define RFM22B_DEVICE_TYPE_VALUE                    0x08
  /***************************************************/
  #define RFM22B_DEVICE_VERSION_VALUE                 0x06
/** 
  * @brief  driver structure definition
  */
typedef struct
{
  uint32_t  EventID; //[EVENT_ID_LEN];/*store ID of Event*/
  uint32_t 	ID;/*store ID of this device*/  
  uint8_t 	NameSrc;/*Store name of source device*/
  uint8_t 	SubNameSrc;/*store subname of source divice*/
  uint8_t 	NameDes;/*Store name of destination device*/
  uint8_t 	SubNameDes;/*store subname of destination device*/
  uint32_t  Time; /*  */   
  uint16_t  Cmd; /* store command of device */
  uint16_t  Len;  /* Len of Data Structure */   
  uint8_t   Data[RF_USER_DATA_LEN];/*data*/
  uint16_t 	Random;/*random number*/
  uint16_t 	Check;/*check number*/
}RF_DataTypeDef;

typedef enum
{
  RF_PWR_1dBm,
  RF_PWR_2dBm,
  RF_PWR_5dBm,
  RF_PWR_8dBm,
  RF_PWR_11dBm,
  RF_PWR_14dBm,
  RF_PWR_17dBm,
  RF_PWR_20dBm
}TxPower_EnumTypeDef;
typedef struct
{
  bool      Enable;/*enable or disable Error checking module*/
  bool      Display;/*enable or disable error display*/
  bool      Error;/*indicate the error status*/
  bool      ErrorFlag;/*time to error occur*/
  uint16_t  Cnt;/*error conter*/
}Rfm22b_ErrorTypeDef;
typedef struct
{  
  /* Interface */
    void                  (*Init)(void);
    void                  (*DeInit)(void);
    uint8_t               (*ReadID)(void);
    uint8_t               (*ReadVersion)(void);
    void                  (*Reset)(void);
    void                  (*InputTask)(void);
    void                  (*OutputTask)(void);
    bool                  (*DetectRxData)(void);
    RF_DataTypeDef        (*GetRxData)(void);/* Get data from buffer RX */
    void                  (*TxData)(RF_DataTypeDef Data);/* tranmit data to Tx buffer */
    bool                  (*DetectTxData)(void);
    RF_DataTypeDef        (*GetTxData)(void);/* Get data from buffer TX */
    void                  (*PutDataToRFModule)(RF_DataTypeDef Data);/* put data send to LL RF module */
    bool                  (*TxBusy)(void);/* Check RF TX is busy or not */
    void                  (*ClearTxBusy)(void);/* Force clear Tx Busy flag */
    uint16_t              (*CheckSum)(RF_DataTypeDef *Data);/* Calculate check sum for RF signal */
    void                  (*SetTxFlag)(void);
    void                  (*ClearTxFlag)(void);
    bool                  (*CheckTxFlag)(void);
    void                  (*ClearTxBuffer)(void);
    void                  (*ClearRxBuffer)(void);

    void                  (*ISR)(void);
    void                  (*Force2RxMode)(void);
    /* Config */
    void                  (*LoadDFConfig)(void);
    void                  (*SetTxIntervalTime)(uint16_t Time);
    uint16_t              (*GetTxIntervalTime)(void);
    void                  (*SetTxPower)(TxPower_EnumTypeDef Power);
    
    /* Addition function */
    bool                  (*CheckDataFlag)(void);/* Check Data Get and pass checksum */
    void                  (*ClearDataFlag)(void);
    
    bool                  (*CheckDataOK)(void);/* Check Data is OK, pass check sum and ID */
    void                  (*ClearDataOK)(void);
    RF_DataTypeDef        (*CurrentRxData)(void);
    void                  (*TxDataExt)(uint32_t ID, uint8_t NameSrc,uint8_t SubNameSrc,uint8_t NameDes,uint8_t SubNameDes,uint16_t Cmd,uint16_t Len,uint8_t *Data);
    uint8_t               (*CheckEx)(RF_DataTypeDef *Data);/* Additional check */
    void                  (*TurnOff)(void);
    void                  (*SWEnable)(void);
    void                  (*SWDisable)(void);
    bool                  (*IsEnable)(void);
  
}RF_DrvTypeDef;

/**
  * @}
  */
void RF_Init(void);
void RF_InputTask(void);
void RF_OutputTask(void);
void RF_ClearDataFlag(void);
bool RF_CheckDataFlag(void);
void RF_ClearDataOK(void);
bool RF_CheckDataOK(void);
void RF_SetTxFlag(void);
void RF_ClearTxFlag(void);
bool RF_CheckTxFlag(void);

RF_DataTypeDef RF_CurrentRxData(void);

/* Config module */
void      RF_LoadDFConfig(void);
void      RF_SetTxIntervalTime(uint16_t Time);
uint16_t  RF_GetTxIntervalTime(void);

uint16_t RF_GenerateCheckSum(RF_DataTypeDef *Data);
uint8_t RF_CheckEx(RF_DataTypeDef *Data);
void RF_TxDataExt(uint32_t ID, uint8_t NameSrc,uint8_t SubNameSrc,uint8_t NameDes,uint8_t SubNameDes,uint16_t Cmd,uint16_t Len,uint8_t *Data);

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

#endif /* __RF_H */

/************************ (C) COPYRIGHT ACIS *****END OF FILE****/
