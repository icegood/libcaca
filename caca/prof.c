/*
 *  libcaca       Colour ASCII-Art library
 *  Copyright (c) 2009 Sam Hocevar <sam@hocevar.net>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/*
 *  This file contains profiling functions. They are not supposed to be
 *  built with release versions.
 */

#include "config.h"

#if !defined(__KERNEL__)
#   include <stdio.h>
#   include <stdarg.h>
#   include <stdlib.h>
#endif

#include "caca.h"
#include "caca_internals.h"

#if defined PROF
static struct caca_stat **stats = NULL;
static int nstats = 0;

void _caca_dump_stats(void)
{
    int i, j;

    fprintf(stderr, "** libcaca profiling stats **\n");

    for (i = 0; i < nstats; i++)
    {
         int64_t total = 0;

         for (j = 0; j < STAT_VALUES; j++)
             total += stats[i]->itable[j];

         fprintf(stderr, " %s: last %i mean %i sliding mean %i\n",
                 stats[i]->name, stats[i]->itable[0],
                 (int)((total + STAT_VALUES / 2) / STAT_VALUES),
                 stats[i]->imean);
/*fprintf(stderr, "%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n",
stats[i]->itable[0], stats[i]->itable[1], stats[i]->itable[2],
stats[i]->itable[3], stats[i]->itable[4], stats[i]->itable[5],
stats[i]->itable[6], stats[i]->itable[7], stats[i]->itable[8],
stats[i]->itable[9], stats[i]->itable[10], stats[i]->itable[11],
stats[i]->itable[12], stats[i]->itable[13], stats[i]->itable[14],
stats[i]->itable[15], stats[i]->itable[16], stats[i]->itable[17],
stats[i]->itable[18], stats[i]->itable[19]);*/
    }

    fprintf(stderr, "** %i counters dumped **\n", nstats);
}

void _caca_init_stat(struct caca_stat *s, const char *format, ...)
{
    int i;

    s->name = malloc(128 * sizeof(char));
    va_list args;
    va_start(args, format);
    vsnprintf(s->name, 128, format, args);
    s->name[127] = '\0';
    va_end(args);

    nstats++;
    stats = realloc(stats, nstats * sizeof(struct caca_stat *));
    stats[nstats - 1] = s;

    for (i = 0; i < STAT_VALUES; i++)
        s->itable[i] = 0;
    s->imean = 0;
}

void _caca_fini_stat(struct caca_stat *s)
{
    int i, j;

    for (i = 0; i < nstats; i++)
    {
        if (stats[i] == s)
        {
            free(stats[i]->name);

            for (j = i + 1; j < nstats; j++)
                stats[j - 1] = stats[j];
            nstats--;
            stats = realloc(stats, nstats * sizeof(struct caca_stats *));

            return;
        }
    }
}

#endif

