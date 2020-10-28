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
#define ID_MENU_HOMEPAGE 12 //首页button
#define ID_MENU_TESTPAGE 13 //测试button
#define ID_MENU_DEPTPAGE 14 //科室简介button
#define ID_MENU_LOGIN 18    //右上角登录状态

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

#define ID_HOMEPAGE_LINK1 31
#define ID_HOMEPAGE_LINK2 32
#define ID_HOMEPAGE_LINK3 33
#define ID_HOMEPAGE_LINK4 34
#define ID_HOMEPAGE_LINK5 35
#define ID_HOMEPAGE_LINK6 36
#define ID_HOMEPAGE_LINK7 37
#define ID_HOMEPAGE_LINK8 38
#define ID_HOMEPAGE_LINK9 39
#define ID_HOMEPAGE_LINK10 40
#define ID_HOMEPAGE_AISEARCH 41
#define ID_HOMEPAGE_AIBUTTON 42

#define ID_HOMEPAGE_BUTTON1 43
#define ID_HOMEPAGE_BUTTON2 44
#define ID_HOMEPAGE_BUTTON3 45
#define ID_HOMEPAGE_BUTTON4 46

#define ID_HOMEPAGE_REGS 50
#define ID_HOMEPAGE_CHARGE1 51
#define ID_HOMEPAGE_DIAGNOSE 52
#define ID_HOMEPAGE_RESULT 53
#define ID_HOMEPAGE_CHARGE2 54
#define ID_HOMEPAGE_DRUG 55
#define ID_HOMEPAGE_LOGISTICS 56

////////////////////test page///////////////////////////////////////////////////
#define ID_TESTPAGE 10000
#define ID_TEST_HYPERLINK 10001
#define ID_TEST_CHECKBOX 10002
#define ID_TEST_TEXTBOX 10003
#define ID_TEST_TEXTBOX2 10004

//////////////////////News page defined////////////////////////////////
#define ID_NEWSPAGE 500
#define ID_NEWS_RETURN 503
#define ID_NEWS_TEXT 505

////////////////////////////Dept Info page//////////////////////////////////////////////
#define ID_DEPTPAGE 600
#define ID_DEPT_RETURN 601
#define ID_DEPT_TEXT 602
#define ID_DEPT_DOCLINK 603 //转跳医生挂号
#define ID_DEPT_LINK 610

//////////////////////Login windows defined////////////////////////////////
#define ID_LOGINPAGE 1000
#define ID_LOGIN_REGISTER 1001
#define ID_LOGIN_LOGIN 1002
#define ID_LOGIN_RETURN 1003
#define ID_LOGIN_USERNAME 1004
#define ID_LOGIN_KEY 1005

////////////////////////////registerpage//////////////////////////////////////////////
#define ID_REGISTERPAGE 2000     //挂号页面
#define ID_REGISTER_RETURN 2001  //首页
#define ID_REGISTER_DISEASE 2002 //病情描述
#define ID_REGISTER_OK 2003 //确认挂号

// #define ID_REGISTER_DEPT_MEDICINE 2002      //内科
// #define ID_REGISTER_DEPT_SURGERY 2003       //外科
// #define ID_REGISTER_DEPT_PAEDIATRICS 2004   //儿科
// #define ID_REGISTER_DEPT_GYNAECOLOGY 2005   //妇科
// #define ID_REGISTER_DEPT_OPHTHALMOLOGY 2006 //眼科
// #define ID_REGISTER_DEPT_ENT 2007           //耳鼻喉科
// #define ID_REGISTER_DEPT_STOMATOLOGY 2008   //口腔科
// #define ID_REGISTER_DEPT_DERMATOLOGY 2009   //皮肤科

////////////////////////////物流 page//////////////////////////////////////////////
#define ID_POSTPAGE 2500
#define ID_POST_RETURN 2501
#define ID_POST_PREV 2502
#define ID_POST_NEXT 2503
#define ID_POST_UPDATEINFO 2504
#define ID_POST_UPDATEADDR 2505
#define ID_POST_UPDATESTATUS 2506

#define ID_POST_QUERY_BILLNUM 2510
#define ID_POST_QUERY_SHIPPER 2511
#define ID_POST_QUERY_SHIPPERTEL 2512
#define ID_POST_QUERY_RECEIVER 2513

#define ID_POST_LINK 2550

////////////////////////////药房 page//////////////////////////////////////////////
#define ID_DRUGPAGE 3000 //取药-发药页面
#define ID_DRUG_RETURN 3001

#define ID_DRUG_PSCHK 3400
#define ID_DRUG_PSLINK 3500
#define ID_DRUG_PREV 3550
#define ID_DRUG_NEXT 3551

#define ID_DRUG_STOCK 3552   //查看库存
#define ID_DRUG_SHIP 3553    //发货
#define ID_DRUG_CONFIRM 3554 //缴费

// #define ID_DRUG_ADDITEM 3552
// #define ID_DRUG_DELITEM 3553

////////////////////////////处方缴费 page//////////////////////////////////////////////
#define ID_PAYPSPAGE 4000 //取药-发药页面
#define ID_PAYPS_RETURN 4001

#define ID_PAYPS_PSCHK 4400
#define ID_PAYPS_PSLINK 4500
#define ID_PAYPS_PREV 4550
#define ID_PAYPS_NEXT 4551

#define ID_PAYPS_CONFIRM 4554 //缴费

////////////////////////////查询医生 page//////////////////////////////////////////////
#define ID_FINDDOCPAGE 5000
#define ID_FINDDOC_RETURN 5001

#define ID_FINDDOC_DOC 5020
#define ID_FINDDOC_DOCMAX 5040
#define ID_FINDDOC_DOCLINK 5050
#define ID_FINDDOC_DOCLINKMAX 5070

////////////////////////////医生问诊 page//////////////////////////////////////////////
#define ID_DOCPAGE 5100
#define ID_DOC_RETURN 5101
#define ID_DOC_TEXTBOX_RESULT 5102
#define ID_DOC_TEXTBOX_HANDLER 5103

#define ID_DOC_QUERY_DRUG 5104
#define ID_DOC_QUERY 5105

#define ID_DOC_PSCHK 5200
#define ID_DOC_PSLINK 5200

// #define ID_DOC_PREV 5550
// #define ID_DOC_NEXT5551

#define ID_DOC_REGSCHK 5300  //挂号checkbox
#define ID_DOC_REGSLINK 5350 //挂号link

#define ID_DOC_DRUGCHK 5400  //查询药品chekbox
#define ID_DOC_DRUGLINK 5450 //查询药品link

#define ID_DOC_PSDRUGCHK 5500  //处方药品chekbox
#define ID_DOC_PSDRUGLINK 5550 //处方药品link

#define ID_DOC_BTN_GEN 5701     //生成处方
#define ID_DOC_BTN_CONFIRM 5702 //诊断完成
#define ID_DOC_ADDDRUG 5703     //添加药品
#define ID_DOC_DELDRUG 5704     //删除药品

///////////////未授权页面/////////////////////////////
// #define ID_DENYPAGE 60000

#endif