/**
  ******************************************************************************
  * @file    repeater.c
  * @author  DatLe
  * @version v2.00
  * @date    08-Sep-2014
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
#include "repeater.h"

/** @addtogroup Utilities
  * @{
  */ 
 
 extern RF_DrvTypeDef *RF;
  
 extern Repeater_DrvTypeDef Repeater_Drv;
 extern Repeater_DrvTypeDef *Repeater;
 extern Repeater_TypeDef RFRepeaterHandle; 


/** @defgroup Abstraction_Layer
  * @{
  */ 
  


/** @defgroup Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 
/** @defgroup STM32_EVAL_Exported_Constants
  * @{
  */

									   
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Variables
  * @{
  */
Repeater_DrvTypeDef Repeater_Drv=
{
  Repeater_Init,  
  Repeater_ProcessInputData,
  Repeater_Input,
  Repeater_Task,
  Repeater_IsNewData,
  Repeater_ClearPendingNewData,
  Repeater_Status,
  Repeater_SetTOTime
};
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_FunctionPrototypes
  * @{
  */


/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Functions
  * @{
  */
  /* LL function */
  static uint16_t     Repeater_Rand(void);
  static void         Repeater_StartRand(uint16_t seed);
  static uint16_t     Repeater_RandNotEqual(uint16_t *NotEqual);
  
  /* Private functions */
	static uint16_t     Repeater_SearchPipelineFree(Repeater_TypeDef *Repeater);//checking which pipeline is empty,piority from 0-->7
	static void         Repeater_PipelineBusy(Repeater_TypeDef *Repeater,uint16_t pipeline);//pipeline is busy
	static void         Repeater_PipelineFree(Repeater_TypeDef *Repeater,uint16_t pipeline);//pipeline is free
	static void         Repeater_PipelineFreeData(Repeater_TypeDef *Repeater,uint16_t pipeline);//clear data in pipeline
	static void         Repeater_StorePattern(Repeater_TypeDef *Repeater,uint16_t newRandom,uint16_t pipeline);//get new pattern signal

  /**
  * @brief  initial the repeater module
  * @param  Repeater_TypeDef
  * @retval none
  */
  void Repeater_Init(Repeater_TypeDef *Repeater)
  {
    uint16_t i;
    Repeater_StartRand(12345);
    Repeater->Status=REPEATER_NOT_FULL;
    Repeater->NewDataFlag=false;
    Repeater->TOTime=PIPELINE_TIMEOUT_DEFAULT;
    /* Clear all pattern and set all pipeline is free */
    for(i=0;i<PIPELINE_NUM;i++)
    {
      Repeater_PipelineFreeData(Repeater,i);
    }
  }
  /**
  * @brief  Start number for randomizer
  * @param  16bits start seed
  * @retval none
  */
  static void Repeater_StartRand(uint16_t seed)
  {
    srand(seed);
  }
  
  /**
  * @brief  Generate the random number
  * @param  none
  * @retval 16bits random number 
  */
  static uint16_t Repeater_Rand(void)
  {
    return rand();
  }
  
  /**
  * @brief  Generate the random number and not equal with NotEqual array and 0
  * @param  none
  * @retval 16bits random number 
  */
  static uint16_t Repeater_RandNotEqual(uint16_t *NotEqual)
  {
    const uint8_t retries=30;
    uint8_t i,j,retry;
    uint16_t tmp;
    i=retries;
    tmp=Repeater_Rand();
    retry=0;
    while(i!=0)
    {
      for(j=0;j<PIPELINE_NUM;j++)
      {
        if((tmp==*(NotEqual+j))||(!tmp))
        {
          retry=1;
          break;
        }
      }
      if(retry)
      {
        tmp=Repeater_Rand();
        i--;
      }
      else
      {
        i=0;
      }
    }
    return tmp; 
  }
  
	/**
  * @brief  Search the pipeline which is free
  * @param  repeater: **if repeater->PipeLine[i]==true:
												pipeline i is busy
											**if repeater->PipeLine[i]==false:
												pipeline i is free
  * @retval pipeline is free
						if return ALL_PIPELINE_FULL, all pipeline are busy
  */
	static uint16_t Repeater_SearchPipelineFree(Repeater_TypeDef *Repeater)
	{
		uint16_t i;
    /* Make sure pipeline is not full */
		for(i=0;i<PIPELINE_NUM;i++)
		{
			if(!Repeater->PipeLineStatus[i])
			{
        return i;
			}
		}
		return 0;
	}
	
	/**
  * @brief  Set the pipeline busy
  * @param  repeater: **if repeater->PipeLine[pipeline]=1:
												pipeline 1 is busy
											**if repeater->PipeLine[pipeline]=0:
												pipeline 1 is free
						pipeline : index of busy pipeline
  * @retval none
  */
  static void Repeater_PipelineBusy(Repeater_TypeDef *Repeater,uint16_t pipeline)
	{
		if(pipeline<PIPELINE_NUM)
		{Repeater->PipeLineStatus[pipeline]=true;}
	}
	
	/**
  * @brief  Set the pipeline free
  * @param  repeater: **if repeater->PipeLine[pipeline]=true:
												pipeline 1 is busy
											**if repeater->PipeLine[pipeline]=false:
												pipeline 1 is free
						pipeline : index of free pipeline
  * @retval none
  */
	static void Repeater_PipelineFree(Repeater_TypeDef *Repeater,uint16_t pipeline)
	{
		if(pipeline<PIPELINE_NUM)
		{Repeater->PipeLineStatus[pipeline]=false;}
	}
	
	/**
  * @brief  Clear data in free pipeline
  * @param  pipeline : index of free pipeline need to clear data
  * @retval none
  */
	static void Repeater_PipelineFreeData(Repeater_TypeDef *Repeater,uint16_t pipeline)
	{
		if(pipeline<PIPELINE_NUM)
		{
			Repeater_PipelineFree(Repeater,pipeline);//set pipeline status free
			Repeater->Random[pipeline]=0;//clear data in pipeline
		}
	}
	
	
	/**
  * @brief  get new data to free pipeline
  * @param  newRandom: new data need to get
						pipeline : index of free pipeline need to update data
  * @retval none
  */
	static void Repeater_StorePattern(Repeater_TypeDef *Repeater,uint16_t newRandom,uint16_t pipeline)
	{
		if(pipeline<PIPELINE_NUM)
		{
			Repeater->Random[pipeline]=newRandom;
		}
	}
	

	/**
  * @brief  process
						1.checking the new pattern is not the same as any data in pipeline
						2.if yes
							a.find new pipeline free to store-->set it busy
  * @param  pattern: new data
  * @retval none
  */
	void Repeater_ProcessInputData(Repeater_TypeDef *Repeater,uint16_t pattern)
	{
		uint16_t i;
    bool findNewPipeline=true;/* Flag indicate need to find new pipeline */
		uint16_t freePipeline;
    
    /* 1##Checking pipeline is full or not?? 
    if full:no load data */
    if(Repeater->Status==REPEATER_NOT_FULL)
    {
      //LCD_DbgLog("> Repeater Start Process\n");
      /* 2##Checking pattern is new or old??? */
      for(i=0;i<PIPELINE_NUM;i++)
      {
        if(pattern==Repeater->Random[i])/* if pattern value is equal with one of value of Random storage */
        {
          findNewPipeline=false;/* pattern is old,no need to find new pipeline to store */
          break;
        }
      }
      /* 3##if its new pattern, find free pipeline and store data */
      if(findNewPipeline)
      {
        freePipeline=Repeater_SearchPipelineFree(Repeater);
        //LCD_DbgLog("> Free pipeline is: %d\n",freePipeline);
        /* if it is the last pipeline */
        if(freePipeline>=PIPELINE_NUM-1)
        {
          Repeater->Status=REPEATER_FULL;
          freePipeline=PIPELINE_NUM-1;/* make sure this variable is not large than PIPELINE_NUM-1 */
          //LCD_UsrLog("> Repeater cache is FULL\n"); 
        }
        /* if not the last pipeline */
        else
        {
          Repeater->Status=REPEATER_NOT_FULL;
          //LCD_DbgLog("> Repeater cache is NOT full\n");
        };
        /* Store new pattern to the free pipeline which has just find */
        //LCD_DbgLog("> Store new pattern( %d ) to %d pipeline\n",pattern,freePipeline);
        Repeater_StorePattern(Repeater,pattern,freePipeline);
        /* Set this pipeline is busy */
        //LCD_DbgLog("> Set %d pipeline is to be busy\n",freePipeline);
        Repeater_PipelineBusy(Repeater,freePipeline);
        /* Start TO to free this pipeline automatically */
        //LCD_DbgLog("> Start TO of pipeline %d\n",freePipeline);
        TO_Start(&Repeater->PipelineTO[freePipeline],Repeater->TOTime);
        /* Set new data flag */
        //LCD_DbgLog("> Set flag of new data comming\n");
        Repeater->NewDataFlag=true;
      }
      else
      {
        //LCD_DbgLog("> OLD DATA...\n");
      };
    }
	}
  
  /**
  * @brief  process to input new pattern to repeater
						1.checking the new pattern is not the same as any data in pipeline
						2.if yes
							a.find new pipeline free to store-->set it busy
  * @param  none
  * @retval none
  */
	uint16_t Repeater_Input(Repeater_TypeDef *Repeater)
	{
		uint16_t freePipeline;
    uint16_t tmp=0;
    
    /* 1##Checking pipeline is full or not?? 
    if full:no load data */
    if(Repeater->Status==REPEATER_NOT_FULL)
    {
      /* 2##Generating new pattern */
      tmp=Repeater_RandNotEqual(Repeater->Random);/* Make sure new pattern is not equal with patterns are stored in Repeater.Random array */
      
      
      /* 3##its new pattern certainlly, find free pipeline and store data */
      freePipeline=Repeater_SearchPipelineFree(Repeater);
      /* if it is the last pipeline */
      if(freePipeline>=PIPELINE_NUM-1)
      {
        Repeater->Status=REPEATER_FULL;
        freePipeline=PIPELINE_NUM-1;/* make sure this variable is not large than PIPELINE_NUM-1 */          
      }
      /* if not the last pipeline */
      else
      {
        Repeater->Status=REPEATER_NOT_FULL;
      };
      /* Store new pattern to the free pipeline which has just find */
      Repeater_StorePattern(Repeater,tmp,freePipeline);
      /* Set this pipeline is busy */
      Repeater_PipelineBusy(Repeater,freePipeline);
      /* Start TO to free this pipeline automatically */
      TO_Start(&Repeater->PipelineTO[freePipeline],Repeater->TOTime);
    }
    return tmp;
	}
	
	/**
  * @brief : Process TO of all pipeline
  * @param : none  
  * @retval : none
  */
	void Repeater_Task(Repeater_TypeDef *Repeater)
	{
		uint16_t i=0;
		for(i=0;i<PIPELINE_NUM;i++)
		{
			if(TO_ReadStatus(&Repeater->PipelineTO[i]))//if pipeline1 is timeout
			{
				TO_ClearStatus(&Repeater->PipelineTO[i]);//clear timeout status
				Repeater_PipelineFreeData(Repeater,i);//clear data
				Repeater_PipelineFree(Repeater,i);//clear busy flag
        Repeater->Status=REPEATER_NOT_FULL;
        //LCD_UsrLog("> Pipeline %d is free!!!\n",i);
			}
      TO_Task(&Repeater->PipelineTO[i]);
		}
	}
  
  /**
  * @brief : checking comming data is new or old???
  * @param : none  
  * @retval : true: new data
  */
  bool Repeater_IsNewData(Repeater_TypeDef *Repeater)
  {
    return Repeater->NewDataFlag;
  }
  
  /**
  * @brief : Clear pending new data flag
  * @param : none  
  * @retval :
  */
  void Repeater_ClearPendingNewData(Repeater_TypeDef *Repeater)
  {
    Repeater->NewDataFlag=false;
  }
  /**
  * @brief : Checking repeater module is full or not
  * @param : none  
  * @retval :
  */
  Repeater_EnumStatus   Repeater_Status(Repeater_TypeDef *Repeater)
  {
    return Repeater->Status;
  }
	
	void Repeater_SetTOTime(Repeater_TypeDef *Repeater,uint16_t Time)
  {
    Repeater->TOTime=Time;
  }

	
/**
  * @}
  */ 
 
void Repeater_RFData(void)
{
  #define REPEATER_NUMBER_DEVICE_IN_A_LAYER 3
  static TO_TypeDef TO;
  static RF_DataTypeDef Data;

  /* If get data but data is not belong to device */
  if(RF->CheckDataOK())
  {
    Data=RF->CurrentRxData();
    TO_Start(&TO,MIN((6*(rand()%REPEATER_NUMBER_DEVICE_IN_A_LAYER)+1),6*(REPEATER_NUMBER_DEVICE_IN_A_LAYER+1)));
    /* Clear pending flag */
    RF->ClearDataFlag();
    RF->ClearDataOK();
  }
  if(TO_ReadStatus(&TO))
  {
    TO_ClearStatus(&TO);
    if(Repeater->Status(&RFRepeaterHandle)==REPEATER_NOT_FULL)
    {
      /* Put data to LL */
      RF->PutDataToRFModule(Data);/* Put to TX data for sending */
    }
  }
  TO_Task(&TO);
}

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
