#ifndef _LOONGARCH_ASM_H
#define _LOONGARCH_ASM_H

#include <machine/regdef.h>

# define SZREG  8
# define SZFREG 8
# define REG_L ld.d
# define REG_S st.d
# define FREG_L fld.d
# define FREG_S fst.d

/* Declare leaf routine.  */
#define LEAF_NO_ALIGN(symbol)           \
        .text;                          \
        .globl  symbol;                 \
        .type   symbol, @function;      \
symbol:

/* Mark end of function.  */
#undef END
#define END(function)                   \
        .size   function,.-function;

#endif
