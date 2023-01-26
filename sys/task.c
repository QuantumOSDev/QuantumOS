/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <sys/task.h>
#include <sys/paging.h>

#include <core/print.h>

static task_t* current_task;
static task_t main_task;
static task_t other_task;

void other_main()
{
    fork();
}

void quantum_task_init() 
{
    asm volatile("movl %%cr3, %%eax\nmovl %%eax, %0\n" : "=m"(main_task.regs.cr3) : : "%eax");
    asm volatile("pushfl\nmovl (%%esp), %%eax\nmovl %%eax, %0\npopfl\n" : "=m"(main_task.regs.eflags) : : "%eax");
 
    create_task(&other_task, other_main, main_task.regs.eflags, (unsigned int*)main_task.regs.cr3);
    main_task.next = &other_task;
    other_task.next = &main_task;
 
    current_task = &main_task;
    ((void (*)())current_task->next->regs.eip)();
}

void create_and_run_task(task_t* task, task_main_t main)
{
    asm volatile("movl %%cr3, %%eax\nmovl %%eax, %0\n" : "=m"(main_task.regs.cr3) : : "%eax");
    asm volatile("pushfl\nmovl (%%esp), %%eax\nmovl %%eax, %0\npopfl\n" : "=m"(main_task.regs.eflags) : : "%eax");
 
    create_task(task, main, main_task.regs.eflags, (unsigned int*)main_task.regs.cr3);
    main_task.next = task;
    task->next = &main_task; 
 
    current_task = &main_task;
    ((void (*)())current_task->next->regs.eip)();
}

void reset_registers_task(task_t* task)
{
    task->regs.eax    = 0;
    task->regs.ebx    = 0;
    task->regs.ecx    = 0;
    task->regs.edx    = 0;
    task->regs.esi    = 0;
    task->regs.edi    = 0;
}

void set_task_structure(task_t* task, task_main_t main, unsigned int flags, unsigned int *pagedir)
{
    task->regs.eflags = flags;
    task->regs.eip = (unsigned int)main;
    task->regs.cr3 = (unsigned int)pagedir;
    task->regs.esp = (unsigned int)alloc_page() + 0x1000;
    task->next = 0;
}

void create_task(task_t* task, task_main_t main, unsigned int flags, unsigned int *pagedir)
{
    reset_registers_task(task);
    set_task_structure(task, main, flags, pagedir);
}
 
void fork()
{
    task_t* last = current_task;
    current_task = current_task->next;

    switch_task(&last->regs, &current_task->regs);
}