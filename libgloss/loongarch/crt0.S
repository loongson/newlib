#include "newlib.h"
#include <machine/asm.h>

  .text
  .global _start
  .type   _start, @function
_start:
  # Clear the bss segment
  la      a0, _edata
  la      a2, _end
  sub.d   a2, a2, a0
  li.d    a1, 0
  la.global  ra, memset
  jirl    ra, ra, 0

#ifdef _LITE_EXIT
  # Make reference to atexit weak to avoid unconditionally pulling in
  # support code.  Refer to comments in __atexit.c for more details.
  .weak   atexit
  la      a0, atexit
  beqz    a0, .Lweak_atexit
  .weak   __libc_fini_array
#endif

  la      a0, __libc_fini_array       # Register global termination functions
  la.global   ra, atexit              #  to be called upon exit
  jirl    ra, ra, 0

#ifdef _LITE_EXIT
.Lweak_atexit:
#endif
  la.global   ra, __libc_init_array   # Run global initialization functions
  jirl    ra, ra, 0

  ld.d    a0, sp, 0                  # a0 = argc
  addi.d  a1, sp, SZREG              # a1 = argv
  li.d    a2, 0                      # a2 = envp = NULL
  la.global  ra, main

  # save orignal sp to s0 and make sp 16 bytes aligned
  move    s0, sp
  bstrins.d  sp, zero, 3, 0

  jirl    ra, ra, 0

  la.global  ra, exit
  jirl    zero, ra, 0
  .size  _start, .-_start
