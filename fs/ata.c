/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Code-Sploit <samuelthart@pm.me>
 */

#include <quantum/init.h>

#include <drivers/pci.h>

#include <core/print.h>

#include <sys/pio.h>

#include <fs/ata.h>

__ata_channel_t __ata_channels[ATA_MAX_CHANNELS];
__ata_device_t  __ata_devices[ATA_MAX_DEVICES];

static volatile unsigned char __ata_irq_invoked = 0;

void ata_handler(unsigned int __prim_channel_base_addr, unsigned int __prim_channel_control_addr,
                        unsigned int __sec_channel_base_addr,  unsigned int __sec_channel_control_addr,
                        unsigned int __bus_master_addr)
{
    int i;
    int j;
    int k;

    int __count = 0;

    unsigned char __buffer[2048] = {0};

    /* Setup I/O Ports */

    __ata_channels[ATA_PRIMARY].__base = __prim_channel_base_addr;
    __ata_channels[ATA_PRIMARY].__ctrl = __prim_channel_control_addr;
    __ata_channels[ATA_PRIMARY].__bmid = __bus_master_addr;
    
    __ata_channels[ATA_SECONDARY].__base = __sec_channel_base_addr;
    __ata_channels[ATA_SECONDARY].__ctrl = __sec_channel_control_addr;
    __ata_channels[ATA_SECONDARY].__bmid = __bus_master_addr;

    /* Disable IRQs */

    ata_write_register(ATA_PRIMARY,   ATA_REG_CONTROL, 2);
    ata_write_register(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    /* Detect ATA-ATAPI devices */
    /* TODO: Move this to the PCI Driver */

    for (i = 0; i < ATA_MAX_CHANNELS; i++)
    {
        for (j = 0; j < ATA_MAX_CHANNELS; j++)
        {
            unsigned char __error  = 0;
            unsigned char __type   = IDE_ATA;
            unsigned char __status = 0;

            /* At first glance we assume there is no drive here */

            __ata_devices[__count].__active = 0;

            /* Select drive */

            ata_write_register(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));

            /* Waste 3x 400ns to wait for select command to get processed */

            for (int y = 0; y < 3; y++)
            {
                for (int z = 0; z < 4; z++)
                {
                    ata_read_register(i, ATA_REG_ALTSTATUS);
                }
            }

            /* Send ATA identify command */

            ata_write_register(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

            /* Waste 3x 400ns to wait for identify command to get processed */

            for (int y = 0; y < 3; y++)
            {
                for (int z = 0; z < 4; z++)
                {
                    ata_read_register(i, ATA_REG_ALTSTATUS);
                }
            }            

            /* Poll the drive */

            if (ata_read_register(i, ATA_REG_STATUS) == 0)
            {
                /* Status == 0? -> No device here -> Move on */

                continue;
            }

            while (1)
            {
                /* Get the status */

                __status = ata_read_register(i, ATA_REG_STATUS);

                if ((__status & ATA_SR_ERR))
                {
                    __error = 1;

                    break;
                }

                if (!(__status & ATA_SR_BSY) && (__status & ATA_SR_DRQ))
                {
                    break;
                }
            }

            if (__error != 0)
            {
                unsigned char __cl = ata_read_register(i, ATA_REG_LBA1);
                unsigned char __ch = ata_read_register(i, ATA_REG_LBA2);

                if (__cl == 0x14 && __ch == 0xEB)
                {
                    __type = IDE_ATAPI;
                }
                else if (__cl == 0x69 && __ch == 0x96)
                {
                    __type = IDE_ATAPI;
                }
                else
                {
                    /* Unknown type wtf... */

                    continue;
                }

                ata_write_register(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
            }

            ata_read_buffer(i, ATA_REG_DATA, (unsigned int *) __buffer, 128);

            __ata_devices[__count].__active  = 1;
            __ata_devices[__count].__type    = __type;
            __ata_devices[__count].__channel = i;
            __ata_devices[__count].__drive   = j;

            __ata_devices[__count].__sign     = *((unsigned short *) (__buffer + ATA_IDENT_DEVICETYPE));
            __ata_devices[__count].__features = *((unsigned short *) (__buffer + ATA_IDENT_CAPABILITIES));
            __ata_devices[__count].__commands = *((unsigned int *)   (__buffer + ATA_IDENT_COMMANDSETS));

            if (__ata_devices[__count].__commands & (1 << 26))
            {
                __ata_devices[__count].__size = *((unsigned int *) (__buffer + ATA_IDENT_MAX_LBA_EXT));
            }
            else
            {
                __ata_devices[__count].__size = *((unsigned int *) (__buffer + ATA_IDENT_MAX_LBA));
            }

            for (k = 0; k < 40; k += 2)
            {
                __ata_devices[__count].__model[k]     = __buffer[ATA_IDENT_MODEL + k + 1];
                __ata_devices[__count].__model[k + 1] = __buffer[ATA_IDENT_MODEL + k];
            }

            __ata_devices[__count].__model[40] = '\0';

            for (k = 39; k >= 0; k--)
            {
                char __c = __ata_devices[__count].__model[k];

                if (__c == ' ')
                {
                    __ata_devices[__count].__model[k] = '\0';
                }
                else
                {
                    break;
                }
            }

            __count++;
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (__ata_devices[i].__active == 1)
        {
            printf("Drive Detected: \n");
            printf("\t->\tModel: %s\n", __ata_devices[i].__model);
            printf("\t->\tType: %s\n", (const char *[]) {"ATA", "ATAPI"}[__ata_devices[i].__type]);
            printf("\t->\tSize: %d\n", __ata_devices[i].__size);
        }
    }
}

unsigned char ata_read_register(unsigned char __channel, unsigned char __register)
{
    /* The (__data) variable will contain the data at the end of the function */

    unsigned char __data;

    /* Tell ATA_REG_CONTROL that IRQ is ready */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__nint);
    }

    /* Read the data from ATA */

    if (__register < 0x08)
    {
        __data = pio_inb(__ata_channels[__channel].__base + __register - 0x00);
    }
    else if (__register < 0x0C)
    {
        __data = pio_inb(__ata_channels[__channel].__base + __register - 0x06);
    }
    else if (__register < 0x0E)
    {
        __data = pio_inb(__ata_channels[__channel].__ctrl + __register - 0x0A);
    }
    else if (__register < 0x16)
    {
        __data = pio_inb(__ata_channels[__channel].__bmid + __register - 0x0E);
    }

    /* Tell ATA_REG_CONTROL that reading is done */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__nint);
    }

    /* Return data */

    return __data;
}

