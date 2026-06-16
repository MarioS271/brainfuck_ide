/**
 * @file confirm_handlers.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "popup.h"
#include <stdlib.h>

void exit_popup_confirm_handler(UIState* state) {
    shutdown_ui();
#ifdef LOG_KEY_PRESSES
    fclose(key_log_file);
#endif
    free(state->editor_buffer);
    free(state->output_buffer);
    exit(0);
}

void save_popup_confirm_handler(UIState* state) {

}

void load_popup_confirm_handler(UIState* state) {

}
