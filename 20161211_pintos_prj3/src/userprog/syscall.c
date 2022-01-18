#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "threads/synch.h"
#include "devices/input.h"
#include "userprog/process.h"

struct semaphore mutex;

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
	sema_init(&mutex, 1);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	check_address(f->esp);
	
	int syscall = *(int*)(f->esp);
	switch (syscall) {
	/* ====== newly implemeted for prj1 ====== */
	case SYS_HALT:
		halt();
		break;
	case SYS_EXIT: {
		check_address(f->esp + 4);

		int status = *(int*)(f->esp + 4);
		exit(status);
		break;
	}
	case SYS_EXEC: {
		check_address(f->esp + 4);

		const char* filename = *(char**)(f->esp + 4);
		f->eax = exec(filename);
		break;
	}
	case SYS_WAIT: {
		check_address(f->esp + 4);

		pid_t pid = *(pid_t*)(f->esp + 4);
		f->eax = wait(pid);
		break;
	}
	/* ====== newly implemeted for prj2 ====== */
	case SYS_CREATE: {
		for (int i = 1; i <= 2; i++)
			check_address(f->esp + 4 * i);

		const char* file = *(char**)(f->esp + 4);
		unsigned initial_size = *(unsigned*)(f->esp + 8);
		f->eax = create(file, initial_size);
		break;
	}
	case SYS_REMOVE: {
		check_address(f->esp + 4);

		const char* file = *(char**)(f->esp + 4);
		f->eax = remove(file);
		break;
	}
	case SYS_OPEN: {
		check_address(f->esp + 4);

		const char* file = *(char**)(f->esp + 4);
		f->eax = open(file);
		break;
	}
	case SYS_FILESIZE: {
		check_address(f->esp + 4);

		int fd = *(int*)(f->esp + 4);
		f->eax = filesize(fd);
		break;

	}
	/* ====== newly implemeted for prj1 ====== */
	case SYS_READ: {
		for (int i = 1; i <= 3; i++)
			check_address(f->esp + 4 * i);

		int fd = *(int*)(f->esp + 4);
		void* buffer = *(void**)(f->esp + 8);
		unsigned size = *(unsigned*)(f->esp + 12);
		f->eax = read(fd, buffer, size);
		break;
	}
	case SYS_WRITE: {
		for (int i = 1; i <= 3; i++) 
			check_address(f->esp + 4 * i);

		int fd = *(int*)(f->esp + 4);
		const void* buffer = *(void**)(f->esp + 8);
		unsigned size = *(unsigned*)(f->esp + 12);
		f->eax = write(fd, buffer, size);
		break;
	}
	/* ====== newly implemeted for prj2 ====== */
	case SYS_SEEK: {
		for (int i = 1; i <= 2; i++)
			check_address(f->esp + 4 * i);

		int fd = *(int*)(f->esp + 4);
		unsigned position = *(unsigned*)(f->esp + 8);
		seek(fd, position);
		break;
	}
	case SYS_TELL: {
		check_address(f->esp + 4);

		int fd = *(int*)(f->esp + 4);
		f->eax = tell(fd);
		break;
	}
	case SYS_CLOSE: {
		check_address(f->esp + 4);

		int fd = *(int*)(f->esp + 4);
		close(fd);
		break;
	}
	/* ====== additional implemeted for prj1 ====== */
	case SYS_FIBO: {
		check_address(f->esp + 4);

		int n = *(int*)(f->esp + 4);
		f->eax = fibonacci(n);
		break;
	}
	case SYS_MAX: {
		for (int i = 1; i <= 4; i++) 
			check_address(f->esp + 4 * i);

		int a = *(int*)(f->esp + 4);
		int b = *(int*)(f->esp + 8);
		int c = *(int*)(f->esp + 12);
		int d = *(int*)(f->esp + 16);
		f->eax = max_of_four_int(a, b, c, d);
		break;
	}
	}
}

/* ====== newly implemeted for prj1 ====== */
void check_address(const void* addr) {
	if (!is_user_vaddr(addr)) 
		exit(-1);
}

void halt(void) {
	shutdown_power_off();
}

