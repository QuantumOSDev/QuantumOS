/*
 * QuantumOS Copyright (c) 2021-2022
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <fs/ext2.h>
#include <fs/ata.h>

#include <core/kpanic.h>
#include <core/string.h>

#include <quantum/init.h>
#include <sys/memory.h>

static __ext2_fs_t* ext2_fs;

void ext2_show_superblock_info(__ext2_superblock_t* ext2_superblock)
{
    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "__ninodes = %d", ext2_superblock->__ninodes);
    quantum_info(0, " Ext2   ", "__nblocks = %d", ext2_superblock->__nblocks);
    quantum_info(0, " Ext2   ", "__nrblocks = %d", ext2_superblock->__nrblocks);
    quantum_info(0, " Ext2   ", "__free_blocks = %d", ext2_superblock->__free_blocks);
    quantum_info(0, " Ext2   ", "__free_inodes = %d", ext2_superblock->__free_inodes);
    quantum_info(0, " Ext2   ", "__dblock = %d", ext2_superblock->__dblock);
    quantum_info(0, " Ext2   ", "__block_size = %d", ext2_superblock->__block_size);
    quantum_info(0, " Ext2   ", "__frag_size = %d", ext2_superblock->__frag_size);
    quantum_info(0, " Ext2   ", "__gblocks = %d", ext2_superblock->__gblocks);
    quantum_info(0, " Ext2   ", "__gfrags = %d", ext2_superblock->__gfrags);
    quantum_info(0, " Ext2   ", "__ginodes = %d", ext2_superblock->__ginodes);
    quantum_info(0, " Ext2   ", "__mtime = %d", ext2_superblock->__mtime);
    quantum_info(0, " Ext2   ", "__wtime = %d", ext2_superblock->__wtime);
    quantum_info(0, " Ext2   ", "__times_mounted = %d", ext2_superblock->__times_mounted); 
    quantum_info(0, " Ext2   ", "__allowed_times_mounted = %d", ext2_superblock->__allowed_times_mounted);
    quantum_info(0, " Ext2   ", "__ext2_signature = 0x%x", ext2_superblock->__ext2_signature);
    quantum_info(0, " Ext2   ", "__filesystem_state = %d", ext2_superblock->__filesystem_state);
    quantum_info(0, " Ext2   ", "__on_error = %d", ext2_superblock->__on_error);
    quantum_info(0, " Ext2   ", "__minor_version = %d", ext2_superblock->__minor_version);
    quantum_info(0, " Ext2   ", "__last_fsck = %d", ext2_superblock->__last_fsck);
    quantum_info(0, " Ext2   ", "__time_between_fsck = %d", ext2_superblock->__time_between_fsck); 
    quantum_info(0, " Ext2   ", "__os_id = %d", ext2_superblock->__os_id);
    quantum_info(0, " Ext2   ", "__major_version = %d", ext2_superblock->__major_version);
    quantum_info(0, " Ext2   ", "__uid = %d", ext2_superblock->__uid);
    quantum_info(0, " Ext2   ", "__gid = %d", ext2_superblock->__gid);
    quantum_info(0, " Ext2   ", "__first_non_reserved_inode = %d", ext2_superblock->__first_non_reserved_inode);
    quantum_info(0, " Ext2   ", "__inode_size = %d", ext2_superblock->__inode_size);
    quantum_info(0, " Ext2   ", "__sb_block_group = %d", ext2_superblock->__sb_block_group);
    quantum_info(0, " Ext2   ", "__compression_algorithms = %d", ext2_superblock->__compression_algorithms);
    quantum_info(0, " Ext2   ", "__jinode = %d", ext2_superblock->__jinode);    
    quantum_info(0, " Ext2   ", "__jdevice = %d", ext2_superblock->__jdevice);  
    quantum_info(0, " Ext2   ", "__head_of_orphan_inode_list = %d", ext2_superblock->__head_of_orphan_inode_list);
    quantum_info(0, " Ext2   ", "__fsid = %s", ext2_superblock->__fsid);
    quantum_info(0, " Ext2   ", "__name = %s", ext2_superblock->__name);
    quantum_info(0, " Ext2   ", "__path_last_mounted_to = %s", ext2_superblock->__path_last_mounted_to);
    quantum_info(0, " Ext2   ", "__jid = %s", ext2_superblock->__jid);
}

#undef EXT2_DEBUG

void ext2_read_superblock(__ext2_superblock_t* ext2_superblock)
{
    ata_read_sectors(HARDDISK, 1, 2, (unsigned int*)ext2_superblock);

#ifdef EXT2_DEBUG
    ext2_show_superblock_info(ext2_superblock);
#endif
}

char* ext2_creator_os(unsigned int creator_os)
{
    switch (creator_os) 
    {
        case EXT2_CREATOR_ID_LINUX:
            return "Linux";
            break;
        case EXT2_CREATOR_ID_GNU_HURD:
            return "GNU Hurd";
            break;
        case EXT2_CREATOR_ID_MASIX:
            return "Masix";
            break;
        case EXT2_CREATOR_ID_FREEBSD:
            return "FreeBSD";
            break;
        case EXT2_CREATOR_ID_OTHER:
            return "Other";
            break;
    }
}

void ext2_read_inode_metadata(__ext2_inode_t* inode, unsigned int inode_idx)
{
    /* Idk what is happening here */
    unsigned int group = inode_idx / ext2_fs->__sb->__ginodes;
    unsigned int inode_table_block = ext2_fs->__bgs_desc[group].__i_tables;
    unsigned int idx_in_group = inode_idx - group * ext2_fs->__sb->__ginodes;
    unsigned int block_offset = (idx_in_group - 1) * ext2_fs->__sb->__inode_size / (ext2_fs->__sb->__block_size == 0 ? 1 : ext2_fs->__sb->__block_size);
    unsigned int offset_in_block = (idx_in_group - 1) - block_offset * (ext2_fs->__sb->__block_size / ext2_fs->__sb->__inode_size);

    char* block_buf = (char*)kmalloc(ext2_fs->__sb->__block_size);
    ata_read_sectors(HARDDISK, inode_table_block + block_offset, 2, block_buf);
    kmemcpy(inode, block_buf + offset_in_block * ext2_fs->__sb->__inode_size, ext2_fs->__sb->__inode_size);
    kfree(block_buf);
}

