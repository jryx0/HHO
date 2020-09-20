#include "userinfo.h"
#include "list.h"
#include "macrodef.h"
#include <stdio.h>

/***********�߼�ģ��************/

list_t *GetUserList()
{
  // list_t *pUserlist = ReadUserFile(USERINFOFILE);
  // return pUserlist;
  return NULL;
}

/***********����ģ��************/
list_t *ReadFileToList(const char *filename, list_t *list, size_t size)
{
  FILE *fp;
  void *pTemp;

  if (NULL == list)
    return NULL;
  if ((fp = fopen(filename, "r")) == NULL)
  {
    printf("unable to open %s\r\n", filename);
    return NULL;
  }

  while (1)
  {
    pTemp = malloc(size);

    if (1 == fread(pTemp, size, 1, fp))
    {
      list_rpush(list, list_node_new(pTemp));
    }
    else
    {
      free(pTemp);
      break;
    }
  }

  fclose(fp);
  return list;
}

/**
 * ��������ʽ�洢�û�����
 * @filename:�����ļ���
 * @userinfo:�û���Ϣ����ͷָ��
 * @return:���������������ڵ�������ͬ������1����ͬ�򷵻�0
 */
int SaveUserInfo(const char *filename, list_t *userinfolist)
{
  return userinfolist->len == SaveList(filename, userinfolist, sizeof(UserInfo)) ? 1 : 0;
}

/**
 * ͨ����������Ʊ��溯��
 * @filename:�����ļ���
 * @list:�洢����ͷָ��
 * @size:����ڵ�ṹ��С
 * @return:����ʧ��,����0;����ɹ�,���ر���Ľڵ�����
 */
int SaveList(const char *filename, list_t *list, size_t size)
{
  FILE *fp;
  list_iterator_t *it;
  list_node_t *node;
  int counter = 0;

  //ֻд���ļ�
  if ((fp = fopen(filename, "wb")) == NULL)
  {
    printf("unable to open %s", filename);
    return 0;
  }

  //��������,ȡ��ÿ���ڵ㴢��
  it = list_iterator_new(list, LIST_HEAD);
  while ((node = list_iterator_next(it)))
  {
    if (1 == fwrite(node->val, size, 1, fp))
      counter++;
  }
  fclose(fp);

  return counter;
}

/**
 * ͨ�������ı����溯��
 * @filename:�����ļ���
 * @list:�洢����ͷָ��
 * @size:����ڵ�ṹ��С
 * @return:����ʧ��,����0;����ɹ�,���ر���Ľڵ�����
 * ��ʽ:
 * ÿ��Ϊһ����¼,�����ָ�.
 * ----10----|-----10-----|--1--|---5----
 * 
 */