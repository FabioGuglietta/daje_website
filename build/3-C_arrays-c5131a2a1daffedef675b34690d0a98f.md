We slightly modify the [example code](C_BASIC.md) written to numerically evaluate an integral with the trapezoid rule: 

```C
#include <stdio.h>
#include <math.h>

// Define the function to integrate: f(x) = x^3
double f(double x) {
    return pow(x, 3); // pow(a,b) computes a^b
}

// Trapezoidal rule for numerical integration
double trapezoidal_rule(double (*func)(double), double a, double b, int n, double x_values[], double f_values[]) {
    double p = (b - a) / n;                  // Width of each trapezoid
    double sum = 0.5 * (func(a) + func(b));  // End points contribution

    // Store the x values and function evaluations in arrays
    for (int i = 1; i < n; i++) {
        x_values[i] = a + i * p;
        f_values[i] = func(x_values[i]);  // Store the function evaluation
        sum += f_values[i];
    }

    return sum * p;
}

int main() {
    double a = 0.0;  // Lower limit of integration
    double b = 1.0;  // Upper limit of integration
    int n = 1000;    // Number of trapezoids (higher n for better accuracy)

    printf("This program performs numerical integration of f(x) = x^3 from a = %.2f to b = %.2f using %d trapezoids.\n", a, b, n);

    // Check if n is a valid number of trapezoids
    if (n > 0) {
        printf("The number of trapezoids is positive.\n");
    } else if (n < 0) {
        printf("Error: The number of trapezoids is negative.\n");
        return 1;
    } else {
        printf("Error: The number of trapezoids is zero.\n");
        return 1;
    }

    // Arrays to store x values and function evaluations at those points
    double x_values[n];  // Array to store x points
    double f_values[n];  // Array to store function evaluations f(x)

    // Perform numerical integration
    double result = trapezoidal_rule(f, a, b, n, x_values, f_values);

    // Print the result of the integration
    printf("The integral of f(x) = x^3 from %.2f to %.2f is approximately: %.5f\n", a, b, result);

    // Optionally, print out the x values and their corresponding f(x) values
    printf("x values and f(x) evaluations:\n");
    for (int i = 1; i < n; i++) {
        printf("x[%d] = %.5f, f(x[%d]) = %.5f\n", i, x_values[i], i, f_values[i]);
    }

    return 0;
}
```

Two arrays are now introduced in the `main()` function:

- `x_values[n]`: This array stores the $x$ values where the function $f(x)$ is evaluated during the integration.
- `f_values[n]`: This array stores the computed values of the function $f(x)$ at those $x$ values.


In C, an ***array*** is a collection of elements of the same type stored in contiguous memory locations. Arrays are useful when you need to store multiple values of the same type and access them using an index.

```type array_name[size];```

where `type` is the data type of the elements (e.g., `int`, `double`, `char`, etc.), `array_name` is the name of the array and `size` is the number of elements in the array. 

> [!WARNING]
> 
> - Indexing starts at 0: the first element is accessed using arr[0], not arr[1].
> - Accessing an index outside the array size (e.g., arr[5] when the array has only 5 elements) results in undefined behavior.

The function `trapezoidal_rule()` is modified to take two additional array parameters (`x_values[]` and `f_values[]`).

```double trapezoidal_rule(double (*func)(double), double a, double b, int n, double x_values[], double f_values[])```

Inside the for loop, each $x$ value is stored in `x_values[]`, and the corresponding function evaluation is stored in `f_values[]`.

```C
for (int i = 1; i < n; i++) {
    x_values[i] = a + i * p;
    f_values[i] = func(x_values[i]);  // Store the function evaluation
    sum += f_values[i];
}
```

After the integration, the code optionally prints the stored $x$ values and their corresponding function evaluations $f(x)$. 

```C
printf("x values and f(x) evaluations:\n");
for (int i = 1; i < n; i++) {
    printf("x[%d] = %.5f, f(x[%d]) = %.5f\n", i, x_values[i], i, f_values[i]);
}
```

