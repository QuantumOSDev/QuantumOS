#ifndef EXT2_H
#define EXT2_H

#define HARDDISK 0

#define EXT2_MAGIC 0xEF53

#define EXT2_CREATOR_ID_LINUX    0
#define EXT2_CREATOR_ID_GNU_HURD 1
#define EXT2_CREATOR_ID_MASIX    2
#define EXT2_CREATOR_ID_FREEBSD  3
#define EXT2_CREATOR_ID_OTHER    4

#define EXT2_FILE_SYSTEM_CLEAN   1
#define EXT2_FILE_SYSTEM_ERROR   2

#pragma pack(push)
#pragma push(1)

typedef struct __ext2_optional_features_flags {
    unsigned char preallocate_for_directories : 1;
    unsigned char afs_server_inodes : 1;
    unsigned char has_journaling : 1;
    unsigned char inodes_have_extended_attributes : 1;
    unsigned char can_be_resized : 1;
    unsigned char directories_use_hash_index : 1;
} __ext2_optional_features_flags;

typedef struct __ext2_required_features_flags {
    unsigned char compressed : 1;
    unsigned char directories_has_types : 1;
    unsigned char needs_to_replay_journal : 1;
    unsigned char uses_journal_device : 1;
} __ext2_required_features_flags;

typedef struct __ext2_read_only_features_flags {
    unsigned char sparse_sb_and_gdt : 1;
    unsigned char uses_64bit_file_size : 1;
    unsigned char directories_contents_are_binary_tree : 1;
} __ext2_read_only_features_flags;

typedef struct __ext2_superblock_t
{
    unsigned int __ninodes;
    unsigned int __nblocks;
    unsigned int __nrblocks;
    unsigned int __free_blocks;
    unsigned int __free_inodes;
    unsigned int __dblock;
    unsigned int __block_size;
    unsigned int __frag_size;
    unsigned int __gblocks;
    unsigned int __gfrags;
    unsigned int __ginodes;
    unsigned int __mtime;
    unsigned int __wtime;

    unsigned short __times_mounted; 
    unsigned short __allowed_times_mounted;
    unsigned short __ext2_signature;
    unsigned short __filesystem_state;
    unsigned short __on_error;
    unsigned short __minor_version;
    unsigned int   __last_fsck;
    unsigned int   __time_between_fsck; 
    unsigned int   __os_id;
    unsigned int   __major_version;
    unsigned short __uid;
    unsigned short __gid;

    unsigned int   __first_non_reserved_inode;
    unsigned short __inode_size;
    unsigned short __sb_block_group;

    __ext2_required_features_flags   __optional_features;
    __ext2_required_features_flags   __required_features;
    __ext2_read_only_features_flags  __read_only_features;

    unsigned char  __fsid[16];
    unsigned char  __name[16];
    unsigned char  __path_last_mounted_to[64];
    unsigned int   __compression_algorithms;
    unsigned char  __blocks_to_preallocate_for_files;
    unsigned char  __blocks_to_preallocate_for_directories;
    unsigned short ____unused__;
    unsigned char  __jid[16];
    unsigned int   __jinode;    
    unsigned int   __jdevice;  
    unsigned int   __head_of_orphan_inode_list;  
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
} __ext2_block_buf_t;

typedef struct __ext2_fs_t
{
    __ext2_superblock_t* __sb;
    __ext2_bg_desc_t* __bgs_desc;

    unsigned int __bgd_blocks;
    unsigned int __total_groups;

    __ext2_inode_t* __root_inode;

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

void ext2_read_superblock(__ext2_superblock_t* ext2_superblock);

char* ext2_creator_os(unsigned int creator_os);

void quantum_ext2_init();

#endif
