#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>

/* OS */
#define QUANTUM_OS 1

#define __linux__ 1
#define __linux 1

#define __unix__ 1
#define __unix 1

/* Some important macros */
#define BUFSIZ 8192
#define EOF -1

#ifndef NULL
# define NULL (void*)0
#endif 

#define P_tmpdir	"/tmp"

/* Types */

#define __WINT_TYPE__ unsigned int

typedef long          __off_t;
typedef unsigned long __size_t;
typedef long int      __ssize_t;

/* mbstate structure */
typedef struct __mbstate_t {
    int __count;
    union {
        __WINT_TYPE__ __wch;
        char __wchb[4];
    } __value;		/* Value so far.  */
} __mbstate_t;

/* fpos structure */
typedef struct __fpos_t {
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;

/* FILE structure */
struct _IO_FILE;
struct _IO_marker;
struct _IO_codecvt;
struct _IO_wide_data;

typedef void _IO_lock_t;

typedef struct _IO_FILE {
  int _flags;	

  char* _IO_read_ptr;	
  char* _IO_read_end;	
  char* _IO_read_base;	
  char* _IO_write_base;
  char* _IO_write_ptr;	
  char* _IO_write_end;	
  char* _IO_buf_base;	
  char* _IO_buf_end;	

  char* _IO_save_base; 
  char* _IO_backup_base; 
  char* _IO_save_end;

  struct _IO_marker* _markers;

  struct _IO_FILE* _chain;

  int _fileno;
  int _flags2;
  __off_t _old_offset; 

  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;
} __FILE;

typedef __FILE     FILE;
typedef __fpos_t   fpos_t;
typedef __off_t    off_t;
typedef __size_t   size_t;
typedef __ssize_t  ssize_t;

/* IO Functions */
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

int printf(const char *restrict, ...);

int putc(int, FILE *);

int putchar(int);

int putc_unlocked(int, FILE *);

int putchar_unlocked(int);

int puts(const char *);

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

#endif /* _STDIO_H */