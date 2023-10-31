#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  lock_init(&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  /************* Project 2-3 System Call *************/
  int size_arg = sizeof(uintptr_t);
  int check_end = sizeof(uintptr_t) - 1;
  is_accessing_user_memory(f->esp);
  is_accessing_user_memory(f->esp + check_end);
  void *sysnum_addr = f->esp;
  int sysnum = *(int*)sysnum_addr;

  switch (sysnum)
  {
    case SYS_HALT: // 0 args
      halt();
      break;

    case SYS_EXIT: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("EXIT\n");
      exit(*(int*)(sysnum_addr + 1 * size_arg));
      break;

    case SYS_EXEC: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("EXEC\n");
      f->eax = exec(*(const char**)(sysnum_addr + 1 * size_arg));
      break;

    case SYS_WAIT: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("WAIT\n");
      f->eax = wait(*(pid_t*)(sysnum_addr + 1 * size_arg));
      break;

    case SYS_CREATE: // 2 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 2 * size_arg);
      is_accessing_user_memory(sysnum_addr + 2 * size_arg + check_end);
      //printf("CREATE\n");
      f->eax = create(*(const char**)(sysnum_addr + 1 * size_arg), *(unsigned*)(sysnum_addr + 2 * size_arg));
      break;

    case SYS_REMOVE: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("REMOVE\n");
      f->eax = remove(*(const char**)(sysnum_addr + 1 * size_arg));
      break;

    case SYS_OPEN: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("OPEN\n");
      f->eax = open(*(const char**)(sysnum_addr + 1 * size_arg));
      break;

    case SYS_FILESIZE: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("FILESIZE\n");
      f->eax = filesize(*(int*)(sysnum_addr + 1 * size_arg));
      break;
    
    case SYS_READ: // 3 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 2 * size_arg);
      is_accessing_user_memory(sysnum_addr + 3 * size_arg);
      is_accessing_user_memory(sysnum_addr + 3 * size_arg + check_end);
      //printf("READ\n");
      f->eax = read(*(int*)(sysnum_addr + 1 * size_arg), *(void**)(sysnum_addr + 2 * size_arg), *(unsigned*)(sysnum_addr + 3 * size_arg));
      break;

    case SYS_WRITE: // 3 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 2 * size_arg);
      is_accessing_user_memory(sysnum_addr + 3 * size_arg);
      is_accessing_user_memory(sysnum_addr + 3 * size_arg + check_end);
      //printf("WRITE\n");
      f->eax = write(*(int*)(sysnum_addr + 1 * size_arg), *(const void**)(sysnum_addr + 2 * size_arg), *(unsigned*)(sysnum_addr + 3 * size_arg));
      break;

    case SYS_SEEK: // 2 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 2 * size_arg);
      is_accessing_user_memory(sysnum_addr + 2 * size_arg + check_end);
      //printf("SEEK\n");
      seek(*(int*)(sysnum_addr + 1 * size_arg), *(unsigned*)(sysnum_addr + 2 * size_arg));
      break;

    case SYS_TELL: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("TELL\n");
      f->eax = tell(*(int*)(sysnum_addr + 1 * size_arg));
      break;

    case SYS_CLOSE: // 1 args
      is_accessing_user_memory(sysnum_addr + 1 * size_arg);
      is_accessing_user_memory(sysnum_addr + 1 * size_arg + check_end);
      //printf("CLOSE\n");
      close(*(int*)(sysnum_addr + 1 * size_arg));
      break;

    default:
      //printf("DEFAULT\n");
      exit(-1);
  }
  /***************************************************/
  
  // printf("system call!\n");
  // thread_exit();
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
  is_safe_access(cmd_line);

  int cmd_size = strlen(cmd_line) + 1;
  char *file_name = palloc_get_page(0);
  strlcpy(file_name, cmd_line, cmd_size);
  //is_accessing_user_memory(file_name);
  //is_safe_access(file_name);
	pid_t pid = process_execute(file_name);
  palloc_free_page(file_name);
  for(e = list_begin(&thread_current()->children_list); e != list_end(&thread_current()->children_list); e = list_next(e))
  {
    child = list_entry(e, struct pcb, children_elem);
    if (pid == child->pid) break;
  }
  // pid가 잘못되거나, child pocess가 load 되지 않으면 -1 리턴
	if(child == NULL || child->load_done == false){
		pid = -1;
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
  //printf("@@ FILE : %s \n", file);
  //printf("@@ INITIAL_SIZE : %d \n", initial_size);
  if (file == NULL) exit(-1);
  is_accessing_user_memory(file);
  is_safe_access(file);
  
  lock_acquire(&filesys_lock);
  bool success = filesys_create(file, (off_t)initial_size);
  lock_release(&filesys_lock);
  return success;
}

bool
remove (const char *file)
{
  if (file == NULL) exit(-1);
  is_accessing_user_memory(file);
  is_safe_access(file);
  
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
  is_accessing_user_memory(file);
  is_safe_access(file);
  
  struct file *f;
  struct fd_entry *fd_entry = palloc_get_page(0);

  if(!fd_entry) return -1;

  lock_acquire(&filesys_lock);
  f = filesys_open(file);
  if(!f){
    palloc_free_page(fd_entry);
    lock_release(&filesys_lock);
    return -1;
  }

  fd_entry_init(fd_entry, f, &thread_current()->fd_table_list);
  list_push_back(&thread_current()->fd_table_list, &fd_entry->fd_table_elem);
  lock_release(&filesys_lock);
  return fd_entry->fd;
}

