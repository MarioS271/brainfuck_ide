/**
 * @file main.c
 * @authors MarioS271
 * 
 * SPDX-FileCopyrightText: (C) MarioS271 2026
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <demos/tui.h>
#include "ui/ui.h"
#include "interpreter/interpreter.h"

int main(void) {
    init_ui();

    int input;

    while (true) {
        resize_and_clear_ui();

        if (COLS > 40 && COLS < 200
            && LINES > 10)
        {
            draw_menubar();
            draw_panel_borders();
            refresh();
        }

        input = getch();

        switch (input) {
            case KEY_ESC: goto exit;

            default: break;
        }
    }

    // char prog[] = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    // run_brainfuck(prog, sizeof(prog) - 1);

exit:
    shutdown_ui();
    return 0;
}