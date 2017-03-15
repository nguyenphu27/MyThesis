/**
  ******************************************************************************
  * @file:   cap1214.c
  * @author: Dai Ve
  * @version:v1.00
  * @date:   04-Sep-2014
  * @brief:  STM32xx-EVAL abstraction layer. 
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
#include "cap1214.h"

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
/**
  * @}
  */ 


/** @defgroup Private_Variables
  * @{
  */
static void     Cap1214_LL_Init (void);
static uint8_t  Cap1214_ReadByte(uint8_t Addr, uint8_t Reg, uint8_t *Data);
static uint8_t  Cap1214_SendData(uint8_t Addr,uint8_t *Data,uint32_t Len);
static void     Cap1214_StoreBuf(uint16_t Data);
static uint16_t Cap1214_GetBuf(void);
static bool     Cap1214_CheckBuf(void);
static void     Cap1214_ClearBuf(void);

  
  
static Cap1214_Buffer  Cap1214_Buff;

Button_DrvTypeDef Cap1214_Drv =
{
  Cap1214_Init,
  Cap1214_DeInit,
  Cap1214_ReadID,
  0,/* Reset */
  0,/* input task */
  
  Cap1214_CheckBuf,
  Cap1214_GetBuf,
  Cap1214_ClearBuf,
  
  0,/* Detect Press */
  0,/* Detect Release */
  0,/* Detect Hold */
  0,/* Detect Double Press */
  0,/* Clear Press */
  0,/* Clear Release */
  0,/* Clear Hold */
  0,/* Clear double press */
  
  0,/* Clear all data handle */
  
  Cap1214_ISR,
  
  0,//Cap1214_LoadDFConfig,
  0,//Cap1214_SetHoldTimeAll,
  0,//Cap1214_SetDoubleClickTimeAll,
  0,//Cap1214_SetHoldTime,
  0,//Cap1214_SetDoubleClickTime
  
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



/**
  * @brief : Init the LL of cap1214 function
  * @param : Init the i2C, ext interrupt, GPIO, Turn on Power....   
  * @retval : none
  */
static void Cap1214_LL_Init (void)
{
  GPIO_InitTypeDef    GPIO_InitStructure;
  //EXTI_InitTypeDef 	  EXTI_InitStructure;
	//NVIC_InitTypeDef 	  NVIC_InitStructure;
  I2C_InitTypeDef     I2C_InitStructure;
  
  /* Some of weird */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);
	/*##-1- Enable EN Pin ####################################################*/
  /* Configure the EN pin */
  CAP1214_EN_GPIO_CLK_ENABLE();
  GPIO_InitStructure.GPIO_Pin = CAP1214_EN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
  GPIO_Init(CAP1214_EN_GPIO_PORT, &GPIO_InitStructure);
  
  /*##-2- Enable I2C peripherals  ###########################################*/
  /* Configure the I2C pin */
  CAP1214_SCL_GPIO_CLK_ENABLE();
  CAP1214_SDA_GPIO_CLK_ENABLE();

  /*##-3- Configure peripheral GPIO ##########################################*/  
  /* I2C SDA and SCL configuration */
  GPIO_InitStructure.GPIO_Pin = CAP1214_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(CAP1214_SCL_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CAP1214_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(CAP1214_SDA_GPIO_PORT, &GPIO_InitStructure);
  
  /* Enable I2C1 clock */
  CAP1214_CLOCK_ENABLE();
  /*!< CAP1214 Periph clock enable */
  I2C_DeInit(CAP1214_I2C);

  /*!< CAP1214 I2C configuration  */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = CAP1214_I2C_SPEED ;
  
  
  
  I2C_Init(CAP1214_I2C, &I2C_InitStructure);
  
  /*enable I2C*/
  I2C_Cmd(CAP1214_I2C,ENABLE);

  /*##-4- INT pin ############################################################*/
  /* Enable INT pin clock */
	CAP1214_INT_GPIO_CLK_ENABLE();
	
	/*!< Configure RFM22B_SPI pins: INT */
  GPIO_InitStructure.GPIO_Pin = CAP1214_INT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
  GPIO_Init(CAP1214_INT_GPIO_PORT, &GPIO_InitStructure);

  #if 0
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTIx Line to INT pin */
  GPIO_EXTILineConfig(CAP1214_EXTI_PORT, CAP1214_EXTI_PIN	);

  /* Configure EXTIx line */			
  EXTI_InitStructure.EXTI_Line = CAP1214_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* Enable and set INT Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = CAP1214_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  #endif

}
/**
  * @brief  Configures Cap1214 GPIO and I2C interface.
  * @param  
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */
void Cap1214_Init (void)
{
	Cap1214_LL_Init();
  /* Reset device */
  CAP1214_RESET;
  Cap1214_SoftwareInit();
}



/**
  * @brief  DeInit Cap1214 GPIO and I2C interface.
  * @param   
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */
void Cap1214_DeInit (void)
{
  /* Turn off power */
  GPIO_WriteBit(CAP1214_EN_GPIO_PORT,CAP1214_EN_PIN,Bit_SET );
  
  /*##-1- Reset peripherals ##################################################*/


  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure I2C Tx as alternate function  */
  /* Configure I2C Rx as alternate function  */
}

/**
  * @brief : Read the ID of CAP1214 chip
  * @param : none  
  * @retval : the ID value (0x5A)
  */
uint8_t Cap1214_ReadID(void)
{
  //Cap1214_LL_Init();
  return Cap1214_ReadRegister(CAP1214_PRODUCT_ID);
}

/**
  * @brief  Reads a register of the device through BUS.
  * @param  Addr: Device address on BUS.  
  * @param  Reg: The target register address to write
  * @retval Data read at register address
  */
static uint8_t Cap1214_ReadByte(uint8_t Addr, uint8_t Reg, uint8_t *Data)
{
  volatile uint32_t I2C_timeOut = I2C_TIMEOUT;
	volatile uint8_t res,i;
	/* Disable the acknowledgement */
	for (i = 0;i < MAX_I2C_TRIES;  ++i)
	{
    //I2C_CheckSlaveStatus(slaveAddr);
		I2C1->CR1 &= ~I2C_CR1_ACK;
		/* Generate a START condition */
		I2C1->CR1 |= I2C_CR1_START;
		/* Test on I2C1 EV5 and clear it */
		I2C_timeOut = I2C_TIMEOUT;
		while (((I2C1->SR1&0x0001) != 0x0001) && --I2C_timeOut);
		if(I2C_timeOut == 0){I2C1->CR1 |= I2C_CR1_STOP; continue;}
		
		/* Send slave address for write */
		I2C1->DR = Addr & 0xfe;	
		 /* Test on I2C1 EV6  and clear it */
		I2C_timeOut = I2C_TIMEOUT;
		while (((I2C1->SR1 &0x0002) != 0x0002) && --I2C_timeOut);
		if(I2C_timeOut == 0){I2C1->CR1 |= I2C_CR1_STOP; continue;}
		/* Clear ADDR flag by reading SR2 register */
		res = I2C1->SR2;
		/*Send data*/
		I2C1->DR = Reg;
		/* Test on I2C1 EV8  and clear it */
		I2C_timeOut = I2C_TIMEOUT;
		while (((I2C1->SR1 & 0x00004) != 0x000004) && --I2C_timeOut);
		if(I2C_timeOut == 0){I2C1->CR1 |= I2C_CR1_STOP; continue;}
		/* Generate a START condition */
		I2C1->CR1 |= I2C_CR1_START;
		/* Test on I2C1 EV5 and clear it */
		I2C_timeOut = I2C_TIMEOUT;
		while (((I2C1->SR1&0x0001) != 0x0001) && --I2C_timeOut);
		if(I2C_timeOut == 0){I2C1->CR1 |= I2C_CR1_STOP; continue;}
		/* Send slave address for write */
		I2C1->DR = Addr | I2C_OAR1_ADD0;
		 /* Test on I2C1 EV6  and clear it */
		I2C_timeOut = I2C_TIMEOUT;
		while (((I2C1->SR1&0x0002) != 0x0002) && --I2C_timeOut);
		if(I2C_timeOut == 0){I2C1->CR1 |= I2C_CR1_STOP; continue;}
		I2C1->CR1 &= ~I2C_CR1_ACK;
		res = I2C1->SR2;
		/* Send I2C1 STOP Condition */
		I2C1->CR1 |= I2C_CR1_STOP;
		/* Wait until a data is received in DR register (RXNE = 1) EV7 */
		I2C_timeOut = I2C_TIMEOUT;
		while (((I2C1->SR1 & 0x00040) != 0x000040) && --I2C_timeOut);
		if(I2C_timeOut == 0){I2C1->CR1 |= I2C_CR1_STOP; continue;}
		*Data = (uint8_t)I2C1->DR;
		/* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
		I2C_timeOut = I2C_TIMEOUT;
		while (((I2C1->CR1&0x200) == 0x200) && --I2C_timeOut);
		if(I2C_timeOut == 0){I2C1->CR1 |= I2C_CR1_STOP; continue;}
		return 0;
		/* Enable Acknowledgement to be ready for another reception */
	}
	/* Send I2C1 STOP Condition */
	I2C1->CR1 |= I2C_CR1_STOP;
	I2C1->CR1 |= I2C_CR1_ACK;
	return 1;
}

/**
  * @brief : Send a brust byte data to register which has add
  * @param : 
  *        @arg addr: register address
  *        @arg data: ponter to data send
  *        @arg len: len of data to send 
  * @retval :
  */

static uint8_t Cap1214_SendData(uint8_t Addr,uint8_t *Data,uint32_t Len)
{
	volatile uint32_t I2C_timeOut = I2C_TIMEOUT,temp;
	/* Disable the acknowledgement */
	I2C1->CR1 &= ~I2C_CR1_ACK;
	/* Generate a START condition */
	I2C1->CR1 |= I2C_CR1_START;
	/* Test on I2C1 EV5 and clear it */
	while ((I2C1->SR1&0x0001) != 0x0001)
	{
		if(I2C_timeOut-- == 0){I2C1->CR1 |= I2C_CR1_STOP; return 0xff;}
	}
	I2C1->DR = Addr & 0xfe;	/* Send slave address for write */
	/* Test on I2C1 EV6  and clear it */
	I2C_timeOut = I2C_TIMEOUT;
	while ((I2C1->SR1 &0x0002) != 0x0002)
	{
		if(I2C_timeOut-- == 0){I2C1->CR1 |= I2C_CR1_STOP; return 0xff;}
	}
	/* Clear ADDR flag by reading SR2 register */
   temp = I2C1->SR2;
	I2C1->DR = *Data;
	Data++;
	Len--;
	while(Len--)
	{
		/* Test on I2C1 EV8  and clear it */
		I2C_timeOut = I2C_TIMEOUT;
		while ((I2C1->SR1 & 0x00004) != 0x000004)
		{
			if(I2C_timeOut-- == 0){I2C1->CR1 |= I2C_CR1_STOP; return 0xff;}
		}
		/*Send data*/
		I2C1->DR = *Data;
		Data++;
	}
	/* EV8_2: Wait until BTF is set before programming the STOP */
	I2C_timeOut = I2C_TIMEOUT;
  while ((I2C1->SR1 & 0x00004) != 0x000004)
	{
		if(I2C_timeOut-- == 0){I2C1->CR1 |= I2C_CR1_STOP; return 0xff;}
	}
	/* Send I2C1 STOP Condition */
	I2C1->CR1 |= I2C_CR1_STOP;
	/* Make sure that the STOP bit is cleared by Hardware */
	I2C_timeOut = I2C_TIMEOUT;
  while ((I2C1->CR1&0x200) == 0x200)
	{
		if(I2C_timeOut-- == 0){I2C1->CR1 |= I2C_CR1_STOP; return 0xff;}
	}
	return 0;
}
/*********************************************************************
 * Function:   uint8_t Cap1214_ReadRegister(uint8_t add);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           register address
 *
 * Output:          value of this register
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
uint8_t Cap1214_ReadRegister(uint8_t add)
{
	uint8_t tmp;	
	Cap1214_ReadByte(CAP1214_ADD,add,&tmp);
	return tmp;
}
/*********************************************************************
 * Function:   uint8_t Cap1214_WriteRegister(uint8_t add,uint8_t data);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           register address
 *
 * Output:          write data to this address
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
uint8_t Cap1214_WriteRegister(uint8_t add,uint8_t data)
{
  uint8_t tmp[2];
  tmp[0]=add;
  tmp[1]=data;
	if(Cap1214_SendData(CAP1214_ADD,tmp,2)){return CAP1214_BUS_BUSY;}
  else {return CAP1214_TRANSMIT_DONE;};
}

/* Hight Level ######################################################### */

/**
  * @brief : CAP1214_ADD register config
  * @param : none 
  * @retval : none
  */
void Cap1214_SoftwareInit(void)
{
	// Cau hinh che do hoat dong o Deactive
	Cap1214_SetMode(CAP1214_DEACTIVE| CAP1214_PWR_LED_ENABLE);
  Cap1214_LEDControl(0x00);

  Cap1214_SetTouch( CAP1214_GP_EN | CAP1214_S2_EN | CAP1214_S3_EN | CAP1214_S4_EN | CAP1214_S5_EN | CAP1214_S6_EN | CAP1214_S7_EN);
  Cap1214_ProximityControl(CAP1214_CS1_PROX_DIS | CAP1214_PROX_AVR_32 |CAP1214_PROX_D_SENSE_64);
  Cap1214_MultiTouchConfig(CAP1214_MULT_BLK_EN | CAP1214_B_MULT_T_4);
  Cap1214_SetSensitivity(CAP1214_SENSE_16x|CAP1214_BASE_SHIFT_256X);
  Cap1214_Config1(0x2B);//0x09//0x2A
  
  Cap1214_WriteRegister(CAP1214_GROUP_CONFIG_1,0x00);
  Cap1214_WriteRegister(CAP1214_GROUP_CONFIG_2,0xA1);
  
  Cap1214_Config2(0x02);
  Cap1214_ButtonConfig(0x74);//3360ms button can press after auto-recalibre 
  
  Cap1214_ReCalibrationConfig(0x13);//0x91//11
  Cap1214_CalibreEnable(0xFF);
  Cap1214_CalibreActive(0x04);
  
  Cap1214_SamplingConfig(0x00);//2.5ms
  Cap1214_QueueControl(0x03);//3 sample
  //Cap1214_SamplingConfig(0x00);//0.64ms
  //Cap1214_QueueControl(0x01);//1 sample
  
  Cap1214_Sensor1Threshold(0x40);
  Cap1214_Sensor2Threshold(0x40);
  Cap1214_Sensor3Threshold(0x40);
  Cap1214_Sensor4Threshold(0x40);
  Cap1214_Sensor5Threshold(0x40);
  Cap1214_Sensor6Threshold(0x40);
  Cap1214_Sensor7Threshold(0x40);
  Cap1214_GroupThreshold(0x40);
  
  //Cap1214_ButtonNoiseThreshold1(0xB8);//50%0xBA
  //Cap1214_ButtonNoiseThreshold2(0xAE);//50%
  
  Cap1214_ButtonNoiseThreshold1(0xFC);//50%
  Cap1214_ButtonNoiseThreshold2(0xFF);//50%
  Cap1214_InterruptEnable1(0xFF);
  Cap1214_InterruptEnable2(0x00);
  
  Cap1214_SetMode(CAP1214_ACTIVE | CAP1214_PWR_LED_ENABLE | CAP1214_INT_ENABLE);
}

/*ISR processing*/
void Cap1214_ISR(void)
{
	Num32 data;
  Cap1214_SetMode(CAP1214_ACTIVE | CAP1214_PWR_LED_ENABLE | CAP1214_CLEAR_INT);  
  if(!Cap1214_NoiseStatus1()&&!Cap1214_NoiseStatus2())
  {
    data.n8.n[1]=Cap1214_ReadRegister(CAP1214_BUTTON_STATUS_2);
    data.n8.n[0]=Cap1214_ReadRegister(CAP1214_BUTTON_STATUS_1);
    Cap1214_StoreBuf(data.n16.n[0]);
  }
}






/*********************************************************************
 * Function:        Cap1214_SetMode(uint8_t mode);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	ACTIVE/DEACTIVE/SLEEP/DSLEEP |
 *				PWR_LED_ENABLE|
 *				INT_ENABLE
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            	config Main Status Control Register (00h).
 ********************************************************************/
uint8_t Cap1214_SetMode(uint8_t mode)
{
	return Cap1214_WriteRegister(CAP1214_MAIN_STATUS_CONTROL, mode);
}

/*********************************************************************
 * Function:        Cap1214_Set_Touch(uint8_t btn);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:			GP_EN | S7_EN | S6_EN | S5_EN | S4_EN
 *				S3_EN | S2_EN | S1_EN
 *				(default FFh)
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
uint8_t Cap1214_SetTouch(uint8_t btn)
{
	return Cap1214_WriteRegister(CAP1214_SENSOR_ENABLE,btn | 0x00);
}

/*********************************************************************
 * Function:        Cap1214_Calib_Enable(uint8_t btn);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:			G_CEN | S7_CEN | S6_CEN | S5_CEN | S4_CEN
 *				S3_CEN | S2_CEN | S1_CEN
 *				(default FFh)
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_CalibEnable(uint8_t btn)
{
	Cap1214_WriteRegister(CAP1214_CALIB_ENABLE,btn);
}


/*********************************************************************
 * Function:        Cap1214_SetSensitivity(uint8_t sens_n_base);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	SENSE_nx | BASE_SHIFT_mx
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note: 			set Data Sensitivity Register
 ********************************************************************/
 void Cap1214_SetSensitivity(uint8_t sens_n_base)
{
	Cap1214_WriteRegister(CAP1214_DATA_SENSITIVITY,sens_n_base);
}

/*********************************************************************
 * Function:        Cap1214_Config1(uint8_t config_w);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	TIMEOUT_ENA |
 *				POS_VOL |
 *				DIS_DIG_NOISE |
 *				DIS_ANA_NOISE |
 *				MAX_DUR_EN_B  |
 *				RPT_EN_B |
 *				MAX_DUR_EN_G |
 *				RPT_EN_G |
 *				or DEFAULT_CONFIG_1
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note: 			set Configuration Register (20h)
 ********************************************************************/
uint8_t Cap1214_Config1(uint8_t config_w)
{
	return Cap1214_WriteRegister(CAP1214_CONFIGURATION, config_w);
}

/*********************************************************************
 * Function:        Cap1214_Config_2(uint8_t config_w);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	INV_LINK_TRAN |
 *				LED11_CFG |
 *				BLK_POL_MIR |
 *				SHOW_RF_NOISE |
 *				DIS_RF_NOISE |
 *				VOL_UP_DOWN |
 *				INT_REL_n
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note: 			set Configuration Register (20h)
 ********************************************************************/
 uint8_t Cap1214_Config2(uint8_t config_w)
{
	return Cap1214_WriteRegister(CAP1214_CONFIGURATION_2, config_w | 0x00);
}

/*********************************************************************
 * Function:        Cap1214_Config_2(uint8_t config_w);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	CS1_PROX_EN/CS1_PROX_DIS |
 *				PROX_SUM|
 *				PROX_AVR_16/32/64/128 |
 *				PROX_D_SENSE_1/2/4/8/16/32/64/128 |
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note: 			set Configuration Register (20h)
 ********************************************************************/
uint8_t Cap1214_ProximityControl(uint8_t cConfig)
{
	return Cap1214_WriteRegister(CAP1214_PROXIMITY_CONTROL,cConfig | 0x00);
}

/*********************************************************************
 * Function:        Cap1214_Calib_Activate(uint16_t channel);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	G_CAL | S7_CAL | S6_CAL | S5_CAL | S4_CAL |
 *				S3_CAL | S2_CAL | S1_CAL | S8_CAL | S9_CAL |
 *				S10_CAL | S11_CAL | S12_CAL | S13_CAL | S14_CAL
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_CalibActivate(uint16_t channel)
{
	uint8_t tmp;
	tmp = channel << 8;
	Cap1214_WriteRegister(0x46,tmp);
	tmp = channel >> 8;
	Cap1214_WriteRegister(0x26,tmp);
}

/*********************************************************************
 * Function:        Cap1214_Interrupt_Enable1(uint8_t channel);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:			GP_EN | S7_EN | S6_EN | S5_EN | S4_EN
 *				S3_EN | S2_EN | S1_EN
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
 void Cap1214_InterruptEnable1(uint8_t channel)
{
	Cap1214_WriteRegister(CAP1214_INTERRUPT_ENABLE_1,channel);
}

/*********************************************************************
 * Function:        Cap1214_Interrupt_Enable(uint8_t channel);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:			GPIO8 | GPIO7 | GPIO6 | GPIO5 | GPIO4 | GPIO3 | GPIO2 | GPIO1 
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
 void Cap1214_InterruptEnable2(uint8_t channel)
{
	Cap1214_WriteRegister(CAP1214_INTERRUPT_ENABLE_2,channel);
}

/*********************************************************************
 * Function:        Cap1214_Sleep_Channel_Control(uint8_t channel);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:			GP_EN | S7_EN | S6_EN | S5_EN | S4_EN
 *				S3_EN | S2_EN | S1_EN
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
 void Cap1214_SleepChannelControl(uint8_t channel)
{
	Cap1214_WriteRegister(CAP1214_SLEEP_CHANNEL_CONTROL,channel);
}

/*********************************************************************
 * Function:        Cap1214_Multi_Touch_Config(uint8_t cConfig);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:			MULT_BLK_EN | B_MULT_T | G_MULT_T
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
 void Cap1214_MultiTouchConfig(uint8_t cConfig)
{
	Cap1214_WriteRegister(CAP1214_MULT_PRESS_CONFIG,cConfig);
}

/*********************************************************************
 * Function:        Cap1214_LidClose_Config(uint8_t channel);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:			LID_CLOSE | COMP_PTRN | LID_ALRT
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
 void Cap1214_LidCloseConfig(uint8_t cConfig)
{
	Cap1214_WriteRegister(CAP1214_LID_CLOSURE_CONFIG,cConfig);
}

/*********************************************************************
 * Function:        Cap1214_Digital_ReCalib_Control(uint8_t cConfig);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:      		GP_EN | S7_EN | S6_EN | S5_EN | S4_EN
 *				S3_EN | S2_EN | S1_EN
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
 void Cap1214_DigitalReCalib_Control(uint8_t cConfig)
{
	Cap1214_WriteRegister(CAP1214_DIGITAL_RECALIB,cConfig);
}

/*********************************************************************
 * Function:        Cap1214_Set_Led_Direction(uint16_t led);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7 | LED8
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
 void Cap1214_SetLedOutput(uint16_t cConfig)
{
	Cap1214_WriteRegister(CAP1214_LED_GPIO_DIR,cConfig);
}

/*********************************************************************
 * Function:        Cap1214_Linked_Led_Transition_Control(uint8_t cLed);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	LED7_LTRAN | LED6_LTRAN | LED5_LTRAN | LED4_LTRAN |
 *				LED3_LTRAN | LED2_LTRAN | LED1_LTRAN
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          control the LED drive when the LED is linked to a Capacitive Touch sensor
 ********************************************************************/
void Cap1214_LinkedLedTransitionControl(uint8_t cLed)
{
	Cap1214_WriteRegister(CAP1214_LINKED_LED_TRANSITION_CONTROL_1,cLed);
}

/*********************************************************************
 * Function:		Cap1214_Sensor_LED_Linking(uint8_t cLed);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	UP_DOWN_LINK | CS7_LED7 | CS6_LED6 | CS5_LED5 |
 *				CS4_LED4 | CS3_LED3 | CS2_LED2 | CS1_LED1
 *				or NO_LINK
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          controls whether a Capacitive Touch Sensor is linked to an LED output
 ********************************************************************/
uint8_t Cap1214_SensorLEDLinking(uint8_t cLed)
{
	return Cap1214_WriteRegister(CAP1214_SENSOR_LED_LINKING,cLed);
}

/*********************************************************************
 * Function:		Cap1214_LED_Output_Type(uint8_t cLed);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:		LED8 | LED7 | LED6 | LED5 | LED4 | LED3 | LED2 | LED1
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          Control the type of output: "0" - open-drain, "1"-push-pull
 ********************************************************************/
void Cap1214_LEDOutputType(uint8_t cLed)
{
	Cap1214_WriteRegister(CAP1214_LED_GPIO_OUTPUT_TYPE,cLed);
}


/*********************************************************************
 * Function:		Cap1214_LED_Output_Control(uint8_t cLed);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:         LED8 | LED7 | LED6 | LED5 | LED4 | LED3 | LED2 | LED1
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          control the output state of the LED pins when they are configure as
 *			output and are not linked to sensor inputs.
 ********************************************************************/
uint8_t Cap1214_LEDOutputControl(uint8_t cLed)
{
	return Cap1214_WriteRegister(CAP1214_LED_OUTPUT_CONTROL_1,cLed);
}


/*********************************************************************
 * Function:		Cap1214_LED_Polarity(uint8_t cLed);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	LED8 | LED7 | LED6 | LED5 | LED4 | LED3 | LED2 | LED1
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          control the logical polarity of the LED outputs.s
 ********************************************************************/
void Cap1214_LEDPolarity(uint8_t cLed)
{
	Cap1214_WriteRegister(CAP1214_LED_POLARITY_1,cLed);
}

/*********************************************************************
 * Function:		Cap1214_LED_Mirror_Enable(uint8_t cLed);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	LED8 | LED7 | LED6 | LED5 | LED4 | LED3 | LED2 | LED1
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          determine the meaning of duty cycle settings when polarity is
 *			non-inverted for each LED channel.
 ********************************************************************/
void Cap1214_LEDMirrorEnable(uint8_t cLed)
{
	Cap1214_WriteRegister(CAP1214_LED_MIRROR_CONTROL_1,cLed);
}

/*********************************************************************
 * Function:		Cap1214_LED_Behavior_Control(uint8_t cLed1,
 *											uint8_t cLed2,
 *											uint8_t cLed3);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	cLed1: LED4_mode | LED3_mode | LED2_mode | LED1_mode
 *				cLed2: LED8_mode | LED7_mode | LED6_mode | LED5_mode
 *				cLed1: LED11_mode | LED10_mode | LED9_mode
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_LEDBehaviorControl(uint8_t cLed1, uint8_t cLed2, uint8_t cLed3)
{
	Cap1214_WriteRegister(CAP1214_LED_BEHAVIOR_1,cLed1);
	Cap1214_WriteRegister(CAP1214_LED_BEHAVIOR_2,cLed2);
	Cap1214_WriteRegister(CAP1214_LED_BEHAVIOR_3,cLed3);
}

/*********************************************************************
 * Function:		Cap1214_LED_Pulse1_Period(uint8_t cConfig);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	ST_TRIG_TOUCH/ST_TRIG_RELEASE | <period>
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          determines the overall period of a pulse operation as determined
 *			by the LED_CTL registers
 *			<period> from 32ms to 4064ms
 ********************************************************************/
void Cap1214_LEDPulse1Period(uint8_t cConfig)
{
	Cap1214_WriteRegister(CAP1214_LED_PULSE_1_PERIOD,cConfig);
}

/*********************************************************************
 * Function:		Cap1214_LED_Pulse2_Period(uint8_t cPeriod);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	uint8_t
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          determines the overall period of a pulse operation as determined by
 *			the LED_CTL registers.
 ********************************************************************/
void Cap1214_LEDPulse2Period(uint8_t cPeriod)
{
	Cap1214_WriteRegister(CAP1214_LED_PULSE_2_PERIOD,cPeriod);
}

/*********************************************************************
 * Function:		Cap1214_LED_Breathe_Period(uint8_t cPeriod);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	uint8_t
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          determines the overall period of a breathe operation as determined
 *			by the LED_CTL registers.
 ********************************************************************/
void Cap1214_LEDBreathePeriod(uint8_t cPeriod)
{
	Cap1214_WriteRegister(CAP1214_LED_BREATHE_PERIOD,cPeriod);
}

/*********************************************************************
 * Function:		Cap1214_LED_Cnfig(uint8_t cConfig);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	RAMP_ALERT | PULSE2_CNT_x | PULSE1_CNT_x
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:          control the number of pulses that are sent for the Pulse1 and Pulse2
 ********************************************************************/
void Cap1214_LEDConfig(uint8_t cConfig)
{
	Cap1214_WriteRegister(CAP1214_LED_CONFIG,cConfig);
}

/*********************************************************************
 * Function:		Cap1214_LED_Pulse1_DutyCycle(uint8_t minDuty,
 *											uint8_t maxDuty);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	minDuty : 4bit
 *				maxDuty : 4bit
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_LEDPulse1DutyCycle(uint8_t maxDuty, uint8_t minDuty)
{
	uint8_t tmp;
	tmp = minDuty & (maxDuty<<4);
	Cap1214_WriteRegister(CAP1214_LED_PULSE_1_DUTY_CYCLE,tmp);
}

/*********************************************************************
 * Function:		Cap1214_LED_Pulse2_DutyCycle(uint8_t minDuty,
 *											uint8_t maxDuty);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	minDuty : 4bit
 *				maxDuty : 4bit
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_LEDPulse2DutyCycle(uint8_t maxDuty, uint8_t minDuty)
{
	uint8_t tmp;
	tmp = minDuty & (maxDuty<<4);
	Cap1214_WriteRegister(CAP1214_LED_PULSE_2_DUTY_CYCLE,tmp);
}

/*********************************************************************
 * Function:		Cap1214_LED_Breathe_DutyCycle(uint8_t minDuty,
 *											uint8_t maxDuty);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	minDuty : 4bit
 *				maxDuty : 4bit
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_LEDBreatheDutyCycle(uint8_t minDuty, uint8_t maxDuty)
{
	uint8_t tmp;
	tmp = minDuty & (maxDuty<<4);
	Cap1214_WriteRegister(CAP1214_LED_BREATHE_DUTY_CYCLE,tmp);
}

/*********************************************************************
 * Function:		Cap1214_LED_Direct_DutyCycle(uint8_t minDuty,
 *											uint8_t maxDuty);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	minDuty : 4bit
 *				maxDuty : 4bit
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_LEDDirectDutyCycle(uint8_t minDuty, uint8_t maxDuty)
{
	uint8_t tmp;
	tmp = minDuty & (maxDuty<<4);
	Cap1214_WriteRegister(CAP1214_LED_DIRECT_DUTY_CYCLE,tmp);
}

/*********************************************************************
 * Function:		Cap1214_LED_Off_Delay(uint8_t cConfig);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	BR_OFF_DLY_x | DIR_OFF_DLY_x
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
void Cap1214_LEDOffDelay(uint8_t cConfig)
{
	Cap1214_WriteRegister(CAP1214_LED_OFF_DELAY,cConfig);
}

/*********************************************************************
 * Function:		Cap1214_Product_ID(void);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	none
 *
 * Output:          Product ID (0x5A)
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
uint8_t Cap1214_Product_ID(void)
{
	return(Cap1214_ReadRegister(CAP1214_PRODUCT_ID));
}

/*********************************************************************
 * Function:		Cap1214_Touch_Register1(void);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:           	none
 *
 * Output:          Product ID
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:
 ********************************************************************/
uint8_t Cap1214_TouchRegister1(void)
{
	return(Cap1214_ReadRegister(CAP1214_BUTTON_STATUS_1));
}

/*********************************************************************
 * Function:   Cap1214_LED_Direction(uint8_t cConfig);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:		LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7 | LED8
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note: 		controls the data flow direction for the LED / GPIO pins
 ********************************************************************/
uint8_t Cap1214_LEDDirection(uint8_t cConfig)
{
	return Cap1214_WriteRegister(CAP1214_LED_GPIO_DIR,cConfig);
}

/*********************************************************************
 * Function:   Cap1214_LED_Control(uint8_t cConfig);
 *
 * PreCondition:    Cap1214_Init();
 *
 * Input:		LED1_ON/OFF | LED2_ON/OFF |
 *			LED3_ON/OFF | LED4_ON/OFF |
 *			LED5_ON/OFF | LED6_ON/OFF |
 *			LED7_ON/OFF | LED8_ON/OFF
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note: 		controls the data flow direction for the LED / GPIO pins
 ********************************************************************/
uint8_t Cap1214_LEDControl(uint8_t cLed)
{
	return Cap1214_WriteRegister(CAP1214_LED_OUTPUT_CONTROL_1,cLed);
}

void Cap1214_ButtonConfig(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_BUTTON_CONFIG,cfg);
}