and the output looks like
```
...
x[997] = 0.99700, f(x[997]) = 0.99103
x[998] = 0.99800, f(x[998]) = 0.99401
x[999] = 0.99900, f(x[999]) = 0.99700
```

## Two ways of declaring and using arrays: Static vs Dynamic Memory Allocation

In C, arrays can be allocated in two main ways:
1.	**Statically** — typically on the _stack_ — using direct declarations like `int arr[10];`
2.	**Dynamically** — on the _heap_ — using `malloc` or similar functions.

Each approach has pros and cons. Understanding the difference is crucial for writing efficient and safe programs.

### Static Allocation (Stack)

When you declare an array like this:
```C
int w[10];
```
the memory is allocated at **compile time** on the _stack_. Stack allocation is **fast** and requires **no manual cleanup**. However:
- The **size** must usually be **known** at compile time (or with some compilers, at runtime using variable-length arrays).
- The **stack has limited space**. This means that very large arrays can cause a **stack overflow**.

> [!NOTE]
> In Linux, the size of the stack available to a user is set by the `ulimit` command. To see how big a stack you can use `ulimit -s`
> Typically, the stack is limited to 8 MB, but you can allow your program to use the complete memory by using `ulimit -s unlimited`

### Dynamic Allocation (Heap)

Dynamic allocation allows you to **request memory** from the _heap_ at runtime using functions like `malloc`. This provides flexibility:
- You can allocate arrays of "any" size, determined during execution.
- You can **free** the memory when it’s no longer needed.
- You avoid the limitations of the stack.

However, heap allocation is slightly slower and you must manage it explicitly: **forgetting to call free** causes **memory leaks.**

## Example: Three Ways to Work with Memory in Functions

The example below demonstrates three common patterns:
- `f1`: dynamically allocates memory inside the function and returns the pointer.
- `f2`: takes a pointer to a pointer and allocates memory, modifying the original pointer.
- `f3`: receives a pointer to an existing array and writes into it.

Each function writes the value 100 into the first element of the memory it accesses.

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Allocates memory dynamically and returns the pointer
int *f1(int s) {
    int *v;
    v = malloc(s * sizeof(int));  // allocate s integers on the heap
    v[0] = 100;                   // store value at first position
    return v;                     // return the pointer
}

// Allocates memory dynamically and modifies the input pointer
void f2(int **v, int s) {
    *v = malloc(s * sizeof(int)); // allocate s integers on the heap
    (*v)[0] = 100;                // store value at first position
}

// Writes into a statically allocated array passed in from caller
void f3(int w[], int s) {
    w[0] = 100;  // modify the first element
}

int main(int argc, char **argv) {
    int s = 2;           // size of all arrays
    int *v1, *v2;        // pointers for dynamic memory
    int w[s];            // stack-allocated array (on the stack)

    v1 = f1(s);          // v1 now points to heap memory returned by f1
    f2(&v2, s);          // f2 allocates memory and updates v2
    f3(w, s);            // w is modified in-place (stack memory)

    // All three memory blocks should now have 100 in the first element
    printf("%d %d %d\n", v1[0], v2[0], w[0]);

    free(v1);  // important: release the heap memory
    free(v2);  // avoid memory leaks

    return 0;
}
```

This example shows that while all three techniques can be used to initialize memory, only dynamic memory (via `malloc`) gives you the freedom to allocate and manage large or variable-size arrays safely — at the cost of increased responsibility to free them.

> [!WARNING]
> Forgetting to call `free()` leads to memory leaks. For long-running programs or systems with limited RAM, this can severely degrade performance or cause crashes.

> [!NOTE]
> If you attempt to allocate **very large arrays** on the stack (e.g. `int w[10^7]`), you may hit a **segmentation fault**. For such cases, `malloc` is the safer choice.

### Array Size Management with `#define` vs `const int`

