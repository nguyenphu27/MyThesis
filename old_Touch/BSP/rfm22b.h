/**
  ******************************************************************************
  * @file    rfm22b.h
  * @author  DatLe
  * @version v1.00
  * @date    04-Sep-2014
  * @brief   Header file for rfm22b.c module.
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
#ifndef __RFM22B_H
#define __RFM22B_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
  #include "stm32f10x.h"
  #include "common.h"
  #include "rf.h"
  #include "delay.h"    

  /* ----------------------------------------------------------------------- */
  /* Define for RFM22B ext gpio:Enable and interrupt */  
  #define RFM22B_EN_PIN                          GPIO_Pin_12
  #define RFM22B_EN_GPIO_PORT                    GPIOB
  #define RFM22B_EN_GPIO_CLK_ENABLE()            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE) 
  #define RFM22B_EN_GPIO_CLK_DISABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE)  

  #define RRM22B_IRQHandler                      EXTI3_IRQHandler
	#define RFM22B_INT_GPIO_PIN 									 GPIO_Pin_3
	#define RFM22B_INT_GPIO_PORT 									 GPIOA
	#define RFM22B_INT_GPIO_CLK_ENABLE() 				   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
   
  #define RFM22B_EXTI_PIN												 GPIO_PinSource3
  #define RFM22B_EXTI_PORT											 GPIO_PortSourceGPIOA
	#define RFM22B_EXTI_LINE											 EXTI_Line3
	#define RFM22B_EXTI_IRQ												 EXTI3_IRQn
  
  /*define for ANT pin*/
  #define RFM22B_RXANT_PIN                       GPIO_Pin_1
  #define RFM22B_RXANT_GPIO_PORT                 GPIOA
  #define RFM22B_RXANT_GPIO_CLK_ENABLE()         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE) 
  #define RFM22B_RXANT_GPIO_CLK_DISABLE()        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE) 
  
  #define RFM22B_TXANT_PIN                       GPIO_Pin_2
  #define RFM22B_TXANT_GPIO_PORT                 GPIOA
  #define RFM22B_TXANT_GPIO_CLK_ENABLE()         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE) 
  #define RFM22B_TXANT_GPIO_CLK_DISABLE()        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE)
  
  /* Define for RFM22B SPI */   
  #define RFM22B_SPI                            SPI1
  #define RFM22B_SPI_SPEED											SPI_BaudRatePrescaler_64
  #define RFM22B_SPI_CLK          						  RCC_APB2Periph_SPI1
  
  #define RFM22B_CS_PIN                         GPIO_Pin_4
  #define RFM22B_CS_GPIO_PORT                   GPIOA
  #define RFM22B_CS_GPIO_CLK_ENABLE()           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
  #define RFM22B_CS_GPIO_CLK_DISABLE()          RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE)
  
  #define RFM22B_SCK_PIN                        GPIO_Pin_5
  #define RFM22B_SCK_GPIO_PORT                  GPIOA
  #define RFM22B_SCK_GPIO_CLK_ENABLE()          RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
  #define RFM22B_SCK_GPIO_CLK_DISABLE()         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE)

  #define RFM22B_MOSI_PIN                       GPIO_Pin_7
  #define RFM22B_MOSI_GPIO_PORT                 GPIOA
  #define RFM22B_MOSI_GPIO_CLK_ENABLE()         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
  #define RFM22B_MOSI_GPIO_CLK_DISABLE()        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE)

  #define RFM22B_MISO_PIN                       GPIO_Pin_6
  #define RFM22B_MISO_GPIO_PORT                 GPIOA
  #define RFM22B_MISO_GPIO_CLK_ENABLE()         RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
  #define RFM22B_MISO_GPIO_CLK_DISABLE()        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE)
  

  /* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
  #define RFM22B_SPI_TIMEOUT                     ((uint32_t)0x1000)

  /* RFM22B marco */
  #define RFM22B_RESET					                  GPIO_WriteBit(RFM22B_EN_GPIO_PORT,RFM22B_EN_PIN,Bit_RESET );\
                                                  HAL_Delay(20);\
                                                  GPIO_WriteBit(RFM22B_EN_GPIO_PORT,RFM22B_EN_PIN,Bit_SET );\
                                                  HAL_Delay(50)
														
  #define RFM22B_OFF					                    GPIO_WriteBit(RFM22B_EN_GPIO_PORT,RFM22B_EN_PIN,Bit_RESET )
  
  #define READ_RFM22B_INT_PIN                     GPIO_ReadInputDataBit(RFM22B_INT_GPIO_PORT,RFM22B_INT_PIN)
  
  #define RFM22B_CS_DEASSERT  		                GPIO_WriteBit(RFM22B_CS_GPIO_PORT, RFM22B_CS_PIN, Bit_SET)
	#define RFM22B_CS_ASSERT    		                GPIO_WriteBit(RFM22B_CS_GPIO_PORT, RFM22B_CS_PIN, Bit_RESET)
  
  #define RFM22B_RXANT_ON  				                GPIO_SetBits(RFM22B_RXANT_GPIO_PORT,RFM22B_RXANT_PIN)
  #define RFM22B_RXANT_OFF  			                GPIO_ResetBits(RFM22B_RXANT_GPIO_PORT,RFM22B_RXANT_PIN)
  
  #define RFM22B_TXANT_ON  				                GPIO_SetBits(RFM22B_TXANT_GPIO_PORT,RFM22B_TXANT_PIN)
  #define RFM22B_TXANT_OFF  			                GPIO_ResetBits(RFM22B_TXANT_GPIO_PORT,RFM22B_TXANT_PIN)
  

  /* RFM22B Register define ------------------------------------------------------------------------------------------- */
    #define RFM22B_DEVICE_TYPE                          0x00
    #define RFM22B_DEVICE_VERSION                       0x01
    #define RFM22B_DEVICE_STATUS                        0x02
    #define RFM22B_INTERRUPT_STATUS_1                   0x03
    #define RFM22B_INTERRUPT_STATUS_2                   0x04
    #define RFM22B_INTERRUPT_ENABLE_1                   0x05 
    #define RFM22B_INTERRUPT_ENABLE_2                   0x06
    #define RFM22B_OPERATING_FUCTION_CONTROL_1          0x07
    #define RFM22B_OPERATING_FUCTION_CONTROL_2          0x08
    #define RFM22B_CRYTAL_OSC_LOAD_CAP                  0x09
    #define RFM22B_MCU_OUTPUT_CLOCK                     0x0A
    #define RFM22B_GPIO0_CONFIGURATION                  0x0B
    #define RFM22B_GPIO1_CONFIGURATION                  0x0C
    #define RFM22B_GPIO2_CONFIGURATION                  0x0D
    #define RFM22B_IO_PORT_CONFIGURATION                0x0E
    #define RFM22B_ADC_CONFIGURATION                    0x0F
    #define RFM22B_ADC_SENSOR_AMP_OFFSET                0x10
    #define RFM22B_ADC_VALUE                            0x11
    #define RFM22B_TEMPERATURE_SENSOR_CONTROL           0x12
    #define RFM22B_TEMPERATURE_VALUE_OFFSET             0x13
    #define RFM22B_WAKEUP_TIMER_PERIOD_1                0x14
    #define RFM22B_WAKEUP_TIMER_PERIOD_2                0x15
    #define RFM22B_WAKEUP_TIMER_PERIOD_3                0x16
    #define RFM22B_WAKEUP_TIMER_VALUE_1                 0x17
    #define RFM22B_WAKEUP_TIMER_VALUE_2                 0x18
    #define RFM22B_LOW_DUTY_CYCLE_MODE_DURATION         0x19
    #define RFM22B_LOW_BATTERY_DETECTOR_THRESHOLD       0x1A
    #define RFM22B_BATTERY_VOLTAGE_LEVEL                0x1B
    #define RFM22B_IF_FILTER_BANDWIDTH                  0x1C
    #define RFM22B_AFC_LOOP_GEARSHIFT_OVERRIDE          0x1D
    #define RFM22B_AFC_TIMMING_CONTROL                  0x1E
    #define RFM22B_CLK_RECOVERY_GEARSHIFT_OVERRIDE      0x1F
    #define RFM22B_CLK_RECOVERY_OVERSAMPLING_RATIO      0x20
    #define RFM22B_CLK_RECOVERY_OFFSET_2                0x21
    #define RFM22B_CLK_RECOVERY_OFFSET_1                0x22
    #define RFM22B_CLK_RECOVERY_OFFSET_0                0x23
    #define RFM22B_CLK_RECOVERY_TIMMING_LOOP_GAIN_1     0x24
    #define RFM22B_CLK_RECOVERY_TIMMING_LOOP_GAIN_0     0x25
    #define RFM22B_RECEIVED_SIGNAL_STRENGTH_INDICATOR   0x26
    #define RFM22B_RSSI_TH_CLEAR_CHANNEL_INDICATOR      0x27
    #define RFM22B_ANTENNA_DIVERSITY_REGISTER_1         0x28
    #define RFM22B_ANTENNA_DIVERSITY_REGISTER_2         0x29
    #define RFM22B_AFC_LIMITER                          0x2A
    #define RFM22B_AFC_CORRECT_READ                     0x2B
    #define RFM22B_OOK_COUNTER_VALUE_1                  0x2C
    #define RFM22B_OOK_COUNTER_VALUE_2                  0x2D
    #define RFM22B_SLICER_PEAK_HOLD                     0x2E
    
    #define RFM22B_DATA_ACCESS_CONTROL                  0x30
    #define RFM22B_EZMAC_STATUS                         0x31
    #define RFM22B_HEADER_CONTROL_1                     0x32
    #define RFM22B_HEADER_CONTROL_2                     0x33
    #define RFM22B_PREAMBLE_LENGTH                      0x34
    #define RFM22B_PREAMBLE_DETECTION_CONTROL           0x35
    #define RFM22B_SYNC_WORD_3                          0x36
    #define RFM22B_SYNC_WORD_2                          0x37
    #define RFM22B_SYNC_WORD_1                          0x38
    #define RFM22B_SYNC_WORD_0                          0x39
    #define RFM22B_TRANSMIT_HEADER_3                    0x3A
    #define RFM22B_TRANSMIT_HEADER_2                    0x3B
    #define RFM22B_TRANSMIT_HEADER_1                    0x3C
    #define RFM22B_TRANSMIT_HEADER_0                    0x3D
    #define RFM22B_TRANSMIT_PACKAGE_LENGTH              0x3E
    #define RFM22B_CHECK_HEADER_3                       0x3F
    #define RFM22B_CHECK_HEADER_2                       0x40
    #define RFM22B_CHECK_HEADER_1                       0x41
    #define RFM22B_CHECK_HEADER_0                       0x42
    #define RFM22B_HEADER_ENABLE_3                      0x43
    #define RFM22B_HEADER_ENABLE_2                      0x44
    #define RFM22B_HEADER_ENABLE_1                      0x45
    #define RFM22B_HEADER_ENABLE_0                      0x46
    #define RFM22B_RECEIVED_HEADER_3                    0x47
    #define RFM22B_RECEIVED_HEADER_2                    0x48
    #define RFM22B_RECEIVED_HEADER_1                    0x49
    #define RFM22B_RECEIVED_HEADER_0                    0x4A
    #define RFM22B_RECEIVED_PACKAGE_LENGTH              0x4B
    
    #define RFM22B_ADC8_CONTROL                         0x4F
    
    #define RFM22B_CHANNEL_FILTER_COEFFICIENT_ADDRESS   0x60
    
    #define RFM22B_CRYTAL_OCS_CONTROL_TEST              0x62
    
    #define RFM22B_AGC_OVERRIDE_1                       0x69
    
    #define RFM22B_TX_POWER                             0x6D
    #define RFM22B_TX_DATA_RATE_1                       0x6E
    #define RFM22B_TX_DATA_RATE_0                       0x6F
    #define RFM22B_MODULATION_MODE_CONTROL_1            0x70
    #define RFM22B_MODULATION_MODE_CONTROL_2            0x71
    #define RFM22B_FREQUENCY_DEVIATION                  0x72
    #define RFM22B_FREQUENCY_OFFSET_1                   0x73
    #define RFM22B_FREQUENCY_OFFSET_2                   0x74
    #define RFM22B_FREQUENCY_BAND_SELECT                0x75
    #define RFM22B_NOMINAL_CARRIER_FREQUENCY_1          0x76
    #define RFM22B_NOMINAL_CARRIER_FREQUENCY_2          0x77
    
    #define RFM22B_FREQUENCY_HOPEPING_CHANNEL_SELECT    0x79
    #define RFM22B_FREQUENCY_HOPEPING_STEP_SIZE         0x7A
    
    #define RFM22B_TX_FIFO_CONTROL_1                    0x7C
    #define RFM22B_TX_FIFO_CONTROL_2                    0x7D
    #define RFM22B_RX_FIFO_CONTROL                      0x7E
    #define RFM22B_FIFO_ACCESS                          0x7F
    
    
    
