#include <sys/time.h>
#include <machine/syscall.h>
#include "internal_syscall.h"

int
nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
  return syscall_errno (SYS_nanosleep, 2, rqtp, rmtp, 0, 0, 0, 0);
}
