# Modular Programming in C

When writing small programs, it’s tempting to put everything into a single `.c` file. It works —- until it doesn’t. 
As your project grows in size and complexity, a single-file approach quickly becomes a nightmare: hard to navigate, difficult to debug, and nearly impossible to maintain.

**Modular programming** solves this by **breaking a large program** into **smaller**, manageable, and **reusable components**, or modules. 
Each module focuses on a specific task or feature, such as input/output, mathematical computations, data structures, etc. 
This not only makes the **code more readable** and **maintainable**, but also enhances **reusability**, **testability**, and **collaboration** among developers.

Think of it like assembling a machine: you don’t build it as one giant lump of metal—you use bolts, gears, levers, each crafted separately, then put together to form the complete system.

In C, modularity is achieved by organizing code into **source files** (`.c`) and **header files** (`.h`). 
You use **header** files to declare the **public interface** of a module (functions, macros, structs), and the **source files** to **define** how those declarations actually **work**.

This approach is essential in real-world applications and is the backbone of professional software development in C. It lays the groundwork for:
- Creating libraries (static or shared).
- Building scalable systems
- Collaborating on large codebases
- Unit testing individual components
- Managing code versioning and updates efficiently
  
## File Structure and Naming Conventions

In modular C programming, organizing files clearly is essential to keeping your codebase readable, maintainable, and scalable. 
Typically, your code will be split across **source** files (`.c`) and **header** files (`.h`). 
Each **module** (meaning each logical component of your program) should ideally have:
- A **header** file to declare its interface (functions, types, constants, etc.)
- A **source** file to define the implementation of that interface

Basic Naming Convention:
| File Type     | Extension | Purpose                           | Example        |
|---------------|-----------|-----------------------------------|----------------|
| Header file   | `.h`      | Declarations (what is available)  | `math_utils.h` |
| Source file   | `.c`      | Definitions (how it works)        | `math_utils.c` |
| Main program  | `.c`      | The entry point of the program    | `main.c`       |

For example, a simple calculator project might have this layout:

```Plain
calculator/
├── main.c
├── math_utils.h
└── math_utils.c
```

What Goes Where?
- **Header** (`.h`) files should contain:
  - Function declarations (prototypes)
  - Macro definitions (`#define`)
  - Type definitions (`typedef`, `struct`)
  - `#include` guards (to prevent double inclusion)
- **Source** (`.c`) files should contain:
  - The actual function implementations
  - Internal helper functions (static if used only within that file)
  - `#include` of the corresponding `.h` file and any other needed headers


> [!TIP]
> ## Include Guards
> When writing modular C programs with multiple header files, you often include the **same header** in **several source files**. This can accidentally lead to **multiple inclusion** of the same declarations, which causes compilation errors like “_redefinition of…_”.
> To avoid this, we use include guards: a simple mechanism that ensures a header file is included only once per compilation unit.
> Every header file should be protected against multiple inclusions with `#ifndef` / `#define` / `#endif`:
> ```C
> // math_utils.h
> #ifndef MATH_UTILS_H
> #define MATH_UTILS_H
>
> int add(int a, int b);
> int subtract(int a, int b);
>
> #endif
> ```
> The basic idea is to **define a unique macro the first time** the file is included. On subsequent includes, the macro is already defined, so the file is skipped.
> 
> Use **uppercase letters** and underscores for header guards, often based on the filename.

## Declaring and Defining Functions with Header Files

When writing modular code in C, functions are typically declared in header files (`.h`) and defined in source files (`.c`). This separation promotes code reuse, clarity, and compilation speed.

### 1. Header File (`.h`) — Function Declarations
   
Also known as the function prototype, the declaration tells the compiler:
- The function’s name
- Its return type
- The types of its arguments

It **does not contain** the **function** body.

```C
// math_utils.h

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Declare functions (no body!)
int add(int a, int b);
int multiply(int a, int b);

#endif  // MATH_UTILS_H
```

### 2. Source File (`.c`) — Function Definitions
   
Here you define how the function actually works.

```C
// math_utils.c

#include "math_utils.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}
```
> [!NOTE]
> Always `#include` the corresponding header in the source file to ensure consistency between the declaration and the definition.

### 3. Main File — Using the Functions
   
The main program can now use the functions by including the header:

```C
// main.c

#include <stdio.h>
#include "math_utils.h"

int main() {
    int x = 3, y = 4;

    printf("x + y = %d\n", add(x, y));
    printf("x * y = %d\n", multiply(x, y));

    return 0;
}
```

