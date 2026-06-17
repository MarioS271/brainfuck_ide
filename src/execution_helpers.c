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

void replay_debug_trace(UIState* state) {
    uint8_t* tape = state->debug.tape;
    unsigned int* data_ptr = &state->debug.data_ptr;

    int jh_idx = (state->debug.jump_head - state->debug.jump_count + DEBUG_JUMP_HISTORY_SIZE) % DEBUG_JUMP_HISTORY_SIZE;
    int jh_consumed = 0;
    int i = 0;

    while (i < state->cursor_pos || jh_consumed < state->debug.jump_count) {
        if (i >= state->editor_buffer_len) break;

        if (jh_consumed < state->debug.jump_count
            && i == state->debug.jump_history[jh_idx].src)
        {
            i = state->debug.jump_history[jh_idx].dst;
            jh_idx = (jh_idx + 1) % DEBUG_JUMP_HISTORY_SIZE;
            ++jh_consumed;
            continue;
        }

        switch (state->editor_buffer[i]) {
            case INCREMENT_PTR:
                if (*data_ptr < TAPE_LEN - 1) ++(*data_ptr);
                break;

            case DECREMENT_PTR:
                if (*data_ptr > 0) --(*data_ptr);
                break;

            case INCREMENT_VAR:
                if (tape[*data_ptr] < 255) ++tape[*data_ptr];
                break;

            case DECREMENT_VAR:
                if (tape[*data_ptr] > 0) --tape[*data_ptr];
                break;

            case OUTPUT_CHAR:
                if (state->output_buffer_len < OUTPUT_BUFFER_SIZE - 1) {
                    snprintf(
                        state->output_buffer + state->output_buffer_len,
                        OUTPUT_BUFFER_SIZE - state->output_buffer_len,
                        "%c", tape[*data_ptr]);
                    ++state->output_buffer_len;
                }
                break;

            case INPUT_BYTE:
                tape[*data_ptr] = _return_e_as_input();
                break;

            default:
                break;
        }
        ++i;
    }
}

void regenerate_debug_tape(UIState* state) {
    memset(state->debug.tape, 0, TAPE_LEN);
    state->debug.data_ptr = 0;
    memset(state->output_buffer, 0, OUTPUT_BUFFER_SIZE);
    state->output_buffer_len = 0;

    replay_debug_trace(state);

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
