#ifndef __SVGA_H__
#define __SVGA_H__

void initSVGA64k(void);
unsigned int getSVGA(void);
void selectpage(register char page);
void putpixel64k(int x, int y, unsigned int color);
unsigned int getpixel64k(int x, int y);

#endif