#ifndef INIT_H
#define INIT_H

void quantum_info(char* header, char* format, ...);

void quantum_gdt_init(void);

void quantum_isr_init(void);

void quantum_memory_init(void);

void quantum_keyboard_init(void);
 
#endif