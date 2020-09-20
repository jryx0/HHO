#ifndef __USERINFO_H__
#define __USERINFO_H__

#include "list.h"

/**
 * 用户登录信息
 */ 
typedef struct
{
  char username[10];//登录名
  char password[10];//密码
  char type;//0=患者,1=医生,2=药剂师,3=物流,100=管理员
  unsigned int id;//type的对应类型(如医生)的唯一编码
} UserInfo;

/***********逻辑模块************/
list_t *GetUserList();

list_t *ReadFileToList(const char *filename, list_t *list, size_t size);

int SaveUserInfo(const char *filename, list_t *userinfolist);

int SaveList(const char *filename, list_t *list, size_t size);

#endif