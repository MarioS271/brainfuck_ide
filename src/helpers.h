/**
 * @file helpers.h
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include "ui/ui.h"

void move_cursor_up(UIState* state);
void move_cursor_down(UIState* state);
void move_to_line_start(UIState* state);
void move_to_line_end(UIState* state);

bool is_typable_char(char c);
