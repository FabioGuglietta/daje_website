# Bootstrap: Uncertainty of a Derived Quantity

Suppose a derived observable depends on two measured quantities:

$$
F=A\sin(B)+C,
$$

where C is a known offset. We estimate A and B from finite samples of
measurements. Their sample means have statistical uncertainty, so the result

$$
\widehat F=\bar A\sin(\bar B)+C
$$

also has uncertainty. Repeating the measurements would generally give different
means and therefore a different estimate of F. Reporting only one value of F
does not tell us how precisely it has been determined.

The goal is to estimate this uncertainty. We can calculate the value of
$\widehat F$ directly, but how much would it fluctuate if we repeated the
experiment? The sine makes the dependence on B nonlinear. Propagating errors
with derivatives is possible, but gives a local approximation.

The **bootstrap** approaches this question by repeatedly resampling the
available measurements and recalculating F. The spread of the results estimates
the standard error of $\widehat F$, without requiring a new set of measurements
or an explicit error-propagation formula.

## Resampling with replacement

Suppose four independent measurements give:

```text
Original sample:  1  2  3  4       mean = 2.5
```

A bootstrap sample is obtained by drawing four values from this sample
**with replacement**:

```text
Bootstrap sample: 4  2  2  1       mean = 2.25
```

Each draw selects an original observation with equal probability. A value can
appear several times, while others may not appear at all. The sample size
stays unchanged.

Repeat this procedure M times and calculate the mean of each resample.
The spread of these M means estimates the **standard error of the original
sample mean**. It describes uncertainty in the estimated mean, rather than
the spread of individual measurements.

The bootstrap reuses the measured values; it does not generate new independent
experimental data. The method described here assumes observations are independent
and representative of the underlying distribution. Correlated simulation time
series require a different treatment, such as block resampling.

## Exercise: propagate uncertainty through a nonlinear function

Two independent measurement series A and B are used to estimate the
dimensionless quantity

$$
F=\mu_A\sin(\mu_B)+C,
$$

where $\mu_A$ and $\mu_B$ are the population means. The angle B is in
radians, and the offset C = 0.5 is known exactly in this first part.

Using the sample means, calculate the estimate

$$
\widehat F=\bar A\sin(\bar B)+C.
$$

Write a C or Fortran program to estimate its standard error by bootstrap.
Apply the sine **after computing the mean of B**: this is a function of the means, not the average of
pointwise values $A_i\sin(B_i)$.

> [!NOTE]
> **A function of means and a mean of a function are different quantities.**
>
> In this exercise we estimate a quantity defined from mean parameters:
> $\widehat F=\bar A\sin(\bar B)+C$.
> If A and B instead describe the state of a system in each observation,
> the mean observable is
> $\overline F=N^{-1}\sum_i[A_i\sin(B_i)+C]$.
> In general these differ: averaging and applying a nonlinear function do
> not commute. Correlations between A and B can also affect the mean observable.
> Choose the expression from the physical question. The second exercise below
> considers the mean observable.

### Input data

Download [A.dat](bootstrap/A.dat) and [B.dat](bootstrap/B.dat). These are synthetic,
independent measurements with different sample sizes: NA = 500 and NB = 300.

Each file starts with its number of measurements, followed by one value per
line:

```text
N
value_1
value_2
...
value_N
```

Read each dataset into a dynamically allocated array. Calculate the original
means and $\widehat F$ before resampling.

### Bootstrap procedure

Use M = 2000 replicates. For each replicate r:

1. Draw NA indices independently and uniformly from 0 to NA - 1, with replacement.
   Use them to calculate the resampled mean $\bar A_r^*$.
2. Independently draw NB indices from 0 to NB - 1 and calculate $\bar B_r^*$.
3. Store
   $$
   F_r^*=\bar A_r^*\sin(\bar B_r^*)+C.
   $$

Always resample the **original** arrays. A and B are independent samples:
their indices do not form pairs, and their different lengths must be retained.

```text
READ A and B
CALCULATE the original means and F_hat

FOR r = 1 TO M
    sum_A = 0
    REPEAT NA TIMES
        DRAW an index from 0 to NA - 1
        sum_A += A[index]

    sum_B = 0
    REPEAT NB TIMES
        DRAW an independent index from 0 to NB - 1
        sum_B += B[index]

    F_bootstrap[r] = (sum_A / NA) * sin(sum_B / NB) + C
END FOR
```

Use floating-point variables for the sums. You can calculate a resampled mean
directly from the sampled indices without allocating another array of data.

### Calculate and report the uncertainty

Calculate the mean of the bootstrap estimates:

$$
\overline{F^*}=\frac{1}{M}\sum_{r=1}^{M}F_r^*,
$$

then their sample standard deviation:

