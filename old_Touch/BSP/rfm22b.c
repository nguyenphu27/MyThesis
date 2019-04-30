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

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
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
typedef struct
{
  volatile bool             TxBusy;/* Indicate TX is busy or not */
  volatile bool             ToRxMode;/* Force to RX mode */
    
  RF_DataTypeDef            DataRx;/* Cache RX */
}Rfm22b_TypeDef;  
/**
  * @}
  */ 
static void             RFM22B_ToTxMode(RF_DataTypeDef data);
static bool             RFM22B_TxIsBusy(void);
static void             RFM22B_ClearTxBusy(void);
/* LL buffer ---------------------------------------------------- */
static void             StoreRx(RF_DataTypeDef Data);
static RF_DataTypeDef   GetRx(void);
static bool             CheckRx(void);
static void             ClearRx(void);

static void             StoreTx(RF_DataTypeDef Data);
static RF_DataTypeDef   GetTx(void);
static bool             CheckTx(void);
static void             ClearTx(void);

//static void             RFM22B_TurnOff(void);

/** @defgroup Private_Variables
  * @{
  */
static Rfm22b_TypeDef Rfm22b;
static Rfm22b_RxBuffer Rx;
static Rfm22b_RxBuffer Tx;
static uint32_t Time;

RF_DrvTypeDef Rfm22b_Drv = 
{
  RFM22B_Init,
  RFM22B_DeInit,
  RFM22B_ReadID,
  RFM22B_ReadVersion,
  0,//Reset
  0,//InputTask
  0,//OutputTask
  CheckRx,
  GetRx,
  StoreTx,
  CheckTx,
  GetTx,
  RFM22B_ToTxMode,
  RFM22B_TxIsBusy,
  RFM22B_ClearTxBusy,
  0,
  0,
  0,
  0,
  ClearTx,
  ClearRx,
  
  RFM22B_ISR,
  RFM22B_Force2RxMode,
  
  /* Config */
  0,
  0,
  0,
  0,
  
  /* Addition function */
  0,
  0,
  0,
//  RFM22B_TurnOff
};
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

static void     LL_Init (void);

static uint8_t  SPIx_WriteRead(uint8_t Byte);
static void     SPIx_Write(uint8_t Value);
#if 0
static void     SPIx_Error(void);
#endif
static uint8_t  Rfm22b_Spi(uint8_t data);
static void     Rfm22b_Txdata(uint8_t data);
static uint8_t  Rfm22b_Rxdata(void);
static void     Rfm22b_Write(uint8_t address,uint8_t data);
static char     Rfm22b_Read(uint8_t address);

static void     RFM22B_SoftwareInit(void);

