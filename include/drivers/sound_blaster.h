#ifndef SOUND_BLASTER_H
#define SOUND_BLASTER_H

#include <core/stdlib.h>

#define DSP_MIXER_PORT      0x224
#define DSP_MIXER_DATA_PORT 0x225
#define DSP_RESET           0x226
#define DSP_READ            0x22A
#define DSP_WRITE           0x22C	
#define DSP_READ_STATUS     0x22E	
#define DSP_16BIT_INTERRUPT 0x22F	

void quantum_sound_blaster_init();

BOOL is_sound_blaster_found();

void sound_blaster_dma_channel_16(unsigned char channel_number, unsigned char* sound_data, int sound_data_length);

void sound_blaster_irq();

void sound_blaster_reset();

void sound_blaster_turn_on();

#endif
