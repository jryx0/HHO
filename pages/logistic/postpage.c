#include "macrodef.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "wResource.h"
#include "hhosvga.h"
#include "list.h"
#include "hglobal.h"
#include "postpage.h"
#include "htextbox.h"
#include "hbutton.h"
#include "data.h"

#define COL1 100
#define COL2 (COL1 + 90)
#define COL3 (COL2 + 180)
#define COL4 (COL3 + 120)
#define COL5 (COL4 + 80)
#define COL6 (COL5 + 180)
#define COL7 (COL6 + 160)
#define COL8 120

void createPostinfo(hbasewinAttr *parent, int userid)
{
  int i;
  list_node_t *node;
  postInfo *pi;
  int x, y;
  hbasewinAttr *lnk;
  char info[256];
  list_t *postlist;

  TESTNULLVOID(parent);
  TESTNULLVOID(postlist);
  postlist = ReadPostInfo(DATAPATH "database\\logistic\\postinfo.txt");

  x = 20;
  y = 110;

  for (i = 0; i < postlist->len && i < 7; i++)
  {
    node = list_at(postlist, i);
    if (node && node->val)
    {

      pi = (postInfo *)node->val;

      sprintf(info, "%d       %s       %s    %s     %s   %s    %s    %s",
              pi->postid,
              pi->receiver, pi->receiveraddr, pi->tel,
              pi->shipper, pi->shipperaddr, pi->shiptime,
              pi->status);

      if (userid == -1)
      { //物流人员可以查看所有运单
        lnk = Createhyperlink(parent, x, y + 30 * i, PAGE_W, 25, ID_POST_LINK + i, info);
        lnk->data = pi->postid;
        lnk->wintype = HYPERLINK_BK;
      }
      else if (userid == pi->userid)
      {
        lnk = Createhyperlink(parent, x, y + 30 * i, PAGE_W, 25, ID_POST_LINK + i, info);
        lnk->data = pi->postid;
        lnk->wintype = HYPERLINK_BK;
      }
      //btn = CreateButton(parent, x, y + 50 * i, 125, 45, ID_DEPT_LINK + i, dept->deptname);
      //btn->data = dept->id;
    }
  }

  list_destroy(postlist);
}

void redrawpostitem(int x, int y)
{
  fillRegionEx(x + 1, y + 370 + 1, 460 - 1, 179, 0xFFFF);
  lineyEx(x + 150, y + 370, 179, 0x6BAF);
  lineyEx(x + 350, y + 370, 179, 0x6BAF);
}

