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
#include <time.h>
#include <PDCurses/curses.h>

#include "helpers.h"
#include "ui/ui.h"
#include "ui/popup/popup.h"
#include "interpreter/interpreter.h"

#include "macros.h"

int ask_for_keypress_popup(UIState* state) {
    open_ask_for_input_popup(state);
    doupdate();

    bool valid = false;
    while (!valid) {
        valid = true;

        int input = getch();
        if (is_typable_char((char)input) && input != KEY_TAB) {
            state->popup.keypress = (unsigned char)input;
            break;
        }

        valid = false;
    }

    close_popup(state);

    if (state->dirty.panel_borders) draw_panel_borders(state);
    if (state->dirty.menubar) draw_menubar(state);
    if (state->dirty.editor) draw_editor_panel(state);
    if (state->dirty.tape) draw_tape_panel(state);
    if (state->dirty.output) draw_output_panel(state);

    doupdate();

    return state->popup.keypress;
}

int ask_for_debug_start_pos_popup(UIState* state) {
    open_ask_for_debug_start_pos_popup(state);
    doupdate();

    bool end = false;
    while (!end) {
        end = true;

        const int input = getch();
        if (input >= '0' && input <= '9') {
            const size_t len = strlen(state->popup.textbox_contents);
            if (len < sizeof(state->popup.textbox_contents) - 1) {
                state->popup.textbox_contents[len] = (char)input;
                state->popup.textbox_contents[len + 1] = '\0';

                state->popup.refresh_handler(state);
                doupdate();
            }
        }
        else if (input == KEY_BACKSPACE) {
            size_t len = strlen(state->popup.textbox_contents);
            if (len > 0)
                state->popup.textbox_contents[len - 1] = '\0';

            state->popup.refresh_handler(state);
            doupdate();
        }
        else if (input == KEY_ENTER) {
            break;
        }

        end = false;
    }

    close_popup(state);

    if (state->dirty.panel_borders) draw_panel_borders(state);
    if (state->dirty.menubar) draw_menubar(state);
    if (state->dirty.editor) draw_editor_panel(state);
    if (state->dirty.tape) draw_tape_panel(state);
    if (state->dirty.output) draw_output_panel(state);

    doupdate();

    return atoi(state->popup.textbox_contents);
}

