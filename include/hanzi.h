#ifndef SVGAMODE_INCLUDE_HANZI_H
#define SVGAMODE_INCLUDE_HANZI_H
#include <stdio.h>
void printHZ12(int x, int y, char *s, int color, int dx, int dy, int gap,
               int mode);
void printHZ16(int x, int y, char *s, long color, int dx, int dy);

char *drawHZWord(FILE *fp, int x, int y, char hanzi[2], int color, int Scalex,
                 int Scaley, int fontsize);
char *drawHZWord16(FILE *fp, int x, int y, char hanzi[2], int color);
char *drawHZWord12(FILE *fp, int x, int y, char hanzi[2], int color);
void drawHZBuffer(int x, int y, char *hanzibuffer, int color, int fontsize);
void drawHZBuffer2(int x, int y, char *hanzibuffer, int color, int fontsize);

#endif