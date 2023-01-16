#include <fs/ext2.h>
#include <fs/ata.h>

#include <quantum/init.h>
#include <sys/memory.h>

<<<<<<< HEAD
void ext2_superblock_read(ext2_superblock_t* ext2_superblock)
{
    // ata_read_sectors(1, 2, 1, (unsigned int *)(ext2_superblock));
    //ata_read_sectors(1, 3, 1, (unsigned int*)(ext2_superblock + 512));
}
=======
static ext2_superblock_t* ext2_superblock;
>>>>>>> fc4fb24e70455f09e02db0624ec91342826b0051

void quantum_ext2_init() 
{
    /* Allocate space */

    unsigned int *__superblock_buffer = kmalloc(sizeof(*__superblock_buffer));

<<<<<<< HEAD
    ata_read_sectors(0, 1, 2, (unsigned int *)(ext2_superblock));
=======
    ata_read_sectors(HARDDISK, 1, 2, (unsigned int *) ext2_superblock);
>>>>>>> fc4fb24e70455f09e02db0624ec91342826b0051

    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "\tTotal inodes: %d", ext2_superblock->total_inodes);
    quantum_info(0, " Ext2   ", "\tTotal blocks: %d", ext2_superblock->total_blocks);
    quantum_info(0, " Ext2   ", "\tMagic: 0x%x", ext2_superblock->magic);
    quantum_info(0, " Ext2   ", "\tCreator OS: %d", ext2_superblock->creator_os);
} 