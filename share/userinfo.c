#include "userinfo.h"
#include "list.h"
#include "macrodef.h"
#include <stdio.h>

/***********逻辑模块************/

list_t *GetUserList()
{
  // list_t *pUserlist = ReadUserFile(USERINFOFILE);
  // return pUserlist;
  return NULL;
}

/***********数据模块************/
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
 * 二进制形式存储用户链表
 * @filename:保存文件名
 * @userinfo:用户信息链表头指针
 * @return:当保存结点的数量与节点总数相同，返回1，不同则返回0
 */
int SaveUserInfo(const char *filename, list_t *userinfolist)
{
  return userinfolist->len == SaveList(filename, userinfolist, sizeof(UserInfo)) ? 1 : 0;
}

/**
 * 通用链表二进制保存函数
 * @filename:保存文件名
 * @list:存储链表头指针
 * @size:链表节点结构大小
 * @return:保存失败,返回0;保存成功,返回保存的节点数量
 */
int SaveList(const char *filename, list_t *list, size_t size)
{
  FILE *fp;
  list_iterator_t *it;
  list_node_t *node;
  int counter = 0;

  //只写打开文件
  if ((fp = fopen(filename, "wb")) == NULL)
  {
    printf("unable to open %s", filename);
    return 0;
  }

  //遍历链表,取出每个节点储存
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
 * 通用链表文本保存函数
 * @filename:保存文件名
 * @list:存储链表头指针
 * @size:链表节点结构大小
 * @return:保存失败,返回0;保存成功,返回保存的节点数量
 * 格式:
 * 每行为一条记录,定长分割.
 * ----10----|-----10-----|--1--|---5----
 * 
 */