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
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include <rtthread.h>
//#include <stddef.h>
//#include <stdio.h>
//#include "N9H30.h"
//#include "TouchPanel.h"
#include "GUI.h"
#include "GUIDRV_Lin.h"
//#include "GUIDRV_FlexColor.h"
#include "LCDConf.h"
#include "lcd.h"


/*********************************************************************
*
*       Layer configuration
*
**********************************************************************
*/
//
// Buffers / VScreens
//
#define NUM_BUFFERS  1 // Number of multiple buffers to be used
#define NUM_VSCREENS 1 // Number of virtual screens to be used

//
// Orientation
//
//#define DISPLAY_ORIENTATION (0)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y | GUI_SWAP_XY)


/*********************************************************************
*
*       Configuration checking
*
**********************************************************************/

#ifndef   DISPLAY_ORIENTATION
    #define DISPLAY_ORIENTATION 0
#endif

// Touch panel
//#define GUI_TOUCH_AD_LEFT   72
//#define GUI_TOUCH_AD_TOP    926  //82
//#define GUI_TOUCH_AD_RIGHT  938
//#define GUI_TOUCH_AD_BOTTOM 82  //926

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
extern uint8_t *g_VAFrameBuf;
extern int ts_phy2log(int *sumx, int *sumy);

void GUI_TOUCH_X_ActivateX(void)
{

}

void GUI_TOUCH_X_ActivateY(void)
{

}

int  GUI_TOUCH_X_MeasureX(void)
{
    return -1;
}

int  GUI_TOUCH_X_MeasureY(void)
{
    return -1;
}
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/

static rt_device_t g_LCDDev = RT_NULL;
static struct rt_device_graphic_info g_sRTGraphicInfo = {0};

void LCD_X_Config(void)
{
    const LCD_API_COLOR_CONV *pColorConvAPI;
    const GUI_DEVICE_API *pDeviceAPI;

    if (!g_LCDDev)
    {
        g_LCDDev = rt_device_find("lcd");
        if (!g_LCDDev)
        {
            LOG_E("find %s failed!", "lcd");
            goto exit_LCD_X_Config;
        }
        if (rt_device_open(g_LCDDev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
        {
            LOG_E("Fail to open %s!", "lcd");
            goto exit_LCD_X_Config;
        }
    }

    if (rt_device_control(g_LCDDev, RTGRAPHIC_CTRL_GET_INFO, &g_sRTGraphicInfo) != RT_EOK)
    {
        LOG_E("fail to get RT-Graphics information!");
        goto exit_LCD_X_Config;
    }

    switch (g_sRTGraphicInfo.pixel_format)
    {
    case RTGRAPHIC_PIXEL_FORMAT_ARGB888:
    case RTGRAPHIC_PIXEL_FORMAT_RGB888:
        pColorConvAPI = GUICC_M888;
        pDeviceAPI = GUIDRV_LIN_32;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_RGB565:
    default:
        pDeviceAPI = GUIDRV_LIN_16;
        pColorConvAPI = GUICC_M565;
        break;
    }


    /* LCD calibration */
#if (NUM_BUFFERS > 1)
    GUI_MULTIBUF_Config(NUM_BUFFERS);
#endif

    /* Set display driver and color conversion for 1st layer */
    GUI_DEVICE_CreateAndLink(pDeviceAPI, pColorConvAPI, 0, 0);
    if (LCD_GetSwapXY())
    {
        LCD_SetSizeEx(0, g_sRTGraphicInfo.height, g_sRTGraphicInfo.width);
        LCD_SetVSizeEx(0, g_sRTGraphicInfo.height * NUM_VSCREENS, g_sRTGraphicInfo.width);
    }
    else
    {
        LCD_SetSizeEx(0, g_sRTGraphicInfo.width, g_sRTGraphicInfo.height);
        LCD_SetVSizeEx(0, g_sRTGraphicInfo.width, g_sRTGraphicInfo.height * NUM_VSCREENS);
    }
    LCD_SetVRAMAddrEx(0, (void *)g_sRTGraphicInfo.framebuffer);

    //
    // Set user palette data (only required if no fixed palette is used)
    //
#if defined(PALETTE)
    LCD_SetLUTEx(0, PALETTE);
#endif

// LCD calibration
//
// Calibrate touch screen
//
//  GUI_TOUCH_Calibrate(GUI_COORD_X, 0, g_sRTGraphicInfo.width, 0, g_sRTGraphicInfo.width);
//  GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, g_sRTGraphicInfo.height, 0, g_sRTGraphicInfo.height);

exit_LCD_X_Config:

    return;
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void *pData)
{
    int r;

    switch (Cmd)
    {
    //
    // Required
    //
    case LCD_X_INITCONTROLLER:
    {
        //
        // Called during the initialization process in order to set up the
        // display controller and put it into operation. If the display
        // controller is not initialized by any external routine this needs
        // to be adapted by the customer...
        //
        //LCD_ON();
        //TODO - rt_ioctl LCD_ON;
        return 0;
    }
    case LCD_X_SETVRAMADDR:
    {
        //
        // Required for setting the address of the video RAM for drivers
        // with memory mapped video RAM which is passed in the 'pVRAM' element of p
        //
        LCD_X_SETVRAMADDR_INFO *p;
        p = (LCD_X_SETVRAMADDR_INFO *)pData;
        pData = (void *)g_sRTGraphicInfo.framebuffer;
        GUI_USE_PARA(p);
        //...
        return 0;
    }
    case LCD_X_SETORG:
    {
        //
        // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
        //
        LCD_X_SETORG_INFO *p;
        p = (LCD_X_SETORG_INFO *)pData;
        pData = (void *)g_sRTGraphicInfo.framebuffer;
        GUI_USE_PARA(p);
        //...
        return 0;
    }
    case LCD_X_SHOWBUFFER:
    {
        //
        // Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
        //
        LCD_X_SHOWBUFFER_INFO *p;
        p = (LCD_X_SHOWBUFFER_INFO *)pData;
        GUI_USE_PARA(p);
        //...
        return 0;
    }
    case LCD_X_SETLUTENTRY:
    {
        //
        // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
        //
        LCD_X_SETLUTENTRY_INFO *p;
        p = (LCD_X_SETLUTENTRY_INFO *)pData;
        GUI_USE_PARA(p);
        //...
        return 0;
    }
    case LCD_X_ON:
    {
        //
        // Required if the display controller should support switching on and off
        //
        return 0;
    }
    case LCD_X_OFF:
    {
        //
        // Required if the display controller should support switching on and off
        //
        return 0;
    }
    default:
        r = -1;
    }
    return r;
}

/*************************** End of file ****************************/
