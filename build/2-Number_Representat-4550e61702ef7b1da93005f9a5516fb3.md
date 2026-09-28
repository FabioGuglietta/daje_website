# How Numbers Are Represented in a Modern CPU (and Why You Should Care)

Understanding how numbers are represented in hardware is essential for anyone working close to the metal — and even more so in high-performance computing (HPC). This unit introduces the basic building blocks of numerical representation in modern CPUs, starting from integers to floating-point numbers, and discusses the caveats that every scientific programmer must understand.

---

## 1. Number Bases: Decimal, Binary, and Hexadecimal

Before diving into data representation, it's important to understand how different **numerical bases** are used in computing.

### Decimal (Base 10)

* This is the number system we use every day.
* It uses 10 digits: 0 through 9.
* Each position represents a power of 10.
* Example: `745 = 7×10² + 4×10¹ + 5×10⁰`

### Binary (Base 2)

* The fundamental language of computers.
* Only two digits: 0 and 1.
* Each position represents a power of 2.
* Example: `1101₂ = 1×2³ + 1×2² + 0×2¹ + 1×2⁰ = 13₁₀`

### Hexadecimal (Base 16)

* Often used as a shorthand for binary because it's more compact.
* Uses digits 0–9 and letters A–F to represent values 10–15.
* Each hex digit represents **4 binary bits**.
* Example: `0x3F = 0011 1111₂ = 63₁₀`

Hex is frequently used in programming (e.g., memory addresses, bit masks) and debugging.

## 2. Bits, Bytes, and Words

### Bit

The **bit** (short for "binary digit") is the smallest unit of data in computing. A bit can take only two values: `0` or `1`.

### Byte

A **byte** is a group of 8 bits. A single byte can represent 2⁸ = 256 different values. This is the fundamental addressable unit in modern computer memory.

### Word

A **word** is the natural data size handled by a CPU. On modern 64-bit machines (e.g., most Intel/AMD architectures), a word is 64 bits = 8 bytes.


## 3. Integer Representation

Integer numbers are stored in binary form. That means every number is represented using powers of two.

### Unsigned Integers

For unsigned integers, all bits are used to represent the magnitude:

| Bits | Range                           |
| ---- | ------------------------------- |
| 8    | 0 to 255                        |
| 16   | 0 to 65,535                     |
| 32   | 0 to 4,294,967,295              |
| 64   | 0 to 18,446,744,073,709,551,615 |

### Signed Integers (Two's Complement)

Signed integers use **two's complement** representation:

* The **most significant bit (MSB)** is used as the sign bit.
* The range becomes asymmetrical: one more negative value than positive.

For example, in 8-bit signed integers:

| Value | Binary   |
| ----- | -------- |
| 0     | 00000000 |
| 1     | 00000001 |
| -1    | 11111111 |
| -128  | 10000000 |

In two's complement, negative values are obtained by inverting the bits of the absolute value and adding one.


## 4. Endianness

When integers (or any binary data) are stored in memory, the **byte order** matters. This is called **endianness**.

### Little Endian vs Big Endian

* **Little endian** stores the **least significant byte first**.
* **Big endian** stores the **most significant byte first**.

For example, the 32-bit hexadecimal number `0x12345678` would be stored as:

| Address | Little Endian | Big Endian |
| ------- | ------------- | ---------- |
| 0x00    | 0x78          | 0x12       |
| 0x01    | 0x56          | 0x34       |
| 0x02    | 0x34          | 0x56       |
| 0x03    | 0x12          | 0x78       |

### Intel Architecture

Intel x86 and x86\_64 processors use **little-endian** representation. This means when you view memory on an Intel CPU, the least significant bytes appear first.

Endianness matters when dealing with binary file I/O, networking (which uses big endian), and cross-platform data serialization.


# Real Number Representations

Numerical computing depends critically on how numbers are represented in hardware. In high-performance computing (HPC), this isn't just a side note — it's a source of bugs, performance bottlenecks, and surprising behavior. In this unit, we’ll explore how real numbers are stored using the IEEE-754 floating-point standard, examine common pitfalls of finite precision, and learn how to write safer, faster numerical code.



## 1. Precision Levels in Modern Systems

Most CPUs follow the **IEEE-754** standard to represent floating-point numbers. Modern 64-bit systems support multiple levels of precision, each with its own speed, memory cost, and numerical range.