int wait(pid_t pid) {
	return process_wait(pid);
}

int fibonacci(int n) {
	int* fib = (int*)malloc(sizeof(int) * (n + 1));
	fib[0] = 0;
	fib[1] = 1;
	for (int i = 2; i <= n; i++)
		fib[i] = fib[i - 1] + fib[i - 2];

	int res = fib[n];
	free(fib);

	return res;
}

int max_of_four_int(int a, int b, int c, int d) {
	int m1 = a > b ? a : b;
	int m2 = c > d ? c : d;
	return m1 > m2 ? m1 : m2;
}

/* ====== modified for prj2 ====== */
void exit(int status) {
	struct thread* cur = thread_current();
	cur->exit_status = status;

	struct file* file;
	for (int i = 2; i < 130; i++) {
		if (!cur->fd_used[i]) continue;
		file = cur->files[i];
		file_close(file);
		sema_down(&(cur->fd_wait));
		cur->fd_used[i] = false;
		cur->files[i] = NULL;
		sema_up(&(cur->fd_wait));
	}
	
	printf("%s: exit(%d)\n", cur->name, status);

	thread_exit();
}

pid_t exec(const char* filename) {
	char* command = get_command(filename);
	int res = -1;

	struct file* file = filesys_open(command);
	if (file != NULL)
		res = (pid_t)process_execute(filename);

	if (!(thread_current()->child_load_success))
		res = -1;

	return res;
}


/* ====== newly implemeted for prj2 ====== */

bool create(const char* file, unsigned initial_size) {
	if (file == NULL) exit(-1);
	return filesys_create(file, initial_size);
}

bool remove(const char* file) {
	return filesys_remove(file);
}

int open(const char* file) {
	if (file == NULL) return -1;
	int res = -1;

	struct thread* cur = thread_current();

	sema_down(&mutex);
	struct file* new_file = filesys_open(file);

	if (new_file != NULL) {
		for (int i = 2; i < 130; i++) {
			if (cur->fd_used[i] == false) {
				sema_down(&(cur->fd_wait));
				cur->files[i] = new_file;
				cur->fd_used[i] = true;
				sema_up(&(cur->fd_wait));

				res = i;
				break;
			}
		}

		if (!strcmp(file, cur->name)) {
			cur->current_file = new_file;
		}
	}

	sema_up(&mutex);
	
	return res;
}

int filesize(int fd) {
	struct thread* cur = thread_current();
	struct file* file = cur->files[fd];

	return (int)file_length(file);
}

int read(int fd, void* buffer, unsigned size) {
	int res = -1;

	check_address(buffer);
	
	sema_down(&mutex);

	if (fd == 0) {
		for (int i = 0; i < size; i++)
			*(char*)(buffer + i) = input_getc();
		res = size;
	}
	else if (2 <= fd && fd < 130) {
		struct thread* cur = thread_current();
		struct file* file = cur->files[fd];
		if (file != NULL) 
			res = (int)file_read(file, buffer, size);
	}

	sema_up(&mutex);

	return res;
}

int write(int fd, const void* buffer, unsigned size) {
	int res = -1;
	check_address(buffer);
	
	sema_down(&mutex);

	if (fd == 1) {
		putbuf(buffer, size);
		res = size;
	}
	else if (2 <= fd && fd < 130) {
		struct thread* cur = thread_current();
		struct file* file = cur->files[fd];
		if (file != NULL) {
			if (cur->current_file == file)
				file_deny_write(file);
			res = (int)file_write(file, buffer, size);
		}	
	}

	sema_up(&mutex);

	return res;
}

void seek(int fd, unsigned position) {
	struct thread* cur = thread_current();
	struct file* file = cur->files[fd];

	file_seek(file, position);
}

unsigned tell(int fd) {
	struct thread* cur = thread_current();
	struct file* file = cur->files[fd];

	return (int)file_tell(file);
}

void close(int fd) {
	struct thread* cur = thread_current();
	struct file* file = cur->files[fd];

	file_close(file);

	sema_down(&(cur->fd_wait));
	cur->fd_used[fd] = false;
	cur->files[fd] = NULL;
	sema_up(&(cur->fd_wait));
}