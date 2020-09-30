#ifndef __RESOUCE_H__
#define __RESOUCE_H__
#include "macrodef.h"
///////////////////////Graphics define/////////////////////////////////////

#if defined(SVGA64K)
#define CapColor 0x0015
#define Shade0 0xFFFF
#define Shade1 0xAD55
#define Shade2 0x52AA
#define Shade3 0
#define FontColor 0
#define desktopColor 0x0555
#define capFontColor 0xFFFF
#elif defined(SVGA32K)
#define CapColor 0x0015
#define Shade0 0x7FFF
#define Shade1 0x56B5
#define Shade2 0x294A
#define Shade3 0
#define FontColor 0
#define desktopColor 0x56B5
#define capFontColor 0x7FFF
#else
#define CapColor BLUE
#define Shade0 WHITE
#define Shade1 LIGHTGRAY
#define Shade2 DARKGRAY
#define Shade3 BLACK
#define FontColor BLACK
#define desktopColor CYAN
#define capFontColor WHITE
#endif

//////////////////////Desktop windows defined////////////////////////////////
#define EVENT_MOUSE 1
#define EVENT_KEYPRESS 2
#define EVENT_PAINT 3

#define EVENT_DATA 50

#define ID_DESKTOP 0
#define ID_WIN_LOG_PANEL 1
#define ID_WIN_LOGIN 2

//////////////////////Login windows defined////////////////////////////////
#define ID_LOGIN_REGISTER 1001
#define ID_LOGIN_LOGIN 1002
#define ID_LOGIN_EXIT 1003

#define EVENT_LOG_INFO 1100

#endif