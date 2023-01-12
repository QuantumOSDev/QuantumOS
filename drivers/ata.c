#include <quantum/init.h>

#include <drivers/ata.h>

#include <core/kpanic.h>

#include <sys/memory.h>
#include <sys/pio.h>
#include <sys/pic.h>

#define ATA_PRIMARY_IO   0x1F0
#define ATA_SECONDARY_IO 0x170

#define ATA_PRIMARY_DCR_AS   0x3F6
#define ATA_SECONDARY_DCR_AS 0x376

#define ATA_PRIMARY_IRQ 14
#define ATA_SECONDARY_IRQ 15

unsigned char __ata_pm = 0;
unsigned char __ata_pc = 0;
unsigned char __ata_sm = 0;
unsigned char __ata_sc = 0;

unsigned char *__ata_buffer;

void ata_primary_irq(void)
{
    pic_eoi(ATA_PRIMARY_IRQ);
}

void ata_secondary_irq(void)
{
    pic_eoi(ATA_SECONDARY_IRQ);
}

void ata_delay(unsigned char __io)
{
    for (int i = 0; i < 4; i++)
    {
        pio_inb(__io + ATA_REG_ALTSTATUS);
    }
}

void ata_select_drive(unsigned char __bus, unsigned char __index)
{
    if (__bus == ATA_PRIMARY)
    {
        if (__index == ATA_MASTER)
        {
            pio_outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0);
        }
        else
        {
            pio_outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xB0);
        }
    }
    else
    {
        if (__index == ATA_MASTER)
        {
            pio_outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xA0);
        }
        else
        {
            pio_outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xB0);
        }
    }
}

unsigned char ata_identify_drive(unsigned char __bus, unsigned char __drive)
{
    unsigned short __io = 0;

    ata_select_drive(__bus, __drive);

    if (__bus == ATA_PRIMARY)
    {
        __io = ATA_PRIMARY_IO;
    }
    else
    {
        __io = ATA_SECONDARY_IO;
    }

    pio_outb(__io + ATA_REG_SECCOUNT0, 0);
    pio_outb(__io + ATA_REG_LBA0, 0);
    pio_outb(__io + ATA_REG_LBA1, 0);
    pio_outb(__io + ATA_REG_LBA2, 0);

    pio_outb(__io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

    unsigned char __drive_status = pio_inb(__io + ATA_REG_STATUS);

    if (__drive_status)
    {
        while (pio_inb(__io + ATA_REG_STATUS) & ATA_SR_BSY != 0)
        {
__ata_stat_read:
            __drive_status = pio_inb(__io + ATA_REG_STATUS);
        }

        if (__drive_status & ATA_SR_ERR)
        {
            quantum_info(2, " ATA\t", "Disk: [%s] [%s] has ERR set. Disabling it...", (__bus == ATA_PRIMARY) ? "Primary" : "Secondary", (__drive == ATA_PRIMARY) ? "master" : "child");

            return 0;
        }

        while (!(__drive_status & ATA_SR_ERR))
        {
            goto __ata_stat_read;
        }

        quantum_info(0, " ATA\t", "Disk: [%s] [%s] is online!", (__bus == ATA_PRIMARY) ? "Primary" : "Secondary", (__drive == ATA_PRIMARY) ? "master" : "child");

        for (int i = 0; i < 256; i++)
        {
            *(unsigned short *) (__ata_buffer + i * 2) = pio_ins(__io + ATA_REG_DATA);
        }
    }
}

void ata_poll_drive(unsigned short __port)
{
    ata_delay(__port);

    unsigned char __bsy_cleared = 0;
    unsigned char __err_cleared = 0;
    unsigned char __drq_cleared = 0;

    unsigned char __status = pio_inb(__port + ATA_REG_STATUS);

    quantum_info(0, " ATA\t", "Polling drive at port 0x%x", __port);

    while (__bsy_cleared == 0)
    {
        if (__status & ATA_SR_BSY)
        {
            __status = pio_inb(__port + ATA_REG_STATUS);

            continue;
        }

        __bsy_cleared = 1;
    }

    quantum_info(0, " ATA\t", "BSY flag cleared");

    while (__err_cleared == 0)
    {
        if (__status & ATA_SR_ERR)
        {
            quantum_info(2, " ATA\t", "Drive has errors!");

            kpanic("ATA Device Failure!");

            return;
        }

        __err_cleared = 1;
    }

    quantum_info(0, " ATA\t", "ERR flag cleared");

    while (__drq_cleared == 0)
    {
        if (__status & ATA_SR_DRQ)
        {
            __status = pio_inb(__port + ATA_REG_STATUS);

            continue;
        }

        __drq_cleared = 1;
    }

    quantum_info(0, " ATA\t", "DRQ flag cleared");
    quantum_info(0, " ATA\t", "Drive polling done [BSY, ERR, DRQ] OK!");
}

void ata_initialize(void)
{
    quantum_info(0, " ATA\t", "Initializing ATA Drivers!");

    __ata_buffer = (unsigned short *) kmalloc(512);

    ata_poll_drive(ATA_PRIMARY_IO);
}