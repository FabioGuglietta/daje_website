# Getting close to peak performance when optimizing code: SIMD, loop unrolling and associative algebra

Modern compilers are doing quite a large part of the heavy lifting that's necessary to 
produce performing code. The number of strategies a modern compiler is using in order to e the code for speed 
can be quite impressive, and choosing the correct optimization flags at compile time can really make a difference. However,
this is true only if the data layout and basic structure of the code allow for these strategies to be implemented. Still,
by performing some aations by hand and looking at the resulting performances and disassembled code we can learn which
optimization strategies works best and why. We will show that, with a bit of patience, we can even outperform the compiler and
reach performances that are close to the theoretical maximum that, as we will see, is set by the cache bandwdith for this 
specific problem.

## Basic performance optimization 
Let's start with a simple code that adds up floating point numbers:

```c
/* sum_std.c */
double sum(int N, double *a) {
    double s = 0.0;
    for (int i = 0; i < N; i++) {
        s += a[i];
    }
    return s;
}
```

We can test this by calling the function repeatedly and collecting some statistics, using this code:

```c
/* runner.c */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

double sum(int N, double *a);

#define NUM_RUNS 200
void time_function(int N, double *a, double (*func)(int, double*), double *mean, double *std) {
    clock_t start, end;
    double time, m=0.0, s=0.0;
    for (int i = 0; i < NUM_RUNS; i++) {
        start = clock();
        func(N, a);
        end = clock();
        time = (double)(end - start) / CLOCKS_PER_SEC;
        m += time;
	      s += time*time;
    }
    m/=NUM_RUNS;
    *mean = m;
    *std = sqrt(s/NUM_RUNS - m*m);
}

int main(void) {
    int N = 1<<20;
    double mean, std;
    /* Let's enforce 16 byte memory alignment, to be sure */
    double *a = (double*) aligned_alloc( (size_t)16, N * sizeof(double));

    for (int i = 0; i < N; i++) a[i] = (double)(i + 1);

    time_function(N, a, sum, &mean, &std);
    return printf("Average execution time for sum: %g ± %g ms \n", 1e3*mean, 1e3*std);
}
```

We can compile this with different optimization flags, for example:

```bash
> # no optimization
> gcc  -O0 -lm  runner.c sum_std.c -o sum  && ./sum
Average execution time for sum: 4.11662 ± 0.0645093 ms
```

We're doing 2**20 = 1,048,576 additions in 4 ms, this means that we're crunching numbers at 
the rate of 0.26 Gflops (1 flops = one floating point operation per second). This is far 
from the 16 Gflops theoretical peak of the EPYC 7713 @ 2GHz where this code has been ran.

Switching on `-O3` improves the performances by a factor 4,

```bash
> # -O3
> gcc  -O3  -lm  runner.c sum_std.c -o sum  && ./sum
Average execution time for sum: 1.03888 ± 0.0608442 ms 
```

bringing us to 1 Gflops, still more than 10 times slower than the theoretical peak. 
First of all, where does this factor 4 come from? 

## Difference in the compiled code

If we produce the assembly code for the first case with

```bash
gcc -O0 -S sum_std.c 
```

we see that the main loop, `.L3`, is mainly loading one double after the other, summing them up: 

```asm
	.file	"sum_std.c"
	.text
	.globl	sum
	.type	sum, @function
sum:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -8(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L2
.L3:
	movl	-12(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movsd	-8(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -8(%rbp)
	addl	$1, -12(%rbp)
.L2:
	movl	-12(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L3
	movsd	-8(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	sum, .-sum
	.ident	"GCC: (GNU) 11.4.1 20231218 (Red Hat 11.4.1-3)"
	.section	.note.GNU-stack,"",@progbits
```

Here's an annotated version of `.L3`:

```asm
.L3:
	movl	-12(%rbp), %eax      # Load the loop index (stored at -12(%rbp)) into %eax
	cltq                       # Sign-extend %eax (32-bit) into %rax (64-bit)
	leaq	0(,%rax,8), %rdx     # Compute the offset by multiplying the loop index by 8 and store it in %rdx
	movq	-32(%rbp), %rax      # Load the base address of the array (or pointer) from -32(%rbp) into %rax
	addq	%rdx, %rax           # Add the computed offset to the base address to get the current element address
	movsd	(%rax), %xmm0        # Load the double-precision float at the current element address into %xmm0
	movsd	-8(%rbp), %xmm1      # Load the running sum (or previous value) from -8(%rbp) into %xmm1
	addsd	%xmm1, %xmm0         # Add the current element value (in %xmm0) to the running sum (in %xmm1)
	movsd	%xmm0, -8(%rbp)      # Store the updated running sum back into -8(%rbp)
	addl	$1, -12(%rbp)        # Increment the loop index (stored at -12(%rbp)) by 1
```

