#include "pinyin.h"
#include "mouse.h"

#include <graphics.h>

//get input ->find pinyin ->output hanzi at the bottom and add
//number before them. ->if click on one of the hanzi, show it on
//textbox. if click on the next\before button, change hanzi at
//the bottom.

// void pinyin_input(char *py, pyInput *idx)
// {
//   //initPYHZIndex();
//   char *hanzi; //max=103
//   char temphanzi[16];
//   int i, j, k;
//   int judgemouse, judgehanzi = 0, judgeend = 0;
//   int mousex, mousey, button;

//   hanzi = getCandidateHZbyPY(idx, py);
//   for (i = 0, j = 0; *(hanzi + i); i += 2)
//   {
//     printHZ16();
//     //�����������:��������֣���������֣��м���һ���ո�
//     *(hanzi + i) = temphanzi[j * 2];
//     *(hanzi + i + 1) = temphanzi[j * 2 + 1];
//     if (!(*(hanzi + i + 2)))
//       judgeend = 1;
//     j++;
//     if (j == 8 || judgeend == 1)
//     {
//       while (1)
//       {
//         ReadMouse(button, mousex, mousey);
//         //�����ж��ڲ������뷨����
//         for (k = 0; k < 8; k++)
//         {
//           //judgemouse=IsMouseInBox();
//           if (judgemouse == 1)
//           {
//             //�����������ı���
//             judgehanzi = 1;
//             break;
//           }
//         }
//         // ��һҳ
//         // if (IsMouseInBox())
//         // {
//         //   j = 0;
//         //   if (i - 32 > 0)
//         //     i -= 32;
//         //   break;
//         // }

//         // ��һҳ
//         // if (IsMouseInBox())
//         // {
//         //   if (judgeend == 1)
//         //     i -= 16;
//         //   j = 0;
//         //   break;
//         // }
//         delay(30);
//       }
//       if (judgehanzi == 1)
//       {
//         //������뷨
//         setfillstyle(SOLID_FILL, RealFillColor(0));
//         //bar(10, 10, 80, 80);
//         break;
//       }
//     }
//   }
// }