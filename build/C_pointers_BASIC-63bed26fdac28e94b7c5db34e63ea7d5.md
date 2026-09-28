# Pointers

We propose the third variation of the [example code](C_BASIC.md). In particular, this is a variation of the [code](C_arrays_BASIC.md) in which arrays where used. Indeed, we now read the number of the trapezoids from the command line, and we ***dynamically*** allocate the memory to store `x_values` and `f_values` (if you didn't read the previous examples, don't worry: you will easliy understand what these varaibles are):


```C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define the function to integrate: f(x) = x^3
double f(double x) {
    return pow(x, 3); // pow(a,b) computes a^b
}

// Trapezoidal rule for numerical integration
double trapezoidal_rule(double (*func)(double), double a, double b, int n, double *x_values, double *f_values) {
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

int main(int argc, char *argv[]) {
    double a = 0.0;  // Lower limit of integration
    double b = 1.0;  // Upper limit of integration

    // Ensure there is at least one command line argument for the number of trapezoids
    if (argc < 2) {
        fprintf("Usage: %s <number_of_trapezoids>\n", argv[0]);
        return 1;
    }

    // Read the number of trapezoids from the command line
    int n = atoi(argv[1]);

    // Check if n is a valid number of trapezoids
    if (n <= 0) {
        fprintf("Error: The number of trapezoids must be a positive integer.\n");
        return 1;
    }

    printf("This program performs numerical integration of f(x) = x^3 from a = %.2f to b = %.2f using %d trapezoids.\n", a, b, n);

    // Allocate memory for x_values and f_values dynamically
    double *x_values = malloc(n * sizeof(double));
    double *f_values = malloc(n * sizeof(double));
    if (x_values == NULL || f_values == NULL) {
        fprintf("Memory allocation failed\n");
        return 1;
    }

    // Perform numerical integration
    double result = trapezoidal_rule(f, a, b, n, x_values, f_values);

    // Print the result of the integration
    printf("The integral of f(x) = x^3 from %.2f to %.2f is approximately: %.5f\n", a, b, result);

    // Optionally, print out the x values and their corresponding f(x) values
    printf("x values and f(x) evaluations:\n");
    for (int i = 1; i < n; i++) {
        printf("x[%d] = %.5f, f(x[%d]) = %.5f\n", i, x_values[i], i, f_values[i]);
    }

    // Free allocated memory
    free(x_values);
    free(f_values);

    return 0;
}
```

Let's look at the differences w.r.t. the [code that used the arrays](C_arrays_BASIC.md).

## Read command line arguments: `argc` and `argv`


In C, `argc` and `argv` are used to handle command line arguments passed to a program. They provide a way to pass information from the command line when starting the program.

- `argc` (***Argument Count***):
	- Definition: `argc` is an integer that holds the number of command line arguments passed to the program, including the program’s name.
	- Example: If a program is executed with the command `./program arg1 arg2`, `argc` will be 3 (the program name and two arguments).
- `argv` (***Argument Vector***):
	- Definition: `argv` is an array of strings (character arrays) where each string is one of the command line arguments.
	- Example: For the same command `./program arg1 arg2`, `argv[0]` will be `"./program"`, `argv[1]` will be `"arg1"`, and `argv[2]` will be `"arg2"`.

In our example: 

```C
int main(int argc, char *argv[]) {
	...
    // Ensure there is at least one command line argument for the number of trapezoids
    if (argc < 2) {
        fprintf("Usage: %s <number_of_trapezoids>\n", argv[0]);
        return 1;
    }
```

Assuming the executable is called `test`, the code must be run as `./test <n>`, where n is the number of trapeziods (for example, `./test 1000` to use `n=1000` trapezoids).

The if statement (`if (argc < 2)`) checks if the correct number of arguments has been provided. 

## Convert ASCII to integer: `atoi`

In C, `atoi` (ASCII to Integer) is a standard library function used to convert a string representing a number into its integer value. It is part of the `stdlib.h` library:

