#include "signpage.h"
#include "HBaseWin.h"
#include "macrodef.h"
#include "hhosvga.h"
#include "Resource.h"
#include "hglobal.h"
#include "htextbox.h"
#include "hyperlnk.h"
#include "hbutton.h"
#include "data.h"
#include <time.h>
#include <string.h>

hbasewinAttr *Createsignpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "signpage");
  hbasewinAttr *pwd;
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_signpage;
  page->EventHandler = EventHandler_signpage;

  Createhyperlink(page, 20, 10, 65, 25, ID_SIGNIN_RETURN, "[首 页]");
  CreateTextBox(page, 240, 95, 100, 26, ID_SIGNIN_NAME, "", 1);
  CreateTextBox(page, 610, 95, 80, 26, ID_SIGNIN_SEX, "", 1);
  CreateTextBox(page, 240, 166, 80, 26, ID_SIGNIN_MARRIAGESTATUS, "", 1);
  CreateTextBox(page, 610, 166, 230, 26, ID_SIGNIN_IDNUMBER, "", 1);
  CreateTextBox(page, 240, 237, 80, 26, ID_SIGNIN_BIRTH_YEAR, "", 1);
  CreateTextBox(page, 360, 237, 50, 26, ID_SIGNIN_BIRTH_MONTH, "", 1);
  CreateTextBox(page, 610, 237, 200, 26, ID_SIGNIN_TELENUM, "", 1);
  CreateTextBox(page, 240, 297, 250, 26, ID_SIGNIN_ADDRESS, "", 1);
  CreateTextBox(page, 610, 297, 150, 26, ID_SIGNIN_ALLERGY, "", 1);
  CreateTextBox(page, 240, 354, 120, 26, ID_SIGNIN_USERNAME, "", 1);
  pwd = CreateTextBox(page, 240, 411, 150, 26, ID_SIGNIN_PASSWORD1, "", 1);
  pwd->wintype = TEXTBOX_PASSWORD;
  pwd = CreateTextBox(page, 610, 411, 150, 26, ID_SIGNIN_PASSWORD2, "", 1);
  pwd->wintype = TEXTBOX_PASSWORD;
  CreateButton(page, PAGE_W / 2 - 60, 485, 120, 35, ID_SIGNIN_SIGNIN, "注册");
  return page;
}

