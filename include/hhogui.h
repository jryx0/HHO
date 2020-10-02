#ifndef __HHOSVGA_H__
#define __HHOSVGA_H__
#include "macrodef.h"
#include "SVGA.h"

void clearScreen(char color);
void fillRegion(int x1, int y1, int x2, int y2, int color);
void fillRegionEx(int x, int y, int width, int height, int color);
void setStandardRegion(int *x1, int *y1, int *x2, int *y2);

void lineh(int x, int y, int width, int color);
void linev(int x, int y, int height, int color);
void linex(int x1, int y1, int x2, int y2, int color);
void liney(int x1, int y1, int x2, int y2, int color);
void line(int x1, int y1, int x2, int y2, int color);
 
#endif