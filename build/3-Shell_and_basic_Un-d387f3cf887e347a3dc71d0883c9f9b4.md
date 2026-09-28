# The Shell and Basic Unix Commands: Navigating the Unix Shell

This document introduces some basic Unix commands. You should try them in a terminal while reading: using commands directly is usually the fastest way to remember them.
Using a **command-line interface** is one of the most powerful ways to interact with a computer. Unix, Linux, and macOS provide so-called **shells**, that is, command-line interpreters running inside **terminals**. A shell allows you to invoke, or run, other programs by typing commands on the keyboard.

## 1. What is a shell?

A **shell** is a program that reads commands typed by the user and asks the operating system to execute them.
For example, when you type:
```bash
ls
```
the shell interprets this command and runs the program `ls`, which lists the contents of the current directory.

The **shell is therefore an interface between the user and the operating system**:
```text
user → shell → operating system → programs/files
```
There are several Unix shells. One of the most common is **Bash**, the _Bourne Again SHell_. Bash was developed as a free replacement for the earlier Bourne shell and is available on most Unix/Linux systems.

On recent versions of macOS, the default shell is usually **zsh**, but Bash is still commonly available. For the basic commands introduced here, the differences between Bash and zsh are not important.

## 2. Opening a terminal

To use a shell, you usually open a terminal. The **terminal is the window or text interface in which the shell runs**.

On a Linux machine, you can interact with the system through one or more **terminals**.

If you are working directly on a Linux computer, you can usually switch to a text-only terminal using:
```text
Ctrl + Alt + F1
Ctrl + Alt + F2
```
Here `F1`, `F2`, etc. are function keys. These text terminals are often called **virtual consoles**.

To return to the graphical interface, use another function key, commonly `F7` or one of the higher-numbered keys. The exact key may depend on the Linux distribution and desktop environment.

More commonly, you will open a terminal window inside the graphical interface. Typical Linux terminal applications include `xterm`, `gnome-terminal`, `konsole`, or similar programs.

On macOS, use **Terminal.app**, located in:
```text
Applications → Utilities → Terminal
```

## 3. The prompt

Once you open a terminal, the shell starts and shows a **prompt**.

The prompt is the text displayed by the shell when it is ready to accept a command. It may contain information such as: your username, the name of the computer, the current directory, etc. For example, a prompt may look like this:

```bash
fabio@laptop:~/Documents$
```

or simply:

```bash
>
```

In these notes, we will use `>` to represent the prompt.

> [!IMPORTANT]
> The prompt is not part of the command.
>
> If you see an example such as:
>
> ```bash
> > ls
> ```
>
> you should type only:
>
> ```bash
> ls
> ```

When the prompt is visible and followed by a static or blinking cursor, the shell is ready to accept a new command.

If the prompt is not visible, the shell may not be ready for a new command. For example, a program may still be running or waiting for input.

To run a command, type it and press `Enter`.

For example:

```bash
> echo "hello world"
hello world
```

Here:

- `echo` is the command;
- `"hello world"` is an argument passed to the command;
- `hello world` is the output printed by the command.

Many commands have the general form:

```text
command [options] arguments
```

For example:

```bash
> ls -l Documents
```

Here:

- `ls` is the command;
- `-l` is an option;
- `Documents` is an argument.

Options usually modify the behavior of a command. Arguments usually tell the command what to operate on.

## 4. First commands: `pwd`, `ls`, `echo`

Let us start with three simple commands:
- `pwd`
- `ls`
- `echo`

### `pwd`

The command `pwd` means **print working directory**.
It shows the directory in which you currently are.
```bash
> pwd
/home/m.sega
```
The output tells us that the current directory is:
```text
/home/m.sega
```

### `ls`

The command `ls` lists the contents of the current directory.
```bash
> ls
Applications  Documents
```
In this example, the current directory contains two entries:
```text
Applications
Documents
```
You can also use `ls` to inspect another directory:
```bash
> ls Documents
Articles  Notes  Minutes.pdf
```

### `echo`

