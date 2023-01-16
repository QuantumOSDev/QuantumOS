#include <fs/ext2.h>
#include <fs/ata.h>

#include <quantum/init.h>
#include <sys/memory.h>

void quantum_ext2_init() 
{
    /* Allocate space */

    unsigned int *__superblock_buffer = kmalloc(sizeof(*__superblock_buffer));

    ata_read_sectors(HARDDISK, 1, 2, (unsigned int *) __superblock_buffer);

    __ext2_superblock_t* __ext_superblock = (__ext2_superblock_t*)__superblock_buffer;

    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "\tTotal inodes: %d", __ext_superblock->__icount);
    quantum_info(0, " Ext2   ", "\tTotal blocks: %d", __ext_superblock->__bcount);
    quantum_info(0, " Ext2   ", "\tMagic: 0x%x", __ext_superblock->__magic);
    quantum_info(0, " Ext2   ", "\tCreator OS: %d", __ext_superblock->__creator);
    quantum_info(0, " Ext2   ", "\tVName: %s", __ext_superblock->__vname);
    quantum_info(0, " Ext2   ", "\tUUID: %s", __ext_superblock->__uuid);
} 