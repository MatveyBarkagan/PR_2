#ifdef MYLIB_H
#define MYLIB_H

#include <stdio.h>

//Уровни журналирования
#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3
#define CRITICAL 4

//Драйверы вывода
#define DRIVER_CONSOLE 0
#define DRIVER_FILE 1

//Форматы сообщений 
#define FORMAT_PLAIN 0
#define FORMAT_JSON  1

int mysyslog(const char* msg, int level, int format, const char* path);

#endif
