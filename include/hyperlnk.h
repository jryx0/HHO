#ifndef __HYPERLNK_H__
#define __HYPERLNK_H__

#include "hhosvga.h"
#include "HBaseWin.h"

#define UNDERLINE 1
#define NONE 0

/**
 * ���������ӿؼ�
 * 
 */
hbasewinAttr *Createhyperlink(hbasewinAttr *parent, int x, int y, int nWidth,
                               int nHeight, int winID, const char *title);

/**
 * ���ó����ӵ��ʱ����Ļ��ʾ
 * 
 */
void OnClick_hyperlink(hbasewinAttr *link, void *value);

/**
 * ���ó����ӵ����ʱ����Ļ��ʾ
 * 
 */
void OnLeave_hyperlink(hbasewinAttr *btn, void *value);
/**
 * ���ó�������Ļ�����ʾ
 * 
 */
void OnPaint_hyperlink(hbasewinAttr *link, void *value);

/**
 * ����ȱʡ��ť����
 * Ĭ�ϱ�׼��ť
 * ɾ��buttonʱ�� freeWin�ͷ��ڴ�
 */
WinStyle *getlinkStyle(void);

#endif