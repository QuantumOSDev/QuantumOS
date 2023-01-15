#ifndef VFS_H
#define VFS_H

#define VFS_MAX_FILES       100
#define VFS_MAX_DIRECTORIES 100
#define VFS_MAX_NAME_LENGTH 256

#define VFS_ROOT_FILE_DESCRIPTOR 0

#define VFS_EFILE_NOT_FOUND        -1
#define VFS_EFILE_EXISTS           -2
#define VFS_EFILE_EXCEEDS_BOUNDARY -3

typedef enum
{
    __VFS_SUCCESS,
    __VFS_ERROR,
    __VFS_PANIC,
} __vfs_status;

typedef enum
{
    __VFS_RO,
    __VFS_RW,
} __vfs_mode;

typedef struct __vfs_dir_t __vfs_dir_t;

typedef struct __vfs_file_t
{
    char __name[VFS_MAX_NAME_LENGTH];

    char *__data;

    __vfs_dir_t *__parent;

    int __data_size;
    int __data_count;
    
    int __file_descriptor;
} __vfs_file_t;

typedef struct __vfs_dir_t
{
    char __name[VFS_MAX_NAME_LENGTH];

    __vfs_file_t *__files[VFS_MAX_FILES];

    struct __vfs_dir_t *__directories[VFS_MAX_DIRECTORIES];
    struct __vfs_dir_t *__parent;

    int __dir_descriptor;
    int __num_directories;
    int __num_files;
} __vfs_dir_t;

typedef struct __vfs_t
{
    unsigned char __initialized;
    unsigned char __mode;

    unsigned int __fid;
    unsigned int __did;

    __vfs_dir_t *__rootfs;
} __vfs_t;

void vfs_initialize(__vfs_mode __mode);

__vfs_dir_t *vfs_get_rootfs();

__vfs_file_t *vfs_get_file_in_dir(__vfs_dir_t *__directory, char *__fname);
__vfs_file_t *vfs_get_file_by_desc(__vfs_dir_t *__directory, int __desc);

__vfs_dir_t *vfs_get_dir_in_dir(__vfs_dir_t *__directory, char *__dname);
__vfs_dir_t *vfs_get_dir_by_path(__vfs_dir_t *__directory, char *__path);
__vfs_dir_t *vfs_get_dir_by_desc(__vfs_dir_t *__directory, int __desc);

__vfs_file_t *vfs_create_file(__vfs_dir_t *__directory, char *__fname);
__vfs_dir_t *vfs_create_dir(__vfs_dir_t *__directory, char *__dname);

__vfs_status vfs_putchar(int __c, __vfs_file_t *__file);
__vfs_status vfs_write(const void *__buffer, int __size, int __count, __vfs_file_t *__file);
__vfs_status vfs_read(const void *__buffer, int __size, int __count, __vfs_file_t *__file);

#endif