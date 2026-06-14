/**
 * @file ui.h
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include <PDCurses/curses.h>

// Color Pairs
#define DEFAULT_COLOR_PAIR 0
#define HIGHLIGHT_COLOR_PAIR 1
#define PTR_OP_COLOR_PAIR 2
#define VAL_OP_COLOR_PAIR 3
#define OUT_OP_COLOR_PAIR 4
#define IN_OP_COLOR_PAIR 5
#define LOOP_OP_COLOR_PAIR 6


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


// Menu Bar
#define MENUBAR_NUM_ITEMS 4
// Text
#define MENUBAR_RUN_TEXT "RUN"
#define MENUBAR_SAVE_TEXT "SAVE"
#define MENUBAR_LOAD_TEXT "LOAD"
#define MENUBAR_EXIT_TEXT "EXIT"
// Position Presets
#define MENUBAR_Y 0
#define MENUBAR_BASE_POS 4
#define MENUBAR_OPTION_SPACING 2
#define MENUBAR_OPTION_PADDING 1
// Final Positions
#define MENUBAR_RUN_POS MENUBAR_BASE_POS
#define MENUBAR_SAVE_POS (MENUBAR_RUN_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_RUN_TEXT) + (MENUBAR_OPTION_PADDING * 2))
#define MENUBAR_LOAD_POS (MENUBAR_SAVE_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_LOAD_TEXT) + (MENUBAR_OPTION_PADDING * 2))
#define MENUBAR_EXIT_POS (COLS - MENUBAR_BASE_POS - strlen(MENUBAR_EXIT_TEXT) - (MENUBAR_OPTION_PADDING * 2))


// Editor Panel
#define EDITOR_BUFFER_SIZE 131'072   // 128 KiB
#define EDITOR_PADDING_X 2
#define EDITOR_PADDING_Y 1
#define EDITOR_DRAWABLE_WIDTH (EDITOR_PANEL_WIDTH - (EDITOR_PADDING_X * 2))
#define EDITOR_DRAWABLE_HEIGHT (EDITOR_PANEL_HEIGHT - (EDITOR_PADDING_Y * 2))


// Output Panel
#define OUTPUT_BUFFER_SIZE 32'768   // 32 KiB
#define OUTPUT_PADDING_X 2
#define OUTPUT_PADDING_Y 1
#define OUTPUT_DRAWABLE_WIDTH (OUTPUT_PANEL_WIDTH - (OUTPUT_PADDING_X * 2))
#define OUTPUT_DRAWABLE_HEIGHT (OUTPUT_PANEL_HEIGHT - (OUTPUT_PADDING_Y * 2))


// Globals
extern WINDOW* editor_panel;
extern WINDOW* output_panel;
extern WINDOW* tape_panel;
extern WINDOW* popup;
extern int editor_cursor_x;
extern int editor_cursor_y;
extern int editor_scroll;


// Data Structures
// Structs
typedef struct {
    int last_event;

    char* editor_buffer;
    size_t editor_buffer_len;

    char* output_buffer;
    size_t output_buffer_len;

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
void ui_set_cursor_pos(UIState* state);

void draw_menubar(UIState* state);
void draw_panel_borders();
void draw_editor_panel(UIState* state);
void draw_output_panel(UIState* state);
void draw_tape_panel(UIState* state);
