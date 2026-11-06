# Organizing a Scientific Code

As a simulation grows, organize it so you can change one component, run a small
test, and reproduce a calculation without reconstructing the entire workflow.

Use the [writing conventions](1-Introduction.md) to make individual functions
readable and the [numerical practices](2-Numerical_best_practices.md) to keep
their calculations reliable. This lesson connects those pieces into a project.

## Separate responsibilities and expose clear interfaces

A particle simulation can separate its source files by responsibility:

| File | Responsibility |
| --- | --- |
| `main.c` | Read run parameters and coordinate the calculation |
| `io.c` | Read configurations and write output |
| `physics.c` | Evaluate forces and physical quantities |
| `integrator.c` | Advance the state in time |

Keep reading, physical calculations, time integration, and output in separate
functions. A simulation might use:

```text
read_system
compute_accelerations
step_verlet
write_snapshot
```

This makes it easier to change the input format without touching the force
calculation, or to check the force calculation without running a full simulation.

Put a repeated physical operation in one function when it should have one
definition throughout the program. For example, all calls should use the same
interaction law and parameters.

Simple expressions do not automatically need their own functions. Choose
boundaries that help explain, check, or reuse the calculation. Function length
is a clue to readability, rather than a fixed rule.

Keep declarations in headers and implementations in source files. For example,
the kinetic-energy function from
[No programmer is an island](1-Introduction.md#make-the-calculation-readable)
can be implemented in `src/physics.c` and declared in `include/physics.h`:

```c
#ifndef PHYSICS_H
#define PHYSICS_H

double kinetic_energy(double mass, const double velocity[2]);

#endif
```

Include `"physics.h"` in the implementation and in code that calls the function.
The header makes the interface visible to the compiler. Document required
array lengths, units, and which functions modify their arguments. Pass needed
parameters explicitly instead of relying on mutable global state.

## Give project files clear places

One possible layout is:

```text
project/
├── README.md
├── Makefile
├── include/
│   └── physics.h
├── src/
│   ├── main.c
│   ├── physics.c
│   ├── io.c
│   └── integrator.c
├── tests/
│   └── test_kinetic_energy.c
├── input/
│   └── initial.dat
├── scripts/
│   └── plot_orbits.py
├── build/
└── runs/
```

Keep compiled files in `build` and generated simulation results in `runs`.
Start with the files and directories you actually need; split a component
when its responsibility becomes distinct. A small program does not need an
elaborate directory hierarchy.

## Use tests to check and preserve behavior

A **test** runs a calculation with a specified input and compares the result
with an expected outcome. It gives a repeatable answer to a precise question:
does this part of the program behave as intended for this case?

Tests help detect mistakes early, locate their source, and check that a later
change has not broken something that previously worked. This matters when
adding an interaction, changing a file format, or optimizing a numerical loop.
A short test can catch a missing factor before you launch a long simulation.

### Start with results you can verify independently

For the kinetic-energy function introduced in the first lesson, a mass of 2 and velocity (3, 4) must
give 25. Other useful cases are zero velocity and a velocity along one axis.

Derive expected results from an analytical formula or an independently checked
example. Copying a program's output into a reference file does not establish
that the output is correct.

Implement `kinetic_energy` in `src/physics.c` as described above, without a
`main`. Then write `tests/test_kinetic_energy.c`:

```c
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "physics.h"

int main(void) {
    double velocity[2] = {3.0, 4.0};
    double expected = 25.0;
    double computed = kinetic_energy(2.0, velocity);
    double tolerance = 1e-12;  /* Suitable for this small example. */

    if (!isfinite(computed) || fabs(computed - expected) > tolerance) {
        fprintf(stderr, "FAIL: expected %.15g, obtained %.15g\n",
                expected, computed);
        return EXIT_FAILURE;
    }

    printf("PASS: kinetic energy\n");
    return EXIT_SUCCESS;
}
```

Compile and run it:

```bash
mkdir -p build
gcc -Iinclude tests/test_kinetic_energy.c src/physics.c -o build/test_kinetic_energy -lm
./build/test_kinetic_energy
```

The test reports a failure if the function returns an incorrect result,
including a non-finite value. Its exit status also lets a script detect the
failure automatically. Both files use the same declaration from `physics.h`.

### Check more than individual functions

| Check | Example | Purpose |
| --- | --- | --- |
| Unit test | Kinetic energy for known mass and velocity | Check one operation in isolation |
| Integration test | Read a configuration, advance one step, write a snapshot | Check that components work together |
| Regression test | Repeat a previously verified case after editing the code | Detect unintended changes |
| Numerical verification | Reduce the time step at fixed final time | Check convergence toward the intended solution |

These categories overlap: a unit test can also serve as a regression test.

The [numerical lesson](2-Numerical_best_practices.md#check-convergence-and-physical-invariants)
explains time-step convergence and physical invariants. Automating these
checks makes them easier to repeat after changes.

### What passing tests tells you

Passing tests show that the selected cases meet the specified requirements.
They do not prove correctness for every input or establish that the physical
model describes the real system. Comparing with experimental data addresses
a further question: whether the model is appropriate for the physics.

Begin with a few small, meaningful cases. Run them after changing the code
and before launching expensive calculations. Keep test inputs and expected
results under version control so collaborators can repeat the same checks.

## Handle runtime errors and compiler warnings

Tests exercise selected cases before or during development. Runtime checks
handle problems in the actual run, such as a missing input file or a failed
allocation. Both are useful.

Compiler warnings are another useful check:

```bash
gcc -Wall -Wextra -Iinclude src/main.c src/physics.c src/io.c src/integrator.c -o planetary -lm
```

These options help identify issues such as incompatible types or unused
variables. Also handle failures that can occur during ordinary use: missing
files, invalid parameters, and failed allocations. Report which operation
failed and enough context to locate the problem.

For example, before using a stream returned by `fopen`:

**Problematic:**

```c
FILE *input = fopen(input_path, "r");
fscanf(input, "%d", &n);  // input could be NULL.
```

**Improved, inside `main`:**

```c
FILE *input = fopen(input_path, "r");
if (input == NULL) {
    fprintf(stderr, "Cannot open input file: %s\n", input_path);
    return EXIT_FAILURE;
}

if (fscanf(input, "%d", &n) != 1 || n <= 0) {
    fprintf(stderr, "Invalid particle count in %s\n", input_path);
    fclose(input);
    return EXIT_FAILURE;
}
```

Here `input_path` names the file and `n` is an `int`. Include `<stdio.h>` and
`<stdlib.h>`. Continue reading only after these checks, and close the stream
when finished.

## Keep simulation data organized and reproducible

For every set of results, you should be able to answer two questions:
**which version of the code produced them, and which input was used?**

Save a copy of the input file alongside the results. Record the Git commit
used to build the executable in a small text file, such as `code_version.txt`.
If the source has uncommitted changes, the commit alone is not enough: commit
those changes or preserve a copy of the source used for that run.

### Organize folders by simulation parameters

Use a consistent directory hierarchy. For example, group runs first by
particle count, then by temperature, and finally by run number:

```text
runs/
└── N_1000/
    ├── T_0.8/
    │   ├── run_01/
    │   │   ├── input.dat
    │   │   ├── code_version.txt
    │   │   ├── raw/
    │   │   │   └── trajectory.dat
    │   │   └── analysis/
    │   │       └── mean_squared_displacement.dat
    │   └── run_02/
    │       └── ...
    └── T_1.0/
        └── run_01/
            └── ...
```

Here `N` is the particle count and `T` is the temperature in the simulation's
chosen units. Choose folder levels that match the parameters you vary.
Folder names help you find a run; the saved input records its full settings.

**Do not overwrite previous data.** Create a new run directory for each
execution, including repeated runs with the same parameters.

### Save raw data before analyzing it

Keep data analysis in a separate program or script. The simulation should
save the raw data needed for later analysis, such as particle positions at
chosen time intervals. The analysis script reads those files and writes its
results separately.

For example, if the simulation writes only a mean squared displacement and
discards the particle trajectories, you cannot later change the averaging
procedure or calculate a different observable without running the simulation
again. With the trajectories saved, you can repeat or modify the analysis
without repeating the simulation.

Choose what to save and how often according to the questions you want to
study and the storage available. Keep the raw files unchanged when generating
new analysis results.

## Make changes that can be understood and repeated

Use version control to record the evolution of the code. Make changes in
manageable steps: establish a working force calculation before adding a new
interaction, and check that earlier examples still behave correctly.

A useful commit message explains the change, for example
`Use minimum-image distances for periodic boundaries`. Track source files,
input configurations, and analysis scripts; choose a separate storage strategy
for large simulation outputs when necessary.

Automate a sequence once it becomes repetitive. A short script can build the
program, run a set of configurations, and generate plots. It should stop and
report a failed run before proceeding to analyze missing or incomplete output.
A build tool can also record dependencies between source files.

The lessons on [Git](../../6-Tools/3-Git_for_version_control/1-basics.md) and
[Make](../../6-Tools/1-Building_Code/1-make.md) introduce tools for these tasks.
