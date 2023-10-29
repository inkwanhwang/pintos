#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
/************* Project 2-3 System Call *************/
#include "threads/vaddr.h"
/***************************************************/

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  /************* Project 2-3 System Call *************/
  is_accessing_user_memory(f->esp);
  uintptr_t *sysnum_addr = f->esp;
  int sysnum = *sysnum_addr;
  int size_arg = sizeof(uintptr_t);
  int check_end = sizeof(uintptr_t) - 1;
  
  switch (sysnum)
  {
    case SYS_HALT: // 0 args
      halt();
      break;

    case SYS_EXIT: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      exit((int)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_EXEC: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = exec((const char *)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_WAIT: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = wait((pid_t)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_CREATE: // 2 args // 이상함
      is_accessing_user_memory(sysnum_addr + 2 * size_arg + check_end);
      f->eax = create((const char *)*(sysnum_addr + 1 * size_arg), (unsigned)*(sysnum_addr + 2 * size_arg));
      break;

    case SYS_REMOVE: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = remove((const char *)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_OPEN: // 1 args // 이상함
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = open((const char *)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_FILESIZE: // 1 args // 이상함
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = filesize((int)*(sysnum_addr + 1 * size_arg));
      break;
    
    case SYS_READ: // 3 args
      is_accessing_user_memory(sysnum_addr + 3 * size_arg + check_end);
      f->eax = read((int)*(sysnum_addr + 1 * size_arg), (void *)*(sysnum_addr + 2 * size_arg), (unsigned)*(sysnum_addr + 3 * size_arg));
      break;

    case SYS_WRITE: // 3 args
      is_accessing_user_memory(sysnum_addr + 3 * size_arg + check_end);
      f->eax = write((int)*(sysnum_addr + 1 * size_arg), (const void *)*(sysnum_addr + 2 * size_arg), (unsigned)*(sysnum_addr + 3 * size_arg));
      break;

    case SYS_SEEK: // 2 args
      is_accessing_user_memory(sysnum_addr + 2 * size_arg + check_end);
      seek((int)*(sysnum_addr + 1 * size_arg), (unsigned)*(sysnum_addr + 2 * size_arg));
      break;

    case SYS_TELL: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = tell((int)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_CLOSE: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      close((int)*(sysnum_addr + 1 * size_arg));
      break;

    default:
      exit(-1);
  }
  /***************************************************/
  
  printf("system call!\n");
  thread_exit();
}

/************* Project 2-3 System Call *************/

void
is_accessing_user_memory (const void *addr)
{
  if (!is_user_vaddr(addr) || addr == NULL)
  {
    exit(-1);
  }
}

void
halt (void)
{
	shutdown_power_off();
}

void
exit (int status)
{
  /************* Project 2-1 Process Termination Messages *************/
	printf("%s: exit(%d)\n", thread_name(), status);
  /********************************************************************/
	thread_exit();
}

// pid_t
// exec (const char *cmd_line)
// {
// 	is_accessing_user_memory(cmd_line);

// 	pid_t pid; // pid_t have to be implemented
// 	pid = process_execute(cmd_line);

// 	if(can_load_pid(pid) || can_run_pid(pid)){
// 		return -1;
// 	}
// 	return pid;
// }

// int
// wait (pid_t pid)
// {
// 	return process_wait((tid_t) pid);
// }
/***************************************************/