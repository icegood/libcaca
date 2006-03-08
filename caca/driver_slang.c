/*
 *  libcaca       ASCII-Art library
 *  Copyright (c) 2002-2006 Sam Hocevar <sam@zoy.org>
 *                All Rights Reserved
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the Do What The Fuck You Want To
 *  Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
 */

/** \file driver_slang.c
 *  \version \$Id$
 *  \author Sam Hocevar <sam@zoy.org>
 *  \brief SLang driver
 *
 *  This file contains the libcaca SLang input and output driver
 */

#include "config.h"

#if defined(USE_SLANG)

#if defined(HAVE_SLANG_SLANG_H)
#   include <slang/slang.h>
#else
#   include <slang.h>
#endif

#include <stdlib.h>
#include <string.h>

#if defined(HAVE_SIGNAL_H)
#   include <signal.h>
#endif

#include "caca.h"
#include "caca_internals.h"
#include "cucul.h"
#include "cucul_internals.h"

/*
 * Global variables
 */

/* Tables generated by test/optipal.c */
static int const slang_palette[2*16*16] =
{
     1,  0,   2,  0,   3,  0,   4,  0,   5,  0,   6,  0,   7,  0,   8,  0,
     9,  0,  10,  0,  11,  0,  12,  0,  13,  0,  14,  0,  15,  0,   0,  8,
     8,  7,   7,  8,  15,  7,   7, 15,  15,  9,   9, 15,   1,  9,   9,  1,
     7,  9,   9,  7,   8,  1,   1,  8,   0,  1,  15, 10,  10, 15,   2, 10,
    10,  2,   7, 10,  10,  7,   8,  2,   2,  8,   0,  2,  15, 11,  11, 15,
     3, 11,  11,  3,   7, 11,  11,  7,   8,  3,   3,  8,   0,  3,  15, 12,
    12, 15,   4, 12,  12,  4,   7, 12,  12,  7,   8,  4,   4,  8,   0,  4,
    15, 13,  13, 15,   5, 13,  13,  5,   7, 13,  13,  7,   8,  5,   5,  8,
     0,  5,  15, 14,  14, 15,   6, 14,  14,  6,   7, 14,  14,  7,   8,  6,
     6,  8,   0,  6,   4,  6,   6,  4,  12, 14,  14, 12,   6,  2,   2,  6,
    14, 10,  10, 14,   2,  3,   3,  2,  10, 11,  11, 10,   3,  1,   1,  3,
    11,  9,   9, 11,   1,  5,   5,  1,   9, 13,  13,  9,   5,  4,   4,  5,
    13, 12,  12, 13,   4, 14,   6, 12,  12,  6,  14,  4,   6, 10,   2, 14,
    14,  2,  10,  6,   2, 11,   3, 10,  10,  3,  11,  2,   3,  9,   1, 11,
    11,  1,   9,  3,   1, 13,   5,  9,   9,  5,  13,  1,   5, 12,   4, 13,
    13,  4,  12,  5,   0,  7,   0, 15,  15,  8,   8, 15,  15,  1,   7,  1,
     1,  6,   2,  5,   3,  4,   4,  3,   5,  2,   6,  1,   0,  0,   1,  1,
     9,  6,  10,  5,  11,  4,  12,  3,  13,  2,  14,  1,   2,  2,   3,  3,
     4,  4,   5,  5,   6,  6,   7,  7,  14,  9,   1, 15,   8,  9,   8,  8,
     9,  9,   1,  7,   0,  9,   9,  8,   6,  9,  13, 10,   2, 15,   8, 10,
     7,  2,  15,  2,   2,  7,   0, 10,  10,  8,   5, 10,  12, 11,   3, 15,
     8, 11,   7,  3,  15,  3,   3,  7,   0, 11,  11,  8,   4, 11,  11, 12,
     4, 15,   8, 12,   7,  4,  15,  4,   4,  7,   0, 12,  12,  8,   3, 12,
    10, 13,   5, 15,   8, 13,   7,  5,  15,  5,   5,  7,   0, 13,  13,  8,
     2, 13,   9, 14,   6, 15,   8, 14,   7,  6,  15,  6,   6,  7,   0, 14,
    14,  8,   1, 14,   5,  6,   2,  4,  13, 14,  10, 12,   4,  2,   3,  6,
    12, 10,  11, 14,   6,  3,   1,  2,  14, 11,   9, 10,   2,  1,   5,  3,
    10,  9,  13, 11,   3,  5,   4,  1,  11, 13,  12,  9,   1,  4,   6,  5,
     9, 12,  14, 13,   5, 14,   2, 12,  13,  6,  10,  4,   4, 10,   3, 14,
    12,  2,  11,  6,   6, 11,   1, 10,  14,  3,   9,  2,   2,  9,   5, 11,
    10,  1,  13,  3,   3, 13,   4,  9,  11,  5,  12,  1,   1, 12,   6, 13,
     9,  4,  14,  5,  10, 10,  11, 11,  12, 12,  13, 13,  14, 14,  15, 15,
};

