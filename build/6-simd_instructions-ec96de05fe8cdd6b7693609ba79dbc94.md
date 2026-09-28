# Introduction to SIMD instructions on x86-64

Modern x86-64 CPUs can operate not only on *scalar* values (one integer or floating-point number at a time), but also on *vectors* of values using **SIMD** (Single Instruction, Multiple Data) instructions. SIMD instructions allow the same arithmetic operation to be applied simultaneously to multiple data elements packed inside a single register.

This capability is central to high-performance numerical code and is one of the main reasons why optimized code can outperform naïve implementations by an order of magnitude or more, even when the algorithm itself is unchanged.

This section introduces:

* SIMD register families and their sizes
* Supported data types
* Load / compute / store operations
* How SIMD instructions appear in assembly
* How to use SIMD intrinsics from C

This background will be sufficient to understand the optimisation strategies discussed in the next section on **SIMD, loop unrolling, and associative algebra**.


## Scalar vs SIMD execution

In scalar execution, a CPU instruction operates on a single data element:

```asm
addsd %xmm1, %xmm0   # xmm0 = xmm0 + xmm1 (one double)
```

Here, one floating-point addition is performed per instruction.

In SIMD execution, a single instruction operates on multiple data elements packed in a wider register:

```asm
vaddpd %ymm1, %ymm0, %ymm0   # add 4 doubles at once
```

Conceptually, the difference is:

```
scalar:  s += a[i]

SIMD:    s[0..3] += a[i..i+3]
```

The instruction count may be similar, but the **amount of work done per instruction** is higher. This is the fundamental source of SIMD speedups.


## SIMD register families on x86

Modern x86-64 CPUs expose several generations of SIMD registers. The most relevant ones today are:

| Extension  | Register     | Width   | Typical use          |
| ---------- | ------------ | ------- | -------------------- |
| MMX        | `mm0–mm7`    | 64 bit  | legacy integer SIMD  |
| SSE        | `xmm0–xmm15` | 128 bit | float + integer SIMD |
| AVX / AVX2 | `ymm0–ymm15` | 256 bit | wider vectors        |
| AVX-512    | `zmm0–zmm31` | 512 bit | high-end CPUs        |

A few important points:

* SIMD registers are **distinct** from general-purpose registers (`rax`, `rbx`, …).
* Floating-point SIMD arithmetic does not use the scalar FPU stack found in very old x86 CPUs.
* SIMD registers form a **hierarchy**:

  * `xmm0` is the lower 128 bits of `ymm0`
  * `ymm0` is the lower 256 bits of `zmm0`

This overlap matters in practice. For example, mixing SSE (`xmm`) and AVX (`ymm`) instructions incorrectly can introduce performance penalties, which is why you often see a `vzeroupper` instruction in compiler-generated code. Modern compilers manage this automatically, but it is useful to recognise it when reading assembly.

In this course we will mainly focus on **SSE (128-bit)** and **AVX (256-bit)**.


## SIMD data types and packing

SIMD registers hold *packed* data: several values of the same type laid out contiguously inside the register.

### Integers

| Type    | Bits | Elements in XMM | Elements in YMM |
| ------- | ---- | --------------- | --------------- |
| `int8`  | 8    | 16              | 32              |
| `int16` | 16   | 8               | 16              |
| `int32` | 32   | 4               | 8               |
| `int64` | 64   | 2               | 4               |

### Floating point

| Type     | Bits | Elements in XMM | Elements in YMM |
| -------- | ---- | --------------- | --------------- |
| `float`  | 32   | 4               | 8               |
| `double` | 64   | 2               | 4               |

All elements in a SIMD register are processed **independently and in parallel** by most arithmetic instructions. There is no implicit communication between lanes unless an explicit instruction requests it.

Throughout the optimisation examples, we will mainly use **packed double precision (`double`)**, since it makes the effect of SIMD explicit and avoids precision issues when mixing scalar and vector code.


## Basic SIMD operations

SIMD code typically follows a **load → compute → store** pattern.

### Load

