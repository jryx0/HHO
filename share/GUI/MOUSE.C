#include "macrodef.h"
#include "hhogui.h"
#include "mouse.h"

#include <dos.h>
#include <stdio.h>

#define MOUSEWIDTH 20
#define MOUSEHIGHT 30
int const mouse_shape[MOUSEHIGHT][MOUSEWIDTH] =
    {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 2, 1, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};
int mouse_bk[MOUSEHIGHT][MOUSEWIDTH];

int initMouse(void)
{
  union REGS regs;

  /*设置鼠标的功能号*/
  regs.x.ax = 0;
  int86(0x33, &regs, &regs);
  return regs.x.ax;
}

void setMouseRange(int Xmin, int Ymin, int Xmax, int Ymax)
{
  union REGS Inr, Outr;
  Inr.x.cx = Xmin;
  Inr.x.dx = Xmax;
  Inr.x.ax = 7;
  int86(0x33, &Inr, &Outr);

  Inr.x.cx = Ymin;
  Inr.x.dx = Ymax;
  Inr.x.ax = 8;
  int86(0x33, &Inr, &Outr);
}

void MouseXYB(int *status, int *x, int *y)
{

  union REGS Inr, Outr;
  Inr.x.ax = 3;
  int86(0x33, &Inr, &Outr);
  *status = Outr.x.bx;
  *x = Outr.x.cx;
  *y = Outr.x.dx;
}

int MouseBarLeft(Area mouse_area) //4
{
  /*存放鼠标状态的结构体*/
  MOUSE mouse = {0, 0, 0};

  /*读取鼠标状态*/
  MouseXYB(&mouse.but, &mouse.position.x, &mouse.position.y);

  /*判断鼠标左键是否在指定区域内按下*/
  if ((mouse.position.x >= mouse_area.lt.x) && (mouse.position.x <= mouse_area.rb.x) && (mouse.position.y >= mouse_area.lt.y) && (mouse.position.y <= mouse_area.rb.y) && ((mouse.but & 1) == 1))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void MouseStoreBk(Coordinate position) //5
{
  savebackgroundEx(mouse_bk, position.x, position.y, MOUSEWIDTH, MOUSEHIGHT);
}

void MousePutBk(Coordinate position) //6
{
  restorebackgroundEx(mouse_bk, position.x, position.y, MOUSEWIDTH, MOUSEHIGHT);
}

void MouseReset(void) //7
{
  Area mouse_area = {{0, 0}, {SCR_WIDTH - 1, SCR_HEIGHT - 1}};
  if (initMouse() == 0)
  {
    printf("ERROR In MouseInit\n");
    exit(1);
  }

  setMouseRange(0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1);
  MouseStoreBk(mouse_area.lt);
}

void MouseDraw(MOUSE mouse) //8
{
  int i, j;
  for (i = 0; i < MOUSEHIGHT; i++)
  {
    for (j = 0; j < MOUSEWIDTH; j++)
    {
      if (mouse_shape[i][j] == 0)
      {
        continue;
      }
      else if (mouse_shape[i][j] == 1)
      {
        putpixel64k(j + mouse.position.x, i + mouse.position.y, 0);
      }
      else if (mouse_shape[i][j] == 2)
      {
        putpixel64k(j + mouse.position.x, i + mouse.position.y, 0xffff);
      }
    }
  }
}
