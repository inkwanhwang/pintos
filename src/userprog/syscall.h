#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
/************* Project 2-3 System Call *************/
#define MAX_ARGS 128
typedef int pid_t;
#include <stdbool.h>
/***************************************************/

void syscall_init (void);
/************* Project 2-3 System Call *************/
void is_accessing_user_memory(const void *vaddr);

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
/***************************************************/
#endif /* userprog/syscall.h */
