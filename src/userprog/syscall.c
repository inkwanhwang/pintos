#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
/************* Project 2-3 System Call *************/
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "filesys/filesys.h"
/***************************************************/

/************* Project 2-3 System Call *************/
struct lock filesys_lock;
/***************************************************/
static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&filesys_lock);
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

    case SYS_CREATE: // 2 args
      is_accessing_user_memory(sysnum_addr + 2 * size_arg + check_end);
      f->eax = create((const char *)*(sysnum_addr + 1 * size_arg), (unsigned)*(sysnum_addr + 2 * size_arg));
      break;

    case SYS_REMOVE: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = remove((const char *)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_OPEN: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      f->eax = open((const char *)*(sysnum_addr + 1 * size_arg));
      break;

    case SYS_FILESIZE: // 1 args
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
  if (!is_user_vaddr(addr) || addr == NULL || !pagedir_get_page(thread_current()->pagedir, addr))
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
  thread_current()->pcb->exit_code = status;
  /********************************************************************/
	thread_exit();
}

pid_t
exec (const char *cmd_line)
{
  struct list_elem *e;
  struct pcb *child = NULL;
	is_accessing_user_memory(cmd_line);

	pid_t pid = process_execute(cmd_line);


  for(e = list_begin(&thread_current()->children_list); e != list_end(&thread_current()->children_list); e = list_next(e))
  {
    child = list_entry(e, struct pcb, children_elem);
  }
  // pid가 잘못되거나, child pocess가 load 되지 않으면 -1 리턴
	if(child == NULL || child->load_done == false){
		return -1;
	}
	return pid;
}

int
wait (pid_t pid)
{
	return process_wait((tid_t) pid);
}

bool
create (const char *file, unsigned initial_size)
{
  lock_acquire(&filesys_lock);
  bool success = filesys_create(file, (off_t)initial_size);
  lock_release(&filesys_lock);
  return success;
}

bool
remove (const char *file)
{
  lock_acquire(&filesys_lock);
  bool success = filesys_remove(file);
  lock_release(&filesys_lock);
  return success;
}

int
open (const char *file)
{
  // Returns a file descriptor
  // If file is not valid (could not open), return -1
  struct file *f;
  //struct fi

  lock_acquire(&filesys_lock);
  f = filesys_open(file);
  if(!f)
  {
    lock_release(&filesys_lock);
    return -1;
  }
}

int
filesize (int fd)
{
  return 0;
}

int
read (int fd, void *buffer, unsigned size)
{
  return 0;
}

int
write (int fd, const void *buffer, unsigned size)
{
  return 0;
}

void
seek (int fd, unsigned position)
{
  return;
}

unsigned
tell (int fd)
{
  return 0;
}

void
close (int fd)
{
  return;
}
/***************************************************/