As we have already done, you can define array sizes with preprocessor macros. 
Another option is tu use constants:

```c
#define SIZE 100 
int arr1[SIZE];

const int SIZE_CONST = 100;
int arr2[SIZE_CONST];  // Works in C99 and later
```
<!---
> [!WARNING]
> TODO add discussion on C standards somewhere in the beginning/end
--->

- `#define` is resolved at preprocessing time.
- `const int` offers type checking and scope control.

> [!NOTE]
> `const int` may not work in all compilers for array sizing unless C99 or later is enabled.


## Array initialisation

Initializing an array means assigning initial values to its elements at the time of declaration. Proper initialization is crucial to avoid unpredictable behavior caused by garbage values.

When you declare an array, you can initialize it in several ways:

1. **Full Initialization:** you provide explicit values for all elements:\
    ```int arr[3] = {1, 2, 3};  // Array of 3 integers, initialized to {1, 2, 3}```
1. **Partial Initialization:** you provide values for some elements. The remaining elements are automatically initialized to 0. \
   ```int arr[5] = {1, 2};  // Array of 5 integers, initialized to {1, 2, 0, 0, 0}```
1. **Zero Initialization:** if you initialize the first element to 0, all other elements are also set to 0.\
   ```int arr[4] = {0};  // Array of 4 integers, initialized to {0, 0, 0, 0}```

> [!WARNING]
> In C, arrays **are not automatically initialized** when declared. If you declare an array without explicitly initializing it, the array elements will contain **garbage values** (random values left in memory). This can lead to unpredictable behavior in your program.
> **Remember to always initialise them!**

Here an example of arrays declaration and initialisation:

```C
#include <stdio.h>

int main() {
    float a[3];              	   // Uninitialized float array of size 3
    int b[5], c[5];          	   // Uninitialized int arrays of size 5
    float d[3] = {1.2, 2.0, 3.7};  // Initialized float array of size 3
    int e[7] = {1};                // Initialized int array of size 7 (first element is 1, rest are 0)

    return 0;
}
```

## Common Pitfalls When Using Arrays

C arrays are powerful but come with caveats. Here are some common mistakes:

### 1. Out-of-Bounds Access
Accessing elements beyond the array limits leads to undefined behavior. In some cases, it may appear to work, but it may also crash the program or corrupt data.
```c
int arr[5] = {1, 2, 3, 4, 5};
printf("Valid: %d\n", arr[4]);    // Defined behavior
printf("Invalid: %d\n", arr[5]);  // Undefined behavior: could crash or print garbage
```
**Possible output:**
```
Valid: 5
Invalid: -183942
```
Or:
```
Segmentation fault (core dumped)
```

### 2. Uninitialized Arrays
Arrays declared without explicit initialization contain garbage values.
```c
int arr[3];
for (int i = 0; i < 3; i++) {
    printf("arr[%d] = %d\n", i, arr[i]);
}
```
**Possible output:**
```
arr[0] = 32767
arr[1] = 0
arr[2] = -1938123
```
Values depend on leftover memory content.

### 3. Misusing `sizeof` Inside Functions
The `sizeof` operator returns the size of a pointer when used on array parameters.
```c
#include <stdio.h>

void print_size(int arr[]) {
    printf("Size: %lu\n", sizeof(arr));  // Typically 8 bytes on 64-bit systems
}

int main() {
    int data[10];
    print_size(data);
    return 0;
}
```
**Output:**
```
Size: 8
```
To get the actual size, pass it as a separate argument:
```c
void print_size_fixed(int arr[], int size) {
    printf("Size: %d\n", size);
}
```

## `const` Arrays and Parameters

Use `const` to prevent functions from modifying arrays:
```c
void print_readonly(const double arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%.2f\n", arr[i]);
}
```

This allows the compiler to optimize and avoids accidental changes. 

> [!WARNING]
> `const` applies only at the level declared. Use `const double *const arr` for a truly immutable pointer.
