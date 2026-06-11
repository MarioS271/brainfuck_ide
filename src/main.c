/**
 * @file main.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <PDCurses/curses.h>

#include "ui/ui.h"
#include "interpreter/interpreter.h"

#ifndef KEY_ESC
#define KEY_ESC 27
#endif

int main(void) {
    init_ui();

    UIState state;

    state.dirty.panel_borders = true;
    state.dirty.menubar = true;
    state.dirty.editor = true;
    state.dirty.output = true;
    state.dirty.tape = true;

    state.cursor_pos.x = 0;
    state.cursor_pos.y = 0;
    state.cursor_pos.in_menubar = false;

    while (true) {
        resize_and_clear_ui();

        if (COLS > 40 && COLS < 200
            && LINES > 10)
        {
            if (state.dirty.panel_borders) draw_panel_borders();
            if (state.dirty.menubar) draw_menubar();
            if (state.dirty.editor) draw_editor_panel(&state);
            if (state.dirty.output) draw_output_panel(&state);
            if (state.dirty.tape) draw_tape_panel(&state);

            refresh();
        }

        state.dirty.panel_borders = false;
        state.dirty.menubar = false;
        state.dirty.editor = false;
        state.dirty.output = false;
        state.dirty.tape = false;

        state.last_event = getch();

        switch (state.last_event) {
            case KEY_ESC:
                goto exit;

            case KEY_RESIZE:
                state.dirty.panel_borders = true;
                state.dirty.menubar = true;
                state.dirty.editor = true;
                state.dirty.output = true;
                state.dirty.tape = true;
                break;

            default: break;
        }
    }

    // char prog[] = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    // run_brainfuck(prog, sizeof(prog) - 1);

exit:
    shutdown_ui();
    return 0;
}