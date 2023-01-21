/*
 * QuantumOS Copyright (c) 2021-2022
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <drivers/sound_blaster.h>

#include <quantum/init.h>
#include <core/string.h>
#include <core/print.h>

#include <sys/isr.h>
#include <sys/pio.h>

BOOL sound_blaster_found = FALSE;

void quantum_sound_blaster_init() 
{
    // Reset sound blaster device
    sound_blaster_reset();

    // If this port return 0xAA this means that sound blaster
    // have been found
    unsigned char dsp_status = pio_inb(DSP_READ);
    if (dsp_status == 0xAA) 
    {
        quantum_info(0, " Sb16   ", "Successfully detected sound blaster device");
        quantum_info(0, " Sb16   ", "DSP_READ port returned 0x%x", dsp_status);
        sound_blaster_found = TRUE;

        // Turn on speakers
        sound_blaster_turn_on();

        // Get the IRQ port and set it
        sound_blaster_irq();
    }
    else 
    {
        quantum_info(1, " Sb16   ", "Can't detect sound blaster device");
        quantum_info(1, " Sb16   ", "DSP_READ port returned 0x%x", dsp_status);
        sound_blaster_found = FALSE;
    }

}

void sound_blaster_reset() 
{
    pio_outb(DSP_RESET, 1);  // Send 1 to DSP_RESET
    pio_sleep();             // Wait 3 microseconds
    pio_outb(DSP_RESET, 0);  // Send 0 to DSP_RESET
}

void sound_blaster_turn_on()
{
    pio_outb(DSP_WRITE, 0xD1);
}

BOOL is_sound_blaster_found()
{
    return sound_blaster_found;
}

void sound_blaster_irq_handler(__registers_t* regs) 
{
    printf("Got an interrupt from SB16\n");
}

void sound_blaster_dma_channel_16(unsigned char channel_number, unsigned char* sound_data, int sound_data_length)
{
    unsigned char channel = channel_number + 0x04;
    unsigned char transfer_mode = channel_number + 0x48; // single mode + channel

    char buf[32] = { 0 };
    itoa((int)sound_data, buf, 16);

    unsigned char page_data     = (unsigned char)((int)sound_data >> (strlen(buf) - 2) * 4);  // if sound_data ptr = 0x71e241 then page_data = 0x71
    unsigned char high_bits_pos = (unsigned char)(((int)sound_data & 0xff00) >> 8);       // if sound_data ptr = 0x71e241 then high_bits_pos = 0xe2
    unsigned char low_bits_pos  = (unsigned char)((int)sound_data & 0xff);                 // if sound_data ptr = 0x71e241 then low_bits_pos = 0x41

    char buf2[32] = { 0 };
    itoa((int)sound_data_length, buf2, 16);

    unsigned char low_bits_length  = (unsigned char)(sound_data_length >> (strlen(buf2) * 4));
    unsigned char high_bits_length = (unsigned char)(sound_data_length & 0xff);

    pio_outb(0xD4, channel);          // disable channel, channel number + 0x04
    pio_outb(0xD8, 69);               // write any value to 0xD8, i will write 69 ^_^
    pio_outb(0xD6, transfer_mode);    // send transfer mode
    pio_outb(0x8B, page_data);        // send page of sound data, for example if sound data is at 0x71e241, page is 0x71
    pio_outb(0xC4, low_bits_pos);     // send low bit of sound data, for example if sound data is at 0x71e241, page is 0x41
    pio_outb(0xC4, high_bits_pos);    // send high bit of sound data, for example if sound data is at 0x71e241, page is 0xe2
    pio_outb(0xC6, low_bits_length);  // send low bits of sound data length
    pio_outb(0xC6, high_bits_length); // send high bits of sound data length
    pio_outb(0xD4, channel_number);   // send channel number that need to be enabled
}

void sound_blaster_irq() 
{
    unsigned char irq_set = 0x02;           // IRQ 5
    pio_outb(DSP_MIXER_PORT, 0x80);         // Send 0x80 to Mixer port, because we want to set irq
    pio_outb(DSP_MIXER_DATA_PORT, irq_set); // Send 0x02 (IRQ 5) to Mixer data port now IRQ 5 will be the sb16 IRQ

    // Check is it set successfully
    unsigned char irq_sb16 = pio_inb(DSP_MIXER_DATA_PORT);
    if (irq_sb16 == irq_set) 
    {
        quantum_info(0, " Sb16   ", "Sound blaster is set to IRQ 5, (DSP_MIXER_DATA_PORT returned 0x%x)", irq_sb16);
        quantum_info(0, " Sb16   ", "Setting IRQ 5 to sound blaster output");
        isr_register_interrupt_handler(IRQ5_RESERVED, sound_blaster_irq_handler);
    }
    else 
    {
        quantum_info(1, " Sb16   ", "Could not set sound blaster to IRQ 5");
        sound_blaster_found = FALSE;
    }
}
