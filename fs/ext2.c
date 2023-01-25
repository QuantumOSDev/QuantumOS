/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <fs/ext2.h>
#include <fs/ata.h>

#include <quantum/init.h>
#include <sys/memory.h>

static __ext2_superblock_t *ext2_superblock;

void quantum_ext2_init()
{
    unsigned int __buffer[ATA_SECTOR_SIZE];

    ata_read_sectors(1, 1, 2, (unsigned int*)__buffer);
    kmemcpy(ext2_superblock, __buffer, ATA_SECTOR_SIZE);
    // ata_read_sectors(HARDDISK, 1, 2, (unsigned int*)ext2_superblock);

    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "\tTotal inodes: %d", ext2_superblock->__icount);
    quantum_info(0, " Ext2   ", "\tTotal blocks: %d", ext2_superblock->__bcount);
    quantum_info(0, " Ext2   ", "\tMagic: 0x%x", ext2_superblock->__magic);
    quantum_info(0, " Ext2   ", "\tCreator OS: %d", ext2_superblock->__creator);
    quantum_info(0, " Ext2   ", "\tVName: %s", ext2_superblock->__vname);
    quantum_info(0, " Ext2   ", "\tUUID: %s", ext2_superblock->__uuid);
}
