#include "macrodef.h"
#include "hglobal.h"
#include "pinyin.h"

#include <memory.h>

#define TEST(X, G)             \
  if (X == NULL)               \
  {                            \
    fprintf(stderr, "%s", #X); \
    destoryGlobalSettting(G);  \
    exit(-1);                  \
    return NULL;               \
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
  TEST(_global, _global);

  memset(_global, 0, sizeof(globaldef));

  ///////////加载资源//////////////////
  //加载拼音索引文件
  _global->pingyin = initPYHZIndex();
  TEST(_global, _global->pingyin);

  //打开字库文件
  _global->fphanzi_ss24 = fopen(FILE_SIMSUN24, "r");
  TEST(_global, _global->fphanzi_ss24);

  _global->fphanzi_sh24 = fopen(FILE_SIMHEI24, "r");
  TEST(_global, _global->fphanzi_sh24);

  _global->fphanzi_ss16 = fopen(FILE_SIMSUN16, "r");
  TEST(_global, _global->fphanzi_ss16);

  _global->fphanzi_sh16 = fopen(FILE_SIMHEI16, "r");
  TEST(_global, _global->fphanzi_ss16);

  //加载鼠标形状
  ReadCursor((unsigned char *)_global->cursor_arrow, MOUSE_WIDTH, MOUSE_HEIGHT, FILE_CURSOR_ARROW);
  ReadCursor((unsigned char *)_global->cursor_hand, MOUSE_WIDTH, MOUSE_HEIGHT, FILE_CURSOR_HAND);

  return _global;
}

void destoryGlobalSettting(globaldef *_g)
{
  if (_g)
  {
    ClosePY(_g->pingyin);

    fclose(_g->fpBK);
    fclose(_g->fphanzi_ss24);
    fclose(_g->fphanzi_sh24);
    fclose(_g->fphanzi_ss16);
    fclose(_g->fphanzi_sh16);

    free(_g);
  }
}
