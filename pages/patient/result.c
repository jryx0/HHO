

#include "hhosvga.h"
#include "HBaseWin.h"
#include "hbutton.h"
#include "wResource.h"
#include "macrodef.h"
#include "hglobal.h"
#include "hyperlnk.h"
#include "hlabel.h"
#include "data.h"
#include "list.h"
#include "result.h"

hbasewinAttr *CreateResultPage(hbasewinAttr *parent, int winID, int userid)
{
  WinStyle *style;
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "registerpage");
  hbasewinAttr *label;
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_ResultPage;
  page->EventHandler = EventHandler_resultpage;
  style = malloc(sizeof(WinStyle));
  getWinTheme(style, 1);
  page->style = style;
  style->type = userid;

  Createhyperlink(page, 20, 10, 65, 25, ID_RESULT_RETURN, "[首 页]");
  CreateButton(page, 638, 510, 100, 32, ID_RESULT_OK, "确   认");

  return page;
}

//绘制挂号列表窗口
void drawResultRegister(int x, int y)
{
  rectangleEx(x + 20, y + 40, 330, 200, 0x6BAF, 1, 1);
  //fillRegionEx(x + 2, y + 320, 320, 200, 0x0000);

  lineyEx(x + 80, y + 40, 200, 0x6BAF);
  lineyEx(x + 150, y + 40, 200, 0x6BAF);
  lineyEx(x + 225, y + 40, 200, 0x6BAF);
  lineyEx(x + 290, y + 40, 200, 0x6BAF);
  linex_styleEx(x + 20, y + 70, 330, 0x6BAF, 1, 1);
}
void drawResultRegisterInfo(hbasewinAttr *win)
{
  int x = getAbsoluteX(win);
  int y = getAbsoluteY(win);
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0000);

  printTextLineXY(x + 20, y + 250, "挂号时间:", _h);
  printTextLineXY(x + 20, y + 280, "病情描述:", _h);

  printTextLineXY(x + 370, y + 45, "诊    断:", _h);
  rectangleEx(x + 460, y + 38, 510, 32, 0x6BAF, 1, 1);
  printTextLineXY(x + 370, y + 80, "治疗/处置:", _h);
  rectangleEx(x + 370, y + 110, 600, 130, 0x6BAF, 1, 1);
  printTextLineXY(x + 380, y + 250, "处方:", _h);

  freeFont(_h);
}
void fillResultRegisterInfo(hbasewinAttr *win, int regsid)
{
  int x, y;
  RegisterInfo *rgs;
  //PatientInfo *paInfo;
  MedicalRecord *mrecord;
  //WinStyle *style;
  char info[32];
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  //style = (WinStyle *)win->style;

  _h = getFont(SIMHEI, 16, 0x0000);
  //获取挂号信息
  rgs = fFindRegisterInfo(REGISTRATIONFILE, regsid);
  if (rgs)
  {
    mrecord = fFindMedicalRecordbyregId(MEDICALRECORDFILE, rgs->id);
    if (mrecord)
    {
      hregion _r;
      printTextLineXY(x + 100, y + 250, rgs->datetime, _h);
      printTextLineXY(x + 465, y + 43, mrecord->diagnosis, _h);
      _r.left_top.x = x + 370 + 5;
      _r.left_top.y = y + 110 + 5;
      _r.right_bottom.x = _r.left_top.x + 600 - 10;
      _r.right_bottom.y = _r.left_top.y + 130 - 10;
      printTextEx(&_r, mrecord->handler, _h, 0);

      _r.left_top.x = x + 20 + 5;
      _r.left_top.y = y + 300 + 5;
      _r.right_bottom.x = _r.left_top.x + 330 - 10;
      _r.right_bottom.y = _r.left_top.y + 160 - 10;
      rectangleEx(x + 20, y + 300, 330, 150, 0x6BAF, 1, 1);

      printTextEx(&_r, rgs->disease, _h, 0);
      free(mrecord);
    }
    free(rgs);
  }
  freeFont(_h);
}
//填充挂号单列表
void fillResultRegisterList(hbasewinAttr *win, int userid)
{
  hbasewinAttr *lnk;
  int i, j;
  list_t *rgslist, *palist, *doclist;
  list_node_t *node;
  RegisterInfo *regs;
  PatientInfo *pa;
  DoctorInfo *doc;
  WinStyle *style;

  char *status[] = {"未缴费", "已缴费", "已完成"};

  TESTNULLVOID(win);
  style = (WinStyle *)win->style;
  // x = getAbsoluteX(win);
  // y = getAbsoluteY(win);

  rgslist = ReadRegistration(REGISTRATIONFILE);
  TESTNULLVOID(rgslist);
  palist = ReadPatientInfo(PATIENTINFOFILE);
  doclist = ReadDoctorInfo(DOCTORINFOFILE);

  //清空link数据
  for (i = ID_PAYREGS_LINK; i < ID_PAYREGS_LINK + 8; i++)
  {
    lnk = findWinByID(win, i);
    if (lnk && lnk->title)
    {
      free(lnk->title);
      lnk->title = NULL;
    }
  }

  for (i = 0, j = 0; i < rgslist->len && i < 8; i++)
  { //一次最多8行, page控制显示那个8个
    hbasewinAttr *lnk = findWinByID(win, ID_PAYREGS_LINK + j);
    node = list_at(rgslist, i);
    regs = (RegisterInfo *)node->val;

    //只显示本人
    if (regs->userid != userid)
      continue;

    //控制显示全部或未缴费单据
    if (style->height == 0 && regs->status != 0)
      continue;

    if (lnk == NULL)
    { //创建lnk
      lnk = Createhyperlink(win, 15, 75 + 25 * j, 340, 25, ID_PAYREGS_LINK + j, NULL);
      lnk->wintype = HYPERLINK_BK;
      //CreateCheckBox(win, 10, 95 + 8 + 25 * j, 10, 10, ID_PAYREGS_CHK + j, NULL);
    }

    if (lnk->title)
      free(lnk->title);
    lnk->title = malloc(256);
    memset(lnk->title, 0, 256);

    pa = FindPatientInfo(palist, regs->userid);    //患者
    doc = FindDoctorInfo(doclist, regs->doctorid); //医生

    if (pa && doc)
    {
      sprintf(lnk->title, " %-7d%-8s%-8s%-8s%s", regs->id, pa->name, regs->dept, doc->name, status[regs->status]);
      lnk->data = regs->id; //挂号id
      j++;
    }
  }

  if (rgslist)
    list_destroy(rgslist);
  if (palist)
    list_destroy(palist);
  if (doclist)
    list_destroy(doclist);
}
//绘制处方窗口
void drawPrescriptionInfo_Re(hbasewinAttr *win)
{
  int x, y;
  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  // fillRegionEx(x + 3, y + 346, 360, 200, 0xFFFF);
  rectangleEx(x + 380, y + 300, 480, 200, 0x6BAF, 1, 1);
  lineyEx(x + 550, y + 300, 200, 0x6BAF);
  lineyEx(x + 600, y + 300, 200, 0x6BAF);
  lineyEx(x + 660, y + 300, 200, 0x6BAF);
  lineyEx(x + 810, y + 300, 200, 0x6BAF);
  lineyEx(x + 870, y + 300, 200, 0x6BAF);
  linex_styleEx(x + 380, y + 335, 480, 0x6BAF, 1, 1);
}
void OnPaint_ResultPage(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = (WinStyle *)win->style;

  _h = getFont(SIMHEI, 24, style->fontcolor);
  printTextLineXY(x + 450, y + 25, win->title, _h);
  freeFont(_h);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  fillResultRegisterList(win, style->type);
  repaintChildren(win, NULL);

  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- 查看问诊结果", _h);

  //绘制挂号窗口头
  printTextLineXY(x + 20, y + 45, "  单号   姓名    科室    医生   状态", _h);
  drawResultRegister(x, y);
  freeFont(_h);

  drawResultRegisterInfo(win);
  drawPrescriptionInfo_Re(win);
}
void EventHandler_resultpage(hbasewinAttr *win, int type, void *val)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(val);

  _g = (globaldef *)val;
  hitwin = checkmousewin(win, &_g->mouse);

  if (type == EVENT_MOUSE)
    switch (hitwin->winID)
    {
    case ID_RESULTPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;
    case ID_RESULT_RETURN:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //转跳homepage
        if (win->parent && win->parent->winID == ID_DESKTOP) //找到desktop
        {
          _g->activePageID = ID_HOMEPAGE;
          win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
        }
      }
      break;
    default:
      if (hitwin->winID >= ID_PAYREGS_LINK && hitwin->winID < ID_PAYREGS_LINK + 8)
      {
        if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
          _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

        if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
        { //鼠标按下
          if (hitwin->onClick)
            hitwin->onClick(hitwin, NULL);
        }
        else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        { //点击挂号链接释放
          int x = getAbsoluteX(win);
          int y = getAbsoluteY(win);
          if (hitwin->onLeave)
            hitwin->onLeave(hitwin, NULL);

          //填充信息
          fillRegionEx(x + 370, y + 40, 620, 200, 0xFFFF);
          // fillPayRegisterInfo(win, hitwin->data);
          // drawPayRegisterInfo(win);
          drawResultRegisterInfo(win);
          fillResultRegisterInfo(win, hitwin->data);
          //设置当前挂号单,用于更新挂号单状态
          drawResultRegister(x, y);

          win->data = hitwin->data;
        }
      }
      break;
    }
}
