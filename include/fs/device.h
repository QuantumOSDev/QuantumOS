#ifndef DEVICE_H
#define DEVICE_H

#include <fs/vfs.h>

typedef enum __device_type
{
    DEVICE_UNKNOWN = 0,
    DEVICE_CHAR    = 1,
    DEVICE_BLOCK   = 2,
} __device_type;

typedef struct __device_t
{
    char *__name;

    unsigned int __unique_id;

    __device_type __dev_type;

    __vfs_node_t *__fs;

    unsigned char (*__read)(unsigned char *__buffer, unsigned int __offset, unsigned int __len, void *__device);
    unsigned char (*__write)(unsigned char *__buffer, unsigned int __offset, unsigned int __len, void *__device);

    void *__private;
} __device_t;

void device_print(void);
void device_init(void);

int device_add(__device_t *__device);

__device_t *device_get(unsigned int __id);
__device_t *device_get_by_id(unsigned int __id);

int device_get_number(void);

#endif
