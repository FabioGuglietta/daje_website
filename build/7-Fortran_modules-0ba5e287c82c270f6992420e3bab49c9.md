# Modular Programming in Fortran

When writing small Fortran programs, it may seem convenient to place everything inside a single file. This will work for trivial cases, but becomes unmanageable as a project grows. Large monolithic codes are difficult to read, maintain, extend, and debug.

Modular programming solves this by splitting a program into independent, reusable components. In Fortran, this is achieved through modules, which group together variables, procedures, constants, and derived types. Each module exposes a clear interface and hides internal implementation details.

Just like assembling a machine from well‑designed parts, modular programming lets you build software from cleanly separated components. This approach improves:
- readability
- maintainability
- reusability
- testability
- collaboration among developers

In Fortran, modularity is built into the language. A typical project contains:
- one or more modules (`.f90`) defining procedures and data types
- a main program using those modules
- possibly additional source files for organization or libraries

## File Structure and Naming Conventions

A modular Fortran project is usually organized into `.f90` files.
Each logical component should have:
- One module file containing its procedures, data, and public interface
- A main program that uses those modules

## Basic Naming Convention

| File Type       | Extension | Purpose                               | Example           |
|------------------|-----------|----------------------------------------|--------------------|
| Module file      | `.f90`    | Public interface + implementations     | `math_utils.f90`   |
| Module output    | `.mod`    | Compiler‑generated module metadata     | `math_utils.mod`   |
| Source file      | `.f90`    | Additional code or modules             | `string_utils.f90` |
| Main program     | `.f90`    | Entry point of the program             | `main.f90`         |

Example project layout:
```Plain
project/
├── main.f90
├── math_utils.f90
└── string_utils.f90
```
What Goes Where?
- Module files (`.f90`) contain:
  - procedure definitions
  - public declarations (public, private)
  - constants and parameters
  - derived types
  - internal helper procedures
- Main program contains:
  - the program block
  - use module_name statements
  - input/output logic

Unlike C, Fortran does not use include guards because modules are compiled separately and imported explicitly.

## Declaring and Defining Procedures in Modules

In Fortran, both the declaration and definition of a procedure live inside a module. This guarantees type safety and avoids many common C‑style linking errors.

### `math_utils.f90`
```Fortran
module math_utils
  implicit none
contains

  function add(a, b) result(res)
    integer, intent(in) :: a, b
    integer :: res
    res = a + b
  end function add

  function multiply(a, b) result(res)
    integer, intent(in) :: a, b
    integer :: res
    res = a * b
  end function multiply

end module math_utils
```
### `string_utils.f90` 
```Fortran
module string_utils
  implicit none
contains

  subroutine greet(name)
    character(len=*), intent(in) :: name
    print *, "Hello, ", name, "!"
  end subroutine greet

end module string_utils
```
### `main.f90`
```Fortran
program main
  use math_utils
  use string_utils
  implicit none

  integer :: x, y

  x = 5
  y = 3

  call greet("Boltzmann")

  print *, x, "+", y, "=", add(x, y)
  print *, x, "*", y, "=", multiply(x, y)

end program main
```

## Compiling Modular Fortran Code

Fortran modules must be compiled before any program that uses them.

> [!NOTE]
> Compiling a module creates:
> - an object file (`math_utils.o`)
> - a module interface file (`math_utils.mod`)
> 
> The `.mod` file is required by any `use math_utils` statement.

### Step 1: Compile Each Module
```Plain
gfortran -c math_utils.f90     # produces math_utils.o + math_utils.mod
gfortran -c string_utils.f90   # produces string_utils.o + string_utils.mod
```

### Step 2: Compile the Main Program
```Plain
gfortran -c main.f90
```

### Step 3: Link Everything Together
```Plain
gfortran main.o math_utils.o string_utils.o -o program
```

### Step 4: Run
```Plain
./program
```

> [!TIP]
> One‑liner compilation for small projects:
> ```Plain
> gfortran main.f90 math_utils.f90 string_utils.f90 -o program
> ```

> [!WARNING]
> ### Common Issues and Fixes
> | Problem                                  | Cause                                      | Fix                                              |
> |------------------------------------------|--------------------------------------------|--------------------------------------------------|
> | Undefined reference to `add_`            | Module not compiled or not linked          | Compile all `.f90` files and link all `.o` files |
> | Can't open module file `math_utils.mod`  | `.mod` file missing or wrong compile order | Compile modules before files that use them       |
> | Confusing implicit typing                | Missing `implicit none`                    | Always put `implicit none` in every program/module |
> | Multiple interface mismatches            | Procedure not inside a module              | Always place procedures inside modules           |
