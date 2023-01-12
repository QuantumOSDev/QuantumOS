#include <core/string.h>
#include <core/print.h>

#include <sys/memory.h>

int strlen(const char* str) {
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

char to_uppercase(char c) {
    return (char)c - 0x20;
}

char to_lowercase(char c) {
    return (char)c + 0x20;
}

void reverse(char* str) {
    int c, i, j;
    for (i = 0, j = strlen(str)-1; i < j; i++, j--) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

int strcmp(const char *s1, char *s2) {
    int i = 0;

    while ((s1[i] == s2[i])) {
        if (s2[i++] == 0)
            return 0;
    }
    return 1;
}

void pop_last_str(char* str) {
    int len = strlen(str);
    str[len-1] = '\0';
}

void append_str(char* str, char c) {
    int len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

char* strcpy(char* dest, const char* src) {
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

char* strtok(char* str, char* delim) {
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

char* strpbrk(const char* str, const char* accept) {
    for (int i = 0; str[i] != '\0'; i++) {
        for (int j = 0; accept[j] != '\0'; j++) {
            if (str[i] == accept[j])
                return ((char*)(str + i));
        }
    }
    return NULL;
}

char* strdup(const char* str) {
    char* copy = (char*)kmalloc(strlen(str) + 1);
    return kmemcpy((void*)copy, (const void*)str, strlen(str) + 1);
}

char* strrchr(const char* str, int c) {
    for (int i = strlen(str) - 1; i > 0; i--) {
        if (str[i] == c)
            return ((char*)(str + 1));
    }
    return NULL;
}

void itoa(int num, char* str, int base) {
    int i = 0;
    int is_neg = 0;
    
    if (num == 0) { 
        str[i++] = '0';
        str[i] = '\0';
        return;
    } 
    if (num < 0 && base == 10) {
        is_neg = 1;
        num = -num;
    }
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    if (is_neg)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str);
}

void ltoa(long long value, char* buf) {
    char* p;
    unsigned long long v;

    v = (value < 0) ? -value: value;
    p = buf + 31;
    do{
        *p -- = '0' + (v%10);
        v /= 10;
    } while(v);

    if(value < 0) *p-- = '-';
    p++;

    int len = 32 - (p - buf);
    char *s = (char*)kmalloc(sizeof(char) * (len + 1));
    kmemcpy(s, p, len);
    s[len] = '\0';
}