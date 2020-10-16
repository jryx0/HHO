#include "macrodef.h"
#include "mouse.h"
#include "hglobal.h"
#include "hhosvga.h"
#include "wdesktop.h"
#include "HBaseWin.h"
#include "wResource.h"
#include "hlabel.h"
#include "hyperlnk.h"
#include "hbutton.h"
#include "hinput.h"

#include "testpage.h"
#include "homepage.h"

/**
 * ���ڻ��Ƴ���
 * 
 */
void OnPaint_Desktop(hbasewinAttr *win, void *val)
{
  hfont *_font;

  if (win == NULL)
    return;

  clearScreen(COLOR_white);
  //draw header
  Putbmp64k(0, 2, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linex_styleEx(0, HEADER_HEIGHT - 44, SCR_WIDTH, 0x4A96, 3, 1);

  repaintChildren(win);

  //draw footer
  linex_styleEx(0, SCR_HEIGHT - FOOTER_HEIGHT, SCR_WIDTH, 0x4A96, 3, 1);
  _font = getFont(SIMSUN, 16, 0x00);
  _font->fontcolor = 0x4228;
  printTextLineXY(215, SCR_HEIGHT - FOOTER_HEIGHT + 15, "��ַ������ʡ�人�к�ɽ������·1037��      ��ѯ�绰:027-87543469", _font);
  freeFont(_font);

  (void)val;
}

/**
 * ����������
 * 
 */
hbasewinAttr *CreateDesktop(void)
{
  hbasewinAttr *desktop;

  desktop = CreateWindowsEx(NULL, 0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, ID_DESKTOP, "desktop");

  //�����˵�,�л�ҳ��,��ʱʹ��
  CreateButton(desktop, 450, HEADER_HEIGHT - 46, 120, 44, ID_MENU_HOMEPAGE, "��ҳ");
  CreateButton(desktop, 600, HEADER_HEIGHT - 46, 150, 44, ID_MENU_TESTPAGE, "����ҳ");
  CreateButton(desktop, 850, HEADER_HEIGHT - 46, 150, 44, ID_MENU_EXIT, "�˳�");

  Createhyperlink(desktop, 900, SCR_HEIGHT - FOOTER_HEIGHT / 2, 100, 30, ID_MENU_PY, "����");

  //������ҳ
  CreateHomepage(desktop, ID_HOMEPAGE);

  desktop->onPaint = OnPaint_Desktop;
  desktop->EventHandler = eventhandlerdesktop;

  return desktop;
}

/**
 * Desktopר�ú�����Ϊdesktop����ҳ��
 * 
 */
hbasewinAttr *pageFactory(hbasewinAttr *desktop, int winID)
{ //page�Ĵ����������Ʋ�һ����������һ����������ֵһ��������������ͳһ����������page
  hbasewinAttr *newpage = NULL;

  TESTNULL(desktop, NULL);
  switch (winID)
  {
  case ID_HOMEPAGE:
    newpage = CreateHomepage(desktop, ID_HOMEPAGE);
    break;

  case ID_TESTPAGE:
    newpage = CreateTestPage(desktop, ID_TESTPAGE, "����ҳ��-������.....");
    break;
  /*
   * case :
   * case : 
   */
  default:
    break;
  }
  return newpage;
}
/**
 * Desktopר�ú�����Ϊdesktop�л�ҳ��
 * 
 */
void Desktop_changePage(hbasewinAttr *desktop, int pageID, hbasewinAttr *activePage)
{
  globaldef *_g;
  list_node_t *node;
  hbasewinAttr *py;

  TESTNULLVOID(desktop);
  if (activePage && activePage->winID != pageID)
  { //��ǰ�page����pageIDҳ�棬��ɾ������ҳ��
    clearWinRegion(activePage, 0xFFFF);
    if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
      _g->foucsedTextBox->onActivate(NULL, _g);

    if (activePage->onDestroy)
    {
      activePage->onDestroy(activePage, NULL);
    }
    activePage = NULL;
  }

  if (activePage == NULL)
  { //������homepage
    activePage = pageFactory(desktop, pageID);
    if (activePage == NULL)
    {
      TRACE(("%s(%d):Desktop_changePage(), ��������ʧ�ܣ�\n", __FILE__, __LINE__));
      return;
    }
    if (activePage->onPaint)
      activePage->onPaint(activePage, NULL);
  }

  // py = findWinByID(desktop, ID_MENU_PY);
  // if(py)
  //   hidePYInput(py);
}

/**
 * ������Ϣ������
 * 
 * @param win ������Ϣ����
 * @param type �¼�����
 * @param value ����
 */
void eventhandlerdesktop(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  switch (type)
  {
  case EVENT_PAGE_CHANGE:
    //���͵�desktop����
    Desktop_changePage(win, _g->activePageID, _g->activePage); //ʹ��win
    break;
  case EVENT_MOUSE:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

    switch (win->winID)
    {
    case ID_MENU_TESTPAGE:

      /* �ı������״���ı䱳����ɫ */
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //��갴��,�ı䰴ť��ʽ
        if (win->onClick)
          win->onClick(win, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //��굯��
        //�л�ҳ��
        if (win->onLeave)
          win->onLeave(win, NULL);
        Desktop_changePage(win->parent, ID_TESTPAGE, _g->activePage); //ʹ��win->parent
      }

      break;
    case ID_MENU_HOMEPAGE:
      /* �ı������״���ı䱳����ɫ */
      //.....
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //��갴��
        if (win->onClick)
          win->onClick(win, NULL);       
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //��굯��
        //�л�ҳ��
        if (win->onLeave)
          win->onLeave(win, NULL);
        Desktop_changePage(win->parent, ID_HOMEPAGE, _g->activePage);
      }
      break;
    case ID_MENU_EXIT:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //��갴��
        if (win->onClick)
          win->onClick(win, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //��굯��
        //�л�ҳ��
        _g->isExit = TRUE;
      }
      break;
    default:
      break;
    }
  default:
    break;
  }
}

