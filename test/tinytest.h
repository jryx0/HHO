/* TinyTest: A really really really tiny and simple no-hassle C unit-testing
 * framework.
 *
 * Features:
 *   - No library dependencies. Not even itself. Just a header file.
 *   - Simple ANSI C. Should work with virtually every C or C++ compiler on
 *     virtually any platform.
 *   - Reports assertion failures, including expressions and line numbers.
 *   - Stops test on first failed assertion.
 *   - ANSI color output for maximum visibility.
 *   - Easy to embed in apps for runtime tests (e.g. environment tests).
 *
 * Example Usage:
 *
 *    #include "tinytest.h"
 *    #include "mylib.h"
 *
 *    void test_sheep()
 *    {
 *      ASSERT("Sheep are cool", are_sheep_cool());
 *      ASSERT_EQUALS(4, sheep.legs);
 *    }
 *
 *    void test_cheese()
 *    {
 *      ASSERT("Cheese is tangy", cheese.tanginess > 0);
 *      ASSERT_STRING_EQUALS("Wensleydale", cheese.name);
 *    }
 *
 *    int main()
 *    {
 *      RUN(test_sheep);
 *      RUN(test_cheese);
 *      return TEST_REPORT();
 *    }
 *
 * To run the tests, compile the tests as a binary and run it.
 *
 * Project home page: http://github.com/joewalnes/tinytest
 *
 * 2010, -Joe Walnes <joe@walnes.com> http://joewalnes.com
 */

#ifndef _TINYTEST_INCLUDED
#define _TINYTEST_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#ifdef __BORLANDC__
#include <conio.h>
#elif defined _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* Main assertion method */
#define ASSERT(msg, expression)                                                \
  if (!tt_assert(__FILE__, __LINE__, (msg), (#expression),                     \
                 (expression) ? 1 : 0))                                        \
  return

/* Convenient assertion methods */
/* TODO: Generate readable error messages for assert_equals or assert_str_equals
 */
#define ASSERT_EQUALS(expected, actual)                                        \
  ASSERT(("ASSERT_EQUALS(" #expected "," #actual ")"), (expected) == (actual))

#define ASSERT_STRING_EQUALS(expected, actual)                                 \
  ASSERT(("ASSERT_STRING_EQUALS(" #expected "," #actual ")"),                  \
         strcmp((expected), (actual)) == 0)

/* Run a test() function */
#define RUN(test_function)                                                     \
  assert_num = 1;                                                              \
  printf("Test%d:" #test_function "\r\n", ++test_num);                         \
  tt_execute((#test_function), (test_function));

#define TEST_REPORT() tt_report()

#define TT_COLOR_CODE 0x1B
#define TT_COLOR_RED "[1;31m"
#define TT_COLOR_GREEN "[1;32m"
#define TT_COLOR_RESET "[0m"

int tt_passes = 0;
int tt_fails = 0;
int tt_current_test_failed = 0;
const char *tt_current_msg = NULL;
const char *tt_current_expression = NULL;
const char *tt_current_file = NULL;
int tt_current_line = 0;
int test_num = 0;
int assert_num = 1;

////////////////console printf with Color////////////////////////////
void TextColor(int color);
#ifdef __BORLANDC__
#define printf cprintf
void TextColor(int color) { textcolor(color); }
#elif defined WIN32 || defined _WIN32 || defined _WIN64
enum CONSOLECOLOR {
  BLACK = 0,
  BLUE = 1,
  GREEN = 2,
  CYAN = 3,
  RED = 4,
  MAGENTA = 5,
  BROWN = 6,
  LIGHTGRAY = 7,
  DARKGRAY = 8,
  LIGHTBLUE = 9,
  LIGHTGREEN = 10,
  LIGHTCYAN = 11,
  LIGHTRED = 12,
  LIGHTMAGENTA = 13,
  YELLOW = 14,
  WHITE = 15
};
void TextColor(int color) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
int getTextColor() {
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
  return csbiInfo.wAttributes;
}
void clrscr() { system("cls"); }
#else
void TextColor(int color) {
  switch (color) {
  case BLACK:
    printf("\033[0;30m");
    break;
  case RED:
    printf("\033[0;31m");
    break;
  case GREEN:
    printf("\033[0;32m");
    break;
  case DARK_YELLOW:
    printf("\033[0;33m");
    break;
  case BLUE:
    printf("\033[0;34m");
    break;
  case PURPLE:
    printf("\033[0;35m");
    break;
  case CYAN:
    printf("\033[0;36m");
    break;
  case GRAY:
    printf("\033[0;37m");
    break;
  case DARK_GRAY:
    printf("\033[1;30m");
    break;
  case LIGHT_RED:
    printf("\033[1;31m");
    break;
  case LIGHT_GREEN:
    printf("\033[1;32m");
    break;
  case YELLOW:
    printf("\033[1;33m");
    break;
  case LIGHT_BLUE:
    printf("\033[1;34m");
    break;
  case LIGHT_MAGENTA:
    printf("\033[1;35m");
    break;
  case LIGHT_CYAN:
    printf("\033[1;36m");
    break;
  case WHITE:
    printf("\033[1;37m");
    break;
  }
}

#endif
///////////////////////////////////////////////////////////////////

void tt_execute(const char *name, void (*test_function)()) {
  tt_current_test_failed = 0;
  assert_num++;

  test_function();
  if (tt_current_test_failed) {
    // printf("failure: %s:%d: In test %s(BLUE):\n    %s (%s)\n",
    // tt_current_file,
    //        tt_current_line, name, tt_current_msg, tt_current_expression);
    TextColor(RED);
    printf("\r\nFailure in: %s( %s )\r\n", name, tt_current_file);
    TextColor(LIGHTRED);
    printf("\tAssert%d(Line : %d): %s\r\n", assert_num,
           tt_current_line, tt_current_msg);
    tt_fails++;
  } else {
    TextColor(GREEN);
    printf("%s Passed.\r\n", name);
    TextColor(LIGHTGRAY);
    tt_passes++;
  }

  printf("\r\n");
}

int tt_assert(const char *file, int line, const char *msg,
              const char *expression, int pass) {
  tt_current_msg = msg;
  tt_current_expression = expression;
  tt_current_file = file;
  tt_current_line = line;
  tt_current_test_failed = !pass;
  return pass;
}

int tt_report(void) {

  TextColor(LIGHTGRAY);
  printf("=======================Finish all test========================\r\n");
  TextColor(LIGHTGRAY);

  if (tt_fails) {
    // printf("%c%sFAILED%c%s [%s] (passed:%d, failed:%d, total:%d)\n",
    //        TT_COLOR_CODE, TT_COLOR_RED, TT_COLOR_CODE, TT_COLOR_RESET,
    //        tt_current_file, tt_passes, tt_fails, tt_passes + tt_fails);
    TextColor(RED);
    printf("FAILED:");
    TextColor(LIGHTGRAY);
    printf(" [%s] (passed:%d, failed:%d, total:%d)\r\n", tt_current_file,
           tt_passes, tt_fails, tt_passes + tt_fails);
    return -1;
  } else {
    // printf("%c%sPASSED%c%s [%s] (total:%d)\n", TT_COLOR_CODE, TT_COLOR_GREEN,
    //        TT_COLOR_CODE, TT_COLOR_RESET, tt_current_file, tt_passes);
    TextColor(GREEN);
    printf("PASSED:");
    TextColor(LIGHTGRAY);
    printf(" [%s] (total:%d)\r\n", tt_current_file, tt_passes);
    return 0;
  }
}

#endif