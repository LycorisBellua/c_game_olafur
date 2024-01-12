#ifndef __FILE_HANDLING_H__
#define __FILE_HANDLING_H__

#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

#define CHAR_IS_DIGIT(c) (c >= '0' && c <= '9')

char* read_file(const char* filepath);
int char_is_digit(const char c);

#endif

