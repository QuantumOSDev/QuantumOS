#include <quantum/init.h>

#include <core/string.h>

#include <sys/pio.h>

#include <fs/ata.h>

__ATA_CHANNELS __ata_channels[MAXIMUM_CHANNELS];
__ATA_DEVICE   __ata_devices[MAXIMUM_IDE_DEVICES];

void ata_push_register(const char *__name)
{
    if (strcmp(__name, "es"))
    {
        asm volatile ("pushw %es");
        asm volatile ("movw %ds, %ax");
        asm volatile ("movw %ax, %es");
    }
}

void ata_pop_register(const char *__name)
{
    if (strcmp(__name, "es"))
    {
        asm volatile ("popw %es;");
    }
}

void ata_insl(unsigned short __register, unsigned int *__buffer, int __quads)
{
    for (int i = 0; i < __quads; i++)
    {
        __buffer[i] = pio_inl(__register);
    }
}

void ata_outsl(unsigned short __register, unsigned int *__buffer, int __quads)
{
    for (int i = 0; i < __quads; i++)
    {
        pio_outl(__register, __buffer[i]);
    }
}

void ata_write_register(unsigned char __channel, unsigned char __register, unsigned char __data)
{
    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__no_intr);
    }

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
        pio_outb(__ata_channels[__channel].__control + __register - 0x0A, __data);
    }
    else if (__register < 0x16)
    {
        pio_outb(__ata_channels[__channel].__bm_ide + __register - 0x0E, __data);
    }

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__no_intr);
    }
}

unsigned char ata_read_register(unsigned char __channel, unsigned char __register)
{
    unsigned char __result;

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__no_intr);
    }

    if (__register < 0x08)
    {
        __result = pio_inb(__ata_channels[__channel].__base + __register - 0x00);
    }
    else if (__register < 0x0C)
    {
        __result = pio_inb(__ata_channels[__channel].__base + __register - 0x06);
    }
    else if (__register < 0x0E)
    {
        __result = pio_inb(__ata_channels[__channel].__control + __register - 0x0A);
    }
    else if (__register < 0x16)
    {
        __result = pio_inb(__ata_channels[__channel].__bm_ide + __register - 0x0E);
    }

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__no_intr);
    }

    return __result;
}

void ata_read_buffer(unsigned char __channel, unsigned char __register, unsigned int *__buffer, unsigned int __quads)
{
    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__no_intr);
    }

    ata_push_register("es");

    if (__register < 0x08)
    {
        ata_insl(__ata_channels[__channel].__base + __register - 0x00, __buffer, __quads);
    }
    else if (__register < 0x0C)
    {
        ata_insl(__ata_channels[__channel].__base + __register - 0x06, __buffer, __quads);
    }
    else if (__register < 0x0E)
    {
        ata_insl(__ata_channels[__channel].__control + __register - 0x0A, __buffer, __quads);
    }
    else if (__register < 0x16)
    {
        ata_insl(__ata_channels[__channel].__bm_ide + __register - 0x0E, __buffer, __quads);
    }

    ata_pop_register("es");

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__no_intr);
    }
}

void ata_write_buffer(unsigned char __channel, unsigned char __register, unsigned int *__buffer, unsigned int __quads)
{
    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, 0x80 | __ata_channels[__channel].__no_intr);
    }

    ata_push_register("es");

    if (__register < 0x08)
    {
        ata_outsl(__ata_channels[__channel].__base + __register - 0x00, __buffer, __quads);
    }
    else if (__register < 0x0C)
    {
        ata_outsl(__ata_channels[__channel].__base + __register - 0x06, __buffer, __quads);
    }
    else if (__register < 0x0E)
    {
        ata_outsl(__ata_channels[__channel].__control + __register - 0x0A, __buffer, __quads);
    }
    else if (__register < 0x16)
    {
        ata_outsl(__ata_channels[__channel].__bm_ide + __register - 0x0E, __buffer, __quads);
    }

    ata_pop_register("es");

    if (__register > 0x07 && __register < 0x0C)
    {
        ata_write_register(__channel, ATA_REG_CONTROL, __ata_channels[__channel].__no_intr);
    }
}

