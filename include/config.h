#if !defined( CONFIG_H )
#define CONFIG_H

/*
 * User configuration file
 * Copyright (C) 2018 Michael Goehler
 *
 * This file is part of mdp.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

// unordered list characters
//
// you can also override via env vars:
// export MDP_LIST_OPEN1="    " MDP_LIST_OPEN2="    " MDP_LIST_OPEN3="    "
// export MDP_LIST_HEAD1=" ■  " MDP_LIST_HEAD2=" ●  " MDP_LIST_HEAD3=" ▫  "
// or export MDP_LIST_OPEN to override all MDP_LIST_OPENx variables
// and similarly for MDP_LIST_HEAD
static const char *list_open1 = " |  ";
static const char *list_open2 = " |  ";
static const char *list_open3 = " |  ";
static const char *list_head1 = " +- ";
static const char *list_head2 = " +- ";
static const char *list_head3 = " +- ";

#define FADE_DELAY 15000    // micro seconds
#define GOTO_SLIDE_DELAY 5  // tenths of seconds

// colors - you can only set in 8-bit color mode
//
/* Use the ncurses defined colors, here's a list of them:
 *      COLOR_BLACK
 *      COLOR_RED
 *      COLOR_GREEN
 *      COLOR_YELLOW
 *      COLOR_BLUE
 *      COLOR_MAGENTA
 *      COLOR_CYAN
 *      COLOR_WHITE
 */
#define FG_COLOR        COLOR_WHITE
#define BG_COLOR        COLOR_BLACK
#define TITLE_COLOR     COLOR_YELLOW
#define HEADER_COLOR    COLOR_BLUE
#define BOLD_COLOR      COLOR_RED

// color ramp for fading from black to color
static short white_ramp[24] = { 16, 232, 233, 234, 235, 236,
                               237, 238, 239, 240, 241, 242,
                               244, 245, 246, 247, 248, 249,
                               250, 251, 252, 253, 254, 255 };

static short blue_ramp[24]  = { 16,  17,  17,  18,  18,  19,
                                19,  20,  20,  21,  27,  33,
                                32,  39,  38,  45,  44,  44,
                                81,  81,  51,  51, 123, 123 };

static short red_ramp[24]   = { 16,  52,  52,  53,  53,  89,
                                89,  90,  90, 126, 127, 127,
                               163, 163, 164, 164, 200, 200,
                               201, 201, 207, 207, 213, 213 };

// color ramp for fading from white to color
static short white_ramp_invert[24] = { 15, 255, 254, 254, 252, 251,
                                      250, 249, 248, 247, 246, 245,
                                      243, 242, 241, 240, 239, 238,
                                      237, 236, 235, 234, 233, 232 };

static short blue_ramp_invert[24]  = { 15, 231, 231, 195, 195, 159,
                                      159, 123, 123,  87,  51,  44,
                                       45,  38,  39,  32,  33,  33,
                                       26,  26,  27,  27,  21,  21 };

static short red_ramp_invert[24]   = { 15, 231, 231, 224, 224, 225,
                                      225, 218, 218, 219, 212, 213,
                                      206, 207, 201, 200, 199, 199,
                                      198, 198, 197, 197, 196, 196 };

// keybindings
static const int prev_slide_binding[] = {
    KEY_UP,
    KEY_LEFT,
    KEY_PPAGE,
    8,   // BACKSPACE (ascii)
    127, // BACKSPACE (xterm)
    263, // BACKSPACE (getty)
    'h',
    'k',
    0
};
static const int next_slide_binding[] = {
    KEY_DOWN,
    KEY_RIGHT,
    KEY_NPAGE,
    '\n', // ENTER
    ' ',  // SPACE
    'j',
    'l',
    0
};
static const int first_slide_binding[] = {
    'g',
    KEY_HOME,
    0
};
static const int last_slide_binding[] = {
    'G',
    KEY_END,
    0
};
static const int reload_binding[] = {
    'r',
    0
};
static const int quit_binding[] = {
    'q',
    0
};

#endif // !defined( CONFIG_H )
