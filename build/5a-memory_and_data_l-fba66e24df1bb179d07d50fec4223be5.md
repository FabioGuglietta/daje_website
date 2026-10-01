# Memory: RAM, Registers, Cache, and Data Layout

A simulation repeatedly reads numbers, performs operations, and stores results.
For example, updating a particle's position requires its current position and
velocity. A fast CPU helps only if those values reach it quickly enough.

This lesson introduces two practical questions: **how much memory do the data
need, and how efficiently can the CPU access them?** The diagrams and examples
use arrays of numbers without requiring C or Fortran syntax.

## RAM: the workspace of a calculation

**RAM** (Random Access Memory) holds the instructions and data currently needed
by running programs. Unlike a saved file on an SSD or disk, ordinary RAM is
volatile: its contents are lost when power is removed.

“Random access” means that locations can be addressed directly; it does not
mean that a program should access data in random order.

Memory capacity is measured in bytes: one byte contains eight bits. If one
floating-point value occupies eight bytes, an array of one million values
needs eight million bytes, or **8 MB** in decimal units. Ten such arrays need
80 MB, before accounting for the rest of the program and temporary storage.

RAM capacity determines whether the working data fit. It does not, by itself,
tell us how quickly the calculation will run.

## Registers and cache: keeping data close to the CPU

Memory is organized into a hierarchy:

```text
CPU operations
      ↕
  Registers       very small working storage inside a CPU core
      ↕
  Cache           small, fast storage, typically in levels L1, L2, L3
      ↕
  RAM             larger working storage
```

**Registers** hold values directly involved in CPU instructions: for example,
the operands of an addition and its result. Individual registers may hold
one value or several packed values, depending on the instruction. The total
register space is small, so a large simulation array cannot live entirely there.

**Caches** retain copies of memory data and instructions close to the CPU.
When a requested value is already in cache, access is a **cache hit**. When
it is absent, a **cache miss** requires fetching it from a lower level of
the hierarchy. Hardware manages ordinary CPU caches automatically.

L1 is generally the smallest and fastest cache; later levels offer more
capacity at higher access latency. Exact sizes, sharing between cores, and
access times depend on the processor.

Think of the cooking example in the slides: RAM is the refrigerator, cache
is the work surface, and registers are what you can work with immediately.
Repeatedly returning to the refrigerator is slower than reusing ingredients
already on the work surface. This is an analogy for data reuse, not a literal
model of how the hardware moves data.

Two properties matter:

- **Latency**: how long you wait for a requested piece of data.
- **Bandwidth**: how much data can be transferred per second.

A calculation doing little arithmetic per array element can spend much of its
time moving data rather than calculating.

## Memory addresses: where a value is stored

For an ordinary program, memory can be viewed as a sequence of addressable
bytes. An **address** is a number identifying a location. A value occupying
eight bytes spans eight consecutive byte addresses.

Do not confuse a variable's name, its address, and its value:

| Concept | Example |
| --- | --- |
| Name used in the program | `temperature` |
| Address of its first byte | `0x1000` |
| Value stored there | `1.5` |

Addresses are often written in **hexadecimal**, or base 16. The digits are
`0` to `9`, then `A` to `F` for ten to fifteen; `0x` marks the notation.
For example, `0x10` means sixteen in decimal. Each hexadecimal digit represents
four bits. Hexadecimal is a compact way to write numbers, not a different kind
of memory.

Programs normally use **virtual addresses**, which the operating system and
hardware map to physical memory. Consecutive virtual addresses need not occupy
consecutive physical RAM locations across page boundaries. In the examples
below, “contiguous” describes the addresses visible to the program.

## Contiguous arrays

An **array** stores a sequence of elements. For a contiguous array of eight-byte
values starting at the illustrative address `0x1000`:

| Element | Address of its first byte |
| --- | --- |
| `a[0]` | `0x1000` |
| `a[1]` | `0x1008` |
| `a[2]` | `0x1010` |
| `a[3]` | `0x1018` |

Here square brackets label elements, starting at zero. Each element starts
eight bytes after the previous one. In general:

```text
address of element i = starting address + i × bytes per element
```

Contiguity gives a predictable layout. It also makes nearby values available
together when data enter the cache.

## Why access order matters

Caches transfer data in blocks called **cache lines**, rather than fetching
only the single number requested by the calculation.

As an illustrative example, a 64-byte cache line holds eight eight-byte values.
If our array begins at a cache-line boundary, fetching its first line makes
`a[0]` through `a[7]` available together. Cache-line sizes are hardware-dependent.

Compare these access patterns:

```text
Sequential:    a[0], a[1], a[2], a[3], ...
Large strides: a[0], a[8], a[16], a[24], ...
```

With the assumptions above, sequential access can use all eight values from
each fetched line. The strided pattern uses only one value per line in that
pass. It may therefore transfer much more data per useful value.

Using nearby data is **spatial locality**. Reusing the same data while they
remain in cache is **temporal locality**. Predictable sequential access also
helps hardware fetch upcoming data in advance.

Contiguous storage alone is not sufficient: the order in which a program
visits its elements matters. Actual speedups depend on data size, cache reuse,
and the amount of arithmetic, so they should be measured.

## A matrix is still stored in a linear sequence

Consider a matrix:

```text
A00  A01  A02
A10  A11  A12
```

Two common contiguous layouts are:

```text
Row-major:    A00 A01 A02 A10 A11 A12
Column-major: A00 A10 A01 A11 A02 A12
```

C's built-in multidimensional arrays use row-major order: the last index
varies fastest. Ordinary contiguous Fortran arrays use column-major order:
the first index varies fastest.

For a simple sweep through all elements, visiting rows in C and columns in
Fortran follows consecutive memory locations. The index corresponding to
adjacent elements should change in the **innermost loop**, the loop making
the most frequent updates. We will implement this in the array lessons.

## Stack and heap are not extra levels of cache

You will also encounter **stack** and **heap**. These describe ways of
organizing program storage, not separate physical memory technologies.

The stack commonly supports function calls and local working data. The heap
supports dynamic allocations whose size or lifetime is decided while the
program runs. Exact placement depends on the language and compiler.

Data in either can be cached. A dynamically allocated array can be contiguous
and efficient to traverse; it is not inherently slower to read because it
comes from the heap. Large numerical arrays require deliberate allocation
choices, which we will cover when introducing C and Fortran.

## Summary

For your simulations, remember both **capacity** (do the arrays fit?) and
**locality** (does the access pattern reuse the data brought close to the CPU?).
The next lesson, [Programs, Processes, and System Resources](6-programs_processes_and_resources.md),
applies these ideas to running several calculations on one machine.
