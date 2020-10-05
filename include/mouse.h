#ifndef __MOUSE_H__
#define __MOUSE_H__

typedef struct Coordinate
{
	int x;
	int y;
} Coordinate;

// typedef struct Area
// {
//     Coordinate lt;
//     Coordinate rb;
// } Area;

/*鼠标结构体，包含鼠标状态*/
typedef struct mouse_
{
	int xpos, ypos;
	int but; /*按钮的状态，0,1位分别表示左右键，该位为1表示按下，0表示松开*/
} MOUSE;

/**********************************************************
Function：		 MouseInit

Description：	鼠标复位

Input：			None

Output：		鼠标复位

Return：		unsigned int	函数执行结果，
								0x0000	不支持
								0xffff	支持
**********************************************************/
extern int MouseInit(void);

/**********************************************************
Function：		 MouseRange

Description：	设置鼠标移动范围

Input：			Area结构体变量（记录了屏幕上某块区域的坐标范围）

Output：		None
Return：		None
**********************************************************/
// extern void MouseRange(Area mouse_area);

/**********************************************************
Function：		 MouseXYB

Description：	读取当前鼠标状态，存入指定结构体

Input：			MOUSE * mouse	存放鼠标状态结构体的地址

Output：		MOUSE * mouse	存放鼠标状态结构体被更改

Return：		mouse->but		鼠标按钮状态
**********************************************************/
extern int MouseXYB(MOUSE *mouse);

/**********************************************************
Function：		 MouseBarLeft

Description：	判断鼠标左键是否在指定区域内按下

Input：			Area结构体变量（记录了屏幕上某块区域的坐标范围）

Output：		None

Return：		1		鼠标左键在指定区域内按下
				0		鼠标左键没在指定区域内按下
**********************************************************/
// extern int MouseBarLeft(Area mouse_area);

/**********************************************************
Function：		 MouseStoreBk

Description：	储存被鼠标覆盖区域的显存内容（Bk是background的缩写）

Input：			Coordinate结构体变量（对应屏幕中某个点的坐标）	鼠标所在区域左上角的坐标

Output：		存储屏幕上被鼠标覆盖区域的显存内容

Return：		None
**********************************************************/
extern void MouseStoreBk(int xpos, int ypos);

/**********************************************************
Function：		 MousePutBk

Description：	恢复被鼠标覆盖区域的显存内容（Bk是background的缩写）

Input：			Coordinate结构体变量（对应屏幕中某个点的坐标）	鼠标所在区域左上角的坐标

Output：		在屏幕上恢复被鼠标覆盖区域的显存内容

Return：		None
**********************************************************/
extern void MousePutBk(int xpos, int ypos);

/**********************************************************
Function：		 MouseReset

Description：	重置鼠标状态，范围为全屏

Input：			None

Output：		重置鼠标状态

Return：		None
**********************************************************/
extern void MouseReset(void);

/**********************************************************
Function		MouseDraw

Description：	画鼠标光标函数

Input：			MOUSE mouse						存放鼠标状态的结构体
				int const mouse_shape[10][10]	鼠标形状全局变量数组

Output：		在指定坐标处画鼠标光标

Return：		None
**********************************************************/
extern void MouseDraw(MOUSE mouse);

#endif