| Type | Total Bits | Sign | Exponent | Fraction | Approx Decimal Digits | Range |
| - | - | - | - | - | - | - |
| **Half (binary16)**   | 16         | 1    | 5        | 10       | \~3–4                  | \~±6.10e±5   |
| **Single (binary32)** | 32         | 1    | 8        | 23       | \~6–7                  | \~±3.40e±38  |
| **Double (binary64)** | 64         | 1    | 11       | 52       | \~15–16                | \~±1.79e±308 |

> **Double precision** is the default in most scientific and HPC applications.
> **Half precision** is increasingly used in AI/ML workloads for speed and memory efficiency.



## 2. IEEE-754 Storage Layout

The IEEE-754 standard breaks each floating-point number into three fields:

| Field            | Role                              |
| -                | -                                 |
| **Sign bit (S)** | Determines positive or negative   |
| **Exponent (E)** | Scales the number by a power of 2 |
| **Mantissa (M)** | Holds the significant digits      |

The number is computed as:

```text
(-1)^S × 1.M × 2^(E - Bias)
```

### ASCII Diagrams

```text
Double Precision (64-bit)
++-+-+
| S |      Exponent     |                         Mantissa                         |
| 1 |       11 bits     |                         52 bits                          |
++-+-+

Single Precision (32-bit)
++--++
| S | Exponent  |            Mantissa             |
| 1 | 8 bits    |           23 bits               |
++--++

Half Precision (16-bit)
++--++
| S | Expt   | Mantissa   |
| 1 | 5 bits | 10 bits    |
++--++
```



## 3. Common Issues in Floating-Point Arithmetic

### 3.1 Inexact Decimal Representation

Many decimal numbers (e.g., `0.1`, `0.2`) **cannot be represented exactly** in binary.

```c
#include <stdio.h>

int main() {
    float a = 0.1f;
    float b = 0.2f;
    printf("a + b = %.17f\n", a + b);
}
```

**Output:**

```
a + b = 0.30000001192092896
```

Even though the result "looks" like 0.3, it isn’t exactly — and `a + b == 0.3f` would return `false`.



### 3.2 Loss of Associativity

Floating-point arithmetic is **not associative**:

```c
#include <stdio.h>

int main() {
    double a = 1e16;
    double b = -1e16;
    double c = 1.0;

    printf("(a + b) + c = %.1f\n", (a + b) + c);
    printf("a + (b + c) = %.1f\n", a + (b + c));
}
```

**Output:**

```
(a + b) + c = 1.0
a + (b + c) = 0.0
```

Why? Because `(a + b)` is zero (due to cancellation), while `(b + c)` adds `1.0` to a huge negative number, losing it entirely.



### 3.3 Subnormal Numbers (Denormals)

Subnormal numbers allow representing very small values near zero — but at a **huge performance cost**.

```c
#include <stdio.h>
#include <float.h>

int main() {
    printf("FLT_MIN = %e\n", FLT_MIN);
    printf("FLT_TRUE_MIN (subnormal) = %e\n", FLT_TRUE_MIN);
}
```

> On some CPUs, subnormals are 100× slower to handle. Compilers may flush them to zero to avoid this.



### 3.4 Overflow and Underflow

```c
#include <stdio.h>
#include <float.h>

int main() {
    float big = FLT_MAX * 2.0f;
    float small = FLT_MIN / 2.0f;

    printf("Overflow: %f\n", big);
    printf("Underflow: %f\n", small);
}
```

**Output:**

```
Overflow: inf
Underflow: 0.000000
```

### 3.5 NaNs Are Contagious but Not Comparable

```c
#include <stdio.h>
#include <math.h>

int main() {
    double x = 0.0 / 0.0;
    printf("x == x? %d\n", x == x);  // false!
}
```

NaNs are special: they're unordered, and `x != x` is always true for NaN.



### 3.6 Unsafe Comparisons

Instead of this:

```c
if (a == b) { ... }  // fragile!
```

Use an **epsilon-based** comparison:

```c
#include <math.h>
#define EPSILON 1e-9

if (fabs(a - b) < EPSILON * fmax(fabs(a), fabs(b))) {
    // a and b are approximately equal
}
```


## 4. Why Machine Epsilon Matters More Than the Smallest Number

While it's interesting to know the **smallest representable number** (e.g., `~4.94 × 10⁻³²⁴` for subnormals), this value is rarely useful in practical computation. In contrast, the **smallest *meaningful difference*** you can represent near a number — known as **machine epsilon** — is critically important for numerical stability.

### What Is Machine Epsilon?

