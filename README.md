# Brainfuck IDE

A TUI-based IDE for writing, running, and debugging **brainfuck** programs, built with PDCurses.
(i apologize for the insanely bad and messy code)

For a guide on how to use it and how the editor works, see [GUIDE.md](GUIDE.md).

---

## Features

- Syntax-highlighted editor
- Built-in interpreter
- Memory tape visualizer
- Program output panel

---

## Dependencies

- [PDCurses](https://github.com/wmcbrine/PDCurses) (MIT license)

---

## AI Usage Disclaimer

I used AI for writing the github workflow, a part of the README youre reading right now and for debugging when stuff was broken a lot :(
I sometimes also used AI for working through the concepts like the popup system in order to have a good architecture in my head before I start writing funny code

---

## Supported Platforms
| OS      | Supported? |
|---------|------------|
| Windows | ✅ Yes      |
| Linux   | ❌ No       |
| macOS   | ❌ No       |

> If you are willing to add support for any of the unsupported platforms, I'd be happy to merge a PR! :) 

---

## How to try it yourself

The IDE is self-contained. It does not need any external libraries or whatsoever. To run it, simply open the executable file.

### Special Program: `demo.bf`

When a file named `demo.bf` exists, the IDE will automatically load it on start.
The demo file is located [here](assets/demo.bf) and also included in all releases (`1.1.0` and later) 

---

## Brainfuck Primer

Brainfuck operates on an array of memory cells (the "tape"), each initialized to zero, with a pointer starting at the leftmost cell. There are only 8 commands:

| Command | Effect                                                         |
|---------|----------------------------------------------------------------|
| `>`     | Move the pointer right                                         |
| `<`     | Move the pointer left                                          |
| `+`     | Increment the current cell                                     |
| `-`     | Decrement the current cell                                     |
| `.`     | Output the current cell as an ASCII character                  |
| `,`     | Read one byte of input into the current cell                   |
| `[`     | If the current cell is zero, jump forward to the matching `]`  |
| `]`     | If the current cell is non-zero, jump back to the matching `[` |

Everything else in the source is treated as a comment.

### Hello, World!

```brainfuck
++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
```

For a full language reference, see the [Wikipedia article on Brainfuck](https://wikipedia.org/wiki/Brainfuck).

---

This project is licensed under the **GNU General Public License v3.0** (`GPL-3.0-only`). See the [LICENSE](LICENSE) file for details.

---

## Gallery

![The IDE](assets/preview_main.png)
![The Save Dialog](assets/preview_popup.png)
