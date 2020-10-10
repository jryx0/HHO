#ifndef __RESOUCE_H__
#define __RESOUCE_H__
#include "macrodef.h"

//标准事件定义
#define EVENT_MOUSE 1000
#define EVENT_KEYBORAD 1010
#define EVENT_DELETEWIN 5000
#define EVENT_DATA 50
#define EVENT_LOG_INFO 1100

///////////////////////////颜色定义//////////////////////////////////////////////
#define COLOR_white 0XFFFF
#define COLOR_ligtgray 0xEF7D //0xAD55
#define COLOR_darkgray 0x52AA

//////////////////////Desktop windows defined////////////////////////////////
#define ID_DESKTOP 0
#define ID_MENU_LOG 1       //label
#define ID_MENU_HOMEPAGE 2  //button
#define ID_MENU_TESTPAGE 3  //button


//是否点击desktop本身的控件
#define ID_DESKTOP_MAX 30 

////////////////////////////homepage//////////////////////////////////////////////
#define ID_HOMEPAGE 30

#define ID_LABEL_1 31
#define ID_LABEL_2 32
#define ID_LABEL_3 33

////////////////////test page///////////////////////////////////////////////////
#define ID_TESTPAGE 10000




//////////////////////Login windows defined////////////////////////////////
#define ID_LOGIN_REGISTER 1001
#define ID_LOGIN_LOGIN 1002
#define ID_LOGIN_EXIT 1003



#endif