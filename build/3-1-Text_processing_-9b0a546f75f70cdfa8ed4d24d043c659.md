# Text Processing in the Shell

Many files used in scientific computing are plain text files: input files, configuration files, log files, tables of numerical data, lists of filenames, and program outputs.

The shell provides several small tools to inspect, filter, transform, and summarize text files. These tools are especially powerful when combined with pipes.

In this lecture we introduce:

- `grep`, to select lines matching a pattern;
- `wc`, to count lines, words, and characters;
- `cut`, to extract columns;
- `sort`, to sort lines;
- `uniq`, to remove or count repeated lines;
- `sed`, to perform simple text substitutions;
- `awk`, to process columns and numerical data.

---

## 1. Preparing an example file

We start by creating a small text file.

```bash
> cat > measurements.dat
# time velocity pressure
0.0  1.2  10.1
0.1  1.4  10.3
0.2  1.5  10.4
0.3  1.7  10.8
0.4  1.6  10.6
^D
```

The file contains three columns:

```text
time velocity pressure
```

We can inspect it with:

```bash
> cat measurements.dat
# time velocity pressure
0.0  1.2  10.1
0.1  1.4  10.3
0.2  1.5  10.4
0.3  1.7  10.8
0.4  1.6  10.6
```

This kind of file is common in scientific computing: each row contains one measurement or one output step, and each column contains one quantity.

---

## 2. Searching lines with `grep`

The command `grep` searches for lines matching a pattern.

The general syntax is:
```bash
grep [options] pattern file
```
where:
* `pattern` is the text or expression to search for;
* `file` is the file where the search is performed;
* `[options]` are optional flags that modify the behavior of grep.

For example:

```bash
> grep velocity measurements.dat
# time velocity pressure
```

This prints all lines containing the word `velocity`.

Another example:

```bash
> grep 0.3 measurements.dat
0.3  1.7  10.8
```

This prints all lines containing the string `0.3`.

### Excluding lines with `grep -v`

The option `-v` means **invert match**. It prints lines that do **not** match the pattern.

For example:

```bash
> grep -v "#" measurements.dat
0.0  1.2  10.1
0.1  1.4  10.3
0.2  1.5  10.4
0.3  1.7  10.8
0.4  1.6  10.6
```

This removes the header line, because the header starts with `#`.

This is useful because data files often contain comments or headers beginning with `#`.

### Counting matching lines with `grep -c`

The option `-c` counts matching lines.

```bash
> grep -c 10 measurements.dat
5
```

This counts how many lines contain the string `10`.

### Showing line numbers with `grep -n`

The option `-n` prints line numbers.

```bash
> grep -n 1.7 measurements.dat
5:0.3  1.7  10.8
```

The result tells us that the matching line is line 5 of the file.

### Useful `grep` examples

```bash
> grep pattern file.txt       # print lines containing pattern
> grep -v pattern file.txt    # print lines not containing pattern
> grep -c pattern file.txt    # count matching lines
> grep -n pattern file.txt    # print matching lines with line numbers
```

---

## 3. Counting text with `wc`

The command `wc` means **word count**.

It counts:

- lines;
- words;
- bytes.

For example:

```bash
> wc measurements.dat
       6      18      98 measurements.dat
```

The output means:

```text
6 lines, 18 words, 98 bytes
```

The exact number of bytes may depend on spaces and line endings.

### Counting only lines

```bash
> wc -l measurements.dat
6 measurements.dat
```

This is often useful for checking the number of rows in a file.

If we want to count only the data rows and exclude the header:

```bash
> grep -v "#" measurements.dat | wc -l
5
```

This pipeline means:

```text
remove header lines → count remaining lines
```

### Counting only words or bytes

```bash
> wc -w measurements.dat
18 measurements.dat
```

```bash
> wc -c measurements.dat
98 measurements.dat
```

Summary:

```bash
> wc file.txt       # count lines, words, and bytes
> wc -l file.txt    # count lines
> wc -w file.txt    # count words
> wc -c file.txt    # count bytes
```

---

## 4. Extracting columns with `cut`

