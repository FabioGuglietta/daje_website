# Multidimensional arrays

Multidimensional arrays in C are arrays of arrays. They allow you to store data in a table-like structure, such as matrices or grids. 

```C 
type array_name[size1][size2][...];
```

An example of a 2D array:

```C
int matrix[3][4];  // Array of 3 rows and 4 columns
```

It can be initialised either with a *full initialisation*:

```C
	int matrix[2][3] = {
	{1, 2, 3},
	{4, 5, 6}
	};
```

or with a *partial initialisation* (remaining elements are set to 0):

```C
int matrix[2][3] = {
    {1, 2}
}; 
```

Example:

```C
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


## Memory Layout and Access Patterns

C uses **row-major order** for multi-dimensional arrays. This affects how efficiently memory is accessed:

```c
int matrix[100][100];

for (int i = 0; i < 100; i++)      // Fast: row-wise access
    for (int j = 0; j < 100; j++)
        matrix[i][j]++;

for (int j = 0; j < 100; j++)      // Slower: column-wise access
    for (int i = 0; i < 100; i++)
        matrix[i][j]++;
```

> [!WARNING]
> TODO add a link to a short snippet to show how to get the timings in C. START ORGANIZING A SMALL LIBRARY OF SNIPPETS


## Multidimensional arrays as input parameters for functions

Let's look a the following code:

```C
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

```C
#define ROWS 3
#define COLS 4
```

we are defining the number of rows and columns for the 2D array. Whenever ROWS (or COLS) appears in the code, it gets replaced with the value 3 (or 4).

### function definition

When defining a function like:
```C
void increment_array(int arr[][COLS], int rows_local)
```
you need to include one set of square brackets [] for each dimension of the array. For example, a 3D array would be passed as:
```C
void function(int arr[][M][L], int N)
```
The size of the first dimension can be left empty, but the sizes of all subsequent dimensions (from the second one onward) must be explicitly declared. Since the size of the first dimension is not automatically known inside the function, you must pass it as an additional argument.

In our example, the variable rows_local was used for instructional purposes. However, a better approach would be to follow the method used in the print_array function, where macros are utilized for clarity:
```C
void print_array(int arr[ROWS][COLS])
```
This way, ROWS and COLS are predefined constants (by using `#define`), which makes the code more readable and maintainable.
