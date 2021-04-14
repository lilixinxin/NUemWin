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
File    : Global.h
Purpose : Global types
          In case your application already has a Global.h, you should
          merge the files. In order to use Segger code, the types
          U8, U16, U32, I8, I16, I32 need to be defined in Global.h;
          additional definitions do not hurt.
Revision: $Rev: 9374 $
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GLOBAL_H            // Guard against multiple inclusion
#define GLOBAL_H

#define U8    unsigned char
#define I8    signed char
#define U16   unsigned short
#define I16   signed short
#ifdef __x86_64__
    #define U32   unsigned
    #define I32   int
#else
    #define U32   unsigned long
    #define I32   signed long
#endif

#if defined(_WIN32) && !defined(__clang__) && !defined(__MINGW32__)
    //
    // Microsoft VC6 compiler related
    //
    #define U64   unsigned __int64
    #define U128  unsigned __int128
    #define I64   __int64
    #define I128  __int128
    #if _MSC_VER <= 1200
        #define U64_C(x) x##UI64
    #else
        #define U64_C(x) x##ULL
    #endif
#else
    //
    // C99 compliant compiler
    //
    #define U64   unsigned long long
    #define I64   signed long long
    #define U64_C(x) x##ULL
#endif

#ifndef PTR_ADDR
    #if (defined(_WIN64) || defined(__LP64__))  // 64-bit symbols used by Visual Studio and GCC, maybe others as well.
        #define PTR_ADDR  U64
    #else
        #define PTR_ADDR  U32
    #endif
#endif

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
