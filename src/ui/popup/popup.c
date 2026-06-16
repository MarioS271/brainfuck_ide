/**
 * @file popup.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "popup.h"
#include "handlers.h"

void create_popup_base() {
    werase(popup);

    cchar_t c;

    // Upper Border
    setcchar(&c, POPUP_BORDER_CORNER_UL, A_NORMAL, 0, nullptr);
    mvwadd_wch(popup, 0, 0, &c);

    setcchar(&c, POPUP_HORIZONTAL_BORDER, A_NORMAL, 0, nullptr);
    for (int x = 1; x < POPUP_WIDTH - 1; ++x)
        mvwadd_wch(popup, 0, x, &c);

    setcchar(&c, POPUP_BORDER_CORNER_UR, A_NORMAL, 0, nullptr);
    mvwadd_wch(popup, 0, POPUP_WIDTH - 1, &c);


    // Left & Right Borders
    setcchar(&c, POPUP_VERTICAL_BORDER, A_NORMAL, 0, nullptr);

    for (int y = 1; y < POPUP_HEIGHT - 1; ++y)
        mvwadd_wch(popup, y, 0, &c);

    for (int y = 1; y < POPUP_HEIGHT - 1; ++y)
        mvwadd_wch(popup, y, POPUP_WIDTH - 1, &c);


    // Upper Border
    setcchar(&c, POPUP_BORDER_CORNER_LL, A_NORMAL, 0, nullptr);
    mvwadd_wch(popup, POPUP_HEIGHT - 1, 0, &c);

    setcchar(&c, POPUP_HORIZONTAL_BORDER, A_NORMAL, 0, nullptr);
    for (int x = 1; x < POPUP_WIDTH - 1; ++x)
        mvwadd_wch(popup, POPUP_HEIGHT - 1, x, &c);

    setcchar(&c, POPUP_BORDER_CORNER_LR, A_NORMAL, 0, nullptr);
    mvwadd_wch(popup, POPUP_HEIGHT - 1, POPUP_WIDTH - 1, &c);


    wnoutrefresh(popup);
}

void open_save_popup(UIState* state) {
    state->popup_active = true;
    state->popup_has_textbox = true;
    state->popup_refresh_handler = save_popup_refresh_handler;
    state->popup_confirm_handler = save_popup_confirm_handler;

    create_popup_base();
}

void open_load_popup(UIState* state) {
    state->popup_active = true;
    state->popup_has_textbox = true;
    state->popup_refresh_handler = load_popup_refresh_handler;
    state->popup_confirm_handler = load_popup_confirm_handler;

    create_popup_base();
}

void open_exit_popup(UIState* state) {
    state->popup_active = true;
    state->popup_has_textbox = false;
    state->popup_refresh_handler = exit_popup_refresh_handler;
    state->popup_confirm_handler = exit_popup_confirm_handler;

    create_popup_base();
}

void close_popup(UIState* state) {
    state->popup_active = false;
    clear();
    refresh();
    state->dirty.panel_borders = true;
    state->dirty.menubar = true;
    state->dirty.editor = true;
    state->dirty.output = true;
    state->dirty.tape = true;
}
