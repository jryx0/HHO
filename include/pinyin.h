#ifndef __PINYIN_H__
#define __PINYIN_H__
#include <stdio.h>

#define MAXNUMPY 403

typedef struct
{
  char pinying[7];
  int postion;
} hanziIndex;

typedef struct
{
  FILE *fpHanzi;
  hanziIndex index[MAXNUMPY];
} pyInput;

/**
 * ��ƴ�������ļ�,���������С�
 * Ϊ����Ƶ���򿪹ر��ֿ��ļ������溺�ֿ��ļ�ָ�롣 
 * @return pyInput ƴ���ṹָ��
 */
pyInput *initPYHZIndex(void);

/**
 * ����������ȡƴ����Ӧ�ĺ�����ʼ��ַ������һ��ƴ����Ӧ����ͬ�����ĺ���
 * ע��һ�����������ֽڡ�
 * @param hzIdx ƴ���ṹ
 * @param py ƴ���ַ����������ǲ�����ƴ��
 * 
 * @return ��ѡ�����ַ�����δ�ҵ�����NULL
 */
char *getCandidateHZbyPY(pyInput *hzIdx, const char *py);

/**
 * �ر�ƴ���ļ�
 * 
 */
void ClosePY(pyInput *hzIdx);

/**
 * ����Ƿ�����Чƴ����
 * Ŀǰֻ����Ƿ�����ĸ
 * 
 * @param py ƴ���ַ���
 * @return 1 �Ϸ�ƴ�� 0 �ǺϷ�ƴ��
 */
int checkvalidatepy(char *py);

/**
 * ��ӡascii�� * 
 * @param fpACSII ascii�ֿ��ļ�ָ��
 * @param x
 * @param y
 * @param xsize x���ű���
 * @param ysize y���ű���
 * @param asc �ַ�
 * @param color ��ɫ
 */

void printASCII(FILE *fpACSII, int x, int y, int xsize, int ysize, char asc, int color);
/**
 * ��ӡһ���ı���������Ӣ�Ļ��
 * 
 * @param x
 * @param y ����
 * @param text �ı���
 * @param fonttype ����-�궨�� ����(SIMSUN)����(SIMHEI)����(SIMKAI)
 * @param fontsize �ֺ� 16��24��32��48
 * @param gap �ֺ���֮��ļ�࣬����Ϊ��λ
 * @param color ��ɫ
 */
void printText(int x, int y, char *text, int fonttype, int fontsize, int gap, int color);

#endif