/* --------------------------------------------------------------- */
/*Interface MCU config*/
    /***************************************************
    ****************************************************/
    
    #define RFM22B_EN_FIFO_UNDER_OVER_INT             0x80
    #define RFM22B_EN_TX_FIFO_ALMOST_FULL_INT         0x40
    #define RFM22B_EN_TX_FIFO_ALMOST_EMPTY_INT        0x20
    #define RFM22B_EN_RX_FIFO_ALMOST_FULL_INT         0x10
    #define RFM22B_EN_EXTERNAL_INT                    0x08
    #define RFM22B_ENABLE_PACKAGE_SEND_INT            0x04
    #define RFM22B_ENABLE_VALID_PACKAGE_RECEIVED_INT  0x02
    #define RFM22B_ENABLE_CRC_ERROR_INT               0x01
    
    static void Rfm22b_InterruptEnable1(uint8_t cfg);
    
    /***************************************************/
    #define RFM22B_EN_SYNC_WORD_DETECTED_INT          0x80
    #define RFM22B_EN_VALID_PREAMBLE_DETECTED_INT     0x40
    #define RFM22B_EN_INVALID_PREAMBLE_DETECTED_INT   0x20
    #define RFM22B_EN_RSSI_INT                        0x10
    #define RFM22B_EN_WAKEUP_TIMER_INT                0x08
    #define RFM22B_EN_LOW_BATTERY_DETECT_INT          0x04
    #define RFM22B_EN_CHIP_READY                      0x02
    #define RFM22B_EN_POR_INT                         0x01
    static void Rfm22b_InterruptEnable2(uint8_t cfg);
    
    /***************************************************/
    #define RFM22B_SW_REGISTER_RESET_BIT              0x80
    #define RFM22B_EN_LOW_BATTERY_DETECT              0x40
    #define RFM22B_EN_WAKEUP_TIMER                    0x20
    #define RFM22B_EN_32KHZ_CRYTAL_SELECT             0x10
    #define RFM22B_TX_ON_IN_MANUAL_TX_MODE            0x08
    #define RFM22B_RX_ON_IN_MAUAL_RX_MODE             0x04
    #define RFM22B_TUNE_MODE                          0x02
    #define RFM22B_READY_MODE                         0x01
    static void Rfm22b_OperatingModeFunctionControl1(uint8_t cfg);
    
    /***************************************************/
    #define RFM22B_RX_MULTI_PACKET                    0x10
    #define RFM22B_AUTOMATIC_TRANSMISSION             0x08
    #define RFM22B_EN_LOW_DUTY_MODE                   0x04
    #define RFM22B_RX_FIFO_RESET                      0x02
    #define RFM22B_TX_FIFO_RESET                      0x01
    static void Rfm22b_OperatingModeFunctionControl2(uint8_t cfg);
    
    /***************************************************/
    #define RFM22B_PA_PEAK_DETECTOR_VALUE_READ_REGISTER 0x80
    #define RFM22B_PA_PEAK_DETECTOR_ENABLE              0x40
    #define RFM22B_PA_PEAK_DETECT_LEVEL_6_5             0x00
    #define RFM22B_PA_PEAK_DETECT_LEVEL_7               0x10
    #define RFM22B_PA_PEAK_DETECT_LEVEL_7_5             0x20
    #define RFM22B_PA_PEAK_DETECT_LEVEL_8               0x30
    #define RFM22B_LNA_SWITCH_CONTROLLER                0x08
    #define RFM22B_TX_OUTPUT_POWER_7                    0x07
    #define RFM22B_TX_OUTPUT_POWER_6                    0x06
    #define RFM22B_TX_OUTPUT_POWER_5                    0x05
    #define RFM22B_TX_OUTPUT_POWER_4                    0x04
    #define RFM22B_TX_OUTPUT_POWER_3                    0x03
    #define RFM22B_TX_OUTPUT_POWER_2                    0x02
    #define RFM22B_TX_OUTPUT_POWER_1                    0x01
    #define RFM22B_TX_OUTPUT_POWER_0                    0x00
    static void Rfm22b_TxPower(uint8_t cfg);
    

    /*config module*/
    /****************************************************
    Select Module Type: GFSK
    Select Enable/Disable Manchester Mode: ON
    Select Creytal Tolerance[ppm] : TX side Crytal: 20
                                    RX side Crytal: 20
    RX/TX carrier Frequency(Mhz): 920Mhz
    Rb[Kbps]:100
    
    GFSK/FSK Modem Setting:
      AFC: Enable
      Max.Rb Error : =< 1%
      Frequency Deviation : 50 Khz
    OOK RX Modem Setting:
      OOK RX BW [Khz] : 200 Khz
    XtalFreq[Khz]:30000
    ****************************************************/
    static void Rfm22b_RxTxCarrierFreqSet(void);
    static void Rfm22b_TxDataRateSet(void);
    static void Rfm22b_TxFreqDeviationSet(void);
    static void Rfm22b_GfskFskRxModemSet(void);    
    
    /*General Configurations*/
    /****************************************************
    Select Turn ON Packet Handler       : ON
    Select LSB/MSB First                : MSB
    Enable CRC                          : YES
    CRC Over Data Only                  : YES
    Select CRC Type                     : CCIT
    
    Select Moduleation Type             : GFSK
    Select DATA INVESION                : OFF
    
    Select Manchester Enable            : ON
    Select Manchester Data Inversion    : Invert  "1->0" = "1"
    Select Manchester Preamaple Polarity: 1010...
    Select Data Whitening               : ON
    
    Select Headers In Packet            : Header 3&2&1&0
    Select Variable Packet Length       : NO
    Select Sync Word Lenght             : Sync Word 3&2&1&0
    
    Select Preamble Length              : 8
    
    Configure Sync Word 3 Value         : 0x2D
    Configure Sync Word 2 Value         : 0xD4
    Configure Sync Word 1 Value         : 0x43
    Configure Sync Word 0 Value         : 0x75
    
    Select Data Length in Packet Structure: 24
    
    Configure TX Header 3 Value         : 0x05
    Configure TX Header 2 Value         : 0xC6
    Configure TX Header 1 Value         : 0xA9
    Configure TX Header 0 Value         : 0x9D
    
    TX Clock Configuration              : 0x00 No TX data CLK is available outside the chip
    
    RX Header 3 individual bit check Mask Value : 0xFF
    RX Header 2 individual bit check Mask Value : 0xFF
    RX Header 1 individual bit check Mask Value : 0xFF
    RX Header 0 individual bit check Mask Value : 0xFF
    
    RX Expect Broadcast at Header 3     : YES
    RX Expect Broadcast at Header 2     : YES
    RX Expect Broadcast at Header 1     : YES
    RX Expect Broadcast at Header 0     : YES
    
    Perform Header 3 Value Comparison   : YES
    Perform Header 2 Value Comparison   : YES
    Perform Header 1 Value Comparison   : YES
    Perform Header 0 Value Comparison   : YES
    
    Select Preamble Detect Threshold (in Nible resolution)  : 4  16bits
    
    ****************************************************/
    static void Rfm22b_GeneralConfig(void);

    static void Rfm22b_ToRxMode(void);
    static void Rfm22b_RxReset(void);
    


