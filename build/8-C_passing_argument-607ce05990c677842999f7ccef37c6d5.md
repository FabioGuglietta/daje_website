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


## A more complete example

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_help(const char *program) {
    printf("Usage: %s [OPTIONS]\n", program);
    printf("Options:\n");
    printf("  --name <string>         Specify a name (string)\n");
    printf("  --count <int>           Specify a count (integer)\n");
    printf("  --values <int>...       List of integer values (array)\n");
    printf("  --help                  Display this help message\n");
    return 0; // 0 is the standard return value in case of success
}

int print_error(const char *message) {
    fprintf(stderr, "Error: %s\n",message);
    return 1; // 1 is the standard return value in case of error
}

int main(int argc, char **argv) {
    #define string_size 100
    #define values_size 100
    char name[string_size] = "";
    int count = 0;
    int values[values_size];
    int num_values=0;

    if (argc==1) return print_help(argv[0]); // no argument passed, let's print the help message

    for (int i = 1; i < argc; i++) { // loop over the arguments
        if (strcmp(argv[i], "--help")==0) {
                return print_help(argv[0]);
        }
        else if (strcmp(argv[i], "--name")==0) {

            if (i+1 == argc) return print_error("--name requires a string argument");

            strncpy(name, argv[i+1], string_size); // copy at most size elements
            i++ ; // go to the next element

        }
        else if (strcmp(argv[i], "--count")==0) {

            if (i+1 == argc) return print_error("--count requires an integer argument");

            count = atoi(argv[i+1]);
            i++;

        }
        else if (strcmp(argv[i], "--values") == 0) {
            if (i+1 == argc) return print_error("--values requires an array of integers ");
            i++;
            while (i < argc && argv[i][0] != '-') {
                if (num_values > values_size) return print_error("too many elements passed to --values");
                values[num_values] = atoi(argv[i++]);
                num_values++;
            }
            i--; // Adjust since for loop will increment
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    // Output parsed values
    printf("Name: %s\n", name);
    printf("Count: %d\n", count);
    printf("Values:");
    for (int i = 0; i < num_values; i++) {
        printf(" %d", values[i]);
    }
    printf("\n");

    return 0;
}
```
