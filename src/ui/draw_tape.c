/**
 * @file draw_tape.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui.h"

#include <stdlib.h>
#include "../helpers.h"
#include "../interpreter/interpreter.h"

int _return_e_as_input() {
    // e
    return 'e';
}

bool is_displayable_char(char c) {
    if ((c >= ' ' && c <= 'z') || c == KEY_ENTER || c == '\0')
        return true;

    return false;
}

void draw_tape_panel(UIState* state) {
    uint8_t tape[TAPE_LEN] = {};
    unsigned int data_ptr = 0;

    // yes, i know recomputing the ENTIRE tape EVERY edit and cursor move is stupid af
    // BUUUT im to lazy to do it properly :)
    generate_tape(
        tape,
        &data_ptr,
        state,
        _return_e_as_input
    );

    werase(tape_panel);

    int first = data_ptr - TAPE_MAX_VISIBLE_CELLS / 2;
    if (first < 0) first = 0;

    for (int i = 0; i < TAPE_MAX_VISIBLE_CELLS; ++i) {
        int index = first + i;
        // if (index > data_ptr) break;

        int x = TAPE_PADDING_X + i * (TAPE_CELL_WIDTH + TAPE_CELL_SPACING);

        if (is_displayable_char((char)tape[index])) {
            const auto c = (char)tape[index];

            switch (c) {
                case '\0': mvwprintw(tape_panel, TAPE_PADDING_Y, x, " NUL ", c); break;
                case '\n': mvwprintw(tape_panel, TAPE_PADDING_Y, x, " LF  ", c); break;
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
