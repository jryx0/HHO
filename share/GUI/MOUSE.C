#include "SVGAUTIL.H"
#include "mouse.h"
#include <dos.h>
#include <graphics.h>
#include <stdio.h>

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

// void MouseSavebk(int x, int y)
// {
//   int i, j;
//   for (i = 0; i <= 15; i++)
//     for (j = 0; j <= 15; j++)
//       mouseback[i][j] = getpixel(x + i, y + j);
//   // GetColor64k(x + i, y + j, &mouseback[i][j]);
// }

// void MousePutbk(int x, int y)
// {
//   int i, j;
//   for (i = 0; i <= 15; i++)
//     for (j = 0; j <= 15; j++)
//       putpixel(x + i, y + j, mouseback[i][j]);
//   // PaintPoint(x + i, y + j, mouseback[i][j]);
// }

// void DrawMouse(int x, int y)
// {
//   setcolor(RealDrawColor(0xFFFFFF));
//   line(x, y, x, y + 15);
//   line(x + 1, y + 1, x + 1, y + 13);
//   line(x + 2, y + 3, x + 2, y + 11);
//   line(x + 3, y + 4, x + 3, y + 9);
//   line(x + 4, y + 5, x + 4, y + 9);
//   line(x + 5, y + 7, x + 5, y + 10);
//   line(x + 6, y + 8, x + 6, y + 10);
//   line(x + 7, y + 9, x + 7, y + 11);
//   putpixel(x + 8, y + 11, RealDrawColor(0xFFFFFF));
//   putpixel(x + 9, y + 12, RealDrawColor(0xFFFFFF));
// }

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