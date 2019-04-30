/**
  ******************************************************************************
  * @file    :debug.c
  * @author  :DatLe
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
#include "debug.h"

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
#ifdef __USING_DEBUG
//struct __FILE { int handle; /* Add whatever is needed */ };
//FILE __stdout;
//FILE __stdin;

//int fputc(int ch, FILE *f) {
//  if (DEMCR & TRCENA) {
//    while (ITM_Port32(0) == 0);
//    ITM_Port8(0) = ch;
//  }
//  return(ch);
//}
#endif

#ifdef __USING_UART_DEBUG
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
struct __FILE { int handle; /* Add whatever is needed */ };
FILE __stdout;
FILE __stdin;
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  USART_SendData(USARTa, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USARTa, USART_FLAG_TC) == RESET)
  {}

  return ch;
}


/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void UART_DEBUG_Init(void)
{  
  /* USARTy and USARTz configured as follow:
        - BaudRate = 19200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USARTa_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USARTa Clock */
  RCC_APB1PeriphClockCmd(USARTa_CLK, ENABLE); 
  
  /* Configure USARTa Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTa_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTa_GPIO, &GPIO_InitStructure);
	
  USART_InitStructure.USART_BaudRate = USARTa_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;

  /* Configure USARTa */
  USART_Init(USARTa, &USART_InitStructure);
  
  /* Enable the USARTa */
  USART_Cmd(USARTa, ENABLE);
  
  /* Output a message on Hyperterminal using printf function */
  printf(">>>>>>>>>>>>>UART debug for RE mini device <<<<<<<<<<<<<<\r\n");
}

void UART_DBG_Print(char *data)
{
  static uint32_t Cnt=0;
  static char tmp[200];
  static char tmp1[20];
  
  memset(tmp,0,100);
  memset(tmp1,0,20);
  Cnt++;
  itoa(Cnt,tmp1);
  strcpy(tmp,">");
  strcat(tmp,tmp1);
  strcat(tmp,": ");
  strcat(tmp,data);
  strcat(tmp,"\r\n");
  
  printf("%s",tmp);
}


///* itoa:  convert n to characters in s */
//void itoa(int n, char s[])
//{
//    int i, sign;

//    if ((sign = n) < 0)  /* record sign */
//        n = -n;          /* make n positive */
//    i = 0;
//    do {       /* generate digits in reverse order */
//        s[i++] = n % 10 + '0';   /* get next digit */
//    } while ((n /= 10) > 0);     /* delete it */
//    if (sign < 0)
//        s[i++] = '-';
//    s[i] = '\0';
//    reverse(s);
//}

///* reverse:  reverse string s in place */
//void reverse(char s[])
//{
//    int i, j;
//    char c;

//    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
//        c = s[i];
//        s[i] = s[j];
//        s[j] = c;
//    }
//}


#endif


    
/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
