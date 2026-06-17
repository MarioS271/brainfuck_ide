/**
 * @file execution_helpers.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui/ui.h"
#include "macros.h"
#include "interpreter/interpreter.h"
#include <string.h>

int _return_e_as_input() {
    // e
    return 'e';
}

bool is_typable_char(char c) {
    if ((c >= ' ' && c <= 'z') || c == KEY_ENTER || c == KEY_TAB)
        return true;

    return false;
}


void regenerate_debug_tape(UIState* state) {
    memset(state->debug.tape, 0, TAPE_LEN);
    state->debug.data_ptr = 0;
    memset(state->output_buffer, 0, OUTPUT_BUFFER_SIZE);
    state->output_buffer_len = 0;

    generate_tape(
        state->debug.tape,
        &state->debug.data_ptr,
        state,
        _return_e_as_input,
        true
    );

    state->dirty.editor = true;
    state->dirty.output = true;
    state->dirty.tape = true;
}

int debug_loop_begin_find_dst(UIState* state) {
    int i = state->cursor_pos;
    int depth = 1;
    while (depth != 0) {
        ++i;
        if (i >= state->editor_buffer_len) return -1;
        if (state->editor_buffer[i] == LOOP_BEGIN) depth++;
        if (state->editor_buffer[i] == LOOP_END)   depth--;
    }
    return i;
}
int debug_loop_end_find_dst(UIState* state) {
    int i = state->cursor_pos;
    int depth = 1;
    while (depth != 0) {
        --i;
        if (i < 0) return -1;
        if (state->editor_buffer[i] == LOOP_END)   depth++;
        if (state->editor_buffer[i] == LOOP_BEGIN)  depth--;
    }
    return i;
}

void clear_debug_jump_history(UIState* state) {
    state->debug.jump_head = 0;
    state->debug.jump_count = 0;
}
