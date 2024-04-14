#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/pte.h"
#include "userprog/pagedir.h"
#include "threads/synch.h"

void syscall_init (void);
void sys_exit(int exit_status);
int symlink (char *, char *);

struct lock filesys_lock;

#endif /* userprog/syscall.h */
