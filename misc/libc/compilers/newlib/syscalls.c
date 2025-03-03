/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-11     Meco Man     remove _gettimeofday_r() and _times_r()
 * 2021-02-13     Meco Man     re-implement exit() and abort()
 * 2021-02-21     Meco Man     improve and beautify syscalls
 * 2021-02-24     Meco Man     fix bug of _isatty_r()
 */

#include <reent.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>




// void *_malloc_r(struct _reent *ptr, size_t size)
// {
//     void *result = NULL;

//     return result;
// }

// void *_realloc_r(struct _reent *ptr, void *old, size_t newlen)
// {
//     void *result = NULL;

//     return result;
// }

// void *_calloc_r(struct _reent *ptr, size_t size, size_t len)
// {
//     void *result = NULL;

//     return result;
// }

// void _free_r(struct _reent *ptr, void *addr)
// {
//     return;
// }



void __libc_init_array(void)
{
    /* we not use __libc init_aray to initialize C++ objects */
    /* __libc_init_array is ARM code, not Thumb; it will cause a hardfault. */
}

/* Reentrant versions of system calls.  */
#ifndef _REENT_ONLY
int *__errno(void)
{
    return NULL;
}
#endif

int _getpid_r(struct _reent *ptr)
{
    return 0;
}

int _close_r(struct _reent *ptr, int fd)
{
    return -1;
}

int _execve_r(struct _reent *ptr, const char *name, char *const *argv, char *const *env)
{
    return -1;
}

int _fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
    return -1;
}

int _fork_r(struct _reent *ptr)
{
    return -1;
}

int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
    return -1;
}

int _isatty_r(struct _reent *ptr, int fd)
{
    if (fd >= 0 && fd < 3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int _kill_r(struct _reent *ptr, int pid, int sig)
{
    return -1;
}

int _link_r(struct _reent *ptr, const char *old, const char *new)
{
    return -1;
}

int _wait_r(struct _reent *ptr, int *status)
{
    return -1;
}

mode_t umask(mode_t mask)
{
    return 022;
}

int flock(int fd, int operation)
{
    return 0;
}

_off_t _lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
    return -1;
}

int _mkdir_r(struct _reent *ptr, const char *name, int mode)
{
    return -1;
}

int _open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
    return -1;
}

_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    return -1;
}

int _rename_r(struct _reent *ptr, const char *old, const char *new)
{
    return -1;
}

int _stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
    return -1;
}

int _unlink_r(struct _reent *ptr, const char *file)
{
    return -1;
}

_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    return -1;
}

/* for exit() and abort() */
__attribute__((noreturn)) void _exit(int status)
{
    while (1);
}

/*
These functions are implemented and replaced by the 'common/time.c' file
int _gettimeofday_r(struct _reent *ptr, struct timeval *__tp, void *__tzp);
_CLOCK_T_  _times_r(struct _reent *ptr, struct tms *ptms);
*/
