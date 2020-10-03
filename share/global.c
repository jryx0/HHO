#include "hglobal.h"
#include <GRAPHICS.H>

#define TEST(X, G)             \
  if (X == NULL)               \
  {                            \
    fprintf(stderr, "%s", #X); \
    destoryGlobalSettting(G);  \
    return NULL;               \
  }

globaldef *initGlobalSetting()
{
  globaldef *_global = malloc(sizeof(globaldef));

  //��ʼ�����뷨�ռ�
  _global->pingyin = malloc(sizeof(pyInput));
  TEST(_global, _global);

  //��ʼ����ͷ���ռ�
  _global->cursor_arrow = malloc(MOUSE_WIDHT * MOUSE_HEIGHT);
  TEST(_global->cursor_arrow, _global);

  //��ʼ���������ռ�
  _global->cursor_hand = malloc(MOUSE_WIDHT * MOUSE_HEIGHT);
  TEST(_global->cursor_hand, _global);

  //��ʼ����걳���ռ�
  _global->cursorBK = malloc(MOUSE_WIDHT * MOUSE_HEIGHT);
  TEST(_global->cursorBK, _global);

  memset(&(_global->mouse), 0, sizeof(mousestatus));
  return _global;
}

void destoryGlobalSettting(globaldef *_g)
{
  if (_g)
  {
    if (_g->cursorBK)
      free(_g->cursorBK);
    if (_g->cursor_arrow)
      free(_g->cursor_arrow);
    if (_g->cursor_hand)
      free(_g->cursor_hand);
    if (_g->pingyin)
      free(_g->pingyin);

    free(_g);
  }
}

globaldef *loadSvgaResouce(globaldef *_g)
{
  ReadCursor(_g->cursor_arrow, MOUSE_WIDHT, MOUSE_HEIGHT, FILE_CURSOR_ARROW);
  ReadCursor(_g->cursor_hand, MOUSE_WIDHT, MOUSE_HEIGHT, FILE_CURSOR_HAND);

  _g->pingyin = initPYHZIndex();
  TEST(_g->pingyin, _g);
}

void loadMouse(globaldef *_g)
{
  InitMouse();
  SetMouseCoord(0, 0);
  MouseSavebk(_g->cursorBK, _g->mouse.x, _g->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);
  SetMouseArea(0, 0, getmaxx(), getmaxy());
}