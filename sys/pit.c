#include <sys/pit.h>
#include <sys/pio.h>

#include <drivers/debug.h>

static unsigned long ticks = 0;

void quantum_pit_init() 
{
    // Disable interrupts
    __asm__("cli");

    // Set PIT timer frequency to 1000 so the timer runs every 0.001s 
    // which is 1 milisecond, also set the irq handler to 0x20 (32) 
    // which is PIT timer irq gate
    pit_set_freq(1000);                                    
    isr_register_interrupt_handler(IRQ_BASE, pit_handler); 

    // Enable interrupts 
    __asm__("sti");
}


void pit_set_freq(int freq)
{
    int divisor = 1193180 / freq; /// Calculate our divisor

    // Send our frequency to PIT to set it!
    pio_outb(PIT_MODE_COMMAND_REG,     0x36);
    pio_outb(PIT_CHANNEL_O_DATA_PORT,  divisor & 0xFF);
    pio_outb(PIT_CHANNEL_O_DATA_PORT,  divisor >> 8);
}

void pit_handler(__registers_t* regs) 
{
    ticks++;
}

void pit_sleep(int ms)
{
    unsigned long eticks = ticks + ms;
    while (ticks < eticks) ;
}