void showPostInfo(hbasewinAttr *win, int postid)
{
  char infostr[64];
  hfont *_h;
  WinStyle *style;
  postInfo *pi;
  FILE *fp;
  int i = 0;
  int x, y;

  TESTNULLVOID(win);
  pi = fFindPostInfo(DATAPATH "database\\logistic\\postinfo.txt", postid);

  if (pi)
  {
    style = win->style;
    _h = getFont(SIMHEI, style->fontsize, 0x0000);
    //运单号
    sprintf(infostr, "%d", pi->postid);
    fillRegionEx(win->x + 570, win->y + 350, 100, 20, 0xFFFF);
    printTextLineXY(win->x + 570, win->y + 350, infostr, _h);
    //发货人
    sprintf(infostr, "%s", pi->shipper);
    fillRegionEx(win->x + 745, win->y + 350, 100, 20, 0xFFFF);
    printTextLineXY(win->x + 745, win->y + 350, infostr, _h);
    //收货人
    sprintf(infostr, "%s", pi->receiver);
    fillRegionEx(win->x + 920, win->y + 350, 100, 20, 0xFFFF);
    printTextLineXY(win->x + 920, win->y + 350, infostr, _h);
    //收货人地址
    sprintf(infostr, "%s", pi->receiveraddr);
    fillRegionEx(win->x + 630, win->y + 390, 150, 20, 0xFFFF);
    printTextLineXY(win->x + 630, win->y + 390, infostr, _h);

    //收货人电话
    sprintf(infostr, "%s", pi->tel);
    fillRegionEx(win->x + 630, win->y + 430, 150, 20, 0xFFFF);
    printTextLineXY(win->x + 630, win->y + 430, infostr, _h);

    sprintf(infostr, "%sdatabase\\logistic\\%d.txt", DATAPATH, postid);

    redrawpostitem(getAbsoluteX(win), getAbsoluteY(win));
    fp = fopen(infostr, "r");
    if (fp)
    {
      postitem pi;
      while (fgets(infostr, 64, fp))
      {
        if (infostr[0] == '#')
          continue;

        sscanf(infostr, "%s\t%s\t%s", pi.arrivedaddr, pi.status, pi.arrivedtime);
        sprintf(infostr, "%s            %s      %s", pi.arrivedaddr, pi.arrivedtime, pi.status);
        printTextLineXY(win->x + 10, win->y + 380 + i * 20, infostr, _h);
        i++;
      }
      fclose(fp);
    }

    freeFont(_h);

    //重新画竖线
    x = getAbsoluteX(win);
    y = getAbsoluteY(win);
    lineyEx(x + COL1, y + 80, PAGE_H / 2 - 50, 0x6BAF);
    lineyEx(x + COL2, y + 80, PAGE_H / 2 - 50, 0x6BAF);
    lineyEx(x + COL3, y + 80, PAGE_H / 2 - 50, 0x6BAF);
    lineyEx(x + COL4, y + 80, PAGE_H / 2 - 50, 0x6BAF);
    lineyEx(x + COL5, y + 80, PAGE_H / 2 - 50, 0x6BAF);
    lineyEx(x + COL6, y + 80, PAGE_H / 2 - 50, 0x6BAF);
    lineyEx(x + COL7, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  }
}

hbasewinAttr *CreatePostpage(hbasewinAttr *parent, int winID, int userid)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, NULL);
  page->onPaint = OnPaint_Post;
  Createhyperlink(page, 20, 10, 65, 25, ID_POST_RETURN, "[首 页]");
  page->EventHandler = EventHandler_postpage;

  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);

  Createhyperlink(page, page->x, page->y + 190, 55, 25, ID_POST_PREV, "上一页");
  Createhyperlink(page, page->x + 65, page->y + 190, 55, 25, ID_POST_NEXT, "下一页");

  CreateTextBox(page, 72, 43, 120, 18, ID_POST_QUERY_BILLNUM, NULL, 1);
  CreateTextBox(page, 280, 43, 100, 20, ID_POST_QUERY_SHIPPER, NULL, 1);
  CreateTextBox(page, 500, 43, 150, 20, ID_POST_QUERY_SHIPPERTEL, NULL, 1);
  CreateTextBox(page, 730, 43, 150, 20, ID_POST_QUERY_RECEIVER, NULL, 1);

  CreateButton(page, 890, 43, 120, 30, ID_POST_QUERY_RECEIVER, "查  询");
  CreateButton(page, 790, 510, 150, 40, ID_POST_UPDATEINFO, "更新物流信息");
  CreateTextBox(page, 580, 465, 150, 20, ID_POST_UPDATEADDR, NULL, 1);
  CreateTextBox(page, 805, 465, 150, 20, ID_POST_UPDATESTATUS, NULL, 1);

  createPostinfo(page, userid);

  return page;
}

