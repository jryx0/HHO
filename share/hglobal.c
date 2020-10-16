#include "macrodef.h"
#include "pinyin.h"
#include "hglobal.h"

#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define TESTGLOBAL(X, Y)                                  \
  if (X == NULL)                                          \
  {                                                       \
    fprintf(stderr, "%s(%d):%s", __FILE__, __LINE__, #X); \
    destoryGlobalSettting(Y);                             \
    exit(-1);                                             \
  }

/**
 * 初始化系统参数，并加载系统资源文件
 * 
 * @return globaled指针
 * 错误退出程序 
 */
globaldef *initGlobalSetting(void)
{
  globaldef *_global = malloc(sizeof(globaldef));
  TESTNULL(_global, NULL);

  memset(_global, 0, sizeof(globaldef));

  ///////////加载资源//////////////////
  //加载拼音索引文件
  _global->pinyin = initPYHZIndex();
  TESTNULL(_global->pinyin, NULL);
  _global->activePageID = -1;

  //加载鼠标形状
  ReadCursor((unsigned char *)_global->cursor_arrow, FILE_CURSOR_ARROW);
  ReadCursor((unsigned char *)_global->cursor_hand, FILE_CURSOR_HAND);
  _global->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_global->cursor_arrow;

  return _global;
}

void destoryGlobalSettting(globaldef *_g)
{
  if (_g)
  {
    ClosePY(_g->pinyin);
    // fclose(_g->fpBK);

    //fclose(_g->fpLog);
    free(_g);

    _g = NULL;
  }
}

/**
 * 删除字符串中指定位置的字符
 */
int DelPosChar(char *str, int pos)
{
  int len = 0, i = 0;
  if (str == NULL)
  {
    return -1;
  }
  len = strlen(str);
  for (i = pos; i < len; i++)
  {
    str[i - 1] = str[i];
  }
  str[len - 1] = '\0';
  return 0;
}

/*
**  Call as: db_print(level, format, ...);
**  Print debug information if debug flag set at or above level.
*/
void dbg_printf(const char *fmt, ...)
{
  va_list args;
  FILE *fp = fopen("hho.log", "at+");

  TESTNULLVOID(fp);
  //fprintf("[%s]", time(&t));
  va_start(args, fmt);
  vfprintf(fp, fmt, args);
  va_end(args);
  fflush(fp);
  fclose(fp);
}
