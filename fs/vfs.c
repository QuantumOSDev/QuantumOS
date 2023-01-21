/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <core/string.h>
#include <core/errno.h>
#include <core/print.h>
#include <core/gtree.h>
#include <core/list.h>

#include <sys/memory.h>

#include <fs/ext2.h>
#include <fs/vfs.h>

__gtree_t    *__vfs_tree;
__vfs_node_t *__vfs_root;

unsigned int vfs_get_file_size(__vfs_node_t *node)
{
    if (node && node->get_file_size)
    {
        return node->get_file_size(node);
    }

    return 0;
}

unsigned int vfs_read(__vfs_node_t *__node, unsigned int __offset, unsigned int __size, char *__buffer)
{
    if (__node && __node->read)
    {
        unsigned int __ret = __node->read(__node, __offset, __size, __buffer);

        return __ret;
    }

    return -1;
}

unsigned int vfs_write(__vfs_node_t *__node, unsigned int __offset, unsigned int __size, char *__buffer)
{
    if (__node && __node->read)
    {
        unsigned int __ret = __node->write(__node, __offset, __size, __buffer);

        return __ret;
    }

    return -1;
}

void vfs_open(__vfs_node_t *__node, unsigned int __flags)
{
    if (!__node)
    {
        return;
    }

    if (__node->__refcount >= 0)
    {
        __node->__refcount++;
    }

    __node->open(__node, __flags);
}

void vfs_close(__vfs_node_t *__node)
{
    if (!__node || __node == __vfs_root || __node->__refcount == -1)
    {
        return;
    }

    __node->__refcount--;

    if (__node->__refcount == 0)
    {
        __node->close(__node);
    }
}

__vfs_node_t *vfs_finddir(__vfs_node_t *node, char *name)
{
    return NULL;
}

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

    __vfs_node_t *__parent_node = file_open(__parent, 0);

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

    __vfs_node_t *__parent_node = file_open(__parent, 0);

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

__vfs_node_t *file_open(const char *__file, unsigned int __flags)
{
    char *__curr_token = NULL;
    char *__filename   = strdup(__file);
    char *__ffilename  = __filename;
    char *__sfilename  = strdup(__filename);
    char *__ofilename  = __filename;
    char *__nfilename  = NULL;

    __vfs_node_t *__next  = NULL;
    __vfs_node_t *__start = vfs_get_mountpoint(&__filename);

    if (!__start)
    {
        return NULL;
    }

    if (__filename)
    {
        __nfilename = strstr(__sfilename + (__filename - __ofilename), __filename);
    }

    while (__filename != NULL && ((__curr_token = strsep(&__nfilename, "/")) != NULL))
    {
        __next = vfs_finddir(__start, __curr_token);

        if (!__next)
        {
            return NULL;
        }

        __start = __next;
    }

    if (!__next)
    {
        __next = __start;
    }

    vfs_open(__next, __flags);

    kfree(__sfilename);
    kfree(__ffilename);

    return __next;
}

char *expand_path(char *__input)
{
    __list_t *__list = list_strtok(__input, "/", NULL);

    return list_to_str(__list, "/");
}

int vfs_ioctl(__vfs_node_t *__node, int __request, void *__argp)
{
    if (!__node)
    {
        return -1;
    }

    if (__node->ioctl)
    {
        return __node->ioctl(__node, __request, __argp);
    }

    return ENOTTY;;
}

void vfs_chmod(__vfs_node_t *__node, unsigned int __mode)
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

    __vfs_node_t *__parent_node = file_open(__parent, 0);

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

int vfs_readlink(__vfs_node_t * node, char * buf, unsigned int size);

void vfs_init(void)
{
    __vfs_tree = gtree_create();

    struct __vfs_entry *__root = kmalloc(sizeof(struct __vfs_entry));

    __root->__name = strdup("root");
    __root->__file = NULL;

    gtree_insert(__vfs_tree, NULL, __root);
}

