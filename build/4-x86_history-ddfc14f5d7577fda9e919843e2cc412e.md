# The architecture of a CPU

If you're using a modern personal computer in the 2020s, it's likely that you're using a 
CPU with either x86-64 or ARM architecture. By architecture (more precisely an instruction set architecture, or ISA),
we mean a standard set of rules that define the way a CPU is supposed to be composed and behave. The ISA defines, for example the set 
of instructions (addition, subtraction, ...) , data types (integers, floating points and their representation) and memory 
(registers and addresses, memory management system, ...). 
Of course, every CPU manufacturer has usually several implementations of a specific ISA, and they are called 
microarchitecture.

In the following we restrict our discussion and historical overview to Intel-compatible architectures only.
Examples of architectures, microarchitectures and CPUs you might have heard of include:

- x86-16 architecture
     - Intel 8086
     - Intel 80286
- IA-32
     - Intel 80386
     - Intel 80486
     - Intel Pentium
     - AMD K6
- IA-64
   - Intel Itanium
- x86-64
   - Intel Core i5 (Haswell)
   - Intel Core i7 (Broadwell)
   - Intel Skylake
   - AMD Ryzen
   - AMD Epyc (Zen microarchitecture family)


We can say that a specific microarchitecture is the description of the specific components of a CPU, 
and entails concepts like those of arithmetic logic unit (ALU), Floating Point Unit (FPU), instruction sets, pipelines, cache, 
branch predictors, multithreading, and more. 

We will focus on Intel architectures, but our discussion is rather general in terms of modern CPU architectures. 
If you are interested in historical development of different architectural philosophies, you might want to explore,
for example

- Reduced Instruction Set Computers (RISC) vs Complex Instruction Set Computers (CISC)
- Scalar processors vs Vector processors

Most modern (2020s) CPUs are CISC/RISC hybrids, with the CPU receiving CISC instruction but executing them using 
proprietary RISC microinstructions. They are classified as scalar processors because they process finite-size chuncks
of data, even though this is usually done in a vectorised way with SIMD (single instruction multiple data) units. 

Now, a bit of history of microprocessors:

The first commercially produced microprocessor was the Intel 4004, a 4-bit CPU based on 
Metal-Oxide-Semiconductor (MOS)  silicon gate technology, designed by Federico Faggin and released in 1971, and it opened
the doors to the future of mass-produced, general purpose CPUs. Only few months later, Intel released the first 8-bit 
microprocessor, the 8008 (which Intel started developing even before the 4004 hit the market, showing the 
company's appetite to what became the modern microprocessor proliferation). Few years later, in 1978, Intel released the 
8086 and its close cousin, the 8088, the foundation of what we now know as the x86 architecture. 
The 8088 CPUs were a huge success as they were used in the IBM PC, debuting in 1981, and destined to become one of the more
influential lines of home and office computers, the x86-based architecture being one of the most used nowadays (excluding mobile
computing).

## Intel 8086
The 8086 was a 16-bit processor with a few core parts:

- ALU (Arithmetic Logic Unit): handles basic math and logic.
- Registers: (named AX, BX, CX, etc.) holding data and memory addresses temporarily.
- Control Unit: reads and executes instructions, telling the ALU and registers what to do.
- Memory Interface: Addressed up to 1 MB of memory

## Intel 80386

The 80386, released in 1985, was a game-changer. It was the first 32-bit processor in the 
x86 family, which meant it could handle more data and address a lot more memory—up to 4 GB. 
The 80386 could be supplemented by the 80387 floating point unit (FPU) coprocessor. In later CPUs, the
FPUs were typically integrated on-chip.

## i486

By 1989, the 80486 (then rebranded i486) brought some serious upgrades. 
It integrated the Floating Point Unit (FPU) directly into the CPU, making scientific calculations much faster. 
It also introduced an on-chip cache, which stored frequently used data close to the CPU, speeding up processing by 
reducing the need to fetch data from slower main memory.

## The Pentium Series: Parallel Processing

The early 1990s saw the arrival of the Pentium series, which introduced a superscalar architecture. 
This meant the CPU could execute multiple instructions at the same time, thanks to having multiple ALUs and FPUs. 
It also brought in branch prediction (guessing which way a program will go at a fork) and out-of-order execution 
(rearranging instruction execution to avoid delays), both of which boosted performance.

## The x86-64 architecture

The x86-64 architecture was introduced by AMD; Also known as AMD64, it was developed by AMD as an extension of 
Intel's x86 architecture to support 64-bit computing while maintaining backward compatibility with 32-bit and 
16-bit x86 code, which was lost in the competing 64 bit architecture of Intel (IA-64, Itanium processors). 
Evenually, Intel adopted the AMD64 architecture starting with the Pentium Pro in the mid-1990s.
These CPUs feature deep pipelines, sophisticated speculative execution (executing instructions before they're needed,
based on predictions), and multiple levels of cache (L1, L2, and L3) to keep the processor fed with data.
These CPUS also introduced MMX (the MultiMedia eXtensions) a type of SIMD (Single Instruction, Multiple Data) 
instructions able to operate on multiple data points at once

Building on the success of MMX, Intel introduced SSE (Streaming SIMD Extensions) with the Pentium III. 
SSE expanded SIMD capabilities beyond the integer operations of MMX to include floating-point operations.
Each new iteration (SSE2, SSE4) added more instructions and improved the performance and versatility to 
the SIMD units, making it increasingly valuable for a wider range of applications. By the time of SSE4, 
SIMD units were not just a tool for multimedia but also a significant asset in tasks like encryption, 
data compression, and many aspects of scientific computing.

With AVX (Advanced Vector Extensions), first appearing in Intel's Sandy Bridge processors in 2011, AVX brought 
wider vector registers (256 bits compared to 128 bits in SSE), allowing the CPU to process even more data in 
parallel. AVX is particularly impactful in areas requiring heavy numerical computation.

In summary:


- Intel's MMX (MultiMedia eXtensions): provides arithmetic and logic operation on 64-bits integer numbers in
  blocks of 2 32-bit, 4 16-bit or 8 8-bit operations in one instruction. Ther registers are called MM0, MM1, ...
- AMD's 3DNow: added single-precision (32-bit) floating point support to the MMX instruction set
- Intel's SSE (Streaming SIMD Extensions): providing single precision floating point operations and 128-bit registers (XMM0, XMM1,...)
- Intel's SSE2/SSE4: providing double precision (64-bit) floating point operations on 128-bit registers (
- Intel's AVX/AVX2 (Advanced Vector Extensions): introduces 256-bit (YMM) registers and wider instructions sets
- Intel's AVX-512: expands AVX to 512-bit support
  

