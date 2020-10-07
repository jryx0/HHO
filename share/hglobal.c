#include "macrodef.h"
#include "hglobal.h"
#include "pinyin.h"

#include <memory.h>

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

  _global->fpLog = fopen("hho.log", "w"); //日志文件
  //TESTNULL(_global->fpLog, NULL);

  setvbuf(_global->fpLog, NULL, _IOFBF, 512);
  fprintf(stderr, "test %s", "hho.log");

  memset(_global, 0, sizeof(globaldef));

  ///////////加载资源//////////////////
  //加载拼音索引文件
  _global->pingyin = initPYHZIndex();
  TESTNULL(_global->pingyin, NULL);

  //加载鼠标形状
  ReadCursor((unsigned char *)_global->cursor_arrow, FILE_CURSOR_ARROW);
  ReadCursor((unsigned char *)_global->cursor_hand, FILE_CURSOR_HAND);

  _global->mouse.currentCur = _global->cursor_hand;

  return _global;
}

/**
 * 获取汉字字库文件
 * @param type 在marcodef中定义 
 * @param size 字号 16、24、32、48
 * 
 * @return 字库文件指针
 */
FILE *getFontFile(int type, int size)
{
  char fontfile[32];

  switch (type)
  {
  case SIMKAI: //楷体
    sprintf(fontfile, FONTPATH "HZK%dK", size);
    break;
  case SIMHEI: //黑体
    sprintf(fontfile, FONTPATH "HZK%dH", size);
    break;
  case ASCII:
    sprintf(fontfile, FONTPATH "ASC16");
    break;
  default: //宋体
    sprintf(fontfile, FONTPATH "HZK%dS", size);
    break;
  }

  return fopen(fontfile, "rb");
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
