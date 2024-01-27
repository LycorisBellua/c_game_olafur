#ifndef __FILES_H__
#define __FILES_H__

#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

char* read_file(const char* filepath);
int char_is_digit(const char c);
int char_is_lowercase(const char c);
int char_is_uppercase(const char c);
int hex_char_to_int(const char c);

#define CHAR_IS_DIGIT(c)     ((c) >= '0' && (c) <= '9')
#define CHAR_IS_LOWERCASE(c) ((c) >= 'a' && (c) <= 'z')
#define CHAR_IS_UPPERCASE(c) ((c) >= 'A' && (c) <= 'Z')

#endif

