#include "list.h"
#include "macrodef.h"
#include "memory.h"
#include "tinytest.h"
#include "userinfo.h"
#include <conio.h>
#include <string.h>

void test_userinfo_save() {
  UserInfo *user = (UserInfo *)malloc(sizeof(UserInfo));
  list_t *userinfo;
  int counter = 0;

  memset(user, 0, sizeof(UserInfo));
  user->id = 12;
  strcpy(user->password, "key");
  user->type = 7;
  strcpy(user->username, "name");

  userinfo = list_new();
  list_rpush(userinfo, list_node_new(user));

  user = (UserInfo *)malloc(sizeof(UserInfo));
  memset(user, 0, sizeof(UserInfo));
  user->id = 13;
  strcpy(user->password, "kse");
  user->type = 9;
  strcpy(user->username, "3eme");
  list_rpush(userinfo, list_node_new(user));

  counter = SaveUserInfo(USERINFOFILE, userinfo);
  ASSERT_EQUALS(1, counter);

  list_destroy(userinfo);
}

void test_userinfo_read() {
  UserInfo *user = (UserInfo *)malloc(sizeof(UserInfo));
  list_t *userinfolist;
  list_node_t *ptest;

  userinfolist = list_new();

  userinfolist = ReadFileToList(USERINFOFILE, userinfolist, sizeof(UserInfo));
  if (userinfolist) {
    ptest = list_at(userinfolist, 0);
    ASSERT_EQUALS(12, ((UserInfo *)ptest->val)->id);
  } else
    ASSERT_EQUALS(1, 0);
}

// test in console both in vscode and borlandc
void console_test() {
  clrscr();
  TextColor(WHITE);
  printf("\r\nStart test....\r\n\r\n");
  TextColor(LIGHTGRAY);


  RUN(test_userinfo_save);
  RUN(test_userinfo_read);

  
  TEST_REPORT();
}

// borland GUI test
void borland__GUI_test() {
  
}

/* test runner */
int main() {
#ifdef GUI
  borland__GUI_test();
#else
  console_test();
#endif

  getch();
  return 0;
}