#include "macrodef.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "Resource.h"
#include "hhosvga.h"
#include "list.h"
#include "hglobal.h"

#include "htextbox.h"
#include "hbutton.h"
#include "data.h"
#include "docpage.h"
#include "hchkbox.h"
#include "hlabel.h"

#include <string.h>
#include <ctype.h>

#define COL1 120
#define COL2 (COL1 + 150)
#define COL3 (COL2 + 100)
#define COL4 (COL3 + 65)
#define COL5 (COL4 + 65)
#define COL6 (COL5 + 100)
#define COL7 (COL6 + 100)
#define COL8 (COL7 + 120)

//win->data 选择的挂号单
//style->type 医生
hbasewinAttr *CreateDocPage(hbasewinAttr *parent, int winID, int userid)
{
  hbasewinAttr *ctrl;
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
        // CreateButton(page, page->x + 780, page->y + 390, 100, 32, ID_DOC_BTN_GEN, "生成处方");
        CreateButton(page, page->x + 540, page->y + 390, 100, 32, ID_DOC_BTN_CAL, "计算金额");
        free(dp);
      }
    }
    free(doc);
  }

  Createhyperlink(page, 20, 10, 65, 25, ID_DOC_RETURN, "[首 页]");
  ((WinStyle *)page->style)->type = userid; //保存医生

  CreateTextBox(page, 600, 85, 400, 32, ID_DOC_TEXTBOX_RESULT, "", 1);
  CreateTextBox(page, 600, 145, 400, 120, ID_DOC_TEXTBOX_HANDLER, "", 0);

  CreateTextBox(page, 80, 275, 150, 32, ID_DOC_QUERY_DRUG, "", 1);
  CreateButton(page, 250, 275, 80, 32, ID_DOC_QUERY, "查询");

  CreateButton(page, 470, 380, 40, 30, ID_DOC_ADDDRUG, "-->");
  CreateButton(page, 470, 420, 40, 30, ID_DOC_DELDRUG, "<--");

  ctrl = CreateLabel(page, 790, 282, 100, 20, ID_DOC_AMOUNT, "");
  ctrl->wintype = LABEL_TITLE;

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

  //清空link数据
  for (i = ID_DOC_REGSLINK; i < ID_DOC_REGSLINK + 8; i++)
  {
    lnk = findWinByID(win, i);
    if (lnk && lnk->title)
    {
      free(lnk->title);
      lnk->title = NULL;
    }
  }

  if (palist && doclist)
    for (i = 0, j = 0; i < rgslist->len && i < 8; i++)
    { //一次最多8行, page控制显示那个8个
      hbasewinAttr *lnk = findWinByID(win, ID_DOC_REGSLINK + j);
      node = list_at(rgslist, i);
      rgs = (RegisterInfo *)node->val;

      //只显示医生本人单据和已缴费单据
      if (!(rgs->status == 1 && rgs->doctorid == style->type))
        continue;

      if (lnk == NULL)
      { //创建lnk
        lnk = Createhyperlink(win, 10, 95 + 25 * j, 229, 25, ID_DOC_REGSLINK + j, NULL);
        lnk->wintype = HYPERLINK_BK;
        //CreateCheckBox(win, 10, 95 + 8 + 25 * j, 10, 10, ID_DOC_REGSLINK + j, NULL);
      }
      j++;

      if (lnk->title)
        free(lnk->title);
      lnk->title = malloc(256);
      memset(lnk->title, 0, 256);

      pi = FindPatientInfo(palist, rgs->userid);  //患者
      d = FindDoctorInfo(doclist, rgs->doctorid); //医生
      if (rgs && pi && d)
      {

        sprintf(lnk->title, "%-9d%-7s%-4d%d", rgs->id, pi->name, 2020 - pi->year, rgs->status);
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
  hregion r;
  char info[20];
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
  printTextLineXY(x + 440, y + 115, "过敏史:", _h);
  rectangleEx(x + 260, y + 135, 310, 130, 0x6BAF, 1, 1);
  freeFont(_h);

  rgs = fFindRegisterInfo(REGISTRATIONFILE, rgsid);
  if (rgs)
  {
    _h = getFont(SIMHEI, 16, 0x0000);

    pa = fFindPatientInfo(PATIENTINFOFILE, rgs->userid);
    if (pa)
    {
      printTextLineXY(x + 310, y + 66, pa->name, _h);
      printTextLineXY(x + 420, y + 66, pa->sex ? "男" : "女", _h);

      sprintf(info, "%d", 2020 - pa->year);
      printTextLineXY(x + 490, y + 66, info, _h);
      printTextLineXY(x + 310, y + 90, pa->addr, _h);
      printTextLineXY(x + 500, y + 115, pa->allergy, _h);

      r.left_top.x = x + 265;
      r.left_top.y = y + 140;
      r.right_bottom.x = x + 265 + 300;
      r.right_bottom.y = y + 140 + 124;
      printTextEx(&r, rgs->disease, _h, 0);

      free(pa);
    }

    free(rgs);
    freeFont(_h);
  }
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

  for (i = 0; i < 6; i++)
  {
    ctrl = findWinByID(win, ID_DOC_DRUGLINK + i);
    if (ctrl)
      ctrl->onDestroy(ctrl, NULL);

    ctrl = findWinByID(win, ID_DOC_DRUGCHK + i);
    if (ctrl)
      ctrl->onDestroy(ctrl, NULL);
  }

  if (drugname && strlen(drugname) > 1)
  {
    list_t *druglist = ReadDrugItembyName(DRUGFILE, drugname);
    //TRACE(("%s(%d):find drugs =  %d\n", __FILE__, __LINE__, druglist->len));
    fillRegionEx(x + 3, y + 346, 460, 200, 0xFFFF);
    // //清除link
    // for (i = 0; i < 6; i++)
    // {
    //   ctrl = findWinByID(win, ID_DOC_DRUGLINK + i);
    //   if (ctrl && ctrl->title)
    //   {
    //     free(ctrl->title);
    //     ctrl->title = NULL;
    //     ctrl->nWidth = 0;
    //   }

    //   ctrl = findWinByID(win, ID_DOC_DRUGCHK + i);
    //   if (ctrl)
    //   {
    //     ctrl->nWidth = 0;
    //     ctrl->data = 0;
    //   }
    // }

    for (i = 0; i < druglist->len && i < 6; i++)
    {
      node = list_at(druglist, i);
      if (node->val)
      {
        drug = (DrugItem *)node->val;
        sprintf(info, "%-27s%-11s%-5s%.2f", drug->name, drug->kind, drug->unit, drug->price / 100.00);
        ctrl = findWinByID(win, ID_DOC_DRUGLINK + i);
        if (ctrl == NULL)
        {
          ctrl = Createhyperlink(win, x + 20, 348 + 25 * i, 435, 25, ID_DOC_DRUGLINK + i, info);
          ctrl->data = drug->id;
          ctrl->wintype = HYPERLINK_BK;
          ctrl->onPaint(ctrl, NULL);

          ctrl = CreateCheckBox(win, x + 5, 355 + 25 * i, 10, 10, ID_DOC_DRUGCHK + i, NULL);
          ctrl->onPaint(ctrl, NULL);
        }
        else
        {
          ctrl->nWidth = 435;
          if (ctrl->title == NULL)
            ctrl->title = malloc(256);

          strcpy(ctrl->title, info);
          ctrl->onPaint(ctrl, NULL);
          ctrl = findWinByID(win, ID_DOC_DRUGCHK + i);
          if (ctrl)
          {
            ctrl->nWidth = 10;
            ctrl->onPaint(ctrl, NULL);
          }
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
  lineyEx(x + 690, y + 315, 200, 0x6BAF);
  lineyEx(x + 740, y + 315, 200, 0x6BAF);
  lineyEx(x + 800, y + 315, 200, 0x6BAF);
  lineyEx(x + 850, y + 315, 200, 0x6BAF);
  lineyEx(x + 920, y + 315, 200, 0x6BAF);
  linex_styleEx(x + 520, y + 345, 480, 0x6BAF, 1, 1);
}

void delDrugfromPSwin(hbasewinAttr *win, hbasewinAttr *druglink)
{
  int i, j = 0, k;
  int x, y;
  hbasewinAttr *lnk, *chk, *chk2, *tb, *tb2;

  TESTNULLVOID(win);
  TESTNULLVOID(druglink);
  TESTNULLVOID(win->children);

  k = druglink->winID - ID_DOC_PSDRUGLINK;
  druglink->onDestroy(druglink, NULL);
  chk = findWinByID(win, ID_DOC_PSDRUGCHK + k);
  if (chk)
    chk->onDestroy(chk, NULL);
  tb = findWinByID(win, ID_DOC_PSDRUGTEXTBOX + k);
  if (tb)
    tb->onDestroy(tb, NULL);

  tb2 = findWinByID(win, ID_DOC_PSDRUGTEXTBOX2 + k);
  if (tb2)
    tb2->onDestroy(tb2, NULL);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  fillRegionEx(x + 520, y + 315 + 30, 480, 170, 0xFFFF);
  for (i = 0, j = 0; i < 6; i++)
  {
    lnk = findWinByID(win, ID_DOC_PSDRUGLINK + i);
    tb = findWinByID(win, ID_DOC_PSDRUGTEXTBOX + i);
    chk = findWinByID(win, ID_DOC_PSDRUGCHK + i);
    tb2 = findWinByID(win, ID_DOC_PSDRUGTEXTBOX2 + i);
    if (lnk)
    {
      textInfo *ti;
      lnk->x = x + 535;
      lnk->y = 353 + 30 * j;

      tb->x = x + 800;
      tb->y = 350 + 30 * j;
      ti = (textInfo *)tb->style;
      ti->r.left_top.x = getAbsoluteX(tb) + 6;
      ti->r.left_top.y = getAbsoluteY(tb) + 7;
      ti->r.right_bottom.x = ti->r.left_top.x + tb->nWidth - 6 - 2;
      ti->r.right_bottom.y = ti->r.left_top.y + tb->nHeight - 6 - 2;
      // tb->nWidth = 20;
      // tb->nHeight = 16 * 5 / 3;

      tb2->x = x + 930;
      tb2->y = 350 + 30 * i;
      ti = (textInfo *)tb2->style;
      ti->r.left_top.x = getAbsoluteX(tb2) + 6;
      ti->r.left_top.y = getAbsoluteY(tb2) + 7;
      ti->r.right_bottom.x = ti->r.left_top.x + tb->nWidth - 6 - 2;
      ti->r.right_bottom.y = ti->r.left_top.y + tb->nHeight - 6 - 2;

      chk->x = x + 520;
      chk->y = 360 + 30 * j;

      j++;
      lnk->onPaint(lnk, NULL);
      chk->onPaint(chk, NULL);
      tb->onPaint(tb, NULL);
      tb2->onPaint(tb2, NULL);
    }
  }
}

void delallPSDrug(hbasewinAttr *win)
{
  int i;
  //int x, y;
  hbasewinAttr *ctrl;
  for (i = 0; i < 8; i++)
  {
    ctrl = findWinByID(win, ID_DOC_PSDRUGCHK + i);
    if (ctrl)
    {
      ctrl->onDestroy(ctrl, NULL);

      ctrl = findWinByID(win, ID_DOC_PSDRUGLINK + i);
      if (ctrl)
        ctrl->onDestroy(ctrl, NULL);

      ctrl = findWinByID(win, ID_DOC_PSDRUGTEXTBOX + i);
      if (ctrl)
        ctrl->onDestroy(ctrl, NULL);

      ctrl = findWinByID(win, ID_DOC_PSDRUGTEXTBOX2 + i);
      if (ctrl)
        ctrl->onDestroy(ctrl, NULL);
    }
  }
  // x = getAbsoluteX(win);
  // y = getAbsoluteY(win);
}

void addDrugtoPSwin(hbasewinAttr *win, hbasewinAttr *druglink)
{
  int i, j;
  int x, y;
  hbasewinAttr *pslink, *chk, *tb, *tb2;
  char Info[64];
  //int drugid;
  DrugItem *drug = NULL;

  TESTNULLVOID(win);
  TESTNULLVOID(druglink);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  //drugid = druglink->data;

  for (i = 0; i < 5; i++)
  {
    pslink = findWinByID(win, ID_DOC_PSDRUGLINK + i);
    if (pslink && pslink->title != NULL)
      continue;

    if (pslink == NULL)
    {
      hfont *_h = getFont(SIMYOU, 16, 0x0000);
      pslink = Createhyperlink(win, x + 535, 353 + 30 * i, 260, 25, ID_DOC_PSDRUGLINK + i, NULL);
      pslink->wintype = HYPERLINK_BK;
      pslink->data = druglink->data;
      chk = CreateCheckBox(win, x + 520, 360 + 30 * i, 10, 10, ID_DOC_PSDRUGCHK + i, NULL);
      tb = CreateTextBox(win, 805, 350 + 30 * i, 40, 20, ID_DOC_PSDRUGTEXTBOX + i, "1", 1);
      tb2 = CreateTextBox(win, 930, 350 + 30 * i, 20, 20, ID_DOC_PSDRUGTEXTBOX2 + i, "2", 1);
      printTextLineXY(960, y + 360 + 30 * i, "次/天", _h);
      freeFont(_h);
    }

    chk = findWinByID(win, ID_DOC_PSDRUGCHK + i);
    tb = findWinByID(win, ID_DOC_PSDRUGTEXTBOX + i);
    tb2 = findWinByID(win, ID_DOC_PSDRUGTEXTBOX2 + i);
    drug = fFindDrugItem(DRUGFILE, druglink->data);
    if (drug)
    {

      pslink->title = malloc(128);
      sprintf(pslink->title, "%-19s%-4s%.2f", drug->name, drug->unit, drug->price / 100.00);

      pslink->nWidth = 260;
      pslink->onPaint(pslink, NULL);
      if (chk && tb && tb2)
      {
        chk->nWidth = 10;
        chk->onPaint(chk, NULL);

        tb->data = 1;
        tb->onPaint(tb, NULL);
        tb2->data = 2;
        tb2->onPaint(tb2, NULL);
      }
      free(drug);
    }
    break;
  }
}

//返回checkbox选择的hyperlink
hbasewinAttr *getcheckedlink_doc(hbasewinAttr *win, int linkid, int chkid)
{
  hbasewinAttr *lnk = NULL, *chk = NULL;
  int i;
  TESTNULL(win, NULL);

  for (i = 0; i < chkid + 8; i++)
  {
    chk = findWinByID(win, chkid + i);

    if (chk && chk->data == 1)
    { //第一个 checbox 选择
      //根据i找到相应的link
      lnk = findWinByID(win, linkid + i);
      chk->data = 0;
      chk->onPaint(chk, NULL);
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
  printTextLineXY(x + 520, y + 285, "处方:", _h);
  printTextLineXY(x + 720, y + 285, "总金额:", _h);
  printTextLineXY(x + 520, y + 320, "     药品名称       单位   单价  数量  价格      用法", _h);
  drawPrescriptionInfo(win);
  freeFont(_h);

  //绘制患者信息
  drawPatientInfo(win, 0);
}

long calDrugAmount(hbasewinAttr *win)
{
  hbasewinAttr *q;
  hbasewinAttr *lnk;
  int i;
  int qulity;
  long sum = 0;
  char info[10];
  hfont *_h;
  int x, y;

  TESTNULL(win, 0);
  x = getAbsoluteY(win);
  y = getAbsoluteY(win);

  _h = getFont(SIMYOU, 16, 0x0000);
  for (i = 0; i < 5; i++)
  {
    lnk = findWinByID(win, ID_DOC_PSDRUGLINK + i);
    if (lnk)
    {
      DrugItem *drug = fFindDrugItem(DRUGFILE, lnk->data);
      q = findWinByID(win, ID_DOC_PSDRUGTEXTBOX + i);
      if (drug && q && q->title)
      {
        qulity = atoi(q->title);
        sum += qulity * drug->price;
        sprintf(info, "%.2f", (qulity * drug->price) / 100.00);
        fillRegionEx(x + 730, y + 353 + 30 * i, 60, 32, 0xFFFF);
        printTextLineXY(x + 730, y + 353 + 30 * i, info, _h);
        free(drug);
      }
    }
  }
  sprintf(info, "%.2f 元", sum / 100.0);
  lnk = findWinByID(win, ID_DOC_AMOUNT);
  if (lnk)
  {
    if (lnk->title)
      free(lnk->title);

    lnk->title = malloc(strlen(info) + 1);
    strcpy(lnk->title, info);
    //fillRegionEx(x + 720, y + 285, 100, 32, 0x0000);
    lnk->onPaint(lnk, NULL);
  }
  //printTextLineXY(x + 720, x + 286, info, _h);

  freeFont(_h);
  return sum;
}

int SavePS(hbasewinAttr *win)
{
  list_t *pslist;
  list_node_t *node;
  Prescription *ps, *ps1;
  RegisterInfo *regi;
  int i, maxid = 2000;
  char datebuf[9], timebuf[9];
  TESTNULL(win, -1);

  pslist = ReadPrescription(PRESCRITIONFILE);
  if (pslist == NULL)
    pslist = list_new();

  for (i = 0; i < pslist->len; i++)
  { //获取最大id
    node = list_at(pslist, i);
    if (node && node->val)
    {
      ps1 = (Prescription *)node->val;
      if (maxid < ps1->id)
        maxid = ps1->id;
    }
  }
  regi = fFindRegisterInfo(REGISTRATIONFILE, win->data);
  if (regi)
  {
    ps = malloc(sizeof(Prescription));
    memset(ps, 0, sizeof(Prescription));
    ps->id = maxid + 1;
    ps->userid = regi->userid;
    ps->doctorid = regi->doctorid;
    strcpy(ps->dept, regi->dept);
    _strtime(timebuf);
    _strdate(datebuf);
    sprintf(ps->date, "%sT%s", datebuf, timebuf);
    ps->amount = calDrugAmount(win);
    strcpy(ps->status, "未缴费");
    ps->postid = 0;
    ps->registerid = regi->id;

    list_lpush(pslist, list_node_new(ps));
    SavePrescription(PRESCRITIONFILE, pslist);

    free(regi);
  }

  list_destroy(pslist);
  return maxid + 1;
}
void saveDruglist(hbasewinAttr *win, int psid)
{
  hbasewinAttr *druglink = NULL, *drugamounttb = NULL, *drugcounttb = NULL;
  DrugItem *drugitem;

  char buf[64];
  int i;
  FILE *fp;

  TESTNULLVOID(win);
  sprintf(buf, "%sdatabase\\ps\\%d.txt", DATAPATH, psid);
  if ((fp = fopen(buf, "wt")) == NULL)
  {
    TRACE(("unable to open %s\r\n", buf));
    return;
  }
  fprintf(fp, "#药品id  数量  用法 单价\n");
  for (i = 0; i < 8; i++)
  {
    druglink = findWinByID(win, ID_DOC_PSDRUGLINK + i);
    drugamounttb = findWinByID(win, ID_DOC_PSDRUGTEXTBOX + i);
    drugcounttb = findWinByID(win, ID_DOC_PSDRUGTEXTBOX2 + i);

    if (druglink && drugamounttb && drugcounttb)
    {
      drugitem = fFindDrugItem(DRUGFILE, druglink->data);
      if (drugitem)
      {
        // psdrug = malloc(sizeof(PrescriptionDrugItem));
        // psdrug->drugItemid = drugitem->id;
        // psdrug->amount = atio(drugamounttb->title);
        // psdrug->count = atio(drugcounttb->title);
        // psdrug->price = drugitem->price;
        fprintf(fp, "%d\t%d\t%d\t%ld\n",
                drugitem->id, atoi(drugamounttb->title), atoi(drugcounttb->title), drugitem->price);
        // free(drugitem);
      }
    }
  }
  fflush(fp);
  fclose(fp);
}
void SaveDiagnose(hbasewinAttr *win)
{
  hbasewinAttr *ctrl;
  list_t *recordlist, *regslist;
  list_node_t *node;
  RegisterInfo *regs;
  MedicalRecord *mrecord, *mtemp;
  char datebuf[9], timebuf[9];

  int i;
  Prescription *ps;

  regslist = ReadRegistration(REGISTRATIONFILE);
  if (regslist)
  { //更新挂号单
    regs = FindRegisterInfo(regslist, win->data);
    regs->status = 2; //已完成
    //更新挂号状态
    SaveRegistration(REGISTRATIONFILE, regslist);
    list_destroy(regslist);
  }

  mrecord = malloc(sizeof(MedicalRecord));
  memset(mrecord, 0, sizeof(MedicalRecord));
  mrecord->id = 8000;

  recordlist = ReadMedicalRecord(MEDICALRECORDFILE);
  if (recordlist == NULL)
    recordlist = list_new();

  for (i = 0; i < recordlist->len; i++)
  { //获取病例最大id
    node = list_at(recordlist, i);
    if (node && node->val)
    {
      mtemp = (MedicalRecord *)node->val;
      if (mrecord->id < mtemp->id)
        mrecord->id = mtemp->id;
    }
  }
  mrecord->id++;
  mrecord->regid = win->data;
  _strdate(datebuf);
  _strtime(timebuf);
  sprintf(mrecord->record_date, "%sT%s", datebuf, timebuf);

  ctrl = findWinByID(win, ID_DOC_TEXTBOX_RESULT);
  if (ctrl)
    strcpy(mrecord->diagnosis, ctrl->title);
  ctrl = findWinByID(win, ID_DOC_TEXTBOX_HANDLER);
  if (ctrl)
    strcpy(mrecord->handler, ctrl->title);
  mrecord->prescriptionid = SavePS(win);

  //保存药品清单
  saveDruglist(win, mrecord->prescriptionid);

  //保存病例
  list_lpush(recordlist, list_node_new(mrecord));
  SaveMedicalRecord(MEDICALRECORDFILE, recordlist);
  list_destroy(recordlist);
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
  case ID_DOC_DELDRUG:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在窗口部分显示手型鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
    //删除药品
    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //鼠标按下
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //鼠标释放
      hbasewinAttr *btn;
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      //btn = findWinByID(win, ID_DOC_QUERY_DRUG);
      //if (btn)
      {
        //查询druglist 中选择的药品,
        hbasewinAttr *druglink = getcheckedlink_doc(win, ID_DOC_PSDRUGLINK, ID_DOC_PSDRUGCHK);
        delDrugfromPSwin(win, druglink);
        drawPrescriptionInfo(win);
      }
    }
    break;
  case ID_DOC_ADDDRUG:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在窗口部分显示手型鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
    //添加药品
    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //鼠标按下
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //鼠标释放
      hbasewinAttr *chk;
      hbasewinAttr *druglink;
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      //查询druglist 中选择的药品,
      druglink = getcheckedlink_doc(win, ID_DOC_DRUGLINK, ID_DOC_DRUGCHK);
      if (druglink)
      { //创建处方link
        addDrugtoPSwin(win, druglink);
        drawPrescriptionInfo(win);
      }
      //TRACE(("%s(%d): 查询药品=%s\n", __FILE__, __LINE__, druglink->title));
    }
    break;
  case ID_DOC_BTN_CONFIRM:
    //诊断完成
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在homepage窗口部分显示标准鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //鼠标按下
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //鼠标释放,修改挂号状态
      hbasewinAttr *ctrl;
      int x = getAbsoluteX(win);
      int y = getAbsoluteY(win);
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      if (win->data < 1)
      {
        hfont *_h;
        _h = getFont(SIMSUN, 16, 0xF801);
        printTextLineXY(x + 20, y + 45, "请选择患者", _h);
        break;
      }
      else
      {
        fillRegionEx(x + 20, y + 45, 200, 16, 0xFFFF);
      }

      ctrl = findWinByID(win, ID_DOC_TEXTBOX_RESULT);

      if (ctrl == NULL || strlen(ctrl->title) < 1)
      {
        hfont *_h;
        _h = getFont(SIMSUN, 16, 0xF801);
        printTextLineXY(x + 710, y + 65, "请输入诊断结果", _h);
        break;
      }
      else
      {
        fillRegionEx(x + 710, y + 65, 200, 16, 0xFFFF);
      }

      ctrl = findWinByID(win, ID_DOC_TEXTBOX_HANDLER);
      if (ctrl == NULL || strlen(ctrl->title) < 1)
      {
        hfont *_h;
        _h = getFont(SIMSUN, 16, 0xF801);
        printTextLineXY(x + 710, y + 125, "请输入处置措施", _h);
        break;
      }
      else
      {
        fillRegionEx(x + 710, y + 125, 200, 16, 0xFFFF);
      }

      SaveDiagnose(win);
      //挂号信息
      fillRegionEx(x + 1, y + 93, 240 - 1, 200 - 5, 0xFFFF);
      fillRegistration(win);
      //删除所有药品
      delallPSDrug(win);
      repaintChildren(win, NULL);

      //绘制处方窗口
      drawRegistration(x, y);
      //绘制患者信息
      drawPatientInfo(win, 0);
      //绘制药品查询窗口
      drawDrugQuery(win);
      //绘制处方药品查询窗口
      fillRegionEx(x + 520, y + 315 + 30, 480, 170, 0xFFFF);
      //fillRegionEx(x + 519, y + 335, 480 - 2, 180, 0x0000);
      drawPrescriptionInfo(win);
      //填充总金额
      fillRegionEx(x + 790, y + 282, 150, 20, 0xFFFF);

      ctrl = findWinByID(win, ID_DOC_TEXTBOX_RESULT);
      if (ctrl)
      {
        if (ctrl->title)
          free(ctrl->title);
        strcpy(ctrl->title, "");
        // if (strlen(ctrl->title) >= 1)
        //   ctrl->title[0] = 0;
        ctrl->onPaint(ctrl, NULL);
      }
      ctrl = findWinByID(win, ID_DOC_TEXTBOX_HANDLER);
      if (ctrl)
      {
        if (strlen(ctrl->title) >= 1)
          ctrl->title[0] = 0;
        ctrl->onPaint(ctrl, NULL);
      }
      // ctrl = findWinByID(win, ID_DOC_QUERY_DRUG);
      // if (ctrl)
      //   ctrl->onPaint(ctrl, NULL);
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

        //TRACE(("%s(%d): 查询药品=%s\n", __FILE__, __LINE__, tb->title));
      }
    }
    break;
  case ID_DOC_BTN_CAL:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在窗口部分显示手型鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
    //计算金额
    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //鼠标按下
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //鼠标释放
      hbasewinAttr *chk;
      hbasewinAttr *druglink;
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      calDrugAmount(win);
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
    if (hitwin->winID >= ID_DOC_REGSLINK && hitwin->winID < ID_DOC_REGSLINK + 8)
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
        int x = getAbsoluteX(win);
        int y = getAbsoluteY(win);
        // hbasewinAttr *pslink;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //挂号单划线
        drawRegistration(x, y);
        //填充患者信息
        drawPatientInfo(win, hitwin->data);

        fillRegionEx(x + 20, y + 45, 200, 16, 0xFFFF);
        fillRegionEx(x + 710, y + 125, 200, 16, 0xFFFF);
        fillRegionEx(x + 710, y + 65, 200, 16, 0xFFFF);

        win->data = hitwin->data;
      }
    }
    else if (hitwin->winID >= ID_DOC_DRUGLINK && hitwin->winID < ID_DOC_DRUGLINK + 6)
    {                                                                             //药品链接
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
    else if (hitwin->winID >= ID_DOC_DRUGCHK && hitwin->winID < ID_DOC_DRUGCHK + 8)
    {                                                                             //药品选择                                                                   //checkbox
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
        hitwin->onPaint(hitwin, NULL);
      }
    }
    else if (hitwin->winID >= ID_DOC_PSDRUGCHK && hitwin->winID < ID_DOC_PSDRUGCHK + 8)
    {
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
        hitwin->onPaint(hitwin, NULL);
      }
    }
    else if (hitwin->winID >= ID_DOC_PSDRUGLINK && hitwin->winID < ID_DOC_PSDRUGLINK + 6)
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

        drawPrescriptionInfo(win);
      }
    }
    else if (hitwin->winID >= ID_DOC_PSDRUGTEXTBOX && hitwin->winID < ID_DOC_PSDRUGTEXTBOX + 6)
    {
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
      }
    }
    else if (hitwin->winID >= ID_DOC_PSDRUGTEXTBOX2 && hitwin->winID < ID_DOC_PSDRUGTEXTBOX2 + 6)
    {
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
      }
    }

    break;
  }
}