void ata_write_register(unsigned char __channel, unsigned char __register, unsigned char __data)
{
    /* Tell ATA_REG_CONTROL that IRQ is ready */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__nint);
    }

    /* Write the data to ATA Ports */

    if (__register < 0x08)
    {
        pio_outb(__ata_channels[__channel].__base + __register - 0x00, __data);
    }
    else if (__register < 0x0C)
    {
        pio_outb(__ata_channels[__channel].__base + __register - 0x06, __data);
    }
    else if (__register < 0x0E)
    {
        pio_outb(__ata_channels[__channel].__ctrl + __register - 0x0A, __data);
    }
    else if (__register < 0x16)
    {
        pio_outb(__ata_channels[__channel].__bmid + __register - 0x0E, __data);
    }

    /* Tell ATA_REG_CONTROL that writing is done */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__nint);
    }
}

void ata_flush_cache(unsigned char __drive)
{

}

void ata_read_buffer(unsigned char __channel, unsigned char __register, unsigned int *__buffer, unsigned int __quads)
{
    /* Tell ATA_REG_CONTROL that IRQ is ready */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__nint);
    }

    /* Save data-segment */

    asm volatile ("pushw %es");
    asm volatile ("movw %ds, %ax");
    asm volatile ("movw %ax, %es");

    /* Read the data from ATA */

    if (__register < 0x08)
    {
        pio_qinl(__ata_channels[__channel].__base + __register - 0x00, __buffer, __quads);
    }
    else if (__register < 0x0C)
    {
        pio_qinl(__ata_channels[__channel].__base + __register - 0x06, __buffer, __quads);
    }
    else if (__register < 0x0E)
    {
        pio_qinl(__ata_channels[__channel].__ctrl + __register - 0x0A, __buffer, __quads);
    }
    else if (__register < 0x16)
    {
        pio_qinl(__ata_channels[__channel].__bmid + __register - 0x0E, __buffer, __quads);
    }

    /* Restore data-segment */

    asm ("popw %es");

    /* Tell ATA_REG_CONTROL that reading is done */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__nint);
    }
}

