/**
 * @file handlers.h
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include "../ui.h"

void ask_for_keypress_popup_refresh_handler(UIState* state);
void ask_for_debug_start_pos_popup_refresh_handler(UIState* state);
void exit_popup_refresh_handler(UIState* state);
void save_popup_refresh_handler(UIState* state);
void load_popup_refresh_handler(UIState* state);

void ask_for_debug_start_pos_popup_confirm_handler(UIState* state);
void exit_popup_confirm_handler(UIState* state);
void save_popup_confirm_handler(UIState* state);
void load_popup_confirm_handler(UIState* state);