unsigned char ata_ide_polling(unsigned char __channel, unsigned char __advanced_check)
{
    for (int i = 0; i < 4; i++)
    {
        ata_read_register(__channel, ATA_REG_ALTSTATUS);
    }

    quantum_info(0, " ATA\t", "Probing drive for errors...");

    while (ata_read_register(__channel, ATA_REG_STATUS) & ATA_SR_BSY)
    {
        /* Wait */
    }

    if (__advanced_check)
    {
        unsigned int __status = ata_read_register(__channel, ATA_REG_STATUS);

        if (__status & ATA_SR_ERR)
        {
            quantum_info(2, " ATA\t", "Probing drive encountered errors: [ERR flag]");

            return 2;
        }

        if (__status & ATA_SR_DF)
        {
            quantum_info(2, " ATA\t", "Probing drive encountered errors: [DF flag]");

            return 1;
        }

        if ((__status & ATA_SR_DRQ) == 0)
        {
            quantum_info(2, " ATA\t", "Probing drive encountered errors: [DRQ flag]");
            
            return 3;
        }
    }

    quantum_info(0, " ATA\t", "Probing successfull! [ERR, DF, DRQ] flags OK!");
}

unsigned char ata_print_error(unsigned int __drive, unsigned char __error)
{
    if (__error == 0)
    {
        return __error;
    }

    if (__error == 1)
    {
        quantum_info(2, " IDE\t", "Human Readable Error: Device Fault");
    }
    else if (__error == 2)
    {
        unsigned char __status = ata_read_register(__ata_devices[__drive].__channel, ATA_REG_ERROR);

        if (__status & ATA_ER_AMNF)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: No Address Mark Found");

            __error = 7;
        }

        if (__status & ATA_ER_TK0NF)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: No Media Or Media Error");

            __error = 3;
        }

        if (__status & ATA_ER_ABRT)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: Command Aborted");

            __error = 20;
        }

        if (__status & ATA_ER_MCR)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: No Media Or Media Error");

            __error = 3;
        }

        if (__status & ATA_ER_IDNF)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: ID Mark Not Found");

            __error = 21;
        }

        if (__status & ATA_ER_MC)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: No Media Or Media Error");

            __error = 3;
        }

        if (__status & ATA_ER_UNC)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: Uncorrectable Data Error");

            __error = 22;
        }

        if (__status & ATA_ER_BBK)
        {
            quantum_info(2, " IDE\t", "Human Readable Error: Bad Sectors");
        }
    }
    else if (__error == 3)
    {
        quantum_info(2, " IDE\t", "Human Readable Error: Reading Nothing");
    }
    else if (__error == 4)
    {
        quantum_info(2, " IDE\t", "Human Readable Error: Write Protected");
    }

    quantum_info(2, " ATA\t", "DRIVE: [%s] [%s]", (const char*[]) {"Primary", "Secondary"}[__ata_devices[__drive].__channel], (const char*[]) {"Master", "Child"}[__ata_devices[__drive].__drive]);

    return __error;
}

void ata_select_drive(unsigned int __channel, unsigned int __childbit, unsigned char __lba_mode, unsigned char __head)
{
    /* Assuming drive is not BSY */

    if (__lba_mode == LBA_MODE_CHS)
    {
        ata_write_register(__channel, ATA_REG_HDDEVSEL, 0xA0 | (__childbit << 4) | __head);
    }
    else
    {
        ata_write_register(__channel, ATA_REG_HDDEVSEL, 0xE0 | (__childbit << 4) | __head);
    }
}

void ata_send_command(unsigned char __channel, unsigned char __lba_mode, unsigned char __direction, unsigned char __dma)
{
    unsigned char __cmd;

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
}

void ata_flush_drive(unsigned char __channel, unsigned char __lba_mode)
{
    ata_write_register(__channel, ATA_REG_COMMAND, (char[]) {ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH_EXT}[__lba_mode]);
}

