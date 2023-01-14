#ifndef EXT2_H
#define EXT2_H

#define EXT2_MAGIC 0xEF53

#define EXT2_CREATOR_ID_LINUX    0	
#define EXT2_CREATOR_ID_GNU_HURD 1
#define EXT2_CREATOR_ID_MASIX    2	
#define EXT2_CREATOR_ID_FREEBSD  3	
#define EXT2_CREATOR_ID_OTHER    4	

typedef struct ext2_superblock_t {  
    int total_inodes;
    int total_blocks;
    int number_of_blocks;
    int unallocated_blocks;
    int unallocated_inodes;
    int block_number;
	int log_block_size;		
	int log_frag_size;
    int block_per_group;
    int fragments_per_group;
    int inodes_per_group;
    int mount_time;
    int written_time;
    short mnt_count;				
	short max_mnt_count;			
	short magic;					
	short state;
	short errors;
	short minor_rev_level;
	int lastcheck;
	int checkinterval;
	int creator_os;
	int rev_level;
	short def_resuid;
	short def_resgid;
} ext2_superblock_t;

void ext2_superblock_read();

void qunatum_ext2_init();

#endif