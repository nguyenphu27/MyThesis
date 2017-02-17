/**
  ******************************************************************************
  * @file    :adc.c
  * @author  :DatLe
  * @version :v1_00
  * @date    :27/11/2014
  * @brief   :adc configurations
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
#include "adc.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */

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
__IO uint16_t ADCConvertedValue[2]={0,0};
/**
  * @brief : ADC for zero detect config
  * @param : none  
  * @retval : none
  */
void ADC_ZeroDetectInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  
  ZERO_DETECT_CLK_ENABLE();
  GPIO_InitStructure.GPIO_Pin = ZERO_DETECT_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ZERO_DETECT_PORT, &GPIO_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //enable ADC1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  //ADC1 configuration
  //select independent conversion mode (single)
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  //We will convert single channel only
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  //we will convert one time
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  //select no external triggering
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  //right 12-bit data alignment in ADC data register
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //single channel conversion
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  //load structure values to control and status registers
  ADC_Init(ADC1, &ADC_InitStructure);
  //wake up temperature sensor
  ADC_TempSensorVrefintCmd(ENABLE);
  //ADC1 channel0 configuration
  //we select 41.5 cycles conversion for channel0
  //and rank=1 which doesn't matter in single mode
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_41Cycles5);
  //Enable ADC1
  ADC_Cmd(ADC1, ENABLE);
  //Enable ADC1 reset calibration register
  ADC_ResetCalibration(ADC1);
  //Check the end of ADC1 reset calibration register
  while(ADC_GetResetCalibrationStatus(ADC1));
  //Start ADC1 calibration
  ADC_StartCalibration(ADC1);
  //Check the end of ADC1 calibration
  while(ADC_GetCalibrationStatus(ADC1));
  //Start ADC1 Software Conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  //wait for conversion complete
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
  //read ADC value
  ADC_GetConversionValue(ADC1);
  //clear EOC flag
  ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
} 

/**
  * @brief : Read ADC value for zero detect 
  * @param : none  
  * @retval : ADC value
  */
uint16_t ADC_ZeroDetectRead(void)
{
  #ifdef _USING_DMA__
  return  ADCConvertedValue[0];
  #else
  volatile uint32_t cnt;
  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  // Get the conversion value
  return ADC_GetConversionValue(ADC1);
  #endif  
}

/**
  * @brief : READ current sensor data
  * @param : none  
  * @retval : current sennsor
  */
uint16_t ADC_CurrentRead(void)
{
  return  ADCConvertedValue[1];
}


void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* ADCCLK = PCLK2/2 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
  #else
    /* ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  #endif
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

  /* Configure GPIO ADC pin as analog input -------------------------*/
  ZERO_DETECT_CLK_ENABLE();
  GPIO_InitStructure.GPIO_Pin = ZERO_DETECT_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ZERO_DETECT_PORT, &GPIO_InitStructure);
  
  CURRENT_DETECT_CLK_ENABLE();
  GPIO_InitStructure.GPIO_Pin = CURRENT_DETECT_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(CURRENT_DETECT_PORT, &GPIO_InitStructure);
  
  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_41Cycles5);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_41Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

                                 
    
/******************* (C) COPYRIGHT 2014 ACIS *****END OF FILE****/
