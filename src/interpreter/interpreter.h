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
    int (*read_input)(void)
);

void generate_tape(
    uint8_t tape[],
    unsigned int* data_ptr,
    UIState* state,
    int (*read_input)(void),
    bool write_output
);