void ata_write_buffer(unsigned char __channel, unsigned char __register, unsigned int *__buffer, unsigned int __quads)
{
    /* Tell ATA_REG_CONTROL that IRQ is ready */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__nint);
    }

    /* Save data-segment */

    asm volatile ("pushw %es");
    asm volatile ("movw %ds, %ax");
    asm volatile ("movw %ax, %es");

    /* Write the data to ATA ports */

    if (__register < 0x08)
    {
        pio_qoutl(__ata_channels[__channel].__base + __register - 0x00, __buffer, __quads);
    }
    else if (__register < 0x0C)
    {
        pio_qoutl(__ata_channels[__channel].__base + __register - 0x06, __buffer, __quads);
    }
    else if (__register < 0x0E)
    {
        pio_qoutl(__ata_channels[__channel].__ctrl + __register - 0x0A, __buffer, __quads);
    }
    else if (__register < 0x16)
    {
        pio_qoutl(__ata_channels[__channel].__bmid + __register - 0x0E, __buffer, __quads);
    }

    /* Restore data-segment */

    asm ("popw %es");

    /* Tell ATA_REG_CONTROL that writing is done */

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__nint);
    }
}

unsigned char ata_probe_drive(unsigned char __channel, unsigned char __advanced)
{
    /* Sleep for 400 ns for BSY flag to be set */

    for (int i = 0; i < 4; i++)
    {
        ata_read_register(__channel, ATA_REG_ALTSTATUS);
    }

    /* Wait for BSY flag to be cleared */

    while (ata_read_register(__channel, ATA_REG_STATUS) & ATA_SR_BSY)
    {
        /* Do nothing... */
    }

    if (__advanced)
    {
        /* Get status of drive */

        unsigned char __state = ata_read_register(__channel, ATA_REG_STATUS);

        /* Check drive for ERR flag */

        if (__state & ATA_SR_ERR)
        {
            printf("ata_probe_drive() -> ATA_SR_ERR -> returning (1)\n");

            return 1;
        }

        /* Check drive for DEV-Fault flag */

        if (__state & ATA_SR_DF)
        {
            printf("ata_probe_drive() -> ATA_SR_DF -> returning (2)\n");

            return 2;
        }

        /* Check drive for DRQ flag */

        if ((__state & ATA_SR_DRQ) == 0)
        {
            printf("ata_probe_drive() -> ATA_SR_DRQ -> returning (3)\n");

            return 3;
        }

        /* Still no errors so returning 0 */
    }

    /* Everything went as expected */
    
    return 0;
}

unsigned char ata_print_error(unsigned int __drive, unsigned char __error)
{
    /* If (__error) == 0 then we can assume that ata_probe_drive() returned no error -> no error needs to be printed */

    if (__error == 0)
    {
        return 0;
    }

    /* Seems like something went wrong */

    printf("ata_print_error() ->");

    if (__error == 1)
    {
        /* Device fault as mentioned above in ata_probe_drive() */

        printf(" Device Fault\n");

        __error = ATA_ERROR_CODE_DF;
    }
    else if (__error == 2)
    {
        /* ERR Flag is set as mentioned above in ata_probe_drive() */

        /* Figure out the precise error */

        unsigned char __st = ata_read_register(__ata_devices[__drive].__channel, ATA_REG_ERROR);

        if (__st & ATA_ER_AMNF)
        {
            printf(" No Address Mark Found\n");

            __error = ATA_ERROR_CODE_AMNF;
        }

        if (__st & ATA_ER_TK0NF)
        {
            printf(" Track [0] not found\n");

            __error = ATA_ERROR_CODE_TK0NF;
        }

        if (__st & ATA_ER_ABRT)
        {
            printf(" Command Aborted\n");

            __error = ATA_ERROR_CODE_ABRT;
        }

        if (__st & ATA_ER_MCR)
        {
            printf(" No Media or Media Error\n");

            __error = ATA_ERROR_CODE_MCR;
        }

        if (__st & ATA_ER_IDNF)
        {
            printf(" ID Mark not found\n");

            __error = ATA_ERROR_CODE_IDNF;
        }

        if (__st & ATA_ER_MC)
        {
            printf(" No Media or Media Error\n");

            __error = ATA_ERROR_CODE_MC;
        }

        if (__st & ATA_ER_UNC)
        {
            printf(" Uncorrectable Data Error\n");

            __error = ATA_ERROR_CODE_UNC;
        }

        if (__st & ATA_ER_BBK)
        {
            printf(" Bad Sectors\n");

            __error = ATA_ERROR_CODE_BBK;
        }
    }
    else if (__error == 3)
    {
        /* DRQ Flag is not set as mentioned above in ata_probe_drive() */

        printf(" Null Read\n");

        __error = ATA_ERROR_CODE_NULL_READ;
    }
    else if (__error == 4)
    {
        printf(" Write Protected\n");

        __error = ATA_ERROR_CODE_WRITE_PROTECTED;
    }

    const char *__ps[] = {"Primary", "Secondary"};
    const char *__mc[] = {"Master", "Child"};

    printf("\n\t-> Error occured in: {%s %s} %s\n", __ps[__ata_devices[__drive].__channel], __mc[__ata_devices[__drive].__drive], __ata_devices[__drive].__model);

    return __error;
}

