/**
  ******************************************************************************
  * @file    common.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdint.h" 
#include "stdbool.h" 
#include "string.h"

typedef struct
{
  void (*Init)(void); /* Call back to first time call function */
  void (*Handle)(void); /* function handle */
}HANDLE_ProcessTypeDef;

typedef struct
{
  uint8_t x;
  uint8_t y;
}Coordinate_TypeDef;

/* Exported types ------------------------------------------------------------*/
typedef union
{
  uint32_t n32;
  struct 
  {
    uint8_t n[4]; 
  }n8;
  struct 
  {
    uint16_t n[2]; 
  }n16;
  struct 
  {
    uint32_t bit31:1;
    uint32_t bit30:1;
    uint32_t bit29:1;
    uint32_t bit28:1;
    uint32_t bit27:1;
    uint32_t bit26:1;
    uint32_t bit25:1;
    uint32_t bit24:1;
    uint32_t bit23:1;
    uint32_t bit22:1;
    uint32_t bit21:1;
    uint32_t bit20:1;
    uint32_t bit19:1;
    uint32_t bit18:1;
    uint32_t bit17:1;
    uint32_t bit16:1;
    uint32_t bit15:1;
    uint32_t bit14:1;
    uint32_t bit13:1;
    uint32_t bit12:1;
    uint32_t bit11:1;
    uint32_t bit10:1;
    uint32_t bit9:1;
    uint32_t bit8:1;
    uint32_t bit7:1;
    uint32_t bit6:1;
    uint32_t bit5:1;
    uint32_t bit4:1;
    uint32_t bit3:1;
    uint32_t bit2:1;
    uint32_t bit1:1;
    uint32_t bit0:1;
  }bits;
}Num32;

typedef union
{
  uint16_t n16;
  struct 
  {
    uint8_t n[2]; 
  }n8;
  struct 
  {
    uint16_t bit15:1;
    uint16_t bit14:1;
    uint16_t bit13:1;
    uint16_t bit12:1;
    uint16_t bit11:1;
    uint16_t bit10:1;
    uint16_t bit9:1;
    uint16_t bit8:1;
    uint16_t bit7:1;
    uint16_t bit6:1;
    uint16_t bit5:1;
    uint16_t bit4:1;
    uint16_t bit3:1;
    uint16_t bit2:1;
    uint16_t bit1:1;
    uint16_t bit0:1;
  }bits;
}Num16;

typedef union
{
  uint8_t n8;
  struct
  {
    uint8_t bcd1:4;
    uint8_t bcd0:4;
  }n4;
  struct 
  {
    uint8_t bit7:1;
    uint8_t bit6:1;
    uint8_t bit5:1;
    uint8_t bit4:1;
    uint8_t bit3:1;
    uint8_t bit2:1;
    uint8_t bit1:1;
    uint8_t bit0:1;
  }bits;
}Num8;

typedef struct
{
  bool      Enable;/*enable or disable Error checking module*/
  bool      Display;/*enable or disable error display*/
  bool      Error;/*indicate the error status*/
  bool      ErrorFlag;/*time to error occur*/
  uint32_t  Cnt;/*error conter*/
}Error_TypeDef;

typedef struct
{
  uint8_t   Hour;
  uint8_t   Min;
  uint8_t   Sec;
  uint8_t   DayWeek;
  uint8_t   Day;
  uint8_t   Month;
  uint16_t  Year;
}Time_TypeDef;



typedef struct
{
  int x;
  int y;
}Location_TypeDef;


/* define for device */
#define ALL         '0'
#define RE          '1'
#define REM         '2'
#define SB          '3'

#define RE_MAX      90
#define REM_MAX     20
#define SB_MAX      30
/* Exported constants --------------------------------------------------------*/
/* Exported marco ------------------------------------------------------------*/

#define MIN(a,b)            ((a<b)?a:b)
#define MAX(a,b)            ((a>b)?a:b)
  
void itoa(int n, char s[]); 
void reverse_string(char s[]);
uint32_t reverse(uint32_t x);
uint32_t u32_ReadUniqueID(void);    



#endif /* __COMMON_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
