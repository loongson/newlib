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

#ifndef	_SYS_FENV_H_
#define	_SYS_FENV_H_

#include <sys/_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	__fenv_static
#define	__fenv_static	static
#endif

typedef	int	fenv_t;
typedef	int	fexcept_t;

/* Exception flags */
#define	_FCSR_CAUSE_SHIFT	8

#define FE_INEXACT      0x010000
#define FE_UNDERFLOW    0x020000
#define FE_OVERFLOW     0x040000
#define FE_DIVBYZERO    0x080000
#define FE_INVALID      0x100000

#define FE_ALL_EXCEPT \
        (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

/* Rounding modes */
#define	FE_TONEAREST	0x000
#define	FE_TOWARDZERO	0x100
#define	FE_UPWARD	0x200
#define	FE_DOWNWARD	0x300
/* Mask for rounding control.  */
#define	_ROUND_MASK	0x300

/* Masks for interrupts.  */
#define _FPU_MASK_V     0x10  /* Invalid operation */
#define _FPU_MASK_Z     0x08  /* Division by zero  */
#define _FPU_MASK_O     0x04  /* Overflow          */
#define _FPU_MASK_U     0x02  /* Underflow         */
#define _FPU_MASK_I     0x01  /* Inexact operation */

#define _ENABLE_MASK     0x0000001FU
#define _ENABLE_SHIFT    16

/* Default floating-point environment */
extern const fenv_t	*_fe_dfl_env;
#define	FE_DFL_ENV	(_fe_dfl_env)

#ifndef __loongarch_soft_float
#define _FPU_GETCW(cw) __asm__ volatile ("movfcsr2gr %0,$r0" : "=r" (cw))
#define _FPU_SETCW(cw) __asm__ volatile ("movgr2fcsr $r0,%0" : : "r" (cw))
#endif

#if __BSD_VISIBLE
int feenableexcept(int);
int fedisableexcept(int);
int fegetexcept(void);
#endif /* __BSD_VISIBLE */

#ifdef __cplusplus
}
#endif

#endif	/* !_FENV_H_ */
