#ifndef THREADS_FRAME_H
#define THREADS_FRAME_H

#include <list.h>
#include "threads/palloc.h"
#include "threads/thread.h"

struct frame_table_entry {
    void *frame;               
    void *page;                
    struct thread *owner;     
    struct list_elem elem;     
    struct list_elem lru_elem; 
    bool pinned;               
};

void frame_table_init(void);
void* frame_alloc_and_setup(enum palloc_flags flags, void* page);
void frame_free(void *frame);
void frame_accessed(struct frame_table_entry *fte);

#endif 
