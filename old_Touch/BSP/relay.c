/**
  ******************************************************************************
  * @file    relay.c
  * @author  Dai Ve
  * @version V1.00
  * @date    23-May-2013
  * @brief   STM32xx-EVAL abstraction layer. 
  *          This file should be added to the main application to use the provided
  *          functions that manage Relays, push-buttons, COM ports and low level 
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
#include "relay.h"

/** @addtogroup Utilities
  * @{
  */ 
  
/** @defgroup STM32_EVAL 
  * @brief This file provides firmware functions to manage Relays, push-buttons, 
  *        COM ports and low level HW resources initialization of the different 
  *        modules available on STM32 Evaluation Boards from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32_EVAL_Abstraction_Layer
  * @{
  */ 
  


/** @defgroup STM32_EVAL_Private_TypesDefinitions
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


/** @defgroup STM32_EVAL_Private_Variables
  * @{
  */
GPIO_TypeDef* RELAY_PORT[] =   { RELAY1_GPIO_PORT, 
                                 RELAY2_GPIO_PORT, 
                                 RELAY3_GPIO_PORT,
                                 RELAY4_GPIO_PORT};

const uint16_t RELAY_PIN[] =   { RELAY1_PIN, 
                                 RELAY2_PIN, 
                                 RELAY3_PIN,
                                 RELAY4_PIN};
const uint32_t  RELAY_CLK[] = { RELAY1_GPIO_CLK,
                               RELAY2_GPIO_CLK,
                               RELAY3_GPIO_CLK,
                               RELAY4_GPIO_CLK};
                               
                               
static uint8_t Status[RELAY_NUM];
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
	/*----------------------------------------------------------------------------
  initialize RELAY Pins
 *----------------------------------------------------------------------------*/

                               /**
  * @brief  Configures RELAY GPIO.
  * @param  Relay: Specifies the Relay to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */
void RELAY_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t i;
	for(i=0;i<3;i++) //(sizeof(RELAY_PIN)/sizeof(RELAY_PIN[0]))
  {
    /* Enable the GPIO_RELAY Clock */
    RCC_APB2PeriphClockCmd(RELAY_CLK[i], ENABLE);

    /* Configure the GPIO_RELAY pin */
    GPIO_InitStructure.GPIO_Pin = RELAY_PIN[i] ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RELAY_PORT[i], &GPIO_InitStructure);
    RELAY_Off((Relay_TypeDef)i);
    Status[i]=0;
  }
	
    /* Enable the GPIO_RELAY Clock */
    RCC_APB2PeriphClockCmd(RELAY_CLK[3], ENABLE);

    /* Configure the GPIO_RELAY pin */
    GPIO_InitStructure.GPIO_Pin = RELAY_PIN[3] ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(RELAY_PORT[3], &GPIO_InitStructure);

	//for(i = 2;i<4;i++){
		/* Enable the GPIO_RELAY Clock */
    //
	  //RCC_APB2PeriphClockCmd(RELAY_CLK[3], ENABLE);

    //RELAY_Off((Relay_TypeDef)i);
    //Status[i]=0;
	//}
}

void RELAY_On(Relay_TypeDef Relay)
{
  #ifdef RELAY_ON_LOW_LEVEL
    GPIO_ResetBits(RELAY_PORT[Relay],RELAY_PIN[Relay] );
  #else
    GPIO_SetBits(RELAY_PORT[Relay],RELAY_PIN[Relay] );
  #endif
  Status[Relay]=1;
}

void RELAY_Off(Relay_TypeDef Relay)
{
  #ifdef RELAY_ON_LOW_LEVEL
    GPIO_SetBits(RELAY_PORT[Relay],RELAY_PIN[Relay] );
  #else
    GPIO_ResetBits(RELAY_PORT[Relay],RELAY_PIN[Relay] );
  #endif
  Status[Relay]=0;
}

void RELAY_Toggle(Relay_TypeDef Relay)
{
  if(GPIO_ReadOutputDataBit(RELAY_PORT[Relay],RELAY_PIN[Relay]))
  {
    GPIO_ResetBits(RELAY_PORT[Relay],RELAY_PIN[Relay] );
  }
  else
  {
    GPIO_SetBits(RELAY_PORT[Relay],RELAY_PIN[Relay] );
  };
  Status[Relay]=!Status[Relay];
}


uint8_t RELAY_GetStatus(Relay_TypeDef Relay)
{
  return Status[Relay];
}

void RELAY_SetStatus(Relay_TypeDef Relay,uint8_t RelayStatus)
{
  Status[Relay]=RelayStatus;
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
