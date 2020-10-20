#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hglobal.h"
#include "homepage.h"
#include "wResource.h"
#include "hyperlnk.h"
#include <string.h>
#include <stdio.h>

void Homepage_MouseHandler(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  //确定是否是子窗口事件，非子窗口事件返回本身
  hitwin = checkmousewin(win, &_g->mouse);

  switch (hitwin->winID)
  {                 //鼠标在那个子窗口或本身
  case ID_HOMEPAGE: //本身
    if (type == EVENT_MOUSE)
    {                                                                              //homepage 处理鼠标移动理鼠标移动
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.rightClickState == MOUSE_BUTTON_UP)
      {
        // hbasewinAttr *child = CreateLabel(hitwin, random(800), random(600), 300, 150, ID_LABEL_3, NULL);
        // if (child)
        //   child->onPaint(child, NULL);
      }
    }
    break;
    /*
  case ID_LABEL_1: //label1
    if (type == EVENT_MOUSE)
    {                                                                             //label1 处理鼠标移动理鼠标移动
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //改变显示 鼠标左键释放
        if (hitwin && hitwin->onPaint)
          hitwin->onPaint(hitwin, "c:\\hho\\data\\news\\1.txt");
      }
    }
    break;

  case ID_LABEL_2: //label2
    if (type == EVENT_MOUSE)
    {                                                                             //label1 处理鼠标移动理鼠标移动
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //改变显示
        if (hitwin && hitwin->onPaint)
          hitwin->onPaint(hitwin, "main.c");
      }
    }
    break;
  case ID_LABEL_3: //label3
    if (type == EVENT_MOUSE)
    { //不改变鼠标形状
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        list_node_t *n;
        //删除label
        int x = getAbsoluteX(hitwin);
        int y = getAbsoluteY(hitwin);
        fillRegionEx(x, y, hitwin->nWidth + 1, hitwin->nHeight + 1, 0xFFFF); //清除子窗口区域

        TRACE(("%s(%d): 删除label%u\n", __FILE__, __LINE__, hitwin->winID));
        hitwin->onDestroy(hitwin, NULL);
      }
    }
    break;*/
  default:
    break;
  }
}

void EventHandler_homepage(hbasewinAttr *win, int type, void *value)
{
  //globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);
  //_g = (globaldef *)value;

  switch (type)
  {
  case EVENT_MOUSE:
    Homepage_MouseHandler(win, type, value);
    break;
  case EVENT_KEYBORAD:
    break;
  default:
    break;
  }
}

hbasewinAttr *CreateHomepage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "homepage");
  //hbasewinAttr *label;
  TESTNULL(page, NULL);

  // CreateLabel(page, 15, 90, 300, 150, ID_LABEL_1, "readme");
  // CreateLabel(page, 15 + 350, 90, 300, 150, ID_LABEL_2, "c:\\hho\\data\\news\\1.txt");
  // label = CreateLabel(page, 15 + 700, 90, 300, 150, ID_LABEL_3, NULL);
  // label->value = malloc(10);
  // strcpy((char *)label->value, "test");
  Createhyperlink(page, 310, 40, 450, 30, ID_HOMEPAGE_LINK1, "校医院职工大会强调：党建引领 练内功 做服务 办特色");
  Createhyperlink(page, 310, 40 + 24, 450, 30, ID_HOMEPAGE_LINK2, "【抗击新冠肺炎 华中大在行动】校医院临时后勤团队为一线人员提供坚实保障");
  Createhyperlink(page, 310, 40 + 24 * 2, 450, 30, ID_HOMEPAGE_LINK3, "【战疫坚守一线】校医院线上问诊送药到家，做师生健康守门人");
  Createhyperlink(page, 310, 40 + 24 * 3, 450, 30, ID_HOMEPAGE_LINK4, "【战疫坚守一线】校医院为教职工安全复岗保驾护航");
  Createhyperlink(page, 310, 40 + 24 * 4, 450, 30, ID_HOMEPAGE_LINK5, "校医院线上健康教育课启发学生新感受");
  Createhyperlink(page, 310, 40 + 25 * 5, 450, 30, ID_HOMEPAGE_LINK6, "校医院暖心服务高考阅卷");
  Createhyperlink(page, 310, 40 + 25 * 6, 450, 30, ID_HOMEPAGE_LINK7, "【别样毕业季】校医院为毕业生返校保驾护航");
  Createhyperlink(page, 310, 40 + 25 * 7, 450, 30, ID_HOMEPAGE_LINK8, "大“疫”当前 急诊人显担当");
 // Createhyperlink(page, 310, 40 + 25 * 8, 450, 30, ID_HOMEPAGE_LINK9, "【抗击新冠肺炎 华中大在行动】校医院周飞鹏：做师生健康的守门人");

  page->onPaint = OnPaint_homepage;
  page->EventHandler = EventHandler_homepage;

  return page;
}

void OnPaint_homepage(hbasewinAttr *win, void *value)
{
  FILE *fpfile;
  int x, y;

  hfont *_font;

  TESTNULLVOID(win);

  _font = getFont(SIMHEI, 24, 0x3244);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  printTextLineXY(x, y, "医院新闻", _font);
  linex_styleEx(x, y + 30, 770, 0x3244, 2, 1);

  printTextLineXY(x, y + 280, "就诊流程", _font);
  linex_styleEx(x, y + 310, 1000, 0x3244, 2, 1);

  printTextLineXY(x, y + 480, "智能医导", _font);
  linex_styleEx(x, y + 510, 1000, 0x3244, 2, 1);
  freeFont(_font);

  Putbmp64k(x, y + 40, "data\\news\\newspic2.bmp");

  _font = getFont(SIMHEI, 16, 0x3244);
  //printTextLineXY(x + 60, y + 250, "校医院职工大会强调...", _font);

  Putbmp64k(x + 40, y + 325, "data\\bmp\\flow-11.bmp");
  printTextLineXY(x + 20, y + 420, "选择医生挂号", _font);
  Putbmp64k(x + 190, y + 325, "data\\bmp\\flow-12.bmp");
  printTextLineXY(x + 180, y + 420, "门诊缴费", _font);

  Putbmp64k(x + 360, y + 325, "data\\bmp\\flow-13.bmp");
  printTextLineXY(x + 340, y + 420, "医生问诊", _font);

  Putbmp64k(x + 530, y + 325, "data\\bmp\\flow-14.bmp");
  printTextLineXY(x + 520, y + 420, "诊断结果", _font);

  Putbmp64k(x + 670, y + 325, "data\\bmp\\flow-12.bmp");
  printTextLineXY(x + 650, y + 420, "处方缴费", _font);

  Putbmp64k(x + 810, y + 325, "data\\bmp\\flow-15.bmp");
  printTextLineXY(x + 810, y + 420, "药房发药", _font);

   Putbmp64k(x + 910, y + 325, "data\\bmp\\flow-16.bmp");
  printTextLineXY(x + 910, y + 420, "快递信息", _font);

   

  repaintChildren(win, value);

  rectangleEx(x + 810, y + 30, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 40, "就诊须知", _font);

  rectangleEx(x + 810, y + 90, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 100, "操作指南", _font);

  rectangleEx(x + 810, y + 150, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 160, "投诉电话", _font);

  rectangleEx(x + 810, y + 210, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 220, "交通位置", _font);

  freeFont(_font);
  (void)value;
}
