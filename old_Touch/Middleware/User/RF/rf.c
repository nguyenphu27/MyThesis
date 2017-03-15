/**
  ******************************************************************************
  * @file    RFM22B.c
  * @author  DatLe
  * @version v1.00
  * @date    04-Sep-2014
  * @brief   STM32xx-EVAL abstraction layer. 
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "rfm22b.h"
#include "common.h"
#include "save.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
  * @{
  */ 
  
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
RF_DrvTypeDef *RF;
Error_TypeDef RFError;
Error_TypeDef RFErrorNow;
bool RF_Initialized = false;
extern RF_DrvTypeDef Rfm22b_Drv;

extern Repeater_DrvTypeDef Repeater_Drv;
Repeater_DrvTypeDef *Repeater;
Repeater_TypeDef RFRepeaterHandle;

static RF_DataTypeDef RxData;
RF_DataTypeDef TxData;
uint8_t RepeaterFlag=0;


static bool DataFlag;
static bool DataOK;
static uint16_t TxInterval;
static bool TxFlag;
static bool Enable=true;
static void SWEnable(void);
static void SWDisable(void);
static bool IsEnable(void);
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
  
void RF_Init(void)
{
  static uint8_t DeviceID;
  static uint8_t Version;
  /* Linker RF Driver to LL */
  RF=&Rfm22b_Drv;
  /* Init RF module */
  RF->Init();
  /* Retarget some of functions */
  RF->InputTask=RF_InputTask;
  RF->OutputTask=RF_OutputTask;
  RF->CheckSum=RF_GenerateCheckSum;
  RF->LoadDFConfig=RF_LoadDFConfig;
  RF->SetTxIntervalTime=RF_SetTxIntervalTime;
  RF->GetTxIntervalTime=RF_GetTxIntervalTime;
  RF->SetTxFlag=RF_SetTxFlag;
  RF->CheckTxFlag=RF_CheckTxFlag;
  RF->ClearTxFlag=RF_ClearTxFlag;
  
  RF->CheckDataFlag=RF_CheckDataFlag;
  RF->ClearDataFlag=RF_ClearDataFlag;
  RF->CheckDataOK=RF_CheckDataOK;
  RF->ClearDataOK=RF_ClearDataOK;
  RF->CurrentRxData=RF_CurrentRxData;

  RF->TxDataExt=RF_TxDataExt;
  RF->CheckEx=RF_CheckEx;
  RF->SWEnable=SWEnable;
  RF->SWDisable=SWDisable;
  RF->IsEnable=IsEnable;
  /* Run load default config and next is update to store config*/
  RF->LoadDFConfig();
  /* Linker Repeater driver to LL */
  Repeater=&Repeater_Drv;
  /* Init RF repeater handle */
  Repeater->Init(&RFRepeaterHandle);
  RF->SWEnable();
  /* Assign repeater handle to RF */
  RF_Initialized=true;
  
  DeviceID=RF->ReadID();
  Version=RF->ReadVersion();
  
}

/**
  * @brief : Input task handle of RF module
  * @param :   
  * @retval :
  */
void RF_InputTask(void)
{
  if((RF->DetectRxData())&&(Repeater->Status(&RFRepeaterHandle)==REPEATER_NOT_FULL))
  {
    RxData=RF->GetRxData();
    Repeater->ProcessInputData(&RFRepeaterHandle,RxData.Random);
    
    /* Get here */
    if(Repeater->IsNewData(&RFRepeaterHandle))
    {
      Repeater->ClearPendingNewData(&RFRepeaterHandle);
      if(RxData.Check==RF->CheckSum(&RxData))
      {
        DataFlag=true;
        /* Addition checking */
        if(RF->CheckEx(&RxData))
        {
          DataOK=true;
        }
//        else
//        {
//          if(RF->Check4Repeater(&RxData))
//          {
//            /* Repeater */  
//          }
//          else
//          {
//            /* Do nothing */
//          }
//        };
      }
    }
  }
  Repeater->Task(&RFRepeaterHandle);
}

/**
  * @brief : Output Task handle of RF module
  * @param :   
  * @retval :
  */
