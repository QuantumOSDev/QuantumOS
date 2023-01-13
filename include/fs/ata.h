#ifndef ATA_H
#define ATA_H

typedef struct
{
    unsigned short __base;
    unsigned short __control;
    unsigned short __bm_ide;
    unsigned short __no_intr;
} __ATA_CHANNELS;

typedef struct
{
    unsigned char __reserved;
    unsigned char __channel;
    unsigned char __drive;

    unsigned short __type;
    unsigned short __signature;
    unsigned short __features;

    unsigned int __command_sets;
    unsigned int __size;

    unsigned char __model[41];
} __ATA_DEVICE;

#define MAXIMUM_CHANNELS    2
#define MAXIMUM_IDE_DEVICES    5

// ATA register ports for read/write
#define ATA_REG_DATA         0x00
#define ATA_REG_ERROR        0x01
#define ATA_REG_FEATURES     0x01
#define ATA_REG_SECCOUNT0    0x02
#define ATA_REG_LBA0         0x03
#define ATA_REG_LBA1         0x04
#define ATA_REG_LBA2         0x05
#define ATA_REG_HDDEVSEL     0x06
#define ATA_REG_COMMAND      0x07
#define ATA_REG_STATUS       0x07
#define ATA_REG_SECCOUNT1    0x08
#define ATA_REG_LBA3         0x09
#define ATA_REG_LBA4         0x0A
#define ATA_REG_LBA5         0x0B
#define ATA_REG_CONTROL      0x0C
#define ATA_REG_ALTSTATUS    0x0C
#define ATA_REG_DEVADDRESS   0x0D

// ATA drive status
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error

// ATA drive error status
#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark


// Channels
#define ATA_PRIMARY      0x00
#define ATA_SECONDARY    0x01

// IDE types
#define IDE_ATA      0x00
#define IDE_ATAPI    0x01

// Command types
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

// Identify types
#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200


#define ATA_SECTOR_SIZE    512

// Directions
#define ATA_READ     0x00
#define ATA_WRITE    0x01

// LBA(Linear Block Address) modes
#define LBA_MODE_48   0x02
#define LBA_MODE_28   0x01
#define LBA_MODE_CHS  0x00

void ata_initialize(unsigned short __prim_base, unsigned short __prim_control,
                      unsigned short __sec_base,  unsigned short __sec_control,
                      unsigned short __bus_addr);

void ata_wait_irq(void);
void ata_irq(void);

int ata_read_sectors(unsigned char __drive, unsigned char __nsectors, unsigned char __lba, unsigned int *__buffer);
int ata_write_sectors(unsigned char __drive, unsigned char __nsectors, unsigned char __lba, unsigned int *__buffer);

#endif
