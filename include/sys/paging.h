#ifndef PAGING_H
#define PAGING_H

typedef struct __page_t {
   unsigned int present    : 1;
   unsigned int rw         : 1;
   unsigned int user       : 1;
   unsigned int accessed   : 1;
   unsigned int dirty      : 1;
   unsigned int unused     : 7;
   unsigned int frame      : 20;
} page_t;

void quantum_paging_init();

unsigned int* alloc_page();

#endif