/**
 * @file main.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "interpreter/interpreter.h"
#include "ui/ui.h"

int main(void) {
    char prog[] = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    run_brainfuck(prog, sizeof(prog) - 1);

    return 0;
}