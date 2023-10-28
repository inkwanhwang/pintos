#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
/************* Project 2-3 System Call *************/
typedef int pid_t;
/***************************************************/

void syscall_init (void);
/************* Project 2-3 System Call *************/
void is_accessing_user_memory(const void *vaddr);
/***************************************************/
#endif /* userprog/syscall.h */
