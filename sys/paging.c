/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <sys/paging.h>

#include <quantum/kernel.h>

unsigned int page_directory[1024] __attribute__((aligned(4096)));
unsigned int first_page_table[1024] __attribute__((aligned(4096)));

int page_directory_curr = 0;

extern void load_page_directory(unsigned int* page_ptr);
extern void enable_paging();

void quantum_paging_init()
{
    for(int i = 0; i < 1024; i++)
        page_directory[i] = 0x00000002;

    for(unsigned int i = 0; i < 1024; i++)
        first_page_table[i] = (i * 0x1000) | 3;

    page_directory[0] = ((unsigned int)first_page_table) | 3;

    load_page_directory(((unsigned int*)page_directory));
    enable_paging();
}

unsigned int* alloc_page()
{
    int copy_curr = page_directory_curr;
    page_directory_curr++;
    return ((unsigned int*)page_directory[copy_curr]);
}