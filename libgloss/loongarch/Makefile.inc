multilibtool_DATA += \
	%D%/crt0.o

multilibtool_LIBRARIES += %D%/libgloss.a
%C%_libgloss_a_CPPFLAGS = -I$(srcdir)/%D%
%C%_libgloss_a_SOURCES = \
	%D%/nanosleep.c \
	%D%/sys_access.c \
	%D%/sys_chdir.c \
	%D%/sys_chmod.c \
	%D%/sys_chown.c \
	%D%/sys_close.c \
	%D%/sys_conv_stat.c \
	%D%/sys_execve.c \
	%D%/sys_exit.c \
	%D%/sys_faccessat.c \
	%D%/sys_fork.c \
	%D%/sys_fstat.c \
	%D%/sys_fstatat.c \
	%D%/sys_getpid.c \
	%D%/sys_gettimeofday.c \
	%D%/sys_isatty.c \
	%D%/sys_kill.c \
	%D%/sys_link.c \
	%D%/sys_lseek.c \
	%D%/sys_lstat.c \
	%D%/sys_open.c \
	%D%/sys_openat.c \
	%D%/sys_read.c \
	%D%/sys_sbrk.c \
	%D%/sys_stat.c \
	%D%/sys_sysconf.c \
	%D%/sys_times.c \
	%D%/sys_unlink.c \
	%D%/sys_utime.c \
	%D%/sys_rename.c \
	%D%/sys_wait.c \
	%D%/sys_write.c
