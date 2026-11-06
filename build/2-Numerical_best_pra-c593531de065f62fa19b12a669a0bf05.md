# Reliable Numerical Calculations

Floating-point operations have finite precision. A mathematically equivalent
expression can behave differently on a computer. The following examples assume
the usual IEEE 754 double precision. Use `double` as a starting point for these
calculations, and choose precision according to the accuracy you need.

## Adding quantities with very different magnitudes

```c
double total = 1e16;
total += 1.0;
printf("%.0f\n", total - 1e16);  // Prints 0, not 1.
```

At this magnitude, double precision cannot retain that increment of one.
Repeatedly adding small contributions to a large running total can lose them.

The order of summation also matters:

```c
double a = 1e16, b = -1e16, c = 1.0;
printf("%f\n", (a + b) + c);  // 1.000000
printf("%f\n", a + (b + c));  // 0.000000
```

For long sums, consider pairwise or compensated summation, such as Kahan's
method. Check sensitivity to summation order. Different parallel reduction
orders can produce different rounded results.

> [!NOTE]
> **Kahan compensated summation**
>
> A simple running sum can lose small contributions through rounding.
> Kahan's method keeps a correction that approximately tracks the rounding
> error of each addition and accounts for it in the next one:
>
> ```c
> double kahan_sum(const double values[], int n) {
>     double sum = 0.0;
>     double correction = 0.0;
>
>     for (int i = 0; i < n; ++i) {
>         double adjusted = values[i] - correction;
>         double updated = sum + adjusted;
>         correction = (updated - sum) - adjusted;
>         sum = updated;
>     }
>
>     return sum;
> }
> ```
>
> `adjusted` compensates the next value using the previous correction.
> `(updated - sum) - adjusted` estimates the rounding error introduced by
> the current addition. For example, summing `1e16` followed by ten values
> equal to `1.0` loses those increments with a simple running sum, whereas
> this method recovers their combined contribution in ordinary double precision.
>
> Compensation improves many sums but does not guarantee an exact result.
> Preserve the order of operations: compiler options such as `-ffast-math`
> can reassociate expressions and invalidate the correction.

<details>
<summary>Worked example: recovering two small contributions</summary>

Consider the sum `1e16 + 1.0 + 1.0`. Its exact value is
`10000000000000002`. Near `1e16`, consecutive representable `double` values
are separated by 2. With the usual rounding to nearest, adding `1.0` to
`1e16` rounds back to `1e16`.

A simple running sum therefore loses both contributions:

```text
10000000000000000 + 1 → 10000000000000000
10000000000000000 + 1 → 10000000000000000
```

Now follow the variables in `kahan_sum`, starting with `sum = 0` and
`correction = 0`.

**1. Add `10000000000000000`.** This value is representable exactly:

```text
adjusted = values[i] - correction
    = 10000000000000000 - 0 = 10000000000000000

updated = sum + adjusted
    = 0 + 10000000000000000 = 10000000000000000

correction = (updated - sum) - adjusted
    = (10000000000000000 - 0) - 10000000000000000 = 0

sum = updated
    = 10000000000000000

```

**2. Add the first `1`.** The addition rounds back to the previous sum,
but the correction records the lost contribution:

```text
adjusted = values[i] - correction
    = 1 - 0 = 1

updated = sum + adjusted
    = 10000000000000000 + 1 → 10000000000000000

correction = (updated - sum) - adjusted
    = (10000000000000000 - 10000000000000000) - 1 = -1

sum = updated
    = 10000000000000000

```

**3. Add the second `1`.** Subtracting the correction combines this new
contribution with the one lost in the previous step:

```text
adjusted = values[i] - correction
    = 1 - (-1) = 2

updated = sum + adjusted
    = 10000000000000000 + 2 = 10000000000000002

correction = (updated - sum) - adjusted
    = (10000000000000002 - 10000000000000000) - 2 = 0

sum = updated
    = 10000000000000002

```

The increment of 2 is representable, so it enters the sum. The correction
returns to zero because this addition loses nothing.