void OnPaint_Post(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  repaintChildren(win, val);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  Putbmp565(PAGE_W - 250, y, DATAPATH "database\\logistic\\postlogo.565");
  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- 物流信息", _h);

  rectangleEx(x, y + 80, PAGE_W, PAGE_H / 2 - 50, 0x6BAF, 1, 1);

  printTextLineXY(x + 10, y + 84, " 运单号      收货人       收货地址         收货人电话    发货人      发货地址           发货时间         状态", _h);
  lineyEx(x + COL1, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL2, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL3, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL4, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL5, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL6, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL7, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  linex_styleEx(x, y + 104, PAGE_W, 0x6BAF, 1, 1);

  rectangleEx(x, y + 345, 460, 205, 0x6BAF, 1, 1);
  printTextLineXY(x + 10, y + 350, "   到达位置            到达时间           状态", _h);
  lineyEx(x + 150, y + 345, 205, 0x6BAF);
  lineyEx(x + 350, y + 345, 205, 0x6BAF);
  linex_styleEx(x, y + 370, 460, 0x6BAF, 1, 1);

  printTextLineXY(x + 10, y + 50, "运单号:", _h);
  printTextLineXY(x + 210, y + 48, "收货人:", _h);
  printTextLineXY(x + 400, y + 48, "收货人电话:", _h);
  printTextLineXY(x + 665, y + 48, "发货人:", _h);

  printTextLineXY(x + 510, y + 350, "运单号:", _h);
  printTextLineXY(x + 680, y + 350, "发货人:", _h);
  printTextLineXY(x + 850, y + 350, "收货人:", _h);
  //printTextLineXY(x + 510, y + 420, "收货人:", _h);
  printTextLineXY(x + 510, y + 390, "收货人地址:", _h);
  printTextLineXY(x + 510, y + 430, "收货人电话:", _h);

  printTextLineXY(x + 510, y + 470, "配送站:", _h);
  printTextLineXY(x + 750, y + 470, "状态:", _h);

  freeFont(_h);
  //repaintChildren(win, val);

  // rectangleEx(x + 15, y + 35, 140, PAGE_H - 10, 0x0000, 1, 2);
  // rectangleEx(x + 160, y + 25, PAGE_W - 250, PAGE_H - 100, 0x0000, 1, 2);
}

void EventHandler_postpage(hbasewinAttr *win, int type, void *val)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(val);
  _g = (globaldef *)val;
  hitwin = checkmousewin(win, &_g->mouse);

  if (type == EVENT_MOUSE)
  {
    switch (hitwin->winID)
    {
    case ID_POSTPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
      break;
    case ID_POST_RETURN:
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
    case ID_POST_UPDATEADDR:   //更新配送站
    case ID_POST_UPDATESTATUS: //更新状态
    case ID_POST_QUERY_BILLNUM:
    case ID_POST_QUERY_RECEIVER:
    case ID_POST_QUERY_SHIPPER:
    case ID_POST_QUERY_SHIPPERTEL:
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
      break;
    case ID_POST_UPDATEINFO:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放
        hbasewinAttr *addr, *status;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        addr = findWinByID(win, ID_POST_UPDATEADDR);
        status = findWinByID(win, ID_POST_UPDATESTATUS);

        if (addr && addr->title && status && status->title)
        { //写文件
          //更新界面
          char info[64];
          char datebuf[9], timebuf[9];
          FILE *fp;
          //文件名
          sprintf(info, "%sdatabase\\logistic\\%d.txt", DATAPATH, hitwin->data);
          fp = fopen(info, "a");
          if (fp)
          {
            _strdate(datebuf);
            _strtime(timebuf);
            //运单编号 配送站 状态
            sprintf(info, "%s\t%s\t%sT%s\n", addr->title, status->title, datebuf, timebuf);
            fputs(info, fp);
            TRACE(("%s\n", info));
            fclose(fp);
            showPostInfo(win, hitwin->data);
          }
        }
      }
      break;
    default:
      if (hitwin->winID >= ID_POST_LINK && hitwin->wintype < ID_POST_LINK + 10)
      {                                                                             //点击物流运单信息
        if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
          _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

        if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
        { //鼠标按下
          if (hitwin->onClick)
            hitwin->onClick(hitwin, NULL);
        }
        else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        { //鼠标释放

          hbasewinAttr *info;
          if (hitwin->onLeave)
            hitwin->onLeave(hitwin, NULL);

          info = findWinByID(win, ID_POST_UPDATEINFO);
          //保留当前点击的运单号导"更新物流信息按钮"
          info->data = hitwin->data;
          //显示明细
          showPostInfo(win, hitwin->data);
        }
      }
      break;
    }
  }
}