void quantum_ext2_init()
{
    ext2_fs = kcalloc(1, sizeof(__ext2_fs_t));
    ext2_read_superblock(ext2_fs->__sb);

    if (ext2_fs->__sb->__ext2_signature != EXT2_MAGIC) 
    {
        kpanic("ext2_read_superblock() function failed, we can't successfully initialize ext2");
        return;
    }

    if (ext2_fs->__sb->__filesystem_state != EXT2_FILE_SYSTEM_CLEAN)
    {
        kpanic("ext2 filesystem have errors (filesystem_state != EXT2_FILE_SYSTEM_CLEAN)");
        return;
    }

    ext2_fs->__bsize = (1024 << ext2_fs->__sb->__block_size);
    ext2_fs->__igp = ext2_fs->__sb->__ginodes;
    ext2_fs->__bpg = ext2_fs->__sb->__gblocks;
    ext2_fs->__inode_size = ext2_fs->__sb->__inode_size;

    ext2_fs->__total_groups = ext2_fs->__sb->__nblocks / ext2_fs->__bpg;
    if (ext2_fs->__bpg * ext2_fs->__total_groups < ext2_fs->__total_groups)
    {
        ext2_fs->__total_groups++;
    }

    ext2_fs->__bgd_blocks = (ext2_fs->__total_groups * sizeof(__ext2_bg_desc_t)) / ext2_fs->__bsize;
    if (ext2_fs->__bgd_blocks * ext2_fs->__sb->__block_size < ext2_fs->__bpg * sizeof(__ext2_bg_desc_t))
    {
        ext2_fs->__bgd_blocks++;
    }

    ext2_fs->__bgs_desc = (__ext2_bg_desc_t*)kcalloc(sizeof(__ext2_bg_desc_t), ext2_fs->__bgd_blocks * ext2_fs->__sb->__block_size);
    for (int i = 0; i < ext2_fs->__bgd_blocks; i++)
    {
        ata_read_sectors(HARDDISK, 2, 2, (unsigned int*)(ext2_fs->__bgs_desc + i * ext2_fs->__sb->__block_size));
    }

    ext2_fs->__root_inode = (__ext2_inode_t*)kcalloc(sizeof(__ext2_inode_t), 1);
    ext2_read_inode_metadata(ext2_fs->__root_inode, 2);

    quantum_info(0, " Ext2   ", "Successfully initialized Ext2 filesystem");
    quantum_info(0, " Ext2   ", "File system has been created on: %s", ext2_creator_os(ext2_fs->__sb->__os_id));
}