/**
  * @brief  Configures RFM22B GPIO and I2C interface.
  * @param  
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */
void RFM22B_Init (void)
{
  LL_Init();
  /* Reset device */
  RFM22B_RESET;
  RFM22B_SoftwareInit();
}

/**
  * @brief : Init the LL of RFM22B function
  * @param : Init the i2C, ext interrupt, GPIO, Turn on Power....   
  * @retval : none
  */

void RFM22B_DeInit (void)
{
//  LL_Init();
  /* Reset device */
//  RFM22B_RESET;
//  RFM22B_SoftwareInit();
}

static void LL_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SpiInitStructure;
  EXTI_InitTypeDef 	EXTI_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	/*##-1- Enable EN Pin ####################################################*/
  /* Configure the EN pin */
    RFM22B_EN_GPIO_CLK_ENABLE();
    GPIO_InitStructure.GPIO_Pin = RFM22B_EN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(RFM22B_EN_GPIO_PORT, &GPIO_InitStructure);
    
  /* Configure the RX TX ANT pin */
    RFM22B_RXANT_GPIO_CLK_ENABLE();
    GPIO_InitStructure.GPIO_Pin = RFM22B_RXANT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(RFM22B_RXANT_GPIO_PORT, &GPIO_InitStructure);
    
    RFM22B_TXANT_GPIO_CLK_ENABLE();
    GPIO_InitStructure.GPIO_Pin = RFM22B_TXANT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(RFM22B_TXANT_GPIO_PORT, &GPIO_InitStructure);
  
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure the SPI CS pin */
    RFM22B_CS_GPIO_CLK_ENABLE();
    GPIO_InitStructure.GPIO_Pin = RFM22B_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(RFM22B_CS_GPIO_PORT, &GPIO_InitStructure);
  
    RFM22B_CS_DEASSERT;
  
  /* configure SPI SCK, MOSI and MISO */
    RFM22B_SCK_GPIO_CLK_ENABLE();
    RFM22B_MOSI_GPIO_CLK_ENABLE();
    RFM22B_MISO_GPIO_CLK_ENABLE();
    
    /*!< Configure RFM22B_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = RFM22B_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(RFM22B_SCK_GPIO_PORT, &GPIO_InitStructure);


  /*!< Configure RFM22B_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = RFM22B_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(RFM22B_MOSI_GPIO_PORT, &GPIO_InitStructure);

	/*!< Configure RFM22B_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = RFM22B_MISO_PIN;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(RFM22B_MISO_GPIO_PORT, &GPIO_InitStructure);

	
	/*!< RFM22B_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RFM22B_SPI_CLK, ENABLE);
	
	/*!< SPI configuration : mode 0*/
  SpiInitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SpiInitStructure.SPI_Mode = SPI_Mode_Master;
  SpiInitStructure.SPI_DataSize = SPI_DataSize_8b;
  //SpiInitStructure.SPI_CPOL = SPI_CPOL_High;
	SpiInitStructure.SPI_CPOL = SPI_CPOL_Low;
  //SpiInitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SpiInitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SpiInitStructure.SPI_NSS = SPI_NSS_Soft;
  SpiInitStructure.SPI_BaudRatePrescaler = RFM22B_SPI_SPEED;
  

  SpiInitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SpiInitStructure.SPI_CRCPolynomial = 7;
  
  SPI_Init(RFM22B_SPI, &SpiInitStructure);
	
	SPI_Cmd(RFM22B_SPI, ENABLE); 

  /*##-4- INT pin ############################################################*/
  /* Configure the INT pin */
  /*!< Configure RFM22B_SPI pins: INT */
  RFM22B_INT_GPIO_CLK_ENABLE();
  
  GPIO_InitStructure.GPIO_Pin = RFM22B_INT_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  GPIO_Init(RFM22B_INT_GPIO_PORT, &GPIO_InitStructure);
	
	/* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTIx Line to INT pin */
  GPIO_EXTILineConfig(RFM22B_EXTI_PORT, RFM22B_EXTI_PIN	);

  /* Configure EXTIx line */			
  EXTI_InitStructure.EXTI_Line = RFM22B_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* Enable and set INT Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = RFM22B_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0E;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  RFM22B_RXANT_OFF;//turn off RX anttena
	RFM22B_TXANT_OFF;//turn off TX anttena
}