The command `echo` prints text to the screen.
```bash
> echo "hello world"
hello world
```
In this example, the shell runs the command `echo` with the argument `"hello world"`.

The output is:
```text
hello world
```
At first sight, `echo` may seem trivial. However, it is useful because it can print variables, generate text, and send output to files.

For example:
```bash
> echo "This is a test"
This is a test
```
Later, we will see that this output can also be redirected to a file.

## 5. Paths: absolute, relative, `.`, `..`, `~`

Files and directories in Unix-like systems are organized in a **directory tree**.

A **path** tells the shell where a file or directory is located.

For example, imagine the following simplified directory tree:
```text
/
└── home
    └── m.sega
        ├── Applications
        └── Documents
            ├── Articles
            ├── Notes
            └── Minutes.pdf
```
The top of the tree is the root directory, written as:
```text
/
```
Inside `/`, there is a directory called `home`. Inside `home`, there is a directory called `m.sega`. Inside `m.sega`, there are two directories: `Applications` and `Documents`.

The file `Minutes.pdf` is inside the directory `Documents`.

Therefore, the full path of `Minutes.pdf` is:
```text
/home/m.sega/Documents/Minutes.pdf
```
This representation is useful because it allows us to describe the position of every file or directory in the system.

There are two main kinds of paths:
- **absolute paths**;
- **relative paths**.

### Absolute paths
An **absolute path** starts from the root directory `/`.
For example:
```text
/home/m.sega/Documents
```
This path identifies the directory `Documents` inside the home directory of the user `m.sega`.

Because it starts with `/`, it does not depend on the directory where you currently are.

For example, this command lists the content of that directory:
```bash
> ls /home/m.sega/Documents
```
It will work whether your current directory is `/home/m.sega`, `/tmp`, `/home/m.sega/Documents/Articles`, or any other directory.

### Relative paths

A **relative path** is interpreted starting from the current directory.
For example, suppose your current directory is:
```text
/home/m.sega
```

and suppose the directory tree is:
```text
/home/m.sega
├── Applications
└── Documents
    ├── Articles
    ├── Notes
    └── Minutes.pdf
```
Then the relative path:
```text
Documents/Articles
```
means:
```text
starting from /home/m.sega,
enter Documents,
then enter Articles
```
Therefore, it refers to the absolute path:
```text
/home/m.sega/Documents/Articles
```
So these two commands refer to the same directory:
```bash
> ls Documents/Articles
> ls /home/m.sega/Documents/Articles
```
The **first command uses a relative path**, because `Documents/Articles` is interpreted starting from the current directory.

The **second command uses an absolute path**, because `/home/m.sega/Documents/Articles` starts from the root directory `/`.

If your current directory changed, for example to:
```text
/tmp
```
then the relative path:
```
Documents/Articles
```
would no longer refer to:
```
/home/m.sega/Documents/Articles
```
because relative paths always depend on the directory where you currently are.

### Special path symbols

Some special symbols are frequently used in paths.
```bash
~	  # your home directory
.	  # the current directory
..	# the parent directory
./	# a path starting from the current directory
../	# a path starting from the parent directory
```

For example:
```bash
> ls ~  # lists the content of your home directory.
> ls .  # lists the content of the current directory.
> ls .. # lists the content of the parent directory.
```
If your current directory is:
```bash
/home/m.sega/Documents
```
then:
```bash
.   # means: /home/m.sega/Documents
```
while:
```bash
..   # means: /home/m.sega
```

The notation `./` is often used when we want to explicitly say “starting from the current directory”.

For example:
```bash
> ls ./Documents
```
means:
```bash
> ls Documents
```
provided that `Documents` is inside the current directory.

Similarly:
```bash
> ls ../Documents
```
means: go one level up, then look for a directory called `Documents`.

## 6. Moving around the filesystem

To move from one directory to another, we use the command `cd`, which means **change directory**.

