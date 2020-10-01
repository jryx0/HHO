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
