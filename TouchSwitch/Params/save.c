/**
  ******************************************************************************
  * @file    :Save.c
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
#include "save.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */
Setting_TypeDef Setting;
Data_TypeDef Data;
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
void Save_SettingDF(Setting_TypeDef * Setting)
{
  /* 
  uint32_t  DeviceID;
  uint16_t  Reserved0;
  uint16_t  SubName;
  
  uint16_t  RE4Total;
  uint16_t  REMTotal;
  uint16_t  SB6Total;
  uint16_t  Reserved1;
  */
  Setting->DeviceID=u32_ReadUniqueID();
  Setting->Debug=1;
  Setting->Name=RE;
  Setting->SubName=1;
  Setting->RE4Total=1;
  Setting->REMTotal=0;
  Setting->SB6Total=0;
  
  Setting->RFResponseTime=RF_RESPONSE_TIME_DF;
  
  Setting->Checksum=Save_GenSettingChecksum(Setting);
  Save_Setting(Setting);
  Save_SettingBk(Setting);
}
uint32_t Save_GenSettingChecksum(Setting_TypeDef * Setting)
{
  uint16_t i;
  uint32_t sum;
  sum=0;
  
  for(i=0;i<(sizeof(Setting_TypeDef)-4);i++)
  {
    sum+=*(((uint8_t *)Setting)+i);
  }
  sum+=0x2805;
  return sum;
}
void Save_Setting(Setting_TypeDef * Setting)
{
  uint32_t i;
  Setting->Checksum=Save_GenSettingChecksum(Setting);
  /* Save to previous flash sector */  
  FLASH_Unlock();
  while(FLASH_ErasePage(SAVE_SETTING_PARTITION_OFFSET)!=FLASH_COMPLETE);
  for(i=0;i<(sizeof(Setting_TypeDef)/4);i++)
  {
    FLASH_ProgramWord(SAVE_SETTING_PARTITION_OFFSET+4*i,*(((uint32_t *)Setting)+i));
  }
  FLASH_Lock();
}
uint8_t Load_Setting(Setting_TypeDef * Setting)
{
  uint32_t i;
  uint32_t Tmp;
  for(i=0;i<sizeof(Setting_TypeDef)/4;i++)
  {
    *(((uint32_t *)Setting)+i)=(*(__IO uint32_t*)(SAVE_SETTING_PARTITION_OFFSET+4*i));
  }
  /* Calculate Checksum */
  Tmp=Save_GenSettingChecksum(Setting);
  if(Tmp==Setting->Checksum)
  {
    return 1;
  }
  else
  {
    return 0;
  };
}

void Save_SettingBk(Setting_TypeDef * Setting)
{
  uint32_t i;
  Setting->Checksum=Save_GenSettingChecksum(Setting);
  /* Save to previous flash sector */
  FLASH_Unlock();
  while(FLASH_ErasePage(SAVE_SETTING_BK_PARTITION_OFFSET)!=FLASH_COMPLETE);
  for(i=0;i<(sizeof(Setting_TypeDef)/4);i++)
  {
    FLASH_ProgramWord(SAVE_SETTING_BK_PARTITION_OFFSET+4*i,*(((uint32_t *)Setting)+i));
  }
  FLASH_Lock();
}
uint8_t Load_SettingBk(Setting_TypeDef * Setting)
{
  uint32_t i;
  uint32_t Tmp;
  for(i=0;i<sizeof(Setting_TypeDef)/4;i++)
  {
    *(((uint32_t *)Setting)+i)=(*(__IO uint32_t*)(SAVE_SETTING_BK_PARTITION_OFFSET+4*i));
  }
  /* Calculate Checksum */
  Tmp=Save_GenSettingChecksum(Setting);
  if(Tmp==Setting->Checksum)
  {
    return 1;
  }
  else
  {
    return 0;
  };
}


/* ---------------------Save for remote------------------------------ */
void Save_DataDF(Data_TypeDef * Data)
{
  memset((uint8_t *)Data,0,sizeof(Data_TypeDef));
  Data->Checksum=Save_GenDataChecksum(Data);
  Save_Data(Data);
  Save_DataBk(Data);
}
uint32_t Save_GenDataChecksum(Data_TypeDef * Data)
{
  uint16_t i;
  uint32_t sum;
  sum=0;
  
  for(i=0;i<(sizeof(Data_TypeDef)-4);i++)
  {
    sum+=*(((uint8_t *)Data)+i);
  }
  sum+=0x2805;
  return sum;
}
void Save_Data(Data_TypeDef * Data)
{
  uint32_t i;
  Data->Checksum=Save_GenDataChecksum(Data);
  /* Save to previous flash sector */  
  FLASH_Unlock();
  while(FLASH_ErasePage(SAVE_DATA_PARTITION_OFFSET)!=FLASH_COMPLETE);
  for(i=0;i<(sizeof(Data_TypeDef)/4);i++)
  {
    FLASH_ProgramWord(SAVE_DATA_PARTITION_OFFSET+4*i,*(((uint32_t *)Data)+i));
  }
  FLASH_Lock();
}
uint8_t Load_Data(Data_TypeDef * Data)
{
  uint32_t i;
  uint32_t Tmp;
  for(i=0;i<sizeof(Data_TypeDef)/4;i++)
  {
    *(((uint32_t *)Data)+i)=(*(__IO uint32_t*)(SAVE_DATA_PARTITION_OFFSET+4*i));
  }
  /* Calculate Checksum */
  Tmp=Save_GenDataChecksum(Data);
  if(Tmp==Data->Checksum)
  {
    return 1;
  }
  else
  {
    return 0;
  };
}

void Save_DataBk(Data_TypeDef * Data)
{
  uint32_t i;
  Data->Checksum=Save_GenDataChecksum(Data);
  /* Save to previous flash sector */
  FLASH_Unlock();
  while(FLASH_ErasePage(SAVE_DATA_BK_PARTITION_OFFSET)!=FLASH_COMPLETE);
  for(i=0;i<(sizeof(Data_TypeDef)/4);i++)
  {
    FLASH_ProgramWord(SAVE_DATA_BK_PARTITION_OFFSET+4*i,*(((uint32_t *)Data)+i));
  }
  FLASH_Lock();
}
uint8_t Load_DataBk(Data_TypeDef * Data)
{
  uint32_t i;
  uint32_t Tmp;
  for(i=0;i<sizeof(Data_TypeDef)/4;i++)
  {
    *(((uint32_t *)Data)+i)=(*(__IO uint32_t*)(SAVE_DATA_BK_PARTITION_OFFSET+4*i));
  }
  /* Calculate Checksum */
  Tmp=Save_GenDataChecksum(Data);
  if(Tmp==Data->Checksum)
  {
    return 1;
  }
  else
  {
    return 0;
  };
}






/* --------------------------utility----------------------------- */


    
/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
