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
#endif