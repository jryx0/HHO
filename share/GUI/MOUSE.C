#include "macrodef.h"
#include "hhogui.h"
//#include "SVGAUTIL.H"
#include "mouse.h"

#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <mem.h>

// long mouseback[16][16];

int InitMouse(void)
{
  union REGS Inr, Outr;
  Inr.x.ax = 0;
  int86(0x33, &Inr, &Outr);
  return Outr.x.ax;
}
void ShowMouse(void)
{
  union REGS Inr, Outr;
  Inr.x.ax = 1;
  int86(0x33, &Inr, &Outr);
}
void HideMouse(void)
{
  union REGS Inr, Outr;
  Inr.x.ax = 2;
  int86(0x33, &Inr, &Outr);
}

void ReadMouse(int *f, int *x, int *y)
{
  union REGS Inr, Outr;
  Inr.x.ax = 3;
  int86(0x33, &Inr, &Outr);
  *f = Outr.x.bx;
  *x = Outr.x.cx;
  *y = Outr.x.dx;
}
void SetMouseCoord(int x, int y)
{
  union REGS Inr, Outr;
  Inr.x.cx = x;
  Inr.x.dx = y;
  Inr.x.ax = 4;
  int86(0x33, &Inr, &Outr);
}

void SetMouseArea(int Xmin, int Ymin, int Xmax, int Ymax)
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
void MouseHideArea(int x1, int y1, int x2, int y2)
{
  union REGS Inr, Outr;
  Inr.x.cx = x1;
  Inr.x.dx = y1;
  Inr.x.si = x2;
  Inr.x.di = y2;
  Inr.x.ax = 0x10;
  int86(0x33, &Inr, &Outr);
}
int IsMouseInBox(int x1, int y1, int x2, int y2)
{
  int button, x, y;
  ReadMouse(&button, &x, &y);
  return ((x >= x1 && x <= x2 && y >= y1 && y <= y2) ? 1 : 0);
}

void updateMouseStatus(mousestatus *status)
{
  unsigned int xPos, yPos, bState;
  if (status == NULL)
    return;

  ReadMouse(&bState, &xPos, &yPos);
  // asm {
  //       mov ax,0x0003
  //       int 0x33
  //       mov bState,bx
  //       mov xPos,cx
  //       mov yPos,dx
  // }
  switch (bState)
  {
  case 0:
    status->oldLeftDown = status->leftDown;
    status->leftDown = 0;
    status->oldRightDown = status->rightDown;
    status->rightDown = 0;
    break;
  case 1:
    status->oldLeftDown = status->leftDown;
    status->leftDown = 1;
    break;
  case 2:
    status->oldRightDown = status->rightDown;
    status->rightDown = 1;
    break;
  case 3:
    status->oldLeftDown = status->leftDown;
    status->leftDown = 1;
    status->oldRightDown = status->rightDown;
    status->rightDown = 1;
    break;
  default:
    break;
  }

  //检测鼠标状态
  if ((status->leftDown == 1) && (status->oldLeftDown == 0))
    status->leftClickState = MOUSE_BUTTON_DOWN;
  if ((status->leftDown == 0) && (status->oldLeftDown == 1))
    status->leftClickState = MOUSE_BUTTON_UP;
  if ((status->leftDown == 1) && (status->oldLeftDown == 1))
    status->leftClickState = MOUSE_BUTTON_STILL_DOWN;
  if ((status->leftDown == 0) && (status->oldLeftDown == 0))
    status->leftClickState = MOUSE_BUTTON_STILL_UP;
  if ((status->rightDown == 1) && (status->oldRightDown == 0))
    status->leftClickState = MOUSE_BUTTON_DOWN;
  if ((status->rightDown == 0) && (status->oldRightDown == 1))
    status->leftClickState = MOUSE_BUTTON_UP;
  if ((status->rightDown == 1) && (status->oldRightDown == 1))
    status->rightClickState = MOUSE_BUTTON_STILL_DOWN;
  if ((status->rightDown == 0) && (status->oldRightDown == 0))
    status->rightClickState = MOUSE_BUTTON_STILL_UP;

  status->x = xPos;
  status->y = yPos;
}

void ReadCursor(unsigned char *buf)
{
  FILE *fpcur;
  int i;
  char line[17];

  if (buf == NULL)
    return;

  memset(buf, 0, 16 * 16);
  fpcur = fopen("c:\\hho\\data\\cursor", "r");
  if (fpcur == NULL)
    return;

  for (i = 0; i < 16; i++)
  {
    fgets(line, 18, fpcur); //\r\n
    memcpy(buf + i * 16, line, 16);
  }
  fclose(fpcur);

  return buf;
}

void DrawCursor(unsigned char *cur, int x, int y)
{
  int i = 0, j = 0;
  if (cur == NULL)
    return;

  for (i = 0; i < 16; i++)
    for (j = 0; j < 16; j++)
    {
      if (cur[j * 16 + i] == '0')
        continue;
      else if (cur[j * 16 + i] == '1') //黑色
        putpixel(x + i, y + j, RealColor(0x003F));
      else //白色
        putpixel(x + i, y + j, WhitePixel());
    }
}

void MouseSavebk(unsigned int *far buffer, int x, int y)
{
  savebackgroundEx(buffer, x, y, 16, 16);
}

void MousePutbk(unsigned int *far buffer, int x, int y)
{
  restorebackgroundEx(buffer, x, y, 16, 16);
}

// void Mouse()
// {
//   int Button, Oldx, Oldy;
//   int x, y, First = TRUE;
//   Button = Oldx = Oldy = 0;
//   if (!InitMouse())
//     return;
//   SetMouseArea(0, 0, 790, 599);
//   HideMouse();
//   while (Button != LEFT)
//   {
//     ReadMouse(&Button, &x, &y);
//     if (Oldx != x || Oldy != y)
//     {
//       if (!First)
//         MousePutbk(Oldx, Oldy);
//       MouseSavebk(x, y);
//       DrawMouse(x, y);
//       Oldx = x;
//       Oldy = y;
//       First = FALSE;
//     }
//     delay(30);
//   }
//   MousePutbk(x, y);
// }