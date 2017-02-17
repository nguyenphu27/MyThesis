/**
  ******************************************************************************
  * @file   :save
  * @author :DatLe
  * @version:v1_00 
  * @date   :   
  * @brief  :Header file for save.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SAVE_H
#define __SAVE_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   #include "common.h" 
   #include "relay.h"
   #include "config.h"

/**
@code  
 
@endcode
*/
/* Define Default value */
#define DEVICE_ID_DF                            0x0000
#define DEVICE_SUBNAME_DF                       0x0000

#define RE4_DEVICE_TOTAL_DF                     0x0000
#define REM_DEVICE_TOTAL_DF                     0x0000
#define SB6_DEVICE_TOTAL_DF                     0x0000

#define RE4_DEVICE_TOTAL_MAX                    RE_MAX
#define REM_DEVICE_TOTAL_MAX                    REM_MAX
#define SB6_DEVICE_TOTAL_MAX                    SB_MAX

#define RF_RESPONSE_TIME_DF                     320
                                                
#define SAVE_SETTING_PARTITION_OFFSET           0x0800FC00
#define SAVE_SETTING_BK_PARTITION_OFFSET        (SAVE_SETTING_PARTITION_OFFSET-0x400)

#define SAVE_DATA_PARTITION_OFFSET              (SAVE_SETTING_BK_PARTITION_OFFSET-0x400)
#define SAVE_DATA_BK_PARTITION_OFFSET           (SAVE_DATA_PARTITION_OFFSET-0x400)

/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
typedef struct
{
  uint32_t  DeviceID;
  uint8_t   Debug;
  uint8_t   Name;
  uint16_t  SubName;
  
  uint16_t  RE4Total;
  uint16_t  REMTotal;
  uint16_t  SB6Total;
  uint16_t  RFResponseTime;

  uint32_t  Checksum;
}Setting_TypeDef;

typedef struct
{
  uint8_t   RemoteChannel[REM_MAX][RELAY_NUM];
  uint8_t   RemoteScript[REM_MAX][SCRIPT_NUM];
  uint8_t   REScript[SCRIPT_NUM][RE_MAX];
  uint32_t  Checksum;
}Data_TypeDef;

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
void Save_SettingDF(Setting_TypeDef * Setting);
uint32_t Save_GenSettingChecksum(Setting_TypeDef * Setting);
void Save_Setting(Setting_TypeDef * Setting);
uint8_t Load_Setting(Setting_TypeDef * Setting);
void Save_SettingBk(Setting_TypeDef * Setting);
uint8_t Load_SettingBk(Setting_TypeDef * Setting);

void Save_DataDF(Data_TypeDef * Data);
uint32_t Save_GenDataChecksum(Data_TypeDef * Data);
void Save_Data(Data_TypeDef * Data);
uint8_t Load_Data(Data_TypeDef * Data);
void Save_DataBk(Data_TypeDef * Data);
uint8_t Load_DataBk(Data_TypeDef * Data);


/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


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
