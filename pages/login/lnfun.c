/**
 * 需要函数：
 * 1.按下登录按钮后判断输入的用户名密码是否正确匹配，并返回用户类型
 * 2.从注册按钮转跳至注册界面
 * 3.根据类型跳转至对应的主界面
 * 4.注册按钮按下后，将各输入内容存入文件
 * 其他为控件自带功能
 */

#include <stdio.h>
#include <string.h>
//规定用户名不超过8个字，密码不超过8位，密码只有英文和数字，可用ASCII码存，userindex.txt用来存储“用户名，密码，用户类型；”
int judge(char *username, char *password)
{
  FILE *fp;
  char name[17], password[9];
  if (strlen(username) > 16)
    return 0; //用户名超长
  if (strlen(password) > 8)
    return 1; //密码超长
  if (fp = fopen("data//userindex.txt", "rt") == NULL)
    return -1;
  while (fp != EOF)
  {
    while (fread(name, 1, 1, fp) != ';')
      
  }
}