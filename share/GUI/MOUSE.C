#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "SVGAUTIL.H"

#include <dos.h>
#include <stdio.h>
#include <mem.h>

/**
 * 初始化鼠标
 * @return 是否成功
 */
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

/**
 * 更新鼠标状态
 * @param status 鼠标
 * 
 */
void updateMouseStatus(mousestatus *status)
{
  unsigned int xPos, yPos, bState;
  if (status == NULL)
    return;

  ReadMouse(&bState, &xPos, &yPos);

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
int ReadCursor(unsigned char *buf, int width, int height, char *filename)
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

/**
 * 根据cur中存储的鼠标形状,绘制鼠标。 cur[i][j] = 0 不绘制 = 1 黑色 =其他 白色
 * @param cur 鼠标形状, 必须申请 width * height 大小的缓存
 * @param x
 * @param y 鼠标左上坐标
 * @param width 鼠标宽度
 * @param height 鼠标高度
 */
void DrawCursor(unsigned char *cur, int x, int y, int width, int height)
{
  int i = 0, j = 0;
  if (cur == NULL)
  {
    fprintf(stderr, "DrawCursor Error! cur is null!");
    return;
  }

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
    {
      if (cur[j * width + i] == '0') //空白区域
        continue;
      else if (cur[j * width + i] == '2') //黑色
        putpixel64k(x + i, y + j, 0x00);
      else //白色
        putpixel64k(x + i, y + j, 0xffff);
    }
}

/**
 * 保存鼠标覆盖区域
 * @param buffer 背景保存缓存
 * @param x
 * @param y 左上坐标
 * @param width 鼠标宽度
 * @Param height 鼠标高度
 */
void MouseSavebk(unsigned int *far buffer, int x, int y, int width, int height)
{
  savebackgroundEx(buffer, x, y, width, height);
}

/**
 * 恢复鼠标覆盖区域
 * @param buffer 背景保存缓存
 * @param x
 * @param y 左上坐标
 * @param width 鼠标宽度
 * @Param height 鼠标高度
 */
void MousePutbk(unsigned int *far buffer, int x, int y, int width, int height)
{
  restorebackgroundEx(buffer, x, y, width, height);
}