void vfs_mount(char *__path, __vfs_node_t *__fs)
{
    __fs->__refcount = -1;
    __fs->__fs_type  = 0;

    if (__path[0] == '/' && strlen(__path) == 1)
    {
        struct __vfs_entry *__ent = (struct __vfs_entry *) __vfs_tree->__root->__val;

        if (__ent->__file)
        {
            printf("Error: The path is already mounted, please unmount before mounting again\n");

            return;
        }

        __vfs_root = __fs;

        __ent->__file = __fs;

        return;
    }

    vfs_mount_recur(__path + 1, __vfs_tree->__root, __fs);
}

void vfs_register(char * name, __vfs_mount_callback callme)
{

}

void vfs_mount_dev(char *__mountpoint, __vfs_node_t *__node)
{
    vfs_mount(__mountpoint, __node);
}

void vfs_mount_recur(char *__path, __gtreenode_t *__subroot, __vfs_node_t *__fs)
{
    int __found = 0;

    char *__curr_token = strsep(&__path, "/");

    if (__curr_token == NULL || !strcmp(__curr_token, ""))
    {
        struct __vfs_entry *__ent = (struct __vfs_entry *) __subroot->__val;

        if (__ent->__file)
        {
            printf("Error: The path is already mounted, please unmount before mounting again\n");

            return;
        }

        if (!strcmp(__ent->__name, "/"))
        {
            __vfs_root = __fs;
        }

        __ent->__file = __fs;

        return;
    }

    FOREACH(__child, __subroot->__children)
    {
        __gtreenode_t *__tchild = (__gtreenode_t *) __child->__val;

        struct __vfs_entry *__ent = (struct __vfs_entry *) (__tchild->__val);

        if (strcmp(__ent->__name, __curr_token) == 0)
        {
            __found = 1;

            __subroot = __tchild;
        }
    }

    if (!__found)
    {
        struct __vfs_entry *__ent = kcalloc(1, sizeof(struct __vfs_entry));

        __ent->__name = strdup(__curr_token);

        __subroot = gtree_insert(__vfs_tree, __subroot, __ent);
    }

    vfs_mount_recur(__path, __subroot, __fs);
}

void print_vfstree();

void vfs_db_listdir(char *__name)
{
    __vfs_node_t *__node = file_open(__name, 0);

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

__vfs_node_t *vfs_get_mountpoint_recur(char **__path, __gtreenode_t *__subroot) {
    int __found = 0;

    char *__curr_token = strsep(__path, "/");

    if (__curr_token == NULL || !strcmp(__curr_token, "")) {
        struct __vfs_entry *__ent = (struct __vfs_entry *) __subroot->__val;

        return __ent->__file;
    }

    FOREACH(__child, __subroot->__children)
    {
        __gtreenode_t *__tchild = (__gtreenode_t *) __child->__val;

        struct __vfs_entry *__ent = (struct __vfs_entry *) (__tchild->__val);

        if (strcmp(__ent->__name, __curr_token) == 0) {
            __found = 1;

            __subroot = __tchild;

            break;
        }
    }

    if (!__found) {
        *__path = __curr_token;

        return ((struct __vfs_entry *) (__subroot->__val))->__file;
    }

    return vfs_get_mountpoint_recur(__path, __subroot);
}

__vfs_node_t *vfs_get_mountpoint(char **__path)
{
    if (strlen(*__path) > 1 && (*__path)[strlen(*__path) - 1] == '/')
    {
        *(__path)[strlen(*__path) - 1] = '\0';
    }

    if (!*__path || *(__path)[0] != '/')
    {
        return NULL;
    }

    if (strlen(*__path) == 1)
    {
        *__path = '\0';

        struct __vfs_entry *__ent = (struct __vfs_entry *) __vfs_tree->__root->__val;

        return __ent->__file;
    }

    (*__path)++;

    return vfs_get_mountpoint_recur(__path, __vfs_tree->__root);
}