static int const slang_assoc[16*16] =
{
    134, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    28, 135, 214, 86, 219, 91, 133, 127, 26, 23, 240, 112, 245, 117, 141, 126,
    37, 211, 142, 83, 206, 132, 78, 160, 35, 237, 32, 109, 232, 140, 104, 161,
    46, 87, 82, 143, 131, 215, 210, 169, 44, 113, 108, 41, 139, 241, 236, 170,
    55, 222, 203, 130, 144, 94, 75, 178, 53, 248, 229, 138, 50, 120, 101, 179,
    64, 90, 129, 218, 95, 145, 223, 187, 62, 116, 137, 244, 121, 59, 249, 188,
    73, 128, 79, 207, 74, 202, 146, 196, 71, 136, 105, 233, 100, 228, 68, 197,
    122, 153, 162, 171, 180, 189, 198, 147, 16, 25, 34, 43, 52, 61, 70, 18,
    15, 27, 36, 45, 54, 63, 72, 17, 151, 155, 164, 173, 182, 191, 200, 124,
    154, 22, 238, 110, 243, 115, 156, 24, 150, 152, 216, 88, 221, 93, 148, 20,
    163, 235, 31, 107, 230, 165, 102, 33, 159, 213, 250, 85, 208, 157, 80, 29,
    172, 111, 106, 40, 174, 239, 234, 42, 168, 89, 84, 251, 166, 217, 212, 38,
    181, 246, 227, 183, 49, 118, 99, 51, 177, 224, 205, 175, 252, 96, 77, 47,
    190, 114, 192, 242, 119, 58, 247, 60, 186, 92, 184, 220, 97, 253, 225, 56,
    199, 201, 103, 231, 98, 226, 67, 69, 195, 193, 81, 209, 76, 204, 254, 65,
    123, 149, 158, 167, 176, 185, 194, 19, 125, 21, 30, 39, 48, 57, 66, 255,
};

/*
 * Local functions
 */
static void slang_init_palette(void);

#if defined(HAVE_SIGNAL)
static RETSIGTYPE sigwinch_handler(int);
static caca_t *sigwinch_kk; /* FIXME: we ought to get rid of this */
#endif
#if defined(HAVE_GETENV) && defined(HAVE_PUTENV)
static void slang_check_terminal(void);
#endif

static int slang_init_graphics(caca_t *kk)
{
#if defined(HAVE_GETENV) && defined(HAVE_PUTENV)
    slang_check_terminal();
#endif

#if defined(HAVE_SIGNAL)
    sigwinch_kk = kk;
    signal(SIGWINCH, sigwinch_handler);
#endif

    /* Initialise slang library */
    SLsig_block_signals();
    SLtt_get_terminfo();

    if(SLkp_init() == -1)
    {
        SLsig_unblock_signals();
        return -1;
    }

    SLang_init_tty(-1, 0, 1);

    if(SLsmg_init_smg() == -1)
    {
        SLsig_unblock_signals();
        return -1;
    }

    SLsig_unblock_signals();

    SLsmg_cls();
    SLtt_set_cursor_visibility(0);
    SLkp_define_keysym("\e[M", 1001);
    SLtt_set_mouse_mode(1, 0);
    SLsmg_refresh();

    /* Disable scrolling so that hashmap scrolling optimization code
     * does not cause ugly refreshes due to slow terminals */
    SLtt_Term_Cannot_Scroll = 1;

    slang_init_palette();

    /* Disable alt charset support so that we get a chance to have all
     * 256 colour pairs */
    SLtt_Has_Alt_Charset = 0;

    cucul_set_size(kk->qq, SLtt_Screen_Cols, SLtt_Screen_Rows);

    return 0;
}

