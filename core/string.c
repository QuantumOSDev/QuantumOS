/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <core/string.h>
#include <core/print.h>
#include <core/gtree.h>
#include <core/list.h>

#include <sys/memory.h>

int strlen(const char* str)
{
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

char to_uppercase(char c)
{
    return (char)c - 0x20;
}

char to_lowercase(char c)
{
    return (char)c + 0x20;
}

void reverse(char* str)
{
    int c, i, j;
    for (i = 0, j = strlen(str)-1; i < j; i++, j--)
{
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

int strcmp(const char *s1, char *s2)
{
    int i = 0;

    while ((s1[i] == s2[i]))
{
        if (s2[i++] == 0)
            return 0;
    }
    return 1;
}

void pop_last_str(char* str)
{
    int len = strlen(str);
    str[len-1] = '\0';
}

char* strcpy(char* dest, const char* src)
{
    int i;
    for (i = 0; src[i] != '\0'; i++) 
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}

unsigned int is_delim(char c, char *delim)
{
    while (*delim != '\0')
    {
        if (c == *delim)
            return 1;
        
        delim++;
    }

    return 0;
}

char* strtok(char* str, char* delim)
{
    static char *last;

    if (!str)
        str = last;
    
    if (!str)
        return NULL;

    while (1)
    {
        if (is_delim(*str, delim))
        {
            str++;

            continue;
        }

        if (*str == '\0')
        {
            return NULL;
        }

        break;
    }

    char *ret = str;

    while (1)
    {
        if (*str == '\0')
        {
            last = str;

            return ret;
        }

        if (is_delim(*str, delim))
        {
            *str = '\0';

            last = str + 1;

            return ret;
        }

        str++;
    }
}

char* strpbrk(const char* str, const char* accept)
{
    for (int i = 0; str[i] != '\0'; i++)
{
        for (int j = 0; accept[j] != '\0'; j++)
{
            if (str[i] == accept[j])
                return ((char*)(str + i));
        }
    }
    return NULL;
}

char* strdup(const char* str)
{
    char* copy = (char*)kmalloc(strlen(str) + 1);
    return kmemcpy((void*)copy, (const void*)str, strlen(str) + 1);
}

char* strrchr(const char* str, int c)
{
    for (int i = strlen(str) - 1; i > 0; i--)
{
        if (str[i] == c)
            return ((char*)(str + 1));
    }
    return NULL;
}

void itoa(int num, char* str, int base)
{
    int i = 0;
    int is_neg = 0;
    
    if (num == 0)
{ 
        str[i++] = '0';
        str[i] = '\0';
        return;
    } 
    if (num < 0 && base == 10)
{
        is_neg = 1;
        num = -num;
    }
    while (num != 0)
{
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    if (is_neg)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str);
}

void ltoa(long long value, char* buf)
{
    // char* p;
    // unsigned long long v;

    // v = (value < 0) ? -value: value;
    // p = buf + 31;
    // do{
    //     *p -- = '0' + (v%10);
    //     v /= 10;
    // } while(v);

    // if(value < 0) *p-- = '-';
    // p++;

    // int len = 32 - (p - buf);
    // char *s = (char*)kmalloc(sizeof(char) * (len + 1));
    // kmemcpy(s, p, len);
    // s[len] = '\0';
}

int strncmp(const char *__s1, const char *__s2, int __c)
{
    int __result = 0;

    while (__c)
    {
        __result = *__s1 - *__s2++;

        if ((__result != 0) || (*__s1++ == 0))
        {
            break;
        }

        __c--;
    }

    return __result;
}

char *strstr(const char *__in, const char *__str)
{
    char __c;

    unsigned int __len;

    __c = *__str++;

    if (!__c)
    {
        return (char *) __in;
    }

    __len = strlen(__str);

    do
    {
        char __sc;

        do
        {
            __sc = *__in++;

            if (!__sc)
            {
                return (char *) 0;
            }
        } while (__sc != __c);
    } while (strncmp(__in, __str, __len) != 0);

    return (char *) (__in - 1);
}

char *strsep(char **__stringp, const char *__delim)
{
    const char *__spanp;

    char *__tok;
    char *__s;

    int __c, __sc;

    if ((__s = *__stringp) == NULL)
    {
        return NULL;
    }

    for (__tok = __s;;)
    {
        __c = *__s++;

        __spanp = __delim;

        do
        {
            if ((__sc = *__spanp++) == __c)
            {
                if (__c == 0)
                {
                    __s = NULL;
                }
                else
                {
                    __s[-1] = 0;
                }

                *__stringp = __s;

                return __tok;
            }
        } while (__sc != 0);
    }

    return NULL;
}

__list_t *list_strtok(const char *__str, const char *__delim, unsigned int *__ntok)
{
    __list_t *__ret = list_create();

    char *__s     = strdup(__str);
    char *__token;
    char *__rest  = __s;

    while ((__token = strsep(&__rest, __delim)) != NULL)
    {
        if (!strcmp(__token, "."))
        {
            continue;
        }
        
        if (!strcmp(__token, ".."))
        {
            if (list_size(__ret) > 0)
            {
                list_pop(__ret);
            }

                            continue;
        }

        list_push(__ret, strdup(__token));

        if (__ntok)
        {
            (*__ntok)++;
        }
    }

    kfree(__s);

    return __ret;
}

char *list_to_str(__list_t *__list, const char *__delim)
{
    char *__ret = kmalloc(256);

    kmemset(__ret, 0, 256);

    int __len  = 0;
    int __rlen = 256;

    while (list_size(__list) > 0)
    {
        char *__temp = list_pop(__list)->__val;

        int __tlen = strlen(__temp);

        if (__len + __tlen + 2 > __rlen)
        {
            __rlen = __rlen * 2;

            __ret = krealloc(__ret, __rlen);

            __len = __len + __tlen + 1;
        }

        kstrcat(__ret, __delim);
        kstrcat(__ret, __temp);
    }

    return __ret;
}