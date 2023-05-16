/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2004-2005 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */
#include <float.h>

__fenv_static inline int
feclearexcept(int excepts)
{
#ifndef __loongarch_soft_float
  int cw;

  /* Mask out unsupported bits/exceptions.  */
  excepts &= FE_ALL_EXCEPT;

  /* Read the complete control word.  */
  _FPU_GETCW (cw);

  /* Clear exception flag bits and cause bits. If the cause bit is not
     cleared, the next CTC instruction (just below) will re-generate the
     exception.  */

  cw &= ~(excepts | (excepts << _FCSR_CAUSE_SHIFT));

  /* Put the new data in effect.  */
  _FPU_SETCW (cw);
#endif

  return 0;
}

__fenv_static inline int
fegetexceptflag(fexcept_t *flagp, int excepts)
{
#ifndef __loongarch_soft_float
  int temp;

  /* Get the current exceptions.  */
  _FPU_GETCW (temp);

  /* We only save the relevant bits here. In particular, care has to be
     taken with the CAUSE bits, as an inadvertent restore later on could
     generate unexpected exceptions.  */

  *flagp = temp & excepts & FE_ALL_EXCEPT;
#endif

  return 0;
}

__fenv_static inline int
fesetexceptflag(const fexcept_t *flagp, int excepts)
{
#ifndef __loongarch_soft_float
  int temp;

  /* Get the current exceptions.  */
  _FPU_GETCW (temp);

  /* Make sure the flags we want restored are legal.  */
  excepts &= FE_ALL_EXCEPT;

  /* Now clear the bits called for, and copy them in from flagp. Note that
     we ignore all non-flag bits from *flagp, so they don't matter.  */
  temp = (temp & ~excepts) | (*flagp & excepts);

  _FPU_SETCW (temp);
#endif

  return 0;
}

__fenv_static inline int
feraiseexcept(int excepts)
{
#ifndef __loongarch_soft_float
  const float fp_zero = 0.0, fp_one = 1.0, fp_1e32 = 1.0e32f, fp_two = 2.0, fp_three = 3.0;
  const float fp_min = FLT_MIN;
  const float fp_max = FLT_MAX;

  /* Raise exceptions represented by EXPECTS.  But we must raise only
     one signal at a time.  It is important that if the overflow/underflow
     exception and the inexact exception are given at the same time,
     the overflow/underflow exception follows the inexact exception.*/

  /* First: invalid exception.  */
  if (FE_INVALID & excepts)
    __asm__ __volatile__ (
			  "fdiv.s $f0,%0,%0\n\t"
			  :
			  : "f" (fp_zero)
			  :"$f0");

  /* Next: division by zero.  */
  if (FE_DIVBYZERO & excepts)
    __asm__ __volatile__ (
			  "fdiv.s $f0,%0,%1\n\t"
			  :
			  : "f" (fp_one), "f" (fp_zero)
			  :"$f0");

  /* Next: overflow.  */
  if (FE_OVERFLOW & excepts)
    /* There's no way to raise overflow without also raising inexact.  */
    __asm__ __volatile__ (
			  "fadd.s $f0,%0,%1\n\t"
			  :
			  : "f" (fp_max), "f" (fp_1e32)
			  : "$f0");

  /* Next: underflow.  */
  if (FE_UNDERFLOW & excepts)
    __asm__ __volatile__ (
			  "fdiv.s $f0,%0,%1\n\t"
			  :
			  : "f" (fp_min), "f" (fp_three)
			  : "$f0");

  /* Last: inexact.  */
  if (FE_INEXACT & excepts)
    __asm__ __volatile__ (
			  "fdiv.s $f0, %0, %1\n\t"
			  :
			  : "f" (fp_two), "f" (fp_three)
			  : "$f0");
#endif

  return 0;
}

__fenv_static inline int
fetestexcept(int excepts)
{
#ifndef __loongarch_soft_float
  int cw;

  /* Get current control word.  */
  _FPU_GETCW (cw);

  return cw & excepts & FE_ALL_EXCEPT;
#else
  return 0;
#endif
}

