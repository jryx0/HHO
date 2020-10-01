/************************************************/
/* 						*/
/*          SuperVGA utility routines		*/
/*		Copyright (c) 1991		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/
#include "macrodef.h"
#include "HBaseWin.h"
#include "svgautil.h"

#include <dos.h>
#include <graphics.h>
#include <memory.h>
#include <stdio.h>

/************************************************************************/
/*									*/
/* 		    Enhanced color setting functions			*/
/*									*/
/* These functions are used to set the colors for the 32k/64k/TrueColor */
/* modes, as the BGI kernel can only handle 8-bit color values.		*/
/*									*/
/************************************************************************/
/********************************************************/
/* long RGB(char rVal, char gVal, char bVal);   	*/
/*					   		*/
/* Purpose: Returns the color value for a R,G,B triple  */
/*	based on the current graphics mode.		*/
/*							*/
/* Input:						*/
/*	char rVal - Red value   [0..255]		*/
/*	char gVal - Green value [0..255]		*/
/*	char bVal - Blue value  [0..255]		*/
/*							*/
/* Returns:						*/
/* 	long - Color value for this mode.		*/
/*							*/
/********************************************************/
long RGB(char rVal, char gVal, char bVal)
{
  __rColor xColor;

  switch (getmaxcolor())
  {
  case 32767:
    xColor.c15.rVal = (rVal >> 3) & 0x1F;
    xColor.c15.gVal = (gVal >> 3) & 0x1F;
    xColor.c15.bVal = (bVal >> 3) & 0x1F;
    break;
  case 65535:
    xColor.c16.rVal = (rVal >> 3) & 0x1F;
    xColor.c16.gVal = (gVal >> 2) & 0x1F;
    xColor.c16.bVal = (bVal >> 3) & 0x1F;
    break;
  case 16777:
    xColor.c24.rVal = rVal;
    xColor.c24.gVal = gVal;
    xColor.c24.bVal = bVal;
    break;
  }
  return (xColor.cval);
}

/****************************************************************/
/* long RealDrawColor(long color);			  	*/
/* 							  	*/
/* Purpose: Sets the current drawing color for HC/TC modes.	*/
/*	Used for 'setcolor'					*/
/*								*/
/* Input:							*/
/*	long color - Color value				*/
/*								*/
/* Returns:							*/
/*	long - Color value					*/
/*								*/
/****************************************************************/
long RealDrawColor(long color)
{
  __rColor xColor;

  xColor.cval = color;
  /* Do color set hacks for hicolor/truecolor modes */
  switch (getmaxcolor())
  {
  case 32767:
    setrgbpalette(1024, xColor.c15.rVal, xColor.c15.gVal, xColor.c15.bVal);
    break;
  case 65535:
    setrgbpalette(1024, xColor.c16.rVal, xColor.c16.gVal, xColor.c16.bVal);
    break;
  case 16777:
    setrgbpalette(1024, xColor.c24.rVal, xColor.c24.gVal, xColor.c24.bVal);
    break;
  };
  return color;
}

/****************************************************************/
/* long RealFillColor(long color);			  	*/
/* 							  	*/
/* Purpose: Sets the current fill color for HC/TC modes.	*/
/*	Used for 'setfillstyle' and 'setfillpattern'		*/
/*								*/
/* Input:							*/
/*	long color - Color value				*/
/*								*/
/* Returns:							*/
/*	long - Color value					*/
/*								*/
/****************************************************************/
long RealFillColor(long color)
{
  __rColor xColor;

  xColor.cval = color;
  /* Do color set hacks for hicolor/truecolor modes */
  switch (getmaxcolor())
  {
  case 32767:
    setrgbpalette(1025, xColor.c15.rVal, xColor.c15.gVal, xColor.c15.bVal);
    break;
  case 65535:
    setrgbpalette(1025, xColor.c16.rVal, xColor.c16.gVal, xColor.c16.bVal);
    break;
  case 16777:
    setrgbpalette(1025, xColor.c24.rVal, xColor.c24.gVal, xColor.c24.bVal);
    break;
  };
  return color;
}

/****************************************************************/
/* long RealColor(long color);			  		*/
/* 							  	*/
/* Purpose: Sets the current color for HC/TC modes.		*/
/*	Used for 'putpixel' and 'floodfill'			*/
/*								*/
/* Input:							*/
/*	long color - Color value				*/
/*								*/
/* Returns:							*/
/*	long - Color value					*/
/*								*/
/****************************************************************/
long RealColor(long color)
{
  __rColor xColor;

  xColor.cval = color;
  /* Do color set hacks for hicolor/truecolor modes */
  switch (getmaxcolor())
  {
  case 32767:
    setrgbpalette(1026, xColor.c15.rVal, xColor.c15.gVal, xColor.c15.bVal);
    break;
  case 65535:
    setrgbpalette(1026, xColor.c16.rVal, xColor.c16.gVal, xColor.c16.bVal);
    break;
  case 16777:
    setrgbpalette(1026, xColor.c24.rVal, xColor.c24.gVal, xColor.c24.bVal);
    break;
  };
  return color;
}

