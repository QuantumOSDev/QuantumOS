#ifndef PROCESS_H
#define PROCESS_H

#define PROCESS_MODE_KERNEL 0
#define PROCESS_MODE_USER   1

#include <sys/task.h>

typedef enum
{
    PROCESS_RUNNING,
    PROCESS_SLEEPING,
    PROCESS_ZOMBIE,
} __process_status;

typedef struct __process_t
{
    unsigned int __memsize;
    unsigned int __mode;
    unsigned int __pid;
    
    int __priority;
    int __pstatus;

    struct __process_t *__next;
    
    task_t *__task;
    void *__entry;
} __process_t;

__process_t *process_find_by_pid(unsigned int __pid);

void process_initialize(void);
void process_alloc(__process_t *__p);
void process_run(__process_t *__p);

unsigned int process_spawn(void *__entry, unsigned int __mode, unsigned int __memsize, unsigned int __pstatus, unsigned int __instant_run);

void process_kill(unsigned int __pid);
void process_set_status(unsigned int __pid, __process_status __status);

unsigned int process_get_pid(void);

#endif