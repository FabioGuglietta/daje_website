# Environment Variables, export, and PATH

This lesson explains how variables reach other programs and how the shell locates executables. It builds on [Bash Variables](8-Bash_variables.md).

An **environment variable** is a variable passed to programs launched from the shell.
Many are initialized when you log in; you can also define and export your own.

They contain information about the current user, the current shell, the home directory, the current working directory, and many other aspects of the environment.

Common examples are:
```bash
> echo $HOME
/home/m.sega
> echo $USER
m.sega
> echo $PWD
/home/m.sega/simulation
```
Here:
* `HOME` stores the path of the home directory;
* `USER` stores the username;
* `PWD` stores the current working directory.

You can list environment variables with:
```bash
> env
```
or:
```bash
> printenv
```
## Shell variables and environment variables

A variable created in the shell is not automatically available to programs launched from that shell.

For example:
```bash
> MYVAR="hello"
> echo $MYVAR
hello
```
The shell can see `MYVAR`, but external programs may not see it unless we export it.

To export a variable:
```bash
> export MYVAR
```
or define and export it in one line:
```bash
> export MYVAR="hello"
```
After this, programs started from the shell can access `MYVAR`.

> [!NOTE]
> **Why environment variables are useful**
> 
> Environment variables are often used to configure programs.
> For example, many scientific codes use environment variables to control paths, libraries, thread counts, or output locations.
>
> Examples:
> ```bash
> > export OMP_NUM_THREADS=4
> ```
> This tells OpenMP programs to use 4 threads.
> ```bash
> > export PATH="$HOME/bin:$PATH"
> ```
> This adds the directory `$HOME/bin` to the list of directories where the shell searches for executable programs.

## The `PATH` variable

One of the most important environment variables is `PATH`.

It contains a **list of directories** separated by colons:
```bash
> echo $PATH
/usr/local/bin:/usr/bin:/bin:/home/m.sega/bin
```
When you type a command such as:
```bash
> ls
```
the shell searches for an executable called ls inside the directories listed in `PATH`.

You can ask where a command is located using:
```bash
> which ls
/bin/ls
```
or:
```bash
> command -v ls
/bin/ls
```
If a program is not located in one of the directories listed in `PATH`, the shell will not find it unless you provide its path explicitly.

For example:
```bash
> ./my_program
```
means:
```
run the executable my_program located in the current directory
```
The `./` is needed because the current directory is usually not included in `PATH`.

## Summary

- `env` and `printenv` display the environment.
- `export` makes a variable available to subsequently launched programs.
- `PATH` lists directories searched for executable programs.
- `command -v` identifies how the shell resolves a command.

## Exercise

In a fresh Bash session, assign `DAJE_MESSAGE="hello"` and run
`printenv DAJE_MESSAGE`. Then run `export DAJE_MESSAGE` and repeat the check.
Explain the difference. Finally, inspect `PATH` and use `command -v ls`.
