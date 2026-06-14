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
int editor_cursor_x;
int editor_cursor_y;
int editor_scroll;

void init_ui() {
    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(2);

    init_pair(HIGHLIGHT_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(PTR_OP_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(VAL_OP_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(OUT_OP_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(IN_OP_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(LOOP_OP_COLOR_PAIR, COLOR_CYAN, COLOR_BLACK);

    editor_panel = newwin(EDITOR_PANEL_HEIGHT, EDITOR_PANEL_WIDTH, EDITOR_PANEL_Y, EDITOR_PANEL_X);
    output_panel = newwin(OUTPUT_PANEL_HEIGHT, OUTPUT_PANEL_WIDTH, OUTPUT_PANEL_Y, OUTPUT_PANEL_X);
    tape_panel = newwin(TAPE_PANEL_HEIGHT, TAPE_PANEL_WIDTH, TAPE_PANEL_Y, TAPE_PANEL_X);

    editor_cursor_x = EDITOR_PADDING_X;
    editor_cursor_y = EDITOR_PADDING_Y;
    editor_scroll = 0;
}

void shutdown_ui() {
    endwin();
}

void ui_set_cursor_pos(UIState* state) {
    move(EDITOR_PANEL_Y + editor_cursor_y, EDITOR_PANEL_X + editor_cursor_x);

    if (!state->in_menubar)
        curs_set(2);
    else
        curs_set(0);

    wnoutrefresh(stdscr);
}

void resize_ui() {
    resize_term(0, 0);

    wresize(editor_panel, EDITOR_PANEL_HEIGHT, EDITOR_PANEL_WIDTH);
    mvwin(editor_panel, EDITOR_PANEL_Y, EDITOR_PANEL_X);

    wresize(output_panel, OUTPUT_PANEL_HEIGHT, OUTPUT_PANEL_WIDTH);
    mvwin(output_panel, OUTPUT_PANEL_Y, OUTPUT_PANEL_X);

    wresize(tape_panel, TAPE_PANEL_HEIGHT, TAPE_PANEL_WIDTH);
    mvwin(tape_panel, TAPE_PANEL_Y, TAPE_PANEL_X);
}

void draw_menubar(UIState* state) {
    constexpr wchar_t symbol = L'═';
    cchar_t fill;

    setcchar(&fill, &symbol, A_NORMAL, 0, nullptr);

    for (int x = 0; x < COLS; ++x) {
        mvadd_wch(0, x, &fill);
    }

    int pos = 0;
    char text[10] = {};

    for (int i = 0; i < 4; ++i) {
        switch (i) {
            case 0:
                pos = MENUBAR_RUN_POS;
                memcpy(text, MENUBAR_RUN_TEXT, strlen(MENUBAR_RUN_TEXT));
                break;

            case 1:
                pos = MENUBAR_SAVE_POS;
                memcpy(text, MENUBAR_SAVE_TEXT, strlen(MENUBAR_SAVE_TEXT));
                break;

            case 2:
                pos = MENUBAR_LOAD_POS;
                memcpy(text, MENUBAR_LOAD_TEXT, strlen(MENUBAR_LOAD_TEXT));
                break;

            case 3:
                pos = MENUBAR_EXIT_POS;
                memcpy(text, MENUBAR_EXIT_TEXT, strlen(MENUBAR_EXIT_TEXT));
                break;

            default:
                break;
        }

        bool selected = state->current_menubar_option == i && state->in_menubar;

        mvprintw(MENUBAR_Y, pos, " ");

        if (selected)
            wattron(stdscr, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));
        mvprintw(MENUBAR_Y, pos + 1, text);
        if (selected)
            wattroff(stdscr, COLOR_PAIR(HIGHLIGHT_COLOR_PAIR));

        mvprintw(MENUBAR_Y, pos + 1 + strlen(text), " ");
    }

    wnoutrefresh(stdscr);
}

void draw_panel_borders() {
    constexpr wchar_t vertical_symbol = L'┃';
    constexpr wchar_t horizontal_symbol = L'━';
    constexpr wchar_t crossing_symbol = L'┻';

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

    wnoutrefresh(stdscr);
}
