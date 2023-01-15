#include <core/string.h>
#include <core/print.h>

#include <sys/memory.h>

#include <fs/vfs.h>

__vfs_t *__vfs;

__vfs_dir_t *vfs_get_rootfs()
{
    return __vfs->__rootfs;
}

static inline void vfs_append_directory(__vfs_dir_t *__parent, __vfs_dir_t *__ndir)
{
    if (__parent->__num_directories == VFS_MAX_DIRECTORIES)
    {
        return;
    }

    __parent->__directories[__parent->__num_directories++] = __ndir;
}

static inline void vfs_append_file(__vfs_dir_t *__parent, __vfs_file_t *__nfile)
{
    if (__parent->__num_files == VFS_MAX_FILES)
    {
        return;
    }

    __parent->__files[__parent->__num_files++] = __nfile;
}

void vfs_initialize(__vfs_mode __mode)
{
    __vfs->__rootfs = kmalloc(sizeof(__vfs_dir_t));

    strcpy(__vfs->__rootfs->__name, "/");

    /* Set directory & file count to 0 */    

    __vfs->__rootfs->__num_directories = 0;
    __vfs->__rootfs->__num_files       = 0;

    /* Set directory descriptor */

    __vfs->__rootfs->__dir_descriptor = VFS_ROOT_FILE_DESCRIPTOR;

    /* Set initialized flag */
    __vfs->__initialized = __VFS_SUCCESS;

    /* Set mode flag */

    __vfs->__mode = __mode;

    /* Set FID & DID to 0 */
    
    __vfs->__fid = 0;
    __vfs->__did = 0;

    /* Create rootfs directories */

    vfs_append_directory(__vfs->__rootfs, vfs_create_dir(__vfs->__rootfs, "home"));
    vfs_append_directory(__vfs->__rootfs, vfs_create_dir(__vfs->__rootfs, "tmp"));
    vfs_append_directory(__vfs->__rootfs, vfs_create_dir(__vfs->__rootfs, "var"));
    vfs_append_directory(__vfs->__rootfs, vfs_create_dir(__vfs->__rootfs, "usr"));
}

__vfs_file_t *vfs_get_file_in_dir(__vfs_dir_t *__directory, char *__fname)
{
    for (int i = 0; i < __directory->__num_files; i++)
    {
        if (strcmp(__directory->__files[i]->__name, __fname) == 0)
        {
            return __directory->__files[i];
        }
    }
}

__vfs_file_t *vfs_get_file_by_desc(__vfs_dir_t *__directory, int __desc)
{
    for (int i = 0; i < __directory->__num_files; i++)
    {
        if (__directory->__files[i]->__file_descriptor == __desc)
        {
            return __directory->__files[i];
        }
    }
}

__vfs_dir_t *vfs_get_dir_in_dir(__vfs_dir_t *__directory, char *__dname)
{
    for (int i = 0; i < __directory->__num_directories; i++)
    {
        if (strcmp(__directory->__directories[i]->__name, __dname) == 0)
        {
            return __directory->__directories[i];
        }
    }
}

__vfs_dir_t *vfs_get_dir_by_path(__vfs_dir_t *__directory, char *__path)
{
    if (strcmp(__path, "."))
    {
        return __directory;
    }

    if (strcmp(__path, ".."))
    {
        return __directory->__parent;
    }

    if (strcmp(__path, "/"))
    {
        return __vfs->__rootfs;
    }

    for (int i = 0; i < __directory->__num_directories; i++)
    {
        if (strcmp(__directory->__directories[i]->__name, __path) == 0)
        {
            return __directory->__directories[i];
        }
    }

    for (int i = 0; i < __directory->__num_files; i++)
    {
        if (strcmp(__directory->__files[i]->__name, __path) == 0)
        {
            // printf("VFS: Error EOP is a file not a directory! [%s]\n", __path);

            return NULL;
        }
    }

    char *__current_directory = strtok(__path, "/");
    char *__final_directory   = strtok(NULL, "");

    if (__current_directory == NULL)
    {
        // printf("VFS: Error EOP no such file or directory! [%s]\n", __path);

        return NULL;
    }

    __vfs_dir_t *__next = vfs_get_dir_by_path(__directory, __current_directory);

    if (__next == NULL)
    {
        // printf("VFS: Error EOP no such file or directory! [%s]\n", __path);

        return NULL;
    }

    return vfs_get_dir_by_path(__next, __final_directory);
}