void Cap1214_SensorEnable(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_ENABLE,cfg);
}

void Cap1214_CalibreEnable(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_CALIB_ENABLE,cfg);
}

void Cap1214_CalibreActive(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_CALIB_ACTIVATE,cfg);
}

void Cap1214_ReCalibrationConfig(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_RECALIB_CONFIG,cfg);
}

void Cap1214_SamplingConfig(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SAMPLING_CONFIG,cfg);
}

void Cap1214_QueueControl(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_QUEUE_CONTROL,cfg);
}

void Cap1214_Sensor1Threshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_1_THRESHOLD,cfg);
}

void Cap1214_Sensor2Threshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_2_THRESHOLD,cfg);
}

void Cap1214_Sensor3Threshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_3_THRESHOLD,cfg);
}

void Cap1214_Sensor4Threshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_4_THRESHOLD,cfg);
}

void Cap1214_Sensor5Threshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_5_THRESHOLD,cfg);
}

void Cap1214_Sensor6Threshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_6_THRESHOLD,cfg);
}

void Cap1214_Sensor7Threshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_7_THRESHOLD,cfg);
}

void Cap1214_GroupThreshold(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_SENSOR_GROUP_THRESHOLD,cfg);
}

void Cap1214_ButtonNoiseThreshold1(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_BUTTON_NOISE_1_THRESHOLD,cfg);
}

