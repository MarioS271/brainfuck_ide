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

    generate_tape_to_instruction_count(
        state->debug.tape,
        &state->debug.data_ptr,
        state,
        _return_e_as_input,
        true,
        state->debug.step_count,
        &state->debug.pc
    );

    state->cursor_pos = state->debug.pc;
    state->debug.halted = (state->debug.pc >= (int)state->editor_buffer_len);

    state->dirty.editor = true;
    state->dirty.output = true;
    state->dirty.tape = true;
}
