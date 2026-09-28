# Compiling C Code on Linux/Unix Systems: A Practical Introduction

## Common Compilers in HPC Environments

In High Performance Computing (HPC) environments, you will often find a range of compilers available. Each is optimized for specific hardware, standards, or performance goals:

* **GCC (GNU Compiler Collection)**: Default on most Unix systems. Open source and widely used in research and education.
* **Intel Compiler (icc/icx)**: Optimized for Intel CPUs, offering high-performance vectorization and auto-parallelization.
* **Clang/LLVM**: Fast, modular, and increasingly popular. Known for excellent compiler diagnostics.
* **NVIDIA HPC SDK (nvc/nvc++)**: Targets CPU-GPU hybrid codes and includes OpenACC and CUDA support.


> [!NOTE]
> In a standard linux distribution the gcc compiler is almost always present. In the Mac OS you will have to install the developer tools. 
> If you have access to a computing cluster, you can usually choose which compiler to use by loading the appropriate environment modules, e.g.:
> ```bash
> module load gcc
> module load intel/2024
> module load nvhpc
> ```

## What Does a Compiler Do?

When you compile a C program, you convert human-readable source code into an executable binary through three stages:

1. **Compilation**: Each `.c` file is turned into assembly instructions. The compiler checks syntax, types, and semantics.
2. **Assembly**: The assembly code is converted into machine code and saved in an object file (`.o`), containing low-level binary instructions and placeholders for unresolved symbols.
3. **Linking**: All object files are combined. The linker resolves *symbol references* — determining where each function and global variable is actually located and assigning addresses accordingly.

### Linking in Practice

Suppose `main.c` calls a function `compute()`, but that function is defined in `mathutils.c`. The compiler processes each file independently, so `main.o` will simply say “I need a function called `compute`.” The linker then searches the other object files or libraries to find the definition of `compute`, assigns it a memory address, and replaces the placeholder in `main.o`.

If it cannot find it, you will get an error like:

```
undefined reference to `compute'
```

## A Minimal Hello World

Let's start with the simplest possible example.

**File: `hello.c`**

```c
#include <stdio.h>

void main() {
    printf("Hello, world!\n");
}
```

Compile using the `-o` option to generate an executable name `hello`:

```bash
gcc hello.c -o hello
```
You can then run the executable:

```bash
./hello
```

The standard C library (`libc`) is automatically linked. No additional flags or setup are needed, but you will have to include the appropriate header files (with extension `.h`) to tell the compiler how to invoke the functions. The standard C library provides a large set of general-purpose functions grouped in different header files. These functions are not part of the C language itself but are essential for performing common tasks such as input/output, memory management, string handling, math, and more.

### Examples of Functions from Common Standard Library Headers

#### `<stdio.h>` — Standard Input/Output

Functions for reading/writing files and the terminal.

* `printf()` – print to stdout
* `scanf()` – read from stdin
* `fopen()`, `fclose()` – open/close files
* `fread()`, `fwrite()` – binary file I/O
* `fprintf()`, `fscanf()` – file-specific formatted I/O

#### `<stdlib.h>` — General Utilities

Memory management, random numbers, conversion, program control.

* `malloc()`, `calloc()`, `free()` – dynamic memory allocation
* `exit()`, `abort()` – terminate the program
* `rand()`, `srand()` – pseudo-random number generation
* `atoi()`, `atof()`, `strtol()` – convert strings to numbers
* `qsort()` – generic array sorting
* `bsearch()` – binary search in sorted arrays

#### `<string.h>` — String Handling

String and memory manipulation.

* `strlen()` – compute string length
* `strcpy()`, `strncpy()` – copy strings
* `strcat()`, `strncat()` – concatenate strings
* `strcmp()`, `strncmp()` – compare strings
* `memcpy()`, `memset()` – memory block manipulation

#### `<math.h>` — Mathematical Functions (requires `-lm`)

Basic and advanced math operations.

* `sqrt()` – square root
* `sin()`, `cos()`, `tan()` – trigonometric functions
* `pow()` – power
* `log()`, `log10()` – logarithms
* `fabs()` – absolute value (float)

> Note: To link with math functions from `<math.h>`, you typically need to use `-lm`:

```bash
gcc mymathprog.c -lm -o mymathprog
```

#### `<ctype.h>` — Character Classification

Tests and transformations on characters.

* `isalpha()`, `isdigit()`, `isspace()` – test character properties
* `toupper()`, `tolower()` – change character case

#### `<time.h>` — Time and Date

* `time()` – current time as `time_t`
* `difftime()` – compute difference between times
* `clock()` – processor time used
* `localtime()`, `gmtime()` – convert to calendar time


Now you have all the basic information needed to compile and run simple code written in C.
