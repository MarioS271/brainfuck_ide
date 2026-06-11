/**
 * @file ui.h
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include <stddef.h>

// Color Pairs
#define HIGHLIGHT_COLOR_PAIR 1
#define LINE_NUMBER_COLOR_PAIR 2


// Panel Bounds and Seperator Coords
// Positions
#define EDITOR_PANEL_X 0
#define EDITOR_PANEL_Y 1
#define OUTPUT_PANEL_X (UPPER_PANELS_SEPERATOR_X + 1)
#define OUTPUT_PANEL_Y EDITOR_PANEL_Y
#define TAPE_PANEL_X 0
#define TAPE_PANEL_Y (LINES - TAPE_PANEL_HEIGHT)
// Sizes
#define EDITOR_PANEL_WIDTH ((COLS / 3) * 2 - 1)
#define EDITOR_PANEL_HEIGHT (LINES - EDITOR_PANEL_Y - TAPE_PANEL_HEIGHT - 1)
#define OUTPUT_PANEL_WIDTH (COLS - OUTPUT_PANEL_X)
#define OUTPUT_PANEL_HEIGHT (LINES - OUTPUT_PANEL_Y - TAPE_PANEL_HEIGHT - 2)
#define TAPE_PANEL_WIDTH COLS
#define TAPE_PANEL_HEIGHT 4
// Seperator Positions
#define UPPER_PANELS_SEPERATOR_X (EDITOR_PANEL_WIDTH + 1)
#define UPPER_PANELS_SEPERATOR_Y EDITOR_PANEL_Y
#define LOWER_PANEL_SEPERATOR_X TAPE_PANEL_X
#define LOWER_PANEL_SEPERATOR_Y (LINES - TAPE_PANEL_HEIGHT - 1)
// Seperator Heights
#define UPPER_PANELS_SEPERATOR_HEIGHT EDITOR_PANEL_HEIGHT
#define LOWER_PANEL_SEPERATOR_WIDTH COLS


// Menu Bar Positions
// Text
#define MENUBAR_RUN_TEXT " RUN "
#define MENUBAR_SAVE_TEXT " SAVE "
#define MENUBAR_LOAD_TEXT " LOAD "
#define MENUBAR_EXIT_TEXT " EXIT "
// Position Presets
#define MENUBAR_Y 0
#define MENUBAR_BASE_POS 4
#define MENUBAR_OPTION_SPACING 2
// Final Positions
#define MENUBAR_RUN_POS MENUBAR_BASE_POS
#define MENUBAR_SAVE_POS (MENUBAR_RUN_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_RUN_TEXT))
#define MENUBAR_LOAD_POS (MENUBAR_SAVE_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_LOAD_TEXT))
#define MENUBAR_EXIT_POS (COLS - MENUBAR_BASE_POS - strlen(MENUBAR_EXIT_TEXT))


// Editor Panel
#define EDITOR_BUFFER_SIZE 131'072   // 128 KiB
#define EDITOR_PADDING_X 2
#define EDITOR_PADDING_Y 1
#define EDITOR_DRAWABLE_WIDTH (EDITOR_PANEL_WIDTH - (EDITOR_PADDING_X * 2))
#define EDITOR_DRAWABLE_HEIGHT (EDITOR_PANEL_HEIGHT - (EDITOR_PADDING_Y * 2))


// Data Structures
// Structs
typedef struct {
    int last_event;

    char* editor_buffer;
    size_t editor_buffer_len;

    int cursor_pos;

    bool in_menubar;
    int current_menubar_option;

    struct _Dirty {
        bool panel_borders;
        bool menubar;
        bool editor;
        bool output;
        bool tape;
    };
    struct _Dirty dirty;
} UIState;


// Function Headers
void init_ui();
void shutdown_ui();

void resize_ui();
void ui_set_cursor_pos();

void draw_menubar();
void draw_panel_borders();
void draw_editor_panel(UIState* state);
void draw_output_panel(UIState* state);
void draw_tape_panel(UIState* state);
