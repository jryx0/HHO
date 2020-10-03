#ifndef SVGAMODE_INC_MOUSE_H
#define SVGAMODE_INC_MOUSE_H

#define FALSE 0
#define TRUE 1
#define LEFT 1
#define MIDDLE 2
#define RIGHT 4

#define MOUSE_ENTERED 1
#define MOUSE_LEFT 2
#define MOUSE_OUTSIDE 4
#define MOUSE_INSIDE 8
#define MOUSE_BUTTON_DOWN 16
#define MOUSE_BUTTON_UP 32
#define MOUSE_BUTTON_STILL_DOWN 64
#define MOUSE_BUTTON_STILL_UP 128

typedef struct
{
  unsigned int x, y;
  unsigned char leftDown, rightDown;
  unsigned char oldLeftDown, oldRightDown;
  char leftClickState, rightClickState;
  char moveState;
} mousestatus;

int InitMouse(void);
void ShowMouse(void);
void HideMouse(void);
void ReadMouse(int *f, int *x, int *y);
void SetMouseCoord(int x, int y);
void SetMouseArea(int Xmin, int Ymin, int Xmax, int Ymax);
void MouseHideArea(int x1, int y1, int x2, int y2);
int IsMouseInBox(int x1, int y1, int x2, int y2);
void MouseSavebk(unsigned int *far buffer, int x, int y);
void MousePutbk(unsigned int *far buffer, int x, int y);
// void DrawMouse(int x, int y);
void DrawCursor(unsigned char *cur, int x, int y);
void Mouse();
void updateMouseStatus(mousestatus *status);

#endif