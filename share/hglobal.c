#include "macrodef.h"
#include "hglobal.h"
#include "pinyin.h"

#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#define TESTGLOBAL(X, Y)                                  \
  if (X == NULL)                                          \
  {                                                       \
    fprintf(stderr, "%s(%d):%s", __FILE__, __LINE__, #X); \
    destoryGlobalSettting(Y);                             \
    exit(-1);                                             \
  }

/**
 * ��ʼ��ϵͳ������������ϵͳ��Դ�ļ�
 * 
 * @return globaledָ��
 * �����˳����� 
 */
globaldef *initGlobalSetting(void)
{

  globaldef *_global = malloc(sizeof(globaldef));
  TESTNULL(_global, NULL);

  _global->fpLog = fopen("hho.log", "w"); //��־�ļ�
  TESTNULL(_global->fpLog, NULL);

  memset(_global, 0, sizeof(globaldef));

  ///////////������Դ//////////////////
  //����ƴ�������ļ�
  _global->pingyin = initPYHZIndex();
  TESTNULL(_global->pingyin, NULL);

  //���������״
  ReadCursor((unsigned char *)_global->cursor_arrow, FILE_CURSOR_ARROW);
  ReadCursor((unsigned char *)_global->cursor_hand, FILE_CURSOR_HAND);

  _global->mouse.currentCur = _global->cursor_arrow;

  return _global;
}

void destoryGlobalSettting(globaldef *_g)
{
  if (_g)
  {
    ClosePY(_g->pingyin);
    // fclose(_g->fpBK);

    fclose(_g->fpLog);
    free(_g);

    _g = NULL;
  }
}

void dbg_printf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}