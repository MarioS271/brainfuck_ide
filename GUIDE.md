# Brainf\*ck IDE

Hello and welcome to my project: an IDE for the brainf\*ck 
programming language!
Start off by reading the **Short Summary** and the **Known Issues**. If you're interested in the details, feel free to read the rest. :)

> **IMPORTANT:** This IDE does NOT support mouse inputs. You need to use the keyboard to navigate the menubar. Clicking on the menubar options will NOT work, you need to first press `ESC` and then use the arrow keys and press enter to select one of the options.

**Table of Contents**
1. [What is it](#what-is-it)
2. [How to use it](#how-to-use-it)
   - [The Menubar](#the-menubar)
   - [The Editor Panel](#the-editor-panel)
   - [The Output Panel](#the-output-panel)
   - [The Tape Panel](#the-tape-panel)
3. [Short Summary](#short-summary)
4. [Known Issues](#known-issues)
5. [Debug Mode](#debug-mode)

---

## What is it
Brainf\*ck IDE is a text editor, interpreter and debugger for brainf\*ck. It is a powerful tool for developing brain\*ck applications (if you wanna do that for some reason)

## How to use it
The editor has two modes, `Normal` and `Debug`. Normally, youre in normal mode, but you can switch between normal and debug by using the `DEBUG` button.

The editor is divided into three panels and a menubar.
The three panels are:
 - Editor Panel (Top Left)
 - Output Panel (Top Right)
 - Tape Panel (Bottom)

---

## Short Summary
Brainf\*ck IDE is a terminal-based text editor, interpreter and debugger for the brainf\*ck programming language. It features three panels (Editor, Output, and Tape) and a menubar for running, debugging, saving, and loading programs. It operates in two modes: Normal mode, which shows the tape state at the current cursor position for easy code writing, and Debug mode, which steps through the program like a real program counter and prints output live. The tape panel visualizes memory cells with their values, indices, and ASCII representations in real time.

## Known Issues
 - Maximizing the window and resizing it larger than a specific size causes the contents to dissapear, to reverse just make the window smaller again
 - Resizing the window smaller than the size of a popup causes a heap corruption
  
> I would recommend avoiding resizing the window unless absolutely necessary 😅

---

### The Menubar
The menubar contains options like:
 - Run Program (`RUN`)
 - Enter/Exit Debug Mode (`DEBUG`)
 - Save current program to file (`SAVE`)
 - Load program into IDE from file (`LOAD`)
  
You can switch to the menubar at any time (except when a popup is open) by pressing the escape key.

### The Editor Panel
The editor panel is a normal text editor, complete with line numbers, soft wrapping, syntax highlighting and more.
It supports quite a few keybinds, including:

| Key         | Function                   |
|-------------|----------------------------|
| Up Arrow    | Navigate one line up       |
| Down Arrow  | Navigate one line down     |
| Left Arrow  | Navigate one char backward |
| Right Arrow | Navigate one char forward  |
| Home        | Move cursor to line start  |
| End         | Move cursor to line end    |
| Backspace   | Delete char before cursor  |
| Delete      | Delete char after cursor   |
| Enter       | New Line                   |

...and of course lower and uppercase letters, numbers and a few special chars too.

### The Output Panel
The output panel always displays the current program's output, complete with soft line wrapping and more.

### The Tape Panel
The tape panel always displays the state of the "tape" (the thing that holds all the data which the program works with).

The displayed tape looks like this:
<pre>
 NUL   NUL    H     d     W     !    LF    NUL   ESC   BEL
[000] [000] <mark style="background:white">[072]</mark> [100] [087] [033] [010] [000] [027] [007]
 000   001  <mark style="background:white"> 002 </mark>  003   004   005   006   007   008   009
</pre>

The middle row shows the current cells value. The bottom row shows the cells index and the top row shows the cells value in an ASCII representation. The cell thats currently being operated on (where the language's data pointer points to) is highlighted in white.

If we're in normal mode, the tape panel displays what the state of the tape looks like at the current cursor position to make it easy to quickly write brainf\*ck code without manually counting.

> **Important:** In normal mode, the tape does not correctly show loops. If you're inside a loop body, You will always be observing the first iteration of it. As soon as you move the cursor to after the loop, all loop iterations get computed immediately.

## Debug Mode
If you actually wanna be able to correctly step through loops, thats where the debug mode comes in handy. The debug mode is read-only (you cant type in it) and the cursor actually jumps like the program counter would when stepping through the program. In debug mode, output also gets printed live.

When entering debug mode, you will be asked for a number. Enter the number of the row you wanna start debugging in.

> **Important:** It is possible that by entering very large values, the program may crash. I advise you not to do so if you don't wanna lose your work.