Machine epsilon, denoted `ε`, is the smallest number such that:

```
1.0 + ε ≠ 1.0
```

For IEEE-754 **double precision**, this value is:

```
ε = 2⁻⁵² ≈ 2.220446049250313 × 10⁻¹⁶
```

This defines the **relative resolution** of the floating-point system. You can't reliably distinguish between two numbers unless they differ by more than `ε × |x|`.

## 6. Roundoff and the Limits of Precision

### 6.1 Basic Case: The Smallest Difference That Matters

Floating-point numbers have **limited resolution**: there is a smallest change you can make to a number before it becomes indistinguishable due to rounding. This is defined by **machine epsilon**, denoted ε.

For IEEE-754 double precision:

```text
ε ≈ 2⁻⁵³ ≈ 1.110223 × 10⁻¹⁶
```

This means:

* The smallest number such that `1.0 + ε ≠ 1.0` is approximately `1.110223 × 10⁻¹⁶`
* Anything smaller may be **rounded away**

#### Example

```c
#include <stdio.h>

int main() {
    double x = 1.0;
    double y1 = 1e-16;
    double y2 = 1e-17;

    printf("1.0 + 1e-16 = %.17g\n", x + y1); // 1.0000000000000001
    printf("1.0 + 1e-17 = %.17g\n", x + y2); // 1.0
}
```

**Interpretation**:

* `1.0 + 1e-16` changes the result: it’s just barely large enough to affect the binary representation.
* `1.0 + 1e-17` gets **rounded back to 1.0**.

This illustrates that floating-point addition is not infinitely precise — the error threshold is **relative** to the number's size.

### Advanced Example: When Small Differences Do Show Up

You might assume that **any difference smaller than ε × x** should always be invisible in floating-point arithmetic. But that’s not quite true.

```c
double x = 1e-32;
double y = 1e-16;
double z = (x + x*y) - x;
```

Although `x*y = 1e-48` is smaller than machine epsilon relative to `x` (`~2.2e-48`), the final result:

```text
(x + x*y) - x ≈ 1.37e-48
```

is **nonzero** and clearly visible in the output.

#### Why?

* Floating-point operations round to the nearest **representable number**, not based strictly on ε × x.
* Even a very small change — *below the expected resolution* — may land in the next binary float.
* In this case, rounding pushed `x + x*y` **upward**, and subtraction recovered that shift.

#### Bit-Level Explanation

* `x = 1e-32` → stored as a normal float with exponent ≈ -107
* `x*y = 1e-48` → small but still exactly representable
* `x + x*y` gets rounded **just enough** to become a different number than `x`
* So `(x + x*y) - x` = the rounding delta — **not** exactly `x*y`

To truly understand what's going on inside a floating-point computation, you often need to look past decimal approximations. The `%f` or `%e` format specifiers in C show only a rounded view of what's stored in memory. If you want to see the exact value a `double` holds, down to the last binary bit, you should use the `%a` format specifier.

The `%a` output prints floating-point numbers in hexadecimal scientific notation, of the form:

```
0x1.<mantissa>p<exponent>
```

This directly reflects the internal IEEE-754 binary structure: one bit for the sign, an 11-bit exponent, and a 52-bit mantissa (with an implicit leading 1).

Let’s use this to examine a subtle case discussed earlier: computing `(x + x*y) - x` when `x = 1e-32` and `y = 1e-16`. At first glance, it seems the result should be nearly zero—after all, `x*y = 1e-48` is smaller than the machine epsilon times `x` (which is around `2.2e-48`). So we might expect the sum `x + x*y` to be rounded back to `x`. But that’s not what happens.

Consider the following code:

```c
#include <stdio.h>

int main() {
    double x = 1e-32;
    double y = 1e-16;
    double xy = x * y;
    double sum = x + xy;
    double diff = sum - x;

    printf("x        = %.17e = %a\n", x, x);
    printf("x * y    = %.17e = %a\n", xy, xy);
    printf("x + x*y  = %.17e = %a\n", sum, sum);
    printf("(x + x*y) - x = %.17e = %a\n", diff, diff);

    return 0;
}
```

When run on a system using IEEE-754 double precision (as virtually all modern CPUs do), this produces output similar to:

```
x        = 1.00000000000000006e-32 = 0x1.afb4ee5e8f9f1p-107
x * y    = 1.00000000000000004e-48 = 0x1.0000000000000p-159
x + x*y  = 1.00000000000001373e-32 = 0x1.afb4ee5e8fa7fp-107
(x + x*y) - x = 1.37334404724997856e-48 = 0x1.8000000000000p-159
```

