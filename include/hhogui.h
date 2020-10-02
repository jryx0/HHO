#ifndef __HHOSVGA_H__
#define __HHOSVGA_H__
#include "macrodef.h"

// void line(int x1, int y1, int x2, int y2, int color);
// void bar(int x1, int y1, int x2, int y2, long bkcolor);
// void rectangle(int x1, int y1, int x2, int y2, long color);

void clearScreen(int color);
void fillRegion(int x1, int y1, int x2, int y2, int color);
void fillRegionEx(int x, int y, int width, int height, int color);
void setStandardRegion(int *x1, int *y1, int *x2, int *y2);
#endif