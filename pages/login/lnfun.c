/**
 * ��Ҫ������
 * 1.���µ�¼��ť���ж�������û��������Ƿ���ȷƥ�䣬�������û�����
 * 2.��ע�ᰴťת����ע�����
 * 3.����������ת����Ӧ��������
 * 4.ע�ᰴť���º󣬽����������ݴ����ļ�
 * ����Ϊ�ؼ��Դ�����
 */

#include <stdio.h>
#include <string.h>
//�涨�û���������8���֣����벻����8λ������ֻ��Ӣ�ĺ����֣�����ASCII��棬userindex.txt�����洢���û��������룬�û����ͣ���
int judge(char *username, char *password)
{
  FILE *fp;
  char name[17], password[9];
  if (strlen(username) > 16)
    return 0; //�û�������
  if (strlen(password) > 8)
    return 1; //���볬��
  if (fp = fopen("data//userindex.txt", "rt") == NULL)
    return -1;
  while (fp != EOF)
  {
    while (fread(name, 1, 1, fp) != ';')
      
  }
}