/**
 * @file popup.h
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once

#include "../ui.h"

// Chars
#define POPUP_HORIZONTAL_BORDER L"─"
#define POPUP_VERTICAL_BORDER L"│"
#define POPUP_BORDER_CORNER_UL L"╭"
#define POPUP_BORDER_CORNER_UR L"╮"
#define POPUP_BORDER_CORNER_LL L"╰"
#define POPUP_BORDER_CORNER_LR L"╯"


// Functions
void open_save_popup(UIState* state);
void open_load_popup(UIState* state);
void open_exit_popup(UIState* state);
void close_popup(UIState* state);