static int slang_end_graphics(caca_t *kk)
{
    SLtt_set_mouse_mode(0, 0);
    SLtt_set_cursor_visibility(1);
    SLang_reset_tty();
    SLsmg_reset_smg();

    return 0;
}

static int slang_set_window_title(caca_t *kk, char const *title)
{
    /* FIXME */
    return 0;
}

static unsigned int slang_get_window_width(caca_t *kk)
{
    /* Fallback to a 6x10 font */
    return kk->qq->width * 6;
}

static unsigned int slang_get_window_height(caca_t *kk)
{
    /* Fallback to a 6x10 font */
    return kk->qq->height * 10;
}

static void slang_display(caca_t *kk)
{
    int x, y;
    uint8_t *attr = kk->qq->attr;
    uint32_t *chars = kk->qq->chars;
    for(y = 0; y < (int)kk->qq->height; y++)
    {
        SLsmg_gotorc(y, 0);
        for(x = kk->qq->width; x--; )
        {
#if defined(OPTIMISE_SLANG_PALETTE)
            /* If foreground == background, just don't use this colour
             * pair, and print a space instead of the real character. */
            uint8_t fgcolor = *attr & 0xf;
            uint8_t bgcolor = *attr >> 4;
            if(fgcolor != bgcolor)
            {
                SLsmg_set_color(slang_assoc[*attr++]);
                SLsmg_write_char(*chars++ & 0x7f);
            }
            else
            {
                if(fgcolor == CUCUL_COLOR_BLACK)
                    fgcolor = CUCUL_COLOR_WHITE;
                else if(fgcolor == CUCUL_COLOR_WHITE
                         || fgcolor <= CUCUL_COLOR_LIGHTGRAY)
                    fgcolor = CUCUL_COLOR_BLACK;
                else
                    fgcolor = CUCUL_COLOR_WHITE;
                SLsmg_set_color(slang_assoc[fgcolor + 16 * bgcolor]);
                SLsmg_write_char(' ');
                chars++;
                attr++;
            }
#else
            SLsmg_set_color(*attr++);
            SLsmg_write_char(*chars++ & 0x7f);
#endif
        }
    }
    SLsmg_refresh();
}

static void slang_handle_resize(caca_t *kk, unsigned int *new_width,
                                            unsigned int *new_height)
{
    SLtt_get_screen_size();
    *new_width = SLtt_Screen_Cols;
    *new_height = SLtt_Screen_Rows;

    if(*new_width != kk->qq->width || *new_height != kk->qq->height)
        SLsmg_reinit_smg();
}

static unsigned int slang_get_event(caca_t *kk)
{
    unsigned int event;
    int intkey;

    if(kk->resize_event)
    {
        kk->resize_event = 0;
        kk->resize = 1;
        return CACA_EVENT_RESIZE;
    }

    if(!SLang_input_pending(0))
        return CACA_EVENT_NONE;

    /* We first use SLang_getkey() to see whether Esc was pressed
     * alone, then (if it wasn't) we unget the key and use SLkp_getkey()
     * instead, so that escape sequences are interpreted. */
    intkey = SLang_getkey();

    if(intkey != 0x1b /* Esc */ || SLang_input_pending(0))
    {
        SLang_ungetkey(intkey);
        intkey = SLkp_getkey();
    }

    /* If the key was ASCII, return it immediately */
    if(intkey < 0x100)
    {
        return CACA_EVENT_KEY_PRESS | intkey;
    }

    if(intkey == 0x3e9)
    {
        int button = (SLang_getkey() - ' ' + 1) & 0xf;
        unsigned int x = SLang_getkey() - '!';
        unsigned int y = SLang_getkey() - '!';
        _push_event(kk, CACA_EVENT_MOUSE_PRESS | button);
        _push_event(kk, CACA_EVENT_MOUSE_RELEASE | button);

        if(kk->mouse_x == x && kk->mouse_y == y)
            return _pop_event(kk);

        kk->mouse_x = x;
        kk->mouse_y = y;

        return CACA_EVENT_MOUSE_MOTION | (kk->mouse_x << 12) | kk->mouse_y;
    }

    event = CACA_EVENT_KEY_PRESS;

    switch(intkey)
    {
        case SL_KEY_UP: return event | CACA_KEY_UP;
        case SL_KEY_DOWN: return event | CACA_KEY_DOWN;
        case SL_KEY_LEFT: return event | CACA_KEY_LEFT;
        case SL_KEY_RIGHT: return event | CACA_KEY_RIGHT;

        case SL_KEY_IC: return event | CACA_KEY_INSERT;
        case SL_KEY_DELETE: return event | CACA_KEY_DELETE;
        case SL_KEY_HOME: return event | CACA_KEY_HOME;
        case SL_KEY_END: return event | CACA_KEY_END;
        case SL_KEY_PPAGE: return event | CACA_KEY_PAGEUP;
        case SL_KEY_NPAGE: return event | CACA_KEY_PAGEDOWN;

        case SL_KEY_F(1): return event | CACA_KEY_F1;
        case SL_KEY_F(2): return event | CACA_KEY_F2;
        case SL_KEY_F(3): return event | CACA_KEY_F3;
        case SL_KEY_F(4): return event | CACA_KEY_F4;
        case SL_KEY_F(5): return event | CACA_KEY_F5;
        case SL_KEY_F(6): return event | CACA_KEY_F6;
        case SL_KEY_F(7): return event | CACA_KEY_F7;
        case SL_KEY_F(8): return event | CACA_KEY_F8;
        case SL_KEY_F(9): return event | CACA_KEY_F9;
        case SL_KEY_F(10): return event | CACA_KEY_F10;
        case SL_KEY_F(11): return event | CACA_KEY_F11;
        case SL_KEY_F(12): return event | CACA_KEY_F12;
    }

    return CACA_EVENT_NONE;
}