__fenv_static inline int
fegetround(void)
{
#ifndef __loongarch_soft_float
  int cw;

  /* Get control word.  */
  _FPU_GETCW (cw);

  return cw & _ROUND_MASK;
#else
#ifdef FE_TONEAREST
  return FE_TONEAREST;
#else
  return 0;
#endif
#endif
}

__fenv_static inline int
fesetround(int rounding_mode)
{
#ifndef __loongarch_soft_float
  int cw;

  if ((rounding_mode & ~_ROUND_MASK) != 0)
    /* ROUND is no valid rounding mode.  */
    return 1;

  /* Get current state.  */
  _FPU_GETCW (cw);

  /* Set rounding bits.  */
  cw &= ~_ROUND_MASK;
  cw |= rounding_mode;
  /* Set new state.  */
  _FPU_SETCW (cw);
#endif

  return 0;
}

__fenv_static inline int
fegetenv(fenv_t *envp)
{
#ifndef __loongarch_soft_float
  _FPU_GETCW (*envp);
#endif

  return 0;
}

__fenv_static inline int
feholdexcept(fenv_t *envp)
{
#ifndef __loongarch_soft_float
  int cw;

  /* Save the current state.  */
  _FPU_GETCW (cw);
  *envp = cw;

  /* Clear all exception enable bits and flags.  */
  cw &= ~(_FPU_MASK_V|_FPU_MASK_Z|_FPU_MASK_O|_FPU_MASK_U|_FPU_MASK_I|FE_ALL_EXCEPT);
  _FPU_SETCW (cw);
#endif

  return 0;
}

__fenv_static inline int
fesetenv(const fenv_t *envp)
{
#ifndef __loongarch_soft_float
  _FPU_SETCW (*envp);
#endif

  return 0;
}

__fenv_static inline int
feupdateenv(const fenv_t *envp)
{
#ifndef __loongarch_soft_float
  int temp;

  /* Save current exceptions.  */
  _FPU_GETCW (temp);
  temp &= FE_ALL_EXCEPT;

  /* Install new environment.  */
  fesetenv (envp);

  /* Raise the safed exception.  Incidently for us the implementation
     defined format of the values in objects of type fexcept_t is the
     same as the ones specified using the FE_* constants.  */
  feraiseexcept (temp);
#else
#if defined(FE_NOMASK_ENV) && FE_ALL_EXCEPT != 0
  if (envp == FE_NOMASK_ENV)
    return 1;
#endif
#endif
  return 0;
}

#if __BSD_VISIBLE
__fenv_static inline int
feenableexcept(int __mask)
{
#ifndef __loongarch_soft_float
  unsigned int new_exc, old_exc;

  /* Get the current control word.  */
  _FPU_GETCW (new_exc);

  old_exc = (new_exc & _ENABLE_MASK) << _ENABLE_SHIFT;

  __mask &= FE_ALL_EXCEPT;

  new_exc |= __mask >> _ENABLE_SHIFT;
  _FPU_SETCW (new_exc);
  return old_exc;
#else
  return 0;
#endif
}

__fenv_static inline int
fedisableexcept(int __mask)
{
#ifndef __loongarch_soft_float
  unsigned int new_exc, old_exc;

  /* Get the current control word.  */
  _FPU_GETCW (new_exc);

  old_exc = (new_exc & _ENABLE_MASK) << _ENABLE_SHIFT;

  __mask &= FE_ALL_EXCEPT;

  new_exc &= ~(__mask >> _ENABLE_SHIFT);
  _FPU_SETCW (new_exc);

  return old_exc;
#else
  return 0;
#endif
}

__fenv_static inline int
fegetexcept(void)
{
#ifndef __loongarch_soft_float
  unsigned int exc;

  /* Get the current control word.  */
  _FPU_GETCW (exc);

  return (exc & _ENABLE_MASK) << _ENABLE_SHIFT;
#else
  return 0;
#endif
}
#endif /* __BSD_VISIBLE */
