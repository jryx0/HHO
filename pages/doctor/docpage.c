#include "macrodef.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "wResource.h"
#include "hhosvga.h"
#include "list.h"
#include "hglobal.h"

#include "htextbox.h"
#include "hbutton.h"
#include "data.h"
#include "docpage.h"
#include "hchkbox.h"

#include <string.h>

#define COL1 120
#define COL2 (COL1 + 150)
#define COL3 (COL2 + 100)
#define COL4 (COL3 + 65)
#define COL5 (COL4 + 65)
#define COL6 (COL5 + 100)
#define COL7 (COL6 + 100)
#define COL8 (COL7 + 120)

hbasewinAttr *CreateDocPage(hbasewinAttr *parent, int winID, int userid)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "医生诊断");
  page->onPaint = OnPaint_Docpage;

  page->EventHandler = Eventhandler_docpage;

  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);

  page->title = malloc(16);
  if (userid != -1)
  {
    DoctorInfo *doc = fFindDoctorInfo(DOCTORINFOFILE, userid);

    if (doc)
    {
      DeptInfo *dp = fFindDeptInfo(DEPTINFOFILE, doc->deptid);
      if (dp)
      {
        sprintf(page->title, "医生诊断(%s)", dp->deptname);
        CreateButton(page, page->x + 660, page->y + 390, 100, 32, ID_DOC_BTN_CONFIRM, "诊断完成");
        CreateButton(page, page->x + 780, page->y + 390, 100, 32, ID_DOC_BTN_GEN, "生成处方");
        free(dp);
      }
    }
    free(doc);
  }

  Createhyperlink(page, 20, 10, 65, 25, ID_DOC_RETURN, "[首 页]");
  ((WinStyle *)page->style)->type = userid; //保存id区分

  CreateTextBox(page, 600, 85, 400, 32, ID_DOC_TEXTBOX_RESULT, NULL, 1);
  CreateTextBox(page, 600, 145, 400, 120, ID_DOC_TEXTBOX_HANDLER, NULL, 0);

  CreateTextBox(page, 80, 275, 150, 32, ID_DOC_QUERY_DRUG, NULL, 1);
  CreateButton(page, 250, 275, 80, 32, ID_DOC_QUERY, "查询");

  CreateButton(page, 470, 380, 40, 30, ID_DOC_ADDDRUG, "-->");
  CreateButton(page, 470, 420, 40, 30, ID_DOC_DELDRUG, "<--");

  return page;
}

//绘制挂号信息
void drawRegistration(int x, int y)
{
  rectangleEx(x, y + 65, 240, 200, 0x6BAF, 1, 1);
  //fillRegionEx(x + 2, y + 320, 320, 200, 0x0000);

  lineyEx(x + 80, y + 65, 200, 0x6BAF);
  lineyEx(x + 140, y + 65, 200, 0x6BAF);
  lineyEx(x + 180, y + 65, 200, 0x6BAF);
  linex_styleEx(x, y + 90, 240, 0x6BAF, 1, 1);
}

//填充挂号单信息
void fillRegistration(hbasewinAttr *win)
{
  hbasewinAttr *lnk;
  list_t *rgslist, *palist, *doclist;
  PatientInfo *pi;
  RegisterInfo *rgs;
  DoctorInfo *d;
  WinStyle *style;
  //char infostr[256];

  list_node_t *node;
  int i, j = 0;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = win->style;

  rgslist = ReadRegistration(REGISTRATIONFILE);
  TESTNULLVOID(rgslist);
  palist = ReadPatientInfo(PATIENTINFOFILE);
  doclist = ReadDoctorInfo(DOCTORINFOFILE);

  if (palist && doclist)
    for (i = 0, j = 0; i < rgslist->len && i < 8; i++)
    { //一次最多8行, page控制显示那个8个
      hbasewinAttr *lnk = findWinByID(win, ID_DOC_PSLINK + j);
      node = list_at(rgslist, i);
      rgs = (RegisterInfo *)node->val;

      //只显示医生本人单据和已缴费单据
      if (rgs->doctorid != style->type && rgs->status == 1)
        continue;

      if (lnk == NULL)
      { //创建lnk
        lnk = Createhyperlink(win, 10 + 15, 95 + 25 * j, 240, 25, ID_DOC_PSLINK + j, NULL);
        lnk->wintype = HYPERLINK_BK;
        CreateCheckBox(win, 10, 95 + 8 + 25 * j, 10, 10, ID_DOC_PSCHK + j, NULL);
        j++;
      }

      if (lnk->title)
        free(lnk->title);
      lnk->title = malloc(256);
      memset(lnk->title, 0, 256);

      pi = FindPatientInfo(palist, rgs->userid);  //患者
      d = FindDoctorInfo(doclist, rgs->doctorid); //医生
      if (rgs && pi && d)
      {
        sprintf(lnk->title, "%-8d%-6s%-4d已缴费", rgs->id, pi->name, 2020 - pi->year);
        lnk->data = rgs->id; //挂号id
        //TRACE(("%s\n", lnk->title));
      }
    }

  if (rgslist)
    list_destroy(rgslist);
  if (palist)
    list_destroy(palist);
  if (doclist)
    list_destroy(doclist);
}

