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

static void move_cursor_up(UIState* state) {
    int idx = state->cursor_pos;

    while (idx > 0 && state->editor_buffer[idx - 1] != '\n')
        --idx;

    if (idx == 0)
        return;

    int prev_line_start = idx - 1;
    while (prev_line_start > 0 && state->editor_buffer[prev_line_start - 1] != '\n')
        --prev_line_start;

    const int prev_len = (idx - 1) - prev_line_start;
    const int original_cols = state->cursor_pos - idx;

    state->cursor_pos = prev_line_start + (original_cols < prev_len ? original_cols : prev_len);
    state->dirty.editor = true;
}

static void move_cursor_down(UIState* state) {
    int line_start = state->cursor_pos;
    while (line_start > 0 && state->editor_buffer[line_start - 1] != '\n')
        --line_start;

    const int col = state->cursor_pos - line_start;

    int line_end = state->cursor_pos;
    while (line_end < state->editor_buffer_len && state->editor_buffer[line_end] != '\n')
        ++line_end;

    if (line_end >= state->editor_buffer_len)
        return;

    const int next_line_start = line_end + 1;
    int next_line_end = next_line_start;
    while (next_line_end < state->editor_buffer_len && state->editor_buffer[next_line_end] != '\n')
        ++next_line_end;

    const int next_len = next_line_end - next_line_start;
    state->cursor_pos = next_line_start + (col < next_len ? col : next_len);
    state->dirty.editor = true;
}

int main(void) {
    init_ui();

    UIState state;

    state.last_event = 0;

    state.editor_buffer = malloc(EDITOR_BUFFER_SIZE);
    if (state.editor_buffer == nullptr) {
        printf("Error: malloc returned nullptr for allocating editor buffer, aborting");
        goto exit_failure;
    }

    char prog[] =
        "++++++++\n"
        "[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]\n"
        ">>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.\n"
        "++++++++++[>+++++++>++++++++++>+++>+<<<<-]\n"
        ">++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.\n"
        "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.\n"
        "\n"
        "[-]\n"
        ">[-]<\n"
        "++++++[>++++++++<-]>.\n"
        "<+++++[>+++++<-]>.\n"
        "[-]++++++++[>++++++++<-]>+.\n"
        "<++++++[>------<-]>-.\n"
        "+++++++++++++.\n"
        "---------.\n"
        "++++++.\n"
        "\n"
        "[-]\n"
        "\n"
        "+++++++[>+++++++<-]>.\n"
        "<<++++[>++++++++<-]>[<++++>-]\n"
        "+<[>-<[>>+<<-]]>>[<<+>>-]<[>+<-]\n"
        ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>+.\n"
        "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<-.\n"
        "++++++++++\n"
        "[>+>+++>+++++++>++++++++++<<<<-]\n"
        ">>>++.\n"
        "<<++.\n"
        ">>+++++++.\n"
        "<<<+++.\n"
        ">>>.\n"
        "+++.\n"
        "------.\n"
        "<<<.\n"
        ">>>--------.\n"
        "<<<+.\n"
        "[-]>[-]>[-]>[-]<<<\n"
        "++++++++[>++++++++>+++++++++>++++++++++<<<-]\n"
        ">+.>++.>+++.\n"
        "<<<[-]\n"
        "+++++[>+++++[>+++>++++<<-]<-]\n"
        ">>+++.\n"
        ">----.\n"
        "[-]<[-]<[-]\n"
        "+++++++++++[>++++++++++[>+<-]<-]\n"
        ">>+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.\n";
    state.editor_buffer = prog;
    state.editor_buffer_len = strlen(prog);

    state.cursor_pos = 0;

    state.in_menubar = false;
    state.current_menubar_option = 0;

    state.dirty.panel_borders = true;
    state.dirty.menubar = true;
    state.dirty.editor = true;
    state.dirty.output = true;
    state.dirty.tape = true;

    while (true) {
        const bool any_dirty = state.dirty.panel_borders || state.dirty.menubar
            || state.dirty.editor || state.dirty.output || state.dirty.tape;

        if (COLS > 40 && COLS < 200
            && LINES > 10)
        {
            if (any_dirty) curs_set(0);

            if (state.dirty.panel_borders) draw_panel_borders();
            if (state.dirty.menubar) draw_menubar();
            if (state.dirty.editor) draw_editor_panel(&state);
            if (state.dirty.output) draw_output_panel(&state);
            if (state.dirty.tape) draw_tape_panel(&state);

            ui_set_cursor_pos();

            doupdate();

            if (any_dirty) curs_set(2);
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
                    state.in_menubar = !state.in_menubar;
                    break;

                case KEY_RESIZE:
                    resize_ui();
                    clear();
                    state.dirty.panel_borders = true;
                    state.dirty.menubar = true;
                    state.dirty.editor = true;
                    state.dirty.output = true;
                    state.dirty.tape = true;
                    break;

                case KEY_HOME:
                    if (state.cursor_pos != 0) {
                        state.dirty.editor = true;
                        state.cursor_pos = 0;
                    }
                    break;
                case KEY_END:
                    if (state.cursor_pos != state.editor_buffer_len) {
                        state.dirty.editor = true;
                        state.cursor_pos = state.editor_buffer_len;
                    }
                    break;

                case KEY_LEFT:
                    if (state.cursor_pos > 0) {
                        state.dirty.editor = true;
                        --state.cursor_pos;
                    }
                    break;
                case KEY_RIGHT:
                    if (state.cursor_pos < state.editor_buffer_len) {
                        state.dirty.editor = true;
                        ++state.cursor_pos;
                    }
                    break;
                case KEY_UP:
                    move_cursor_up(&state);
                    break;
                case KEY_DOWN:
                    move_cursor_down(&state);
                    break;

                default:
                    valid = false;
                    break;
            }
        }
    }

exit_success:
    shutdown_ui();
    return 0;

exit_failure:
    shutdown_ui();
    return 0;
}