unsigned char ata_access_drive(unsigned char __direction, unsigned char __drive, unsigned int __lba, unsigned char __sectors, unsigned int __buffer)
{
    unsigned char __lba_mode;
    unsigned char __dma;
    unsigned char __cmd;

    unsigned char __lba_io[6];

    unsigned int __channel  = __ata_devices[__drive].__channel;
    unsigned int __childbit = __ata_devices[__drive].__drive;
    unsigned int __bus      = __ata_channels[__channel].__base;

    unsigned int __words = 256;

    unsigned short __cyl;
    unsigned short i;

    unsigned char __head;
    unsigned char __sect;
    unsigned char __error;

    ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__nint = (__ata_irq_invoked = 0x0) + 0x2);

    if (__lba >= 0x10000000)
    {
        __lba_mode = LBA_MODE_48;

        __lba_io[0] = (__lba & 0x000000FF) >> 0;
        __lba_io[1] = (__lba & 0x0000FF00) >> 8;
        __lba_io[2] = (__lba & 0x00FF0000) >> 16;
        __lba_io[3] = (__lba & 0xFF000000) >> 24;
        __lba_io[4] = 0;
        __lba_io[5] = 0;

        __head = 0;
    }
    else if (__ata_devices[__drive].__features & 0x200)
    {
        __lba_mode = LBA_MODE_28;

        __lba_io[0] = (__lba & 0x000000FF) >> 0;
        __lba_io[1] = (__lba & 0x0000FF00) >> 8;
        __lba_io[2] = (__lba & 0x00FF0000) >> 16;
        __lba_io[3] = 0;
        __lba_io[4] = 0;
        __lba_io[5] = 0;

        __head = (__lba & 0xF0000000) >> 24;
    }
    else
    {
        __lba_mode = LBA_MODE_CHS;

        __sect = (__lba % 63) + 1;
        __cyl  = (__lba + 1 - __sect) / (16 * 63);

        __lba_io[0] = __sect;
        __lba_io[1] = (__cyl >> 0) & 0xFF;
        __lba_io[2] = (__cyl >> 8) & 0xFF;
        __lba_io[3] = 0;
        __lba_io[4] = 0;
        __lba_io[5] = 0;

        __head = (__lba + 1 - __sect) % (16 * 63) / 63;
    }

    __dma = 0;

    while (ata_read_register(__channel, ATA_REG_STATUS) & ATA_SR_BSY)
    {
        /* Wait */
    }

    if (__lba_mode == LBA_MODE_CHS)
    {
        ata_write_register(__channel, ATA_REG_HDDEVSEL, 0xA0 | (__childbit << 4) | __head);
    }
    else
    {
        ata_write_register(__channel, ATA_REG_HDDEVSEL, 0xE0 | (__childbit << 4) | __head);
    }

    if (__lba_mode == LBA_MODE_48)
    {
        ata_write_register(__channel, ATA_REG_SECCOUNT1, 0);

        ata_write_register(__channel, ATA_REG_LBA3, __lba_io[3]);
        ata_write_register(__channel, ATA_REG_LBA4, __lba_io[4]);
        ata_write_register(__channel, ATA_REG_LBA5, __lba_io[5]);
    }

    ata_write_register(__channel, ATA_REG_SECCOUNT0, __sectors);

    ata_write_register(__channel, ATA_REG_LBA0, __lba_io[0]);
    ata_write_register(__channel, ATA_REG_LBA1, __lba_io[1]);
    ata_write_register(__channel, ATA_REG_LBA2, __lba_io[2]);

    if (__lba_mode == LBA_MODE_CHS && __dma == 0 && __direction == ATA_READ)  __cmd = ATA_CMD_READ_PIO;
    if (__lba_mode == LBA_MODE_28  && __dma == 0 && __direction == ATA_READ)  __cmd = ATA_CMD_READ_PIO;
    if (__lba_mode == LBA_MODE_48  && __dma == 0 && __direction == ATA_READ)  __cmd = ATA_CMD_READ_PIO_EXT;
    if (__lba_mode == LBA_MODE_CHS && __dma == 1 && __direction == ATA_READ)  __cmd = ATA_CMD_READ_DMA;
    if (__lba_mode == LBA_MODE_28  && __dma == 1 && __direction == ATA_READ)  __cmd = ATA_CMD_READ_DMA;
    if (__lba_mode == LBA_MODE_48  && __dma == 1 && __direction == ATA_READ)  __cmd = ATA_CMD_READ_DMA_EXT;
    if (__lba_mode == LBA_MODE_CHS && __dma == 0 && __direction == ATA_WRITE) __cmd = ATA_CMD_WRITE_PIO;
    if (__lba_mode == LBA_MODE_28  && __dma == 0 && __direction == ATA_WRITE) __cmd = ATA_CMD_WRITE_PIO;
    if (__lba_mode == LBA_MODE_48  && __dma == 0 && __direction == ATA_WRITE) __cmd = ATA_CMD_WRITE_PIO_EXT;
    if (__lba_mode == LBA_MODE_CHS && __dma == 1 && __direction == ATA_WRITE) __cmd = ATA_CMD_WRITE_DMA;
    if (__lba_mode == LBA_MODE_28  && __dma == 1 && __direction == ATA_WRITE) __cmd = ATA_CMD_WRITE_DMA;
    if (__lba_mode == LBA_MODE_48  && __dma == 1 && __direction == ATA_WRITE) __cmd = ATA_CMD_WRITE_DMA_EXT;

    ata_write_register(__channel, ATA_REG_COMMAND, __cmd);

    if (__dma)
    {
        /* We don't support DMA */
    }
    else if (__direction == ATA_READ)
    {
        for (i = 0; i < __sectors; i++)
        {
            if ((__error = ata_probe_drive(__channel, 1)))
            {
                return __error;
            }

            asm ("pushw %es");
            asm("rep insw"
                :
                : "c"(__words), "d"(__bus), "D"(__buffer));  // Receive Data.
            asm("popw %es");

            __buffer += ATA_SECTOR_SIZE;
        }
    }
    else
    {
        for (i = 0; i < __sectors; i++)
        {
            ata_probe_drive(__channel, 0);  // Polling.

            asm("pushw %ds");
            asm("rep outsw" ::"c"(__words), "d"(__bus), "S"(__buffer));  // Send Data
            asm("popw %ds");

            __buffer += ATA_SECTOR_SIZE;
        }

        ata_write_register(__channel, ATA_REG_COMMAND, (char[]) {ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH_EXT}[__lba_mode]);
        ata_probe_drive(__channel, 0);  // Polling.
    }

    return 0;
}

