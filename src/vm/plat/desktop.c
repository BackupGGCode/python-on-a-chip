/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2006 Dean Hall
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#undef __FILE_ID__
#define __FILE_ID__ 0x50
/**
 * PyMite platform-specific routines for Desktop target
 *
 * Log
 * ---
 *
 * 2007/01/10   #75: Added time tick service for desktop (POSIX) and AVR. (P.Adelt)
 * 2006/12/26   #65: Create plat module with put and get routines
 */

/* PyMite build process uses -ansi which disables certain features that
 * in turn disable features needed for signal processing. To work around
 * this, temporarily disable the corresponding #define. This is not
 * needed for Cygwin but for Linux. The -ansi option of GCC is explained
 * here: http://gcc.gnu.org/onlinedocs/gcc-4.0.3/gcc/C-Dialect-Options.html
 */
#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#include <stdio.h>
#define __STRICT_ANSI__
#else
#include <stdio.h>
#endif
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "../pm.h"

/***************************************************************
 * Globals
 **************************************************************/

/***************************************************************
 * Prototypes
 **************************************************************/

void plat_sigalrm_handler(int signal);

/***************************************************************
 * Functions
 **************************************************************/

/* Desktop target shall use stdio for I/O routines. */
PmReturn_t
plat_init(void)
{
    /* Let POSIX' SIGALRM fire every full millisecond. */
    struct sigaction alarmaction;
    memset(&alarmaction, 0, sizeof(struct sigaction));
    alarmaction.sa_handler = plat_sigalrm_handler;

    sigaction(SIGALRM, &alarmaction, NULL);
    ualarm(1000, 1000);

    return PM_RET_OK;
}

void
plat_sigalrm_handler(int signal)
{
    pm_vmPeriodic(1000);
}

/* Desktop target shall use stdio for I/O routines */
PmReturn_t
plat_getByte(uint8_t *b)
{
    int c;
    PmReturn_t retval = PM_RET_OK;

    c = getchar();
    *b = c & 0xFF;

    if (c == EOF)
    {
        PM_RAISE(retval, PM_RET_EX_IO);
    }

    return retval;
}


/* Desktop target shall use stdio for I/O routines */
PmReturn_t
plat_putByte(uint8_t b)
{
    int i;
    PmReturn_t retval = PM_RET_OK;

    i = putchar(b);

    if ((i != b) || (i == EOF))
    {
        PM_RAISE(retval, PM_RET_EX_IO);
    }

    return retval;
}