/**
@code  
 
@endcode
*/

/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */  
  #define RFM22B_MAX_BUF_RX												RF_MAX_BUF_RX
  #define RFM22B_MAX_BUF_TX												RF_MAX_BUF_TX
typedef struct 
{
  volatile uint16_t   Spos;
  volatile uint16_t   Rpos;
  RF_DataTypeDef      Data[RFM22B_MAX_BUF_RX];
}Rfm22b_RxBuffer;

typedef struct 
{
  volatile uint16_t   Spos;
  volatile uint16_t   Rpos;
  RF_DataTypeDef      Data[RFM22B_MAX_BUF_TX];
}Rfm22b_TxBuffer;


/**
  * @}
  */ 
  
/** @defgroup Exported_Constants
  * @{
  */

/**
  * @}
  */ 

/** @defgroup Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup Exported_Functions
  * @{
  */
		
    /* Driver linker functions ######################################################*/
    /* Interface */
    void                  RFM22B_Init (void);
    void                  RFM22B_DeInit(void);
    uint8_t               RFM22B_ReadID(void);
    uint8_t               RFM22B_ReadVersion(void);
    void                  RFM22B_Reset(void);
    bool                  RFM22B_DetectData(void);
    RF_DataTypeDef        RFM22B_GetData(void);/* Get data from buffer RX */
    void                  RFM22B_TxData(RF_DataTypeDef Data);/* tranmit data to Tx buffer */

    void                  RFM22B_ISR(void);
    void                  RFM22B_Force2RxMode(void);
    /* Config */
    void                  RFM22B_LoadDFConfig(void);
    void                  RFM22B_SetTxIntervalTime(uint16_t Time);
    void                  RFM22B_SetTxPower(TxPower_EnumTypeDef Power);
    
    /* Config */

    /* LL function ##################################################################*/

    /* LL function ##################################################################*/
    
    
    
    /*High level function, app can call this function###############################*/
    
    
    
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* __RFM22B_H */

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
