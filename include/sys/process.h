#ifndef PROCESS_H
#define PROCESS_H

typedef struct process_t {
    int pid;
    void* entry;
} process_t;

process_t* init_process(void* entry);

process_t* get_current_process();

void start_process(process_t* process);

int get_pid();

#endif