/**
 * @file interpreter.h
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include <stdint.h>
#include "../ui/ui.h"
#include "../helpers.h"

#define INCREMENT_PTR '>'
#define DECREMENT_PTR '<'
#define INCREMENT_VAR '+'
#define DECREMENT_VAR '-'
#define OUTPUT_CHAR '.'
#define INPUT_BYTE ','
#define LOOP_BEGIN '['
#define LOOP_END ']'

void run_brainfuck(
    UIState* state,
    const char* prog,
    size_t prog_len,
    int (*read_input)(UIState* state)
);

void generate_tape_to_cursor_pos(
    uint8_t tape[],
    unsigned int* data_ptr,
    UIState* state,
    int (*read_input)(),
    bool write_output
);

void generate_tape_to_instruction_count(
    uint8_t tape[],
    unsigned int* data_ptr,
    UIState* state,
    int (*read_input)(),
    bool write_output,
    int step_limit,
    int* out_cursor_pos
);

int find_step_count_for_row(UIState* state, int row, int* out_target_pos);
