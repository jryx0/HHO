#ifndef __PINYIN_H__
#define __PINYIN_H__
#include <stdio.h>

#define MAXNUMPY 403

typedef struct {
  char pinying[7];
  int postion;
} hanziIndex;

typedef struct {
  FILE *fpHanzi;
  hanziIndex index[MAXNUMPY];
} pyInput;

pyInput *initPYHZIndex();
char *getCandidateHZbyPY(pyInput *hzIdx, const char *py);
void ClosePY(pyInput *hzIdx);
int checkvalidatepy(char *py);
#endif