The version compiled with `-O3` is different, in that in `.L4` (other lables like `.L3` here are used in special array length cases) makes use vectorised load operations:

```asm
	vmovupd	(%rax), %ymm3   # loads 4 double at a time, note the ymm 256-bit register
```

```asm
		.file	"sum_std.c"
	.text
	.p2align 4
	.globl	sum
	.type	sum, @function
sum:
.LFB0:
	.cfi_startproc
	movl	%edi, %edx
	movq	%rsi, %rcx
	testl	%edi, %edi
	jle	.L7
	leal	-1(%rdi), %eax
	cmpl	$2, %eax
	jbe	.L8
	movq	%rsi, %rax
	movl	%edi, %esi
	vxorpd	%xmm0, %xmm0, %xmm0
	shrl	$2, %esi
	decl	%esi
	salq	$5, %rsi
	leaq	32(%rcx,%rsi), %rsi
	.p2align 4
	.p2align 3
.L4:
	vmovupd	(%rax), %xmm1
	vmovupd	(%rax), %ymm3
	addq	$32, %rax
	vaddsd	%xmm1, %xmm0, %xmm0
	vunpckhpd	%xmm1, %xmm1, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	vextractf128	$0x1, %ymm3, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	vunpckhpd	%xmm1, %xmm1, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	cmpq	%rsi, %rax
	jne	.L4
	movl	%edx, %eax
	andl	$-4, %eax
	testb	$3, %dl
	je	.L12
	vzeroupper
.L3:
	movslq	%eax, %rsi
	vaddsd	(%rcx,%rsi,8), %xmm0, %xmm0
	leaq	0(,%rsi,8), %rdi
	leal	1(%rax), %esi
	cmpl	%esi, %edx
	jle	.L1
	addl	$2, %eax
	vaddsd	8(%rcx,%rdi), %xmm0, %xmm0
	cmpl	%eax, %edx
	jle	.L1
	vaddsd	16(%rcx,%rdi), %xmm0, %xmm0
	ret
	.p2align 4
	.p2align 3
.L7:
	vxorpd	%xmm0, %xmm0, %xmm0
.L1:
	ret
	.p2align 4
	.p2align 3
.L12:
	vzeroupper
	ret
.L8:
	xorl	%eax, %eax
	vxorpd	%xmm0, %xmm0, %xmm0
	jmp	.L3
	.cfi_endproc
.LFE0:
	.size	sum, .-sum
	.ident	"GCC: (GNU) 11.4.1 20231218 (Red Hat 11.4.1-3)"
	.section	.note.GNU-stack,"",@progbits
```

The remaining operations, though involve unpacking the 4 double and adding them up using scalar operations like

```asm
	vaddsd	%xmm1, %xmm0, %xmm0
```

## Optimizing code by hand with AVX intrinsics

It seems that the code above is not making full use of the vectorised operations. 
Let's try to force its hand by coding the function directly using AVX intrinsics:

```c
/* sum_avx.c */

// this include loads all the necessary definitions
// and constants to use the AVX extensions:
#include <immintrin.h>

double sum(int N, double *a) {
    // vsum will use a 256-bit register and is set to zero
    __m256d vsum = _mm256_set1_pd(0.0);
    
    // we move 4 doubles at a time (4 x 64 = 256)
    for (int i = 0; i < N; i += 4) {
        // load 4 doubles at a time and stores them in an AVX register
        __mm256d value = _mm256_load_pd(&a[i])
        // add the 4 double in column to vsum
        vsum = _mm256_add_pd(vsum, value);  
    }
    // now add up horizontally the 4 double in vsum,
    // and store the result in vsum  
    vsum = _mm256_hadd_pd(vsum, vsum);
    // copy the lower double-precision element;
    // we disregard the rest
    return _mm256_cvtsd_f64(vsum);
}
```

This turns out to be way faster (a factor 16 faster than the `-O0` version, and a factor 4 faster than the `-O3` version) :

```bash
> gcc  -O3 -lm  -mavx2 runner.c sum_avx.c -o sum  && ./sum
Average execution time for sum: 0.264505 ± 0.0396432 ms 
```

