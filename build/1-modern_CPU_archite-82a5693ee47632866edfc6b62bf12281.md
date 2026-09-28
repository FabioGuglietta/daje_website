# SIMD Architectures in Practice: From Laptops to Supercomputers

SIMD (Single Instruction, Multiple Data) exists on essentially every modern CPU, but the *instruction set* and the *programming model* vary a lot across platforms. For HPC work, those differences matter because they affect:

* which intrinsics/assembly you can write,
* what compilers can auto-vectorize,
* and how portable your “hand-optimized” kernels will be.


## SIMD in current x86-based HPC systems

As of 2025, most Tier-0 / pre-exascale European systems remain **x86-based** on the CPU side (Intel and AMD). SIMD here is typically:

* **Intel:** AVX-512 on many HPC/server parts
* **AMD EPYC:** AVX2 (256-bit); AMD EPYC generally does *not* implement AVX-512

Examples of x86-based HPC machines:

### Leonardo (CINECA, Italy)

Leonardo’s documentation describes:

* GPU partition nodes driven by **Intel Xeon Platinum 8358 (Ice Lake)** and **NVIDIA A100 GPUs**, plus a data-centric partition using **Intel Sapphire Rapids** CPUs. ([Cineca HPC][2])

### MareNostrum 5 (BSC, Spain)

EuroHPC describes MareNostrum 5 as a **pre-exascale EuroHPC supercomputer hosted at BSC**, supplied by Bull SAS (Bull Sequana XH3000) + Lenovo architectures. ([EuroHPC][3])

### JUWELS Booster (JSC, Germany)

JSC’s user documentation describes JUWELS Booster as GPU nodes hosted by **AMD EPYC Rome** CPUs with **4× NVIDIA A100** GPUs per node. ([FZ Jülich Apps][4])

(From a SIMD perspective: Intel systems often expose AVX-512; AMD EPYC exposes AVX2. On GPU-heavy partitions, CPU SIMD is still relevant for host-side kernels, packing, reductions, preprocessing, and parts of hybrid codes.)



### Example: `double` array add with SSE2 and AVX

Header:

```c
#include <immintrin.h>
```

SSE2 (2 doubles per vector):

```c
void add_f64_sse2(double *out, const double *a, const double *b, int n) {
    int i = 0;
    for (; i + 1 < n; i += 2) {
        __m128d va = _mm_loadu_pd(&a[i]);
        __m128d vb = _mm_loadu_pd(&b[i]);
        __m128d vc = _mm_add_pd(va, vb);
        _mm_storeu_pd(&out[i], vc);
    }
    for (; i < n; i++) out[i] = a[i] + b[i];
}
```

AVX (4 doubles per vector):

```c
void add_f64_avx(double *out, const double *a, const double *b, int n) {
    int i = 0;
    for (; i + 3 < n; i += 4) {
        __m256d va = _mm256_loadu_pd(&a[i]);
        __m256d vb = _mm256_loadu_pd(&b[i]);
        __m256d vc = _mm256_add_pd(va, vb);
        _mm256_storeu_pd(&out[i], vc);
    }
    for (; i < n; i++) out[i] = a[i] + b[i];
}
```

Typical instructions you will see in disassembly:

* loads/stores: `movupd` / `vmovupd`
* add: `addpd` / `vaddpd`

Compile and inspect:

```bash
gcc -O3 -march=native -S -o add_x86.s add_x86.c
```


## Apple Silicon: ARM AArch64 + NEON on the CPU

HPC machines are usually not based on Apple Silicon, but this architecture is found in many personal computers and worth mentioning here.  Apple Silicon (M1, M2, M3,...) implements **ARM AArch64**. CPU SIMD is provided by ARM’s standard **Advanced SIMD** extension, commonly referred to as **NEON**.

Key characteristics:

* **Vector registers:** `v0–v31` are **32 × 128-bit SIMD/FP registers**. ([developer.arm.com][1])
* **Vector width:** fixed at **128 bits**
* **Data types:** packed integers, `float32`, `float64`
* **Instruction model:** fixed-width SIMD (closest x86 analogue: SSE2)

Apple-specific *accelerators* exist (e.g., matrix engines), but they are not the same thing as CPU SIMD and we won't cover them here.

### Basic NEON intrinsics (Apple Silicon)

Header:

```c
#include <arm_neon.h>
```

Example: add two arrays of doubles with NEON (2 doubles per vector):

