#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

/* ====== newly implemeted for prj1 ====== */
char* get_command(const char* file_name);
int set_argv(char* file_name, char** argv);
void set_stack(int argc, char** argv, void** esp);
/* ======================================= */


#endif /* userprog/process.h */