That's much better, almost  4 Gflops, only 1/4th of the theoretical peak of 14 Gflops.

How did we manage to get to this performance? Let's have a look at the corresponding assembly code generated with

```bash
gcc -O3 -march=native -mtune=native -S sum_avx.c
```
or, equivalently on this machine (an AMD EPYC 7713), with

```bash
gcc -O3 -mavx2 -S sum_avx.c
```

```asm
	.file	"sum_avx.c"
	.text
	.p2align 4
	.globl	sum
	.type	sum, @function
sum:
.LFB5484:
	.cfi_startproc
	testl	%edi, %edi
	jle	.L4
	decl	%edi
	vxorpd	%xmm0, %xmm0, %xmm0
	shrl	$2, %edi
	salq	$5, %rdi
	leaq	32(%rsi,%rdi), %rax
	.p2align 4
	.p2align 3
.L3:
	vappd	(%rsi), %ymm0, %ymm0
	addq	$32, %rsi
	cmpq	%rsi, %rax
	jne	.L3
	vhaddpd	%ymm0, %ymm0, %ymm0
	vzeroupper
	ret
	.p2align 4
	.p2align 3
.L4:
	vxorpd	%xmm0, %xmm0, %xmm0
	vhaddpd	%ymm0, %ymm0, %ymm0
	vzeroupper
	ret
	.cfi_endproc
.LFE5484:
	.size	sum, .-sum
	.ident	"GCC: (GNU) 11.4.1 20231218 (Red Hat 11.4.1-3)"
	.section	.note.GNU-stack,"",@progbits
```


where it is clear that the bulk of the work is done in `.L3`  by these lines:

```asm
.L3:
    vappd  (%rsi), %ymm0, %ymm0   # Add 4 packed double-precision floats from memory (pointed to by %rsi) 
                                   # to the values currently in the %ymm0 register. The result is stored back in %ymm0.
                                   # This is a vectorized addition, processing 4 doubles in parallel.

    addq    $32, %rsi              # Increment the memory pointer %rsi by 32 bytes (4 doubles x 8 bytes per double).
                                   # This moves to the next set of 4 double-precision floating-point numbers.

    cmpq    %rsi, %rax             # Compare the updated pointer %rsi with %rax (which holds the address 
                                   # marking the end of the array). This checks if we’ve processed all the elements.

    jne     .L3                    # If %rsi is still less than %rax (i.e., there are more elements left to process), 
                                   # jump back to label .L3 and repeat the loop.
```


Now, this way we are fully exploiting AVX instructions by adding packed double-precision floats while 
traversing the memory in jumps of 4 doubles.

## Non-associative algebra is blocking further ations

Why is the `-O3` flag not using the `vappd` instruction? This is because using packed operations would 
change the order operations. Remember that in floating point arithmentics
(a+b)+c is not the same as a+(b+c). So, in the code that uses `vappd` we are adding up the values in the four columns first, and 
then we are adding up the four final values. This is not the same as what our initial C code was supposed to do (sum one after another
all the doubles). Let's quick-fix this by releasing the constraint that prevent us from reordering, by passing `-ffast-math` to the compiler:

```bash
> gcc  -O3 -mtune=native -march=native  -ffast-math -lm  runner.c sum_std.c -o sum  && ./sum
Average execution time for sum: 0.265075 ± 0.0677651 ms 
```

In fact, we have now recovered the same performances, and looking at the assembly code it's clear that the code now makes full use of the
AVX instructions, for example, looking at the output of 

```bash
gcc -O3 -march=native -mtune=native -ffast-math -S sum_std.c
```

we find:
```asm
.L4:
	vappd	(%rax), %ymm0, %ymm0
	addq	$32, %rax
	cmpq	%rcx, %rax
	jne	.L4
```



## Providing proper code to the compiler

Using `-ffast-math` might be not what we want in some cases.
How can we convince the compiler to reach this level of optimization without this "trick"? 
The answer is to provide a code that is already arranged in four "columns", so that using 
the packed sum `vappd` won't change the order of the operations; we "unroll" the loop manually:

```c
/* sum_unrolled.c */
double sum(int N, double *a) {
    double s[4];
    for (int j=0;j<4;j++) s[j]=0.0;

    for (int i = 0; i < N-4; i+=4) {
        for (int j=0;j<4;j++) s[j] += a[i+j];
    }
    return s[0]+s[1]+s[2]+s[3];
}
```

