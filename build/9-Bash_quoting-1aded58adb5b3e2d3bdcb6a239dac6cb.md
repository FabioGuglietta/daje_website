# Single Quotes, Double Quotes, and Variable Expansion

Building on [Bash Variables](8-Bash_variables.md), this lesson explains how quoting controls variable and wildcard expansion.

Quotes are important in Bash because they control how the shell interprets text.

The two most common types are:
* double quotes: `"..."`;
* single quotes: `'...'`.

They look similar, but they behave differently.

## Double quotes

Inside double quotes, variables are expanded.

For example:
```bash
> HOME_DIR="$HOME"
> echo "$HOME_DIR"
/home/m.sega
```
Another example:
```bash
> name="Fabio"
> echo "Hello $name"
Hello Fabio
```
The variable `$name` is replaced by its value.

## Single quotes

Inside single quotes, variables are not expanded.

For example:
```bash
> name="Fabio"
> echo 'Hello $name'
Hello $name
```
Here Bash prints the text exactly as written.

This is useful when we want to prevent Bash from interpreting special characters.

Comparison
```bash
> VAR="$HOME/data"
> echo "$VAR"
/home/m.sega/data
> VAR='$HOME/data'
> echo "$VAR"
$HOME/data
```
In the first case, `$HOME` is expanded when the variable is assigned.

In the second case, `$HOME` is stored literally as text.

> [!NOTE]
> **Why quoting matters**
>
> Suppose we have a directory name containing a space:
> ```bash
> > directory="my data"
> ```
> This command is safe:
> ```bash
> > mkdir "$directory"
> ```
> It creates one directory:
> ```
> my data
> ```
> This command is unsafe:
> ```bash
> > mkdir $directory
> ```
> It is interpreted as:
> ```bash
> > mkdir my data
> ```
> and creates two directories: `my` and `data`

> [!TIP]
> As a general rule, when using variables in Bash commands, prefer: `"$variable"` instead of `$variable`

## Quotes and wildcards

Quotes also affect wildcards.

For example, suppose the current directory contains:
```bash
data.1.dat
data.2.dat
data.3.dat
```
Then:
```bash
> echo *.dat
data.1.dat data.2.dat data.3.dat
```
The wildcard is expanded.

But:
```bash
> echo "*.dat"
*.dat
```
The wildcard is not expanded because it is inside quotes.

This is why, when combining variables and wildcards, we often write:
```bash
> ls "$data_dir"/*.dat
```
and not:
```bash
> ls "$data_dir/*.dat"
```

## Summary

- Double quotes preserve spaces while allowing variable expansion.
- Single quotes preserve text literally, including `$` characters.
- Quoted wildcards are literal; unquoted wildcards can expand to filenames.

## Exercise

Assign `name="Fabio"`, then compare `echo "$name"` with `echo '$name'`.
Next, compare `echo *.md` with `echo "*.md"` in a directory containing Markdown files.
Explain each result before running the command.
