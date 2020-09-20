#ifndef __USERINFO_H__
#define __USERINFO_H__

#include "list.h"

/**
 * �û���¼��Ϣ
 */ 
typedef struct
{
  char username[10];//��¼��
  char password[10];//����
  char type;//0=����,1=ҽ��,2=ҩ��ʦ,3=����,100=����Ա
  unsigned int id;//type�Ķ�Ӧ����(��ҽ��)��Ψһ����
} UserInfo;

/***********�߼�ģ��************/
list_t *GetUserList();

list_t *ReadFileToList(const char *filename, list_t *list, size_t size);

int SaveUserInfo(const char *filename, list_t *userinfolist);

int SaveList(const char *filename, list_t *list, size_t size);

#endif