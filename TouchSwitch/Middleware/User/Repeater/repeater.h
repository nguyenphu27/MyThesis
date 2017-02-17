/**
  ******************************************************************************
  * @file    repeater.h
  * @author  DatLe
  * @version v2.00
  * @date    08-Sep-2014
  * @brief   Header file for repeater.c module.
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __REPEATER_H
#define __REPEATER_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"    
#include "timeout.h"
//#include "main.h"   
#include "rf.h"  
#include "common.h"   
/** @addtogroup hardware_define
  * @{
  */ 
	 #define PIPELINE_NUM									128
	 #define PIPELINE_TIMEOUT_DEFAULT			4000
   
   #define REPEATER_MIN(a,b) (a<b)?a:b
   

/* Define variable typedef */   
typedef enum
{
  REPEATER_NOT_FULL=0,
  REPEATER_FULL
}Repeater_EnumStatus;

typedef struct 
{
	volatile Repeater_EnumStatus  Status;/* indicate status of repeater module (pipeline is full or not???) */
  volatile bool 						    NewDataFlag;
	volatile bool 						    PipeLineStatus[PIPELINE_NUM];/* store status of pipeline which is using(busy or not?) */
	uint16_t 						          Random[PIPELINE_NUM];/* store random of pattern signal */
	TO_TypeDef 		                PipelineTO[PIPELINE_NUM];
  /* config TO */
  uint16_t                      TOTime;
}Repeater_TypeDef;


typedef struct
{  
/* Interface */
  void                  (*Init)(Repeater_TypeDef *Repeater);
  void                  (*ProcessInputData)(Repeater_TypeDef *Repeater,uint16_t pattern);
  uint16_t              (*Input)(Repeater_TypeDef *Repeater);
  void                  (*Task)(Repeater_TypeDef *Repeater);
  bool                  (*IsNewData)(Repeater_TypeDef *Repeater);
  void                  (*ClearPendingNewData)(Repeater_TypeDef *Repeater);
  Repeater_EnumStatus   (*Status)(Repeater_TypeDef *Repeater);

/* config */
  void                  (*SetTOTime)(Repeater_TypeDef *Repeater,uint16_t Time);
}Repeater_DrvTypeDef;


void                  Repeater_Init(Repeater_TypeDef *Repeater);
void                  Repeater_ProcessInputData(Repeater_TypeDef *Repeater,uint16_t pattern);//Processing new comeming data
uint16_t              Repeater_Input(Repeater_TypeDef *Repeater);//generating new pattern and store it to repeater module
void                  Repeater_Task(Repeater_TypeDef *Repeater);
bool                  Repeater_IsNewData(Repeater_TypeDef *Repeater);
void                  Repeater_ClearPendingNewData(Repeater_TypeDef *Repeater);
Repeater_EnumStatus   Repeater_Status(Repeater_TypeDef *Repeater);//checking status of repeater(full or not???)

void                  Repeater_SetTOTime(Repeater_TypeDef *Repeater,uint16_t Time);//set the time out time to free pattern

void Repeater_RFData(void);
/* How to use??? */
/* 1##Declare driver variable ############################### */

  /* Repeater_DrvTypeDef *Repeater; */
  /* extern Repeater_DrvTypeDef Repeater_Drv */

/* 2##Linker driver ######################################### */

  /* Repeater=&Repeater_Drv; */

/* 3##Init driver ########################################### */
  
  /* Repeater->Init(); */
/* 4##Add repeater task in while(1)########################## */

  /* Repeater->Task(); */

/* 5## in output task of transmition ######################## */
  /*  a.Checking repeater status 
  if full:not run,wait until its not full or recheck again
  if not full run : random=Repeater->Input();
  using random to transmit data...
  */

/* 6## in input task of receive data */
  /* Get random in receive data and put it to Repeater->ProcessInputData(random);
    if its full, receive data is ignore  
    if its not full, processing is start
    Run Repeater->IsNewData() to check new data or repeater data??? 
    */




/**
  * @}
  */ 
  
/** @defgroup ODERSYS_MAIN_Exported_Constants
  * @{
  */



/**
  * @}
  */ 

/** @defgroup ODERSYS_MAIN_Exported_Macros
  * @{
  */ 
  
/**
  * @}
  */ 

/** @defgroup STM32_EVAL_Exported_Functions
  * @{
  */
	
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* __LED7SEG_H */

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
