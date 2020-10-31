#ifndef __TEST_H__
#define __TEST_H__

#include "macrodef.h"

typedef void (*testfuc)(void);
#define test(x) test##x

#define TARGET "C:\\HHO\\main.c"

#endif