void OnPaint_signpage(hbasewinAttr *win, void *value)
{
  int x, y;
  hfont *_h1 = getFont(SIMSUN, 16, 0);
  hfont *_h2 = getFont(SIMSUN, 16, 0xFFFF);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  Putbmp565(x - 4, y + 30, DATAPATH "bmp\\login.565");

  rectangleEx(x + 120 - 1, y + 40 - 1, 771, 501, 0x6BAF, 1, 1);
  fillRegionEx(x + 120, y + 40, 770, 500, 0xFFFF);
  fillRegionEx(x + 120, y + 40, 770, 40, 0x03DF);
  linex_styleEx(x + 120, y + 80, 770, 0x6BAF, 1, 1);

  printTextLineXY(x + 96, y + 12, "- 用户注册", _h1);
  printTextLineXY(x + 125, y + 53, "请输入基本信息：", _h2);
  printTextLineXY(x + 192, y + 103, "姓名：", _h1);
  printTextLineXY(x + 562, y + 103, "性别：", _h1);
  printTextLineXY(x + 530, y + 138, "（请在此栏中输入“男”或“女”）", _h1);
  printTextLineXY(x + 176, y + 174, "婚姻史：", _h1);
  printTextLineXY(x + 155, y + 209, "（请在此栏中输入“未婚”“已婚”或“离异”）", _h1);
  printTextLineXY(x + 530, y + 174, "身份证号：", _h1);
  printTextLineXY(x + 160, y + 245, "出生年月：", _h1);
  printTextLineXY(x + 332, y + 245, "年", _h1);
  printTextLineXY(x + 422, y + 245, "月", _h1);
  printTextLineXY(x + 530, y + 245, "联系电话：", _h1);
  printTextLineXY(x + 192, y + 307, "住址：", _h1);
  printTextLineXY(x + 546, y + 307, "过敏史：", _h1);
  printTextLineXY(x + 176, y + 364, "用户名：", _h1);
  printTextLineXY(x + 192, y + 421, "密码：", _h1);
  printTextLineXY(x + 530, y + 421, "确认密码：", _h1);
  repaintChildren(win, value);
  freeFont(_h1);
  freeFont(_h2);
}
void EventHandler_signpage(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  hitwin = checkmousewin(win, &_g->mouse);

  switch (type)
  {
  case EVENT_MOUSE:
    switch (hitwin->winID)
    {
    case ID_SIGNINPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;

    case ID_SIGNIN_RETURN:
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

    case ID_SIGNIN_NAME:
    case ID_SIGNIN_SEX:
    case ID_SIGNIN_MARRIAGESTATUS:
    case ID_SIGNIN_IDNUMBER:
    case ID_SIGNIN_BIRTH_YEAR:
    case ID_SIGNIN_BIRTH_MONTH:
    case ID_SIGNIN_TELENUM:
    case ID_SIGNIN_ADDRESS:
    case ID_SIGNIN_ALLERGY:
    case ID_SIGNIN_USERNAME:
    case ID_SIGNIN_PASSWORD1:
    case ID_SIGNIN_PASSWORD2:
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
      break;

    case ID_SIGNIN_SIGNIN:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放
        hfont *_h = getFont(SIMSUN, 16, 0xF801);
        list_t *palist = ReadPatientInfo(PATIENTINFOFILE);
        list_t *userlist = ReadUserInfo(USERINFOFILE);
        hbasewinAttr *temp, *pwd;
        userInfo *user;
        PatientInfo *patient;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        fillRegionEx(win->x + 125, win->y + 445, 760, 38, 0xFFFF);
        patient = (PatientInfo *)malloc(sizeof(PatientInfo));
        memset(patient, 0, sizeof(PatientInfo));

        if (palist->len == 0)
          patient->id = 1001;
        else
        {
          patient = (PatientInfo *)palist->tail->val;
          patient->id++;
        }

        //输入名字
        temp = findWinByID(win, ID_SIGNIN_NAME);
        if (temp->title && strlen(temp->title) > 0 && strlen(temp->title) < 9)
          strcpy(patient->name, temp->title);
        else if (strlen(temp->title) > 8) //to do:判断汉字
        {
          printTextLineXY(PAGE_W / 2 - 80, win->y + 460, "请输入四字以内的姓名", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else
        {
          printTextLineXY(PAGE_W / 2 - 40, win->y + 460, "请输入姓名", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }

        //输入性别
        temp = findWinByID(win, ID_SIGNIN_SEX);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 40, win->y + 460, "请输入性别", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (0 == strcmp(temp->title, "男"))
          patient->sex = 1;
        else if (0 == strcmp(temp->title, "女"))
          patient->sex = 0;
        else
        {
          printTextLineXY(PAGE_W / 2 - 56, win->y + 460, "请正确输入性别", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }

        //输入婚姻史
        temp = findWinByID(win, ID_SIGNIN_MARRIAGESTATUS);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 48, win->y + 460, "请输入婚姻史", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (0 == strcmp(temp->title, "未婚"))
          patient->marriagestatus = 0;
        else if (0 == strcmp(temp->title, "已婚"))
          patient->marriagestatus = 1;
        else if (0 == strcmp(temp->title, "离异"))
          patient->marriagestatus = 2;
        else
        {
          printTextLineXY(PAGE_W / 2 - 64, win->y + 460, "请正确输入婚姻史", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }

        //输入身份证号
        temp = findWinByID(win, ID_SIGNIN_IDNUMBER);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 56, win->y + 460, "请输入身份证号", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (id_num_judge(temp->title) == 1)
          strcpy(patient->id_number, temp->title);
        else
        {
          printTextLineXY(PAGE_W / 2 - 72, win->y + 460, "请正确输入身份证号", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }

        //输入出生年份
        temp = findWinByID(win, ID_SIGNIN_BIRTH_YEAR);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 56, win->y + 460, "请输入出生年份", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (patient->year = atoi(temp->title) == 0 || patient->year < 1900 || patient->year > 2020)
        {
          printTextLineXY(PAGE_W / 2 - 72, win->y + 460, "请正确输入出生年份", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }

        //输入出生月份
        temp = findWinByID(win, ID_SIGNIN_BIRTH_MONTH);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 56, win->y + 460, "请输入出生月份", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (patient->month = atoi(temp->title) == 0 || patient->month < 0 || patient->month > 12)
        {
          printTextLineXY(PAGE_W / 2 - 72, win->y + 460, "请正确输入出生月份", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }

        //输入电话号码
        temp = findWinByID(win, ID_SIGNIN_TELENUM);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 56, win->y + 460, "请输入联系电话", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (telenum_judge(temp->title))
        {
          printTextLineXY(PAGE_W / 2 - 72, win->y + 460, "请正确输入联系电话", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        strcpy(patient->telenum, temp->title);

        //输入住址
        temp = findWinByID(win, ID_SIGNIN_ADDRESS);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 40, win->y + 460, "请输入住址", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (strlen(temp->title) > 31)
        {
          printTextLineXY(PAGE_W / 2 - 48, win->y + 460, "输入住址过长", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        strcpy(patient->addr, temp->title);

        //输入过敏史
        temp = findWinByID(win, ID_SIGNIN_ADDRESS);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 48, win->y + 460, "请输入过敏史", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        else if (strlen(temp->title) > 15)
        {
          printTextLineXY(PAGE_W / 2 - 56, win->y + 460, "输入过敏史过长", _h);
          freeFont(_h);
          temp->title = "";
          temp->onActivate(temp, _g);
          free(patient);
          break;
        }
        strcpy(patient->allergy, temp->title);

        user = (userInfo *)malloc(sizeof(userInfo));
        memset(user, 0, sizeof(userInfo));
        user->userID = patient->id;
        user->userType = PATIENT;

        //输入用户名
        temp = findWinByID(win, ID_SIGNIN_USERNAME);
        if (temp->title == NULL || strlen(temp->title) == 0)
        {
          printTextLineXY(PAGE_W / 2 - 48, win->y + 460, "请输入用户名", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(user);
          free(patient);
          break;
        }
        else if (fFindUsername(USERINFOFILE, temp->title))
        {
          temp->title = "";
          printTextLineXY(PAGE_W / 2 - 48, win->y + 460, "用户名已存在", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(user);
          free(patient);
          break;
        }
        else if (strlen(temp->title) > 9)
        {
          temp->title = "";
          printTextLineXY(PAGE_W / 2 - 40, win->y + 460, "用户名过长", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(user);
          free(patient);
          break;
        }
        strcpy(user->username, temp->title);

        temp = findWinByID(win, ID_SIGNIN_PASSWORD1);
        pwd = findWinByID(win, ID_SIGNIN_PASSWORD2);
        if (strcmp(temp->title, pwd->title) == 0)
          strcpy(user->password, temp->title);
        else
        {
          temp->title = "";
          pwd->title = "";
          printTextLineXY(PAGE_W / 2 - 96, win->y + 460, "两次密码不同，请重新输入", _h);
          freeFont(_h);
          temp->onActivate(temp, _g);
          free(user);
          free(patient);
          break;
        }

        //存储新建病人信息
        list_rpush(palist, list_node_new(patient));
        SavePatientInfo(palist, PATIENTINFOFILE);
        free(patient);
        list_destroy(palist);

        list_rpush(userlist, list_node_new(user));
        SaveUserInfo(userlist, USERINFOFILE);
        free(user);
        list_destroy(userlist);

        printTextLineXY(PAGE_W / 2 - 32, win->y + 460, "注册成功！", _h);
        freeFont(_h);
        delay(1200);
        //转跳homepage
        if (win->parent && win->parent->winID == ID_DESKTOP) //找到desktop
        {
          _g->activePageID = ID_LOGINPAGE;
          win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
        }
      }
      break;

    default:
      break;
    }
    break;
  default:
    break;
  }
  (void)type;
}

int id_num_judge(char *id_num)
{
  int i, len = strlen(id_num);
  if (len != 18)
    return 0;
  for (i = 0; i < len - 1; i++)
    if (id_num[i] < '0' || id_num[i] > '9')
      return 0;
  if ((id_num[i] < '0' || id_num[i] > '9') && id_num[i] != 'X' && id_num[i] != 'x')
    return 0;
  return 1;
}
int telenum_judge(char *telenum)
{
  int i, len = strlen(telenum);
  if (len != 11 && len != 8)
    return 0;
  for (i = 0; i < len; i++)
    if (telenum[i] < '0' || telenum[i] > '9')
      return 0;
  return 1;
}