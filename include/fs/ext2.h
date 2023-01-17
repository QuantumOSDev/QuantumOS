#ifndef EXT2_H
#define EXT2_H

#define HARDDISK 0

#define EXT2_MAGIC 0xEF53

#define EXT2_CREATOR_ID_LINUX    0
#define EXT2_CREATOR_ID_GNU_HURD 1
#define EXT2_CREATOR_ID_MASIX    2
#define EXT2_CREATOR_ID_FREEBSD  3
#define EXT2_CREATOR_ID_OTHER    4

typedef struct __ext2_superblock_t
{
    unsigned int __icount;
    unsigned int __bcount;
    unsigned int __rbcount;
    unsigned int __fbcount;
    unsigned int __first_db_lock;
    unsigned int __log_bsize;
    unsigned int __log_fsize;
    unsigned int __bpg;
    unsigned int __fpg;
    unsigned int __ipg;
    unsigned int __mtime;
    unsigned int __wtime;

    unsigned short __mnt_count;
    unsigned short __max_mnt_count;
    unsigned short __magic;
    unsigned short __state;
    unsigned short __beh;
    unsigned short __minrev;

    unsigned int __last_fsck;
    unsigned int __fsckintv;
    unsigned int __creator;
    unsigned int __rev;

    unsigned short __ruid;
    unsigned short __rgid;

    unsigned int __first_ino;

    unsigned short __inode_size;
    unsigned short __block_group_nr;

    unsigned int __features_incompat;
    unsigned int __features_rocompat;

    unsigned char __uuid[16];

    char __vname[16];
    char __fsmnt[16];

    unsigned int __algo;

    unsigned char __prealloc;
    unsigned char __dir_prealloc;

    unsigned short __reserved_ngbd;
} __ext2_superblock_t;

typedef struct __ext2_bg_desc_t
{
    unsigned int __b_bitmap;
    unsigned int __i_bitmap;
    unsigned int __i_tables;

    unsigned short __nfree_blocks;
    unsigned short __nfree_inodes;
    unsigned short __ndirs;
    unsigned short __reserved;

    unsigned int __reserved_2[3];
} __ext2_bg_desc_t;

typedef struct __ext2_inode_t
{
    unsigned short __mode;
    unsigned short __uid;

    unsigned int __size;
    unsigned int __atime;
    unsigned int __ctime;
    unsigned int __mtime;
    unsigned int __dtime;

    unsigned short __gid;
    unsigned short __nlink;

    unsigned int __nblock;
    unsigned int __flags;
    unsigned int __version;

    unsigned int __blocks[15];

    unsigned int __generation;
    unsigned int __facl;
    unsigned int __size_hi;
    unsigned int __faddr;

    unsigned short __nblock_hi;
    unsigned short __facl_hi;
    unsigned short __uid_high;
    unsigned short __gid_high;
    unsigned short __chksum_lo;
    unsigned short __reserved;
    unsigned short __isize;
    unsigned short __chksum_hi;

    unsigned int __x_ctime;
    unsigned int __x_mtime;
    unsigned int __x_atime;
    unsigned int __crtime;
    unsigned int __x_crtime;
    unsigned int __version_high;
} __ext2_inode_t;

typedef struct __ext2_dirent_t
{
    unsigned int __inode;

    unsigned short __size;

    unsigned char __name_len;
    unsigned char __type;

    char __name[];
} __ext2_dirent_t;

typedef struct __ext2_block_buf_t
{
    unsigned long __block_addr;

    unsigned char *__buffer;
}  __ext2_block_buf_t;

typedef struct __ext2_fs_t
{
    struct __ext2_superblock_t __sb;

    unsigned int __bsize;
    unsigned int __igp;
    unsigned int __bpg;
    unsigned int __inode_size;

    unsigned long __bg_start;
    unsigned long __bg_count;

    unsigned char *__block_cache;

    struct __ext2_block_buf_t __superblock_cache;
    struct __ext2_block_buf_t __block_ptr_buf;
    struct __ext2_block_buf_t __single_indirect_buf;
    struct __ext2_block_buf_t __double_indirect_buf;
} __ext2_fs_t;

void ext2_superblock_read();

void quantum_ext2_init();

#endif
