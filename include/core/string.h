#ifndef STRING_H
#define STRING_H

#include <core/gtree.h>
#include <core/list.h>

#define NULL (void *) 0

int strlen(const char* str);

char to_uppercase(char c);

char to_lowercase(char c);

void reverse(char* str);

int strcmp(const char *s1, char *s2);

void pop_last_str(char* str);

void append_str(char* str, char c);

char* strcpy(char* dest, const char* src);

char* strtok(char* str, char* delim);

char* strpbrk(const char* str, const char* accept);

char* strdup(const char* str);

char* strrchr(const char* str, int c);

void itoa(int num, char* str, int base);

void ltoa(long long value, char* buf);

int strncmp(const char *__s1, const char *__s2, int __c);

char *strstr(const char *__in, const char *__str);

char *strsep(char **__stringp, const char *__delim);

char *list_to_str(__list_t *__list, const char *__delim);

__list_t *list_strtok(const char *__str, const char *__delim, unsigned int *__ntok);

#endif
