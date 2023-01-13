#include <sys/process.h>
#include <sys/memory.h>

#include <quantum/init.h>

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
    quantum_info(0, " Process", "Starting process with pid %d", get_pid());
    int process_return = ((int (*)(void))process->entry)();
    quantum_info(0, " Process", "Process with pid %d, returned with %d", get_pid(), process_return);
}

process_t* get_current_process()
{
    return curr_process;
}

int get_pid()
{
    return curr_process->pid;
}