# Multidimensional arrays

Multidimensional arrays in C are arrays of arrays. They allow you to store data in a table-like structure, such as matrices or grids. 

```c 
type array_name[size1][size2][...];
```

An example of a 2D array:

```c
int matrix[3][4];  // Array of 3 rows and 4 columns
```

It can be initialised either with a *full initialisation*:

```c
	int matrix[2][3] = {
	{1, 2, 3},
	{4, 5, 6}
	};
```

or with a *partial initialisation* (remaining elements are set to 0):

```c
int matrix[2][3] = {
    {1, 2}
}; 
```

Example:

```c
#include <stdio.h>

int main() {
    int matrix[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    // Print the 2D array
    printf("Matrix:\n");
    for (int i = 0; i < 2; i++) {         // Loop through rows
        for (int j = 0; j < 3; j++) {     // Loop through columns
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
```


## Pointers to rows

For `int matrix[2][3]`, each element of the outer array is a row of three
integers. In most expressions, `matrix` converts to a **pointer to its first
row**, not to an `int **`:

```c
int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
int (*row)[3] = matrix;
```

The parentheses matter: `int (*row)[3]` declares a pointer to a row, whereas
`int *row[3]` declares an array of three pointers.

- `matrix + i` points to row `i`; advancing by one row spans `3 * sizeof(int)` bytes.
- `*(matrix + i)` designates that row and converts to a pointer to its first integer when used in the next addition.
- `*(matrix + i) + j` points to element `j` within the row.
- `*(*(matrix + i) + j)` is equivalent to `matrix[i][j]`.

Both indices must stay within their respective bounds. Writing the pointer
expression explicitly does not make access faster than indexing.

## Dynamically allocated matrices: two different layouts

Suppose a simulation stores three velocity components for each of `N` particles.
There are two useful representations, with different pointer types and layouts.

### One contiguous allocation

A pointer to a row preserves the contiguous layout and supports `velocity[i][j]`:

```c
#include <stdio.h>
#include <stdlib.h>

static void initialize(float (*velocity)[3], size_t n) {
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < 3; ++j)
            velocity[i][j] = (float)(3 * i + j);
}

int main(void) {
    const size_t n = 10;
    float (*velocity)[3] = malloc(n * sizeof *velocity);
    if (velocity == NULL) return EXIT_FAILURE;

    initialize(velocity, n);
    printf("Last component: %.1f\n", velocity[n - 1][2]);
    free(velocity);
    return EXIT_SUCCESS;
}
```

`sizeof *velocity` is the size of one complete row. A single allocation stores
all rows consecutively, and a single `free` releases it. The output is
`Last component: 29.0`. For a runtime size obtained from input, validate it and
check that the byte-count multiplication fits in `size_t` before allocating.

### An array of row pointers

A `float **` can instead point to an array of pointers, each referring to a
separately allocated row:

```c
#include <stdio.h>
#include <stdlib.h>

static void initialize(float **velocity, size_t n, size_t columns) {
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < columns; ++j)
            *(*(velocity + i) + j) = (float)(i * columns + j);
}

int main(void) {
    const size_t n = 10, columns = 3;
    float **velocity = malloc(n * sizeof *velocity);
    if (velocity == NULL) return EXIT_FAILURE;

    for (size_t i = 0; i < n; ++i) {
        velocity[i] = malloc(columns * sizeof *velocity[i]);
        if (velocity[i] == NULL) {
            for (size_t k = 0; k < i; ++k) free(velocity[k]);
            free(velocity);
            return EXIT_FAILURE;
        }
    }

    initialize(velocity, n, columns);
    printf("Last component: %.1f\n", velocity[n - 1][2]);
    for (size_t i = 0; i < n; ++i) free(velocity[i]);
    free(velocity);
    return EXIT_SUCCESS;
}
```

Here `velocity + i` points to a slot containing a row pointer. Dereferencing
that slot retrieves the pointer used to locate the row's elements. Each row
is contiguous internally, but separate rows are **not guaranteed to be adjacent**.
This representation also allows rows of different lengths if the program
tracks those lengths separately.

In C, `malloc` returns `void *`, which converts to an object pointer without
an explicit cast. Include `<stdlib.h>` and check for allocation failure.

Although both representations permit `velocity[i][j]`, their types are not
interchangeable. A function expecting `float **` cannot receive a contiguous
`float matrix[N][3]`; casting the argument does not repair the layout mismatch.
For a rectangular numerical grid, one contiguous allocation is often a useful
default because it avoids separate row allocations and supports sequential access.

## Memory Layout and Access Patterns

C uses **row-major order** for multi-dimensional arrays. This affects how efficiently memory is accessed:

```c
int matrix[100][100] = {0};

for (int i = 0; i < 100; i++)      // Row-wise: consecutive elements
    for (int j = 0; j < 100; j++)
        matrix[i][j]++;

for (int j = 0; j < 100; j++)      // Column-wise: a larger stride
    for (int i = 0; i < 100; i++)
        matrix[i][j]++;
```

> [!WARNING]
> TODO add a link to a short snippet to show how to get the timings in C. START ORGANIZING A SMALL LIBRARY OF SNIPPETS


## Multidimensional arrays as input parameters for functions

Let's look a the following code:

```c
#include <stdio.h>

#define ROWS 3
#define COLS 4

// Function that takes a 2D array as a parameter
void print_array(int arr[ROWS][COLS]) {
    printf("Array elements:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

// Function to increment each element by 1
void increment_array(int arr[][COLS], int rows_local) {
    for (int i = 0; i < rows_local; i++) {
        for (int j = 0; j < COLS; j++) {
            arr[i][j] += 1;
        }
    }
}

int main() {
    // Initialize a 2D array
    int arr[ROWS][COLS] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Call the function to print the array
    print_array(arr);

    // Increment the elements
    increment_array(arr, ROWS);

    // Call the function again to print the modified array
    printf("\nAfter incrementing each element by 1:\n");
    print_array(arr);

    return 0;
}
```

### define
In C programming, `#define` is a preprocessor directive used to create symbolic constants or macros. It allows you to define names or constants that will be replaced by their corresponding values or expressions during the preprocessing stage, before the actual compilation of the code. This can help improve code readability, maintainability, and flexibility.

In our example:

```c
#define ROWS 3
#define COLS 4
```

we are defining the number of rows and columns for the 2D array. Whenever ROWS (or COLS) appears in the code, it gets replaced with the value 3 (or 4).

### function definition

When defining a function like:
```c
void increment_array(int arr[][COLS], int rows_local)
```
you need to include one set of square brackets [] for each dimension of the array. For example, a 3D array would be passed as:
```c
void function(int arr[][M][L], int N)
```
The size of the first dimension can be left empty, but the sizes of all subsequent dimensions (from the second one onward) must be explicitly declared. Since the size of the first dimension is not automatically known inside the function, you must pass it as an additional argument.

An array parameter such as `int arr[][COLS]` is adjusted to
`int (*arr)[COLS]`: a pointer to a row of `COLS` integers. Writing `ROWS` in
the first pair of brackets does not pass or enforce a row count. In the
example, `print_array` uses the macro as an agreed fixed size, while
`increment_array` receives the number of rows explicitly. For reusable
functions, pass the row count and ensure that the actual row width matches
the parameter type.