Load packed data from memory into a SIMD register:

```asm
vmovupd (%rax), %ymm0   # load 4 doubles (unaligned)
```

* `vmovupd`: vector move, unaligned, packed double
* Aligned versions (`vmovapd`) exist, but are only safe when memory alignment is guaranteed.

Unaligned loads are generally safe and efficient on modern CPUs, provided accesses do not cross cache-line boundaries too often.

### Arithmetic

Apply an operation element-wise:

```asm
vaddpd %ymm1, %ymm0, %ymm0   # ymm0[i] += ymm1[i]
```

Other common operations include:

* `vsubpd` – subtraction
* `vmulpd` – multiplication
* `vdivpd` – division
* `vfma*` – fused multiply-add (introduced later)

Each operation is applied independently to each lane of the vector.

### Store

Write SIMD register contents back to memory:

```asm
vmovupd %ymm0, (%rax)
```


## SIMD instructions vs scalar instructions

Compare scalar and SIMD addition:

```asm
addsd  %xmm1, %xmm0            # scalar double
vaddpd %ymm1, %ymm0, %ymm0     # 4 doubles at once
```

The SIMD version:

* Performs multiple operations per instruction
* Requires data to be independent
* May change the **order** in which floating-point operations are evaluated

This last point is critical. Floating-point arithmetic is not associative, and reordering operations can change results slightly. This constraint is one of the main reasons why compilers sometimes avoid vectorisation, and it will be analysed in detail in the next chapter.


## SIMD and the memory hierarchy

SIMD increases **compute throughput**, not memory bandwidth.

Important points to keep in mind:

* Data must still be fetched from cache or RAM.
* Many numerical kernels are **memory-bound**, not compute-bound.
* In such cases, wider SIMD registers do not automatically yield proportional speedups.

Typical cache line size on modern CPUs:

* **64 bytes**, corresponding to:

  * 8 doubles
  * exactly two AVX (256-bit) loads

This alignment between cache-line size and SIMD width is intentional and plays an important role in performance modelling.


## Using SIMD intrinsics in C

Writing SIMD assembly directly is rarely necessary. Instead, we use **intrinsics**: C functions that map almost one-to-one to SIMD instructions.

Include the required header:

```c
#include <immintrin.h>
```

Example: vector addition with AVX

```c
__m256d a = _mm256_loadu_pd(ptr);
__m256d b = _mm256_loadu_pd(ptr2);
__m256d c = _mm256_add_pd(a, b);
_mm256_storeu_pd(out, c);
```

Each intrinsic maps directly to a single instruction:

| Intrinsic          | Assembly  |
| ------------------ | --------- |
| `_mm256_loadu_pd`  | `vmovupd` |
| `_mm256_add_pd`    | `vaddpd`  |
| `_mm256_storeu_pd` | `vmovupd` |

Intrinsics are:

* Recognised directly by the compiler
* Inlined automatically
* Not function calls
* A precise way to control vectorisation without writing assembly


## Horizontal vs vertical operations

SIMD arithmetic is **vertical** by default: each lane is processed independently.

```
[a0 a1 a2 a3] + [b0 b1 b2 b3]
```

Some operations, such as summing all elements of an array, require **horizontal** reduction:

```
((a0 + a1) + (a2 + a3))
```

This requires explicit instructions, for example:

```c
v = _mm256_hadd_pd(v, v);
```

Horizontal operations:

* Are more expensive than vertical ones
* Often limit achievable performance
* Are a key reason why loop unrolling and algebraic rearrangement matter

This distinction is central to the optimisation strategies discussed next.


## A note on AVX instruction encoding

AVX instructions differ from SSE in two important ways:

1. **Three-operand form**

   ```asm
   vaddpd src1, src2, dst
   ```

   avoids overwriting inputs and reduces false dependencies.

2. **VEX encoding**
   Enables wider registers and cleaner dependency handling.

Because of this, AVX code often looks structurally different from SSE code in disassembly. Compilers may insert `vzeroupper` instructions to manage transitions safely.


