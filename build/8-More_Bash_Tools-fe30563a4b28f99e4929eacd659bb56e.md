# Bash: some useful tools

The commands introduced so far are enough to move around the filesystem, inspect files, create directories, copy and remove files, and combine commands using pipes and redirection.

In this optional section, we introduce a few additional Bash tools that become useful when commands start to get longer or when we want to write small scripts.

We will discuss:
* variables;
* environment variables;
* single and double quotes;
* integer and floating-point calculations.

##  Variables

A variable is a **name associated with a value**.

In Bash, variables are useful when we want to store a piece of text, a path, a number, or the result of a command.

For example:
```bash
> base="/home/m.sega/data"
```
This creates a variable called base and assigns it the value:
```
/home/m.sega/data
```
To use the value stored in a variable, we put `$` before its name:
```bash
> echo $base
/home/m.sega/data
```
A more explicit form is:
```bash
> echo ${base}
/home/m.sega/data
```
The form `${base}` is often preferable when the variable is followed by other characters.

For example:
```bash
> filename="data"
> echo "${filename}.txt"
data.txt
```
Without braces, Bash may not understand where the variable name ends.

> [!NOTE]
> When assigning a variable in Bash, there must be no spaces around `=`.
> Correct:
> ```bash
> a=3
> ```
> Wrong:
> ```bash
> a = 3
> ```
> The second form does not assign a variable. Bash interprets it as a command called `a` with arguments `=` and `3`.

### Variables and paths

Variables are often used to **store paths**.

For example:
```bash
> data_dir="/home/m.sega/simulation/data"
> output_dir="/home/m.sega/simulation/output"
```
Then we can use them in commands:
```bash
> ls "$data_dir"
> mkdir "$output_dir"
```
The quotes are important. They **protect the value of the variable** if it contains spaces or special characters.

For example:
```bash
> directory="my data"
> mkdir "$directory"
```
This creates one directory called:
```
my data
```
Without quotes:
```bash
> mkdir $directory
```
Bash would interpret this as:
```bash
> mkdir my data
```
and create two directories:
```bash
my
data
```

### Variables and wildcards

When using variables together with wildcards, quote the variable but not the wildcard.

For example:
```bash
> data_dir="/home/m.sega/data"
> ls "$data_dir"/*.dat
```
Here:
* `"$data_dir"` protects the variable value;
* `*.dat` is left unquoted so that Bash can expand the wildcard.

This is usually better than:
```bash
> ls "$data_dir/*.dat"
```
because in the second command the wildcard is inside quotes and may not be expanded.

Example
```bash
> mkdir simulation
> cd simulation
> data_dir="data"
> backup_dir="backup"
> mkdir "$data_dir"
> mkdir "$backup_dir"
> touch "$data_dir"/data.1.dat
> touch "$data_dir"/data.2.dat
> touch "$data_dir"/data.3.dat
> cp "$data_dir"/*.dat "$backup_dir"/
> ls "$backup_dir"
data.1.dat  data.2.dat  data.3.dat
```
In this example, variables make the commands easier to modify. If we decide to change the name of the data directory, we only need to change the variable assignment.


## Environment variables

Some variables are automatically set by the shell or by the operating system. These are called **environment variables**.

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
Shell variables and environment variables

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

### The `PATH` variable

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

## Single and double quotes

Quotes are important in Bash because they control how the shell interprets text.

The two most common types are:
* double quotes: `"..."`;
* single quotes: `'...'`.

They look similar, but they behave differently.

### Double quotes

Inside double quotes, variables are expanded.

For example:
```bash
> HOME_DIR="$HOME"
> echo "$HOME_DIR"
/home/m.sega
```bash
Another example:
```bash
> name="Fabio"
> echo "Hello $name"
Hello Fabio
```
The variable `$name` is replaced by its value.

### Single quotes

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

### Quotes and wildcards

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

## Integer and floating-point calculations

Bash can perform simple integer arithmetic directly.

For example:
```bash
> a=3
> b=$((a + 1))
> echo "$b"
4
```
The syntax:
```bash
$(( ... ))
```
is used for arithmetic expansion.

### Integer arithmetic

Examples:
```bash
> a=10
> b=3
```
Addition:
```bash
> echo $((a + b))
13
```
Subtraction:
```bash
> echo $((a - b))
7
```
Multiplication:
```bash
> echo $((a * b))
30
```
Integer division:
```bash
> echo $((a / b))
3
```
Remainder:
```bash
> echo $((a % b))
1
```
Bash arithmetic is integer arithmetic. Therefore:
```bash
> echo $((10 / 3))
3
```
not:
```bash
3.333...
```