/* Getvgapalette256 gets the entire 256 color palette */
/* PalBuf contains RGB values for all 256 colors      */
/* R,G,B values range from 0 to 63	              */
/* Usage:					      */
/*  DacPalette256 dac256;			      */
/*						      */
/* getvgapalette256(&dac256);			      */
void getvgapalette256(DacPalette256 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1017;
  reg.r_bx = 0;
  reg.r_cx = 256;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10, &reg);
}

/* Setvgapalette256 sets the entire 256 color palette */
/* PalBuf contains RGB values for all 256 colors      */
/* R,G,B values range from 0 to 63	              */
/* Usage:					      */
/*  DacPalette256 dac256;			      */
/*						      */
/* setvgapalette256(&dac256);			      */
void setvgapalette256(DacPalette256 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1012;
  reg.r_bx = 0;
  reg.r_cx = 256;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10, &reg);
}

/* Returns the color for white */
long WhitePixel(void)
{
  switch (getmaxcolor())
  {
  case 32768:
    return 0x7fffL;
  case 65535:
    return 0xffffL;
  default:
    return 15;
  };
}

int huge DetectVGA256()
{
  int Vid = 4;

  // printf("Which video mode would you like to use? \n");
  // printf("  2) 640x480x256\n");
  // printf("  4) 1024x768x256\n");
  // printf("\n> ");
  // scanf("%d", &Vid);

  return Vid;
}

int huge DetectVGA64k()
{
  int Vid = 4;
  // printf("Which video mode would you like to use? \n");
  // printf("  3) 640x480x65536\n");
  // printf("  4) 800x600x65536\n");
  // printf("  5) 1024x768x65536\n");
  // printf("\n> ");
  // scanf("%d", &Vid);
  return Vid;
}

int huge DetectVGA64k1024()
{
  int Vid = 5;
  // printf("Which video mode would you like to use? \n");
  // printf("  3) 640x480x65536\n");
  // printf("  4) 800x600x65536\n");
  // printf("  5) 1024x768x65536\n");
  // printf("\n> ");
  // scanf("%d", &Vid);
  return Vid;
}

int huge DetectVGA32k()
{
  int Vid = 4;

  // printf("Which video mode would you like to use? \n");
  // printf("  3) 640x480x32768\n");
  // printf("  4) 800x600x32768\n");
  // printf("  5) 1024x768x32768\n");
  // printf("\n> ");
  // scanf("%d", &Vid);

  return Vid;
}

int huge DetectVGA32k1024()
{
  int Vid = 5;

  // printf("Which video mode would you like to use? \n");
  // printf("  3) 640x480x32768\n");
  // printf("  4) 800x600x32768\n");
  // printf("  5) 1024x768x32768\n");
  // printf("\n> ");
  // scanf("%d", &Vid);

  return Vid;
}

unsigned int RGB888ToRGB565(long aPixel)
{
  int red = (aPixel >> 16) & 0xFF;
  int green = (aPixel >> 8) & 0xFF;
  int blue = aPixel & 0xFF;

  unsigned short B = (blue >> 3) & 0x001F;
  unsigned short G = ((green >> 2) << 5) & 0x07E0;
  unsigned short R = ((red >> 3) << 11) & 0xF800;

  return (unsigned int)(R | G | B);
}

/*
@mode = 1 SVGA256
          = 2 SVGA32k
          = 3 SVGA64k
*/
int initSvga(int mode)
{
  int Gd = DETECT, Gm;
  char GrErr;

  if (mode == -1)
  {
#if defined(SVGA64K)
#if defined(SVGA800x600)
    mode = 3;
#elif defined(SVGA1024x768)
    mode = 5;
#endif
#elif defined(SVGA32K)
#if defined(SVGA800x600)
    mode = 2;
#elif defined(SVGA1024x768)
    mode = 4;
#endif
#endif
  }

  switch (mode)
  {
  case 1: // 1024*786 256
    installuserdriver("Svga256", DetectVGA256);
    break;
  case 2: // 800*600 32k
    installuserdriver("Svga32k", DetectVGA32k);
    break;
  case 3: // 800*600 64k
    installuserdriver("Svga64k", DetectVGA64k);
    break;
  case 4: // 1024*768 32k
    installuserdriver("Svga32k", DetectVGA32k1024);
    break;
  case 5: // 1024*768 64k
    installuserdriver("Svga64k", DetectVGA64k1024);
    break;
  default:
    installuserdriver("Svga256", DetectVGA256);
    break;
  }

  initgraph(&Gd, &Gm, "");

  /* Test if mode was initialized successfully */
  GrErr = graphresult();
  if (GrErr != grOk)
  {
    printf("%s-%dGraphics error: %s\n", __FILE__, __LINE__,
           grapherrormsg(GrErr));
    exit(1);
  }

  return mode;
}
void SelectPage(register char page)
{
  union REGS r;
  static unsigned char old_page = 0; // 上一次的页面号，用于减少切换次数
  static int is_first_select = 1;    //	用于判断是否是第一次换页

  /* 窗口页面控制功能号 */
  r.x.ax = 0x4f05;
  r.x.bx = 0;

  /* 如果是第一次换页 */
  if (is_first_select == 1)
  {
    old_page = page;
    r.x.dx = page;
    int86(0x10, &r, &r);
    is_first_select = 0;
  }
  /* 如果和上次页面号不同，则进行换页 */
  else if (page != old_page)
  {
    old_page = page;
    r.x.dx = page;
    int86(0x10, &r, &r);
  }
}