void Cap1214_ButtonNoiseThreshold2(uint8_t cfg)
{
  Cap1214_WriteRegister(CAP1214_BUTTON_NOISE_2_THRESHOLD,cfg);
}

uint8_t Cap1214_NoiseStatus1(void)
{
  return Cap1214_ReadRegister(CAP1214_NOISE_STATUS_1);
}
uint8_t Cap1214_NoiseStatus2(void)
{
  return Cap1214_ReadRegister(CAP1214_NOISE_STATUS_2);
}

void Cap1214_ReadDelta(uint8_t *Data)
{
  uint8_t i;
  for(i=0;i<14;i++)
  {
    *(Data+i)=Cap1214_ReadRegister(CAP1214_SENSOR1_DELTA_COUNT+i);  
  }  
}
/* Buffer ############################################################################## */
/**
  * @brief : Buffer store data
  * @param : Data need to store  
  * @retval : none
  */
static void     Cap1214_StoreBuf(uint16_t Data)
{
  if((Cap1214_Buff.Spos+1==Cap1214_Buff.Rpos)||((Cap1214_Buff.Spos+1==CAP1214_MAX_BUFF)&&(!Cap1214_Buff.Rpos)))
  {
    return;
  }
  Cap1214_Buff.Data[Cap1214_Buff.Spos]=Data;
  Cap1214_Buff.Spos++;
  if(Cap1214_Buff.Spos==CAP1214_MAX_BUFF){Cap1214_Buff.Spos=0;};	
}