/**
  * @brief : Config RFM22B module
  * @param : none  
  * @retval : none
  */
static void RFM22B_SoftwareInit(void)
{
    Rfm22b_OperatingModeFunctionControl1(RFM22B_SW_REGISTER_RESET_BIT);// Reset
    HAL_Delay(5);
    Rfm22b_OperatingModeFunctionControl1(RFM22B_TUNE_MODE);// Set READY mode
    Rfm22b_OperatingModeFunctionControl2(0x00);
    Rfm22b_InterruptEnable1(RFM22B_ENABLE_PACKAGE_SEND_INT|
                            RFM22B_ENABLE_VALID_PACKAGE_RECEIVED_INT);
    Rfm22b_InterruptEnable2(0x00); 
    Rfm22b_Write(RFM22B_CRYTAL_OSC_LOAD_CAP, 0x7F);        // Cap = 12.5pF
    Rfm22b_Write(RFM22B_MCU_OUTPUT_CLOCK, 0x06);           // Clk output is 1MHz

    Rfm22b_Write(RFM22B_GPIO0_CONFIGURATION, 0xFF);        // GPIO0 is GND
    Rfm22b_Write(RFM22B_GPIO1_CONFIGURATION, 0xFF);        // GPIO1 is GND
    Rfm22b_Write(RFM22B_GPIO2_CONFIGURATION, 0xFF);        // GPIO2 is GND
    Rfm22b_Write(RFM22B_IO_PORT_CONFIGURATION, 0x00);      // GPIO port use default value

    Rfm22b_Write(RFM22B_ADC_CONFIGURATION, 0x70);           // NO ADC used
    Rfm22b_Write(RFM22B_ADC_SENSOR_AMP_OFFSET, 0x00);       // no ADC used
    Rfm22b_Write(RFM22B_TEMPERATURE_SENSOR_CONTROL, 0x00);  // No temp sensor used
    Rfm22b_Write(RFM22B_TEMPERATURE_VALUE_OFFSET, 0x00);    // no temp sensor used

    Rfm22b_Write(0x56, 0x01);

    Rfm22b_TxPower(RFM22B_LNA_SWITCH_CONTROLLER|RFM22B_TX_OUTPUT_POWER_6);        // Tx power to nearly max

    Rfm22b_Write(RFM22B_FREQUENCY_HOPEPING_CHANNEL_SELECT, 0x0F);        
    Rfm22b_Write(RFM22B_FREQUENCY_HOPEPING_STEP_SIZE, 0x0F);        

    Rfm22b_Write(0x5A, 0x7F);
    Rfm22b_Write(0x59, 0x40);
    Rfm22b_Write(0x58, 0x80);

    Rfm22b_Write(0x6A, 0x0B);
    Rfm22b_Write(0x68, 0x04);
    Rfm22b_Write(0x1F, 0x03);
    
    Rfm22b_RxTxCarrierFreqSet();
    Rfm22b_TxDataRateSet();
    Rfm22b_TxFreqDeviationSet();
    Rfm22b_GfskFskRxModemSet();
    
    Rfm22b_GeneralConfig();
    
    Rfm22b_ToRxMode();
}



