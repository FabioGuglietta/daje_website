# A Short Introduction to VIM
This document is a short introduction to vim, a powerful text-based editor

  

You should consider using vim because

*   it is available virtually on every unix/linux/osx sytem
*   it does not need a GUI to work (you can just connect via ssh to your favourite server and use it straight away)
*   it's very quick in displaying and editing your text
*   it has a powerful set of commands for advanced editing.

If you are programming or manipulating datasets, good chances are you will love it.

You might have heard that learning to use vim is hard. It is not. Just follow these initial steps to start being productive with vim.

Starting vim
------------

To start vim, you can proceed in one of the following ways from :

```bash
> vim

# or

> vim sample.txt # to open a new or an existing file

# or

> vim ./    # to browse the local files. You can use any other target
            # directory rather the one where you are (./)
```

  

Let's choose the first option (just type `vim` ). You will see the following splash screen

  

    ~                                                     
    ~                                                     
    ~                  VIM - Vi IMproved                  
    ~                                                     
    ~                  version 8.2.5032                   
    ~              by Bram Moolenaar et al.               
    ~     Vim is open source and freely distributable     
    ~                                                     
    ~   type  :help iccf<Enter>       for information     
    ~                                                     
    ~   type  :q<Enter>               to exit             
    ~   type  :help<Enter>  or  <F1>  for on-line help    
    ~   type  :help version8<Enter>   for version info    
    ~                                                     
    ~

Don't worry about what all that means, bear with me for a while.

Vim modes
---------

*   vim has two different main operation modes, the COMMAND and the INSERT mode (other modes will be discussed later)
*   in the COMMAND mode, what you type will not be inserted as a text, but will be interpreted as a command
*   in the INSERT mode, what you type will be inserted as text
*   you can recognise that you are in INSERT mode when the text `**--INSERT--**` appears in the bottom line of your editor
*   you can recognise that you are in COMMAND mode when the bottom line of your editor is empty, or has anything else written than `**--INSERT--**`
*   vim starts in COMMAND mode.
*   to switch to INSERT mode, you can press 'i' (you learned your first command!)

INSERT mode
-----------

If you typed `i` in the COMMAND mode (say, from the splash screen above) you will see the following

  
```

~                  VIM - Vi IMproved                  
~                                                     
~                  version 8.2.5032                   
~              by Bram Moolenaar et al.               
~     Vim is open source and freely distributable     
~                                                     
~   type  :help iccf<Enter>       for information     
~                                                     
~   type  :q<Enter>               to exit             
~   type  :help<Enter>  or  <F1>  for on-line help    
~   type  :help version8<Enter>   for version info                                                                
-- INSERT --
```

