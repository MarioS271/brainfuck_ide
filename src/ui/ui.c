/**
 * @file ui.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "ui.h"

#include <string.h>
#include <PDCurses/curses.h>

WINDOW* editor_panel;
WINDOW* output_panel;
WINDOW* tape_panel;

void init_ui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(0);

    editor_panel = newwin(EDITOR_PANEL_HEIGHT, EDITOR_PANEL_WIDTH, EDITOR_PANEL_Y, EDITOR_PANEL_X);
    output_panel = newwin(OUTPUT_PANEL_HEIGHT, OUTPUT_PANEL_WIDTH, OUTPUT_PANEL_Y, OUTPUT_PANEL_X);
    tape_panel = newwin(TAPE_PANEL_HEIGHT, TAPE_PANEL_WIDTH, TAPE_PANEL_Y, TAPE_PANEL_X);
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

    mvprintw(MENUBAR_Y, MENUBAR_RUN_POS, MENUBAR_RUN_TEXT);
    mvprintw(MENUBAR_Y, MENUBAR_SAVE_POS, MENUBAR_SAVE_TEXT);
    mvprintw(MENUBAR_Y, MENUBAR_LOAD_POS, MENUBAR_LOAD_TEXT);
    mvprintw(MENUBAR_Y, MENUBAR_EXIT_POS, MENUBAR_EXIT_TEXT);
}

void draw_panel_borders() {
    wchar_t vertical_symbol = L'┃';
    wchar_t horizontal_symbol = L'━';
    wchar_t crossing_symbol = L'┻';

    cchar_t fill;

    setcchar(&fill, &vertical_symbol, A_NORMAL, 0, nullptr);
    for (int y = UPPER_PANELS_SEPERATOR_Y; y < (UPPER_PANELS_SEPERATOR_Y + UPPER_PANELS_SEPERATOR_HEIGHT); ++y) {
        mvadd_wch(y, UPPER_PANELS_SEPERATOR_X, &fill);
    }

    setcchar(&fill, &horizontal_symbol, A_NORMAL, 0, nullptr);
    for (int x = LOWER_PANEL_SEPERATOR_X; x < LOWER_PANEL_SEPERATOR_WIDTH; ++x) {
        mvadd_wch(LOWER_PANEL_SEPERATOR_Y, x, &fill);
    }

    setcchar(&fill, &crossing_symbol, A_NORMAL, 0, nullptr);
    mvadd_wch(LOWER_PANEL_SEPERATOR_Y, UPPER_PANELS_SEPERATOR_X, &fill);
}

void draw_editor_panel(UIState* state) {
    
}

void draw_output_panel(UIState* state) {

}

void draw_tape_panel(UIState* state) {

}