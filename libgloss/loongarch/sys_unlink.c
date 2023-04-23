#include <machine/syscall.h>
#include "internal_syscall.h"
#include <fcntl.h>

/* Remove a file's directory entry.  */
int
_unlink(const char *name)
{
  return syscall_errno (SYS_unlinkat, 3, AT_FDCWD, name, 0, 0, 0, 0);
}
