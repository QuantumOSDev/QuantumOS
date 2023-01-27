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

typedef struct ext2_superblock_t {
    unsigned int   total_inodes;
    unsigned int   total_blocks;
    unsigned int   su_blocks;
    unsigned int   free_blocks;
    unsigned int   free_inodes;
    unsigned int   superblock_idx;
    unsigned int   log2block_size;
    unsigned int   log2frag_size;
    unsigned int   blocks_per_group;
    unsigned int   frags_per_group;
    unsigned int   inodes_per_group;
    unsigned int   mtime;
    unsigned int   wtime;
    unsigned short mount_count;
    unsigned short mount_allowed_count;
    unsigned short ext2_magic;
    unsigned short fs_state;
    unsigned short err;
    unsigned short minor;
    unsigned int   last_check;
    unsigned int   interval;
    unsigned int   os_id;
    unsigned int   major;
    unsigned short r_userid;
    unsigned short r_groupid;
    unsigned int   first_inode;
    unsigned short inode_size;
    unsigned short superblock_group;
    unsigned int   optional_feature;
    unsigned int   required_feature;
    unsigned int   readonly_feature;
    char           fs_id[16];
    char           vol_name[16];
    char           last_mount_path[64];
    unsigned int   compression_method;
    unsigned char  file_pre_alloc_blocks;
    unsigned char  dir_pre_alloc_blocks;
    unsigned short unused1;
    char           journal_id[16];
    unsigned int   journal_inode;
    unsigned int   journal_device;
    unsigned int   orphan_head;
} ext2_superblock_t;

typedef struct ext2_bgd_t {
    unsigned int   addrblockbmp;
    unsigned int   addrinodebmp;
    unsigned int   inodetable;
    unsigned short unallocb;
    unsigned short unalloci;
    unsigned short dircnt;
    unsigned short unused[7];
} ext2_bgd_t;

typedef struct ext2_inode_t {
    unsigned short perms;
    unsigned short uid;
    unsigned int   sizelo;
    unsigned int   accesstime;
    unsigned int   creationtime;
    unsigned int   modifiedtime;
    unsigned int   deletedtime;
    unsigned short gid;
    unsigned short hardlinkcnt;
    unsigned int   sectors;
    unsigned int   flags;
    unsigned int   oss;
    unsigned int   blocks[15];
    unsigned int   gennum;
    unsigned int   eab;
    unsigned int   sizehi;
    unsigned int   fragaddr;
    unsigned char  reserved[12];
} ext2_inode_t;

typedef struct ext2_direntry_t {
    unsigned int inode;
    unsigned int direntsize;
    unsigned int namelen;
    unsigned int type;
    char*        name;
} ext2_direntry_t;

typedef struct ext2_t {
    ext2_inode_t*          root_inode;
    ext2_superblock_t*     superblock;
    ext2_bgd_t* bgds;

    unsigned int blocks_per_group;
    unsigned int inodes_per_group;
    unsigned int total_groups;

    unsigned int bgd_blocks;
} ext2_t;

void quantum_ext2_init();

ext2_inode_t* ext2_read_inode(int inode_idx);

void ext2_read_superblock(ext2_superblock_t* superblock);

void ext2_show_superblock_info(ext2_superblock_t* superblock);

char* ext2_creator_os(unsigned int creator_os);

#endif
