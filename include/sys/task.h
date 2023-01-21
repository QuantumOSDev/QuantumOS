#ifndef TASK_H
#define TASK_H
 
#include <sys/isr.h>
 
typedef struct __registers_task_t {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    unsigned int esp;
    unsigned int ebp;
    unsigned int eip;
    unsigned int eflags;
    unsigned int cr3;
} registers_task_t;
 
typedef struct __task_t {
    registers_task_t regs;
    struct __task_t *next;
} task_t;
 
typedef void (*task_main_t)();

void quantum_task_init();

void create_and_run_task(task_t* task, task_main_t main);

void create_task(task_t* task, task_main_t main, unsigned int flags, unsigned int *pagedir);
 
void reset_registers_task(task_t* task);

void fork(); 

extern void switch_task(registers_task_t* _old, registers_task_t* _new); 
 
#endif