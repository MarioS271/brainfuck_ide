/**
 * @file confirm_handlers.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "popup.h"
#include <stdlib.h>
#include <string.h>

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
    remove(state->popup.textbox_contents);
    FILE* fp = fopen(state->popup.textbox_contents, "wb");
    if (fp != nullptr) {
        fwrite(state->editor_buffer, 1, state->editor_buffer_len, fp);
        fclose(fp);
    }

    close_popup(state);
}

void load_popup_confirm_handler(UIState* state) {
    FILE* fp = fopen(state->popup.textbox_contents, "r");
    if (fp == nullptr) {
        fp = fopen(state->popup.textbox_contents, "w");
        fclose(fp);
        fp = fopen(state->popup.textbox_contents, "r");
    }
    const size_t bytes_read = fread(state->editor_buffer, 1, EDITOR_BUFFER_SIZE - 1, fp);
    state->editor_buffer[bytes_read] = '\0';
    state->editor_buffer_len = bytes_read;
    fclose(fp);

    close_popup(state);
}
