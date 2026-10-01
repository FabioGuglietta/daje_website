# Bash Variables: Storing and Using Values

Shell variables let you reuse values in commands, including filenames and paths.

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

## Variables and paths

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

## Variables and wildcards

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

## Summary

- Assign a value with `name=value`, without spaces around `=`.
- Read it with `$name` or `${name}`.
- Quote variable expansions, as in `"$data_dir"`, to preserve spaces.
- Keep wildcards outside the quotes when you want them to expand.

## Exercise

Create a variable containing a directory name with a space. Use the quoted
variable to create that directory, list it, and remove it once it is empty.
Explain why the unquoted version would behave differently.
