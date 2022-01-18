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

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	check_address(f->esp);

	int syscall = *(int*)(f->esp);
	switch (syscall) {
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

void check_address(const void* addr) {
	if (!is_user_vaddr(addr)) 
		exit(-1);
}

void halt(void) {
	shutdown_power_off();
}

void exit(int status) {
	struct thread* cur = thread_current();
	cur->exit_status = status;
	printf("%s: exit(%d)\n", cur->name, status);

	thread_exit();
}

pid_t exec(const char* filename) {
	char* command = get_command(filename);
	if (filesys_open(command) == NULL)
		return -1;

	return (pid_t)process_execute(filename);
}

int wait(pid_t pid) {
	return process_wait(pid);
}

int read(int fd, void* buffer, unsigned size) { 
	if (fd == 0) {
		for (int i = 0; i < size; i++)
			*(char*)(buffer + i) = input_getc();
		return size;
	}
	else return -1;
}

int write(int fd, const void* buffer, unsigned size) {
	if (fd == 1) {
		putbuf(buffer, size);
		return size;
	}
	else return -1;
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
