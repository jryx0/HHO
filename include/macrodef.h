#ifndef __MACRODEF_H__
#define __MACRODEF_H__

#define TRUE 1
#define FALSE 0
//////////////////////////////�������////////////////////////////////////////////
// #define TESTNULL(X, Y)                                    \
//   if (X == NULL)                                          \
//   {                                                       \
//     fprintf(stderr, "%s(%d):%s", __FILE__, __LINE__, #X); \
//     return Y;                                             \
//   }
 
#define TESTNULL(X, Y) \
  if (X == NULL)       \
  {                    \
    return Y;          \
  }

#define TESTNULLVOID(X) \
  if (X == NULL)        \
    return;

#define TRACE(x)  \
  do              \
  {               \
    dbg_printf x; \
  } while (0)

#define TRACELOC(x) \
  do                \
  {                 \
    db_printloc(x)  \
  } while (0)

void dbg_printf(const char *fmt, ...);
void db_printloc(const char *file, int line, const char *fmt, ...);

//////////////////////////////////////�궨��///////////////////////////////////
//��ʾģʽ�궨��
#define SVGA64K
#define SVGA1024x768

#define MAXWINDOWS 100

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768

#define HEADER_HEIGHT 126
#define FOOTER_HEIGHT 44

#define EPAGE_MARGIN 5
#define PAGE_X 5
#define PAGE_Y (HEADER_HEIGHT + 5)
#define PAGE_W (SCR_WIDTH - 10)
#define PAGE_H ((SCR_HEIGHT - (HEADER_HEIGHT + FOOTER_HEIGHT + 10)) - 20)

//�û����ͺ궨��
#define PATIENT 1
#define DOCTOR 2
#define PHARMCY 3
#define LOGISTIC 4

//-------------------------ȫ���ļ�·��--------------------------------
//�����ļ��ж���
#if !defined(__BORLANDC__)
#define DATAPATH "data\\"
#else
#define DATAPATH "C:\\HHO\\data\\"
#endif

//�û���Ϣ�洢�ļ�
#define USERINFOFILE DATAPATH "database\\userinfo.txt"
//ҽ����Ϣ�洢�ļ�
#define DOCTORINFOFILE DATAPATH "database\\docinfo.txt"
//������Ϣ�洢�ļ�
#define PATIENTINFOFILE DATAPATH "database\\painfo.txt"
//������Ϣ�洢�ļ�
#define DEPTINFOFILE DATAPATH "database\\deptinfo.txt"

//�˵���Ϣ�洢�ļ�
#define POSTFILE DATAPATH "database\\logistic\\postinfo.txt"

//������Ϣ�洢�ļ�
#define PRESCRITIONFILE DATAPATH "database\\ps\\psinfo.txt"

//pinyin files
#define FILE_PYINDEX DATAPATH "pinyin\\index.txt"
#define FILE_HANZI DATAPATH "pinyin\\hanzi.txt"

//�����ļ�
#define FONTPATH DATAPATH "font\\"
#define SIMSUN 0 //����
#define SIMHEI 1 //����
#define SIMKAI 2 //����
#define SIMYOU 3 //��Բ
#define ASCII 10 //Ӣ���ַ�

//mouse cursors
//����С
// #define MOUSE_WIDTH 20
// #define MOUSE_HEIGHT 30
// #define FILE_CURSOR_ARROW DATAPATH "cursor\\arrowbg.cur"
// #define FILE_CURSOR_HAND DATAPATH "cursor\\handbg.cur"
//#define FILE_CURSOR_ARROW DATAPATH "cursor\\arrowbg.cur"

#define MOUSE_WIDTH 16
#define MOUSE_HEIGHT 16
#define FILE_CURSOR_ARROW DATAPATH "cursor\\arrow.cur"
#define FILE_CURSOR_HAND DATAPATH "cursor\\hand.cur"

//����ƶ�״̬
#define MOUSE_ENTERED 1
#define MOUSE_LEFT 2
#define MOUSE_OUTSIDE 4
#define MOUSE_INSIDE 8
#define MOUSE_BUTTON_DOWN 16
#define MOUSE_BUTTON_UP 32
#define MOUSE_BUTTON_STILL_DOWN 64
#define MOUSE_BUTTON_STILL_UP 128

//����borlandc��vscode�еļ�����
#ifndef __BORLANDC__
#define huge
#define far
#define _interrupt

#ifndef _REG_DEFS
#define _REG_DEFS
struct WORDREGS
{
  unsigned int ax, bx, cx, dx, si, di, cflag, flags;
};

struct BYTEREGS
{
  unsigned char al, ah, bl, bh, cl, ch, dl, dh;
};

union REGS
{
  struct WORDREGS x;
  struct BYTEREGS h;
};

#endif //_REG_DEFS
#endif

#endif