#include <machine/syscall.h>
#include "internal_syscall.h"

/* Transfer control to a new process. */

int
_execve(const char *name, char *const argv[], char *const env[])
{
  return syscall_errno (SYS_execve, 3, name, argv, env, 0, 0, 0);
}
