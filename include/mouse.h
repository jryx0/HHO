#ifndef __MOUSE_H__
#define __MOUSE_H__

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

/**
 * 初始化鼠标
 * @return 是否成功
 */
int InitMouse(void);

void ShowMouse(void);
void HideMouse(void);
void ReadMouse(int *f, int *x, int *y);
void SetMouseCoord(int x, int y);
void SetMouseArea(int Xmin, int Ymin, int Xmax, int Ymax);
void MouseHideArea(int x1, int y1, int x2, int y2);

/**
 * 更新鼠标状态
 * @param status 鼠标
 */
void updateMouseStatus(mousestatus *status);

/**
 * 读取鼠标文件，存储在buf中
 * 
 * @param buf 保存缓存
 * @param width 鼠标宽度
 * @param height 鼠标高度
 * @param filename 鼠标文件 
 */
int ReadCursor(unsigned char *buf, int width, int height, char *filename);

/**
 * 根据cur中存储的鼠标形状,绘制鼠标。 cur[i][j] = 0 不绘制 = 1 黑色 =其他 白色
 * @param cur 鼠标形状, 必须申请 width * height 大小的缓存
 * @param x
 * @param y 鼠标左上坐标
 * @param width 鼠标宽度
 * @param height 鼠标高度
 */
void DrawCursor(unsigned char *cur, int x, int y, int width, int height);

/**
 * 保存鼠标覆盖区域
 * @param buffer 背景保存缓存
 * @param x
 * @param y 左上坐标
 * @param width 鼠标宽度
 * @Param height 鼠标高度
 */
void MouseSavebk(unsigned int *far buffer, int x, int y, int width, int height);

/**
 * 恢复鼠标覆盖区域
 * @param buffer 背景保存缓存
 * @param x
 * @param y 左上坐标
 * @param width 鼠标宽度
 * @Param height 鼠标高度
 */
void MousePutbk(unsigned int *far buffer, int x, int y, int width, int height);

#endif