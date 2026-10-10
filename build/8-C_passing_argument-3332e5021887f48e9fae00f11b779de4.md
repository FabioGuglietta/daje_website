# Command-Line Arguments in C: argc and argv

Suppose we are writing a program to simulate a system of particles. Before
starting the calculation, we need to specify:

- **`N`**, the number of particles;
- **`temperature`**, the temperature of the system, expressed here in reduced units.

For example, we might want to simulate 1000 particles at temperature 1.5.
We could write these values directly into the program:

```c
int N = 1000;
double temperature = 1.5;
```

This works, but changing either value requires editing the source code and
recompiling it. In computational physics, we often repeat the same calculation
for different system sizes or temperatures.

**Command-line arguments** let us choose the parameters when starting the program:

```bash
./simulation 1000 1.5
```

Our program interprets the first argument as the number of particles and the
second as the temperature. Their meaning and order are conventions we choose;
the shell simply passes them as text.

The same executable can then run with different parameters:

```bash
./simulation 2000 1.5
./simulation 1000 2.0
```

No recompilation is needed.

## How arguments reach `main`

To receive command-line arguments, define `main` as:

```c
int main(int argc, char *argv[])
```

- `argc` is the number of arguments, including the program name.
- `argv` is an array of pointers to strings containing those arguments.

For `./simulation 1000 1.5`:

| Expression | Content |
| --- | --- |
| `argc` | `3` |
| `argv[0]` | `"./simulation"` |
| `argv[1]` | `"1000"` |
| `argv[2]` | `"1.5"` |

Although we supplied two parameters, `argc` is 3 because the program name is
included. `argv[argc]` is a null pointer marking the end of the array; it is
not an additional argument and must not be printed as a string.

## Inspecting the arguments

This small program prints every argument:

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] = \"%s\"\n", i, argv[i]);
    }
    return 0;
}
```

Save it as `show_arguments.c`, then compile and run it:

```bash
gcc show_arguments.c -o show_arguments
./show_arguments 1000 1.5
```

Output:

```text
argv[0] = "./show_arguments"
argv[1] = "1000"
argv[2] = "1.5"
```

The shell separates arguments before the program receives them. Quotes keep
text containing spaces together:

```bash
./show_arguments "run at high temperature" 1000
```

Here, `argc` is still 3. `argv[1]` contains `run at high temperature`, without
the grouping quotes.

## Converting strings to numbers with `sscanf`

`argv[1]` points to the string `"1000"`, not to an integer. We must convert
that text before using it as the number of particles.

`sscanf`, declared in `<stdio.h>`, reads values from a string according to a
format. For example:

```c
int N;
double temperature;

/* Conversion only: the complete example below checks the results. */
sscanf(argv[1], "%d", &N);
sscanf(argv[2], "%lf", &temperature);
```

`%d` reads an `int`; `%lf` reads a `double`. The `&` operator supplies the
address where the converted value should be stored.

> [!NOTE]
> **Reading a floating-point number: `%lf`**
>
> A *format specifier* tells a formatted input/output function how to interpret
> a value. In `sscanf`, `%lf` converts text such as `"1.5"` or `"1.5e2"` into a
> `double` and stores it at the supplied address. The `l` distinguishes it from
> `%f`, which requires a pointer to `float`. Since `temperature` is a `double`,
> we use `%lf` with `&temperature`. Using `%f` here would be a type mismatch,
> not a request for fewer decimal places.

The return value tells us how many values were successfully assigned. A
single numeric conversion should return 1. If it fails, we must not use the
uninitialized variable.

A conversion can accept a numeric prefix: `%d` reads 1000 from `"1000abc"`.
These basic checks do not validate the entire input string.

> [!NOTE]
> This introductory example assumes numeric input is within the representable
> range of `int` and `double`. `sscanf` does not provide reliable numeric
> overflow validation. The checks below detect failed conversions and unsuitable
> physical values, but do not make conversion safe for arbitrarily large numbers.

## A complete example: particle count and temperature

The following program validates and prints the two parameters. The actual
simulation would start after these checks.

```c
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: simulation <N> <temperature>\n");
        return EXIT_FAILURE;
    }

    int N;
    double temperature;

    if (sscanf(argv[1], "%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "Error: N must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    if (sscanf(argv[2], "%lf", &temperature) != 1 ||
        !isfinite(temperature) || temperature <= 0.0) {
        fprintf(stderr, "Error: temperature must be positive and finite.\n");
        return EXIT_FAILURE;
    }

    printf("Number of particles: %d\n", N);
    printf("Temperature: %g\n", temperature);

    /* Run the simulation using N and temperature. */

    return EXIT_SUCCESS;
}
```

Save it as `simulation.c`:

```bash
gcc simulation.c -o simulation -lm
./simulation 1000 1.5
```

Output:

```text
Number of particles: 1000
Temperature: 1.5
```

`argc != 3` rejects both missing and extra arguments before the program accesses
`argv[1]` or `argv[2]`. In each conversion check, `||` short-circuits: if conversion
fails, the following condition does not read the uninitialized numeric variable.

> [!NOTE]
> **Printing a floating-point number: `%g`**
>
> In `printf`, `%g` prints a `double` in either ordinary decimal notation or
> scientific notation, depending on its magnitude and the requested precision.
> By default it uses six significant digits and removes trailing fractional
> zeros and an unnecessary decimal point. Thus, `printf("%g", 1.5)` prints
> `1.5`, while `printf("%g", 10000000.0)` prints `1e+07`.
>
> To request more significant digits, write, for example, `%.10g`. In contrast,
> `%f` uses fixed-point notation with six digits after the decimal point by
> default: `printf("%f", 1.5)` prints `1.500000`.
>
> Input and output formats have different rules: `sscanf` needs `%lf` and a
> `double *` to **store** a double, whereas `printf` accepts the double value
> itself with `%g` or `%f`. Printing fewer digits does not change the stored value.

`isfinite`, declared in `<math.h>`, excludes infinity and NaN (not-a-number).
The positive-temperature condition is a requirement of this example's model;
choose physical constraints appropriate to your calculation.

## Reporting incorrect input

These commands are rejected:

```bash
./simulation
./simulation 1000
./simulation 1000 1.5 extra
./simulation abc 1.5
./simulation -100 1.5
./simulation 1000 nan
```

Use `fprintf(stderr, ...)` for diagnostics and `printf` for ordinary output.
The usage message explains the argument order, while validation messages say
which parameter needs correcting.

> [!NOTE]
> `EXIT_FAILURE` is a macro defined in `<stdlib.h>` that represents an
> unsuccessful exit status. Writing `return EXIT_FAILURE;` inside `main`
> ends the program and reports failure to the shell or script that launched it.
> It does not print an error message: that is the purpose of `fprintf(stderr, ...)`.
> Conversely, `return EXIT_SUCCESS;` reports success, as does `return 0;`
> from `main`. Use these names rather than assuming a particular numeric value
> for `EXIT_FAILURE`.

Valid parameters are only the first check: a real simulation must also handle
unavailable memory and other runtime errors.

## Applying the same idea to numerical integration

In the [integration example](1-C_basics.md), the number of trapezoids was set
inside the source code. We can instead accept one positive integer argument:

```bash
./integrate 1000
./integrate 10000
```

The same steps apply: require `argc == 2`, convert `argv[1]` into an `int`,
validate it, and pass it to the integration function. The numerical algorithm
itself does not change.