/**
  * @brief  DeInit RFM22B GPIO and I2C interface.
  * @param   
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */
//void RFM22B_DeInit (void)
//{
//  /* Turn off power */
//  GPIO_WriteBit(RFM22B_EN_GPIO_PORT, RFM22B_EN_PIN, Bit_SET);
//}

/**
  * @brief : Read the ID of RFM22B chip
  * @param : none  
  * @retval : the ID value (0x5A)
  */
uint8_t RFM22B_ReadID(void)
{
  return Rfm22b_Read(RFM22B_DEVICE_TYPE);
}

/**
  * @brief : Read the ID of RFM22B chip
  * @param : none  
  * @retval : the ID value (0x5A)
  */
uint8_t RFM22B_ReadVersion(void)
{
  return Rfm22b_Read(RFM22B_DEVICE_VERSION);
}

/**
  * @brief : RFM22B Reset (I2C, GPIO, ext int)
  * @param : none  
  * @retval : none
  */
void RFM22B_Reset(void)
{
  RFM22B_DeInit();
  RFM22B_Init();
  //RFM22B_LoadDFConfig();
}
 

/**
  * @brief : auto Force to RX mode
  * @param : none  
  * @retval : none
  */
void RFM22B_Force2RxMode(void)
{
  /*auto turn to Rx Mode*/
  if(Rfm22b.ToRxMode)
  {
    Rfm22b.ToRxMode=false;
    Rfm22b_ToRxMode();
  }  
}

/* LL to RFM22b module register */

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte: Byte send.
  * @retval The received byte value
  */
static uint8_t SPIx_WriteRead(uint8_t Byte)
{
  uint32_t cnt;
  while((!(SPI_I2S_GetFlagStatus(RFM22B_SPI, SPI_I2S_FLAG_TXE)))&&(cnt<RFM22B_SPI_TIMEOUT)){cnt++;};
	SPI_I2S_SendData(RFM22B_SPI, Byte);
  cnt=0;
	while((!(SPI_I2S_GetFlagStatus(RFM22B_SPI, SPI_I2S_FLAG_RXNE)))&&(cnt<RFM22B_SPI_TIMEOUT)){cnt++;};
	return(SPI_I2S_ReceiveData(RFM22B_SPI));
}

/**
  * @brief  Writes a byte to device.
  * @param  Value: value to be written
  * @retval None
  */
static void SPIx_Write(uint8_t Value)
{
  SPIx_WriteRead(Value);
}

/**
  * @brief  send and receive a byte throught spi from master -> rfm22b module.
  * @param  uint8_t data: data send.
  * @retval uint8_t dataGet
  */
uint8_t Rfm22b_Spi(uint8_t data)
{
	return SPIx_WriteRead(data);
}

/**
  * @brief : Write data to rfm22b register
  * @param :  address: register address
              data: data write
  * @retval : none
  */
static void Rfm22b_Write(uint8_t address,uint8_t data)
{
    //write any data byte to any single address
    //adds a 0 to the MSB of the address byte (WRITE mode)
    address |= 0x80;

    RFM22B_CS_ASSERT;
		//Rfm22b_Delay(RFM22B_CS_DLY);
	
    SPIx_Write(address);
    SPIx_Write(data);
	
		//Rfm22b_Delay(RFM22B_CS_DLY);
    RFM22B_CS_DEASSERT;
		//Rfm22b_Delay(RFM22B_CS_DLY);
}

/**
  * @brief : Read data from rfm22b register
  * @param :  address: register address
  * @retval : data read
  */
static char Rfm22b_Read(uint8_t address)
{
    //returns the contents of any 1 byte register from any address
    //sets the MSB for every address byte (READ mode)
    uint8_t byte;

    address &= 0x7F;

    RFM22B_CS_ASSERT;
		//Rfm22b_Delay(RFM22B_CS_DLY);
	
    Rfm22b_Txdata(address);
    byte = Rfm22b_Rxdata();
	
		//Rfm22b_Delay(RFM22B_CS_DLY);
    RFM22B_CS_DEASSERT;
		//Rfm22b_Delay(RFM22B_CS_DLY);
    return byte;
}
/**
  * @brief : Tx a byte to RFM22B
* @param : data:data to send  
  * @retval : none
  */
