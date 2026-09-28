# Compiling C Code on Linux/Unix Systems: A Practical Introduction

## Common Compilers in HPC Environments

A **compiler** is a software tool that translates source code written in a high-level programming language, such as C, C++, or Fortran, into machine code or into a lower-level form that can eventually be executed by the CPU.

In High Performance Computing (HPC) environments, you will often find several compilers available. Each compiler may be optimized for specific hardware, standards, or performance goals.

* **GCC (GNU Compiler Collection)**: Default on most Unix systems. Open source and widely used in research and education.
* **Intel Compiler (`icc`/`icx`)**: Optimized for Intel CPUs, offering high-performance vectorization and auto-parallelization.
* **Clang/LLVM**: Fast, modular, and increasingly popular. Known for excellent compiler diagnostics.
* **NVIDIA HPC SDK (`nvc`/`nvc++`)**: Targets CPU-GPU hybrid codes and includes OpenACC and CUDA support.

> [!NOTE]
> In a standard Linux distribution, the `gcc` compiler is almost always present. On macOS, you usually need to install the developer tools.
>
> If you have access to a computing cluster, you can usually choose which compiler to use by loading the appropriate environment module, for example:
>
> ```bash
> module load gcc
> module load intel/2024
> module load nvhpc
> ```

## What Does a Compiler Do?

When you compile a C program, you convert human-readable source code into an executable binary.

For compiled languages such as C and Fortran, the typical process is:

```text
source code → compiler → object file → linker → executable
```

The **source code** is the program written by the programmer.

The **compiler** translates the source code into a lower-level representation, often producing an **object file**.

The **linker** combines one or more object files, together with the required libraries, into the final **executable program**.

More explicitly, the process can be divided into three main stages.

### 1. Compilation

Each `.c` file is translated into lower-level instructions, often assembly instructions. During this stage, the compiler checks that the code is valid.

This includes:

- **lexical analysis**, where the source code is broken into basic elements called tokens;
- **syntax analysis**, where the compiler checks that the code follows the grammar of the C language;
- **semantic analysis**, where the compiler checks that the code is meaningful, for example that variables and functions are used consistently;
- **optimization**, where the compiler may improve the performance of the generated code;
- **code generation**, where the compiler produces assembly code or machine-level code.

For this course, the most important idea is:

```text
the compiler translates human-readable code into instructions that the machine can execute
```

### 2. Assembly

The assembly code is converted into machine code and saved in an **object file**, usually with extension `.o`.

An object file contains low-level binary instructions, but it is usually not yet a complete program. It may still contain placeholders for functions or variables defined somewhere else.

### 3. Linking

The linker combines all object files and required libraries into a final executable.

For example, suppose `main.c` calls a function `compute()`, but that function is defined in another file called `mathutils.c`.

The compiler processes each file independently. Therefore, when compiling `main.c`, it does not yet know where `compute()` is actually defined. The object file `main.o` only contains a placeholder saying:

```text
I need a function called compute
```

The linker then searches the other object files or libraries, finds the definition of `compute()`, assigns it a memory address, and replaces the placeholder with the correct reference.

If the linker cannot find the function definition, you may get an error such as:

```text
undefined reference to `compute'
```

This is a **linking error**, not a syntax error.

## A Minimal Hello World

Let us start with the simplest possible example.

**File: `hello.c`**

```c
#include <stdio.h>

int main() {
    printf("Hello, world!\n");
    return 0;
}
```

We can compile it using `gcc`:

```bash
gcc hello.c -o hello
```

This command tells `gcc` to take the source file `hello.c` and produce an executable file called `hello`.

The program can then be executed with:

```bash
./hello
```

The output is:

```text
Hello, world!
```

The option `-o hello` specifies the name of the output executable. Without this option, `gcc` usually produces an executable called `a.out`.

## Header Files and Libraries

The program above uses the function `printf()`.

This function is not defined by us. It is provided by the **standard C library**, which contains many general-purpose functions for input/output, memory management, string handling, mathematical operations, and more.

To use `printf()`, we include the header file:

```c
#include <stdio.h>
```

The header file tells the compiler how the function `printf()` should be called.

The actual compiled implementation of `printf()` is provided by the standard C library, usually called `libc`.

For this simple program, the standard C library is linked automatically. No additional flags are needed.

Therefore, this line:

```c
#include <stdio.h>
```

does not copy the whole library into our program. It only gives the compiler the information needed to check that we are calling `printf()` correctly.

The linker later connects our program to the actual implementation of `printf()`.

## Error Detection

Compilers help detect errors before the program is executed.

For example, the following code contains a syntax error:

```c
#include <stdio.h>

int main() {
    printf("Hello, world!\n")
    return 0;
}
```

The semicolon after `printf()` is missing.

If we try to compile this program, the compiler stops and prints an error message. The program is not executed because no valid executable has been produced.

This is important: before a program can run, the compiler must first be able to understand it.

## Optimization

Modern compilers can also optimize the generated machine code.

For example, they may:

- reduce the number of instructions;
- remove unnecessary calculations;
- improve memory access patterns;
- reorganize computations to make the program faster;
- use specific CPU instructions when available.

Optimization is especially important in scientific computing, where simulations may require millions or billions of operations.

Compiler optimizations are usually enabled with options such as:

```bash
gcc -O2 program.c -o program
```

or:

```bash
gcc -O3 program.c -o program
```

The option `-O2` enables a standard set of optimizations. The option `-O3` enables more aggressive optimizations.

At the beginning of the course, we will mostly compile without optimization or with simple optimization flags. Later, optimization will become more important when we discuss performance.


## Examples of Functions from Common Standard Library Headers

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
