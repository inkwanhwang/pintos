#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
/************* Project 2-3 System Call *************/
#define MAX_ARGS 128
typedef int pid_t;
#include <stdbool.h>

#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
/***************************************************/

void syscall_init (void);

/************* Project 2-3 System Call *************/
static void fd_entry_init (struct fd_entry *fd_entry, struct file *file, struct list *fd_table_list);
int get_fd_size (struct list *fd_table_list);
void is_accessing_user_memory(const void *vaddr);
static void is_safe_access (char *input);
static void is_safe_access_with_size (char *input, unsigned size);

void halt (void);
void exit (int status);
pid_t exec (const char *cmd_line);
int wait (pid_t pid);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);
struct lock *syscall_get_filesys_lock(void);
/***************************************************/
#endif /* userprog/syscall.h */
