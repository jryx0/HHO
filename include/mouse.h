#ifndef __MOUSE_H__
#define __MOUSE_H__

// typedef struct Coordinate
// {
// 	int x;
// 	int y;
// } Coordinate;

// typedef struct Area
// {
//     Coordinate lt;
//     Coordinate rb;
// } Area;

/*鼠标结构体，包含鼠标状态*/
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
 */
int MouseInit(void);

/**
 * 设置鼠标显示范围 
 * @param Xmin 
 * @param Ymin  左上坐标
 * @param Xmax
 * @param Ymax  右下坐标
 */
void SetMouseRange(int Xmin, int Ymin, int Xmax, int Ymax);

/**
 * 更新鼠标状态
 * @param status 鼠标
 * 
 * @return status 中坐标、按键状态
 */
void UpdateMouseStatus(mousestatus *status);

/**
 *储存被鼠标覆盖区域的显存内容
 **/
void SaveMouseBk(mousestatus *mouse);

/**
 * 恢复被鼠标覆盖区域的显存内容
 */
void RestorMouseBk(mousestatus *mouse);

/**
 * 重置鼠标状态，范围为全屏
 * 
 **/
void ResetMouse(mousestatus *mouse);

/**
 * 画鼠标光标函数 
 **/
void MouseDraw(mousestatus mouse);

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
int ReadCursor(unsigned char *buf, int width, int height, char *filename);
#endif