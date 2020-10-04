/************************************************/
/* 						*/
/*     BGI common defines/utility functions	*/
/*		Copyright (c) 1994		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/
#ifndef __SVGAUTIL_H__
#define __SVGAUTIL_H__
#include "hglobal.h"

typedef unsigned char DacPalette256[256][3];

#define XNOR_PUT 5
#define NOR_PUT 6
#define NAND_PUT 7
#define TRANS_COPY_PUT 8 /* Doesn't work with 16-color drivers */

#pragma pack(1)
typedef struct __bpp24
{
  unsigned char bVal;
  unsigned char gVal;
  unsigned char rVal;
} __bpp24;

typedef struct __bpp16
{
  unsigned int bVal : 5;
  unsigned int gVal : 6;
  unsigned int rVal : 5;
} __bpp16;

typedef struct __bpp15
{
  unsigned int bVal : 5;
  unsigned int gVal : 5;
  unsigned int rVal : 5;
} __bpp15;

typedef union
{
  struct __bpp24 c24;
  struct __bpp16 c16;
  struct __bpp15 c15;
  unsigned long cval;
} __rColor;
#pragma pack()

#ifdef __cplusplus
extern "C"
{
#endif

  /* Define enhanced color setting functions */
  long RGB(char, char, char);
  long RealDrawColor(long);
  long RealFillColor(long);
  long RealColor(long);

  /* Define palette functions */
  void getvgapalette256(DacPalette256 *);

  long WhitePixel(void);
  // 800*600
  int huge DetectVGA32k();
  int huge DetectVGA64k();

  // 1024*768
  int huge DetectVGA32k1024();
  int huge DetectVGA64k1024();
  int huge DetectVGA256();

  int initSvga(int mode);

  // /* These are the Svga256 supported modes */
  // // extern int far _Cdecl Svga256_fdriver[];
  // #define SVGA320x200x256 0   /* 320x200x256 Standard VGA */
  // #define SVGA640x400x256 1   /* 640x400x256 Svga/VESA */
  // #define SVGA640x480x256 2   /* 640x480x256 Svga/VESA */
  // #define SVGA800x600x256 3   /* 800x600x256 Svga/VESA */
  // #define SVGA1024x768x256 4  /* 1024x768x256 Svga/VESA */
  // #define SVGA640x350x256 5   /* 640x350x256 Svga/VESA */
  // #define SVGA1280x1024x256 6 /* 1280x1024x256 Svga/VESA */

  // /* These are the Svga64k supported modes */
  // // extern int far _Cdecl Svga64k_fdriver[];
  // #define SVGA320x200x65536 0   /* 320x200x65536 HiColor VGA */
  // #define SVGA640x350x65536 1   /* 640x350x65536 HiColor VGA */
  // #define SVGA640x400x65536 2   /* 640x400x65536 HiColor VGA */
  // #define SVGA640x480x65536 3   /* 640x480x65536 HiColor VGA */
  // #define SVGA800x600x65536 4   /* 800x600x65536 HiColor VGA */
  // #define SVGA1024x768x65536 5  /* 1024x768x65536 HiColor VGA */
  // #define SVGA1280x1024x65536 6 /* 1280x1024x65536 HiColor VGA */

  // /* These are the Svga32k supported modes */
  // // extern int far _Cdecl Svga32k_fdriver[];
  // #define SVGA320x200x32768 0   /* 320x200x32768 HiColor VGA */
  // #define SVGA640x350x32768 1   /* 640x350x32768 HiColor VGA */
  // #define SVGA640x400x32768 2   /* 640x400x32768 HiColor VGA */
  // #define SVGA640x480x32768 3   /* 640x480x32768 HiColor VGA */
  // #define SVGA800x600x32768 4   /* 800x600x32768 HiColor VGA */
  // #define SVGA1024x768x32768 5  /* 1024x768x32768 HiColor VGA */
  // #define SVGA1280x1024x32768 6 /* 1280x1024x32768 HiColor VGA */

#ifdef __cplusplus
};
#endif

#endif /* __SVGAUTIL_H__ */