unsigned char ata_access(unsigned char __direction, unsigned char __drive, unsigned int __lba, unsigned char __num_sectors, unsigned int __buffer)
{
    unsigned char __lba_mode;
    unsigned char __lba_io[6];
    unsigned char __dma;
    unsigned char __cmd;

    unsigned int __channel  = __ata_devices[__drive].__channel;
    unsigned int __childbit = __ata_devices[__drive].__drive;
    unsigned int __bus      = __ata_channels[__channel].__base;
    unsigned int __words    = 256;

    unsigned char __head, __sect, __err;

    unsigned short __cyl, i;

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

    ata_select_drive(__channel, __childbit, __lba_mode, __head);

    if (__lba_mode == LBA_MODE_48)
    {
        ata_write_register(__channel, ATA_REG_SECCOUNT1, 0);
        ata_write_register(__channel, ATA_REG_LBA3, __lba_io[3]);
        ata_write_register(__channel, ATA_REG_LBA4, __lba_io[4]);
        ata_write_register(__channel, ATA_REG_LBA5, __lba_io[5]);
    }

    ata_write_register(__channel, ATA_REG_SECCOUNT0, __num_sectors);
    ata_write_register(__channel, ATA_REG_LBA0, __lba_io[0]);
    ata_write_register(__channel, ATA_REG_LBA1, __lba_io[1]);
    ata_write_register(__channel, ATA_REG_LBA2, __lba_io[2]);

    ata_send_command(__channel, __lba_mode, __direction, __dma);

    if (__dma)
    {
        /* We don't support DMA so quit */
    }
    else if (__direction == ATA_READ)
    {
        for (i = 0; i < __num_sectors; i++)
        {
            if (__err = ata_ide_polling(__channel, 1))
            {
                return __err;
            }

            asm ("pushw %es");
            asm ("rep insw"
                 :
                 : "c"(__words), "d"(__bus), "D"(__buffer));
            
            asm ("popw %es");

            __buffer = (__buffer + (__words * 2));
        }
    }
    else
    {
        for (i = 0; i < __num_sectors; i++)
        {
            ata_ide_polling(__channel, 0);

            asm ("pushw %ds");
            asm ("rep outsw" ::"c"(__words), "d"(__bus), "S"(__buffer));
            asm ("popw %ds");

            __buffer = (__buffer + (__words * 2));
        }
    }

    ata_flush_drive(__channel, __lba_mode);
    ata_ide_polling(__channel, 0);

    return 0;
}

void ata_initialize(unsigned short __prim_base, unsigned short __prim_control,
                      unsigned short __sec_base,  unsigned short __sec_control,
                      unsigned short __bus_addr)

