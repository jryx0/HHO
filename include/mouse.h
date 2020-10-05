#ifndef __MOUSE_H__
#define __MOUSE_H__

typedef struct
{
	unsigned int x, y;
	unsigned char leftDown, rightDown;
	unsigned char oldLeftDown, oldRightDown;
	char leftClickState, rightClickState;
	char moveState;
} mousestatus;

/**
 * 鼠标初始化
 * 
 * @return  0x0000	不支持鼠标  	0xffff	支持鼠标
 */
int initMouse(void);

/**
 * 
 * 设置鼠标移动范围
 * @param  Xmin 
 * @param  Ymin 左上坐标
 * @param  Xmax 
 * @param  Ymax 右下坐标
 * 
 **/
void setMouseRange(int Xmin, int Ymin, int Xmax, int Ymax);

/**
 * 读取当前鼠标状态，存入指定结构体
 * 
 * @param status 返回鼠标按钮状态
 * @param x 返回鼠标x坐标
 * @param y 返回鼠标y坐标
 */
void getMouseStatus(int *status, int *x, int *y);

/**
 * 储存被鼠标覆盖区域的显存内容
 * 
 * @param curBK 鼠标背景
 * @param x  鼠标x坐标
 * @param y  鼠标y坐标
 * @param width 鼠标宽度
 * @param height 鼠标宽度
 **/
void saveMouseBK(unsigned int *curBK, int x, int y, int width, int height);

/**
 * 恢复被鼠标覆盖区域的显存内容
 * 
 * @param curBK 鼠标背景
 * @param x  恢复位置的x坐标
 * @param y  恢复位置的y坐标
 * 
 */
void restoreMouseBk(unsigned int *curBK, int x, int y, int width, int height);

/**
 * 
 * 重置鼠标状态，范围为全屏
 */
void resetMouset(unsigned int *curBK);

/**
 * 	画鼠标光标
 * @param x  显示位置的x坐标
 * @param y  显示位置的y坐标
 * 
 * 
 */
void drawMousecursor(unsigned char *cur, int x, int y);

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
int readCursor(unsigned char *buf, int width, int height, char *filename);

/**
 * 更新鼠标状态
 * @param status 鼠标 * 
 */
void updateMouseStatus(mousestatus *status);
#endif