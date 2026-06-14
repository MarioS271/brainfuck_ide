/**
 * @file draw_output.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui.h"

void draw_output_panel(UIState* state) {
    werase(output_panel);

    int col = OUTPUT_PADDING_X;
    int row = OUTPUT_PADDING_Y;

    for (int i = 0; i < state->output_buffer_len; ++i) {
        const char current_char = state->output_buffer[i];

        if (current_char == '\n') {
            col = OUTPUT_PADDING_X;
            ++row;
            continue;
        }
        if (col > OUTPUT_DRAWABLE_WIDTH) {
            col = OUTPUT_PADDING_X;
            ++row;
        }

        mvwaddch(output_panel, row, col, current_char);
        ++col;
    }

    wnoutrefresh(output_panel);
}