What’s striking here is that `x + x*y` results in a value that is **not** equal to `x`, even though the added quantity `x*y` is smaller than the precision threshold you'd expect to matter. The `%a` output shows us why: the sum was rounded **upward** to the next representable double. The mantissa changed slightly—from `0xafb4ee5e8f9f1` to `0xafb4ee5e8fa7f`—even though the added value was tiny.

And when we subtract `x` from the rounded-up sum, we recover a value that is not `1e-48` (which would be `0x1.0p-159`), but a slightly larger number: `0x1.8p-159`, or `1.5 × 2⁻¹⁵⁹`. This result didn’t appear because the arithmetic was precise; it appeared because the rounding pushed us just over the boundary into a new representable number.

So, even though the added value `x*y` is smaller than machine epsilon times `x`, the sum `x + x*y` did not round back to `x`. The binary representation had just enough room for the change to "take effect," nudging the result into a new double value. `%a` exposes that subtle shift exactly.

This example illustrates that floating-point arithmetic does not follow a simple threshold model. You can't always assume that "less than epsilon" means "invisible." What matters is how close the result lands to the nearest representable float—and whether it crosses the rounding boundary.

Using `%a` gives you a direct lens into this behavior. It’s an invaluable tool for debugging numerical code, validating rounding behavior, and teaching students how computers actually interpret and store real numbers.



## 5. Roundoff and the Limits of Precision

### 5.1 Basic Case: The Smallest Difference That Matters

Floating-point numbers have **limited resolution**: there is a smallest change you can make to a number before it becomes indistinguishable due to rounding. This is defined by **machine epsilon**, denoted ε.

For IEEE-754 double precision:

```text
ε ≈ 2⁻⁵³ ≈ 1.110223 × 10⁻¹⁶
```

This means:

* The smallest number such that `1.0 + ε ≠ 1.0` is approximately `1.110223 × 10⁻¹⁶`
* Anything smaller may be **rounded away**

#### Example

```c
#include <stdio.h>

int main() {
    double x = 1.0;
    double y1 = 1e-16;
    double y2 = 1e-17;

    printf("1.0 + 1e-16 = %.17g\n", x + y1); // 1.0000000000000001
    printf("1.0 + 1e-17 = %.17g\n", x + y2); // 1.0
}
```

**Interpretation**:

* `1.0 + 1e-16` changes the result: it’s just barely large enough to affect the binary representation.
* `1.0 + 1e-17` gets **rounded back to 1.0**.

This illustrates that floating-point addition is not infinitely precise — the error threshold is **relative** to the number's size.

TODO: add link to advanced section





## 6. Summation Strategies and Trade-Offs

Naive floating-point summation accumulates error linearly. Better strategies exist:

### 6.1 Pairwise Summation

Breaks the list into halves recursively, sums each, and combines the result.

```c
double pairwise_sum(double *arr, int start, int end) {
    if (end - start == 1) return arr[start];
    if (end - start == 0) return 0.0;

    int mid = (start + end) / 2;
    return pairwise_sum(arr, start, mid) + pairwise_sum(arr, mid, end);
}
```

* **Error grows as O(log n)**
* **Parallelizable** and **fast**



### 6.2 Kahan Summation

Tracks lost bits using a compensation variable:

```c
double kahan_sum(const double *a, int n) {
    double sum = 0.0;
    double c = 0.0;
    for (int i = 0; i < n; i++) {
        double y = a[i] - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}
```

* More accurate than naive summation
* Not always better than pairwise
* **Serial-only**, slower



### 6.3 Sorted Summation

Sort values by absolute value before summing:

```c
int compare(const void *a, const void *b) {
    double x = fabs(*(double*)a), y = fabs(*(double*)b);
    return (x > y) - (x < y);
}
```

* Helps when values vary in magnitude
* Useful in accumulation-heavy loops



## 7. Best Practices Summary

* **Understand your precision**: use `double` unless you know why not.
* **Avoid `==`** for floating points — always use tolerance.
* **Watch out** for compiler flags like `-ffast-math`, which break IEEE-754 rules.
* Prefer **pairwise summation** in performance-sensitive reductions.
* Use math libraries (BLAS, LAPACK, MKL) whenever possible — they're optimized and safe.
* **Test for NaNs, Infs, and edge cases** regularly.
