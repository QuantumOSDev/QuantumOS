#ifndef STRING_H
#define STRING_H

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

#endif