The command `cut` extracts parts of each line.

It is useful when the file has columns separated by a delimiter.

First, create a comma-separated file:

```bash
> cat > particles.csv
id,x,y,z
1,0.1,0.2,0.3
2,0.4,0.5,0.6
3,0.7,0.8,0.9
^D
```

Inspect it:

```bash
> cat particles.csv
id,x,y,z
1,0.1,0.2,0.3
2,0.4,0.5,0.6
3,0.7,0.8,0.9
```

The columns are separated by commas.

To extract the first column:

```bash
> cut -d "," -f 1 particles.csv
id
1
2
3
```

Here:

- `-d ","` means that the delimiter is a comma;
- `-f 1` means “extract field 1”.

To extract the second column:

```bash
> cut -d "," -f 2 particles.csv
x
0.1
0.4
0.7
```

To extract more than one column:

```bash
> cut -d "," -f 2,3 particles.csv
x,y
0.1,0.2
0.4,0.5
0.7,0.8
```

### Removing the header before extracting data

If we want only numerical values, we can remove the header first:

```bash
> grep -v "id" particles.csv | cut -d "," -f 2
0.1
0.4
0.7
```

This pipeline means:

```text
remove header → extract second column
```

### Important limitation of `cut`

`cut` works best when columns are separated by a clear delimiter, such as:

- comma;
- colon;
- tab.

For files where columns are separated by a variable number of spaces, `awk` is usually better.

---

## 5. Sorting lines with `sort`

The command `sort` sorts lines.

Create a file:

```bash
> cat > names.txt
beta
alpha
gamma
alpha
delta
beta
^D
```

Sort it:

```bash
> sort names.txt
alpha
alpha
beta
beta
delta
gamma
```

### Numerical sorting

By default, `sort` sorts text alphabetically.

For numbers, use `-n`.

Create a file:

```bash
> cat > numbers.txt
10
2
30
4
^D
```

Alphabetical sort:

```bash
> sort numbers.txt
10
2
30
4
```

Numerical sort:

```bash
> sort -n numbers.txt
2
4
10
30
```

The option `-n` means **numeric sort**.

### Reverse sorting

Use `-r` for reverse order:

```bash
> sort -nr numbers.txt
30
10
4
2
```

Here:

- `-n` means numeric;
- `-r` means reverse.

---

## 6. Removing repeated lines with `uniq`

The command `uniq` removes adjacent repeated lines.

For example:

```bash
> sort names.txt
alpha
alpha
beta
beta
delta
gamma
```

Now pipe the sorted output into `uniq`:

```bash
> sort names.txt | uniq
alpha
beta
delta
gamma
```

This works because repeated lines become adjacent after sorting.

Without sorting first, `uniq` only removes repetitions that are already adjacent.

### Counting repetitions

The option `-c` counts how many times each line appears:

```bash
> sort names.txt | uniq -c
      2 alpha
      2 beta
      1 delta
      1 gamma
```

This is useful for frequency counts.

For example, suppose a simulation writes a status file:

```bash
> cat > status.log
OK
OK
FAILED
OK
FAILED
WARNING
OK
^D
```

We can count how many times each status appears:

```bash
> sort status.log | uniq -c
      2 FAILED
      4 OK
      1 WARNING
```

This is a common pattern:

```bash
> sort file.txt | uniq -c
```

Meaning:

```text
sort lines → group identical lines → count repetitions
```

---

## 7. Editing text streams with `sed`

The command `sed` is a **stream editor**.

It reads text line by line, applies editing commands, and prints the result.

The most common use of `sed` is substitution.

### Basic substitution

Create a file:

```bash
> cat > input.txt
The velocity is small.
The velocity is increasing.
The pressure is constant.
^D
```

Replace the first occurrence of `velocity` with `speed` on each line:

```bash
> sed 's/velocity/speed/' input.txt
The speed is small.
The speed is increasing.
The pressure is constant.
```

The syntax is:

```text
s/old/new/
```

where `s` means **substitute**.

### Replacing all occurrences on a line

Create another file:

```bash
> cat > repeated.txt
velocity velocity pressure
velocity pressure velocity
^D
```

