/**
  ******************************************************************************
  * @file    :function.c
  * @author  :Dai Ve
  * @version :
  * @date    :
  * @brief   :
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "function.h"
#include "touch.h"
#include "relay.h"
#include "led.h"
#include "setting.h"
#include "delay.h"
#include "config.h"
#include "save.h"
#include "debug.h"
#include "common.h"
#include "zdetect.h"
#include "buzzer.h"

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
extern Setting_TypeDef Setting;
extern Data_TypeDef Data;
extern void (*Process)(void);
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
// u8 test = 0;
#define SCRIPT_OFF_TO         300
TO_TypeDef ScriptOffTO[SCRIPT_NUM];  
void v_Run(void)
{ 
  static const Button_EnumTypeDef BUTTON_EXT1[]={BTN1,BTN2,BTN3};
  static const Led_TypeDef LED_EXT1[]={LED1,LED2,LED3};
  
  static uint8_t NotFirstTime=0;
  static uint8_t u8_hold1,u8_hold2;
  
  volatile uint16_t i,j;
	static uint8_t flag = 0;
	static uint8_t flag_1 = 0;
	static uint16_t count = 0;
	static uint8_t n = 0;
	
  if(!NotFirstTime)
  {
    NotFirstTime=1;
    /* Add first time code here */
    u8_hold1=0;
    u8_hold2=0;
    
    /* Load LED for status of relay */
    for(i=0;i<2;i++)
    {
      if(RELAY_GetStatus(i))
      {
        LED_On(LED_EXT1[i]);
      }
      else
      {
        LED_Off(LED_EXT1[i]);
      }
    }
    
    LED_On(LEDBL);
		
		
  }
  /* #1## Scan touch to control channel */
  for(i=0;i<3;i++)
  {
    /* Press */
    if(Touch->DetectPress(BUTTON_EXT1[i]))
    {		
      Touch->ClearPress(BUTTON_EXT1[i]);
			
		/* Control BUZZER */
		//Buzzer_BipTrue();
		//test = GPIO_ReadInputDataBit(ZEROEXTI_GPIO_PORT, ZEROEXTI_PORT_PIN);
		
		if(i == 0){
			if(!RELAY_GetStatus(i))
			{
			/* Control LED */
			LED_On(LED_EXT1[i]);
			LED_Off(LED_EXT1[i+1]);
        
			/* Toggle relay status */
			RELAY_SetStatus((Relay_TypeDef)i,1);
			Zdetect_On((Relay_TypeDef)i);
			RELAY_SetStatus((Relay_TypeDef)i+1,0);
			Zdetect_Off((Relay_TypeDef)i+1);
			}
			else{
				/* Control LED */
				LED_Off(LED_EXT1[i]);
				LED_Off(LED_EXT1[i+1]);

				/* Toggle relay status */
				RELAY_SetStatus((Relay_TypeDef)i,0);
				Zdetect_Off((Relay_TypeDef)i);
				RELAY_SetStatus((Relay_TypeDef)i+1,0);
				Zdetect_Off((Relay_TypeDef)i+1);
			}
		}
		
		if(i == 1){
			if(!RELAY_GetStatus(i))
			{
			/* Control LED */
			LED_On(LED_EXT1[i]);
			LED_Off(LED_EXT1[i-1]);
				
			/* Toggle relay status */
			RELAY_SetStatus((Relay_TypeDef)i,1);
			Zdetect_On((Relay_TypeDef)i);
			RELAY_SetStatus((Relay_TypeDef)i-1,0);
			Zdetect_Off((Relay_TypeDef)i-1);
			}
			else{
				/* Control LED */
				LED_Off(LED_EXT1[i]);
				LED_Off(LED_EXT1[i-1]);
				
				/* Toggle relay status */
				RELAY_SetStatus((Relay_TypeDef)i,0);
				Zdetect_Off((Relay_TypeDef)i);
				RELAY_SetStatus((Relay_TypeDef)i-1,0);
				Zdetect_Off((Relay_TypeDef)i-1);
			}
		}

		if(i == 2){
			if(!RELAY_GetStatus(i))
			{
			/* Control LED */
			LED_On(LED_EXT1[i]);
        
			/* Toggle relay status */
			RELAY_SetStatus((Relay_TypeDef)i,1);
			Zdetect_On((Relay_TypeDef)i);
			}
			
			else{
				/* Control LED */
				LED_Off(LED_EXT1[i]);
				
				/* Toggle relay status */
				RELAY_SetStatus((Relay_TypeDef)i,0);
				Zdetect_Off((Relay_TypeDef)i);
			}
		}
}
		
    /* Release */
    if(Touch->DetectRelease(BUTTON_EXT1[i]))
    {
      Touch->ClearRelease(BUTTON_EXT1[i]);
      break;
    }
	}
	
	if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)) != 1){
		flag = 0;
	}
	
	if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 1) && (flag == 0)){ // | GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)				
	
		
				/* Control LED */
				LED_Off(LED_EXT1[0]);
				LED_Off(LED_EXT1[1]);
					
				/* Toggle relay status */
				RELAY_SetStatus((Relay_TypeDef)0,0);
				Zdetect_Off((Relay_TypeDef)0);
				RELAY_SetStatus((Relay_TypeDef)1,0);
				Zdetect_Off((Relay_TypeDef)1); 
				flag = 1;
	}
}


void Zero_UpdateLedState(u8 new_state)
{
	if(new_state)
	{
		LED_On(LED4);
	}
	else
	{
		LED_Off(LED4);
	}
}


