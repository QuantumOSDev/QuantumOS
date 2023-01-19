#ifndef VFS_H
#define VFS_H

#include <core/gtree.h>
#include <core/list.h>

#define VFS_PATH_SEPERATOR '/'
#define VFS_PATH_UP ".."
#define VFS_PATH_DOT "."

#define VFS_EXT2_MAGIC 0xeeee2222

#define O_RDONLY     0x0000
#define O_WRONLY     0x0001
#define O_RDWR       0x0002
#define O_APPEND     0x0008
#define O_CREAT      0x0200
#define O_TRUNC      0x0400
#define O_EXCL       0x0800
#define O_NOFOLLOW   0x1000
#define O_PATH       0x2000

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x04
#define FS_BLOCKDEVICE 0x08
#define FS_PIPE        0x10
#define FS_SYMLINK     0x20
#define FS_MOUNTPOINT  0x40
#define FS_SOCKET      0x80000000

#define     _IFMT   0170000 /* type of file */
#define     _IFDIR  0040000 /* directory */
#define     _IFCHR  0020000 /* character special */
#define     _IFBLK  0060000 /* block special */
#define     _IFREG  0100000 /* regular */
#define     _IFLNK  0120000 /* symbolic link */
#define     _IFSOCK 0140000 /* socket */
#define     _IFIFO  0010000 /* fifo */

struct __vfs_node;

#include <net/socket.h>

typedef unsigned int (*get_file_size_callback)  (struct __vfs_node *__node);
typedef unsigned int (*read_callback)           (struct __vfs_node *, unsigned int, unsigned int, char *);
typedef unsigned int (*write_callback)          (struct __vfs_node *, unsigned int, unsigned int, char *);
typedef void (*open_callback)                   (struct __vfs_node *, unsigned int __flags);
typedef void (*close_callback)                  (struct __vfs_node *);
typedef struct dirent *(*readdir_callback)      (struct __vfs_node *, unsigned int);
typedef struct vfs_node *(*finddir_callback)    (struct __vfs_node *, char *__name);
typedef void (*create_callback)                 (struct __vfs_node *, char *__name, unsigned short __permission);
typedef void (*unlink_callback)                 (struct __vfs_node *, char *__name);
typedef void (*mkdir_callback)                  (struct __vfs_node *, char *__name, unsigned short __permission);
typedef int (*ioctl_callback)                   (struct __vfs_node *, int __request, void *__argp);
typedef int (*get_size_callback)                (struct __vfs_node *);
typedef void (*chmod_callback)                  (struct __vfs_node *, unsigned int __mode);
typedef char ** (*listdir_callback)             (struct __vfs_node *);

typedef struct __vfs_node {
    // Baisc information about a file(note: in linux, everything is file, so the vfs_node could be used to describe a file, directory or even a device!)
    char __name[256];
    
    void *__device;
    
    unsigned int __mask;
    unsigned int __uid;
    unsigned int __gid;
    unsigned int __flags;
    unsigned int __inode_num;
    unsigned int __size;
    unsigned int __fs_type;
    unsigned int __open_flags;
    // Time
    unsigned int __create_time;
    unsigned int __access_time;
    unsigned int __modified_time;

    unsigned int __offset;

    unsigned __nlink;
    
    int __refcount;

    socket_t* socket;

    // File operations
    read_callback read;
    write_callback write;
    open_callback open;
    close_callback close;
    readdir_callback readdir;
    finddir_callback finddir;
    create_callback create;
    unlink_callback unlink;
    mkdir_callback mkdir;
    ioctl_callback ioctl;
    get_size_callback get_size;
    chmod_callback chmod;
    get_file_size_callback get_file_size;

    listdir_callback listdir;
} __vfs_node_t;

struct dirent {
    char __name[256];
    
    unsigned int __inode_num;
};

typedef struct __vfs_entry {
    char *__name;
    
    __vfs_node_t *__file;
} __vfs_entry_t;

unsigned int vfs_get_file_size(__vfs_node_t *node);

unsigned int vfs_read(__vfs_node_t *__node, unsigned int __offset, unsigned int __size, char *__buffer);
unsigned int vfs_write(__vfs_node_t *__node, unsigned int __offset, unsigned int __size, char *__buffer);

void vfs_open(__vfs_node_t *__node, unsigned int __flags);
void vfs_close(__vfs_node_t *__node);

__vfs_node_t *vfs_finddir(__vfs_node_t *node, char *name);

void vfs_mkdir(char *__name, unsigned short __permission);

void vfs_mkfile(char *__name, unsigned short __permission);

int vfs_create_file(char *__name, unsigned short __permission);

__vfs_node_t *file_open(const char *__file, unsigned int __flags);

char *expand_path(char *__input);

int vfs_ioctl(__vfs_node_t *__node, int __request, void *__argp);

void vfs_chmod(__vfs_node_t *__node, unsigned int __mode);

void vfs_unlink(char *__name);

int vfs_symlink(char *__value, char *__name);
int vfs_readlink(__vfs_node_t *node, char *__buf, unsigned int __size);

void vfs_init(void);
void vfs_mount(char *__path, __vfs_node_t *__fs);

typedef __vfs_node_t *(* __vfs_mount_callback) (char *__arg, char *__mountpoint);

void vfs_register(char *__name, __vfs_mount_callback __caller);
void vfs_mount_dev(char *_mountpoint, __vfs_node_t *_node);
void vfs_mount_recur(char *__path, __gtreenode_t *__subroot, __vfs_node_t *__fs);

void print_vfstree(void);
void vfs_db_listdir(char *__name);

__vfs_node_t *vfs_get_mountpoint_recur(char **__path, __gtreenode_t *__subroot);
__vfs_node_t *vfs_get_mountpoint(char **__path);


#endif
