#ifndef __MACRODEF_H__
#define __MACRODEF_H__

//#define SVGA32K

#ifndef __BORLANDC__
#define DATAPATH "data\\"
#define huge
#else
#define DATAPATH "E:\\HHO\\data\\"
#endif

#define USERINFOFILE DATAPATH"userinfo.txt"
#define DOCTORINFOFILE DATAPATH"doctorinfo.txt"

#define FILE_PYINDEX DATAPATH"index.txt"
#define FILE_HANZI DATAPATH"hanzi.txt"

#endif