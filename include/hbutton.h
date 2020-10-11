#ifndef __HBUTTON_H__
#define __HBUTTON_H__

#include "hhosvga.h"
#include "HBaseWin.h"

//////////////////////////////��ť״̬�궨��//////////////////////////////////////////////
#define BUTTON_UP 0 //����״̬
#define BUTTON_DOWN 1
#define BUTTON_HOVE 2 //��button����

//���ֶ���
#define TEXT_CENTER 0

//IMG button ʹ��
#define TEXT_LEFT 1
#define TEXT_DOWN 2
#define TEXT_RIGHT 3

//////////////////////////////��ť��ʽ�궨��//////////////////////////////////////////////////

#define STANDARD 1   //��׼��ť
#define LISTBUTTON 2 //������ť
#define IMGBUTTON 3  //ͼƬ��ť



/**
 * ��ť����
 * ��value �ֶδ洢buttonStyle���������û��ư�ť
 * 
 */
hbasewinAttr *CreateButton(hbasewinAttr *parent, int x, int y, int nWidth,
                           int nHeight, int winID, const char *title);
/**
 * ��ť���� 
 */
void OnPaint_button(hbasewinAttr *btn, void *value);

void OnClick_button(hbasewinAttr *btn, void *value);
void OnLeave_button(hbasewinAttr *btn, void *value);
/**
 * ����ȱʡ��ť����
 * Ĭ�ϱ�׼��ť
 */
WinStyle *getbtnStyle(void);
#endif