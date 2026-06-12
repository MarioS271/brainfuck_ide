/**
 * @file draw_editor.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui.h"
#include "../interpreter/interpreter.h"

static void draw_line_number(const int vrow, const int scroll, const int digit_count,
                             const int line_number) {
    if (vrow < scroll || vrow >= scroll + EDITOR_DRAWABLE_HEIGHT)
        return;

    mvwprintw(editor_panel, vrow - scroll + EDITOR_PADDING_Y, EDITOR_PADDING_X,
              "%*d", digit_count, line_number);
}

static void walk_editor_buffer(UIState* state, const int digit_count, const int scroll,
                               const bool draw, int* cursor_vrow, int* cursor_col) {
    int vrow = 0;
    int col = EDITOR_PADDING_X + digit_count + 1;
    int line_number = 1;

    if (draw)
        draw_line_number(vrow, scroll, digit_count, line_number);

    for (int i = 0; i < state->editor_buffer_len; ++i) {
        const char current_char = state->editor_buffer[i];

        if (current_char == '\n') {
            if (!draw && state->cursor_pos == i) {
                *cursor_vrow = vrow;
                *cursor_col = col;
            }

            col = EDITOR_PADDING_X + digit_count + 1;
            ++vrow;
            ++line_number;

            if (draw)
                draw_line_number(vrow, scroll, digit_count, line_number);

            continue;
        }

        if (col > EDITOR_DRAWABLE_WIDTH) {
            col = EDITOR_PADDING_X + digit_count + 1;
            ++vrow;
        }

        if (draw && vrow >= scroll + EDITOR_DRAWABLE_HEIGHT)
            break;

        if (!draw && state->cursor_pos == i) {
            *cursor_vrow = vrow;
            *cursor_col = col;
        }

        if (draw && vrow >= scroll) {
            int color_pair;

            switch (current_char) {
                case INCREMENT_PTR:
                case DECREMENT_PTR:
                    color_pair = COLOR_PAIR(PTR_OP_COLOR_PAIR);
                    break;

                case INCREMENT_VAR:
                case DECREMENT_VAR:
                    color_pair = COLOR_PAIR(VAL_OP_COLOR_PAIR);
                    break;

                case OUTPUT_CHAR:
                    color_pair = COLOR_PAIR(OUT_OP_COLOR_PAIR);
                    break;

                case INPUT_BYTE:
                    color_pair = COLOR_PAIR(IN_OP_COLOR_PAIR);
                    break;

                case LOOP_BEGIN:
                case LOOP_END:
                    color_pair = COLOR_PAIR(LOOP_OP_COLOR_PAIR);
                    break;

                default:
                    color_pair = COLOR_PAIR(DEFAULT_COLOR_PAIR);
                    break;
            }

            wattron(editor_panel, color_pair);
            mvwaddch(editor_panel, vrow - scroll + EDITOR_PADDING_Y, col, current_char);
            wattroff(editor_panel, color_pair);
        }

        ++col;
    }

    if (!draw && state->cursor_pos == state->editor_buffer_len) {
        *cursor_vrow = vrow;
        *cursor_col = col;
    }
}

void draw_editor_panel(UIState* state) {
    werase(editor_panel);

    int total_lines = 1;
    for (int i = 0; i < state->editor_buffer_len; ++i) {
        if (state->editor_buffer[i] == '\n')
            ++total_lines;
    }

    const int digit_count = snprintf(nullptr, 0, "%d", total_lines);

    int cursor_vrow = 0;
    int cursor_col = EDITOR_PADDING_X;
    walk_editor_buffer(state, digit_count, 0, false, &cursor_vrow, &cursor_col);

    if (cursor_vrow < editor_scroll)
        editor_scroll = cursor_vrow;
    if (cursor_vrow >= editor_scroll + EDITOR_DRAWABLE_HEIGHT)
        editor_scroll = cursor_vrow - EDITOR_DRAWABLE_HEIGHT + 1;

    walk_editor_buffer(state, digit_count, editor_scroll, true, nullptr, nullptr);

    editor_cursor_x = cursor_col;
    editor_cursor_y = cursor_vrow - editor_scroll + EDITOR_PADDING_Y;

    wnoutrefresh(editor_panel);
}
