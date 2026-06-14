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

void run_brainfuck(
    UIState* state,
    const char* const prog,
    const size_t prog_len,
    int (*read_input)(void))
{
    uint8_t tape[TAPE_LEN] = {};
    unsigned int data_ptr = 0;

    memset(state->output_buffer, 0, state->output_buffer_len);
    state->output_buffer_len = 0;

    for (int i = 0; i < prog_len; ++i) {
        switch (prog[i]) {
            case INCREMENT_PTR: {
                if (data_ptr < TAPE_LEN) ++data_ptr;
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
                tape[data_ptr] = read_input();
                break;
            }

            case LOOP_BEGIN: {
                if (tape[data_ptr] == 0) {
                    int depth = 1;
                    while (depth != 0) {
                        ++i;
                        if (prog[i] == LOOP_BEGIN) depth++;
                        if (prog[i] == LOOP_END) depth--;
                    }
                }
                break;
            }
            case LOOP_END: {
                if (tape[data_ptr] != 0) {
                    int depth = 1;
                    while (depth != 0) {
                        --i;
                        if (prog[i] == LOOP_END) depth++;
                        if (prog[i] == LOOP_BEGIN) depth--;
                    }
                }
                break;
            }

            default: break;
        }
    }
}
