#include "macrodef.h"
#include"mouse.h"

#include <dos.h>

/******************函数清单***************************
1.	int MouseInit(void)
2.	void MouseRange(Area mouse_area)
3.	int MouseXYB(MOUSE * mouse)
4.	int MouseBarLeft(Area mouse_area )
5.	void MouseStoreBk( Coordinate position)
6.	void MousePutBk(Coordinate position )
7.	void MouseReset(void)
8.	void MouseDraw(MOUSE mouse)
***************************************************/
#define MOUSEWIDTH 20
#define MOUSEHIGHT 30
int const mouse_shape[MOUSEHIGHT][MOUSEWIDTH] =
{
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
	{1,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1},
	{1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0},
	{1,2,2,2,2,1,2,2,2,2,2,1,0,0,0,0,0,0,0,0},
	{1,2,2,2,1,0,1,2,2,2,2,2,1,0,0,0,0,0,0,0},
	{1,2,2,1,0,0,0,1,2,2,2,2,1,0,0,0,0,0,0,0},
	{1,2,1,0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,1,2,2,2,2,1,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,2,2,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0}
};
int mouse_bk[MOUSEHIGHT][MOUSEWIDTH];

int MouseInit(void)//1
{
	union REGS mouse;

	/*设置鼠标的功能号*/
	mouse.x.ax = 0;
	int86(0x33, &mouse, &mouse);
	return mouse.x.ax;
}

void MouseRange(Area mouse_area)//2
{
	/*REGS联合体见上*/
	union REGS mouse;

	/*设置横坐标范围*/
	mouse.x.ax = 7;
	mouse.x.cx = mouse_area.lt.x ;
	mouse.x.dx = mouse_area.rb.x ;
	int86(0x33, &mouse, &mouse);

	/*设置纵坐标范围*/
	mouse.x.ax = 8;
	mouse.x.cx = mouse_area.lt.y;
	mouse.x.dx = mouse_area.rb.y;
	int86(0x33, &mouse, &mouse);
}

int MouseXYB(MOUSE * mouse)//3
{
	/*REGS联合体见上*/
	union REGS r;
	r.x.ax = 3;
	int86(0x33, &r, &r);
	mouse->position.x = r.x.cx;
	mouse->position.y = r.x.dx;
	mouse->but = r.x.bx;
	return mouse->but;
}

int MouseBarLeft(Area mouse_area )//4
{
	/*存放鼠标状态的结构体*/
	MOUSE mouse = {0, 0, 0};

	/*读取鼠标状态*/
	MouseXYB(&mouse);

	/*判断鼠标左键是否在指定区域内按下*/
	if ((mouse.position.x >= mouse_area.lt.x  )
		&& (mouse.position.x <= mouse_area.rb.x  )
		&& (mouse.position.y >= mouse_area.lt.y  )
		&& (mouse.position.y <= mouse_area.rb.y )
		&& ((mouse.but & 1) == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void MouseStoreBk( Coordinate position)//5
{
	int i, j;
	Coordinate temp;
	for (i = 0; i < MOUSEHIGHT; i++)
	{
		for (j = 0; j < MOUSEWIDTH; j++)
		{
			if (mouse_shape[i][j] == 0)
			{
				continue;
			}

			temp.x =position.x +j;
			temp.y = position.y + i;

			mouse_bk[i][j] = getpixel64k(temp);
		}
	}
}

void MousePutBk(Coordinate position )//6
{
	int i, j;
	for (i = 0; i < MOUSEHIGHT; i++)
	{
		for (j = 0; j < MOUSEWIDTH; j++)
		{
			if (mouse_shape[i][j] == 0)
			{
				continue;
			}

			putpixel64k(j + position.x, i + position.y, mouse_bk[i][j]);
		}
	}
}

void MouseReset(void)//7
{
	Area mouse_area = {{ 0, 0 },{SCR_WIDTH - 1, SCR_HEIGHT -1 }};
	if (MouseInit() == 0)
	{
		printf("ERROR In MouseInit\n");
		getch();
		exit(1);
	};
	MouseRange(mouse_area);
	MouseStoreBk(mouse_area.lt);
}

void MouseDraw(MOUSE mouse)//8
{
	int i, j;
	for (i = 0; i < MOUSEHIGHT; i++)
	{
		for (j = 0; j < MOUSEWIDTH; j++)
		{
			if (mouse_shape[i][j] == 0)
			{
				continue;
			}
			else if (mouse_shape[i][j] == 1)
			{
				putpixel64k(j + mouse.position.x, i + mouse.position.y, 0);
			}
			else if (mouse_shape[i][j] == 2)
			{
				putpixel64k(j + mouse.position.x, i + mouse.position.y, 0xffff);
			}
		}
	}
}

