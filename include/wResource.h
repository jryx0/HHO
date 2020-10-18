#ifndef __RESOUCE_H__
#define __RESOUCE_H__
#include "macrodef.h"

//��׼�¼�����
#define EVENT_MOUSE 1000
#define EVENT_KEYBORAD 1010

#define EVENT_DATA 50
#define EVENT_LOG_INFO 1100

#define EVENT_PAGE_CHANGE 2000

///////////////////////////��ɫ����//////////////////////////////////////////////
#define COLOR_white 0XFFFF
#define COLOR_ligtgray 0xEF7D //0xAD55
#define COLOR_darkgray 0x52AA

//���ں궨��
//////////////////////Desktop windows defined////////////////////////////////
#define ID_DESKTOP 0

#define ID_MENU_LOG 1      //label
#define ID_MENU_HOMEPAGE 2 //button
#define ID_MENU_TESTPAGE 3 //button
#define ID_MENU_LOGOUT  4 //������ע��
#define ID_MENU_EXIT  20 // �˳�
//�Ƿ���desktop�ؼ����ֵ�������ж��Ƿ���desktop�ؼ�
#define ID_DESKTOP_MAX 30

//ҳ��궨��
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