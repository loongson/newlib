%C%_src = \
	%D%/fenv.c \
	%D%/feclearexcept.c \
	%D%/fegetenv.c \
	%D%/fegetexceptflag.c \
	%D%/fegetround.c \
	%D%/feholdexcept.c \
	%D%/feraiseexcept.c \
	%D%/fesetenv.c \
	%D%/fesetexceptflag.c \
	%D%/fesetround.c \
	%D%/fetestexcept.c \
	%D%/feupdateenv.c

libm_a_SOURCES += $(%C%_src)
