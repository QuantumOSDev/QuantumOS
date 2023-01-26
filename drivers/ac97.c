/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <drivers/ac97.h>
#include <drivers/pci.h>

#include <quantum/init.h>
#include <sys/pio.h>

void ac97_write_global_control(unsigned short bar0, unsigned short bar1, unsigned int port)
{

}

int ac97_initialize(__pci_device_t* dev)
{
    quantum_info(0, " AC97   ", "Initializng AC97 driver sound card");

    // Get the pci common header so we can get the bar0 and bar1
    GET_COMMON_HEADER_PCI(pci_header, dev);
    GET_BAR_FROM_PCI_HEADER(pci_header, ac_bar0, ac_bar1);
    
    // Write to global control register 0x02, but we need pci_write to this!
    // Code-Sploit implement pci_write please my handsome boy

    return 0;
}

void ac97_play_sound(unsigned char* sound_data);