### Going to another directory
Suppose we are in the directory:
```text
/home/m.sega
```
and the directory tree is:
```
/home/m.sega
├── Applications
└── Documents
    ├── Articles
    ├── Notes
    └── Minutes.pdf
```
We can move into the directory `Documents` with:
```bash
> cd Documents
```
Now the current directory has changed. We can check it with `pwd`:
```bash
> pwd
/home/m.sega/Documents
```
We can now move into `Articles`:
```bash
> cd Articles
```
and check again:
```bash
> pwd
/home/m.sega/Documents/Articles
```

### Going to the parent directory

The symbol `..` means parent directory.

If we are in:
```
/home/m.sega/Documents/Articles
```
then:
```bash
> cd ..
```
moves us to:
```
/home/m.sega/Documents
```
We can check this with:
```bash
> pwd
/home/m.sega/Documents
```
Running `cd ..` again moves us one more level up:
```bash
> cd ..
> pwd
/home/m.sega
```

### Going to the home directory

If `cd` is used without arguments, it brings us back to the home directory:
```bash
> cd
> pwd
/home/m.sega
```
The same result can be obtained with:
```bash
> cd ~
```
because `~` represents the home directory.

### Going back to the previous directory

The command:
```bash
> cd -
```
moves back to the directory where you were before.

For example:
```bash
> pwd
/home/m.sega
> cd Documents/Articles
> pwd
/home/m.sega/Documents/Articles
> cd -
/home/m.sega
> pwd
/home/m.sega
```

### Relative and absolute paths with `cd`

The command `cd` can be used with both relative and absolute paths.

For example, if we are in:
```
/home/m.sega
```
we can move to `Articles` using a relative path:
```bash
> cd Documents/Articles
```
or using an absolute path:
```bash
> cd /home/m.sega/Documents/Articles
```
Both commands bring us to the same directory.

The difference is that the relative path depends on the current directory, while the absolute path starts from `/` and therefore works independently of the current directory.

Useful `cd` commands
```bash
> cd                 # go to your home directory
> cd ~               # go to your home directory
> cd ..              # go to the parent directory
> cd Documents       # go to Documents, if it is inside the current directory
> cd /tmp            # go to /tmp using an absolute path
> cd -               # go back to the previous directory
```

### Exercise

Starting from your home directory, practice moving around the filesystem:
```bash
> pwd
> ls
> cd Documents
> pwd
> cd ..
> pwd
> cd ~
> pwd
```
Then try the same operations using absolute paths.


<!-- OLD STUFF
# The Shell and Basic Unix Commands
This documents provides you some information on some basic unix commands. You should try them out in a terminal as you read, usually this helps remembering them faster.

Using a command-line-interface is one of the most powerful ways to use with a computer.
Unix, Linux and Mac OS all provide so called "shells", or command line interpreters running in "terminals" that allow you to invoke ("run") other programs by typing commands on your keyboard.

> [!NOTE]
> On a Linux machine, you can interact with the system through one or more **terminals**.
>
> If you are working directly on a Linux computer, you can usually switch to a text-only terminal using:
>
> ```text
> Ctrl + Alt + F1
> Ctrl + Alt + F2
> ...
> ```
>
> Here `F1`, `F2`, etc. are function keys. These text terminals are often called **virtual consoles**.
>
> To return to the graphical interface, use another function key, commonly `F7` or one of the higher-numbered keys. The exact key may depend on the Linux distribution and desktop environment.
>
> More commonly, you will open a terminal window inside the graphical interface. Typical Linux terminal applications include `xterm`, `gnome-terminal`, `konsole`, or similar programs.
>
> On macOS, use **Terminal.app**, located in:
>
> ```text
> Applications → Utilities → Terminal
> ```

The Bash (*Bourne again shell*) is a popular shell, written originally in 1978 and available (and often the default shell) in almost all variants of Unix/Linux and Mac OS (where the superset zsh is used).

Once you launch a terminal, the shell will run, and show you the so-called *prompt*. 

When you see the prompt followed by a static or blinking cursor, the shell is ready to accept commands. If you don't see it, you can still type, but what you type will not be interpreted as a command. 

My prompt is very simple ">". Yours might be longer, showing some information on the directory you are in, or on which computer, and so on. 

Let's run three different commands given at the prompt (press "enter" at the end of each command (`pwd`, `ls`, `echo "hello world"`) to see the output:
  