{
    __ata_channels[ATA_PRIMARY].__base      = __prim_base;
    __ata_channels[ATA_PRIMARY].__control   = __prim_control;
    __ata_channels[ATA_SECONDARY].__base    = __sec_base;
    __ata_channels[ATA_SECONDARY].__control = __sec_control;
    __ata_channels[ATA_PRIMARY].__bm_ide    = __bus_addr;
    __ata_channels[ATA_SECONDARY].__bm_ide  = __bus_addr;

    ata_write_register(ATA_PRIMARY, ATA_REG_CONTROL, 2);
    ata_write_register(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    int i, j, k, count = 0;

    unsigned char ata_buf[2048] = {0};

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            unsigned char __err = 0;
            unsigned char __type = IDE_ATA;
            unsigned char __status;

            __ata_devices[count].__reserved = 0;

            ata_write_register(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));

            /* TODO put sleep here */

            ata_write_register(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

            /* TODO put sleep here */

            if (ata_read_register(i, ATA_REG_STATUS) == 0)
            {
                continue;
            }

            while (1)
            {
                __status = ata_read_register(i, ATA_REG_STATUS);

                if ((__status & ATA_SR_ERR))
                {
                    __err = 1;

                    break;
                }

                if (!(__status & ATA_SR_BSY) && (__status & ATA_SR_DRQ))
                {
                    break;
                }
            }

            if (__err != 0)
            {
                unsigned char cl = ata_read_register(i, ATA_REG_LBA1);
                unsigned char ch = ata_read_register(i, ATA_REG_LBA2);

                if (cl == 0x14 && ch == 0xEB)
                {
                    __type = IDE_ATAPI;
                }
                else if (cl == 0x69 && ch == 0x96)
                {
                    __type = IDE_ATAPI;
                }
                else
                {
                    continue;
                }

                ata_write_register(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
            }

            ata_read_buffer(i, ATA_REG_DATA, (unsigned int *) ata_buf, 128);

            __ata_devices[count].__reserved     = 1;
            __ata_devices[count].__type         = __type;
            __ata_devices[count].__channel      = i;
            __ata_devices[count].__drive        = j;
            __ata_devices[count].__signature    = *((unsigned short *)(ata_buf + ATA_IDENT_DEVICETYPE));
            __ata_devices[count].__features     = *((unsigned short *)(ata_buf + ATA_IDENT_CAPABILITIES));
            __ata_devices[count].__command_sets = *((unsigned int *)(ata_buf + ATA_IDENT_COMMANDSETS));

            if (__ata_devices[count].__command_sets & (1 << 26))
            {
                quantum_info(0, " ATA\t", "Drive [%d] uses 48-bit addressing", count);

                __ata_devices[count].__size = *((unsigned int *)(ata_buf + ATA_IDENT_MAX_LBA_EXT));
            }
            else
            {
                quantum_info(0, " ATA\t", "Drive [%d] uses CHS/28-bit addressing", count);

                __ata_devices[count].__size = *((unsigned int *)(ata_buf + ATA_IDENT_MAX_LBA));
            }

            for (k = 0; k < 40; k += 2)
            {
                __ata_devices[count].__model[k]     = ata_buf[ATA_IDENT_MODEL + k + 1];
                __ata_devices[count].__model[k + 1] = ata_buf[ATA_IDENT_MODEL + k];
            }

            __ata_devices[count].__model[40] = '\0';

            for (k = 39; k >= 0; k--)
            {
                char ch = __ata_devices[count].__model[k];

                if (ch == ' ')
                {
                    __ata_devices[count].__model[k] = '\0';
                }
                else
                {
                    break;
                }
            }

            count++;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (__ata_devices[i].__reserved == 1)
        {
            quantum_info(0, " ATA\t", "Drive [%d] is online!", __ata_devices[i].__drive);
#ifdef ATA_DEBUG
            quantum_info(0, " ATA\t", "Model: [%s]", __ata_devices[i].__model);
            quantum_info(0, " ATA\t", "Type: [%s]", (const char *[]) {"ATA", "ATAPI"}[__ata_devices[i].__type]);
            quantum_info(0, " ATA\t", "Drive: [%d]", __ata_devices[i].__drive);
            quantum_info(0, " ATA\t", "Channel: [%d]", __ata_devices[i].__channel);
            quantum_info(0, " ATA\t", "Base: [%x]", __ata_channels[i].__base);
            quantum_info(0, " ATA\t", "Control: [%x]", __ata_channels[i].__control);
            quantum_info(0, " ATA\t", "Size: [%d] sectors | [%d] bytes", __ata_devices[i].__size, __ata_devices[i].__size * ATA_SECTOR_SIZE);
            quantum_info(0, " ATA\t", "Signature: [0x%x]", __ata_devices[i].__signature);
            quantum_info(0, " ATA\t", "Features: [%d]\n", __ata_devices[i].__features);
#endif
        }
    }
}

void ata_wait_irq(void)
{

}

void ata_irq(void)
{

}

int ata_read_sectors(unsigned char __drive, unsigned char __nsectors, unsigned char __lba, unsigned int *__buffer)
{
    if (__drive > MAXIMUM_IDE_DEVICES || __ata_devices[__drive].__reserved == 0)
    {
        quantum_info(2, " ATA\t", "Drive [%d] not found", __drive);

        return -1;
    }
    else if (((__lba + __nsectors) > __ata_devices[__drive].__size) && (__ata_devices[__drive].__type == IDE_ATA))
    {
        quantum_info(2, " ATA\t", "LBA Address: (0x%x) requested on drive [%d] exceeds sector boundary (0x%x)", __lba, __ata_devices[__drive].__size);

        return -2;
    }
    else
    {
        unsigned char __err;

        if (__ata_devices[__drive].__type == IDE_ATA)
        {
            __err = ata_access(ATA_READ, __drive, __lba, __nsectors, __buffer);
        }

        return ata_print_error(__drive, __err);
    }

    return 0;
}

int ata_write_sectors(unsigned char __drive, unsigned char __nsectors, unsigned char __lba, unsigned int *__buffer)
{
    if (__drive > MAXIMUM_IDE_DEVICES || __ata_devices[__drive].__reserved == 0)
    {
        quantum_info(2, " ATA\t", "Drive [%d] not found", __drive);

        return -1;
    }
    else if (((__lba + __nsectors) > __ata_devices[__drive].__size) && (__ata_devices[__drive].__type == IDE_ATA))
    {
        quantum_info(2, " ATA\t", "LBA Address: (0x%x) requested on drive [%d] exceeds sector boundary (0x%x)", __lba, __ata_devices[__drive].__size);

        return -2;
    }
    else
    {
        unsigned char __err;

        if (__ata_devices[__drive].__type == IDE_ATA)
        {
            __err = ata_access(ATA_WRITE, __drive, __lba, __nsectors, __buffer);
        }

        return ata_print_error(__drive, __err);
    }

    return 0;
}