int ata_read_sectors(unsigned char __drive,  unsigned char __sectors, unsigned int __lba, unsigned int __buffer)
{
    if (__drive > ATA_MAX_DEVICES || __ata_devices[__drive].__active == 0)
    {
        printf("ATA Error: Drive not found\n");

        return -1;
    }
    else if (((__lba + __sectors) > __ata_devices[__drive].__size) && (__ata_devices[__drive].__type == IDE_ATA))
    {
        printf("ATA Error: LBA Address (0x%x) exceeds disk boundary (0x%x)\n", __lba, __ata_devices[__drive].__size);

        return -2;
    }
    else
    {
        unsigned char __err;

        if (__ata_devices[__drive].__type == IDE_ATA)
        {
            __err = ata_access_drive(ATA_READ, __drive, __lba, __sectors, __buffer);
        }

        return ata_print_error(__drive, __err);
    }

    return 0;
}

int ata_write_sectors(unsigned char __drive, unsigned char __sectors, unsigned int __lba, unsigned int __buffer)
{
    if (__drive > ATA_MAX_DEVICES || __ata_devices[__drive].__active == 0)
    {
        printf("ATA Error: Drive not found\n");

        return -1;
    }
    else if (((__lba + __sectors) > __ata_devices[__drive].__size) && (__ata_devices[__drive].__type == IDE_ATA))
    {
        printf("ATA Error: LBA Address (0x%x) exceeds disk boundary (0x%x)\n", __lba, __ata_devices[__drive].__size);

        return -2;
    }
    else
    {
        unsigned char __err;

        if (__ata_devices[__drive].__type == IDE_ATA)
        {
            __err = ata_access_drive(ATA_WRITE, __drive, __lba, __sectors, __buffer);
        }

        return ata_print_error(__drive, __err);
    }

    return 0;
}

int ata_initialize(__pci_device_t *__dev)
{
    printf("Initializing ATA Drivers!\n");

    return 0;
}