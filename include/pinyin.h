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

/**
 * 打印ascii码 * 
 * @param fpACSII ascii字库文件指针
 * @param x
 * @param y
 * @param xsize x缩放比例
 * @param ysize y缩放比例
 * @param asc 字符
 * @param color 颜色
 */

void printASCII(FILE *fpACSII, int x, int y, int xsize, int ysize, char asc, int color);
/**
 * 打印一行文本，可以中英文混合
 * 
 * @param x
 * @param y 坐标
 * @param text 文本串
 * @param fonttype 字体-宏定义 宋体(SIMSUN)黑体(SIMHEI)楷体(SIMKAI)
 * @param fontsize 字号 16、24、32、48
 * @param gap 字和字之间的间距，像素为单位
 * @param color 颜色
 */
void printText(int x, int y, char *text, int fonttype, int fontsize, int gap, int color);

#endif