//绘制患者信息和患者自述
void drawPatientInfo(hbasewinAttr *win, int rgsid)
{
  int x, y;
  hfont *_h;
  WinStyle *style;
  RegisterInfo *rgs;
  PatientInfo *pa;
  TESTNULLVOID(win);
  TESTNULLVOID(win->style);

  style = win->style;
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  fillRegionEx(x + 260, y + 65, 310, 200, 0xFFFF);

  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + 260, y + 65, "姓名:", _h);
  printTextLineXY(x + 370, y + 65, "性别:", _h);
  printTextLineXY(x + 440, y + 65, "年龄:", _h);
  printTextLineXY(x + 260, y + 90, "住址:", _h);
  printTextLineXY(x + 260, y + 115, "患者主诉:", _h);
  printTextLineXY(x + 480, y + 115, "过敏史:", _h);
  rectangleEx(x + 260, y + 135, 310, 130, 0x6BAF, 1, 1);
  freeFont(_h);
}

//绘制药品查询窗口
void drawDrugQuery(hbasewinAttr *win)
{
  int x, y;
  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  // fillRegionEx(x + 3, y + 346, 360, 200, 0xFFFF);
  rectangleEx(x + 2, y + 315, 460, 200, 0x6BAF, 1, 1);

  lineyEx(x + 250, y + 315, 200, 0x6BAF);
  lineyEx(x + 350, y + 315, 200, 0x6BAF);
  lineyEx(x + 390, y + 315, 200, 0x6BAF);
  linex_styleEx(x + 2, y + 345, 460, 0x6BAF, 1, 1);
}

void fillDrugQuery(hbasewinAttr *win, char *drugname)
{
  int i, x, y;
  hbasewinAttr *ctrl;
  list_node_t *node;
  DrugItem *drug;
  char info[128];
  TESTNULLVOID(win);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  if (drugname)
  {
    list_t *druglist = ReadDrugItembyName(DRUGFILE, drugname);
    TRACE(("%s(%d):find drugs =  %d\n", __FILE__, __LINE__, druglist->len));
    fillRegionEx(x + 3, y + 346, 360, 200, 0xFFFF);
    for (i = 0; i < druglist->len; i++)
    {
      node = list_at(druglist, i);
      if (node->val)
      {
        drug = (DrugItem *)node->val;
        if (drug)
        {
          sprintf(info, "%-27s%-11s%-5s%.2f", drug->name, drug->kind, drug->unit, drug->price / 100.00);
          ctrl = Createhyperlink(win, x + 20, 348 + 25 * i, 435, 25, ID_DOC_DRUGLINK + i, info);
          ctrl->wintype = HYPERLINK_BK;
          ctrl->onPaint(ctrl, NULL);
          ctrl = CreateCheckBox(win, x + 5, 355 + 25 * i, 10, 10, ID_DOC_DRUGCHK + i, NULL);
          ctrl->onPaint(ctrl, NULL);
        }
      }
    }

    list_destroy(druglist);
  }
}