$$
s_F=\sqrt{\frac{1}{M-1}
\sum_{r=1}^{M}\left(F_r^*-\overline{F^*}\right)^2}.
$$

Report the original estimate $\widehat F$ and its estimated standard error
$s_F$. Save the M bootstrap values to a text file, one per line, and plot
their histogram.

> [!NOTE]
> Do not divide $s_F$ by $\sqrt M$. The spread of the bootstrap values
> already estimates the standard error of the statistic. Increasing M improves
> the numerical precision of the bootstrap calculation; it does not increase
> the number of measurements in A or B.

Choose and record a random seed. Initialize the random generator once, before
the bootstrap loop, so the calculation can be reproduced.

Repeat with M = 500 and M = 5000 using the same input data. Compare the
estimated uncertainties; increasing M should make the estimate more stable,
rather than drive the uncertainty to zero.

## Second exercise: average an observable over measured states

Suppose each observation gives an amplitude $A_i$ and angle $B_i$ for the
same system realization. The observable for that realization is

$$
F_i=A_i\sin(B_i)+C.
$$

Estimate its population mean using

$$
\overline F=\frac1N\sum_{i=1}^N F_i,
$$

and calculate the standard error by bootstrap. Keep C = 0.5 known exactly.

### Paired input data

Download [AB_paired.dat](bootstrap/AB_paired.dat). The first line contains N = 300;
each following line contains one pair:

```text
N
A_1 B_1
A_2 B_2
...
A_N B_N
```

Rows represent independent realizations, but A and B **within a row are
correlated**. Keep them together. Do not construct pairs from the separate
`A.dat` and `B.dat` files used in the first exercise.

### Procedure

1. Calculate each $F_i$ from its original pair and compute $\overline F$.
2. For each of M = 2000 replicates, draw N row indices with replacement.
   Use the **same index** to select both A and B:
   $$
   \overline F_r^*=\frac1N\sum_{k=1}^N
   [A_{j_k}\sin(B_{j_k})+C].
   $$
3. Estimate the standard error as the sample standard deviation of the M
   values $\overline F_r^*$, again without dividing by $\sqrt M$.

You may calculate the $F_i$ array once and resample that array: it gives the
same result as selecting the original pairs and recalculating their observables.

### Compare the two definitions

On this **same paired dataset**, also calculate $\bar A\sin(\bar B)+C$.
Compare it with $\overline F$; they need not agree because they estimate
different quantities. The discrepancy is not automatically a bootstrap error.

For each paired bootstrap replicate, also compute
$\bar A_r^*\sin(\bar B_r^*)+C$ and estimate its standard error. Use the
same sampled rows for both means: independently resampling A and B here
would destroy their observed correlation.

Report both estimates and their uncertainties, identifying the definition
used for each.

## Extension to the first exercise: an uncertain offset

Now suppose C is an independent calibration estimate, $\widehat C=0.5$,
with standard uncertainty $s_C=0.02$. No measurement series for C is available.

Assume a Gaussian uncertainty model. For each bootstrap replicate, draw **one**
value

$$
C_r^*\sim\mathcal N(\widehat C,s_C^2),
$$

> [!NOTE]
> **Generating Gaussian values with the Box–Muller method**
>
> Draw two independent uniform random numbers $U_1,U_2$ in $(0,1)$.
> The transformation
> $$
> Z=\sqrt{-2\ln U_1}\cos(2\pi U_2)
> $$
> produces a standard normal variable: mean zero and variance one.
> For this exercise, use
> $$
> C_r^*=\widehat C+s_C Z.
> $$
> In C, the corresponding expression is:
> ```c
> double z = sqrt(-2.0 * log(u1)) * cos(2.0 * pi * u2);
> double c_star = c_hat + s_c * z;
> ```
> Include `<math.h>`, define `pi = acos(-1.0)`, and compile with `-lm`.
> If using `rand()`, generate each uniform value with
> `(rand() + 1.0) / (RAND_MAX + 2.0)`, keeping it away from both endpoints.
> In particular, $U_1$ must not be zero because $\ln(0)$ is undefined.
> Initialize the random generator once before the loop, not for each draw.
> Generate a new independent pair for each replicate and draw C independently
> of the resampling of A and B.

Then use

$$
F_r^*=\bar A_r^*\sin(\bar B_r^*)+C_r^*.
$$

This combines bootstrap resampling for A and B with Monte Carlo propagation
of the assumed uncertainty in C. The Gaussian model is an additional
assumption, and $s_C$ is the uncertainty of the offset itself, not the
spread of a new measurement dataset.

Repeat the standard-error calculation. Because the offset is independent and
additive, the squared uncertainty should increase by approximately $s_C^2$
relative to the known-offset case.
