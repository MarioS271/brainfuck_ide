/**
 * @file interpreter.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "interpreter.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void print_malformed_loop(UIState* state) {
    snprintf(state->output_buffer, OUTPUT_BUFFER_SIZE, "ERROR: malformed loop");
    state->output_buffer_len = strlen(state->output_buffer);
}

void loop_begin(
    int* const i,
    const uint8_t tape[],
    const unsigned int* const data_ptr,
    UIState* state)
{
    if (tape[*data_ptr] == 0) {
        int depth = 1;
        while (depth != 0) {
            ++(*i);
            if (*i >= state->editor_buffer_len) {
                print_malformed_loop(state);
                return;
            }
            if (state->editor_buffer[*i] == LOOP_BEGIN) depth++;
            if (state->editor_buffer[*i] == LOOP_END) depth--;
        }
    }
}

void loop_end(
    int* const i,
    const uint8_t tape[],
    const unsigned int* const data_ptr,
    UIState* state)
{
    if (tape[*data_ptr] != 0) {
        int depth = 1;
        while (depth != 0) {
            --(*i);
            if (*i < 0) {
                print_malformed_loop(state);
                return;
            }
            if (state->editor_buffer[*i] == LOOP_END) depth++;
            if (state->editor_buffer[*i] == LOOP_BEGIN) depth--;
        }
    }
}

void run_brainfuck(
    UIState* state,
    const char* const prog,
    const size_t prog_len,
    int (*read_input)(UIState* state))
{
    uint8_t tape[TAPE_LEN] = {};
    unsigned int data_ptr = 0;

    memset(state->output_buffer, 0, OUTPUT_BUFFER_SIZE);
    state->output_buffer_len = 0;

    for (int i = 0; i < prog_len; ++i) {
        switch (prog[i]) {
            case INCREMENT_PTR: {
                if (data_ptr < TAPE_LEN - 1) ++data_ptr;
                break;
            }
            case DECREMENT_PTR: {
                if (data_ptr > 0) --data_ptr;
                break;
            }

            case INCREMENT_VAR: {
                if (tape[data_ptr] < 255) ++tape[data_ptr];
                break;
            }
            case DECREMENT_VAR: {
                if (tape[data_ptr] > 0) --tape[data_ptr];
                break;
            }

            case OUTPUT_CHAR: {
                if (state->output_buffer_len >= OUTPUT_BUFFER_SIZE - 1) break;
                snprintf(
                    state->output_buffer + state->output_buffer_len,
                    OUTPUT_BUFFER_SIZE - state->output_buffer_len,
                    "%c",
                    tape[data_ptr]
                );
                ++state->output_buffer_len;
                break;
            }

            case INPUT_BYTE: {
                tape[data_ptr] = read_input(state);
                break;
            }

            case LOOP_BEGIN: {
                loop_begin(
                    &i,
                    tape,
                    &data_ptr,
                    state
                );
                break;
            }
            case LOOP_END: {
                loop_end(
                    &i,
                    tape,
                    &data_ptr,
                    state
                );
                break;
            }

            default: break;
        }
    }
}

void generate_tape(
    uint8_t tape[],
    unsigned int* data_ptr,
    UIState* state,
    int (*read_input)(void),
    const bool write_output)
{
    for (int i = 0; i < state->cursor_pos; ++i) {
        switch (state->editor_buffer[i]) {
            case INCREMENT_PTR: {
                if (*data_ptr < TAPE_LEN - 1) ++(*data_ptr);
                break;
            }
            case DECREMENT_PTR: {
                if (*data_ptr > 0) --(*data_ptr);
                break;
            }

            case INCREMENT_VAR: {
                if (tape[*data_ptr] < 255) ++tape[*data_ptr];
                break;
            }
            case DECREMENT_VAR: {
                if (tape[*data_ptr] > 0) --tape[*data_ptr];
                break;
            }

            case OUTPUT_CHAR: {
                if (!write_output) break;
                if (state->output_buffer_len >= OUTPUT_BUFFER_SIZE - 1) break;
                snprintf(
                    state->output_buffer + state->output_buffer_len,
                    OUTPUT_BUFFER_SIZE - state->output_buffer_len,
                    "%c",
                    tape[*data_ptr]
                );
                ++state->output_buffer_len;
                break;
            }

            case INPUT_BYTE: {
                tape[*data_ptr] = read_input();
                break;
            }

            case LOOP_BEGIN: {
                loop_begin(
                    &i,
                    tape,
                    data_ptr,
                    state
                );
                break;
            }
            case LOOP_END: {
                loop_end(
                    &i,
                    tape,
                    data_ptr,
                    state
                );
                break;
            }

            default: break;
        }
    }
}
