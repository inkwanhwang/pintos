#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H
/************* Project 2-2 Argument Passing *************/
#define MAX_ARGS 128
/********************************************************/
#include "threads/thread.h"
/************** Project 2-3 System Call *****************/
#include "threads/synch.h"
typedef int pid_t;
/********************************************************/

/************** Project 2-3 System Call *****************/
struct pcb
{
    const char *filename;
    pid_t pid;
    int exit_code;
    
    struct list fd_table_list;
    struct list_elem children_elem;
    struct thread* parent;

    struct semaphore exit_sema;
    bool exit_done;
    struct semaphore load_sema;
    bool load_done;
};

struct fd_entry
{
    struct list_elem fd_table_elem;
    int fd;
    struct file *file;
};
// static struct lock filesys_lock;
/********************************************************/

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

#endif /* userprog/process.h */