Without `g`, only the first occurrence on each line is replaced:

```bash
> sed 's/velocity/speed/' repeated.txt
speed velocity pressure
speed pressure velocity
```

With `g`, all occurrences are replaced:

```bash
> sed 's/velocity/speed/g' repeated.txt
speed speed pressure
speed pressure speed
```

Here `g` means **global** on each line.

### Saving the result to another file

By default, `sed` prints the modified text to the screen. It does not change the original file.

To save the result:

```bash
> sed 's/velocity/speed/g' repeated.txt > modified.txt
```

Inspect the new file:

```bash
> cat modified.txt
speed speed pressure
speed pressure speed
```

### Editing a file in place

Some versions of `sed` support the option `-i`, which modifies the file directly.

On Linux:

```bash
> sed -i 's/velocity/speed/g' repeated.txt
```

On macOS, the syntax is often:

```bash
> sed -i '' 's/velocity/speed/g' repeated.txt
```

> [!WARNING]
> Be careful with `sed -i`.
>
> It modifies the file directly. If the command is wrong, the file may be changed in an unintended way.
>
> A safer approach is to first write to a new file:
>
> ```bash
> sed 's/old/new/g' file.txt > new_file.txt
> ```

### Deleting lines with `sed`

To delete lines containing a pattern:

```bash
> sed '/pressure/d' input.txt
The velocity is small.
The velocity is increasing.
```

Here:

```text
/pattern/d
```

means:

```text
delete lines matching pattern
```

### Printing selected lines with `sed -n`

To print only lines 2 to 3:

```bash
> sed -n '2,3p' input.txt
The velocity is increasing.
The pressure is constant.
```

Here:

- `-n` suppresses automatic printing;
- `2,3p` prints lines from 2 to 3.

---

## 8. Processing columns with `awk`

The command `awk` is a small programming language designed for processing text files, especially files organized in columns.

It reads input line by line. Each line is split into fields.

By default, fields are separated by spaces or tabs.

In `awk`:

- `$1` is the first column;
- `$2` is the second column;
- `$3` is the third column;
- `$0` is the whole line.

Return to the file:

```bash
> cat measurements.dat
# time velocity pressure
0.0  1.2  10.1
0.1  1.4  10.3
0.2  1.5  10.4
0.3  1.7  10.8
0.4  1.6  10.6
```

### Printing one column

To print the first column:

```bash
> awk '{print $1}' measurements.dat
#
0.0
0.1
0.2
0.3
0.4
```

To print the second column:

```bash
> awk '{print $2}' measurements.dat
time
1.2
1.4
1.5
1.7
1.6
```

The header line is included. To remove it:

```bash
> grep -v "#" measurements.dat | awk '{print $2}'
1.2
1.4
1.5
1.7
1.6
```

### Printing multiple columns

```bash
> grep -v "#" measurements.dat | awk '{print $1, $2}'
0.0 1.2
0.1 1.4
0.2 1.5
0.3 1.7
0.4 1.6
```

### Performing calculations

`awk` can perform calculations on columns.

For example, compute:

```text
velocity × pressure
```

for each row:

```bash
> grep -v "#" measurements.dat | awk '{print $1, $2*$3}'
0.0 12.12
0.1 14.42
0.2 15.6
0.3 18.36
0.4 16.96
```

Here:

- `$1` is time;
- `$2` is velocity;
- `$3` is pressure;
- `$2*$3` is their product.

### Selecting rows with a condition

Print only rows where the velocity is larger than `1.5`:

```bash
> grep -v "#" measurements.dat | awk '$2 > 1.5 {print $0}'
0.3  1.7  10.8
0.4  1.6  10.6
```

Here:

```text
$2 > 1.5
```

is the condition.

```text
{print $0}
```

is the action.

### Computing an average

Compute the average velocity:

```bash
> grep -v "#" measurements.dat | awk '{sum += $2; n += 1} END {print sum/n}'
1.48
```

This means:

- for each line, add the second column to `sum`;
- increase the counter `n`;
- at the end, print `sum/n`.

