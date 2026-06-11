/**
 * @file ui.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui.h"

#include "wincon/pdcwin.h"

void init_ui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void shutdown_ui() {
    endwin();
}

void resize_and_clear_ui() {
    resize_term(0, 0);
    clear();
}

void draw_menubar() {
    cchar_t fill;
    wchar_t symbol = L'═';

    setcchar(&fill, &symbol, A_NORMAL, 0, nullptr);

    for (int x = 0; x < COLS; ++x) {
        mvadd_wch(0, x, &fill);
    }

    mvprintw(0, MENUBAR_RUN_POS, MENUBAR_RUN_TEXT);
    mvprintw(0, MENUBAR_SAVE_POS, MENUBAR_SAVE_TEXT);
    mvprintw(0, MENUBAR_LOAD_POS, MENUBAR_LOAD_TEXT);
    mvprintw(0, MENUBAR_EXIT_POS, MENUBAR_EXIT_TEXT);
}

void draw_panels() {
    
}