### Updating a variable

Arithmetic expansion can be used to update variables.

For example:
```bash
> counter=0
> counter=$((counter + 1))
> echo "$counter"
1
```
This is common in scripts.

Another example:
```bash
> nfiles=$(ls -1 | wc -l)
> echo "$nfiles"
```
Here:
* `ls -1` lists one entry per line;
* `wc -l` counts the lines;
* `$(...)` captures the output of the command;
* the result is assigned to the variable nfiles.

### Floating-point calculations

Bash does not handle floating-point arithmetic directly.

For example, this does not work as one might expect:
```bash
> a=3.2
> echo $((a * 2))
```
For floating-point calculations, use an external program.

Common choices are:
* awk;
* bc;
* Python.

#### Floating-point arithmetic with awk

Example:
```bash
> a=3.2
> awk "BEGIN { print $a * 2 }"
6.4
```
Here Bash first expands `$a` into `3.2`, then `awk` performs the calculation.

Another example:
```bash
> x=1.5
> y=2.0
> awk "BEGIN { print $x + $y }"
3.5
```

#### Floating-point arithmetic with bc

The command `bc` is a calculator language.

Example:
```bash
> a=3.2
> echo "$a * 2" | bc -l
6.4
```
Here:
* `echo "$a * 2"` prints the expression;
* the pipe `|` sends it to `bc`;
* the option `-l` loads the math library and enables floating-point behavior.

Another example:
```bash
> echo "10 / 3" | bc -l
3.33333333333333333333
```
Without `-l`, the result may be integer-like depending on the expression and scale.

#### Floating-point arithmetic with Python

Python can also be used from the shell:
```bash
> python3 -c "print(3.2 * 2)"
6.4
```
or using Bash variables:
```bash
> a=3.2
> python3 -c "print($a * 2)"
6.4
```
For more complex numerical calculations, Python is often the most convenient option.

## Summary
```bash
> name="Fabio"                         # create a shell variable
> echo "$name"                         # print the value of the variable
> data_dir="/home/m.sega/data"         # store a path in a variable
> ls "$data_dir"                       # use the variable safely
> ls "$data_dir"/*.dat                 # use a quoted variable with an unquoted wildcard
> echo "$HOME"                         # print the home directory
> echo "$USER"                         # print the username
> echo "$PWD"                          # print the current directory
> env                                  # list environment variables
> printenv                             # list environment variables
> export MYVAR="hello"                 # define and export an environment variable
> export OMP_NUM_THREADS=4             # example: set number of OpenMP threads
> echo "$PATH"                         # print the executable search path
> which ls                             # show where the command ls is located
> echo "Hello $name"                   # variables are expanded inside double quotes
> echo 'Hello $name'                   # variables are not expanded inside single quotes
> a=3                                  # assign an integer value
> b=$((a + 1))                         # integer arithmetic
> echo "$b"                            # print result
> echo $((10 / 3))                     # integer division
> a=3.2                                # assign a decimal value as text
> awk "BEGIN { print $a * 2 }"         # floating-point arithmetic with awk
> echo "$a * 2" | bc -l                # floating-point arithmetic with bc
> python3 -c "print($a * 2)"           # floating-point arithmetic with Python
```
## Exercise

Create a working directory:
```bash
> mkdir bash_tools_practice
> cd bash_tools_practice
```
Create variables:
```bash
> data_dir="data"
> backup_dir="backup"
```
Create directories using those variables:
```bash
> mkdir "$data_dir"
> mkdir "$backup_dir"
```
Create some files:
```bash
> touch "$data_dir"/data.1.dat
> touch "$data_dir"/data.2.dat
> touch "$data_dir"/data.3.dat
```
Use wildcards together with variables:
```bash
> ls "$data_dir"/*.dat
> cp "$data_dir"/*.dat "$backup_dir"/
> ls "$backup_dir"
```
Count files and store the result in a variable:
```bash
> nfiles=$(ls -1 "$backup_dir" | wc -l)
> echo "Number of files in backup: $nfiles"
```
Try integer arithmetic:
```bash
> a=10
> b=3
> echo $((a + b))
> echo $((a / b))
```
Try floating-point arithmetic:
```bash
> x=10
> y=3
> awk "BEGIN { print $x / $y }"
> echo "$x / $y" | bc -l
> python3 -c "print($x / $y)"
```
Clean up:
```bash
> rm "$data_dir"/*.dat
> rm "$backup_dir"/*.dat
> rmdir "$data_dir"
> rmdir "$backup_dir"
> cd ..
> rmdir bash_tools_practice
```