unsigned int GetPixel(int x, int y)
{
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改
  unsigned char new_page;                                                 // 要切换的页面号
  unsigned long int page;                                                 // 对应显存地址偏移量

  /* 判断点是否在屏幕范围内，不在就退出 */
  if (x < 0 || x > (getmaxx() - 1) || y < 0 || y > (getmaxy() - 1))
  {
    return;
  }

  /* 计算显存地址偏移量和对应的页面号，做换页操作 */
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /* 32k个点一换页，除以32k的替代算法 */
  SelectPage(new_page);

  /* 返回颜色 */
  return video_buffer[page];
}

void PutPixel(int x, int y, unsigned int color)
{
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改
  unsigned char new_page;                                                 // 要切换的页面号
  unsigned long int page;                                                 // 对应显存地址偏移量

  /* 判断点是否在屏幕范围内，不在就退出 */
  if (x < 0 || x > (getmaxx() - 1) || y < 0 || y > (getmaxy() - 1))
  {
    return;
  }

  /* 计算显存地址偏移量和对应的页面号，做换页操作 */
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /* 32k个点一换页，除以32k的替代算法 */
  SelectPage(new_page);

  /* 向显存写入颜色，对应点画出 */
  video_buffer[page] = color;
}

void clearScreen(int color)
{
  long i, j = 16;
  unsigned int pages = 0, pagesize = 0;

  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

#if defined(SVGA64K) || defined(SVGA32K)
#ifdef SVGA1024x768
  pages = 24;
#elif defined(SVGA800x600)
  pages = 14;
#endif
  pagesize = 1024 * 32;
  for (i = 0; i <= pages; i++)
  {
    SelectPage(i);
    memset(video_buffer + (j * (i * 2) << 10), color, 1024 * 32);
    memset(video_buffer + (j * (i * 2 + 1) << 10), color, 1024 * 32);
  }
#else
  setfillstyle(SOLID_FILL, RealFillColor(color));
  bar(0, 0, getmaxx(), getmaxy());
#endif

  /* 
  SelectPage(1);
  memset(video_buffer + (j * 2 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 3 << 10), color, 1024 * 32);

  SelectPage(2);
  memset(video_buffer + (j * 4 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 5 << 10), color, 1024 * 32);

  SelectPage(3);
  memset(video_buffer + (j * 6 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 7 << 10), color, 1024 * 32);

  SelectPage(4);
  memset(video_buffer + (j * 8 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 9 << 10), color, 1024 * 32);

  SelectPage(5);
  memset(video_buffer + (j * 10 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 11 << 10), color, 1024 * 32);

  SelectPage(6);
  memset(video_buffer + (j * 12 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 13 << 10), color, 1024 * 32);

  SelectPage(7);
  memset(video_buffer + (j * 14 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 15 << 10), color, 1024 * 32);

  SelectPage(8);
  memset(video_buffer + (j * 16 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 17 << 10), color, 1024 * 32);

  SelectPage(9);
  memset(video_buffer + (j * 18 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 19 << 10), color, 1024 * 32);

  SelectPage(10);
  memset(video_buffer + (j * 20 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 21 << 10), color, 1024 * 32);

  SelectPage(11);
  memset(video_buffer + (j * 22 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 23 << 10), color, 1024 * 32);

  SelectPage(12);
  memset(video_buffer + (j * 24 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 25 << 10), color, 1024 * 32);

  SelectPage(13);
  memset(video_buffer + (j * 26 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 27 << 10), color, 1024 * 32);

  SelectPage(14);
  memset(video_buffer + (j * 28 << 10), color, 1024 * 32);
  memset(video_buffer + (j * 29 << 10), color, 1024 * 32); 
*/
}
