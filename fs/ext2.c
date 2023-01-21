/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <fs/ext2.h>
#include <fs/ata.h>

#include <quantum/init.h>
#include <sys/memory.h>

static __ext2_superblock_t *__ext2_superblock;

void quantum_ext2_init()
{
    /* Allocate space */

    unsigned int *__sb_buffer = kmalloc(sizeof(*__sb_buffer));

    //ata_read_sectors(HARDDISK, 1, 2, (unsigned int *) __ext2_superblock);

    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "\tTotal inodes: %d", __ext2_superblock->__icount);
    quantum_info(0, " Ext2   ", "\tTotal blocks: %d", __ext2_superblock->__bcount);
    quantum_info(0, " Ext2   ", "\tMagic: 0x%x", __ext2_superblock->__magic);
    quantum_info(0, " Ext2   ", "\tCreator OS: %d", __ext2_superblock->__creator);
    quantum_info(0, " Ext2   ", "\tVName: %s", __ext2_superblock->__vname);
    quantum_info(0, " Ext2   ", "\tUUID: %s", __ext2_superblock->__uuid);
}
