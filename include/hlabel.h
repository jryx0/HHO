#ifndef __HLABEL_H__
#define __HLABEL_H__

#include "HBaseWin.h"

#define LABEL_TITLE 0  //显示title
#define LABEL_FILE_TXT 1   //显示文件名为tile的txt文件 
#define LABEL_FILE_IMG 2   //显示文件名为title的图片文件

hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title);
void OnPaint_Label(hbasewinAttr *label, void *value);

#endif