```bash
> pwd
/home/m.sega
 
> ls
Applications     Documents
 
> echo "hello world"
hello world
```
  

First, `pwd`, (print working directory) shows you where you currently are on the directory tree. Then, `ls`  lists the content of the directory where you currently are. Finally, `echo "hello world"` simply outputs what I have written within quotation marks to the screen (no, it's not that useless, it can expand variables too, it can be redirected to write to a file and so on...)


There are some special sequences of characters that are expanded into directories, that you must be aware of. These include:

```bash
> ~    # your home directory
> ./   # also just . if not followed by another part of a path. The current directory
> ../  # also just .. if not followed by another part of a path. The parent directory
```
  

Now comes an example where the prompt disappears because the system is waiting for input in the form of text:

```bash
>cat > file.txt
This is a test
^D
>
```

The command `cat` (catenate) is used to concatenate the content of several files or, as in this case, with the "redirection to file" character `>` , to wait for keyboard input until `ctrl+D` is hit (appearing as `^D` ). Until `ctrl+D` is pressed, `cat` will keep adding what you type to the file `file.txt` .

Let's use `cat` again to see the result by the content of file `file.txt` to screen.

```
> cat file.txt
This is a test
>
```

Here are some categories of important commands to know.

Getting help
------------

  

type `man <command>` to obtain the manual page of (almost) any command. Learn how to interpret the output of man pages. For example the command `ls` is used to list contents of a directory.

  
```man
LS(1)                     BSD General Commands Manual                    LS(1)
 
NAME
     ls -- list directory contents
 
SYNOPSIS
     ls [-ABCFGHLOPRSTUW@abcdefghiklmnopqrstuwx1%] [file ...]
 
DESCRIPTION
     For each operand that names a file of a type other than directory, ls displays its name as well as any requested, associated informa-
     tion.  For each operand that names a file of type directory, ls displays the names of files contained within that directory, as well
     as any requested, associated information.
 
     If no operands are given, the contents of the current directory are displayed.  If more than one operand is given, non-directory oper-
     ands are displayed first; directory and non-directory operands are sorted separately and in lexicographical order.
 
     The following options are available:
 
     -@      Display extended attribute keys and sizes in long (-l) output.
 
     -1      (The numeric digit ``one''.)  Force output to be one entry per line.  This is the default when output is not to a terminal.
 
     -A      List all entries except for . and ...  Always set for the super-user.
 
     -a      Include directory entries whose names begin with a dot (.).
:
```   



  

Hit `<spacebar>` to advance quickly or `B` to go back. Use the cursor keys to move line-by line. Hit `Q` to exit.

  

Other programs have a built-in help, that you can invoke by passing, typically the options `-h` or `--help` , for example:


```bash
> man --help
```

yields
```
man, version 1.6g
 
usage: man [-adfhktwW] [section] [-M path] [-P pager] [-S list]
    [-m system] [-p string] name ...
 
  a : find all matching entries
  c : do not use cat file
  d : print gobs of debugging information
  D : as for -d, but also display the pages
  f : same as whatis(1)
  h : print this help message
  k : same as apropos(1)
  K : search for a string in all pages
  t : use troff to format pages for printing
  w : print location of man page(s) that would be displayed
      (if no name given: print directories that would be searched)
  W : as for -w, but display filenames only
 
  C file   : use `file' as configuration file
  M path   : set search path for manual pages to `path'
  P pager  : use program `pager' to display pages
  S list   : colon separated section list
  m system : search for alternate system's man pages
  p string : string tells which preprocessors to run
               e - [n]eqn(1)   p - pic(1)    t - tbl(1)
               g - grap(1)     r - refer(1)  v - vgrind(1)
```

  

Moving around the filesystem, inspecting, creating and deleting directories
---------------------------------------------------------------------------

  

```bash
> cd    # change directory to your home (when no other arguments are passed)
 
> cd ~  # same
 
> cd .. # go to the parent directory (mind the space)
 
> cd <directory>  # go to <directory>. This has to be either an absolute path `/home/m.sega/test` or a relative one (without the leading slash)
 
> cd -  # go back to the directory where you were before. ```
```

Let's see an example

  

```bash
> pwd
 /home/m.sega
 
> ls
 Applications    Documents
 
> ls -l Documents  # -l for more infos including permission, owner, group, size and creation date.
  
total 165192
-rw-r--r--@  1 sega  staff    598402 Dec  3  2021 Minutes.pdf
drwxr-xr-x@ 23 sega  staff       736 Aug 10 10:03 Articles
 
> pwd
/home/m.sega
 
> cd Documents/Articles  # move to a relative path from where we are
 
> pwd
/home/m.sega/Documents/Articles
 
> cd -   # go back to where we were before
 
> pwd
/home/m.sega
```

### Exercise

Figure out what the following options of `ls` are doing.

  

**ls -a  
ls -R**

  

Other commands to deal with files and directories

```bash
> mkdir <dir>         # creates the directory <dir>
> rmdir <dir>         # removes the directory <dir>, if empty
> touch <file>        # create an empty file named <file>
> rm <file>           # remove the file <file>
> rm -rf <dir>        # remove the directory and its contents (-f to force), recursively (-r)
 
> mv <source> <dest>  # move file or directory <source> to <dest>. Used also to rename them.
> cp <source> <dest>  # copy file <source> to <dest> <dest> could be a new or existing file
                      # (that will be overwritten), or a destination directory where the file
                      # is copied.
 
> touch sample_file                                 # creates an empty file
> mkdir directory                                   # creates an empty directory
> mv sample_file directory                          # moves the file into the directory
> mv directory/sample_file directory/renamed_file   # renames the file
```

### Wildcards and globbing

  

The simplest wildcards in bash are the asterisk '\*' (or star) and the question mark ( ? ). The first matches an indefinite number of any characters, the second one any single character.

So, for example, if you have a series of files named `data.1.dat, data.2.dat, ..., data.3277.dat`, and you want to move them to a subdirectory, just type:

```bash
> mkdir subdir
> mv data.*.dat subdir/
```
  

See other examples at the [TLDP](https://tldp.org/LDP/GNU-Linux-Tools-Summary/html/x11655.htm)

### Exercise

Play with copying and moving files around.

### Variables

You can assign variables and use them this way:

  

```bash
> base='/home/m.sega/data/'
> mv ${base}/data.*.dat subdir/
```

If you are not sure into what a variable will expand, just use echo

```bash
> base='/home/m.sega/data'
> echo "${base}/data.*.dat"
/home/m.sega/data/data.*.dat
```

Note: Bash does not care about how many consecutive '/' are there in a path. So `'./data/file.txt'` is the same as `'./data////file.txt'.` So, if in doubt, always add a '/' in variables that represent directories. Because `/home/m.sega/data/////data.*.dat` might be a valid path, but `/home/m.sega/datadata.*.dat` not.

  

### Environment variables

Some variables are automatically set by the shell. You can see all of them by typing `export`

Some example include

```bash
> echo $HOME
/home/m.sega
 
> echo $USER
m.sega
 
> echo $PWD
/home/m.sega/data
```

These variables will be available to the programs you launch, not just to the current bash command line. To export a variable you decleared, just do

```bash
> MYVAR='this string'
> export MYVAR
# OR
> export MYVAR='this string'
```

### Single and double quotes

Understanding the difference between the variable-expanding double quotes, and the opposite behaviour of single quotes will be important when you write scripts.

```bash
> VAR="$HOME/data"
> echo $VAR
/home/m.sega/data
 
> VAR='$HOME/data'
> echo $VAR
$HOME/data
```

This is important when you want to generate a string that has the variable name in it (here: $HOME) , and not its associated value (here: /home/m.sega/)

  

Inspecting files
----------------

  

Text files can be dumped to screen ( `stdout` ) with `cat` or can be inspected with the commands `less` or `more` (they are equivalent, more or less :-). Use "<spacebar>" to move down quickly, "B" to move back, and "/" to search.

  

```bash
> less <file>               # inspect the file
> cat  <file1> <file2> ...  # concatenate files together and print them to screen (unless redirection ">" is used )
> head <file>               # show the first 10 lines
> head -n 3 <file>          # show the first 3 lines
> tail <file>               # show the last 10 lines
> tail -n 3 <file>          # show the last 3 lines
```
  

Combining commands
------------------

One of the most powerful capabilities of a shell is to allow to redirect the output of a command directly to another one. A series of commands that feed the next one with ones' output is called a pipeline.

Let's see an example with the `ls` and `wc` commands. The `wc` command, as the name suggests (word count! in the good old times, people had a need for short commands and plenty of humor) counts the characters, words and lines contained in a file, typed in, or passed through a pipeline.

  

```bash
> # we create first a file with some text in
> echo "When shall we three meet again? In thunder, lightning, or in rain?" > witch.txt 
> wc whitch.txt
       1      12      67 witch.txt
# one line, 12 words, 67 characters.
```

  

If a command does not read from file, but only accepts input from `stdin` there's no problem. We can just use `<` , the opposite of the redirection to file `>` . So, we could also count the words in `witch.txt` this way:

```bash
> wc < whitch.txt
       1      12      67 witch.txt
 
# and if we want to store the output in a file, we could just do
 
> wc < whitch.txt > count.dat
> cat count.dat
        1      12      67 witch.txt
```

  

Another way to use `wc` , not very practical in everyday life, shows you how the command accepts input coming from the keyboard.

```bash
> # Counting the words typed
> wc
I am typing something
^D
       1       4      22
# one line, four words, 22 characters
```
  

Knowing that `wc` accepts input also from keyboard or from file through `<` (the so called standard input, or `stdin` ), we can use the "pipe" character `|` (a vertical bar) to combine the output of `ls` (or any other command) and pipe it trough `wc`:

```bash
> ls # let's just check what we have > ls
Applications     Documents
> ls | wc
       2       2      23
# two lines, two words, 23 characters
```

As you can see, the directory content has been sent on a one-by-line basis to `wc` , which counted two files. This is a simple example, but when you have 23472 files in your directory, it's easier to let `wc` do the job rather than counting yourself.

  

You can create a longer pipeline if needed. Let's say we want to count how many files match a given pattern. First, we need to get the strings that match a pattern. Your friend here is `grep.`

```bash
> ls
data1 data2 data3 trash1 trash2
 
> ls | grep data
data1
data2
 
> ls | grep data | wc -l # count lines
2
 
# yes, we could have done this also with the '-c' (count) option of grep
> ls | grep -c data
2
# but this was not the aim of this example...
```


Connecting to remote hosts
==========================

  

Very important if you want to login to your nearest cluster:

  

```bash
> ssh <username>@<hostname>   # open a secure shell (encrypted) connection to <hostname>
 
#example:
 
> ssh ucecxxx@myriad.rc.ucl.ac.uk # if you connect for the first time, it will ask if you trust the host.
                                  # If so, answer 'yes' by spelling the whole word, not just 'y'


```
  

  

  

  

_**This content represents the very basic you need to know to make good use of bash. Go ahead if you want to know slightly more advanced topics.**_

  

  

  

Math with Bash
--------------

  

Bash can handle some simple integer math

```bash
> a=3
> b=$[ $a + 1 ] # notice the spaces
> echo $b
4
```

To handle floating point math you need to use other programs within bash. I recommend awk:


```bash
> a=3.2 ; awk "BEGIN{ print $a*2}"
6.4
```

Note the double quotes. This way bash first expands the variable $a into 3.2, and then it feeds it to awk. If you use single quotes, awk expects (a) to be one if its variables, and expands $a into the value of the a-th column of input. Most tutorials would recommend you to use the command bc instead, as in

```bash
> a=3.2 ; echo "$a * 2" | bc -l
6.4
```

This is indeed shorter. But the other mathematical functions of awk are probably easier to use/remember, and it allows you to write quick c-like code, if needed.

Awk
---

Since we are here, let's make an example of how to use [Awk](https://en.wikipedia.org/wiki/AWK), a powerful tool to process string and numbers

  

```bash
> cat > file.txt
1 2 3
4 5 6
7 8 9
^D. # <- this is ctrl+D
 
# In awk, $1, $2, ... expand into the value of the 1st, 2nd, ... column.
# The separator is by default any number of white spaces, unless specified otherwise with the -F option.
 
> awk '{print $1 * 2, $2 * 3, $3 -1}' file
2 6 2
8 15 5
14 24 8
 
# we can also use internal awk variables and use them directly (a+b+c) or to refer to coluns
> awk '{a=1; b=2; c=3; print $a * 2, $b * 3, $c -1, a+b+c}' file
2 6 2 6
8 15 5 6
14 24 8 6

```

Sed
===

Sed is the [stream editor](https://en.wikipedia.org/wiki/Sed). It processes text line by line using . You can use it to manipulate variables in bash

  

```bash
> cat > file.txt
The cow is a coward
The cows are jumping
^D
 
> sed 's/cow/goat/' file.txt
The goat is a coward
The goats are jumping
 
> sed 's/cow/goat/g' file.txt
The goat is a goatard
The goats are jumping
 
> sed 's/cow\>/goat/g file.txt
The goat is a coward
The cows are jumping
 
# as in other cases, you can let bash expand its own variables within sed, but you must use double quotes
 
> a='cow'
> sed "s/$a/goat/g" file.txt
The goat is a goatard
The goats are jumping
 
# If you need your results in a file, you can redirect stdout
> sed 's/cow/goat/' file.txt > newfile.txt
 
# Or, you can change the file in place with the -i option (not available in all sed implementations)
> sed -i 'sed/cow/goat' file.txt
> cat file.txt
The goat is a coward
The goats are jumping
```
  

  

Loops and conditionals in Bash
------------------------------

  

A simple example shows you how to handle files one by one in a directory

```bash
> ls -l
drwxr-xr-x+   4 Marcello  staff   128 Jul 25 17:21 Public
-rw-r--r--    1 Marcello  staff    15 Aug 24 15:59 file.txt
 
> for f in * ; do echo $f "is some kind of file" ; done
Public is some kind of file
file.txt is some kind of file
 
# bash can tell you, for example if a file is a directory with [ -d <file> ]
 
> for f in * ; do if [ -d $f ] ; then echo $f "is a directory" ; else echo $f "is not a directory" ; fi ; done
Public is a directory
file.txt is not a directory
 
# we can split the loops on multiple lines
 
> for f in * ; do
   if [ $( echo $f | wc -c ) -lt 7 ] ; then
       echo $f "is shorter than 6 characters, will be deleted together with its contents"
       rm -rf $f
   fi
done
 
Public is shorter than 6 characters, will be deleted together with its contents
>
 
# an example with while. Notice that integer "less than" in bash is "-lt"
 
> a=0 ; while [ $a -lt 3 ] ; do a=$[ $a + 1 ] ; echo $a ; done
1
2
3
>
```

Additional resources
====================

  

The list of commands presented here is just the minimal list needed to move around in a unix environment. See some more at the following URLs:

[https://becksteinlab.physics.asu.edu/pages/unix/IntroUnix/p01\_UNIX.html](https://becksteinlab.physics.asu.edu/pages/unix/IntroUnix/p01_UNIX.html)

[https://mally.stanford.edu/~sr/computing/basic-unix.html](https://mally.stanford.edu/~sr/computing/basic-unix.html)

[https://en.wikipedia.org/wiki/List\_of\_Unix\_commands](https://en.wikipedia.org/wiki/List_of_Unix_commands)

  

Some tutorials on bash scripting:

[https://linuxconfig.org/bash-scripting-tutorial-for-beginners](https://linuxconfig.org/bash-scripting-tutorial-for-beginners)

[https://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO.html#toc2](https://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO.html#toc2)

  

And, of course the **Bash reference manual** [https://www.gnu.org/software/bash/manual/html\_node/index.html](https://www.gnu.org/software/bash/manual/html_node/index.html)
-->
