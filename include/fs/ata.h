#ifndef ATA_H
#define ATA_H

#include <drivers/pci.h>

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_CMD        0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

#define ATA_ERROR_BBK   0x080 /* Bad block */
#define ATA_ERROR_UNC   0x040 /* Uncorrectable data */
#define ATA_ERROR_MC    0x020 /* Media changed */
#define ATA_ERROR_IDNF  0x010 /* ID mark not found */
#define ATA_ERROR_MCR   0x008 /* Media change request */
#define ATA_ERROR_ABRT  0x004 /* Command aborted */
#define ATA_ERROR_TK0NF 0x002 /* Track 0 not found */
#define ATA_ERROR_AMNF  0x001 /* No address mark */

/* ATA commands */
#define ATA_CMD_RESET             0x04
#define ATA_CMD_READ_SECTORS      0x20
#define ATA_CMD_READ_SECTORS_EXT  0x24
#define ATA_CMD_WRITE_SECTORS     0x30
#define ATA_CMD_WRITE_SECTORS_EXT 0x34
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

#define ATA_STATUS_BSY  0x80 /* Busy */
#define ATA_STATUS_DRDY 0x40 /* Drive ready */
#define ATA_STATUS_DF   0x20 /* Drive write fault */
#define ATA_STATUS_DSC  0x10 /* Drive seek complete */
#define ATA_STATUS_DRQ  0x08 /* Data request ready */
#define ATA_STATUS_CORR 0x04 /* Corrected data */
#define ATA_STATUS_IDX  0x02 /* Index */
#define ATA_STATUS_ERR  0x01 /* Error */

#define ATA_MODE_CHS   0x10
#define ATA_MODE_LBA28 0x20
#define ATA_MODE_LBA48 0x40

#define ATA_PIO_PORT_P_BASE 0x1F0
#define ATA_PIO_PORT_P_CTRL 0x3F6
#define ATA_PIO_PORT_S_BASE 0x170
#define ATA_PIO_PORT_S_CTRL 0x376

#define ATA_CAPABILITIES_LBA 0x200

#define ATA_PRIMARY   0x00
#define ATA_SECONDARY 0x01

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

#define ATA_TYPE_PATA   0x0000
#define ATA_TYPE_SATA   0xC33C
#define ATA_TYPE_PATAPI 0xEB14
#define ATA_TYPE_SATAPI 0x9669

#define ATA_COMMAND_SET_48 (1 << 26)

typedef struct __ata_drive
{
    unsigned int __base;
    unsigned int __ctrl;

    int __id;
    int __mode;
    int __type;
    int __child;

    int __capabilities;
    int __command_set;
    int __signature;
    int __max_lba;

    char __model[41];
} __ata_drive_t;

int ata_initialize(__pci_device_t *__dev);

#endif