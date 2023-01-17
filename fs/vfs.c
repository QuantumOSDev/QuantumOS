#include <core/string.h>
#include <core/gtree.h>
#include <core/list.h>

#include <sys/memory.h>

#include <fs/ext2.h>
#include <fs/vfs.h>

__gtree_t *vfs_tree;
vfs_node_t *vfs_root;

unsigned int vfs_get_file_size(vfs_node_t *node)
{
    if (node && node->get_file_size)
    {
        return node->get_file_size(node);
    }

    return 0;
}

unsigned int vfs_read(vfs_node_t *__node, unsigned int __offset, unsigned int __size, char *__buffer)
{
    if (__node && __node->read)
    {
        unsigned int __ret = __node->read(__node, __offset, __size, __buffer);

        return __ret;
    }

    return -1;
}

unsigned int vfs_write(vfs_node_t *__node, unsigned int __offset, unsigned int __size, char *__buffer)
{
    if (__node && __node->read)
    {
        unsigned int __ret = __node->read(__node, __offset, __size, __buffer);

        return __ret;
    }

    return -1;
}

void vfs_open(struct vfs_node *__node, unsigned int __flags)
{
    if (!__node)
    {
        return;
    }

    if (__node->refcount >= 0)
    {
        __node->refcount++;
    }

    __node->open(__node, __flags);
}

void vfs_close(vfs_node_t *__node)
{
    if (!__node || __node == vfs_root || __node->refcount == -1)
    {
        return;
    }

    __node->refcount--;

    if (__node->refcount == 0)
    {
        __node->close(__node);
    }
}

vfs_node_t *vfs_finddir(vfs_node_t *node, char *name);

void vfs_mkdir(char *__name, unsigned short __permission)
{
    int i = kstrlen(__name);

    char *__dirname  = strdup(__name);
    char *__tdirname = __dirname;
    char *__parent   = "/";

    while (i >= 0)
    {
        if (__dirname[i] == '/')
        {
            if (i != 0)
            {
                __dirname[i] = '\0';

                __parent = __dirname;
            }

            __dirname = &__dirname[i + 1];

            break;
        }

        i--;
    }

    vfs_node_t *__parent_node = file_open(__parent, 0);

    if (!__parent_node)
    {
        kfree(__tdirname);
    }

    if (__parent_node->mkdir)
    {
        __parent_node->mkdir(__parent_node, __dirname, __permission);
    }

    kfree(__tdirname);

    vfs_close(__parent_node);
}

void vfs_mkfile(char *name, unsigned short permission);

int vfs_create_file(char *__name, unsigned short __permission)
{
    int i = kstrlen(__name);

    char *__dirname  = strdup(__name);
    char *__tdirname = __dirname;
    char *__parent   = "/";

    while (i >= 0)
    {
        if (__dirname[i] = '/')
        {
            if (i != 0)
            {
                __dirname[i] = '\0';

                __parent = __dirname;
            }

            __dirname = &__dirname[i + 1];

            break;
        }

        i--;
    }

    vfs_node_t *__parent_node = file_open(__parent, 0);

    if (!__parent_node)
    {
        kfree(__tdirname);

        return -1;
    }

    if (__parent_node->create)
    {
        __parent_node->create(__parent_node, __dirname, __permission);
    }

    kfree(__tdirname);

    vfs_close(__parent_node);

    return 0;
}

vfs_node_t *file_open(const char *file_name, unsigned int flags);

char *expand_path(char *__input)
{
    __list_t *__list = list_strtok(__input, "/", NULL);

    return list_to_string(__list, "/");
}

int vfs_ioctl(vfs_node_t *__node, int __request, void *__argp)
{
    if (!__node)
    {
        return -1;
    }

    if (__node->ioctl)
    {
        return __node->ioctl(__node, __request, __argp);
    }

    return ENOTTY;
}

void vfs_chmod(vfs_node_t *__node, unsigned int __mode)
{
    if (__node->chmod)
    {
        return __node->chmod(__node, __mode);
    }
}

void vfs_unlink(char *__name)
{
    int i = kstrlen(__name);

    char *__dirname  = strdup(__name);
    char *__tdirname = __dirname;
    char *__parent   = "/";

    while (i >= 0)
    {
        if (__dirname[i] = '/')
        {
            if (i != 0)
            {
                __dirname[i] = '\0';

                __parent = __dirname;
            }

            __dirname = &__dirname[i + 1];

            break;
        }

        i--;
    }

    vfs_node_t *__parent_node = file_open(__parent, 0);

    if (!__parent_node)
    {
        kfree(__tdirname);
    }

    if (__parent_node->unlink)
    {
        __parent_node->unlink(__parent_node, __dirname);
    }

    kfree(__tdirname);

    vfs_close(__parent_node);
}

int vfs_symlink(char * value, char * name);

int vfs_readlink(vfs_node_t * node, char * buf, unsigned int size);

void vfs_init();

void vfs_mount(char * path, vfs_node_t * local_root);

typedef vfs_node_t * (*vfs_mount_callback)(char * arg, char * mountpoint);

void vfs_register(char * name, vfs_mount_callback callme);

void vfs_mount_dev(char * mountpoint, vfs_node_t * node);

void print_vfstree();

void vfs_db_listdir(char *__name)
{
    vfs_node_t *__node = file_open(__name, 0);

    if (!__node)
    {
        printf("Could not list a directory that does not exist!\n");

        return;
    }

    if (!__node->listdir)
    {
        return;
    }

    char **__files = __node->listdir(__node);
    char **__temp  = __files;

    while (*__files)
    {
        printf("%s ", *__files);

        kfree(*__files);

        __files++;
    }

    kfree(__temp);

    printf("\n");
}