static void Rfm22b_Txdata(uint8_t data)
{
  Rfm22b_Spi(data);
}
/**
  * @brief : getchar a byte from RFM22B
  * @param : none  
  * @retval : data
  */
static uint8_t Rfm22b_Rxdata(void)
{
	return Rfm22b_Spi(0x55);
}

#if 0
/**
  * @brief : SPI error handle, LCD display
  * @param : none  
  * @retval : none
  */
static void     SPIx_Error(void)
{
  /* Error Handle */
}
#endif


/* LL interface */
static void Rfm22b_InterruptEnable1(uint8_t cfg)
{
  Rfm22b_Write(RFM22B_INTERRUPT_ENABLE_1,cfg);
}

static void Rfm22b_InterruptEnable2(uint8_t cfg)
{
  Rfm22b_Write(RFM22B_INTERRUPT_ENABLE_2,cfg);
}

static void Rfm22b_OperatingModeFunctionControl1(uint8_t cfg)
{
  Rfm22b_Write(RFM22B_OPERATING_FUCTION_CONTROL_1,cfg);
}

static void Rfm22b_OperatingModeFunctionControl2(uint8_t cfg)
{
  Rfm22b_Write(RFM22B_OPERATING_FUCTION_CONTROL_2,cfg);
}

static void Rfm22b_TxPower(uint8_t cfg)
{
  Rfm22b_Write(RFM22B_TX_POWER,cfg);
}
static void Rfm22b_RxTxCarrierFreqSet(void)
{
  Rfm22b_Write(RFM22B_FREQUENCY_BAND_SELECT, 0x73);
  Rfm22b_Write(RFM22B_NOMINAL_CARRIER_FREQUENCY_1, 0x64);
  Rfm22b_Write(RFM22B_NOMINAL_CARRIER_FREQUENCY_2, 0x00);
}

static void Rfm22b_TxDataRateSet(void)
{
  Rfm22b_Write(RFM22B_TX_DATA_RATE_1, 0x19);
  Rfm22b_Write(RFM22B_TX_DATA_RATE_0, 0x9A);
  Rfm22b_Write(RFM22B_MODULATION_MODE_CONTROL_1, 0x0F);
  Rfm22b_Write(0x58, 0xED);
}

static void Rfm22b_TxFreqDeviationSet(void)
{
  Rfm22b_Write(RFM22B_FREQUENCY_DEVIATION, 0x50);
  Rfm22b_Write(RFM22B_MODULATION_MODE_CONTROL_2, 0x23);
}

static void Rfm22b_GfskFskRxModemSet(void)
{
  Rfm22b_Write(RFM22B_IF_FILTER_BANDWIDTH, 0x88);
  Rfm22b_Write(RFM22B_CLK_RECOVERY_OVERSAMPLING_RATIO, 0x3C);
  Rfm22b_Write(RFM22B_CLK_RECOVERY_OFFSET_2, 0x01);
  Rfm22b_Write(RFM22B_CLK_RECOVERY_OFFSET_1, 0x11);
  Rfm22b_Write(RFM22B_CLK_RECOVERY_OFFSET_0, 0x11);
  Rfm22b_Write(RFM22B_CLK_RECOVERY_TIMMING_LOOP_GAIN_1, 0x07);
  Rfm22b_Write(RFM22B_CLK_RECOVERY_TIMMING_LOOP_GAIN_0, 0xFF);
  Rfm22b_Write(RFM22B_AFC_LOOP_GEARSHIFT_OVERRIDE, 0x44);
  Rfm22b_Write(RFM22B_AFC_TIMMING_CONTROL, 0x02);
  Rfm22b_Write(RFM22B_AFC_LIMITER, 0x50);
  Rfm22b_Write(RFM22B_CLK_RECOVERY_GEARSHIFT_OVERRIDE, 0x03);
  Rfm22b_Write(RFM22B_AGC_OVERRIDE_1, 0x60);
}

