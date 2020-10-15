#include "macrodef.h"
#include "mouse.h"
#include "hglobal.h"
#include "hhosvga.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/**
 * 初始化鼠标 
 */
int MouseInit(void)
{
  union REGS mouse;

  /*设置鼠标的功能号*/
  mouse.x.ax = 0;
  int86(0x33, &mouse, &mouse);
  return mouse.x.ax;
}

/**
 * 设置鼠标显示范围 
 * @param Xmin 
 * @param Ymin 
 * @param Xmax
 * @param Ymax  右下坐标
 */
void SetMouseRange(int Xmin, int Ymin, int Xmax, int Ymax)
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

void ResetMouse(mousestatus *mouse) //7
{

  if (MouseInit() == 0 || mouse == NULL)
  {
    printf("ERROR In MouseInit\n");
    exit(1);
  };
  SetMouseRange(0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1);
  SaveMouseBk(mouse);
}

/**
 * 更新鼠标状态
 * @param status 鼠标
 * 
 */
void UpdateMouseStatus(mousestatus *status)
{
  int xPos, yPos, bState;
  union REGS Inr, Outr;

  TESTNULLVOID(status);

  //RestoreMouseBk(status);

  Inr.x.ax = 3;
  int86(0x33, &Inr, &Outr);
  bState = Outr.x.bx;
  xPos = Outr.x.cx;
  yPos = Outr.x.dx;

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
    // status->oldLeftDown = status->leftDown;
    // status->leftDown = 1;
    // status->oldRightDown = status->rightDown;
    // status->rightDown = 1;
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
    status->rightClickState = MOUSE_BUTTON_DOWN;
  if ((status->rightDown == 0) && (status->oldRightDown == 1))
    status->rightClickState = MOUSE_BUTTON_UP;
  if ((status->rightDown == 1) && (status->oldRightDown == 1))
    status->rightClickState = MOUSE_BUTTON_STILL_DOWN;
  if ((status->rightDown == 0) && (status->oldRightDown == 0))
    status->rightClickState = MOUSE_BUTTON_STILL_UP;

  status->x = xPos;
  status->y = yPos;

  // SaveMouseBk(status);
  // MouseDraw(status);
}

void SaveMouseBk(mousestatus *mouse) //5
{
  if (mouse == NULL)
    return;

  savebackgroundEx((unsigned int *)mouse->cursorBK, mouse->x, mouse->y, MOUSE_WIDTH, MOUSE_HEIGHT);
}

void RestoreMouseBk(mousestatus *mouse) //6
{
  if (mouse == NULL)
    return;
  restorebackgroundEx((unsigned int *)mouse->cursorBK, mouse->x, mouse->y, MOUSE_WIDTH, MOUSE_HEIGHT);
}

void MouseDraw(mousestatus *mouse) //8
{
  int i, j;

  TESTNULLVOID(mouse);
  TESTNULLVOID(mouse->currentCur);
  for (j = 0; j < MOUSE_HEIGHT; j++)
    for (i = 0; i < MOUSE_WIDTH; i++)
      if (mouse->currentCur[j][i] == '0')
      {
        continue;
      }
      else if (mouse->currentCur[j][i] == '1')
      {
        putpixel64k(i + mouse->x, j + mouse->y, 0xffff);
      }
      else if (mouse->currentCur[j][i] == '2')
      {
        putpixel64k(i + mouse->x, j + mouse->y, 0x0);
      }
}

/**
 * 读取鼠标形状文件，存储在buf中
 * 
 * @param buf 保存缓存
 * @param width 鼠标宽度
 * @param height 鼠标高度
 * @param filename 鼠标文件 
 * 
 * @return 1 success 0 failure
 */
int ReadCursor(unsigned char *buf, char *filename)
{
  int i;
  FILE *fpcur;
  char line[MOUSE_WIDTH + 2];

  if (buf == NULL || filename == NULL)
  {
    fprintf(stderr, "ReadCursor Error!");
    return 0;
  }

  memset(buf, 0, MOUSE_WIDTH * MOUSE_HEIGHT);
  fpcur = fopen(filename, "r");
  if (fpcur == NULL)
  {
    fprintf(stderr, "ReadCursor Error! can't open file:%s", filename);
    return 0;
  }

  for (i = 0; i < MOUSE_HEIGHT; i++)
  {
    fgets(line, MOUSE_WIDTH + 2, fpcur); //\r\n
    memcpy(buf + i * MOUSE_WIDTH, line, MOUSE_WIDTH);
  }
  fclose(fpcur);

  return 1;
}