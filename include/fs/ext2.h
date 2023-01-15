#ifndef EXT2_H
#define EXT2_H

#define EXT2_MAGIC 0xEF53

#define EXT2_CREATOR_ID_LINUX    0	
#define EXT2_CREATOR_ID_GNU_HURD 1
#define EXT2_CREATOR_ID_MASIX    2	
#define EXT2_CREATOR_ID_FREEBSD  3	
#define EXT2_CREATOR_ID_OTHER    4	

typedef struct ext2_superblock_t {  
    unsigned int	 total_inodes;
    unsigned int	 total_blocks;
    unsigned int	 number_of_blocks;
    unsigned int	 unallocated_blocks;
    unsigned int	 unallocated_inodes;
    unsigned int	 block_number;
	unsigned int	 log_block_size;		
	unsigned int	 log_frag_size;
    unsigned int	 block_per_group;
    unsigned int	 fragments_per_group;
    unsigned int	 inodes_per_group;
    unsigned int	 mount_time;
    unsigned int	 written_time;
    unsigned short	 mnt_count;				
	unsigned short	 max_mnt_count;			
	unsigned short	 magic;					
	unsigned short	 state;
	unsigned short	 errors;
	unsigned short	 minor_rev_level;
	unsigned int	 lastcheck;
	unsigned int	 checkinterval;
	unsigned int	 creator_os;
	unsigned int	 rev_level;
	unsigned short	 def_resuid;
	unsigned short	 def_resgid;
} ext2_superblock_t;

void ext2_superblock_read();

void quantum_ext2_init();

#endif