static void Rfm22b_GeneralConfig(void)
{
  Rfm22b_Write(RFM22B_DATA_ACCESS_CONTROL, 0xAC);
  Rfm22b_Write(RFM22B_HEADER_CONTROL_1, 0xFF);
  Rfm22b_Write(RFM22B_HEADER_CONTROL_2, 0x4E);
  Rfm22b_Write(RFM22B_PREAMBLE_LENGTH, 0x08);
  Rfm22b_Write(RFM22B_PREAMBLE_DETECTION_CONTROL, 0x22);
  Rfm22b_Write(RFM22B_SYNC_WORD_3, 0x2D);
  Rfm22b_Write(RFM22B_SYNC_WORD_2, 0xD4);
  Rfm22b_Write(RFM22B_SYNC_WORD_1, 0x43);
  Rfm22b_Write(RFM22B_SYNC_WORD_0, 0x75);
  Rfm22b_Write(RFM22B_TRANSMIT_HEADER_3, 0x05);
  Rfm22b_Write(RFM22B_TRANSMIT_HEADER_2, 0xC6);
  Rfm22b_Write(RFM22B_TRANSMIT_HEADER_1, 0xA9);
  Rfm22b_Write(RFM22B_TRANSMIT_HEADER_0, 0x9D);
  Rfm22b_Write(RFM22B_TRANSMIT_PACKAGE_LENGTH, sizeof(RF_DataTypeDef));
  Rfm22b_Write(RFM22B_CHECK_HEADER_3, 0x05);
  Rfm22b_Write(RFM22B_CHECK_HEADER_2, 0xC6);
  Rfm22b_Write(RFM22B_CHECK_HEADER_1, 0xA9);
  Rfm22b_Write(RFM22B_CHECK_HEADER_0, 0x9D);
  Rfm22b_Write(RFM22B_HEADER_ENABLE_3, 0xFF);
  Rfm22b_Write(RFM22B_HEADER_ENABLE_2, 0xFF);
  Rfm22b_Write(RFM22B_HEADER_ENABLE_1, 0xFF);
  Rfm22b_Write(RFM22B_HEADER_ENABLE_0, 0xFF);
  Rfm22b_Write(RFM22B_MODULATION_MODE_CONTROL_1, 0x0F);
  Rfm22b_Write(RFM22B_MODULATION_MODE_CONTROL_2, 0x23);

}

static void Rfm22b_ClearTxFifo(void)
{
  Rfm22b_OperatingModeFunctionControl2(0x01);
  Rfm22b_OperatingModeFunctionControl2(0x00);
}

static void Rfm22b_ClearRxFifo(void)
{
  Rfm22b_OperatingModeFunctionControl2(0x02);
  Rfm22b_OperatingModeFunctionControl2(0x00);
}
#if 0
static void Rfm22b_SetDataLen(uint8_t cfg)
{
  Rfm22b_Write(RFM22B_TRANSMIT_PACKAGE_LENGTH, cfg);
}
#endif
static void Rfm22b_SendDataFifo(uint8_t data)
{
  Rfm22b_Write(RFM22B_FIFO_ACCESS, data);
}
static uint8_t Rfm22b_ReadDataFifo(void)
{
  return Rfm22b_Read(RFM22B_FIFO_ACCESS);
}
uint8_t Rfm22b_ReadSignalStrength(void)
{
  return Rfm22b_Read(RFM22B_RECEIVED_SIGNAL_STRENGTH_INDICATOR);
}

//---------------------------------------------
static void RFM22B_ToTxMode(RF_DataTypeDef Data)
{
  uint8_t i;
  Rfm22b_OperatingModeFunctionControl1(RFM22B_TUNE_MODE);
  Rfm22b_Read(RFM22B_INTERRUPT_STATUS_1);		// Read Interrupt status1 register
	Rfm22b_Read(RFM22B_INTERRUPT_STATUS_2);
  RFM22B_RXANT_OFF;
  RFM22B_TXANT_ON;
  Time=TO_GetCurrentTime();
	Rfm22b_ClearTxFifo();//Tx Fifo reset
	for (i=0; i<sizeof(RF_DataTypeDef); i++)
	{
    Rfm22b_SendDataFifo(*((uint8_t *)&Data+i));	// send payload to the FIFO
	}
  Rfm22b_OperatingModeFunctionControl1(RFM22B_TX_ON_IN_MANUAL_TX_MODE);
  Rfm22b.TxBusy=true;
}


static void Rfm22b_ToRxMode(void)
{
  Rfm22b_OperatingModeFunctionControl1(RFM22B_TUNE_MODE);
  Rfm22b_Read(RFM22B_INTERRUPT_STATUS_1);		// Read Interrupt status1 register
	Rfm22b_Read(RFM22B_INTERRUPT_STATUS_2);
  RFM22B_RXANT_ON;
  RFM22B_TXANT_OFF;
	Rfm22b_RxReset();
}

