/**
 * @file helpers.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui/ui.h"
#include "macros.h"

void move_cursor_up(UIState* state) {
    int idx = state->cursor_pos;

    while (idx > 0 && state->editor_buffer[idx - 1] != '\n')
        --idx;

    if (idx == 0)
        return;

    int prev_line_start = idx - 1;
    while (prev_line_start > 0 && state->editor_buffer[prev_line_start - 1] != '\n')
        --prev_line_start;

    const int prev_len = (idx - 1) - prev_line_start;
    const int original_cols = state->cursor_pos - idx;

    state->cursor_pos = prev_line_start + (original_cols < prev_len ? original_cols : prev_len);
    state->dirty.editor = true;
    state->dirty.tape = true;
}

void move_cursor_down(UIState* state) {
    int line_start = state->cursor_pos;
    while (line_start > 0 && state->editor_buffer[line_start - 1] != '\n')
        --line_start;

    const int col = state->cursor_pos - line_start;

    int line_end = state->cursor_pos;
    while (line_end < state->editor_buffer_len && state->editor_buffer[line_end] != '\n')
        ++line_end;

    if (line_end >= state->editor_buffer_len)
        return;

    const int next_line_start = line_end + 1;
    int next_line_end = next_line_start;
    while (next_line_end < state->editor_buffer_len && state->editor_buffer[next_line_end] != '\n')
        ++next_line_end;

    const int next_len = next_line_end - next_line_start;
    state->cursor_pos = next_line_start + (col < next_len ? col : next_len);
    state->dirty.editor = true;
    state->dirty.tape = true;
}

void move_to_line_start(UIState* state) {
    int line_start = state->cursor_pos;
    while (line_start > 0 && state->editor_buffer[line_start - 1] != '\n')
        --line_start;

    state->cursor_pos = line_start;
    state->dirty.editor = true;
    state->dirty.tape = true;
}

void move_to_line_end(UIState* state) {
    int line_end = state->cursor_pos;
    while (line_end < state->editor_buffer_len && state->editor_buffer[line_end] != '\n')
        ++line_end;

    state->cursor_pos = line_end;
    state->dirty.editor = true;
    state->dirty.tape = true;
}


bool is_typable_char(char c) {
    if ((c >= ' ' && c <= 'z') || c == KEY_ENTER || c == KEY_TAB)
        return true;

    return false;
}
