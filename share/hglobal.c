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
  //TESTNULL(_global->fpLog, NULL);

  setvbuf(_global->fpLog, NULL, _IOFBF, 512);
  fprintf(stderr, "test %s", "hho.log");

  memset(_global, 0, sizeof(globaldef));

  ///////////������Դ//////////////////
  //����ƴ�������ļ�
  _global->pingyin = initPYHZIndex();
  TESTNULL(_global->pingyin, NULL);

  //���������״
  ReadCursor((unsigned char *)_global->cursor_arrow, FILE_CURSOR_ARROW);
  ReadCursor((unsigned char *)_global->cursor_hand, FILE_CURSOR_HAND);

  _global->mouse.currentCur = _global->cursor_hand;

  return _global;
}

/**
 * ��ȡ�����ֿ��ļ�
 * @param type ��marcodef�ж��� 
 * @param size �ֺ� 16��24��32��48
 * 
 * @return �ֿ��ļ�ָ��
 */
FILE *getFontFile(int type, int size)
{
  char fontfile[32];

  switch (type)
  {
  case SIMKAI: //����
    sprintf(fontfile, FONTPATH "HZK%dK", size);
    break;
  case SIMHEI: //����
    sprintf(fontfile, FONTPATH "HZK%dH", size);
    break;
  case ASCII:
    sprintf(fontfile, FONTPATH "ASC16");
    break;
  default: //����
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