static void Rfm22b_RxReset(void)
{
	Rfm22b_ClearRxFifo();
  Rfm22b_OperatingModeFunctionControl1(RFM22B_RX_ON_IN_MAUAL_RX_MODE);
}
static bool RFM22B_TxIsBusy(void)
{
  return Rfm22b.TxBusy;
}

static void RFM22B_ClearTxBusy(void)
{
  Rfm22b.TxBusy=false;
}


void RFM22B_ISR(void)
{
  uint8_t iCount;
	uint8_t status;
  RF_DataTypeDef tmp;
  status=Rfm22b_Read(0x03);		// Read Interrupt status1 register
  Rfm22b_Read(0x04);
  if(status&0x02)/* Receive data */
  {
    for(iCount=0; iCount<sizeof(tmp); iCount++)
    {
      *((uint8_t *)&tmp+iCount)= Rfm22b_ReadDataFifo();
    }
    StoreRx(tmp);
    Rfm22b.ToRxMode=true;/* back again RX mode */
  }
  if(status&0x04)
  {
    Rfm22b.TxBusy=false;/* Module is not busy */
    Rfm22b.ToRxMode=true;/* back again RX mode */
    /* Calc time */
    Time=TO_GetCurrentTime()-Time;
  }
}



static void StoreRx(RF_DataTypeDef Data)
{
  if(!((Rx.Spos+1==Rx.Rpos)||((Rx.Spos+1==RF_MAX_BUF_RX)&&(!Rx.Rpos))))
  {
    memcpy((uint8_t *)&Rx.Data[Rx.Spos],(uint8_t *)&Data,sizeof(Rx.Data[Rx.Spos]));
    Rx.Spos++;
    if(Rx.Spos>=RF_MAX_BUF_RX){Rx.Spos=0;}; 
  }  


}
static RF_DataTypeDef  GetRx(void)
{
  RF_DataTypeDef tmp;
  if(Rx.Rpos!=Rx.Spos)
  {
    Rx.Rpos++;
    memcpy((uint8_t *)&tmp,(uint8_t *)&Rx.Data[Rx.Rpos-1],sizeof(tmp));
  }
  if(Rx.Rpos>=RF_MAX_BUF_RX){Rx.Rpos=0;};
  return tmp;
}
static bool CheckRx(void)
{
  if(Rx.Rpos==Rx.Spos)
  {
    return false;
  }
  else 
  {return true;};
}
static void ClearRx(void)
{
  uint16_t i;
  Rx.Spos=0;
  Rx.Rpos=0;
  for(i=0;i<RF_MAX_BUF_RX;i++)
  {
    memset((uint8_t *)&Rx.Data[i],0,sizeof(Rx.Data[i]));
  }
}

static void StoreTx(RF_DataTypeDef Data)
{
  if(!((Tx.Spos+1==Tx.Rpos)||((Tx.Spos+1==RF_MAX_BUF_TX)&&(!Tx.Rpos))))
  {
    memcpy((uint8_t *)&Tx.Data[Tx.Spos], (uint8_t *)&Data,sizeof(Tx.Data[Tx.Spos]));
    Tx.Spos++;
    if(Tx.Spos>=RF_MAX_BUF_TX){Tx.Spos=0;}; 
  }  


}
static RF_DataTypeDef  GetTx(void)
{
  RF_DataTypeDef tmp;
  if(Tx.Rpos!=Tx.Spos)
  {
    Tx.Rpos++;
    memcpy((uint8_t *)&tmp, (uint8_t *)&Tx.Data[Tx.Rpos-1],sizeof(tmp));
  }
  if(Tx.Rpos>=RF_MAX_BUF_TX){Tx.Rpos=0;};
  return tmp;
}
static bool CheckTx(void)
{
  if(Tx.Rpos==Tx.Spos)
  {
    return false;
  }
  else 
  {return true;};
}
static void ClearTx(void)
{
  uint16_t i;
  Tx.Spos=0;
  Tx.Rpos=0;
  for(i=0;i<RF_MAX_BUF_TX;i++)
  {
    memset((uint8_t *)&Tx.Data[i],0,sizeof(Tx.Data[i]));
  }
}


//static void RFM22B_TurnOff(void)
//{
//  RFM22B_OFF;
//}






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
