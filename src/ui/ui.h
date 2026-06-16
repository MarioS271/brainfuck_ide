/**
 * @file ui.h
 * @authors MarioS271
 *
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include <PDCurses/curses.h>
#include <stdint.h>
#include "../macros.h"

// Color Pairs
#define DEFAULT_COLOR_PAIR 0
#define HIGHLIGHT_COLOR_PAIR 1
#define DEBUG_BORDER_COLOR_PAIR 2
#define PTR_OP_COLOR_PAIR 3
#define VAL_OP_COLOR_PAIR 4
#define OUT_OP_COLOR_PAIR 5
#define IN_OP_COLOR_PAIR 6
#define LOOP_OP_COLOR_PAIR 7

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
#define TAPE_PANEL_HEIGHT 5
// Seperator Positions
#define UPPER_PANELS_SEPERATOR_X (EDITOR_PANEL_WIDTH + 1)
#define UPPER_PANELS_SEPERATOR_Y EDITOR_PANEL_Y
#define LOWER_PANEL_SEPERATOR_X TAPE_PANEL_X
#define LOWER_PANEL_SEPERATOR_Y (LINES - TAPE_PANEL_HEIGHT - 1)
// Seperator Heights
#define UPPER_PANELS_SEPERATOR_HEIGHT EDITOR_PANEL_HEIGHT
#define LOWER_PANEL_SEPERATOR_WIDTH COLS

// Debug Text Params
#define DEBUG_PC_COUNTER_PREFIX "PC: "
#define DEBUG_PC_COUNTER_X 4

// Menu Bar
#define MENUBAR_NUM_ITEMS 5
// Text
#define MENUBAR_RUN_TEXT "RUN"
#define MENUBAR_DEBUG_TEXT "DEBUG"
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
#define MENUBAR_DEBUG_POS (MENUBAR_RUN_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_RUN_TEXT) + (MENUBAR_OPTION_PADDING * 2))
#define MENUBAR_SAVE_POS (MENUBAR_DEBUG_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_DEBUG_TEXT) + (MENUBAR_OPTION_PADDING * 2))
#define MENUBAR_LOAD_POS (MENUBAR_SAVE_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_SAVE_TEXT) + (MENUBAR_OPTION_PADDING * 2))
#define MENUBAR_EXIT_POS (COLS - MENUBAR_BASE_POS - strlen(MENUBAR_EXIT_TEXT) - (MENUBAR_OPTION_PADDING * 2))

// Editor Panel
#define EDITOR_BUFFER_SIZE 131'072   // 128 KiB
#define EDITOR_PADDING_X 2
#define EDITOR_PADDING_Y 1
#define EDITOR_DRAWABLE_WIDTH (EDITOR_PANEL_WIDTH - (EDITOR_PADDING_X * 2))
#define EDITOR_DRAWABLE_HEIGHT (EDITOR_PANEL_HEIGHT - (EDITOR_PADDING_Y * 2))

// Output Panel
#define OUTPUT_BUFFER_SIZE 1'024   // 1 KiB
#define OUTPUT_PADDING_X 2
#define OUTPUT_PADDING_Y 1
#define OUTPUT_DRAWABLE_WIDTH (OUTPUT_PANEL_WIDTH - (OUTPUT_PADDING_X * 2))
#define OUTPUT_DRAWABLE_HEIGHT (OUTPUT_PANEL_HEIGHT - (OUTPUT_PADDING_Y * 2))

// Tape Panel
#define TAPE_CELL_WIDTH 5
#define TAPE_CELL_SPACING 1
#define TAPE_PADDING_X 2
#define TAPE_PADDING_Y 1
#define TAPE_DRAWABLE_WIDTH (TAPE_PANEL_WIDTH - (TAPE_PADDING_X * 2))
#define TAPE_MAX_VISIBLE_CELLS ((TAPE_DRAWABLE_WIDTH + TAPE_CELL_SPACING) / (TAPE_CELL_WIDTH + TAPE_CELL_SPACING))

// Popup
#define POPUP_WIDTH 60
#define POPUP_HEIGHT 15
#define POPUP_X_POS (COLS / 2 - POPUP_WIDTH / 2)
#define POPUP_Y_POS (LINES / 2 - POPUP_HEIGHT / 2)

// Other
#define DEBUG_PC_HISTORY_SIZE 1000
#define TAB_SIZE 4


// Globals
extern WINDOW* popup;
extern WINDOW* editor_panel;
extern WINDOW* output_panel;
extern WINDOW* tape_panel;
extern int editor_cursor_x;
extern int editor_cursor_y;
extern int editor_scroll;


// Enums
typedef enum {
    Normal,
    Debug,
} UIMode;


// Structs
typedef struct UIState {
    int last_event;

    UIMode mode;
    bool in_menubar;

    bool popup_active;
    bool popup_has_textbox;
    void (*popup_refresh_handler)(struct UIState* state);
    void (*popup_confirm_handler)(struct UIState* state);

    char* editor_buffer;
    size_t editor_buffer_len;

    char* output_buffer;
    size_t output_buffer_len;

    int cursor_pos;
    int current_menubar_option;

    int debug_pc_history[DEBUG_PC_HISTORY_SIZE];
    size_t debug_pc_index;

    uint8_t debug_tape[TAPE_LEN];
    unsigned int debug_data_ptr;

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
void draw_panel_borders(UIState* state);
void draw_editor_panel(UIState* state);
void draw_output_panel(UIState* state);
void draw_tape_panel(UIState* state);
