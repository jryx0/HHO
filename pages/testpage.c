
#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"

#include "testpage.h"

hbasewinAttr *CreateTestPage(hbasewinAttr *parent, int winID, char *title)
{
  hbasewinAttr *testPage = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, title);

  TESTNULL(testPage, NULL);

  testPage->onPaint = OnPaint_TestPage;

  return testPage;
}

void OnPaint_TestPage(hbasewinAttr *win, void *value)
{
  OnPaint(win, value);

  if (win->title)
  {
    hfont *_h = getFont(SIMKAI, 24, 0x403F);
    printTextLineXY(100, 100, win->title, _h);
    freeFont(_h);
  }
}