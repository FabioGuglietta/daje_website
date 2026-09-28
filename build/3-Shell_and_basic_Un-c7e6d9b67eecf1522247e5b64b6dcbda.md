# Creating and inspecting files

In this section we introduce some basic commands to inspect and create text files.

We will use the following commands:
* `cat`;
* `more`;
* `less`;
* `head`;
* `tail`;
* `touch`.

## Inspecting files with `cat`

The command `cat` prints the content of a file to the screen.

For example, if a file called `file.txt` contains the text:
```
This is a test
```
then we can inspect it with:
```bash
> cat file.txt
This is a test
```
The name `cat` comes from **concatenate**. The command can also print several files one after another:
```bash
> cat file1.txt file2.txt
```
For very short files, cat is convenient. For long files, it is usually better to use commands such as `more` or `less`, because `cat` prints the whole file at once and the text may scroll too quickly.

## Inspecting files with `more`

The command `more` allows us to inspect a file page by page.
```bash
> more file.txt
```
This is useful when the file is too long to fit on the screen.

Inside `more`, you can usually use:
* `Space` to move forward by one page;
* `Enter` to move down one line;
* `q` to quit.

## Inspecting files with `less`

The command `less` is similar to `more`, but usually more convenient.
```bash
> less file.txt
```
Inside `less`, you can usually use:
* `Space` to move forward by one page;
* `b` to move backward by one page;
* `arrow keys` to move line by line;
* `/word` to search for word;
* `q` to quit.

For this reason, `less` is often preferred over `more`.

## Inspecting the beginning of a file with `head`

The command `head` prints the first lines of a file.
```bash
> head file.txt
```
By default, `head` prints the first 10 lines.

To print only the first 3 lines:
```bash
> head -n 3 file.txt
```
This is useful for quickly checking the structure of a file.

For example, if a data file starts with a header followed by numerical values, `head` lets you inspect the beginning without opening the whole file.

## Inspecting the end of a file with `tail`

The command `tail` prints the last lines of a file.
```bash
> tail file.txt
```
By default, tail prints the last 10 lines.

To print only the last 3 lines:
```bash
> tail -n 3 file.txt
```
The command `tail` is especially useful for checking the end of output or log files.

For example, if a simulation is writing information to output.log, you can inspect the last lines with:
```bash
> tail output.log
```
A particularly useful option is:
```bash
> tail -f output.log
```
The option `-f` means follow.

This command keeps running and prints new lines as they are added to the file. It is useful when a program is running and continuously writing output.

To stop `tail -f`, press:
```
Ctrl + C
```

## Creating an empty file with `touch`

A simple way to create an empty file is the command `touch`.
```bash
> touch empty_file.txt
```
The file `empty_file.txt` now exists, but it contains no text.

We can check that the file exists using ls:
```bash
> ls
empty_file.txt
```
The command `ls -l` shows more information:
```bash
> ls -l empty_file.txt
-rw-r--r--  1 m.sega  users  0 Jun 16 10:30 empty_file.txt
```
The output contains information such as permissions, owner, group, size, modification date, and file name.

In this example, the size of `empty_file.txt` is 0 bytes.

## Creating a file with `cat`

The command `cat` can also be used to create a file when combined with redirection.

If we run cat without giving it a file name, it waits for input from the keyboard.

For example:
```bash
> cat > file.txt
This is a test
^D
>
```
Here:
* `cat` waits for text typed from the keyboard;
* `>` redirects the output into the file `file.txt`;
* `This is a test` is the text we write into the file;
* `Ctrl + D` tells the shell that the input is finished.

> [!NOTE]
> In the terminal, `Ctrl + D` is often displayed as `^D`. After pressing `Ctrl + D`, the prompt appears again.

> [!WARNING]
> The redirection operator `>` overwrites the file if it already exists. For example: `cat > file.txt` will replace the previous content of `file.txt`.

We can now use `cat` again to print the content of `file.txt` to the screen:
```bash
> cat file.txt
This is a test
>
```
In this case, cat receives the name of a file as argument and prints its content.

## Summary
```bash
> cat file.txt        # print the whole file
> more file.txt       # inspect the file page by page
> less file.txt       # inspect the file interactively
> head file.txt       # show the first 10 lines
> head -n 3 file.txt  # show the first 3 lines
> tail file.txt       # show the last 10 lines
> tail -n 3 file.txt  # show the last 3 lines
> tail -f output.log  # follow the file as it changes
> touch file.txt      # create an empty file
```
## Exercise

Create a file with several lines:
```bash
> cat > lines.txt
line 1
line 2
line 3
line 4
line 5
line 6
line 7
line 8
line 9
line 10
line 11
line 12
^D
```
Then try:
```bash
> cat lines.txt
> head lines.txt
> head -n 3 lines.txt
> tail lines.txt
> tail -n 3 lines.txt
> more lines.txt
> less lines.txt
```
Create an empty file:
```bash
> touch empty_file.txt
```
Check that it exists:
```bash
> ls
> ls -l empty_file.txt
```
