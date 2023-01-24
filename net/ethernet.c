/*
 * QuantumOS Copyright (c) 2021-2022
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <net/ethernet.h>

#include <drivers/pci.h>

#include <quantum/init.h>

int ethernet_initalize(__pci_device_t* __dev)
{
    quantum_info(0, "Ethernet", "Initializing Ethernet driver");
}