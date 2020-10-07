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
 * 打开拼音索引文件,读入数组中。
 * 为避免频发打开关闭字库文件，保存汉字库文件指针。 
 * @return pyInput 拼音结构指针
 */
pyInput *initPYHZIndex(void);

/**
 * 根据索引读取拼音对应的汉字起始地址，返回一个拼音对应的相同读音的汉字
 * 注意一个汉字两个字节。
 * @param hzIdx 拼音结构
 * @param py 拼音字符串，可以是不完整拼音
 * 
 * @return 候选汉字字符串。未找到返回NULL
 */
char *getCandidateHZbyPY(pyInput *hzIdx, const char *py);

/**
 * 关闭拼音文件
 * 
 */
void ClosePY(pyInput *hzIdx);

/**
 * 检查是否是有效拼音，
 * 目前只检查是否是字母
 * 
 * @param py 拼音字符串
 * @return 1 合法拼音 0 非合法拼音
 */
int checkvalidatepy(char *py);
#endif