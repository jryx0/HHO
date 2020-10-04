#include "hglobal.h"
#include "pinyin.h"

#include <GRAPHICS.H>
#include <memory.h>

#define TEST(X, G)             \
  if (X == NULL)               \
  {                            \
    fprintf(stderr, "%s", #X); \
    destoryGlobalSettting(G);  \
    exit(-1);                  \
    return NULL;               \
  }

globaldef *initGlobalSetting(void)
{
  globaldef *_global = malloc(sizeof(globaldef));
  TEST(_global, _global);

  memset(_global, 0, sizeof(globaldef));
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

globaldef *loadSvgaResouce(globaldef *_g)
{
  //加载鼠标形状
  ReadCursor((unsigned char *)_g->cursor_arrow, MOUSE_WIDHT, MOUSE_HEIGHT, FILE_CURSOR_ARROW);
  ReadCursor((unsigned char *)_g->cursor_hand, MOUSE_WIDHT, MOUSE_HEIGHT, FILE_CURSOR_HAND);

  //加载拼音索引文件
  _g->pingyin = initPYHZIndex();
  TEST(_g->pingyin, _g);

  //打开字库文件
  _g->fphanzi_ss24 = fopen(FILE_SIMSUN24, "r");
  _g->fphanzi_sh24 = fopen(FILE_SIMHEI24, "r");
  _g->fphanzi_ss16 = fopen(FILE_SIMSUN16, "r");
  _g->fphanzi_sh16 = fopen(FILE_SIMHEI16, "r");
  return _g;
}

void loadMouse(globaldef *_g)
{
  InitMouse();

  SetMouseCoord(0, 0);
  //保存鼠标背景
  MouseSavebk((unsigned int *)_g->cursorBK, _g->mouse.x, _g->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);
  //设置鼠标最大区域
  SetMouseArea(0, 0, getmaxx(), getmaxy());
}