int
filesize (int fd)
{
  struct fd_entry *fd_entry;
  struct list *fd_table;
  int filesize;

  struct list_elem *e;
  bool found = false;

  fd_table = &thread_current()->fd_table_list;
  for (e = list_begin(fd_table); e != list_end(fd_table); e = list_next(e))
  {
    fd_entry = list_entry(e, struct fd_entry, fd_table_elem);
    if (fd_entry->fd == fd)
    {
      found = true;
      break;
    }
  }

  if (found == true && fd_entry != NULL)
  {
    lock_acquire(&filesys_lock);
    filesize = file_length(fd_entry->file);
    lock_release(&filesys_lock);
  }
  else filesize = -1;
  return filesize;
}

int
read (int fd, void *buffer, unsigned size)
{
  is_accessing_user_memory(buffer);
  is_safe_access_with_size(buffer, size);
  int i;
  struct fd_entry *fd_entry;
  struct list *fd_table;

  struct list_elem *e;
  bool found = false;
  off_t read_bytes;

  if(fd == 0)
  {
    uint8_t *buf = (uint8_t *)buffer;
    for(i=0; i<size; i++)
      buf[i] = input_getc();
    return size;
  }

  fd_table = &thread_current()->fd_table_list;
  for (e = list_begin(fd_table); e != list_end(fd_table); e = list_next(e))
  {
    fd_entry = list_entry(e, struct fd_entry, fd_table_elem);
    if (fd_entry->fd == fd)
    {
      found = true;
      break;
    }
  }
  if(!found || !fd_entry)
    return -1;
  lock_acquire(&filesys_lock);
  read_bytes = file_read(fd_entry->file, buffer, (off_t)size);
  lock_release(&filesys_lock);
  return read_bytes;
}

int
write (int fd, const void *buffer, unsigned size)
{
  is_accessing_user_memory(buffer);
  is_safe_access_with_size(buffer, size);

  struct fd_entry *fd_entry;
  struct list *fd_table;
  int write_bytes;

  if (fd == 1) // STDOUT
  {
    putbuf(buffer, size);
    write_bytes = size;
  }

  struct list_elem *e;
  bool found = false;

  fd_table = &thread_current()->fd_table_list;
  for (e = list_begin(fd_table); e != list_end(fd_table); e = list_next(e))
  {
    fd_entry = list_entry(e, struct fd_entry, fd_table_elem);
    if (fd_entry->fd == fd)
    {
      found = true;
      break;
    }
  }
  if (found == false) fd_entry = NULL;
  //printf("FOUND = %d\n", found);
  if (found == true && fd_entry != NULL)
  {
    lock_acquire(&filesys_lock);
    write_bytes = (int)file_write(fd_entry->file, buffer, size);
    lock_release(&filesys_lock);
    //printf("FILE WRITE: WRITE BYTES = %d\n", write_bytes);
  }
  else write_bytes = -1;
  return write_bytes;
}

void
seek (int fd, unsigned position)
{
  struct fd_entry *fd_entry;
  struct list *fd_table;

  struct list_elem *e;
  bool found = false;
  
  fd_table = &thread_current()->fd_table_list;
  for (e = list_begin(fd_table); e != list_end(fd_table); e = list_next(e))
  {
    fd_entry = list_entry(e, struct fd_entry, fd_table_elem);
    if (fd_entry->fd == fd)
    {
      found = true;
      break;
    }
  }
  
  if (found == true && fd_entry != NULL)
  {
    lock_acquire(&filesys_lock);
    file_seek(fd_entry->file, (off_t)position);
    lock_release(&filesys_lock);
  }
  else return;
}

unsigned
tell (int fd)
{
  struct fd_entry *fd_entry;
  struct list *fd_table;
  unsigned position;

  struct list_elem *e;
  bool found = false;

  fd_table = &thread_current()->fd_table_list;
  for (e = list_begin(fd_table); e != list_end(fd_table); e = list_next(e))
  {
    if (e == NULL) break;
    fd_entry = list_entry(e, struct fd_entry, fd_table_elem);
    if (fd_entry->fd == fd)
    {
      found = true;
      break;
    }
  }

  if (found == true && fd_entry != NULL)
  {
    lock_acquire(&filesys_lock);
    position = (unsigned) file_tell(fd_entry->file);
    lock_release(&filesys_lock);
  }
  else position = -1;
  return position;
}

void
close (int fd)
{
  struct fd_entry *fd_entry;
  struct list *fd_table;

  struct list_elem *e;
  bool found = false;

  fd_table = &thread_current()->fd_table_list;
  for (e = list_begin(fd_table); e != list_end(fd_table); e = list_next(e))
  {
    if (e == NULL) break;
    fd_entry = list_entry(e, struct fd_entry, fd_table_elem);
    if (fd_entry->fd == fd)
    {
      found = true;
      break;
    }
  }

  if(!found || !fd_entry)
    return;
  lock_acquire(&filesys_lock);
  file_close(fd_entry->file);
  list_remove(&fd_entry->fd_table_elem);
  palloc_free_page(fd_entry);
  lock_release(&filesys_lock);
}

static
void fd_entry_init (struct fd_entry *fd_entry, struct file *file, struct list *fd_table_list)
{
  if(list_empty(fd_table_list))
    fd_entry->fd = 2;
  else
    fd_entry->fd = (list_entry(list_back(fd_table_list), struct fd_entry, fd_table_elem)->fd) + 1;
  
  fd_entry->file = file;
  return;
}

static void
is_safe_access (char *input)
{
  int i = 1;
  int size = 126;
  while (i < size)
  {
    if ((input + i - 1) == NULL) break;
    is_accessing_user_memory(input + i);
    i++;
  }
}

static void
is_safe_access_with_size (char *input, unsigned size)
{
  int i = 1;
  while (i < size)
  {
    if ((input + i - 1) == NULL) break;
    is_accessing_user_memory(input + i);
    i++;
  }
}
/***************************************************/