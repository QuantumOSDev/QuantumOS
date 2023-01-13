#include <sys/process.h>

#include <sys/memory.h>

process_t* curr_process;
int pid;

process_t* init_process(void* entry)
{
    // Allocate memory for process structure and set up structure
    process_t* process = (process_t*)kcalloc(1, sizeof(process_t));
    process->entry = entry;
    process->pid = pid++;

    return process;
}

void start_process(process_t* process)
{
    curr_process = process;
    ((void (*)(void))process->entry)();
}

process_t* get_current_process()
{
    return curr_process;
}

int get_pid()
{
    return curr_process->pid;
}