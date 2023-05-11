#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char* ptr);
int strnlen(const char* ptr, int max);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, int count);
int strnlen_terminator(const char* str, int max, char terminator);
int strncmp(const char* str1, const char* str2, int n);
int istrncmp(const char* str1, const char* str2, int n);
int tonumericdigit(char c);
bool isdigit(char c);
char tolower(char s1);

#endif