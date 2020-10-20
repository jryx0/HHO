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
  _global->InputMode = ENGLISH;

  //加载鼠标形状
  ReadCursor((unsigned char *)_global->cursor_arrow, FILE_CURSOR_ARROW);
  ReadCursor((unsigned char *)_global->cursor_hand, FILE_CURSOR_HAND);
  _global->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_global->cursor_arrow;
  _global->theme = 1;

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

WinStyle *getWinTheme(WinStyle *_winStyle, int type)
{
  TESTNULL(_winStyle, _winStyle);

  switch (type)
  {
  case 1:
    _winStyle->bkcolor = 0x03DF;
    _winStyle->fontcolor = 0x0000;
    _winStyle->fontsize = 16;
    _winStyle->fonttype = SIMYOU;
    _winStyle->bkcolor1 = 0x003F;
    _winStyle->bkcolor2 = 0x03DF;
    _winStyle->height = _winStyle->fontsize * 2;
    break;
  case 2:
    _winStyle->bkcolor = 0xB81F;
    _winStyle->fontcolor = 0xFFFF;
    _winStyle->fontsize = 16;
    _winStyle->fonttype = SIMSUN;
    _winStyle->bkcolor1 = 0xA815;
    _winStyle->bkcolor2 = 0xB81F;
    //_winStyle->height = _winStyle->fontsize * 5 / 3;
    _winStyle->height = _winStyle->fontsize * 2;
    break;
  case 3:
    _winStyle->bkcolor = 0x050B;
    _winStyle->fontcolor = 0x0000;
    _winStyle->fontsize = 24;
    _winStyle->fonttype = SIMKAI;
    _winStyle->bkcolor1 = 0x2D28;
    _winStyle->bkcolor2 = 0x2D28;
    _winStyle->height = _winStyle->fontsize * 5 / 3;
    //_winStyle->height = _winStyle->fontsize * 2;
  default:
    break;
  }

  return _winStyle;
}