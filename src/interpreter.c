/**
 * @file interpreter.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "defs.h"
#include "funcs.h"

#include <stdio.h>
#include <stdint.h>

void run_brainfuck(char* prog, int len) {
    uint8_t tape[TAPE_LEN] = {};
    int data_ptr = 0;

    for (int i = 0; i < len; ++i) {
        switch (prog[i]) {
            case INCREMENT_PTR: {
                ++data_ptr;
                break;
            }
            case DECREMENT_PTR: {
                --data_ptr;
                break;
            }

            case INCREMENT_VAR: {
                ++tape[data_ptr];
                break;
            }
            case DECREMENT_VAR: {
                --tape[data_ptr];
                break;
            }

            case OUTPUT_CHAR: {
                printf("%c", tape[data_ptr]);
                break;
            }

            case INPUT_BYTE: {
                tape[data_ptr] = fgetc(stdin);
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