//绘制处方窗口
void drawPrescriptionInfo(hbasewinAttr *win)
{
  int x, y;
  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  // fillRegionEx(x + 3, y + 346, 360, 200, 0xFFFF);
  rectangleEx(x + 520, y + 315, 480, 200, 0x6BAF, 1, 1);
}

//绘制处方
//onpaint中调用,画处方显示的外观
void drawPrescription_doc(int x, int y)
{
  rectangleEx(x, y + 65, PAGE_W, PAGE_H / 2 - 50, 0x6BAF, 1, 1);

  //printTextLineXY(x + 10, y + 70, " 处方单号         日  期         患  者    性别   年龄      医生      科室          金额      状态", _h);

  lineyEx(x + COL1, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL2, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL3, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL4, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL5, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL6, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL7, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL8, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  linex_styleEx(x, y + 90, PAGE_W, 0x6BAF, 1, 1);
}

//填充处方数据
//在drawPrescription前调用
//根据用户类型不同,显示不同的处方 style->type
//page 页号从0开始
void fillPrescription_doc(hbasewinAttr *win, int page)
{
  hbasewinAttr *lnk;
  list_t *ps, *patient, *doc;
  PatientInfo *pi;
  Prescription *p;
  DoctorInfo *d;
  WinStyle *style;
  //char infostr[256];

  list_node_t *node;
  int i, j = 0;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = win->style;

  //ps = ReadPrescription(DATAPATH "database\\ps\\psinfo.txt");
  ps = ReadPrescription(PRESCRITIONFILE);
  TESTNULLVOID(ps);

  // patient = ReadPatientInfo(DATAPATH "database\\painfo.txt");
  // doc = ReadDoctorInfo(DATAPATH "database\\docinfo.txt");
  patient = ReadPatientInfo(PATIENTINFOFILE);
  doc = ReadDoctorInfo(DOCTORINFOFILE);

  if (patient && doc)
    for (i = 0, j = 0; i < ps->len && i < 8; i++)
    { //一次最多8行, page控制显示那个8个
      hbasewinAttr *lnk = findWinByID(win, ID_DOC_PSLINK + j);
      node = list_at(ps, i);
      p = (Prescription *)node->val;

      if (style->type != -1)
      { //只显示本人单据
        if (p->userid != style->type)
          continue;
      }

      if (lnk == NULL)
      { //创建lnk
        lnk = Createhyperlink(win, 10 + 15, 95 + 25 * j, PAGE_W, 25, ID_DOC_PSLINK + i, NULL);
        lnk->wintype = HYPERLINK_BK;
        CreateCheckBox(win, 10, 95 + 8 + 25 * j, 10, 10, ID_DOC_PSCHK + i, NULL);
      }

      if (lnk->title)
        free(lnk->title);
      lnk->title = malloc(256);
      memset(lnk->title, 0, 256);

      // node = list_at(ps, i);

      // p = (Prescription *)node->val;            //处方
      pi = FindPatientInfo(patient, p->userid); //患者
      d = FindDoctorInfo(doc, p->doctorid);     //医生
      if (p && pi && d)
      {
        sprintf(lnk->title, "%-12d%-20s%-5s%8s%7d%8s%12s%15.2f%12s", p->id, p->date, pi->name,
                pi->sex ? "男" : "女", 2020 - pi->year, d->name, p->dept, (float)(p->amount / 100), p->status);
        lnk->data = p->id; //处方id
        //TRACE(("%s\n", lnk->title));
      }
      j++;
    }

  if (ps)
    list_destroy(ps);
  if (patient)
    list_destroy(patient);
  if (doc)
    list_destroy(doc);

  (void)page;
}

//绘制药品清单
void drawDruglist_doc(int x, int y)
{
  fillRegionEx(x + 1, y + 360, 540, 205, 0xFFFF);
  rectangleEx(x, y + 325, 540, 205, 0x6BAF, 1, 1);

  lineyEx(x + 170, y + 325, 205, 0x6BAF);
  lineyEx(x + 220, y + 325, 205, 0x6BAF);
  lineyEx(x + 330, y + 325, 205, 0x6BAF);
  lineyEx(x + 370, y + 325, 205, 0x6BAF);
  lineyEx(x + 470, y + 325, 205, 0x6BAF);
  linex_styleEx(x, y + 350, 540, 0x6BAF, 1, 1);
}

//填充药品
//默认不会超出
void fillDruglist_doc(hbasewinAttr *win, int psid)
{
  FILE *fp;
  char info[256];
  PrescriptionDrugItem *pdi;
  DrugItem *di;
  hfont *_h;
  int i = 0;
  TESTNULLVOID(win);
  //TRACE(("linkdata:%d\n",link->data));

  //处方药品清单文件名
  sprintf(info, "%sdatabase\\ps\\%d.txt", DATAPATH, psid);
  fp = fopen(info, "r");
  if (fp == NULL)
  {
    TRACE(("%s(%d):无法打开文件%s\n", __FILE__, __LINE__, info));
    return;
  }
  //查找药品
  pdi = malloc(sizeof(PrescriptionDrugItem));
  _h = getFont(SIMHEI, 16, 0x0000);
  while (fgets(info, 256, fp))
  {
    if (info[0] == '#')
      continue;
    sscanf(info, "%d\t%d", &pdi->drugItemid, &pdi->amount);
    di = fFindDrugItem(DATAPATH "database\\drug\\druglist.txt", pdi->drugItemid);

    if (di)
    {
      sprintf(info, "%-20s%-5s%-14s%-4d%7.2f%8.2f", di->name, di->unit, di->kind, pdi->amount,
              di->price / 100.0, (float)(pdi->amount * di->price) / 100.00);
      printTextLineXY(win->x + 10, win->y + 360 + i * 20, info, _h);
      i++;
      //TRACE(("%s\n", info));
      free(di);
    }
  }

  free(pdi);

  fclose(fp);
  freeFont(_h);
  return;
}

//绘制运单提示信息
/**
 * 
 * @param psid 处方id
 */
void drawPostInfo_doc(hbasewinAttr *win, int psid)
{
  int x, y;
  //WinStyle *style;
  hfont *_h;
  Prescription *ps;
  postInfo *po;
  char info[64];

  TESTNULLVOID(win);
  //TESTNULLVOID(win->style);
  //style = win->style;
  _h = getFont(SIMSUN, 16, 0x0000);

  fillRegionEx(win->x + 580, win->y + 325, 420, 160, 0xFFFF);

  printTextLineXY(win->x + 585, win->y + 335, "运单号:                发货人:", _h);
  printTextLineXY(win->x + 585, win->y + 32 + 335, "收货人:                发货人电话:", _h);
  printTextLineXY(win->x + 585, win->y + 64 + 335, "收货地址:", _h);

  ps = fFindPrescription(PRESCRITIONFILE, psid);
  if (ps)
  {
    po = fFindPostInfo(POSTFILE, ps->postid);
    if (po)
    {
      sprintf(info, "%d", po->postid);
      printTextLineXY(win->x + 650, win->y + 334, info, _h);
      sprintf(info, "%s", po->shipper);
      printTextLineXY(win->x + 850, win->y + 334, info, _h);
      sprintf(info, "%s", po->receiver);
      printTextLineXY(win->x + 650, win->y + 32 + 334, info, _h);
      sprintf(info, "%s", po->tel);
      printTextLineXY(win->x + 890, win->y + 32 + 334, info, _h);
      sprintf(info, "%s", po->receiveraddr);
      printTextLineXY(win->x + 690, win->y + 64 + 334, info, _h);
      free(po);
    }
    free(ps);
  }
  freeFont(_h);
}
/*
void generatePostInfo_pay(hbasewinAttr *win, int psid)
{
  hbasewinAttr *lnk;
  Prescription *ps;
  list_t *polist, *pslist;
  postInfo *po, *newpo;
  PatientInfo *patient;
  list_node_t *node;
  char datebuf[9];
  char timebuf[9];
  int i, maxPostid = 0;

  TESTNULLVOID(win);

  //查询处方文件,查询处方是否是已缴费
  pslist = ReadPrescription(PRESCRITIONFILE);
  TESTNULLVOID(pslist);

  ps = FindPrescription(pslist, psid);

  //fFind 需要释放 Find不要释放
  patient = fFindPatientInfo(PATIENTINFOFILE, ps->userid);
  TESTNULLVOID(patient);
  TESTNULLVOID(ps);

  if (patient && ps && strcmp(ps->status, "已缴费") == 0)
  {
    polist = ReadPostInfo(POSTFILE);
    for (i = 0; i < polist->len; i++)
    {
      node = list_at(polist, i);
      if (node && node->val)
      {
        po = (postInfo *)node->val;
        if (maxPostid <= po->postid)
          maxPostid = po->postid;
      }
    }

    newpo = malloc(sizeof(postInfo));
    if (newpo == NULL)
    {
      TRACE(("%s(%d):ursrid = %d\n", __FILE__, __LINE__, ps->userid));
    }
    else
    {
      newpo->postid = maxPostid + 1;
      newpo->userid = ps->userid;
      strcpy(newpo->shipper, "药房");
      strcpy(newpo->shipperaddr, "华中科技大学校医院");
      strcpy(newpo->receiver, patient->name);
      strcpy(newpo->receiveraddr, patient->addr);
      strcpy(newpo->tel, patient->telenum);
      strcpy(newpo->status, "待收货");
      _strdate(datebuf);
      _strtime(timebuf);
      sprintf(newpo->shiptime, "%sT%s", datebuf, timebuf);

      strcpy(ps->status, "待收货");
      ps->postid = maxPostid + 1;
      SavePrescription(PRESCRITIONFILE, pslist);

      list_lpush(polist, list_node_new(newpo));
      SavePostInfo(POSTFILE, polist);
    }
    list_destroy(polist);
  }

  list_destroy(pslist);

  free(patient);
  TRACE(("%s(%d): save2\n", __FILE__, __LINE__));
}
*/
//返回checkbox选择的hyperlink
hbasewinAttr *getcheckedlink_doc(hbasewinAttr *win)
{
  hbasewinAttr *lnk = NULL, *chk = NULL;
  int i;
  TESTNULL(win, NULL);

  for (i = 0; i < ID_DOC_PSCHK + 8; i++)
  {
    chk = findWinByID(win, ID_DOC_PSCHK + i);

    if (chk && chk->data == 1)
    { //第一个 checbox 选择
      //根据i找到相应的link
      lnk = findWinByID(win, ID_DOC_PSLINK + i);
      return lnk;
    }
  }
  return NULL;
}

void OnPaint_Docpage(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  //填充挂号link
  fillRegistration(win);
  repaintChildren(win, val);

  style = win->style;
  _h = getFont(SIMHEI, 24, 0x0000);
  printTextLineXY(x + 430, y + 25, win->title, _h);
  freeFont(_h);

  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- 医生诊断", _h);

  //绘制诊断
  printTextLineXY(x + 600, y + 65, "诊断结论:", _h);
  printTextLineXY(x + 600, y + 125, "治疗/处置:", _h);

  //绘制挂号窗口头
  printTextLineXY(x + 2, y + 68, "   单号   姓名  年龄  状态", _h);
  //绘制挂号窗口
  drawRegistration(x, y);

  //绘制药品查询
  printTextLineXY(x + 2, y + 285, "药品名称:", _h);
  //绘制药品查询窗口
  printTextLineXY(x + 2, y + 320, "          药品名称              规格    单位   单价", _h);
  drawDrugQuery(win);

  //绘制处方
  printTextLineXY(x + 510, y + 285, "处方:", _h);
  drawPrescriptionInfo(win);
  freeFont(_h);

  //绘制患者信息
  drawPatientInfo(win, 0);

  // //填充处方数据
  // fillPrescription_doc(win, 0);
  // //绘制

  // x = getAbsoluteX(win);
  // y = getAbsoluteY(win);

  // style = win->style;

  // _h = getFont(style->fonttype, style->fontsize, 0x0000);

  // //处方头
  // printTextLineXY(x + 10, y + 70, " 处方单号         日  期         患  者    性别   年龄      医生      科室          金额      状态", _h);
  // //画处方线框
  // drawPrescription_doc(x, y);

  // //药品头
  // printTextLineXY(x + 10, y + 330, "    药品名称       单位   规格       数量    单价     小计", _h);
  // //画药品清单框
  // drawDruglist_doc(x, y);
}

void Eventhandler_docpage(hbasewinAttr *win, int type, void *val)
{
  globaldef *_g;
  hbasewinAttr *hitwin;
  _g = (globaldef *)val;
  //确定是否是子窗口事件，非子窗口事件返回本身
  hitwin = checkmousewin(win, &_g->mouse);

  switch (hitwin->winID)
  {                //鼠标在那个子窗口或本身
  case ID_DOCPAGE: //本身
    if (type == EVENT_MOUSE)
    {                                                                              //homepage 处理鼠标移动理鼠标移动
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
    }
    break;
  case ID_DOC_RETURN:
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
  case ID_DOC_ADDDRUG:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在窗口部分显示手型鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //鼠标按下
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //鼠标释放
      hbasewinAttr *tb;
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      tb = findWinByID(win, ID_DOC_QUERY_DRUG);
      if (tb)
      {
        //查询druglist 中选择的药品,
        hbasewinAttr *chk = getcheckedlink_doc(win);
        if (chk)
        { //保存药品清单到文件
        }
        TRACE(("%s(%d): 查询药品=%s\n", __FILE__, __LINE__, tb->title));
      }
    }
    break;
  case ID_DOC_BTN_CONFIRM:
    //????
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在homepage窗口部分显示标准鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //鼠标按下
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //鼠标释放,修改处方状态

      hbasewinAttr *pslink;
      list_t *tmplist;
      Prescription *ps;
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      //获取处方
      //查询checkbox已选的link
      // pslink = getcheckedlink_doc(win);
      // if (pslink)
      // {
      //   tmplist = ReadPrescription(PRESCRITIONFILE);
      //   if (tmplist)
      //   { //找到
      //     ps = FindPrescription(tmplist, pslink->data);
      //     if (ps)
      //     {
      //       if (strcmp(ps->status, "未缴费") == 0)
      //       {
      //         strcpy(ps->status, "已缴费");
      //         SavePrescription(PRESCRITIONFILE, tmplist);

      //         fillPrescription_doc(win, 0);
      //         repaintChildren(win, NULL);
      //         drawPrescription_doc(win->x, win->y);
      //       }
      //     }
      //     list_destroy(tmplist);
      //   }
      // }

      //已缴费->已确认
    }
    break;
  case ID_DOC_QUERY:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在窗口部分显示手型鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //鼠标按下
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //鼠标释放
      hbasewinAttr *tb;

      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      tb = findWinByID(win, ID_DOC_QUERY_DRUG);
      if (tb)
      {
        fillDrugQuery(win, tb->title);
        drawDrugQuery(win);

        TRACE(("%s(%d): 查询药品=%s\n", __FILE__, __LINE__, tb->title));
      }
    }
    break;
  case ID_DOC_QUERY_DRUG:
  case ID_DOC_TEXTBOX_HANDLER:
  case ID_DOC_TEXTBOX_RESULT:
    if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    {
      if (hitwin->onActivate)
        hitwin->onActivate(hitwin, _g);
    }
    break;
  default:
    if (hitwin->winID >= ID_DOC_PSLINK && hitwin->winID < ID_DOC_PSLINK + 8)
    {
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放

        // hbasewinAttr *pslink;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //处方划线
        drawPrescription_doc(getAbsoluteX(win), getAbsoluteY(win));

        //填充药品 hitwin->data  处方单号
        drawDruglist_doc(win->x, win->y);
        fillDruglist_doc(win, hitwin->data);

        //运单信息
        drawPostInfo_doc(win, hitwin->data);
        //fillPostInfo(win, hitwin->data);
      }
    }
    else if (hitwin->winID >= ID_DOC_DRUGLINK && hitwin->winID < ID_DOC_DRUGLINK + 6)
    {
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

        drawDrugQuery(win);
      }
    }
    if (hitwin->winID >= ID_DOC_DRUGCHK && hitwin->winID < ID_DOC_DRUGCHK + 8)
    {                                                                             //checkbox
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
        hitwin->onPaint(hitwin, NULL);
      }
    }

    break;
  }
}
