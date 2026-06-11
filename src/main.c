/**
 * @file main.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdlib.h>
#include <string.h>
#include <PDCurses/curses.h>

#include "ui/ui.h"
#include "interpreter/interpreter.h"

#ifndef KEY_ESC
#define KEY_ESC 27
#endif

int main(void) {
    init_ui();

    UIState state;

    state.last_event = 0;

    state.editor_buffer = malloc(EDITOR_BUFFER_SIZE);
    if (state.editor_buffer == nullptr) {
        printf("Error: malloc returned nullptr for allocating editor buffer, aborting");
        goto exit_failure;
    }

    char prog[] = "++++++++\n[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]\n>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>+.";
    state.editor_buffer = prog;
    state.editor_buffer_len = strlen(prog);

    state.cursor_pos.x = 0;
    state.cursor_pos.y = 0;

    state.in_menubar = false;
    state.current_menubar_option = 0;

    state.dirty.panel_borders = true;
    state.dirty.menubar = true;
    state.dirty.editor = true;
    state.dirty.output = true;
    state.dirty.tape = true;

    while (true) {
        if (COLS > 40 && COLS < 200
            && LINES > 10)
        {
            resize_ui();

            if (state.dirty.panel_borders) draw_panel_borders();
            if (state.dirty.menubar) draw_menubar();
            if (state.dirty.editor) draw_editor_panel(&state);
            if (state.dirty.output) draw_output_panel(&state);
            if (state.dirty.tape) draw_tape_panel(&state);

            doupdate();
        }

        state.dirty.panel_borders = false;
        state.dirty.menubar = false;
        state.dirty.editor = false;
        state.dirty.output = false;
        state.dirty.tape = false;

        bool valid = false;
        while (!valid) {
            valid = true;
            state.last_event = getch();

            switch (state.last_event) {
                case KEY_ESC:
                    goto exit_success;

                case KEY_RESIZE:
                    clear();
                    state.dirty.panel_borders = true;
                    state.dirty.menubar = true;
                    state.dirty.editor = true;
                    state.dirty.output = true;
                    state.dirty.tape = true;
                    break;

                case KEY_LEFT:
                    if (state.cursor_pos.x > 0) {
                        state.dirty.editor = true;
                        --state.cursor_pos.x;
                    }
                    break;
                case KEY_RIGHT:
                    if (state.cursor_pos.x < state.editor_buffer_len) {
                        state.dirty.editor = true;
                        ++state.cursor_pos.x;
                    }
                    break;

                case KEY_ALT_L:
                case KEY_ALT_R:
                    state.in_menubar = !state.in_menubar;
                    break;

                default:
                    valid = false;
                    break;
            }
        }
    }

    // char prog[] = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    // run_brainfuck(prog, sizeof(prog) - 1);

exit_success:
    shutdown_ui();
    return 0;

exit_failure:
    shutdown_ui();
    return 0;
}
