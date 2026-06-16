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
    const int confirm_button_x = center - POPUP_BUTTON_DISTANCE_FROM_CENTER - strlen(confirm_text);
    const int cancel_button_x = center + POPUP_BUTTON_DISTANCE_FROM_CENTER;

    if (state->popup.selected_button == Confirm)
        wattron(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
    mvwprintw(popup, POPUP_HEIGHT - 3, confirm_button_x, "%s", confirm_text);
    if (state->popup.selected_button == Confirm)
        wattroff(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));

    if (state->popup.selected_button == Cancel)
        wattron(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
    mvwprintw(popup, POPUP_HEIGHT - 3, cancel_button_x, "%s", cancel_text);
    if (state->popup.selected_button == Cancel)
        wattroff(popup, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
}

void draw_text_input_field(UIState* state) {

}

void exit_popup_refresh_handler(UIState* state) {
    print_centered(2, "Do you want to exit?");
    print_centered(3, "All unsaved changes will be lost!");

    draw_confirm_cancel_buttons(state, "CONFIRM");

    wnoutrefresh(popup);
}

void save_popup_refresh_handler(UIState* state) {
    print_centered(2, "Please enter a name for the file to save");

    draw_text_input_field(state);
    draw_confirm_cancel_buttons(state, "SAVE");

    wnoutrefresh(popup);
}

void load_popup_refresh_handler(UIState* state) {
    print_centered(2, "Please enter a name for the file to save");
    print_centered(3, "All unsaved changes will be lost!");

    draw_text_input_field(state);
    draw_confirm_cancel_buttons(state, "LOAD");

    wnoutrefresh(popup);
}
