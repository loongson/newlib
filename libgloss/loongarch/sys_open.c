#include <machine/syscall.h>
#include "internal_syscall.h"
#include <fcntl.h>

/* Open a file.  */
int
_open(const char *name, int flags, int mode)
{
  return syscall_errno (SYS_openat, 4, AT_FDCWD, name, flags, mode, 0, 0);
}