/*
 * XXX: following functions are local
 */

static void slang_init_palette(void)
{
    /* See SLang ref., 5.4.4. */
    static char *slang_colors[16] =
    {
        /* Standard colours */
        "black",
        "blue",
        "green",
        "cyan",
        "red",
        "magenta",
        "brown",
        "lightgray",
        /* Bright colours */
        "gray",
        "brightblue",
        "brightgreen",
        "brightcyan",
        "brightred",
        "brightmagenta",
        "yellow",
        "white",
    };

#if defined(OPTIMISE_SLANG_PALETTE)
    int i;

    for(i = 0; i < 16 * 16; i++)
        SLtt_set_color(i, NULL, slang_colors[slang_palette[i * 2]],
                                slang_colors[slang_palette[i * 2 + 1]]);
#else
    int fg, bg;

    for(bg = 0; bg < 16; bg++)
        for(fg = 0; fg < 16; fg++)
        {
            int i = fg + 16 * bg;
            SLtt_set_color(i, NULL, slang_colors[fg], slang_colors[bg]);
        }
#endif
}

#if defined(HAVE_SIGNAL)
static RETSIGTYPE sigwinch_handler(int sig)
{
    sigwinch_kk->resize_event = 1;

    signal(SIGWINCH, sigwinch_handler);;
}
#endif

#if defined(HAVE_GETENV) && defined(HAVE_PUTENV)
static void slang_check_terminal(void)
{
    char *term, *colorterm, *other;

    term = getenv("TERM");
    colorterm = getenv("COLORTERM");

    if(term && !strcmp(term, "xterm"))
    {
        /* If we are using gnome-terminal, it's really a 16 colour terminal */
        if(colorterm && !strcmp(colorterm, "gnome-terminal"))
        {
            (void)putenv("TERM=xterm-16color");
            return;
        }

        /* Ditto if we are using Konsole */
        other = getenv("KONSOLE_DCOP_SESSION");
        if(other)
        {
            (void)putenv("TERM=xterm-16color");
            return;
        }
    }
}
#endif

/*
 * Driver initialisation
 */

void slang_init_driver(caca_t *kk)
{
    kk->drv.driver = CACA_DRIVER_SLANG;

    kk->drv.init_graphics = slang_init_graphics;
    kk->drv.end_graphics = slang_end_graphics;
    kk->drv.set_window_title = slang_set_window_title;
    kk->drv.get_window_width = slang_get_window_width;
    kk->drv.get_window_height = slang_get_window_height;
    kk->drv.display = slang_display;
    kk->drv.handle_resize = slang_handle_resize;
    kk->drv.get_event = slang_get_event;
}

#endif /* USE_SLANG */

