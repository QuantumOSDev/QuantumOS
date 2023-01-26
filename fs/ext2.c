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

static ext2_t* ext2_fs;

void ext2_show_superblock_info(ext2_superblock_t* superblock)
{
    quantum_info(0, " Ext2   ", "Ext2 superblock info:");
    quantum_info(0, " Ext2   ", "total_inodes        = %d", superblock->total_inodes);
    quantum_info(0, " Ext2   ", "total_blocks        = %d", superblock->total_blocks);
    quantum_info(0, " Ext2   ", "su_blocks           = %d", superblock->su_blocks);
    quantum_info(0, " Ext2   ", "free_blocks         = %d", superblock->free_blocks);
    quantum_info(0, " Ext2   ", "free_inodes         = %d", superblock->free_inodes);
    quantum_info(0, " Ext2   ", "superblock_idx      = %d", superblock->superblock_idx);
    quantum_info(0, " Ext2   ", "log2block_size      = %d", superblock->log2block_size);
    quantum_info(0, " Ext2   ", "log2frag_size       = %d", superblock->log2frag_size);
    quantum_info(0, " Ext2   ", "blocks_per_group    = %d", superblock->blocks_per_group);
    quantum_info(0, " Ext2   ", "frags_per_group     = %d", superblock->frags_per_group);
    quantum_info(0, " Ext2   ", "inodes_per_group    = %d", superblock->inodes_per_group);
    quantum_info(0, " Ext2   ", "mtime               = %d", superblock->mtime);
    quantum_info(0, " Ext2   ", "wtime               = %d", superblock->wtime);
    quantum_info(0, " Ext2   ", "mount_count         = %d", superblock->mount_count);
    quantum_info(0, " Ext2   ", "mount_allowed_count = %d", superblock->mount_allowed_count);
    quantum_info(0, " Ext2   ", "ext2_magic          = %d", superblock->ext2_magic);
    quantum_info(0, " Ext2   ", "fs_state            = %d", superblock->fs_state);
    quantum_info(0, " Ext2   ", "err                 = %d", superblock->err);
    quantum_info(0, " Ext2   ", "minor               = %d", superblock->minor);
    quantum_info(0, " Ext2   ", "last_check          = %d", superblock->last_check);
    quantum_info(0, " Ext2   ", "interval            = %d", superblock->interval);
    quantum_info(0, " Ext2   ", "os_id               = %d", superblock->os_id);
    quantum_info(0, " Ext2   ", "major               = %d", superblock->major);
    quantum_info(0, " Ext2   ", "r_userid            = %d", superblock->r_userid);
    quantum_info(0, " Ext2   ", "r_groupid           = %d", superblock->r_groupid);
    quantum_info(0, " Ext2   ", "first_inode         = %d", superblock->first_inode);
    quantum_info(0, " Ext2   ", "inode_size          = %d", superblock->inode_size);
    quantum_info(0, " Ext2   ", "superblock_group    = %d", superblock->superblock_group);
    quantum_info(0, " Ext2   ", "optional_feature    = %d", superblock->optional_feature);
    quantum_info(0, " Ext2   ", "required_feature    = %d", superblock->required_feature);
    quantum_info(0, " Ext2   ", "readonly_feature    = %d", superblock->readonly_feature);
}

#undef EXT2_DEBUG

void ext2_read_superblock(ext2_superblock_t* superblock)
{
    ata_read_sectors(HARDDISK, 1, 2, (unsigned int*)superblock);

#ifdef EXT2_DEBUG
    ext2_show_superblock_info(superblock);
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

ext2_blockgroupdesc_t* ext2_read_bgd(unsigned int bgd_index)
{
    ext2_blockgroupdesc_t* bgd_return;
    int bgd_offset = (1024UL << ext2_fs->superblock->log2block_size) >= 2048 
        ? (1024UL << ext2_fs->superblock->log2block_size)
        : (1024UL << ext2_fs->superblock->log2block_size) * 2;
    int bgd_disk_sector = bgd_offset + sizeof(ext2_blockgroupdesc_t) * bgd_index;
    
    ata_read_sectors(HARDDISK, bgd_disk_sector, 2, (unsigned int*)bgd_return);
}

ext2_inode_t* ext2_read_inode(unsigned int inode_offset)
{
    int block_group = (inode_offset - 1) / ext2_fs->superblock->blocks_per_group;
    int index = (inode_offset - 1) % ext2_fs->superblock->inodes_per_group;
    int containing_block = (index * ext2_fs->superblock->inode_size) / (1024UL << ext2_fs->superblock->log2block_size);

    ext2_blockgroupdesc_t* bgdesc = ext2_read_bgd(block_group);

	unsigned int* inode_tmp = kmalloc(sizeof(unsigned int*));
	ata_read_sectors(HARDDISK, 2 * (bgdesc->inodetable + containing_block), 2, inode_tmp);
	
	ext2_inode_t* inode;
	inode = (ext2_inode_t*)((unsigned int)inode_tmp + (index % (1024 / ext2_fs->superblock->inode_size)) * ext2_fs->superblock->inode_size);
    // int inode_disk_sector = (bgdesc->inodetable * (1024UL << ext2_fs->superblock->log2block_size) + ext2_fs->superblock->inode_size * index);
    // ata_read_sectors(HARDDISK, inode_disk_sector, 2, (unsigned int*)inode);

    quantum_info(0, " Ext2   ", "Inode Type: 0x%x, Inode UID: 0x%x, Inode Block 0: %d",
        inode->perms, inode->uid, inode->blocks[0]);

    return inode;
}

void quantum_ext2_init()
{
    ext2_fs = kcalloc(1, sizeof(ext2_t));
    ext2_read_superblock(ext2_fs->superblock);

    if (ext2_fs->superblock->ext2_magic != EXT2_MAGIC) 
    {
        kpanic("ext2_read_superblock() function failed, we can't successfully initialize ext2");
        return;
    }

    if (ext2_fs->superblock->fs_state != EXT2_FILE_SYSTEM_CLEAN)
    {
        kpanic("ext2 filesystem have errors (filesystem_state != EXT2_FILE_SYSTEM_CLEAN)");
        return;
    }

    ext2_fs->root_inode = ext2_read_inode(2);

    quantum_info(0, " Ext2   ", "Successfully initialized Ext2 filesystem");
    quantum_info(0, " Ext2   ", "File system has been created on: %s", ext2_creator_os(ext2_fs->superblock->os_id));

    // ext2_inode_t inode_first
}
