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

int editor_cursor_x = EDITOR_PADDING_X;
int editor_cursor_y = EDITOR_PADDING_Y;
int editor_scroll = 0;

void init_ui() {
    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(2);

    init_pair(HIGHLIGHT_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(LINE_NUMBER_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);

    editor_panel = newwin(EDITOR_PANEL_HEIGHT, EDITOR_PANEL_WIDTH, EDITOR_PANEL_Y, EDITOR_PANEL_X);
    output_panel = newwin(OUTPUT_PANEL_HEIGHT, OUTPUT_PANEL_WIDTH, OUTPUT_PANEL_Y, OUTPUT_PANEL_X);
    tape_panel = newwin(TAPE_PANEL_HEIGHT, TAPE_PANEL_WIDTH, TAPE_PANEL_Y, TAPE_PANEL_X);
}

void shutdown_ui() {
    endwin();
}

void ui_set_cursor_pos() {
    move(EDITOR_PANEL_Y + editor_cursor_y, EDITOR_PANEL_X + editor_cursor_x);
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

void draw_menubar() {
    constexpr wchar_t symbol = L'═';
    cchar_t fill;

    setcchar(&fill, &symbol, A_NORMAL, 0, nullptr);

    for (int x = 0; x < COLS; ++x) {
        mvadd_wch(0, x, &fill);
    }

    mvprintw(MENUBAR_Y, MENUBAR_RUN_POS, MENUBAR_RUN_TEXT);
    mvprintw(MENUBAR_Y, MENUBAR_SAVE_POS, MENUBAR_SAVE_TEXT);
    mvprintw(MENUBAR_Y, MENUBAR_LOAD_POS, MENUBAR_LOAD_TEXT);
    mvprintw(MENUBAR_Y, MENUBAR_EXIT_POS, MENUBAR_EXIT_TEXT);

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

static void draw_line_number(const int vrow, const int scroll, const int digit_count,
                             const int line_number) {
    if (vrow < scroll || vrow >= scroll + EDITOR_DRAWABLE_HEIGHT)
        return;

    wattron(editor_panel, COLOR_PAIR(LINE_NUMBER_COLOR_PAIR));
    mvwprintw(editor_panel, vrow - scroll + EDITOR_PADDING_Y, EDITOR_PADDING_X,
              "%*d", digit_count, line_number);
    wattroff(editor_panel, COLOR_PAIR(LINE_NUMBER_COLOR_PAIR));
}

static void walk_editor_buffer(UIState* state, const int digit_count, const int scroll,
                               const bool draw, int* cursor_vrow, int* cursor_col) {
    int vrow = 0;
    int col = EDITOR_PADDING_X + digit_count + 1;
    int line_number = 1;

    if (draw)
        draw_line_number(vrow, scroll, digit_count, line_number);

    for (int i = 0; i < state->editor_buffer_len; ++i) {
        const char current_char = state->editor_buffer[i];

        if (current_char == '\n') {
            if (!draw && state->cursor_pos == i) {
                *cursor_vrow = vrow;
                *cursor_col = col;
            }

            col = EDITOR_PADDING_X + digit_count + 1;
            ++vrow;
            ++line_number;

            if (draw)
                draw_line_number(vrow, scroll, digit_count, line_number);

            continue;
        }

        if (col > EDITOR_DRAWABLE_WIDTH) {
            col = EDITOR_PADDING_X + digit_count + 1;
            ++vrow;
        }

        if (draw && vrow >= scroll + EDITOR_DRAWABLE_HEIGHT)
            break;

        if (!draw && state->cursor_pos == i) {
            *cursor_vrow = vrow;
            *cursor_col = col;
        }

        if (draw && vrow >= scroll)
            mvwaddch(editor_panel, vrow - scroll + EDITOR_PADDING_Y, col, current_char);

        ++col;
    }

    if (!draw && state->cursor_pos == state->editor_buffer_len) {
        *cursor_vrow = vrow;
        *cursor_col = col;
    }
}

void draw_editor_panel(UIState* state) {
    werase(editor_panel);

    int total_lines = 1;
    for (int i = 0; i < state->editor_buffer_len; ++i) {
        if (state->editor_buffer[i] == '\n')
            ++total_lines;
    }

    const int digit_count = snprintf(nullptr, 0, "%d", total_lines);

    int cursor_vrow = 0;
    int cursor_col = EDITOR_PADDING_X;
    walk_editor_buffer(state, digit_count, 0, false, &cursor_vrow, &cursor_col);

    if (cursor_vrow < editor_scroll)
        editor_scroll = cursor_vrow;
    if (cursor_vrow >= editor_scroll + EDITOR_DRAWABLE_HEIGHT)
        editor_scroll = cursor_vrow - EDITOR_DRAWABLE_HEIGHT + 1;

    walk_editor_buffer(state, digit_count, editor_scroll, true, nullptr, nullptr);

    editor_cursor_x = cursor_col;
    editor_cursor_y = cursor_vrow - editor_scroll + EDITOR_PADDING_Y;

    wnoutrefresh(editor_panel);
}

void draw_output_panel(UIState* state) {

}

void draw_tape_panel(UIState* state) {

}