```C
int atoi(const char *str);
```
where `str` is a pointer (read [below](#pointer)) to a null-terminated string that contains the representation of an integer.
The **input** is a string containing numeric characters, optionally with leading white spaces. The **output** is the integer value corresponding to the numeric characters in the string.

> [!WARNING]
> 
> - Remember to add `#include <stdlib.h>'.
> - **Non-Numeric Strings:** If the string does not represent a valid number, `atoi` will return 0. For instance, `atoi("abc")` will return 0.
> - **Error Handling:** `atoi` does not provide error handling for invalid input or overflow.

> [!TIP]
> Always check the input. In the example code, we have:
> 
> ```C
> if (n <= 0) {
> fprintf("Error: The number of trapezoids must be a positive integer.\n");
> return 1;
> }
> ```

# Pointer

In the version of the code with arrays, we statically allocated the memory as:

```C
double x_values[n];  // Array to store x points
double f_values[n];  // Array to store function evaluations f(x)
```

but now we do not know a priori the size of such arrays (i.e., `n` is given at run time). To overcome this issue, we make use of ***pointers***.

In C, ***pointers*** are a fundamental feature that allows you to directly interact with memory. A pointer is a variable that holds the memory address of another variable. Instead of storing a value directly, a pointer stores the location where the value is stored.

To dynamically allocate a pointer, we use the function `malloc` (memory allocation), which allocates a specified number of bytes of memory and returns a pointer to the beginning of this memory block.:

```C
double *x_values = malloc(n * sizeof(double));
```

This declares a pointer `x_values` that can point to a `double` type. `x_values` will be used to store the address of dynamically allocated memory that will hold an array of double values. 

> [!NOTE]
> When declaring a pointer, the asterisk `*` indicates that the
> variable `*var` is a pointer.  In our example, it tells the compiler
> that `x_values` is a pointer and it will store the address of a
> `double` variable or an array of double values. The asterisk is a
> **dereference operator**: it accesses the value stored at the memory
> address that the pointer is pointing to. In our case, `*x_values`
> refers to the double value stored at the address contained in
> `x_values`.

The argument of `malloc` is the correct number of bytes: indeed, it is given by the number of `double` elements `n` multiplied by the size (in bytes) of each `double` element (that is, `n * sizeof(double)`).

The pointer returned by `malloc` is assigned to `x_values`. This pointer now points to the start of a block of memory large enough to hold `n` double values. You can access and manipulate these values using pointer arithmetic or array indexing.

> [!TIP]
> **Check for NULL**: It is important to check if `malloc` returns `NULL`, which indicates that 
> the memory allocation has failed. In robust code, you would include a check to handle this case:
> 
> ```C
> if (x_values == NULL) {
>   fprintf("Memory allocation failed\n");
>   return 1; // Exit or handle the error appropriately
> }
> ```

Once the pointers `x_values` and `f_values` have been allocated, they are passed to the function `trapezoidal_rule` exactly as we did for the arrays. The difference now is in how the function `trapezoidal_rule` gets them as input variables:

```C
double trapezoidal_rule(double (*func)(double), double a, double b, int n, double *x_values, double *f_values) {
```

> [!NOTE]
> Function Pointer: `double (*func)(double)`. The asterisk denotes that `func` is a pointer to a function that takes a `double` argument and returns a `double`. It enables the function to be passed as an argument or stored in a variable.


> [!NOTE]
> Why Use Pointers?
> 
> - **Efficiency**: Pointers allow functions to modify variables directly without copying them.
> - **Dynamic Memory Management**: Pointers are essential for allocating and deallocating memory dynamically.
> - **Data Structures**: Many data structures like linked lists and trees use pointers to manage and navigate data.

# Understanding `*` and `&`
When working with pointers, two key operators in C are the asterisk (*) and the ampersand (&). They play crucial roles in accessing and manipulating memory addresses.

The `*` operator is used to **dereference** a pointer, which means accessing the value stored at the memory address the pointer holds.

```C
int x = 10;
int *ptr = &x;    // ptr holds the address of x
printf("%d\n", *ptr);  // Dereferencing ptr gives the value of x (10)
```

In this example, `ptr` stores the *address* of `x`, `*ptr` accesses the _value_ at that address (which is `10` in this case). 
Dereferencing allows you to manipulate or read the _value stored_ at the location the pointer is pointing to.

The `&` operator is used to get the **memory address** of a variable. 

> [!WARNING]
> `&` gives the address of the variable, not its value!

```C
int x = 10;
int *ptr = &x;  // &x gives the address of x, which is stored in ptr
```

In the last example, `&x` returns the _memory address_ where the variable `x` is stored. This address is assigned to `ptr`, making `ptr` a pointer to `x`.

> [!NOTE]
> Pointers (`*`) and addresses (`&`) are closely related:
>
> - Use `&` to get the **address** of a variable.
> - Use `*` to access (or modify) the **value** at the address stored in a pointer.

## Pointers to function

In C, not only can you create pointers to variables, but you can also create **pointers to functions**. This feature allows for more flexibility in programming, especially when you need to pass functions as arguments to other functions, or when you want to select a specific function at runtime.

A function pointer is a variable that stores the address of a function. 
Indeed, every variable and instruction in a program has a memory address. Similarly, a function’s entry point has its own memory address, which is linked to the function’s name (this is comparable to arrays, where the name of the array represents the address of its first element).

Just like any other pointer, you can dereference a function pointer to call the function it points to. 

In our example, we have this function:
```C
double f(double x)
```
and the corresponding function pointer is:
```C
double (*func)(double);
```
Here, `func` is a pointer to a function that takes one double argument and returns a double. In the above code line, the pointer has not been assigned yet. To assign a function to a function pointer, you simply use the function’s name (without parentheses), which is equivalent to the function’s address:
```C
func = f; 
```
At this point, one can call the function through the pointer. You need to dereference the pointer just like with regular pointers, but with function call syntax:
```C
double result = (*func)(2.3);
```
Alternatively, you can also call the function without dereferencing explicitly:
```C
double result = func(2.3);
```

In our example, in the `main`, we are passing the function `f` as an argument:
```C
double result = trapezoidal_rule(f, a, b, n, x_values, f_values);
```
Remember that the name of the function stores the memory address of the function itself. This means that the function `trapezoidal_rule` must be defined as:
```C
double trapezoidal_rule(double (*func)(double), double a, double b, int n, double *x_values, double *f_values) 
```


Below is an example that shows how function pointers can be used. In this case, we have two functions: `add` and `subtract`. We then use a function pointer to select which function to call based on a condition.

```C
#include <stdio.h>

// Functions
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int main() {
    int (*operation)(int, int);  // Declare a function pointer

    int x = 10, y = 5;
    char op;

    printf("Enter operation (+ or -): ");
    scanf(" %c", &op);

    // Assign function to pointer based on user input
    if (op == '+') {
        operation = add;
    } else if (op == '-') {
        operation = subtract;
    } else {
        printf("Invalid operation\n");
        return 1;
    }

    // Call the function via pointer
    int result = operation(x, y);
    printf("Result: %d\n", result);

    return 0;
}
```

## Pointers to pointer

In C, the `**` symbol is used when you are dealing with **pointers to pointers**. This means you have a pointer that points to another pointer, which in turn points to some data. It’s often used in more complex scenarios such as dynamic memory allocation for multi-dimensional arrays, handling arrays of strings (array of character pointers), or passing pointers by reference to functions.

```C
int x = 5;
int *ptr = &x;   // ptr is a pointer to x (an int)
int **ptr2 = &ptr; // ptr2 is a pointer to ptr (a pointer to int)

printf("%d\n", **ptr2);  // Dereferencing twice gives the value of x (5)
```
In this example:

How it Works:

- `ptr` is a pointer to `x` and it stores the **address** of the integer `x`.
- `ptr2` is a pointer to `ptr`, so it stores the **address** of the pointer `ptr`.
- When you _dereference_ `ptr2` _once_ (i.e., `*ptr2`), you get the **value** of `ptr`, which is the **address** of `x`.
- When you _dereference_ it _twice_ (i.e., `**ptr2`), you get the **value** stored in `x`.

A typical example is given by the following:

```C
void modifyPointer(int **ptr) {
    static int y = 100;
    *ptr = &y;  // Modify the pointer itself to point to y
}

int main() {
    int x = 10;
    int *p = &x;
    modifyPointer(&p);  // Pass the address of p (pointer to pointer)
    printf("%d\n", *p); // Outputs 100, since p now points to y
}
```

# Call by reference and by value 
Note that the function `trapezoidal_rule` is now defined as follows:

```C
double trapezoidal_rule(double (*func)(double), double a, double b, int n, double *x_values, double *f_values)
```

Some of the arguments are pointers (` double *x_values` and `double *f_values`). In C, functions create a **copy** of the arguments (called _formal arguments_). Any modification to these formal arguments acts on these **local copy**. For example, if we change the value of `a` inside the function `trapezoidal_rule`, the value of `a` remains **unchanged** in the `main` program: we are just modifying the value of the _copy_ of `a` in the function. 

On the other hand, if we pass a pointer to a function, like `double *x_values`, the function create a local copy of the pointer (which stores the **address** of the variable, and not its **value**). This means that if we change `x_values` (i.e., the address), we are only modifying the address (and not the value) inside the function. But, if we change the **value** stored at address `p` (i.e., `*p`) this change will be seen also outside the function `trapezoidal_rule` (because we are directly changing the **value** stored at some address). 

[Click here for more details](C_reference_vs_value.md)

## [More about arrays and pointers](C_pointers_and_arrays.md)
