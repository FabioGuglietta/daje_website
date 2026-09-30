# VIM: A Very Short Introduction

Vim is a text editor that runs directly inside a terminal.

You do not need to learn Vim in depth to use it effectively. For now, the goal is much simpler:

* open a file, 
* write some text,
* move text around,
* save the file,
* exit.

That is enough for most of what we will need in this course.

---

## 1. Opening a file

From the terminal:

```bash
vim file.txt
```

If `file.txt` already exists, Vim opens it.

If it does not exist, Vim starts with an empty buffer and the file will be created when you save it.

For example:

```bash
vim hello.c
```

---

## 2. The one thing you must understand: Vim has modes

Vim does not behave like a normal graphical text editor.

At the beginning, Vim is in **COMMAND mode**.

In COMMAND mode, pressing a key does not normally insert that character into the file. Keys are interpreted as commands.

For example, pressing

```text
i
```

means:

**enter INSERT mode.**

In **INSERT mode**, the keyboard behaves more like an ordinary text editor: what you type appears in the file.

You will normally see

```text
-- INSERT --
```

at the bottom of the screen.

To return to COMMAND mode, press:

```text
ESC
```

The basic Vim cycle is therefore:

```text
COMMAND mode
     |
     | i
     v
 INSERT mode
     |
     | ESC
     v
COMMAND mode
```

If you are ever unsure about which mode you are in, press `ESC`.

---

## 3. Writing text

Open a file with `vim hello.txt`: Vim starts in COMMAND mode. 

Then press `i`: you are now in INSERT mode.

Type normally:
```text
Hello world.

This is my first file edited with Vim.
```

When you have finished writing, press: `ESC`

You are back in COMMAND mode.

---

## 4. Moving around

For the moment, using the arrow keys is perfectly fine.

You can use:

```text
↑
↓
←
→
```

Vim also has its traditional movement keys:

```text
h    left
j    down
k    up
l    right
```

So:

```text
        k
        ↑
    h ←   → l
        ↓
        j
```

These become useful when working on remote machines or when you become more comfortable with Vim.

For this course, knowing the arrow keys is sufficient.

---

## 5. Adding text at a particular position

Move the cursor to the desired position while in COMMAND mode.

Press `i`  and start typing: `i` means **insert before the cursor**.

Two other useful commands are:
* `o`: it creates a new line **below** the current one and enter INSERT mode.
* `O`: it creates a new line **above** the current one and enter INSERT mode.

You do not need all of these commands immediately. `i` and `ESC` are enough to start.

---

## 6. Copying and pasting a line

In Vim, copying is traditionally called **yanking**.

Suppose the file contains:

```text
first line
second line
third line
```

Move the cursor to:

```text
second line
```

Make sure you are in COMMAND mode:

```text
ESC
```

Then press:

```text
yy
```

This copies the entire current line.

Now move the cursor somewhere else and press:

```text
p
```

The copied line is pasted **below** the current line.

> [!NOTE]
> If you want to past the copied line **above** the current line, press `P`.


So the three commands to remember are:
```text
yy    copy current line
p     paste below
P     paste above
```

---

## 7. Copying several lines

Commands in Vim can often be preceded by a number.

For example:

```text
3yy
```

copies three lines starting from the current line.

Then:

```text
p
```

pastes those three lines.

---

## 8. Copying only part of a line

Sometimes you do not want to copy an entire line.

In that case, the easiest method is **VISUAL mode**.

Start in COMMAND mode:

```text
ESC
```

Press:

```text
v
```

You can now move the cursor using the arrow keys.

The text you move across will be selected.

For example:

```text
The temperature is very high today.
        ^^^^^^^^^^^
```

Once the desired text is selected, press:

```text
y
```

to copy it.

Move the cursor to the desired destination and press:

```text
p
```

to paste it.

The sequence is therefore:

```text
ESC
v
select text
y
move somewhere else
p
```

This is the simplest way to copy arbitrary strings of text.

---

## 9. Copying entire lines with Visual mode

There is also a version of VISUAL mode specifically for lines.

Press:

```text
V
```

Notice that this is uppercase `V`.

The complete current line is selected.

Use the arrow keys to select more lines.

Then press:

```text
y
```

to copy them.

Move somewhere else and press:

```text
p
```

to paste.

For example:

```text
ESC
V
↓
↓
y
```

selects the current line and the next two lines and copies them.

---

## 10. Cutting and moving a line

Sometimes you want to **move** a line rather than copy it.

In COMMAND mode:

```text
dd
```

deletes the current line.

But Vim also keeps the deleted line in memory.

Therefore:

```text
dd
```

followed by

```text
p
```

effectively moves a line.

Example:

```text
one
two
three
```

Place the cursor on `two` and type:

```text
dd
```

You obtain:

```text
one
three
```

Move to `three` and press:

```text
p
```

You obtain:

```text
one
three
two
```

So:

```text
yy    copy line
dd    cut line
p     paste
```

---

## 11. Saving

Before saving, return to COMMAND mode:

```text
ESC
```

Then type:

```text
:w
```

and press `Enter`.

`w` means **write**.

So:

```text
:w
```

means:

> write the current contents to the file.

If you started Vim without specifying a filename:

```bash
vim
```

you can save the buffer as a new file with:

```text
:w file.txt
```

---

## 12. Saving and quitting

The most useful command is:

```text
:wq
```

It means:

```text
write + quit
```

The complete sequence is therefore:

```text
ESC
:wq
ENTER
```

This saves the file and closes Vim.

---

## 13. Quitting without saving

If you changed the file and try:

```text
:q
```

Vim will normally refuse to exit because there are unsaved changes.

To discard the changes and quit:

```text
:q!
```

The `!` means, approximately:

> do it anyway.

Use it carefully.

---

## 14. The commands you actually need

For now, remember only these:

| Command | Meaning |
|---|---|
| `vim file.txt` | open a file |
| `i` | start inserting text |
| `ESC` | return to COMMAND mode |
| arrows | move around |
| `yy` | copy one line |
| `3yy` | copy three lines |
| `v` | visually select characters/text |
| `V` | visually select complete lines |
| `y` | copy selected text |
| `dd` | cut/delete current line |
| `p` | paste after/below |
| `P` | paste before/above |
| `:w` | save |
| `:q` | quit |
| `:wq` | save and quit |
| `:q!` | quit without saving |

---

## 15. A complete example

Open a file:

```bash
vim notes.txt
```

Press:

```text
i
```

and type:

```text
This is the first line.
This is the second line.
This is the third line.
```

Press:

```text
ESC
```

Move the cursor to the second line.

Copy it:

```text
yy
```

Move to the third line.

Paste:

```text
p
```

The file now contains:

```text
This is the first line.
This is the second line.
This is the third line.
This is the second line.
```

Save and quit:

```text
:wq
```

Press `Enter`.

You are back in the shell.

---

## The essential mental model

Do not try to remember dozens of Vim commands.

Remember the basic cycle:

```text
vim file.txt

        ↓

COMMAND MODE

        ↓ i

INSERT MODE
type your text

        ↓ ESC

COMMAND MODE
copy / paste / move

        ↓

:wq

        ↓

save and exit
```

If you can do this, you already know enough Vim to edit files from a terminal.
