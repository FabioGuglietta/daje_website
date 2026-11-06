# Writing Efficient Scientific Code

An efficient simulation uses less time or memory to solve the same problem at
the required accuracy. Start from a checked program, measure its performance,
and change one thing at a time.

## Measure before changing the code

Run a representative calculation and measure its elapsed time:

```bash
time ./planetary initial.dat trajectory.dat 0.01 10000
```

The shell reports elapsed time as `real`, and CPU time as `user` and `sys`.
Repeat the measurement a few times: other work on the computer can affect it.
Keep the input, number of steps, output frequency, compiler options, and
computing resources the same when comparing two versions.

A **profiler** measures where the program spends time, for example in force
evaluation, memory allocation, or output. Optimize the part that actually
limits the run. If output takes most of the time, rewriting a small arithmetic
expression will have little effect.

Keep the original version under version control. After each change, run the
[tests](3-Organizing_scientific_code.md#use-tests-to-check-and-preserve-behavior)
and compare the numerical results before accepting a speed improvement.

## Avoid unnecessary work in the algorithm

For pair interactions, visit each unordered pair once:

```c
for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
        /* Compute the pair force and update both particles. */
    }
}
```

A loop over all `i` and all `j != i` visits both `(i, j)` and `(j, i)`.
For a force satisfying Newton's third law, compute the force on one particle
and apply its opposite to the other. If storing accelerations, divide by each
particle's own mass. Initialize all force or acceleration accumulators before
the pair loop.

This reduces the number of pair evaluations from `n * (n - 1)` to
`n * (n - 1) / 2`, but the cost still grows approximately as the square of `n`.
For large systems, choosing an algorithm with better scaling can matter much
more than reducing a few instructions inside this loop.

For example, neighbor lists can avoid checking every pair for short-range
interactions. They do not directly apply to an untruncated gravitational
interaction: the algorithm must match the physical model.

## Move repeated calculations out of loops

If every particle has the same nonzero mass, compute its reciprocal once:

```c
double inverse_mass = 1.0 / mass;

for (int i = 0; i < n; ++i) {
    acceleration[i] = force[i] * inverse_mass;
}
```

Here the arrays hold one component per particle. This avoids writing a
division at every iteration:

```c
for (int i = 0; i < n; ++i) {
    acceleration[i] = force[i] / mass;
}
```

Move a calculation only when its inputs stay unchanged throughout the loop.
Recompute `inverse_mass` if `mass` changes. The multiplication and division
can round differently, so compare results using a suitable tolerance.

For a square, write `x * x` rather than a general power calculation such as
`pow(x, 2.0)`. A compiler may already simplify expressions or move invariant
calculations; these changes do not guarantee a speed improvement. Measure
the compiled program.

## Use data locality

Performance depends on how data move between RAM, cache, and CPU. Arrange
accesses so the program uses nearby data and reuses data while they are
still in cache.

### Spatial locality: access neighboring elements

For a contiguous C array `double field[n][n]`, the last index varies fastest
in memory. Visit neighboring elements in the inner loop:

```c
double total = 0.0;

for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
        total += field[i][j];
    }
}
```

The reversed loop order accesses elements separated by an entire row:

```c
double total = 0.0;

for (int j = 0; j < n; ++j) {
    for (int i = 0; i < n; ++i) {
        total += field[i][j];
    }
}
```

This is **spatial locality**: accessing one element brings a cache line
containing nearby elements closer to the CPU. Visiting those neighbors next
uses data already loaded, instead of immediately requesting another distant
part of the array. This usually matters more for large arrays. The
[memory lesson](../1-Operating_Systems_and_Unix/5a-memory_and_data_layout.md)
explains why contiguity matters. Fortran stores multidimensional arrays in
a different order: there, the first index varies fastest.

Changing this summation order can also change the rounded result. Check
agreement at the required accuracy rather than expecting identical bits.

### Temporal locality: reuse data soon

If two independent calculations need the same array, they can sometimes share
one traversal. For example, accumulate both a sum and a sum of squares:

```c
double sum = 0.0;
double sum_squared = 0.0;

for (int i = 0; i < n; ++i) {
    double value = values[i];
    sum += value;
    sum_squared += value * value;
}
```

Two separate loops would traverse `values` twice. Here each element is used
for both calculations immediately, while it can remain in a register.
More generally, **temporal locality** means reusing data soon enough that
they are still in cache, rather than loading them again from RAM later.

For large multidimensional calculations, processing a small block at a time
can similarly keep a working set in cache while several operations use it.
This technique is called **blocking** or **tiling**.

Combining loops is valid only when it preserves the calculation's data
dependencies. It does not always make the program faster: the compiler,
array size, and cache capacity affect the result. Measure the change.

## Reuse allocated memory

Allocate a working array once and reuse it across time steps:

```c
double *forces = malloc(n * sizeof(*forces));
if (forces == NULL) {
    return EXIT_FAILURE;
}

for (int step = 0; step < num_steps; ++step) {
    for (int i = 0; i < n; ++i) {
        forces[i] = 0.0;
    }

    /* Compute forces and advance the system using this array. */
}

free(forces);
```

This fragment belongs inside `main`, with `<stdlib.h>` included and a
positive, validated particle count `n`. It avoids allocating and freeing
the same workspace at every step. Reusing memory does not reset its contents:
initialize quantities that must start from zero at each step.

## Choose output frequency deliberately

Save snapshots at a chosen interval instead of automatically writing every
time step:

```c
if (step % output_every == 0) {
    write_snapshot(output, bodies, n, time);
}
```

Here `write_snapshot` is the simulation's output function and `output_every`
must be a positive integer. Keep the output file open during the run rather
than reopening it for each snapshot.

Writing less often can reduce output time and file size, but removes temporal
information. Choose an interval that resolves the behavior you need to study,
record it with the input, and keep enough
[raw data for later analysis](3-Organizing_scientific_code.md#save-raw-data-before-analyzing-it).
Do not increase the integration time step just to produce fewer output files:
integration accuracy and output frequency are separate choices.

## Enable compiler optimizations and check the results

For a single-file program, try an optimized build:

```bash
gcc -O2 -Wall -Wextra planetary.c -o planetary -lm
```

`-O2` enables compiler optimizations. Compare its runtime with your previous
build using the same calculation, and run the tests on the optimized executable.
Higher optimization levels are not a guarantee of shorter runtime.

> [!NOTE]
> Options such as `-ffast-math` allow transformations that relax floating-point
> rules, including changes to the order of arithmetic. They can invalidate
> algorithms such as Kahan summation. Treat them as changes to numerical
> behavior that require specific verification.

See the [GCC optimization options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
for the compiler's definitions. Use the checks in
[Reliable Numerical Calculations](2-Numerical_best_practices.md) to assess
accuracy after an optimization. A shorter runtime is useful only when the
program still solves the intended problem.
