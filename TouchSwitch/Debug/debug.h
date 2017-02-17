/**
  ******************************************************************************
  * @file   :debug
  * @author :DatLe
  * @version:v1_00 
  * @date   :   
  * @brief  :Header file for .c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f10x.h"
#include "common.h"
/**
@code  
 
@endcode
*/
 //#define __USING_DEBUG
/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup Exported_Constants
  * @{
  */
#define __USING_DEBUG
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
  
  #define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
  #define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
  #define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

  #define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
  #define TRCENA          0x01000000
#endif

#ifdef __USING_UART_DEBUG 
  #define USARTa                   USART2
  #define USARTa_GPIO              GPIOA
  #define USARTa_CLK               RCC_APB1Periph_USART2
  #define USARTa_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USARTa_TxPin             GPIO_Pin_2
	
  #define USARTa_BAUDRATE						115200



  #ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #endif /* __GNUC__ */
#endif
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
#ifdef __USING_UART_DEBUG
void UART_DEBUG_Init(void);
void UART_DBG_Print(char *data);

//void itoa(int n, char s[]);
///* reverse:  reverse string s in place */
//void reverse(char s[]);
#endif

/**
  * @}
  */ 

#ifdef __cplusplus
}


#endif /* ___H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */   

/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
