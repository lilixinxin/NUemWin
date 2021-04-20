/*********************************************************************
*                 SEGGER Software GmbH                               *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.10 - Graphical user interface for embedded applications **
All  Intellectual Property rights in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with the following terms:

The  software has  been licensed by SEGGER Software GmbH to Nuvoton Technology Corporationat the address: No. 4, Creation Rd. III, Hsinchu Science Park, Taiwan
for the purposes  of  creating  libraries  for its
Arm Cortex-M and  Arm9 32-bit microcontrollers, commercialized and distributed by Nuvoton Technology Corporation
under  the terms and conditions  of  an  End  User
License  Agreement  supplied  with  the libraries.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              Nuvoton Technology Corporation, No. 4, Creation Rd. III, Hsinchu Science Park, 30077 Hsinchu City, Taiwan
Licensed SEGGER software: emWin
License number:           GUI-00735
License model:            emWin License Agreement, signed February 27, 2018
Licensed platform:        Cortex-M and ARM9 32-bit series microcontroller designed and manufactured by or for Nuvoton Technology Corporation
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2018-03-26 - 2020-03-27
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Global data
*/
static rt_mutex_t osMutex = RT_NULL;
static rt_sem_t osSemaphore = RT_NULL;

/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime()
*                 GUI_X_Delay(int)

  Some timing dependent routines require a GetTime
  and delay function. Default time unit (tick), normally is
  1 ms.
*/

GUI_TIMER_TIME GUI_X_GetTime(void)
{
    return (GUI_TIMER_TIME)rt_tick_get();
}

void GUI_X_Delay(int ms)
{
    rt_thread_mdelay(ms);
}

void GUI_X_ExecIdle(void)
{
    GUI_X_Delay(1);
}

/*********************************************************************
*
*      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/

void GUI_X_Log(const char *s)
{
    GUI_USE_PARA(s);
}
void GUI_X_Warn(const char *s)
{
    GUI_USE_PARA(s);
}
void GUI_X_ErrorOut(const char *s)
{
    GUI_USE_PARA(s);
}

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()
*
* Note:
*   The following routines are required only if emWin is used in a
*   true multi task environment, which means you have more than one
*   thread using the emWin API.
*   In this case the
*                       #define GUI_OS 1
*  needs to be in GUIConf.h
*/
void GUI_X_Init(void)
{
}

void GUI_X_InitOS(void)
{
    /* Create the Mutex used by the two threads */
    if (!osMutex)
    {
        osMutex = rt_mutex_create("NUemWin", RT_IPC_FLAG_FIFO);
        RT_ASSERT(osMutex != RT_NULL);
    }

    /* Create the Semaphore used by the two threads */
    if (!osSemaphore)
    {
        osSemaphore = rt_sem_create("NUemWin", 1, RT_IPC_FLAG_FIFO);
        RT_ASSERT(osSemaphore != RT_NULL);
    }
}

void GUI_X_Unlock(void)
{
    rt_mutex_release(osMutex);
}

void GUI_X_Lock(void)
{
    rt_err_t ret;
    ret = rt_mutex_take(osMutex, RT_WAITING_FOREVER);
    RT_ASSERT(ret == RT_EOK);
}

U32 GUI_X_GetTaskId(void)
{
    return ((U32)rt_thread_self());
}

/*********************************************************************
*
*      Event driving (optional with multitasking)
*
*                 GUI_X_WaitEvent()
*                 GUI_X_WaitEventTimed()
*                 GUI_X_SignalEvent()
*/

void GUI_X_WaitEvent(void)
{
    rt_err_t ret;
    ret = rt_sem_take(osSemaphore, RT_WAITING_FOREVER);
    RT_ASSERT(ret == RT_EOK);
}

void GUI_X_SignalEvent(void)
{
    rt_sem_release(osSemaphore);
}

void GUI_X_WaitEventTimed(int Period)
{
    rt_sem_take(osSemaphore, Period);
}

/*************************** End of file ****************************/
