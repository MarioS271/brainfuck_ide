/**
 * @file main.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <PDCurses/curses.h>

#include "helpers.h"
#include "ui/ui.h"
#include "interpreter/interpreter.h"

#include "macros.h"

int main(void) {
#ifdef LOG_KEY_PRESSES
    FILE* key_log_file = fopen("input.log", "w");
#endif

    init_ui();

    UIState state;
    int exitcode;

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
        ">++.>+.+++++++..+++.>++.<<+++,++++++++++++.>.+++.------.--------.>+.>.\n"
        "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.\n"
        "\n"
        "[-]\n"
        ">[-]<\n"
        "++++++[>++++++++<-]>.\n"
        "<+++++[>+++++<-]>.\n"
        "[-]++++++++[>++++++++<-]>+.\n"
        "<++++,+[>---,---<-]>-.\n"
        "+++++++++++++.\n"
        "---------.\n"
        "++++++.\n"
        "\n"
        "[-]\n"
        "\n"
        "+++++++[>+++++++<-]>.\n"
        "<<++++[>++++++++<-]>[<++++>-]\n"
        "+<[>-<[>>+<<-]]>>[<<+>>-]<[>+<-]\n"
        ">>>>>>>>>>>>>>>>>>>>>>>>>>,>>>>>>>>>>>>>>>>>>>>>>>>>>>>,>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>+.\n"
        "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<-.\n"
        "+++++++,+++\n"
        "[>+>+++>+++++++>++++++++++<<<<-]\n"
        ">>>++.\n"
        "<<++.\n"
        ">>+++++++.\n"
        "<<<+++.\n"
        ">>>.\n"
        "+++.\n"
        "------.\n"
        "<<<.\n"
        ">>>-,-------.\n"
        "<<<+.\n"
        "[-]>[-]>[-]>[-]<<<\n"
        "++++++++[>++++++++>+++++++++>++++++++++<<<-]\n"
        ">+.>++.>+++.\n"
        "<<<[-]\n"
        "+++++[>+++++[>+++>++++<<-]<-]\n"
        ">>+++.\n"
        ">----.\n"
        "[-]<[-]<[-]\n"
        "+++++,++++++[>++++++++++[>+<-]<-]\n"
        ">>+++++++++++++++++++++,,,++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.\n"
        "[>+>+++>+++++++>++++++++++<<<<-]\n"
        ">>>++.\n"
        "<<++.\n"
        ">>+++++++.\n"
        "<<<+++.\n"
        ">>>.\n"
        "+++.\n"
        "------.\n"
        "<<<.\n"
        ">>>-,-------.\n"
        "<<<+.\n"
        "[-]>[-]>[-]>[-]<<<\n"
        "++++++++[>++++++++>+++++++++>++++++++++<<<-]\n"
        ">+.>++.>+++.\n"
        "<<<[-]\n"
        "+++++[>+++++[>+++>++++<<-]<-]\n"
        ">>+++.\n"
        ">----.\n"
        "[-]<[-]<[-]\n"
        "+++++,++++++[>++++++++++[>+<-]<-]\n"
        ">>+++++++++++++++++++++,,,++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.\n"
        "\n\n\n\ndadadad\n\n\ndad";
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

#ifdef LOG_KEY_PRESSES
            fprintf(
                key_log_file,
                "char %c  |  ascii %3d\n",
                (char)state.last_event,
                (int)state.last_event
            );
#endif

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
                    move_to_line_start(&state);
                    break;
                case KEY_END:
                    move_to_line_end(&state);
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

                case KEY_BACKSPACE:
                    if (state.cursor_pos == 0)
                        break;

                    memmove(
                        state.editor_buffer + state.cursor_pos - 1,
                        state.editor_buffer + state.cursor_pos,
                        state.editor_buffer_len - state.cursor_pos
                    );

                    --state.editor_buffer_len;
                    --state.cursor_pos;
                    state.dirty.editor = true;
                    break;

                case KEY_DELETE:
                    if (state.cursor_pos == state.editor_buffer_len)
                        break;

                    memmove(
                        state.editor_buffer + state.cursor_pos,
                        state.editor_buffer + state.cursor_pos + 1,
                        state.editor_buffer_len - state.cursor_pos - 1
                    );

                    --state.editor_buffer_len;
                    state.dirty.editor = true;
                    break;

                default:
                    if (is_typable_char(&state)
                        && state.editor_buffer_len < EDITOR_BUFFER_SIZE
                        && state.cursor_pos <= state.editor_buffer_len)
                    {
                        memmove(
                            state.editor_buffer + state.cursor_pos + 1,
                            state.editor_buffer + state.cursor_pos,
                            state.editor_buffer_len - state.cursor_pos
                        );

                        state.editor_buffer[state.cursor_pos] = (char)state.last_event;

                        ++state.editor_buffer_len;
                        ++state.cursor_pos;
                        state.dirty.editor = true;
                    } else {
                        valid = false;
                    }
                    break;
            }
        }
    }

exit_success:
    exitcode = 0;
    goto exit;

exit_failure:
    exitcode = 1;
    goto exit;

exit:
    shutdown_ui();
#ifdef LOG_KEY_PRESSES
    fclose(key_log_file);
#endif
    return exitcode;
}
