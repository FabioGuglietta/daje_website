# File Operations and Wildcards
## Creating, copying, moving, and deleting files
In this section we introduce some common commands to create, copy, move, rename, and delete files and directories.
We will use:
- `mkdir`;
- `cp`;
- `mv`;
- `rm`;
- `rmdir`.
  
> [!WARNING]
> Unix commands such as `rm` usually do not move files to a trash folder.
>
> If you remove a file with `rm`, it is normally deleted immediately.


### Creating directories with `mkdir`
The command `mkdir` creates a new directory.
For example:
```bash
> mkdir test_directory
```
We can check that the directory was created with:
```bash
> ls
test_directory
```
The name `mkdir` means **make directory**.

### Copying files with `cp`

The command `cp` copies files.

Suppose we have a file called `file.txt`.
```bash
> ls
file.txt
```
We can create a copy called `copy.txt` with:
```bash
> cp file.txt copy.txt
```
Now both files exist:
```bash
> ls
copy.txt  file.txt
```
The general syntax is:
```bash
cp source destination
```
Here:
* `source` is the file we want to copy;
* `destination` is the name or location of the copy.

We can also copy a file into a directory.

For example:
```bash
> mkdir backup
> cp file.txt backup/
```
The file `file.txt` is now copied inside the directory backup.

We can check this with:
```bash
> ls backup
file.txt
```

### Moving and renaming files with `mv`

The command `mv` moves files or directories.

For example, suppose we have:
```bash
> ls
backup  file.txt
```
We can move `file.txt` into the directory backup with:
```bash
> mv file.txt backup/
```
Now `file.txt` is no longer in the current directory:
```bash
> ls
backup
```
but it is inside backup:
```bash
> ls backup
file.txt
```
The command `mv` is also used to rename files.

For example:
```bash
> mv backup/file.txt backup/data.txt
```
This renames `file.txt` to `data.txt`.

We can check this with:
```bash
> ls backup
data.txt
```
The general syntax is:
```bash
mv source destination
```
If destination is an existing directory, the source is moved into that directory.

If destination is a file name, the source is renamed.

### Removing files with `rm`

The command `rm` removes files.

For example:
```bash
> rm copy.txt
```
This deletes the file `copy.txt`.

> [!WARNING]
> Be careful with `rm`.
> In most Unix-like systems, files removed with `rm` are not moved to a trash folder. They are deleted directly.

### Removing directories and their contents with `rm -r`

To remove a directory together with all its contents, use:
```bash
> rm -r directory_name
```
The option `-r` means recursive: the command descends into the directory and removes its contents.

Sometimes you may see:
```bash
> rm -rf directory_name
```
Here:
* `-r` means recursive;
* `-f` means force.

> [!WARNING]
> Be extremely careful with `rm -rf`.
> It removes files and directories recursively and **does not ask for confirmation!**
> 
> **A wrong path can delete much more than intended.**

### Summary of common commands
```bash
> mkdir <dir>         # create the directory <dir>
> rmdir <dir>         # remove the directory <dir>, if empty
> cp <source> <dest>  # copy file <source> to <dest>
> mv <source> <dest>  # move or rename file/directory <source> to <dest>
> rm <file>           # remove the file <file>
> rm -r <dir>         # remove directory <dir> and its contents recursively
> rm -rf <dir>        # same as above, but force removal
```
### Exercise

Create a small directory structure and practice copying, moving, renaming, and deleting files.

For example:
```bash
> mkdir test
> touch test/file.txt
> cp test/file.txt test/copy.txt
> mv test/copy.txt test/renamed.txt
> ls test
> rm test/file.txt
> rm test/renamed.txt
> rmdir test
```


## Wildcards and globbing
When working in the shell, we often want to operate on **several files at once.**

For example, suppose we have many data files:
```text
data.1.dat
data.2.dat
data.3.dat
data.4.dat
```
Instead of writing each file name explicitly, we can use **wildcards**.

A wildcard is a special character that the **shell expands into matching file names**. This mechanism is called **globbing**.

### The `*` wildcard

The asterisk `*` matches **any sequence of characters**, including no characters.

For example, suppose the current directory contains:
```
data.1.dat
data.2.dat
data.3.dat
notes.txt
```
Then the command:
```bash
> ls data.*.dat
```
matches:
```
data.1.dat
data.2.dat
data.3.dat
```
The pattern:
```
data.*.dat
```
means:
```
data. + anything + .dat
```
So it matches file names that start with `data.`, end with `.dat`, and have **any sequence of characters** in between.

For example, to move all these files into a subdirectory:
```bash
> mkdir subdir
> mv data.*.dat subdir/
```

### The `?` wildcard

The question mark `?` matches **exactly one character**.

For example, suppose the current directory contains:
```
data.1.dat
data.2.dat
data.10.dat
data.A.dat
```
Then:
```bash
> ls data.?.dat
```
matches:
```
data.1.dat
data.2.dat
data.A.dat
```
but not:
```
data.10.dat
```
because 10 contains two characters, while `?` matches only one.

On the other hans, the command
```bash
> ls data.??.dat
```
matches only
```
data.10.dat
```

### Globbing is done by the shell

It is important to understand that wildcard expansion is performed by the shell before the command is executed.

For example, if the current directory contains:
```
data.1.dat
data.2.dat
data.3.dat
```
then the command:
```bash
> ls data.*.dat
```
is expanded by the shell into:
```
> ls data.1.dat data.2.dat data.3.dat
```
The program `ls` receives the expanded list of file names.

### Checking a pattern before using it

Before using a wildcard with commands such as `rm` or `mv`, it is a good idea to **check what the pattern matches**.

For example, instead of immediately running:
```bash
> rm data.*.dat
```
first run:
```bash
> ls data.*.dat
```
or:
```bash
> echo data.*.dat
```
This lets you see which files will be affected.

> [!WARNING]
> Be careful when using wildcards with destructive commands such as rm.
>
> For example:
> ```bash
> rm *.dat
> ```
> removes all files ending in `.dat` in the current directory.

### Examples

Move all `.dat` files into a directory called `data_files`:
```bash
> mkdir data_files
> mv *.dat data_files/
```
Copy all files whose names start with `run_` into a directory called `backup`:
```bash
> mkdir backup
> cp run_* backup/
```

### Exercise

Create some test files:
```bash
> touch data.1.dat data.2.dat data.10.dat notes.txt test.dat
```
Then try:
```bash
> ls *.dat
> ls data.*.dat
> ls data.?.dat
> ls data*
> echo *.txt
```
Before removing the test files, check the pattern:
```bash
> ls *.dat
```
Then remove them:
```bash
> rm *.dat
```



