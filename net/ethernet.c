/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <net/ethernet.h>

#include <drivers/pci.h>

#include <quantum/init.h>

int ethernet_initalize(__pci_device_t* __dev)
{
    quantum_info(__FILE__, 0, "Ethernet", "Initializing Ethernet driver");
}