This yields the wanted performance without using `-ffast-math`:

```bash
> gcc  -O3 -mtune=native -march=native  -lm  runner.c sum_unroll.c -o sum  && ./sum
Average execution time for sum: 0.26451  ± 0.00615288 ms 
```

## Can we get closer to the peak performances? 

Considering that we're doing 2**20 = 1,048,576 additions in 0.27 ms, we're crunching numbers at 
the rate of 3.9 Gflops. The EPYC 7713 is working at a clock speed of 2GHz. It's a Zen 3 architecture, which, for the 
packed add (VADDPS/D) operations has a latency of three clock cycles and a reciprocal throughput of 0.5 
(cycles per instruction), meaning that every cycle it can perform two operations on 4 double precision floats.  
Under ideal conditions (an effortless, continuous stream of data from L1 cache to the SIMD units), this would 
total 8 flops x 2 GHz = 16 Gflops. This is the way we came to the theoretical peak performances mentioned before.

> [!TIP]
> You can find the information on latency and CPI of the instructions of many modern processors on Agner Fog's
> [Software optimization resources](https://www.agner.org/optimize/), in particular in the
> [instruction tables](https://www.agner.org/optimize/instruction_tables.pdf). For Intel processors, you can also
> refer to the [intrinsics guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html).
>

The fact that the CPI is 0.5 suggests that we could pack more than one `vaddpd` instruction within the `for` loop, because at the moment, 
as it is also apparent from the assembly code
```asm
.L4:
	vaddpd	(%rax), %ymm0, %ymm0
	addq	$32, %rax
	cmpq	%rcx, %rax
	jne	.L4
```
only one `vaddpd` is performed before moving the pointer forward. Let's try to force pre-fetching of two 4-double-precision blocks of data in each iteration, simply like this:
```c
/* sum_avx.c */
#include <immintrin.h>
double sum(int N, double *a) {
    double s1,s2;
    __m256d vsum1 = _mm256_set1_pd(0.0);
    __m256d vsum2 = _mm256_set1_pd(0.0);
    for (int i = 0; i < N; i += 8) {
        vsum1 = _mm256_add_pd(vsum1, _mm256_load_pd(&a[i]) );
        vsum2 = _mm256_add_pd(vsum2, _mm256_load_pd(&a[i+4]) );
    }
    vsum1 = _mm256_hadd_pd(vsum1, vsum1);
    vsum2 = _mm256_hadd_pd(vsum2, vsum2);
    _mm256_store_pd(&s1, vsum1);
    _mm256_store_pd(&s2, vsum2);
    return s1+s2;
}
```


```bash
> gcc  -lm -O3 -mavx2  runner.c sum_avx.c -o sum  && ./sum
Average execution time for sum:  0.132805 ± 0.0319479 ms
```
This is a noticeable performance boos, to **7.9 Gflops**!
However, this is still just a bit more than half of peak performance. Where did the the remaining time go "wasted"? 

Let's consider the data size: 2**20 double = 8 MB will only fit in L3 cache on the EPYC 7713 
[(L3: 32 MB/core; L2: 512 kB/core; L1: 32 kB/core)](https://en.wikipedia.org/wiki/Epyc). This means that there is a bottleneck to 
move data from L3 down to the registers. The [cache peak bandwidth is about 32 bytes/clock cycle](https://www.anandtech.com/show/16214/amd-zen-3-ryzen-deep-dive-review-5950x-5900x-5800x-and-5700x-tested/4),
meaning 32 bytes / (8 bytes/flop) x 2 GHz = **8 Gflops**, showing that with the code above crunching numbers at 7.9 Gflops we **managed to saturate the bandwidth, reaching the actual theoretical limit** for this kind of operation on this CPU.

> [!NOTE]
> One cache line on the 7713 is precisely 64 bytes, or, the 8 double-precision floats
> we are loading in the code above. In this sense, the code makes the most efficient use
> of the cache bandwidth.

> [!IMPORTANT]
> It's always important to understand whether your calculation is compute-limited, with
> many operations to be performed on data residing in the registers or close nearby, or
> memory-limited, where the work is less computationally intensive, but it has to be
> performed on a large set of data that needs to be streamed continuously from RAM/cache 
> the registers. In the last case, the bottleneck is the memory bandwidth, and this usually
> prevents to reach the theoretical peak flops.
>
>