*   notice the appearance of --INSERT-- in the bottom line
*   notice that the first line is empty (your cursor might appear there, static or blinking depending on your system settings). I placed a hashtag (#) to mark the cursor just this time.
*   all other lines are starting with a tilde (`~`). In vim this means that they are not really lines of text. It's not important here, but in case you're interested: they just don't exist, they're not even empty! an empty line takes space in memory and is the [LF (line feed) character](https://en.wikipedia.org/wiki/Newline), also known as character:`\n`, Unicode : `U+000A`, ASCII : `10`, or hexadecimal code : `0x0a`)
*   try to type something, it will appear in the first line, and the splash screen lines will disappear

  

```
I typed something!
~                                                     
~                                                     
~                                                     
~                                                                                                      
~                                                     
-- INSERT --
```

COMMAND mode
------------

*   now that you typed something, let's exit from INSERT mode by pressing the Escape (ESC) key (you can press it how many times you want, no worries). The --INSERT-- in the bottom line will disappear.
*   let's save what we have written to a file. Since we invoked vim without arguments, our text is in the so-called buffer, but it is not linked to any file one your hard drive.
*   to save the buffer to a file, type (once you're in COMMAND mode - hit ESC if you're not sure!) `:w file.txt` and press Enter

```
I typed something!
and even a second line!
~                                                     
~                                                     
~                                                     
~                                                                                                      
~                                                     
:w file.txt
```
  

You will see the following

  

```
I typed something!
and even a second line!
~                                                     
~                                                     
~                                                     
"file.txt" [New] 2L, 43B written
```
  

The editor tells you in the bottom line that you have written 2 lines (2L), for a grand total of 43 Bytes to a new file named "file.txt"

You can keep switching to INSERT mode, edit what you need, and then save by going to COMMAND mode (ESC) and writing the file (`:w`) - no need to repeat the file name.

Notes:

*   You can move across your written lines both in INSERT and in COMMAND mode using the cursor keys (small arrows).
*   In some cases, the keybinding might be broken, or some configuration of vim might be wrong. In that case, you can **always** move around in COMMAND mode (hit ESC!) using the `h j k l` keys (left down up right). Learn them just in case it's needed

Save and quit
-------------

  

1.  Go to COMMAND mode (ESC)
2.  type `:wq` (write&quit)
3.  done, you're out from vim

  

**Great! Now you know what is needed to use vim at its most basic level.**

# A more in-depth introduction to VIM

What we have discussed so far is very minimal, of course, and your editing skills will be very much enhanced if you learn some of what follows. You don't have to, but it's strongly recommended.

Undo, Redo
----------

  

In COMMAND mode:

*   `u → undo`
*   `ctrl+r` → redo`

  

Optional: Undo and redo commands are moving back and forth the history of commands given or separate INSERT sessions.

This means that if you do this sequence of commands:

`(ESC) (i) type something (ESC)`, by pressing `(u)` once afterwards you will remove the whole sentence "type something".

If instead you use the sequence:

`(ESC) ( i ) type (ESC) ( i ) something (ESC)`, by pressing `(u)` once afterwards you remove only the word "something"

Swap files
----------

It will happen, sooner or later, that you try to open an already opened file, or that your vim crashed leaving behind a swap file. In this case, when opening a file with vim you will be warned by a message like this:

If you have saved your buffer before with `:w` (do it often) you don't have to worry. Just hit (E) for Edit anyway and proceed as usual, or hit (O) for open read-only. In this case, when the time comes, that you want to save again, you will have to forcedly overwrite the file, typing `:w!` instead of simply `:w`. If instead you had unsaved changes that might have been added to the swap file by vim, you might hit (R)ecover and select which swap file to use. I personally never managed to use the recover function properly/usefully. If you do, teach me how, please.

![image](vim1.png)

  

More on handling files
----------------------

*   `:saveas file.txt → save into a different file`
*   `:e file2.txt → open another file (could be new)`
*   `:sp file3.txt → open another file (could be new) in a split window`
*   `:vsp file3.txt → open another file in a vertically split window`
*   `:wa → write all open files/buffers`
*   `:e ./ → open the local directory browser (use any other directory as starting point)   Browse with arrow keys, use (Enter) to enter directories or open files`
*   `:sp ./ → open the local directory browser in a split window`

The split- and vertical split-windows. (`:sp` and `:vsp`) looks like this:

![image](vim2.png)


You can split/vsplit how many buffers you like, for example as in this case:

![image](vim3.png)


To move the focus between buffers, go to COMMAND mode and hit `ctrl+[arrow]` where `[arrow]` is one of the cursor keys to move up/down/left/right through the buffers. If you have multiple buffers you can combine commands and, e.g., use `3+ctrl+[arrow-down]` to move three buffer down from where you are.

Moving around
-------------

The h j k l and arrow keys are sometimes not enough to move around big files. Here are some important commands

in COMMAND mode (remember, hit ESC to get there)

*   `:1 → go to line 1 (remember to press enter)`
*   `:2 → go to line 2 (ok, you get how this goes on)`
*   `G → go to the end of the file (G stands for ... mhhh... Gototheendofthefile)`
*   `$ → go to the end of the line (you will understand why this symbol once you learn regular expressions)`
*   `^ → go to the begginning of the line (same comment as above)`
*   `w → move one word to the right (go to its beginning). Keep pressed to move forward also across lines`
*   `b → move one word to the left (go to its beginning). Keep pressed to move backward also across lines`
*   `} → move to the next paragraph`
*   `{ → move to the previous paragraph`

If you prepend the (w) and (b) commands with a number, you will move by that amount of words, e.g., (3w) will move three words to the right.

Good. Now you also learned that (some) commands in vim can be combined.

You might have also noticed that some commands require a colon ( `:` ) at the beginning. This is a particular command mode which is known as Ex mode (from [ex](https://en.wikipedia.org/wiki/Ex_(text_editor)), the extended editor, of which [vi](https://en.wikipedia.org/wiki/Vi), the father of vim, was the visual mode. Ex improved the life of many programmers back in 1976, freeing them from the vicious but ubiquitous line editor [ed](https://en.wikipedia.org/wiki/Ed_(text_editor)). If you think that vim is difficult to use, try ed once). The Ex-mode commands are mostly used for managing files (opening, saving, ...) and perform extended search/substitution queries. More on this later.

Cut and Paste (and Copy, or Yank)
---------------------------------

### Cut, and examples on how to combine commands

If you are in INSERT mode, you can of course hit the backspace key as many times as you want. If it's just about few characters, it's ok. This can be tiresome if you need to delete several words or paragraphs (sorry, no mouse selection! remember, you might be working on a remote connection, the remote host most likely does not know about your mouse)

in COMMAND mode, if you move the cursor to a specific line or character you can use the following simple commands to quickly **cut your text**

*   `x → delete the current character`
*   `dw → delete the current word (starting from the character where you are)`
*   `db → delete the previous word`
*   `d} → delete the paragraph (was really poorly written)`
*   `dd → delete the current line`

You can delete multiple objects (characters, words, lines) this way:

*   `3x → delete 3 characters`
*   `3dw → delete 3 words`
*   `d3w → same as above`
*   `3dd → delete 3 lines`

You might have noticed how the deletion command (d) is combined with the commands to move around (w), (b), ( } ), and so on, and that you can make longer chains of commands.

There is some mnemonics in this. For example (d3w) reads "delete 3 words". The equivalent command (3dw) reads "thrice delete a word" (now you see why I chose 3 and not 27 for this example)

### Paste

The commands x and dd do not just delete a character or a line. They also store them them temporarily in memory (until you type another similar command).

This can be used if you want to move lines of text around. For example:

1.  go to a line, enter command mode (ESC), cut it (dd) and place it in memory
2.  move to another line of the text, press (p) to **paste** what you have in memory _below_ the current line. Press (P) to paste it _above_ the current line

The paste commands work also for characters cut with (x). Pressing (p) will paste the characters before the location of your cursor. (P) will paste them after.

Do you want to copy a line three times? Simple, hit (3p)

### Copy (Yank)

*   `yy → copies the line where the cursor is to memory`
*   `yw → copies a word from the position of the cursor`

  

REPLACE mode
------------

If you hit (R), you will enter the REPLACE mode (see the appearance of `--REPLACE--` in the bottom line).

In this mode, when you type, instead of inserting text, you will overwrite what is already there.

  

More ways of entering INSERT mode
---------------------------------

You can enter INSERT mode by pressing (i) (I - capital i), (a) or (A). They differ in the following way:

*   `o → add a new line after the one you are in and start inserting there`
*   `O → add a new line before the one you are in and start inserting there`
*   `i → start inserting right before the character where you are (insert)`
*   `a → start inserting right after the character where you are (append)`
*   `I → start inserting at the beginning of the line`
*   `A → start inserting at the end of the line`

If this seems superfluous, think that the various insert commands can be used in macros. Not superfluous anymore, uh? BTW, (o) and (O) are very handy in everyday use.  
  

VISUAL modes
------------

OK, I lied again. Vim has also other operating modes, the VISUAL, VISUAL-LINE and VISUAL-BLOCK modes. The actual mode will be listed in the bottom line of your editor as usual.

You can enter them from COMMAND mode in the following way:

*   `v → enters VISUAL mode`
*   `V → enters VISUAL-LINE mode`
*   `ctrl+V → enters VISUAL BLOCK mode`

To exit a VISUAL mode, hit (ESC) twice

In any of the VISUAL modes, after entering the mode, you can move around the text with the arrows (or `h j k l` ). The selected text will be highlighted. Once you have selected what you want, use any of the commands you know to process that chunk of text (e.g., (y), (x), ...

The ways the text is selected in the different modes is better explained with some screenshots



![image](vim4.png)


![image](vim5.png)



![image](vim6.png)


The VISUAL BLOCK mode is **extremely** handy if you want to move around columns of data. See also the trick!

  

Search and Replace
------------------

One of the most powerful capabilities of vim is to be able to use [regular expressions](https://en.wikipedia.org/wiki/Regular_expression) (RegExp) for search/replace queries. See some links in the . Note that RegExp will pop up not only when using vim but also in different context. Knowing them can give you [a great advantage](https://xkcd.com/208/).

Here we make just a couple of simple example.

### Search

To search for a pattern, hit ( / ) followed by the pattern you are searching. So, if you look for the pattern 'cow', type `/cow` and vim will put the cursor at the first occurrence of the three letters "cow". Hit (n) and it will go to the next occurrence. Hit (N) and it will move to the previous one. If you want to match both 'cow' and 'wow', just use the dot (.) as a jolly character, so, type `/.ow`

### Replace

In Ex-command mode, `:s/<pattern-A>/<pattern-B>/g` replaces `<pattern-A>` with `<pattern-B>`, as many times `(g)` as it finds the matching pattern.

For example:

```
That cow is a coward!
~                                                     
~   
:s/cow/goat/g
```

yields

```
That goat is a goatard!
~                                                     
~
```

If you instead use the full word delimiter at the end (`/>`)

```
That cow is a coward!
~                                                     
~   
:s/cow\>/goat/g
```

you get

```
That goat is a coward!
~                                                     
~
```


If you add 'c' (check) as the third element of the search command, vim will prompt at every occurrence to ask you if you want to replace or not. So, for example, the previous command will become `:s/cow\>/goat/gc`

Note: vim RegExp are slightly different from other implementations (like perl). The world is not perfect.