This is one of the most useful patterns in `awk`.

### Using a comma-separated file

For comma-separated files, use the option `-F`.

Recall:

```bash
> cat particles.csv
id,x,y,z
1,0.1,0.2,0.3
2,0.4,0.5,0.6
3,0.7,0.8,0.9
```

To tell `awk` that the separator is a comma:

```bash
> awk -F "," '{print $2}' particles.csv
x
0.1
0.4
0.7
```

Remove the header and print the second column:

```bash
> grep -v "id" particles.csv | awk -F "," '{print $2}'
0.1
0.4
0.7
```

---

## 9. Combining tools with pipes

The real power of shell text processing comes from combining simple commands.

For example:

```bash
> grep -v "#" measurements.dat | awk '$2 > 1.5 {print $1, $2}' | sort -n
0.3 1.7
0.4 1.6
```

This means:

```text
remove header → select rows with velocity > 1.5 → print time and velocity → sort numerically
```

Another example:

```bash
> grep -v "#" measurements.dat | awk '{print $2}' | sort -n
1.2
1.4
1.5
1.6
1.7
```

This means:

```text
remove header → extract velocity column → sort numerically
```

Save the result to a file:

```bash
> grep -v "#" measurements.dat | awk '{print $2}' | sort -n > velocities_sorted.txt
```

Inspect the result:

```bash
> cat velocities_sorted.txt
1.2
1.4
1.5
1.6
1.7
```

---

## 10. Summary

```bash
> grep pattern file.txt              # print lines containing pattern
> grep -v pattern file.txt           # print lines not containing pattern
> grep -c pattern file.txt           # count matching lines
> grep -n pattern file.txt           # print matching lines with line numbers

> wc file.txt                        # count lines, words, and bytes
> wc -l file.txt                     # count lines
> wc -w file.txt                     # count words
> wc -c file.txt                     # count bytes

> cut -d "," -f 1 file.csv           # extract first comma-separated column
> cut -d "," -f 2,3 file.csv         # extract second and third columns

> sort file.txt                      # sort lines alphabetically
> sort -n file.txt                   # sort lines numerically
> sort -r file.txt                   # sort lines in reverse order
> sort -nr file.txt                  # sort numerically in reverse order

> uniq file.txt                      # remove adjacent repeated lines
> sort file.txt | uniq               # remove repeated lines after sorting
> sort file.txt | uniq -c            # count repeated lines

> sed 's/old/new/' file.txt          # replace first occurrence on each line
> sed 's/old/new/g' file.txt         # replace all occurrences on each line
> sed '/pattern/d' file.txt          # delete lines containing pattern
> sed -n '2,5p' file.txt             # print lines 2 to 5

> awk '{print $1}' file.txt          # print first column
> awk '{print $1, $3}' file.txt      # print first and third columns
> awk '$2 > 1.5 {print $0}' file.txt # print rows where second column > 1.5
> awk '{sum += $2; n += 1} END {print sum/n}' file.txt # average second column

> command1 | command2                # pipe output of command1 into command2
> command > output.txt               # redirect output to a file
```

---

## 11. Exercise

Create the file:

```bash
> cat > simulation.log
step 0 status OK energy 10.5
step 1 status OK energy 10.2
step 2 status WARNING energy 11.8
step 3 status OK energy 10.1
step 4 status FAILED energy 15.9
step 5 status OK energy 9.9
^D
```

Try the following commands:

```bash
> cat simulation.log
> grep OK simulation.log
> grep WARNING simulation.log
> grep FAILED simulation.log
> grep -c OK simulation.log
> wc -l simulation.log
> awk '{print $2, $6}' simulation.log
> awk '$6 > 11.0 {print $0}' simulation.log
> awk '{sum += $6; n += 1} END {print sum/n}' simulation.log
> sed 's/energy/E/' simulation.log
> sed 's/status/state/' simulation.log
> awk '{print $4}' simulation.log | sort | uniq -c
```

The last command:

```bash
> awk '{print $4}' simulation.log | sort | uniq -c
```

counts how many times each status appears.

It means:

```text
extract status column → sort values → count repetitions
```
