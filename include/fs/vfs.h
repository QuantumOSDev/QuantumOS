#ifndef VFS_H
#define VFS_H

#define VFS_PATH_SEPERATOR '/'
#define VFS_PATH_UP ".."
#define VFS_PATH_DOT "."

#define VFS_EXT2_MAGIC 0xeeee2222

#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		     5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */

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

#define     _IFMT   0170000 /* type of file */
#define     _IFDIR  0040000 /* directory */
#define     _IFCHR  0020000 /* character special */
#define     _IFBLK  0060000 /* block special */
#define     _IFREG  0100000 /* regular */
#define     _IFLNK  0120000 /* symbolic link */
#define     _IFSOCK 0140000 /* socket */
#define     _IFIFO  0010000 /* fifo */

struct vfs_node;

typedef unsigned int (*get_file_size_callback)(struct vfs_node * node);
typedef unsigned int (*read_callback) (struct vfs_node *, unsigned int, unsigned int, char *);
typedef unsigned int (*write_callback) (struct vfs_node *, unsigned int, unsigned int, char *);
typedef void (*open_callback) (struct vfs_node*, unsigned int flags);
typedef void (*close_callback) (struct vfs_node *);
typedef struct dirent *(*readdir_callback) (struct vfs_node *, unsigned int);
typedef struct vfs_node *(*finddir_callback) (struct vfs_node *, char *name);
typedef void (*create_callback) (struct vfs_node *, char *name, unsigned short permission);
typedef void (*unlink_callback) (struct vfs_node *, char *name);
typedef void (*mkdir_callback) (struct vfs_node *, char *name, unsigned short permission);
typedef int (*ioctl_callback) (struct vfs_node *, int request, void * argp);
typedef int (*get_size_callback) (struct vfs_node *);
typedef void (*chmod_callback) (struct vfs_node *, unsigned int mode);
typedef char ** (*listdir_callback) (struct vfs_node *);

typedef struct vfs_node {
    // Baisc information about a file(note: in linux, everything is file, so the vfs_node could be used to describe a file, directory or even a device!)
    char name[256];
    void * device;
    unsigned int mask;
    unsigned int uid;
    unsigned int gid;
    unsigned int flags;
    unsigned int inode_num;
    unsigned int size;
    unsigned int fs_type;
    unsigned int open_flags;
    // Time
    unsigned int create_time;
    unsigned int access_time;
    unsigned int modified_time;

    unsigned int offset;
    unsigned nlink;
    int refcount;

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
}vfs_node_t;

struct dirent {
    char name[256];
    unsigned int inode_num;
};

typedef struct vfs_entry {
    char * name;
    vfs_node_t * file;
}vfs_entry_t;


unsigned int vfs_get_file_size(vfs_node_t * node);

unsigned int vfs_read(vfs_node_t *node, unsigned int offset, unsigned int size, char *buffer);

unsigned int vfs_write(vfs_node_t *node, unsigned int offset, unsigned int size, char *buffer);

void vfs_open(struct vfs_node *node, unsigned int flags);

void vfs_close(vfs_node_t *node);

vfs_node_t *vfs_finddir(vfs_node_t *node, char *name);

void vfs_mkdir(char *name, unsigned short permission);

void vfs_mkfile(char *name, unsigned short permission);

int vfs_create_file(char *name, unsigned short permission);

vfs_node_t *file_open(const char *file_name, unsigned int flags);

char *expand_path(char *input);

int vfs_ioctl(vfs_node_t *node, int request, void * argp);

void vfs_chmod(vfs_node_t *node, unsigned int mode);

void vfs_unlink(char * name);

int vfs_symlink(char * value, char * name);

int vfs_readlink(vfs_node_t * node, char * buf, unsigned int size);

void vfs_init();

void vfs_mount(char * path, vfs_node_t * local_root);

typedef vfs_node_t * (*vfs_mount_callback)(char * arg, char * mountpoint);

void vfs_register(char * name, vfs_mount_callback callme);

void vfs_mount_dev(char * mountpoint, vfs_node_t * node);

void print_vfstree();

void vfs_db_listdir(char * name);

#endif
