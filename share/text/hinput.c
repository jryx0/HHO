#include "hhosvga.h"
#include "HBaseWin.h"
#include "hinput.h"
#include <stdio.h>
#include <memory.h>

#define FONTSIZE 24
#define MARGIN 2
#define FONTTYPE SIMSUN
#define FONTCOLOR 0xFABF

#define PYWIDTH (FONTSIZE * 10)
#define PYHEIGHT (FONTSIZE * 2 + MARGIN * 4)

hbasewinAttr *CreatePYInput(hbasewinAttr *parent, int startx, int starty, int winID, char *title)
{
  winInputInfo *pi = NULL;
  hbasewinAttr *py = CreateWindowsEx(parent, startx, starty, PYWIDTH, PYHEIGHT, winID, title);

  py->onPaint = OnPaint_PY;
  py->onKey = OnKey_PY;
  py->onKeyPress = OnkeyPress_PY;
  py->onLeave = OnLeave_PY;

  // pi = malloc(sizeof(winInputInfo));
  // memset(pi, 0, sizeof(winInputInfo));
  // pi->status = HHOHIDE;
  
  //py->value = pi;

  return py;
}

/**
 * 处理控制键: 回车/空格
 */
void OnLeave_PY(hbasewinAttr *py, void *val)
{
}
/**
 * 处理控制键: 回车/空格
 */
void OnKey_PY(hbasewinAttr *py, void *val)
{
}

/**
 * 处理按键
 */
void OnkeyPress_PY(hbasewinAttr *py, void *val)
{
}

void OnPaint_PY(hbasewinAttr *py, void *val)
{
  winInputInfo *pyi;

  TESTNULLVOID(py);
  TESTNULLVOID(py->value);
  pyi = (winInputInfo *)py->value;

  if (pyi->status == HHOHIDE) //hide
    return;

  //背景
  fillRegionEx(py->x, py->y, PYWIDTH, PYHEIGHT, 0xFFFF);
  rectangleEx(py->x, py->y, PYWIDTH, PYHEIGHT, FONTCOLOR, 1, 1);

  (void)val;
}

void hidePYInput(hbasewinAttr *py)
{
  winInputInfo *pi;
  TESTNULLVOID(py);
  TESTNULLVOID((py->value));

  if (pi->status == HHOSHOW)
  {
    FILE *pf = fopen("input.bg", "wb+");
    pi = (winInputInfo *)(py->value);
    //if (pi->pyBK)
    //  restorebackgroundEx(pi->pyBK, py->x, py->y, PYWIDTH, PYHEIGHT);

    restorebackgroundFile(pf, py->x, py->y, PYWIDTH, PYHEIGHT);
    pi->status = HHOHIDE;

    pi->page = 0;
    memset(pi->pinyin, 0, 6);
    if (pi->hz)
      free(pi->hz);
    pi->hz = NULL;
  }
}

void showPYInput(hbasewinAttr *py, int newx, int newy)
{
  winInputInfo *pi = (winInputInfo *)(py->value);
  FILE *pf = fopen("input.bg", "wb+");

  TESTNULLVOID(py);
  if (pi && pi->status == HHOHIDE)
  {
    pi->status = HHOSHOW;
    py->x = newx;
    py->y = newy;
    //调整坐标
    if (py->x + py->nWidth > PAGE_W)
      py->x = PAGE_W - py->nWidth - 5;
    if (py->y + py->nHeight > PAGE_W)
      py->y = py->y - PYHEIGHT - 5;

    // if (pi->pyBK == NULL)
    // {
    //   TRACE(("%s(%d):输入法窗口背景保存空间为空", __FILE__, __LINE__));
    //   return;
    // }
    //savebackgroundEx(pi->pyBK, py->x, py->y, PYWIDTH, PYHEIGHT);
    savebackgroundFile(pf, py->x, py->y, PYWIDTH, PYHEIGHT);
    OnPaint_PY(py, NULL);
  }
}