```c
void add_f64_neon(double *out, const double *a, const double *b, int n) {
    int i = 0;
    for (; i + 1 < n; i += 2) {
        float64x2_t va = vld1q_f64(&a[i]);     // load 2 doubles
        float64x2_t vb = vld1q_f64(&b[i]);     // load 2 doubles
        float64x2_t vc = vaddq_f64(va, vb);    // add lane-wise
        vst1q_f64(&out[i], vc);                // store 2 doubles
    }
    // tail (if n is odd)
    for (; i < n; i++) out[i] = a[i] + b[i];
}
```

This typically becomes in AArch64 assembly (schematically):

* loads/stores: `ld1` / `st1`
* arithmetic: `fadd v?.2d, v?.2d, v?.2d`

To see the actual assembly on macOS:

```bash
clang -O3 -S -o add.s add.c
```



## ARM in HPC: SVE changes the programming model

ARM has a major SIMD extension designed for HPC: **SVE (Scalable Vector Extension)**.

The key idea is **vector-length agnostic (VLA)** programming:

* the ISA does not hard-code 128/256/512-bit vectors,
* the hardware chooses a vector length,
* software is written to scale across those choices. ([Stony Brook University][5])

SVE also makes **predication** central: per-lane masks are part of the normal execution model, which greatly simplifies “tail handling” in loops. 

Some ARM-based HPC machines include:

### Fugaku (RIKEN, Japan) and Fujitsu A64FX

The processor used in Fugaku ([Fujitsu][6]) is A64FX, an ARMv8.2-A CPU implementing SVE (commonly discussed with a 512-bit implementation). ([Wikipedia][7])

#### Ookami (Stony Brook University, USA)

Ookami is a testbed that uses the **Fujitsu A64FX** processor (the same technology used in Fugaku). ([Stony Brook University][8])

#### Isambard 2 (UK)

Isambard 2 incorporates **Fujitsu A64FX** technology. ([GtR][9])


## Basic SVE intrinsics (HPC ARM)

Header:

```c
#include <arm_sve.h>
```

Example: add two arrays of doubles with SVE (vector-length agnostic, predicated tail):

```c
void add_f64_sve(double *out, const double *a, const double *b, int n) {
    int i = 0;
    while (i < n) {
        svbool_t pg = svwhilelt_b64(i, n);          // predicate active lanes
        svfloat64_t va = svld1(pg, &a[i]);          // predicated load
        svfloat64_t vb = svld1(pg, &b[i]);          // predicated load
        svfloat64_t vc = svadd_f64_z(pg, va, vb);   // predicated add (inactive lanes zeroed)
        svst1(pg, &out[i], vc);                     // predicated store
        i += svcntd();                              // advance by #double lanes in a vector
    }
}
```

What to notice (conceptually):

* There is **no fixed “vector width”** in the source.
* The loop “just works” for the tail because `pg` masks inactive lanes. ([Stony Brook University][5])
* This is the core difference vs NEON/AVX, where you often write a separate scalar remainder loop.

Typical compilation on an SVE system:

```bash
gcc -O3 -march=armv8.2-a+sve -S -o add_sve.s add_sve.c
```


[1]: https://developer.arm.com/documentation/dui0801/l/Overview-of-AArch64-state/Predeclared-extension-register-names-in-AArch64-state?lang=en&utm_source=chatgpt.com "Arm Compiler armasm User Guide"
[2]: https://www.hpc.cineca.it/systems/hardware/leonardo/?utm_source=chatgpt.com "Leonardo | HPC Cineca"
[3]: https://www.eurohpc-ju.europa.eu/supercomputers/our-supercomputers_en?utm_source=chatgpt.com "Our Supercomputers - The European High Performance Computing Joint ..."
[4]: https://apps.fz-juelich.de/jsc/hps/juwels/booster-overview.html?utm_source=chatgpt.com "JUWELS Booster Overview — JUWELS user documentation documentation"
[5]: https://www.stonybrook.edu/commcms/ookami/support/_docs/ARM_SVE_tutorial.pdf?utm_source=chatgpt.com "SVE Architecture & Optimization examples - Stony Brook University"
[6]: https://www.fujitsu.com/global/products/computing/servers/supercomputer/a64fx/?utm_source=chatgpt.com "FUJITSU Processor A64FX"
[7]: https://en.wikipedia.org/wiki/Fujitsu_A64FX?utm_source=chatgpt.com "Fujitsu A64FX"
[8]: https://www.stonybrook.edu/commcms/ookami/?utm_source=chatgpt.com "Home | Ookami - Stony Brook University"
[9]: https://gtr.ukri.org/projects?ref=EP%2FW03218X%2F1&utm_source=chatgpt.com "Isambard 2 expansion to add new testbeds and expand user base"