void RF_OutputTask(void)
{
  static bool Enable=true;
  static TO_TypeDef Interval;
  static TO_TypeDef ErrorCheckTO;/*time out using for error checking*/
  if(RF->IsEnable())
  {
    if( (RF->DetectTxData())&&
        (Repeater->Status(&RFRepeaterHandle)==REPEATER_NOT_FULL)&&
        (!RF->TxBusy())&&
        (Enable))/* Make sure repeater is not full */
    {
      /* Generate Random pattern and update repeater and data send*/
      if(Repeater->Status(&RFRepeaterHandle)==REPEATER_NOT_FULL)
      {
        TxData = RF->GetTxData();
        TxData.Random=Repeater->Input(&RFRepeaterHandle);
        /* Generate check sum */
        TxData.Check=RF->CheckSum(&TxData);
        /* Put data to LL */
        RF->PutDataToRFModule(TxData);
        Enable=false;
      
        RF->SetTxFlag();
        TO_Start(&Interval,RF->GetTxIntervalTime());
        TO_Start(&ErrorCheckTO,12);/*Start checking error process*/
      }
    }
    /*error checking process first time**************************************************/
      /*normally, TxTransmit Time is about 4ms
      if TxTransmit time > 4 ms (in this case is 12ms), RF transmit fail*/
      if(TO_ReadStatus(&ErrorCheckTO))
      {
        TO_ClearStatus(&ErrorCheckTO);
        if(RF->TxBusy())/*if RF module is still busy->error occurred*/
        {
          RFError.ErrorFlag=true;/*error moment->indicate*/
        }
        else
        {
          RFError.Error=false;/*indicating module is not error*/
          RFErrorNow.Error=false;
          RFError.Cnt=0;/*Clear error counter->very important*/
        };
        RF->ClearTxBusy();/*force TxBusy=FALSE*/
      }
      TO_Task(&ErrorCheckTO);
    
    /* Interval TX handle */
    if(TO_ReadStatus(&Interval))
    {
      TO_ClearStatus(&Interval);
      Enable=true;
    }
    TO_Task(&Interval);
  }
}
void RF_ClearDataFlag(void)
{
  DataFlag=false;
}
bool RF_CheckDataFlag(void)
{
  return DataFlag;
}
void RF_ClearDataOK(void)
{
  DataOK=false;
}
bool RF_CheckDataOK(void)
{
  return DataOK;
}


/**
  * @brief  Generating Check Number
  * @param  char *data
  * @retval Check Number
  */
uint16_t RF_GenerateCheckSum(RF_DataTypeDef *Data)
{
  uint16_t i;
  uint16_t Checksum;
  Checksum=0;
  for(i=0;i<(sizeof(RF_DataTypeDef)-sizeof(Data->Check));i++)
  {
    Checksum+=*((uint8_t *)Data+i);
  }
  return  Checksum;
}

/**
  * @brief : Additon check 
  * @param :   
  * @retval : 
  */
uint8_t RF_CheckEx(RF_DataTypeDef *Data)
{
  if(Data->ID==Setting.DeviceID)
  {
    return 1;
  }
  else
  {
    return 0;
  };
}

/**
  * @brief : Set Tx interval
  * @param : Time in ms  
  * @retval :
  */
void RF_SetTxIntervalTime(uint16_t Time)
{
  TxInterval=Time;
}

/**
  * @brief : return Tx interval time
  * @param :   
  * @retval : time
  */
uint16_t  RF_GetTxIntervalTime(void)
{
  return TxInterval;
}
/**
  * @brief : Set default value of all config
  * @param :   
  * @retval :
  */
void RF_LoadDFConfig(void)
{
  RF_SetTxIntervalTime(RF_TX_INTERVAL_DEFAULT);

}



/**
  * @brief : Set Tx Flag
  * @param : none  
  * @retval : none
  */
void RF_SetTxFlag(void)
{
  TxFlag=true;
}

/**
  * @brief : Clear Tx Flag
  * @param : none  
  * @retval : none
  */
void RF_ClearTxFlag(void)
{
  TxFlag=false;
}

/**
  * @brief : Check Tx Flag is set or not
  * @param : none  
  * @retval : none
  */
bool RF_CheckTxFlag(void)
{
  return TxFlag;
}

/**
  * @brief : Get current RX data received in cache
  * @param : none  
  * @retval : RX data received
  */
RF_DataTypeDef RF_CurrentRxData(void)
{
  return RxData;
}



void RF_TxDataExt(uint32_t ID, uint8_t NameSrc,uint8_t SubNameSrc,uint8_t NameDes,uint8_t SubNameDes,uint16_t Cmd,uint16_t Len,uint8_t *Data)
{
  static RF_DataTypeDef RFData;
  volatile uint16_t i;
  /* Generate Event ID */
  RFData.EventID++;
  RFData.ID=ID;
  
  /* Source and des */
  RFData.NameSrc=NameSrc;
  RFData.SubNameSrc=SubNameSrc;
  RFData.NameDes=NameDes;
  RFData.SubNameDes=SubNameDes;
  
  RFData.Cmd=Cmd;
  RFData.Len=Len;
  
  /* Clear for reload new data */
  memset(RFData.Data,0,sizeof(RFData.Data));
  /* Copy new data to send */
  memcpy(RFData.Data,Data,MIN(Len,sizeof(RFData.Data)));
  
  /* Generate Random and CRC */
  /* Will be generated later in output task */
  
  /* Transfer to LL send */
  RF->TxData(RFData);
}

/**
  * @brief : SW enable RF module
  * @param : none  
  * @retval : none
  */
static void SWEnable(void)
{
  Enable=true;
}
static void SWDisable(void)
{
  Enable=false;
}
static bool IsEnable(void)
{
  return Enable;
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
    
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
