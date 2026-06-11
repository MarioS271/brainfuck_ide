/**
 * @file ui.h
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include <PDCurses/curses.h>

// Panel Bounds and Seperator Coords
#define LOWER_PANEL_HEIGHT 3
#define LOWER_PANEL_SEPERATOR (LINES - LOWER_PANEL_HEIGHT)
#define UPPER_PANELS_HEIGHT (LOWER_PANEL_SEPERATOR - 1)
#define UPPER_PANELS_SEPERATOR_X (EDITOR_PANEL_WIDTH + 1)
#define EDITOR_PANEL_WIDTH ((COLS / 3) * 2 - 1)
#define OUTPUT_PANEL_WIDTH (COLS - UPPER_PANELS_SEPERATOR_X - 1)

// Menu Bar Positions
#define MENUBAR_RUN_TEXT " RUN "
#define MENUBAR_SAVE_TEXT " SAVE "
#define MENUBAR_LOAD_TEXT " LOAD "
#define MENUBAR_EXIT_TEXT " EXIT "
#define MENUBAR_BASE_POS 4
#define MENUBAR_OPTION_SPACING 2
#define MENUBAR_RUN_POS MENUBAR_BASE_POS
#define MENUBAR_SAVE_POS (MENUBAR_RUN_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_RUN_TEXT))
#define MENUBAR_LOAD_POS (MENUBAR_SAVE_POS + MENUBAR_OPTION_SPACING + strlen(MENUBAR_LOAD_TEXT))
#define MENUBAR_EXIT_POS (COLS - MENUBAR_BASE_POS - strlen(MENUBAR_EXIT_TEXT))


// Function Headers
void init_ui();
void shutdown_ui();

void resize_and_clear_ui();

void draw_menubar();
void draw_panel_borders();
void draw_editor();
void draw_output();
void draw_tape();
