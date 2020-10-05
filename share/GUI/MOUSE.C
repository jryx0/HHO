#include "hglobal.h"
#include "hhosvga.h"
#include "svga.h"
#include "mouse.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// #define MOUSEWIDTH 20
// #define MOUSEHIGHT 30
// int const mouse_shape[MOUSEHIGHT][MOUSEWIDTH] =
//     {
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 2, 1, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 2, 1, 0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0},
//         {1, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0},
//         {1, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0},
//         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0},
//         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};
// int mouse_bk[MOUSEHIGHT][MOUSEWIDTH];

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

void getMouseStatus(int *status, int *x, int *y)
{

  union REGS Inr, Outr;
  Inr.x.ax = 3;
  int86(0x33, &Inr, &Outr);
  *status = Outr.x.bx;
  *x = Outr.x.cx;
  *y = Outr.x.dx;
}
void saveMouseBK(unsigned int *curBK, int x, int y, int width, int height)
{
  savebackgroundEx(curBK, x, y, width, height);
}

void restoreMouseBk(unsigned int *curBK, int x, int y, int width, int height) //6
{
  restorebackgroundEx(curBK, x, y, width, height);
}

void resetMouse(unsigned int *curBK) //7
{
  //Area mouse_area = {{0, 0}, {SCR_WIDTH - 1, SCR_HEIGHT - 1}};
  if (initMouse() == 0)
  {
    printf("ERROR In MouseInit\n");
    return;
  }

  setMouseRange(0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1);
  saveMouseBK(curBK, 0, 0, MOUSE_WIDTH, MOUSE_HEIGHT);
}

void drawMousecursor(unsigned char *cur, int x, int y) //8
{
  int i, j;
  for (i = 0; i < MOUSE_HEIGHT; i++)
  {
    for (j = 0; j < MOUSE_WIDTH; j++)
    {
      if (*(cur + i + j * MOUSE_WIDTH) == 0)
      {
        continue;
      }
      else if (*(cur + i + j * MOUSE_WIDTH) == 1)
      {
        putpixel64k(j + x, i + y, 0);
      }
      else if (*(cur + i + j * MOUSE_WIDTH) == 2)
      {
        putpixel64k(j + x, i + y, 0xffff);
      }
    }
  }
}

/**
 * 更新鼠标状态
 * @param status 鼠标 * 
 */
void updateMouseStatus(mousestatus *status)
{
  int xPos, yPos, bState;
  if (status == NULL)
    return;

  getMouseStatus(&bState, &xPos, &yPos);

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

/**
 * 读取鼠标文件，存储在buf中
 * 
 * @param buf 保存缓存
 * @param width 鼠标宽度
 * @param height 鼠标高度
 * @param filename 鼠标文件 
 * 
 * @return 1 success 0 failure
 */
int readCursor(unsigned char *buf, int width, int height, char *filename)
{
  int i;
  FILE *fpcur;
  char line[17];

  if (buf == NULL || filename == NULL)
  {
    fprintf(stderr, "ReadCursor Error!");
    return 0;
  }

  memset(buf, 0, width * height);
  fpcur = fopen(filename, "r");
  if (fpcur == NULL)
  {
    fprintf(stderr, "ReadCursor Error! can't open file:%s", filename);
    return 0;
  }

  for (i = 0; i < height; i++)
  {
    fgets(line, width + 2, fpcur); //\r\n
    memcpy(buf + i * width, line, width);
  }
  fclose(fpcur);

  return 1;
}
