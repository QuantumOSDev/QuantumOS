#include <quantum/init.h>

#include <core/string.h>
#include <core/kpanic.h>

#include <sys/process.h>
#include <sys/memory.h>

static __process_t *__phead;
int __pid = 1;

__process_t *process_find_by_pid(unsigned int __pid)
{
    __process_t *__head = __phead;

    if (!__head)
    {
        quantum_info(1, " PROC   ", "Fatal error process head is NULL!");

        kpanic("Fatal error occured kernel crashed");
    }

    while (__head != NULL)
    {
        if (__head->__pid == __pid)
        {
            return __head;
        }

        __head = __head->__next;
    }

    return NULL;
}

void process_initialize(void)
{
    __phead = kmalloc(sizeof(*__phead));

    __phead->__entry    = NULL;
    __phead->__memsize  = 0;
    __phead->__mode     = PROCESS_MODE_KERNEL;
    __phead->__next     = NULL;
    __phead->__pstatus  = PROCESS_SLEEPING;
    __phead->__priority = 0;

    quantum_info(0, " PROC   ", "Successfully initialized processes");
}

void process_alloc(__process_t *__p)
{
    __p->__entry = kmalloc(__p->__memsize);
}

void process_run(__process_t *__p)
{
    quantum_info(0, " PROC   ", "Running process [%d]...", __p->__pid);

    // int __exit = ((int (*) (void)) __p->__entry)();
    create_and_run_task(__p->__task, __p->__entry);

    quantum_info(0, " PROC   ", "Process [%d] finished and is now a zombie process...", __p->__pid);

    process_kill(__p->__pid);
}

unsigned int process_spawn(void *__entry, unsigned int __mode, unsigned int __memsize, unsigned int __pstatus, unsigned int __instant_run)
{
    __process_t *__p = kmalloc(sizeof(*__p));

    __p->__memsize = __memsize;
    __p->__pstatus = __pstatus;
    __p->__mode    = __mode;
    __p->__next    = NULL;

    process_alloc(__p);

    __p->__entry = __entry;
    __p->__task = (task_t*)kmalloc(sizeof(task_t));

    __p->__pid = process_get_pid();

    __process_t *__head = __phead;

    while (__head->__next != NULL && __head->__pstatus != PROCESS_ZOMBIE)
    {
        __head = __head->__next;
    }

    __head->__next = __p;

    if (__instant_run == 1)
    {
        process_run(__p);
    }

    return __p->__pid;
}

void process_kill(unsigned int __pid)
{
    __process_t *__head = __phead;

    while (__head != NULL)
    {
        if (__head->__pid == __pid)
        {
            process_set_status(__pid, PROCESS_ZOMBIE);

            kfree(__head->__entry);
        }

        __head = __head->__next;
    }
}

void process_set_status(unsigned int __pid, __process_status __status)
{
    __process_t *__head = __phead;

    while (__head != NULL)
    {
        if (__head->__pid == __pid)
        {
            __head->__pstatus = __status;
        }

        __head = __head->__next;
    }
}

unsigned int process_get_pid(void)
{
    return __pid++;
}