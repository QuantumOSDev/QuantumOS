#ifndef PIT_H
#define PIT_H

#include <sys/isr.h>

#define PIT_CHANNEL_O_DATA_PORT 0x40
#define PIT_CHANNEL_1_DATA_PORT 0x41
#define PIT_CHANNEL_2_DATA_PORT 0x42

#define PIT_MODE_COMMAND_REG    0x43

void quantum_pit_init();

void pit_set_freq(int freq);

void pit_handler(__registers_t* regs);

void pit_sleep(int ms);

unsigned long pit_get_ticks();

#endif