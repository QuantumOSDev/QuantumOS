#include <fs/ext2.h>
#include <fs/ata.h>

#include <quantum/init.h>
#include <sys/memory.h>

static ext2_superblock_t* ext2_superblock;

void quantum_ext2_init() 
{
    /* Allocate space */

    unsigned int *__superblock_buffer = kmalloc(sizeof(*__superblock_buffer));

    ata_read_sectors(HARDDISK, 1, 2, (unsigned int *) ext2_superblock);

    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "\tTotal inodes: %d", ext2_superblock->total_inodes);
    quantum_info(0, " Ext2   ", "\tTotal blocks: %d", ext2_superblock->total_blocks);
    quantum_info(0, " Ext2   ", "\tMagic: 0x%x", ext2_superblock->magic);
    quantum_info(0, " Ext2   ", "\tCreator OS: %d", ext2_superblock->creator_os);
} 