/*
void Desktop_MouseHandler(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  if (win->winID == ID_DESKTOP) //desktop��������������
    return;

  if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
  { //��갴������
    switch (win->winID)
    {
    case ID_MENU_HOMEPAGE:
      //�����ҳ��ť
      if (_g->activePage && _g->activePage->winID != ID_HOMEPAGE)
      { //��ǰ�page����homepageҳ�棬��ɾ������ҳ��
        clearWinRegion(_g->activePage, 0xFFFF);
        _g->activePage->onDestroy(_g->activePage, NULL);
        _g->activePage = NULL;
      }

      if (_g->activePage == NULL)
      { //������homepage
        _g->activePage = CreateHomepage(win->parent, ID_HOMEPAGE);
        if (_g->activePage == NULL)
        {
          TRACE(("%s(%d):eventhandlerdesktop(), ��������ʧ�ܣ�\n"));
          return NULL;
        }
        if (_g->activePage->onPaint)
          _g->activePage->onPaint(_g->activePage, NULL);
      }

      break;
    case ID_MENU_TESTPAGE: //�������ҳ��ť
      if (_g->activePage && _g->activePage->winID != ID_TESTPAGE)
      { //��ǰ�page����homepageҳ�棬��ɾ������ҳ��
        clearWinRegion(_g->activePage, 0xFFFF);
        _g->activePage->onDestroy(_g->activePage, NULL);
        _g->activePage = NULL;
      }

      if (_g->activePage == NULL)
      { //����homepage

        _g->activePage = CreateTestPage(win->parent, ID_TESTPAGE, "����ҳ��-������.....");
        if (_g->activePage == NULL)
        {
          TRACE(("%s(%d):eventhandlerdesktop(), ��������ʧ�ܣ�\n"));
          return NULL;
        }

        if (_g->activePage->onPaint)
          _g->activePage->onPaint(_g->activePage, NULL);
      }
      break;
    default:
      break;
    }
  }
}
*/