/**
  * @brief : get data from buffer
  * @param : none  
  * @retval : data get
  */
static uint16_t Cap1214_GetBuf(void)
{
  uint16_t data=0; 
  if(Cap1214_Buff.Rpos!=Cap1214_Buff.Spos)
  {
    Cap1214_Buff.Rpos++; 
    data=Cap1214_Buff.Data[Cap1214_Buff.Rpos-1]; 
  }
  if(Cap1214_Buff.Rpos==CAP1214_MAX_BUFF){Cap1214_Buff.Rpos=0;};
  return data;
}

/**
  * @brief : Check buffer is empty or not
  * @param :   
  * @retval : true: has data, false: empty
  */
static bool     Cap1214_CheckBuf(void)
{
  if(Cap1214_Buff.Rpos==Cap1214_Buff.Spos)
  {
    return false;
  }
  else 
  {
    return true;
  };
}

/**
  * @brief : CLEAR all data store in buffer and reset spos and rpos
  * @param : none  
  * @retval : none
  */
static void     Cap1214_ClearBuf(void)
{ 
  uint16_t i;
  Cap1214_Buff.Spos=0;
  Cap1214_Buff.Rpos=0;
  for(i=0;i<CAP1214_MAX_BUFF;i++)
  {
    Cap1214_Buff.Data[i]=0;
  }
}
/* Buffer ############################################################################## */






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
