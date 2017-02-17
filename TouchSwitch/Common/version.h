/**
  ******************************************************************************
  * @file   :version.h
  * @author :DatLe
  * @version:v1_00
  * @date   :17/12/2014     
  * @brief  :Header file for version.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VERSION_H
#define __VERSION_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f10x.h"
   
/**
@code  
 
@endcode
*/
/*
Version   Date         Author       Comment  
------------------------------------------------------------------------------------------------------------------------------------
1.01      25/12/2013   DatLe        1.Remove all RF handle flag
------------------------------------------------------------------------------------------------------------------------------------                                    
1.02      26/12/2013   DatLe        1.Upgrade Cap1214 library, add output gate for this module(cap1214Out) 
                                    and linker it to ACISData_TypeDef. Create new Cap1214_InputTask
                                    2.Add Enable flag for Cap1214 and RF
                                    3.Add handle control between Cap1214 and RF
                                    4.Change goto mode by Press A&B button
------------------------------------------------------------------------------------------------------------------------------------                                    
2.00      27/12/2013   DatLe	    1......
------------------------------------------------------------------------------------------------------------------------------------                                    
2.01	  03/01/2014     Dat Le	    1.Remove all re-config of Cap1214 when it turn off the backlight.
------------------------------------------------------------------------------------------------------------------------------------                                    
2.02	  04/01/2014     Dat Le	    1.Remove auto calibre function of Cap1214
------------------------------------------------------------------------------------------------------------------------------------                                    
2.03	  08/01/2014     Dat Le	    1.Adjust relay timming.
                                  2.ADC pin has problem, die. Vin is out of range. Replace 33k by 10k
------------------------------------------------------------------------------------------------------------------------------------
2.04	  16/01/2014     Dat Le	    1.Turn On auto recalibration.
                                  2.Disable auto resend when RF failed
------------------------------------------------------------------------------------------------------------------------------------
2.05	  17/01/2014     Dat Le	    1.Turn On hold for 4sec is auto-recalibration.
------------------------------------------------------------------------------------------------------------------------------------
3.00    12/02/2014     Dat Le     1.Add checking connection function
                                  2.Add checking version of device function
                                  3.Add setting reponse delay time from MCE, if not set from MCE, load default data.
                                    The maximum reponse delay time is not larger than 1/2 of script time in MCE.
                                  4.Add Turn ON and Turn OFF Backlight hand-shake command. MCE can turn ON and Turn OFF BL of device
                                  5.Add enable or disable Proximity function.
                                  6.Add BackLight Function
                                      .BL is controlled by Prox if using Prox function
                                      .BL is controlled by MCE, if not using Prox function BL is controlled by MCE automatically.
                                  7.Add auto reset for MCU
                                    Reset Time is received from MCE
                                    MCE can Reset Device immediately and can set Reset Time for device
------------------------------------------------------------------------------------------------------------------------------------                                    
3.01    21/02/2014
------------------------------------------------------------------------------------------------------------------------------------
3.02    22/02/2014    Dat Le      1.Add UPDATE_RE4 device name
------------------------------------------------------------------------------------------------------------------------------------
3.03    23/02/2014    DatLe       1.Add noise detect in Cap1214_ISR to filter data get from Cap
                                  2.Change sample of Proximity from 16->64 sample 
                                    It will change total time for proximity detect time from 38->161ms
                                  3.Add TouchPress to Rfm22b_OutputTask
                                    If Touch is pressed, wait until touch is release, transmit RF later
                                  4.Change delay time for button A,B LED is from 250->150
                                  5.Change Proximity sense from 64->32, sample from 16->32
                                  6.If updatetime=0, never update automatically
------------------------------------------------------------------------------------------------------------------------------------
3.04    24/02/2014    Dat Le      1.Chang default update time from 5min->30mins
------------------------------------------------------------------------------------------------------------------------------------
3.05    25/02/2014    DatLe       1.Recalibre zero detect
------------------------------------------------------------------------------------------------------------------------------------
3.06    25/02/2014    DatLe       1.Fix delay for relay on and off
                                  2.Add Led B display for some of setting
------------------------------------------------------------------------------------------------------------------------------------
3.07    26/02/2014    DatLe       1.Add some of config for Cap1214 and received touch config
------------------------------------------------------------------------------------------------------------------------------------
3.08    01/3/2014     DatLe       1.Add Multi touch setup for Cap1214 and update received touch config
------------------------------------------------------------------------------------------------------------------------------------
3.09    02/03/2014    DatLe       1.Fix initial MULTI_TOUCH_DF
                                  2.Add turn ON/OFF LED_BL indicator error
                                  3.Add Rf command to setup Error display and update error status
                                  4.Clear All button press flag before go to run mode again
                                  5.Fix error "no button to exit reset device mode", add button A to do this function
------------------------------------------------------------------------------------------------------------------------------------                                  
3.10    03/03/2014    DatLe       1.Recalibre zero detect
------------------------------------------------------------------------------------------------------------------------------------
3.11    11/03/2014    DatLe       1.Change default multi touch = 2
------------------------------------------------------------------------------------------------------------------------------------
3.12    14/03/2014    DatLe       1.Fix Relay4 control by remote, it control 4 like 3 
------------------------------------------------------------------------------------------------------------------------------------
4.00    11/03/2014    DatLe       1.Change PCB->Change new config pin for new PCB
------------------------------------------------------------------------------------------------------------------------------------
4.01    14/03/2014    DatLe       1.Fix Relay4 control by remote, it control 4 like 3 
------------------------------------------------------------------------------------------------------------------------------------
4.02    06/04/2014    DatLe       1.Add repeater enable/disable and upfate infor for its.
                                  2.Add lock/unlock for all control buttons and update infor for its.
                                  3.Fix repeater for remote
------------------------------------------------------------------------------------------------------------------------------------
4.03    11/04/2014    DatLe       1.Fix Saving of Repeater ON/OFF and button Lock 
------------------------------------------------------------------------------------------------------------------------------------
4.04    21/07/2014    DatLe       1.Add code to set blur LED indicate when turn OFF
------------------------------------------------------------------------------------------------------------------------------------
4.03    25/08/2014    DatLe       1.Hold status when power OFF
                                  2.Change default reset time 7 days->2 days
                                  3.Increase time TURN OFF -> TURN ON RF module when its detected error ->500ms (old 20ms)
------------------------------------------------------------------------------------------------------------------------------------
4.06    17/09/2014    DatLe       1.Remove hold status
------------------------------------------------------------------------------------------------------------------------------------
4.07    07/10/2014    DatLe       1.Change RF off time = 2000ms.
                                  2.Add function set hold status of any channel, defaut is always off
                                  3.Fix hold status initial, no action when power on, wait for zero detect OK
                                  4.If RF transmit fail, tuen OFF back light and turn on.
                                  5.Add display for one time RF error, BL on 2s and off 2s
                                  6.Update status 20mins->60 mins
------------------------------------------------------------------------------------------------------------------------------------                                  
RE4 FW VERSION 2 
1.00    17/12/2014    DatLe       1.Change to HW Version 2-->Re-Write code
1.01    26/12/2014    DatLe       1.Add function display current
                                  2.Add function to setup rooling time
                                  3.Add function to set LED Off or Blur
                                  4.Because of change touch sense so update touch driver
                                    


------------------------------------------------------------------------------------------------------------------------------------
RE4 HW VERSION 
2.00    17/12/2014    DatLe       1.Change to HW Version 2-->Add OLED and Current Sensor.
                                  2.HW is OK
2.01    17/12/2014    DatLe       1.Change to HW Version 2.1-->Add OLED and Current Sensor.
                                  2.Using Touch SENSE 9 and 10 to replace SENSE 8 and 9                                 


*/
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

/******************* (C) COPYRIGHT 2014 ACIS *****END OF FILE****/
