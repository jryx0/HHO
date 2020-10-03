#ifndef __MOUSE_H__
#define __MOUSE_H__

#define MOUSE_ENTERED 1
#define MOUSE_LEFT 2
#define MOUSE_OUTSIDE 4
#define MOUSE_INSIDE 8
#define MOUSE_BUTTON_DOWN 16
#define MOUSE_BUTTON_UP 32
#define MOUSE_BUTTON_STILL_DOWN 64
#define MOUSE_BUTTON_STILL_UP 128

typedef struct
{
  unsigned int x, y;
  unsigned char leftDown, rightDown;
  unsigned char oldLeftDown, oldRightDown;
  char leftClickState, rightClickState;
  char moveState;
} mousestatus;

/**
 * ��ʼ�����
 * @return �Ƿ�ɹ�
 */
int InitMouse(void);

void ShowMouse(void);
void HideMouse(void);
void ReadMouse(int *f, int *x, int *y);
void SetMouseCoord(int x, int y);
void SetMouseArea(int Xmin, int Ymin, int Xmax, int Ymax);
void MouseHideArea(int x1, int y1, int x2, int y2);

/**
 * �������״̬
 * @param status ���
 */
void updateMouseStatus(mousestatus *status);

/**
 * ��ȡ����ļ����洢��buf��
 * 
 * @param buf ���滺��
 * @param width �����
 * @param height ���߶�
 * @param filename ����ļ� 
 */
int ReadCursor(unsigned char *buf, int width, int height, char *filename);

/**
 * ����cur�д洢�������״,������ꡣ cur[i][j] = 0 ������ = 1 ��ɫ =���� ��ɫ
 * @param cur �����״, �������� width * height ��С�Ļ���
 * @param x
 * @param y �����������
 * @param width �����
 * @param height ���߶�
 */
void DrawCursor(unsigned char *cur, int x, int y, int width, int height);

/**
 * ������긲������
 * @param buffer �������滺��
 * @param x
 * @param y ��������
 * @param width �����
 * @Param height ���߶�
 */
void MouseSavebk(unsigned int *far buffer, int x, int y, int width, int height);

/**
 * �ָ���긲������
 * @param buffer �������滺��
 * @param x
 * @param y ��������
 * @param width �����
 * @Param height ���߶�
 */
void MousePutbk(unsigned int *far buffer, int x, int y, int width, int height);

#endif