__vfs_dir_t *vfs_get_dir_by_desc(__vfs_dir_t *__directory, int __desc)
{
    for (int i = 0; i < __directory->__num_directories; i++)
    {
        if (__directory->__directories[i]->__dir_descriptor == __desc)
        {
            return __directory->__directories[i];
        }
    }
}

__vfs_file_t *vfs_create_file(__vfs_dir_t *__directory, char *__fname)
{
    for (int i = 0; i < __directory->__num_files; i++)
    {
        if (strcmp(__directory->__files[i]->__name, __fname) == 0)
        {
            // printf("VFS: Error EOP file already exists! [%s]\n", __fname);

            return NULL;
        }
    }

    for (int i = 0; i < __directory->__num_directories; i++)
    {
        if (strcmp(__directory->__directories[i]->__name, __fname) == 0)
        {
            // printf("VFS: Error EOP a directory with this name already exists! [%s]\n", __fname);

            return NULL;
        }
    }

    if (__directory->__num_files == VFS_MAX_FILES)
    {
        // printf("VFS: Error directory is full! [%s]\n", __directory->__name);

        return NULL;
    }

    __vfs_file_t *__nfile = kmalloc(sizeof(__vfs_file_t *));

    strcpy(__nfile->__name, __fname);

    __nfile->__data = kcalloc(1, sizeof(char *));
    __nfile->__data = "\0";

    __nfile->__parent = __directory;
    
    __nfile->__file_descriptor = __vfs->__fid++;

    vfs_append_file(__directory, __nfile);

    return __nfile;

}
__vfs_dir_t *vfs_create_dir(__vfs_dir_t *__directory, char *__dname)
{   
    for (int i = 0; i < __directory->__num_directories; i++)
    {
        if (strcmp(__directory->__directories[i]->__name, __dname) == 0)
        {
            // printf("VFS: Error EOP directory already exists! [%s]\n", __dname);

            return NULL;
        }
    }

    for (int i = 0; i < __directory->__num_files; i++)
    {
        if (strcmp(__directory->__files[i]->__name, __dname) == 0)
        {
            // printf("VFS: Error EOP a file with this name already exists! [%s]\n", __dname);

            return NULL;
        }
    }

    if (__directory->__num_directories == VFS_MAX_DIRECTORIES)
    {
        // printf("VFS: Error directory is full! [%s]\n", __directory->__name);

        return NULL;
    }

    __vfs_dir_t *__ndir = kmalloc(sizeof(__vfs_file_t *));

    strcpy(__ndir->__name, __dname);

    __ndir->__parent = __directory;
    
    __ndir->__dir_descriptor = __vfs->__did++;

    vfs_append_directory(__directory, __ndir);

    return __ndir;
}

__vfs_status vfs_putchar(int __c, __vfs_file_t *__file)
{
    int __flen = strlen(__file->__data);

    __file->__data = krealloc(__file->__data, __flen + 2);

    if (__file->__data == NULL)
    {
        return __VFS_ERROR;
    }

    __file->__data[__flen]     = __c;
    __file->__data[__flen + 1] = '\0';

    return __VFS_SUCCESS;
}

__vfs_status vfs_write(const void *__buffer, int __size, int __count, __vfs_file_t *__file)
{
    __file->__data = (char *) krealloc(__file->__data, (__size * __count) + 1);
    __file->__data = (char *) __buffer;
    __file->__data_size = __size;
    __file->__data_count = __count;

    return __VFS_SUCCESS;
}

__vfs_status vfs_read(const void *__buffer, int __size, int __count, __vfs_file_t *__file)
{
    int __total_size = __size * __count;
    int __n = 0;

    while (__n < __total_size)
    {
        int __c = __file->__data[__n];

        if (__c == '\0')
        {
            break;
        }

        *((char *) __buffer + __n) = __c;

        __n++;
    }

    return __VFS_SUCCESS;
}