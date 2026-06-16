/**
 * @file draw_tape.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui.h"

#include <stdlib.h>
#include "../interpreter/interpreter.h"
#include "../helpers.h"

bool is_displayable_char(char c) {
    if (
        (c >= ' ' && c <= 'z')
        || c == ASCII_NUL
        || c == ASCII_CR
        || c == ASCII_BEL
        || c == KEY_ESC
        || c == KEY_ENTER
        || c == KEY_TAB
        || c == KEY_BACKSPACE
    ) return true;

    return false;
}

void draw_tape_panel(UIState* state) {
    uint8_t tape[TAPE_LEN] = {};
    unsigned int data_ptr = 0;

    if (state->mode == Debug) {
        memcpy(tape, state->debug_tape, TAPE_LEN);
        data_ptr = state->debug_data_ptr;
    } else {
        // yes, i know recomputing the ENTIRE tape EVERY edit and cursor move is stupid af
        // BUUUT im to lazy to do it properly :)
        // (and everything else in this app is also unoptimized asf so what)
        generate_tape(tape, &data_ptr, state, _return_e_as_input, false);
    }

    werase(tape_panel);

    int first = data_ptr - TAPE_MAX_VISIBLE_CELLS / 2;
    if (first < 0) first = 0;

    for (int i = 0; i < TAPE_MAX_VISIBLE_CELLS; ++i) {
        int index = first + i;

        int x = TAPE_PADDING_X + i * (TAPE_CELL_WIDTH + TAPE_CELL_SPACING);

        if (is_displayable_char((char)tape[index])) {
            const auto c = (char)tape[index];

            switch (c) {
                case ASCII_NUL: mvwprintw(tape_panel, TAPE_PADDING_Y, x, " NUL "); break;
                case ASCII_CR: mvwprintw(tape_panel, TAPE_PADDING_Y, x, " CR  "); break;
                case ASCII_BEL: mvwprintw(tape_panel, TAPE_PADDING_Y, x, " BEL "); break;
                case KEY_SPACE: mvwprintw(tape_panel, TAPE_PADDING_Y, x, "SPACE"); break;
                case KEY_ESC: mvwprintw(tape_panel, TAPE_PADDING_Y, x, " ESC "); break;
                case KEY_ENTER: mvwprintw(tape_panel, TAPE_PADDING_Y, x, " LF  "); break;
                case KEY_TAB: mvwprintw(tape_panel, TAPE_PADDING_Y, x, " TAB "); break;
                case KEY_BACKSPACE: mvwprintw(tape_panel, TAPE_PADDING_Y, x, " BS  "); break;
                default: mvwprintw(tape_panel, TAPE_PADDING_Y, x, "  %c  ", c); break;
            }
        }

        if (index == data_ptr)
            wattron(tape_panel, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));

        mvwprintw(tape_panel, TAPE_PADDING_Y + 1, x, "[%03u]", tape[index]);
        mvwprintw(tape_panel, TAPE_PADDING_Y + 2, x, " %03u ", index);

        if (index == data_ptr)
            wattroff(tape_panel, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
    }

    wnoutrefresh(tape_panel);
}
