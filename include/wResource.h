#ifndef __RESOUCE_H__
#define __RESOUCE_H__
#include "macrodef.h"

//标准事件定义
#define EVENT_MOUSE 1000
#define EVENT_KEYBORAD 1010

#define EVENT_DATA 50
#define EVENT_LOG_INFO 1100

#define EVENT_PAGE_CHANGE 2000

///////////////////////////颜色定义//////////////////////////////////////////////
#define COLOR_white 0XFFFF
#define COLOR_ligtgray 0xEF7D //0xAD55
#define COLOR_darkgray 0x52AA

//窗口宏定义
//////////////////////Desktop windows defined////////////////////////////////
#define ID_DESKTOP 0
#define ID_MENU_LOG 11      //label
#define ID_MENU_HOMEPAGE 12 //button
#define ID_MENU_TESTPAGE 13 //button
#define ID_MENU_LOGIN 14 //右上角登录状态

#define ID_MENU_PY 19
#define ID_MENU_EXIT 20 // 退出
//是否点击desktop控件最大值，用于判断是否点击desktop控件
#define ID_DESKTOP_MAX 30
///拼音输入法
///////////////////////////////////////////////////////////////////////////
#define ID_PINYIN 1

//页面宏定义
////////////////////////////homepage//////////////////////////////////////////////
#define ID_HOMEPAGE 30

#define ID_LABEL_1 31
#define ID_LABEL_2 32
#define ID_LABEL_3 33

////////////////////test page///////////////////////////////////////////////////
#define ID_TESTPAGE 10000
#define ID_TEST_HYPERLINK 10001
#define ID_TEST_CHECKBOX 10002
#define ID_TEST_TEXTBOX 10003
#define ID_TEST_TEXTBOX2 10004

//////////////////////Login windows defined////////////////////////////////
#define ID_LOGINPAGE 1000
#define ID_LOGIN_REGISTER 1001
#define ID_LOGIN_LOGIN 1002
#define ID_LOGIN_EXIT 1003

#endif