# The language of the CPU

> [!NOTE]
> A CPU does not execute C, Fortran, or Python directly. It executes very simple instructions such as “copy this value”, “add these two numbers”, “compare two values”, or “jump to another instruction.”

## Machine language

At the heart of every computer system lies machine language, the most basic form of code that the CPU can execute directly. Machine language consists of binary digits, that is, sequences of 0s and 1s. These binary instructions directly control the operations performed by the CPU.

Writing programs directly in machine language is extremely difficult for humans. The problem is not only that machine language is written in binary form. The real difficulty is that the programmer must manually specify every low-level operation: which instruction to execute, which registers to use, where data are stored, and how the program moves from one instruction to the next.

### Memory and registers

To understand even a very simple machine instruction, we need two basic ideas: memory and registers.

Memory stores the data used by a program. It can be imagined as a long sequence of cells, each identified by an address. Variables in high-level languages, such as C or Fortran, correspond to data stored somewhere in memory.

**A register is a very small and very fast storage location inside the CPU**. Registers are used to temporarily hold values while the CPU executes instructions.

For example, consider the following C instruction:
```C
x = x + 10; 
```
At the CPU level, this operation may correspond conceptually to something like:
```assembly
MOV AX, x     ; copy the value of x into the AX register 
ADD AX, 10    ; add 10 to the value stored in AX 
MOV x, AX     ; copy the result back into memory 
```
This is not meant to be exact assembly code. The important idea is the following: memory → register → operation → memory 

The CPU often loads values from memory into registers, performs simple operations on registers, and stores the results back in memory.

### A simple machine-language example

An example of machine language might look like this:
```text
10110000 01100001 
```
On an x86 processor, this binary sequence can represent an instruction similar to:
```assembly
MOV AL, 97 
```
This means:
```text
Move the value 97 into the register AL. 
```
Here:

- 10110000 encodes the instruction `MOV AL`, meaning “move a value into the register AL”;
- 01100001 is the binary representation of the decimal number 97.

The value 97 also corresponds to the ASCII code of the character 'a', but in this example the CPU simply treats it as a numerical value.

**Machine language is architecture-specific.** This means that the binary instructions depend on the type of processor being used. For example, a machine-code instruction for an Intel/x86 processor is generally different from one for an ARM processor, even if both processors are performing a similar operation.

A complete program in machine language consists of many such binary instructions. Even a simple program may require hundreds or thousands of low-level instructions.

## Assembly language

To make programming more manageable, assembly language was developed. Assembly language uses mnemonic codes and symbols to represent machine-language instructions. It is a **more human-readable** form of the underlying binary code.

For example, instead of writing binary instructions directly, one can write:

```assembly
MOV AX, 5    ; Move the value 5 into the AX register
ADD AX, 10   ; Add the value 10 to the AX register 
```
Here:
- `MOV AX`, 5 moves the value 5 into the register `AX`;
- `ADD AX`, 10 adds the value 10 to the value already stored in `AX`.

After these two instructions, the register `AX` contains the value 15.

Assembly language is much easier to read than raw machine code, but it is still very close to the hardware. The programmer still needs to know the architecture of the processor, the available registers, and the meaning of the instructions.

An **assembler** is a program that **translates assembly code into machine code** that the CPU can execute.

In simple cases, one assembly instruction corresponds to one machine-language instruction. However, the exact relationship depends on the processor architecture and on the assembler being used.

## High-level languages

As computing evolved, programmers needed languages that were easier to read, write, and maintain. This led to the development of high-level languages, such as C, C++, Fortran, and many others.

High-level languages allow programmers to write code using variables, functions, loops, conditions, and mathematical expressions.

For example, in C one can write:
```c
int x = 5; x = x + 10; 
```
This is much easier to understand than the corresponding machine-language or assembly instructions. The programmer can focus on the algorithm instead of manually controlling registers and machine instructions.

For computational physics, this is essential. We usually want to start from a mathematical model, transform it into a numerical algorithm, and then implement that algorithm in a programming language.

A typical path is:
```text
physical problem → mathematical model → numerical method → algorithm → program 
```
However, the CPU still cannot execute high-level code directly. The code must first be **translated into machine code**.

This is the role of a compiler.

# Compilers

A compiler is a **software tool** that translates source code written in a high-level programming language, such as C, C++, or Fortran, into machine code or into a lower-level form that can eventually be executed by the CPU.

For compiled languages such as C and Fortran, the typical process is:
```text
source code → compiler → object file → linker → executable 
```
The **source code** is the program written by the programmer.

The **compiler** translates the source code into a lower-level representation, often producing an **object file**.

The **linker** combines one or more object files, together with the **required libraries**, into the **final executable program**.

For example, suppose we write the following C program:
```c
#include <stdio.h>
int main(){
	printf("Hello, world!\n");
	return 0;
} 
```
We can compile it using gcc:
```bash
gcc hello.c -o hello 
```
This command tells the compiler to take the source file `hello.c` and produce an executable file called `hello`.

The program can then be executed with:
```bash
./hello 
```
The output is:
```text
Hello, world! 
```

## Key Features of Compilers

**Translation**: The compiler’s primary role is to translate the source code into machine language. This is done in stages:

- Lexical Analysis: Breaking the source code into tokens.
- Syntax Analysis: Ensuring the code follows the grammar of the language.
- Semantic Analysis: Checking for meaningful and logical consistency.
- Optimization: Improving the performance and efficiency of the code (for instance, they may reduce the number of instructions, remove unnecessary calculations, or improve memory access patterns).
- Code Generation: Producing machine-level code or assembly code.

**Error Detection**: Compilers help detect syntax errors and other issues in the source code before execution. They provide detailed error messages that help developers find and fix problems.

