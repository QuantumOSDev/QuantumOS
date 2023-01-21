/*
 * QuantumOS Copyright (c) 2021-2022
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <stdio.h>
#include <stdarg.h>

#include "../include/sys/syscall.h"

#ifdef __cplusplus
extern "C" {
#endif

void clearerr(FILE *);

char *ctermid(char *);

int dprintf(int, const char *restrict, ...);

int fclose(FILE *);

FILE *fdopen(int, const char *);

int feof(FILE *);

int ferror(FILE *);

int fflush(FILE *);

int fgetc(FILE *);

int fgetpos(FILE *restrict, fpos_t *_restrict);

char *fgets(char *restrict, int, FILE *_restrict);

int fileno(FILE *);

void flockfile(FILE *);

FILE *fmemopen(void *restrict, size_t, const char *_restrict);

FILE *fopen(const char *restrict, const char *_restrict);

int fprintf(FILE *restrict, const char *_restrict, ...);

int fputc(int, FILE *);

int fputs(const char *restrict, FILE *_restrict);

size_t fread(void *restrict, size_t, size_t, FILE *_restrict);

FILE *freopen(const char *restrict, const char *_restrict, FILE *__restrict);

int fscanf(FILE *restrict, const char *_restrict, ...);

int fseek(FILE *, long, int);

int fseeko(FILE *, off_t, int);

int fsetpos(FILE *, const fpos_t *);

long ftell(FILE *);

off_t ftello(FILE *);

int ftrylockfile(FILE *);

void funlockfile(FILE *);

size_t fwrite(const void *restrict, size_t, size_t, FILE *_restrict);

int getc(FILE *);

int getchar(void);

int getc_unlocked(FILE *);

int getchar_unlocked(void);

ssize_t getdelim(char **restrict, size_t *_restrict, int, FILE *__restrict);

ssize_t getline(char **restrict, size_t *_restrict, FILE *__restrict);

char *gets(char *);

FILE *open_memstream(char **, size_t *);

int pclose(FILE *);

void perror(const char *);

FILE *popen(const char *, const char *);

int printf(const char* __format, ...)
{
    va_list arg;
    va_start(arg, __format);
    // int __return = vprintf(__format, arg);
    int __return = 0;
    va_end(arg);

    return __return;
}

int putc(int, FILE *);

int putchar(int);

int putc_unlocked(int, FILE *);

int putchar_unlocked(int);

int puts(const char* __msg)
{
    int __msg_index = 0;
    int __msg_len = 0;
    int __fd = 0;

    while (__msg[__msg_index] != '\0')
        __msg_index++;

    syscall3(SYS_write,  __fd, __msg, __msg_len);
}

int remove(const char *);

int rename(const char *, const char *);

int renameat(int, const char *, int, const char *);

void rewind(FILE *);

int scanf(const char *restrict, ...);

void setbuf(FILE *restrict, char *_restrict);

int setvbuf(FILE *restrict, char *_restrict, int, size_t);

int snprintf(char *restrict, size_t, const char *_restrict, ...);

int sprintf(char *restrict, const char *_restrict, ...);

int sscanf(const char *restrict, const char *_restrict, ...);

char *tempnam(const char *, const char *);

FILE *tmpfile(void);

char *tmpnam(char *);

int ungetc(int, FILE *);

int vdprintf(int, const char *restrict, va_list);

int vfprintf(FILE *restrict, const char *_restrict, va_list);

int vfscanf(FILE *restrict, const char *_restrict, va_list);

int vprintf(const char *restrict, va_list);

int vscanf(const char *restrict, va_list);

int vsnprintf(char *restrict, size_t, const char *_restrict, va_list);

int vsprintf(char *restrict, const char *_restrict, va_list);

int vsscanf(const char *restrict, const char *_restrict, va_list);

#ifdef __cplusplus
}
#endif