#ifndef AC97_H
#define AC97_H  

#include <drivers/pci.h>

#define AC97_bar0_RESET_REGISTER 0x00
#define AC97_bar0_SET_MASTER_OUTPUT_VOLUME 0x02
#define AC97_bar0_SET_MICROPHONE_VOLUME 0x0E
#define AC97_bar0_SET_OUPUT_VOLUME_PCM 0x18
#define AC97_bar0_SELECT_INPUT_DEVICE 0x1A
#define AC97_bar0_SET_INPUT_GAIN 0x1C
#define AC97_bar0_SET_GAIN_MICROPHONE 0x1E
#define AC97_bar0_SUPPORTED_EXTENDED_FUNCS 0x28
#define AC97_bar0_ENABLE_EXTENDED_FUNCS 0x2A
#define AC97_bar0_SAMPLE_RATE_FRONT_SPEAKERS 0x2C

void ac97_write_global_control(unsigned short bar0, unsigned short bar1, unsigned int port);

int ac97_initialize(__pci_device_t* dev);

void ac97_play_sound(unsigned char* sound_data);

#endif