#include <fs/ext2.h>
#include <fs/ata.h>

#include <quantum/init.h>
#include <sys/memory.h>

void ext2_superblock_read(ext2_superblock_t* ext2_superblock)
{
    ata_read_sectors(1, 2, 1, (unsigned int*)(ext2_superblock));
    ata_read_sectors(1, 3, 1, (unsigned int*)(ext2_superblock + 512));
}

void qunatum_ext2_init() 
{
    ext2_superblock_t* ext2_superblock = (ext2_superblock_t*)kmalloc(sizeof(ext2_superblock_t));
    ext2_superblock_read(ext2_superblock);

    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "\tTotal inodes: %d", ext2_superblock->total_inodes);
    quantum_info(0, " Ext2   ", "\tTotal blocks: %d", ext2_superblock->total_blocks);
    quantum_info(0, " Ext2   ", "\tMagic: 0x%x", ext2_superblock->magic);
    quantum_info(0, " Ext2   ", "\tCreator OS: %d", ext2_superblock->creator_os);
}