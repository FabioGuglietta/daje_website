# The Shell and Basic Unix Commands: Redirection and pipes

One of the most powerful features of the shell is the ability to **connect commands together**.

A command usually receives input, performs some operation, and produces output.

In Unix-like systems, there are three standard streams:
- `stdin` (standard input) usually comes from the keyboard;
- `stdout` (standard output) usually goes to the terminal;
- `stderr` (standard error) also goes to the terminal.

Redirection and pipes allow us to change this behavior.

## Redirecting output with `>`
The symbol `>` redirects the output of a command into a file.
For example:
```bash
> echo "Hello world"
Hello world
```
prints text to the terminal.

Instead, we can redirect the output to a file:
```
> echo "Hello world" > message.txt
```
Now the output is not printed to the terminal. It is written into the file `message.txt`.

We can check the content of the file with `cat`:
```bash
> cat message.txt
Hello world
```

> [!WARNING]
> The redirection operator `>` overwrites the file if it already exists.

## Appending output with `>>`

The symbol `>>` redirects output to a file, but appends it at the end instead of overwriting the file.

For example:
```bash
> echo "First line" > lines.txt
> echo "Second line" >> lines.txt
> echo "Third line" >> lines.txt
```
Now:
```bash
> cat lines.txt
First line
Second line
Third line
```
The first command uses `>` and creates the file. The following commands use `>>` and add new lines at the end.

## Redirecting input with `<`

The symbol `<` redirects input from a file.

For example, suppose we have a file called `lines.txt`:
```bash
> cat lines.txt
First line
Second line
Third line
```
The command `wc` counts lines, words, and characters:
```bash
> wc lines.txt
       3       6      34 lines.txt
```
We can also give the file as standard input using `<`:
```bash
> wc < lines.txt
       3       6      34
```
The numerical result is the same, but the output does not include the file name, because `wc` receives the content from standard input rather than being given the file name directly.

Counting lines, words, and characters with `wc`

The command `wc` means word count.

It prints three numbers:
```
number_of_lines number_of_words number_of_characters
```
For example:
```bash
> echo "When shall we three meet again?" > witch.txt
> wc witch.txt
       1       6      33 witch.txt
```
Here:
* 1 is the number of lines;
* 6 is the number of words;
* 33 is the number of characters;
* `witch.txt` is the file name.

Useful options are:
```bash
> wc -l witch.txt   # count lines
> wc -w witch.txt   # count words
> wc -c witch.txt   # count bytes/characters
```

## Pipes

A pipe sends the output of one command directly into the input of another command.

The pipe symbol is: `|`

For example:
```bash
> ls
Applications  Documents
```
The command `ls` prints the content of the current directory.

We can send this output to `wc`:
```bash
> ls | wc
       2       2      24
```
This means:
```
run ls, then send its output to wc
```
Conceptually:
```
ls → wc
```
The command `ls` produces `output`. The command `wc` receives that `output` as `input` and counts it.

### Example: counting files with `ls -1 | wc -l`

To count how many entries are in a directory, it is better to use:
```bash
> ls -1 | wc -l
```
The option `-1` tells `ls` to print one entry per line.

For example:
```bash
> ls -1
Applications
Documents
notes.txt
> ls -1 | wc -l
3
```
Here:
* `ls -1` prints one file or directory per line;
* `wc -l` counts the number of lines.

Therefore, the result is the number of entries printed by `ls -1`.

## Filtering output with `grep`

The command `grep` searches for lines **matching a pattern**.

For example, suppose the current directory contains:
```
data1
data2
data3
trash1
trash2
```
We can list the entries one per line:
```bash
> ls -1
data1
data2
data3
trash1
trash2
```
Now we can select only the lines containing data:
```bash
> ls -1 | grep data
data1
data2
data3
```
We can then count them:
```bash
> ls -1 | grep data | wc -l
3
```
This pipeline means:
```
list files → keep only lines containing data → count the lines
```
Conceptually:
```
ls -1 → grep data → wc -l
```
This is one of the central ideas of the Unix shell: **simple commands can be combined to perform more useful operations**.

## Redirecting the result of a pipeline

The output of a pipeline can also be redirected to a file.

For example:
```bash
> ls -1 | grep data > data_files.txt
```
This writes the list of matching files into `data_files.txt`.

We can inspect it with:
```bash
> cat data_files.txt
data1
data2
data3
```
Similarly:
```bash
> ls -1 | grep data | wc -l > count.txt
```
writes the number of matching files into count.txt.

## Summary
```bash
> command > file.txt        # redirect output to file, overwriting it
> command >> file.txt       # redirect output to file, appending to it
> command < file.txt        # read input from file
> wc file.txt               # count lines, words, and characters
> wc -l file.txt            # count lines
> wc -w file.txt            # count words
> wc -c file.txt            # count bytes/characters
> command1 | command2       # send output of command1 to command2
> ls -1 | wc -l             # count entries in current directory
> ls -1 | grep data         # list entries containing data
> ls -1 | grep data | wc -l # count entries containing data
```

## Exercise

Create a few files:
```bash
> touch data1 data2 data3 trash1 trash2
```
List them:
```bash
> ls -1
```
Select only the files containing data:
```bash
> ls -1 | grep data
```
Count them:
```bash
> ls -1 | grep data | wc -l
```
Save the result to a file:
```bash
> ls -1 | grep data > data_files.txt
```
Inspect the file:
```bash
> cat data_files.txt
```
Count the matching files and save the number:
```bash
> ls -1 | grep data | wc -l > count.txt
```
Inspect the result:
```bash
> cat count.txt
```
