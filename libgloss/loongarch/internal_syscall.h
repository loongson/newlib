#ifndef _INTERNAL_SYSCALL_H
#define _INTERNAL_SYSCALL_H

#include <errno.h>
#undef errno
extern int errno;

static inline long
__syscall_error(long a0)
{
  errno = -a0;
  return -1;
}

# define __SYSCALL_CLOBBERS \
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8",\
        "memory"

static inline long
__internal_syscall(long n, int argc, long _a0, long _a1, long _a2, long _a3, long _a4, long _a5)
{
  register long syscall_id asm("a7") = n;

  register long a0 asm("a0") = _a0;
  if (argc < 2) {
      asm volatile (
      "syscall 0"
      : "+r"(a0)
      : "r" (syscall_id):
      __SYSCALL_CLOBBERS);
      return a0;
  }

  register long a1 asm("a1") = _a1;
  if (argc == 2) {
      asm volatile (
      "syscall 0"
      : "+r"(a0)
      : "r"(a1), "r"(syscall_id)
      : __SYSCALL_CLOBBERS);
      return a0;
  }

  register long a2 asm("a2") = _a2;
  if (argc == 3) {
      asm volatile (
      "syscall 0"
      : "+r"(a0)
      : "r"(a1), "r"(a2), "r"(syscall_id)
      : __SYSCALL_CLOBBERS);
      return a0;
  }

  register long a3 asm("a3") = _a3;
  if (argc == 4) {
      asm volatile (
      "syscall 0"
      : "+r"(a0)
      : "r"(a1), "r"(a2), "r"(a3), "r"(syscall_id)
      : __SYSCALL_CLOBBERS);
      return a0;
  }

  register long a4 asm("a4") = _a4;
  if (argc == 5) {
      asm volatile
      ("syscall 0"
      : "+r"(a0)
      : "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(syscall_id)
      : __SYSCALL_CLOBBERS);
      return a0;
  }

  register long a5 asm("a5") = _a5;
  asm volatile ("syscall 0"
  : "+r"(a0)
  : "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(syscall_id)
  : __SYSCALL_CLOBBERS);

  return a0;
}

static inline long
syscall_errno(long n, int argc, long _a0, long _a1, long _a2, long _a3, long _a4, long _a5)
{
  long a0 = __internal_syscall (n, argc, _a0, _a1, _a2, _a3, _a4, _a5);

  if (a0 < 0)
    return __syscall_error (a0);
  else
    return a0;
}

#endif
