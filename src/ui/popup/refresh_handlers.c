/**
 * @file refresh_handlers.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "popup.h"
#include <string.h>

inline void print_centered(const int y_pos, const char* str) {
    mvwprintw(popup, y_pos, (POPUP_WIDTH - (int)strlen(str)) / 2, "%s", str);
}

void draw_confirm_cancel_buttons(UIState* state, char* confirm_text) {
    const char* cancel_text = "CANCEL";

    const int center = POPUP_WIDTH / 2;
    const int height = POPUP_HEIGHT - 3;
    const int confirm_button_x = center - POPUP_BUTTON_DISTANCE_FROM_CENTER - strlen(confirm_text);
    const int cancel_button_x = center + POPUP_BUTTON_DISTANCE_FROM_CENTER;

    if (state->popup.selected_button == Confirm)
        wattron(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
    mvwprintw(popup, height, confirm_button_x, "%s", confirm_text);
    if (state->popup.selected_button == Confirm)
        wattroff(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));

    if (state->popup.selected_button == Cancel)
        wattron(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
    mvwprintw(popup, height, cancel_button_x, "%s", cancel_text);
    if (state->popup.selected_button == Cancel)
        wattroff(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
}

void draw_text_input_field(UIState* state) {
    const int center = POPUP_WIDTH / 2;
    const int height = POPUP_HEIGHT - 5;

    const int x_start = center - POPUP_TEXT_INPUT_FIELD_WIDTH / 2;

    wattron(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
    for (int x = x_start; x < (POPUP_TEXT_INPUT_FIELD_WIDTH + x_start); ++x) {
        mvwprintw(popup, height, x, " ");
    }
    mvwprintw(popup, height, x_start, "%s", state->popup.textbox_contents);
    wattroff(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
}

void ask_for_keypress_popup_refresh_handler(UIState* state) {
    create_popup_base();

    print_centered(3, "Brainfuck program has requested input");
    print_centered(4, "Please press a key");

    wnoutrefresh(popup);
}

void ask_for_debug_start_pos_popup_refresh_handler(UIState* state) {
    create_popup_base();

    print_centered(2, "Please enter row number for");
    print_centered(3, "debugger to start in");

    draw_text_input_field(state);

    print_centered(7, "Press ENTER to confirm");

    wnoutrefresh(popup);
}

void exit_popup_refresh_handler(UIState* state) {
    create_popup_base();

    print_centered(2, "Do you want to exit?");
    print_centered(3, "All unsaved changes will be lost!");

    draw_confirm_cancel_buttons(state, "CONFIRM");

    wnoutrefresh(popup);
}

void save_popup_refresh_handler(UIState* state) {
    create_popup_base();

    print_centered(2, "Please enter a name for the file to save");

    draw_text_input_field(state);
    draw_confirm_cancel_buttons(state, "SAVE");

    wnoutrefresh(popup);
}

void load_popup_refresh_handler(UIState* state) {
    create_popup_base();

    print_centered(2, "Please enter a name for the file to save");
    print_centered(3, "All unsaved changes will be lost!");

    draw_text_input_field(state);
    draw_confirm_cancel_buttons(state, "LOAD");

    wnoutrefresh(popup);
}
