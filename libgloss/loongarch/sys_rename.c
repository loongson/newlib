#include <machine/syscall.h>
#include "internal_syscall.h"
#include <fcntl.h>

/* rename file */
int
_rename (const char *old, const char *new)
{
  return syscall_errno (SYS_renameat2, 5, AT_FDCWD, old, AT_FDCWD, new, 0, 0);
}