To try it, add `#include <stdio.h>` at the top of the code containing
`kahan_sum`, then put this `main` after the function:

```c
int main(void) {
    double values[] = {1e16, 1.0, 1.0};
    double simple_sum = 0.0;

    for (int i = 0; i < 3; ++i) {
        simple_sum += values[i];
    }

    printf("Simple sum: %.0f\n", simple_sum);
    printf("Kahan sum:  %.0f\n", kahan_sum(values, 3));
    return 0;
}
```

Output:

```text
Simple sum: 10000000000000000
Kahan sum:  10000000000000002
```

</details>

Reduced units help keep numerical ranges manageable, but changing units alone
does not remove a large ratio between quantities.

## Subtracting nearly equal quantities

For large positive x, this difference loses accuracy:

```c
double x = 1e16;
double difference = sqrt(x + 1.0) - sqrt(x);  // 0 here.
```

The rounded square roots coincide, although their mathematical difference is
nonzero. Rewriting the expression avoids subtracting those nearly equal values:

```c
double difference = 1.0 / (sqrt(x + 1.0) + sqrt(x));  // About 5e-9.
```

This follows by multiplying numerator and denominator by the sum of the roots.
Look for stable formulations when the answer is much smaller than the terms
being subtracted.

## Integer division happens before assignment

**Problematic:**

```c
double energy = (1 / 2) * mass * speed * speed;  // Always zero.
```

Both operands of `1 / 2` are integers, so the result is zero. Assigning the
expression to a double does not recover the fraction.

**Correct:**

```c
double energy = 0.5 * mass * speed * speed;
```

Similarly, use `(double)accepted / attempted` for an acceptance fraction when
both counts are integers, after checking that `attempted > 0`.

## Compare approximate results with a chosen tolerance

For a finite reference value `expected`, testing `computed == expected` can
reject a useful result because of rounding. Combine an absolute and a relative
tolerance:

```c
double abs_tol = 1e-10;
double rel_tol = 1e-8;

if (isfinite(computed) &&
    fabs(computed - expected) <= abs_tol + rel_tol * fabs(expected)) {
    printf("Agreement within tolerance.\n");
}
```

The absolute term matters near zero; the relative term scales with the reference.
These numbers are illustrative: set them from the problem's units and accuracy
requirements. A tolerance is not proof that the physical model is correct.

## Avoid unnecessary overflow in intermediate calculations

Even when the final answer fits in a double, an intermediate square may not:

```c
double x = 1e200, y = 1e200;
double distance = sqrt(x * x + y * y);  // Overflows.
```

For a Euclidean norm, use the standard function:

```c
double distance = hypot(x, y);  // About 1.41421e200.
```

`hypot` avoids unnecessary overflow and underflow in its intermediate steps.
Include `<math.h>` for these mathematical functions and link with `-lm` on Linux.

Further reading: [Goldberg on floating-point arithmetic](https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html),
[GNU C arithmetic](https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Basic-Arithmetic.html),
and [the hypot manual](https://man7.org/linux/man-pages/man3/hypot.3.html).

## Check convergence and physical invariants

Start from cases whose behavior is known: an isolated force calculation, a
circular two-body orbit, or an integral with an analytical solution.

Reduce the time step while keeping the final physical time fixed. Compare
results as the step decreases. Check conserved quantities where applicable,
such as momentum in an isolated system, allowing for the accuracy of the method.

A run that finishes successfully can still calculate the wrong physics.
Use these checks alongside the [automated tests](3-Organizing_scientific_code.md#use-tests-to-check-and-preserve-behavior)
for individual functions and complete workflows.

For performance improvements, see
[Writing Efficient Scientific Code](4-Efficient_scientific_code.md).
Compare speed at the same required accuracy, and repeat the numerical checks
after each optimization.

For naming, comments, and readable formulas, see
[No programmer is an island](1-Introduction.md).
For test organization and reproducible runs, see
[Organizing a Scientific Code](3-Organizing_scientific_code.md).
