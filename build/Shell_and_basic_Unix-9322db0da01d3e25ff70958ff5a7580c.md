# The Shell and Basic Unix Commands (Basic)

This documents provides you some information on some basic unix commands. You should try them out in a terminal as you read, usually this helps remembering them faster.

> Using a command-line-interface is one of the most powerful ways to use with a computer. Unix, Linux and Mac OS all provide so called "shells", or command line interpreters running in "terminals" that allow you to invoke ("run") other programs by typing commands on your keyboard. If you boot a linux machine you can access the system through several terminals. Press `ctrl+alt+Fn` (Fn being a function key) to access them, use F7 or higher to switch back to the graphical user interface (GUI), if it has started. Otherwise launch a terminal in a window in your GUI. Typical Linux terminals include `xterm` or similar applications. On Mac OS you will have to use the `Terminal.app` located in the Applications/Utilities folder.
> The Bash (*Bourne again shell*) is a popular shell, written originally in 1978 and available (and often the default shell) in almost all variants of Unix/Linux and Mac OS (where the superset zsh is used).
>
>Once you launch a terminal, the shell will run, and show you the so-called *prompt*. 
  

Moving around the filesystem, inspecting, creating and deleting directories
==========================

```bash
> cd    # change directory to your home (when no other arguments are passed)
 
> cd ~  # same
 
> cd .. # go to the parent directory (mind the space)
 
> cd <directory>  # go to <directory>. This has to be either an absolute path `/home/m.sega/test` or a relative one (without the leading slash)
 
> cd -  # go back to the directory where you were before. ```
```

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
> pwd                 # prints working directory (the path of the directory where you are)
> ls                  # list all the files in the working directory
> ls <dir>            # list directoy contents

 
> mv sample_file directory                          # moves the file into the directory
> mv directory/sample_file directory/renamed_file   # renames the file
```

> [!CAUTION]
> `rm` is irreversible! There is no "bin". Once removed, it no longer exists.

### Wildcards and globbing

  

The simplest wildcards in bash are the asterisk '\*' (or star) and the question mark ( ? ). The first matches an indefinite number of any characters, the second one any single character.

So, for example, if you have a series of files named `data.1.dat, data.2.dat, ..., data.3277.dat`, and you want to move them to a subdirectory, just type:

```bash
> mkdir subdir
> mv data.*.dat subdir/
```
  

See other examples at the [TLDP](https://tldp.org/LDP/GNU-Linux-Tools-Summary/html/x11655.htm)


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


Getting help
============

  

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