int main(void) {
#ifdef LOG_KEY_PRESSES
    FILE* key_log_file = fopen("input.log", "w");
#endif

    srand(time(nullptr));
    init_ui();

    int exitcode;
    UIState state;

    state.last_event = 0;

    state.mode = Normal;
    state.in_menubar = false;

    state.popup.active = false;
    state.popup.selected_button = Confirm;
    state.popup.has_textbox = false;
    memset(state.popup.textbox_contents, 0, sizeof(state.popup.textbox_contents));
    state.popup.refresh_handler = nullptr;
    state.popup.confirm_handler = nullptr;

    state.editor_buffer = malloc(EDITOR_BUFFER_SIZE);
    state.editor_buffer_len = 0;

    state.output_buffer = malloc(OUTPUT_BUFFER_SIZE);
    state.output_buffer_len = 0;

    state.cursor_pos = 0;
    state.current_menubar_option = 0;

    memset(state.debug.tape, 0, TAPE_LEN);
    state.debug.data_ptr = 0;

    state.dirty.panel_borders = true;
    state.dirty.menubar = true;
    state.dirty.editor = true;
    state.dirty.output = true;
    state.dirty.tape = true;

    if (state.editor_buffer == nullptr) {
        printf("Error: malloc returned nullptr for allocating editor buffer, aborting");
        goto exit_failure;
    }
    if (state.output_buffer == nullptr) {
        printf("Error: malloc returned nullptr for allocating output buffer, aborting");
        goto exit_failure;
    }

    FILE* fp = fopen("demo.bf", "r");
    if (fp != nullptr) {
        const size_t bytes_read = fread(state.editor_buffer, 1, EDITOR_BUFFER_SIZE - 1, fp);
        state.editor_buffer[bytes_read] = '\0';
        state.editor_buffer_len = bytes_read;
        fclose(fp);
    }

    while (true) {
        const bool any_dirty = state.dirty.panel_borders || state.dirty.menubar
            || state.dirty.editor || state.dirty.output || state.dirty.tape;

        if (COLS > 40 && COLS < 200
            && LINES > 10)
        {
            if (any_dirty) curs_set(0);

            if (state.dirty.panel_borders || state.mode == Debug) draw_panel_borders(&state);
            if (state.dirty.menubar) draw_menubar(&state);
            if (state.dirty.editor) draw_editor_panel(&state);
            if (state.dirty.tape) draw_tape_panel(&state);
            if (state.dirty.output) draw_output_panel(&state);

            ui_set_cursor_pos(&state);

            if (state.popup.active) state.popup.refresh_handler(&state);

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

#ifdef LOG_KEY_PRESSES
            fprintf(
                key_log_file,
                "char %c  |  ascii %3d\n",
                (char)state.last_event,
                (int)state.last_event
            );
#endif

            if (state.popup.active) {
                switch (state.last_event) {
                    case KEY_ESC:
                        close_popup(&state);
                        break;

                    case KEY_ENTER:
                        if (state.popup.has_buttons) {
                            if (state.popup.selected_button == Confirm)
                                state.popup.confirm_handler(&state);
                            else
                                close_popup(&state);
                        }
                        break;

                    case KEY_LEFT:
                    case KEY_RIGHT:
                        if (!state.popup.has_buttons) break;
                        if (state.popup.selected_button == Confirm) state.popup.selected_button = Cancel;
                        else state.popup.selected_button = Confirm;
                        break;

                    default:
                        if (state.last_event == KEY_BACKSPACE && state.popup.has_textbox) {
                            size_t len = strlen(state.popup.textbox_contents);
                            if (len > 0)
                                state.popup.textbox_contents[len - 1] = '\0';
                            break;
                        }
                        char c = (char)state.last_event;
                        if (state.popup.has_textbox
                            && strlen(state.popup.textbox_contents) < MAX_FILENAME_LEN
                            && ((c >= '0' && c <= '9')
                                || (c >= 'A' && c <= 'Z')
                                || (c >= 'a' && c <= 'z')
                                || c == '.' || c == '-' || c == '_'))
                        {
                            size_t len = strlen(state.popup.textbox_contents);
                            if (len < sizeof(state.popup.textbox_contents) - 1) {
                                state.popup.textbox_contents[len] = (char)state.last_event;
                                state.popup.textbox_contents[len + 1] = '\0';
                            }
                            break;
                        }

                        valid = false;
                        break;
                }
                continue;
            }

            switch (state.last_event) {
                case KEY_ESC:
                    state.in_menubar = !state.in_menubar;
                    state.dirty.menubar = true;
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
                    if (state.mode == Debug)
                        break;
                    if (state.in_menubar) {
                        valid = false;
                        break;
                    }
                    move_to_line_start(&state);
                    break;
                case KEY_END:
                    if (state.mode == Debug)
                        break;
                    if (state.in_menubar) {
                        valid = false;
                        break;
                    }
                    move_to_line_end(&state);
                    break;

                case KEY_LEFT:
                    if (state.in_menubar) {
                        --state.current_menubar_option;
                        state.dirty.menubar = true;

                        if (state.current_menubar_option < 0)
                            state.current_menubar_option = MENUBAR_NUM_ITEMS - 1;

                        break;
                    }

                    if (state.mode == Debug) {
                        if (state.debug.step_count > 0) {
                            --state.debug.step_count;
                            regenerate_debug_tape(&state);
                            state.cursor_pos = state.debug.pc;
                            state.dirty.editor = true;
                            state.dirty.tape   = true;
                        } else {
                            valid = false;
                        }
                        break;
                    }

                    if (state.cursor_pos < state.editor_buffer_len) {
                        --state.cursor_pos;
                        state.dirty.editor = true;
                        state.dirty.tape = true;
                        break;
                    }

                    valid = false;
                    break;

                case KEY_RIGHT:
                    if (state.in_menubar) {
                        ++state.current_menubar_option;
                        state.dirty.menubar = true;

                        if (state.current_menubar_option >= MENUBAR_NUM_ITEMS)
                            state.current_menubar_option = 0;

                        break;
                    }

                    if (state.mode == Debug) {
                        if (!state.debug.halted) {
                            ++state.debug.step_count;
                            regenerate_debug_tape(&state);
                            state.cursor_pos = state.debug.pc;
                            state.dirty.editor = true;
                            state.dirty.tape   = true;
                        } else {
                            valid = false;
                        }
                        break;
                    }

                    if (state.cursor_pos < state.editor_buffer_len) {
                        ++state.cursor_pos;
                        state.dirty.editor = true;
                        state.dirty.tape   = true;
                        break;
                    }

                    valid = false;
                    break;

                case KEY_UP:
                    if (state.mode == Debug)
                        break;
                    if (state.in_menubar) {
                        valid = false;
                        break;
                    }
                    move_cursor_up(&state);
                    break;
                case KEY_DOWN:
                    if (state.mode == Debug)
                        break;
                    if (state.in_menubar) {
                        valid = false;
                        break;
                    }
                    move_cursor_down(&state);
                    break;

                case KEY_BACKSPACE:
                    if (state.cursor_pos == 0 || state.in_menubar || state.mode == Debug) {
                        valid = false;
                        break;
                    }

                    memmove(
                        state.editor_buffer + state.cursor_pos - 1,
                        state.editor_buffer + state.cursor_pos,
                        state.editor_buffer_len - state.cursor_pos
                    );

                    --state.editor_buffer_len;
                    --state.cursor_pos;
                    state.dirty.editor = true;
                    state.dirty.tape = true;
                    break;

                case KEY_DELETE:
                    if (state.cursor_pos == state.editor_buffer_len || state.in_menubar || state.mode == Debug) {
                        valid = false;
                        break;
                    }

                    memmove(
                        state.editor_buffer + state.cursor_pos,
                        state.editor_buffer + state.cursor_pos + 1,
                        state.editor_buffer_len - state.cursor_pos - 1
                    );

                    --state.editor_buffer_len;
                    state.dirty.editor = true;
                    state.dirty.tape = true;
                    break;

                default:
                    if ((state.last_event == KEY_ENTER || state.last_event == KEY_SPACE)
                        && state.in_menubar)
                    {
                        switch (state.current_menubar_option) {
                            case 0:
                                run_brainfuck(
                                    &state,
                                    state.editor_buffer,
                                    state.editor_buffer_len,
                                    ask_for_keypress_popup
                                );
                                state.mode = Normal;
                                state.dirty.output = true;
                                state.dirty.tape = true;
                                break;

                            case 1:
                                if (state.mode == Normal) {
                                    state.mode = Debug;

                                    int row = ask_for_debug_start_pos_popup(&state);
                                    int target_pos = 0;
                                    int steps = find_step_count_for_row(&state, row, &target_pos);
                                    state.debug.step_count = steps < 0 ? 0 : steps;

                                    memset(state.debug.tape, 0, sizeof(state.debug.tape));
                                    state.debug.data_ptr = 0;

                                    regenerate_debug_tape(&state);

                                    state.cursor_pos = target_pos;
                                    state.debug.pc   = target_pos;
                                }
                                else {
                                    state.mode = Normal;
                                    state.cursor_pos = 0;

                                    memset(state.output_buffer, 0, OUTPUT_BUFFER_SIZE);
                                    state.output_buffer_len = 0;
                                }

                                state.dirty.panel_borders = true;
                                state.dirty.editor = true;
                                state.dirty.output = true;
                                state.dirty.tape = true;
                                break;

                            case 2:
                                open_save_popup(&state);
                                break;

                            case 3:
                                open_load_popup(&state);
                                break;

                            case 4:
                                open_exit_popup(&state);

                            default:
                                break;
                        }

                        state.in_menubar = false;
                        state.dirty.menubar = true;
                    }
                    else if (is_typable_char((char)state.last_event)
                        && state.editor_buffer_len < EDITOR_BUFFER_SIZE
                        && state.cursor_pos <= state.editor_buffer_len
                        && state.mode == Normal
                        && !state.in_menubar)
                    {
                        memmove(
                            state.editor_buffer + state.cursor_pos + (state.last_event == KEY_TAB ? TAB_SIZE : 1),
                            state.editor_buffer + state.cursor_pos,
                            state.editor_buffer_len - state.cursor_pos
                        );

                        if (state.last_event == KEY_TAB) {
                            for (int i = 0; i < TAB_SIZE; ++i)
                                state.editor_buffer[state.cursor_pos + i] = ' ';
                            state.editor_buffer_len += TAB_SIZE;
                            state.cursor_pos += TAB_SIZE;
                        } else {
                            state.editor_buffer[state.cursor_pos] = (char)state.last_event;
                            ++state.editor_buffer_len;
                            ++state.cursor_pos;
                        }

                        state.dirty.editor = true;
                        state.dirty.tape = true;
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
    free(state.editor_buffer);
    free(state.output_buffer);
    return exitcode;
}
