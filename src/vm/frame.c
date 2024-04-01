#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/synch.h"
#include "lib/kernel/list.h"
#include "userprog/pagedir.h"
#include "vm/frame.h"


static struct list frame_table;        
static struct list lru_list;          
static struct lock frame_table_lock;   

void frame_table_init(void) {
    list_init(&frame_table);
    list_init(&lru_list);
    lock_init(&frame_table_lock);
}

struct frame_table_entry *frame_evict(void) {
	//TODO
}

/* Allocates a frame, sets up its frame table entry, and returns the frame's physical address. */
void* frame_alloc_and_setup(enum palloc_flags flags, void* page) {
    lock_acquire(&frame_table_lock);
    void* frame = palloc_get_page(flags);
    if (!frame) {
        struct frame_table_entry *evicted_fte = frame_evict();
        frame = evicted_fte->frame; 
    }

    struct frame_table_entry *fte = malloc(sizeof(struct frame_table_entry));
    if (fte == NULL) {
        palloc_free_page(frame);
        lock_release(&frame_table_lock);
        return NULL;
    }

    fte->frame = frame;
    fte->page = page;
    fte->owner = thread_current();
    fte->pinned = true; 
    list_push_back(&frame_table, &fte->elem);
    list_push_back(&lru_list, &fte->lru_elem);

    lock_release(&frame_table_lock);
    return frame;
}

/* Frees a frame and removes its frame table entry. */
void frame_free(void *frame) {
    lock_acquire(&frame_table_lock);
    struct list_elem *e;

    for (e = list_begin(&frame_table); e != list_end(&frame_table); e = list_next(e)) {
        struct frame_table_entry *fte = list_entry(e, struct frame_table_entry, elem);
        if (fte->frame == frame) {
            list_remove(&fte->elem);
            list_remove(&fte->lru_elem);
            palloc_free_page(frame);
            free(fte);
            break;
        }
    }

    lock_release(&frame_table_lock);
}

/* Updates the LRU list when a frame is accessed. */
void frame_accessed(struct frame_table_entry *fte) {
    lock_acquire(&frame_table_lock);
    if(fte) {
        list_remove(&fte->lru_elem);
        list_push_back(&lru_list, &fte->lru_elem);
    }
    lock_release(&frame_table_lock);
}