### Summary Table

| File         |	Purpose                             |	Contents                                |
|--------------|--------------------------------------|-----------------------------------------|
| `math_utils.h` |	Public interface (declarations)     |	`int add(int, int)`;                      |
| `math_utils.c` |	Internal logic (definitions)	      | `int add(int a, int b) { return a + b; }` |
| `main.c`       |	Program entry point and usage logic	| `#include "math_utils.h"` + calls to add  |

## Compiling Modular C Code

When splitting a C program into multiple source files, each `.c` file must be **compiled separately into an object file** (`.o`), and then all object files are linked together into the final executable.

> [!NOTE]
> **Object files** (`.o`) are **intermediate** binary files produced by compiling individual `.c` source files. **They contain machine code** and information about functions and variables, but **they are not yet executable**. Object files must be **linked together** with others (and possibly libraries) to form the final program. This modular approach speeds up compilation and supports code reuse.

### Step 1: Compile Each Source File

Use `gcc` to compile `math_utils.c` and `main.c` into object files:

```Plain
gcc -c math_utils.c   # creates math_utils.o
gcc -c main.c         # creates main.o
```
Each **`-c` flag** tells the compiler: “compile only, **do not link**.”

### Step 2: Link the Object Files

Now link them into a single executable (e.g., called `myprog`):

```Plain
gcc main.o math_utils.o -o myprog
```

This creates the final executable `myprog`.

### Step 3: Run the Program

```Plain
./myprog
```

If everything is correct, you should see the output:

```Plain
x + y = 7
x * y = 12
```

> [!TIP]
> You can also compile and link in a single step:
> ```Plain
> gcc main.c math_utils.c -o myprog
> ```
> This works well for small projects, but for larger programs or **Makefile**-based builds, it’s better to compile modules separately to **avoid recompiling unchanged files**.

> [!TIP]
> Want to see more details or include debug symbols?
> ```Plain
> gcc -Wall -g main.c math_utils.c -o myprog
> ```
> The flag `-Wall` enables most compiler warnings (highly recommended!), and `-g` includes debugging information (useful with gdb)


## Example

Let's build a more complete example with multiple modules, and use it to highlight common pitfalls and good practices.

We want to write a small program that:
- Has a **math module** (`math_utils`) for common operations
- Has a **string module** (`string_utils`) for handling names
- Has a **main program** that uses both

The file structure is
```Plain
project/
│
├── main.c
├── math_utils.h
├── math_utils.c
├── string_utils.h
├── string_utils.c
```

### `math_utils.h`
```C
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

int add(int a, int b);
int multiply(int a, int b);

#endif // MATH_UTILS_H
```
### `math_utils.c`
```C
#include "math_utils.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}
```

### `string_utils.h`
```C
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

void greet(const char *name);

#endif // STRING_UTILS_H
```

### `string_utils.c`
```C
#include <stdio.h>
#include "string_utils.h"

void greet(const char *name) {
    printf("Hello, %s!\n", name);
}
```

### `main.c`
```C
#include <stdio.h>
#include "math_utils.h"
#include "string_utils.h"

int main(int argc, char *argv[]) {
    int x = 5, y = 3;

    greet("Boltzmann");

    printf("%d + %d = %d\n", x, y, add(x, y));
    printf("%d * %d = %d\n", x, y, multiply(x, y));

    return 0;
}
```

> [!WARNING]
> ### Common Issues and Fixes
>
> | Problem                                     | Cause                                                              | Fix                                                    |
> |---------------------------------------------|--------------------------------------------------------------------|--------------------------------------------------------|
> | `undefined reference to 'add'`              | You compiled `main.c` but forgot to compile or link `math_utils.c` | Make sure all `.c` files are compiled and linked       |
> | `redefinition of function`                  | You accidentally included `.c` file instead of `.h`                | Never `#include` `.c` files                            |
> | `multiple definition of symbol`             | You defined a function in both `.h` and `.c`                       | Only declare in `.h`, define in `.c`                   |
> | Missing include guard causes strange errors | Header gets included twice                                         | Always use `#ifndef / #define / #endif` in headers     |

⸻

> [!NOTE]
> ### Compilation Instructions
> ```Plain
> gcc -c math_utils.c
> gcc -c string_utils.c
> gcc -c main.c
> gcc main.o math_utils.o string_utils.o -o program
> ./program\
> ```
> One-liner:
> ```Plain
> gcc main.c math_utils.c string_utils.c -o program
> ```
