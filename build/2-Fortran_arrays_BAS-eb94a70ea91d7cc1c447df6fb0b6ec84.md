
# Arrays
We slightly modify the [example code](Fortran_BASIC.md) written to numerically evaluate an integral with the trapezoid rule: 

```Fortran
program trapezoidal_integration
    implicit none

    integer, parameter :: n = 1000  ! Number of trapezoids
    real(8) :: a = 0.0d0            ! Lower limit of integration
    real(8) :: b = 1.0d0            ! Upper limit of integration
    real(8) :: result               ! Result of the integration
    real(8), dimension(n) :: x_values, f_values
    integer :: i

    print *, "This program performs numerical integration of f(x) = x^3 from a = ", a, " to b = ", b, " using ", n, " trapezoids."

    ! Check if n is a valid number of trapezoids
    if (n > 0) then
        print *, "The number of trapezoids is positive."
    else if (n < 0) then
        print *, "Error: The number of trapezoids is negative."
        stop 1
    else
        print *, "Error: The number of trapezoids is zero."
        stop 1
    end if

    ! Perform numerical integration
    result = trapezoidal_rule(a, b, n, x_values, f_values)

    ! Print the result of the integration
    print *, "The integral of f(x) = x^3 from ", a, " to ", b, " is approximately: ", result

    ! Optionally, print out the x values and their corresponding f(x) values
    print *, "x values and f(x) evaluations:"
    do i = 1, n-1
        print *, "x(", i, ") = ", x_values(i), ", f(x(", i, ")) = ", f_values(i)
    end do

contains

    ! Define the function to integrate: f(x) = x^3
    real(8) function f(x)
        real(8), intent(in) :: x
        f = x**3
    end function f

    ! Trapezoidal rule for numerical integration
    real(8) function trapezoidal_rule(a, b, n, x_values, f_values)
        real(8), intent(in) :: a, b
        integer, intent(in) :: n
        real(8), dimension(n), intent(out) :: x_values, f_values
        real(8) :: p, sum
        integer :: i

        p = (b - a) / real(n)         ! Width of each trapezoid
        sum = 0.5d0 * (f(a) + f(b))   ! End points contribution

        ! Store the x values and function evaluations in arrays
        do i = 1, n-1
            x_values(i) = a + i * p
            f_values(i) = f(x_values(i))  ! Store the function evaluation
            sum = sum + f_values(i)
        end do

        trapezoidal_rule = sum * p  ! Return the computed integral
    end function trapezoidal_rule

end program trapezoidal_integration
```

Two arrays are now introduced in the `program`:

- `x_values`: This array stores the $x$ values where the function $f(x)$ is evaluated during the integration.
- `f_values`: This array stores the computed values of the function $f(x)$ at those $x$ values.


In FORTRAN, an ***array*** is a collection of elements of the same type stored in contiguous memory locations. Arrays are useful when you need to store multiple values of the same type and access them using an index.

```type, dimension(size) :: array_name```

where `type` is the data type of the elements (e.g., `integer`, `real(8)`, `character`, etc.), `array_name` is the name of the array, and `size` is the number of elements in the array.

> [!WARNING]
> 
> - Indexing starts at 1: the first element is accessed using `arr(1)`, not `arr(0)`.
> - Accessing an index outside the array size (e.g., `arr(6)` when the array has only 5 elements) will typically result in an error.

The function `trapezoidal_rule()` is modified to take two additional array parameters (`x_values` and `f_values`).

```Fortran
real(8) function trapezoidal_rule(func, a, b, n, x_values, f_values)
    real(8), intent(in) :: a, b
    integer, intent(in) :: n
    real(8), dimension(n), intent(out) :: x_values, f_values
    ...
```

Inside the do loop, each `x` value is stored in `x_values(i)`, and the corresponding function evaluation is stored in `f_values(i)`.

```Fortran
do i = 1, n-1
    x_values(i) = a + i * p
    f_values(i) = f(x_values(i))  ! Store the function evaluation
    sum = sum + f_values(i)
end do
```

After the integration, to print the stored `x` values and their corresponding function evaluations `f(x)`:

```Fortran
do i = 1, n-1
    print *, "x(", i, ") = ", x_values(i), ", f(x(", i, ")) = ", f_values(i)
end do
```

## Two Ways of Declaring and Using Arrays: Static vs Dynamic Memory Allocation

In Fortran, arrays can also be allocated in two main ways:
1.	**Statically** — at compile time, using fixed-size declarations.
2.	**Dynamically** — at runtime, using `allocate` and `deallocate`.

Both approaches have advantages and limitations. Understanding them is key to writing efficient and flexible Fortran code.

### Static Allocation

When you declare an array like this:
```Fortran
integer :: w(10)
```
the memory is allocated statically. The compiler knows the **size at compile time** and usually places it in the **stack** or static memory region.

> [!NOTE]
> - **Fast** allocation and access.
> - **Automatically deallocated** at the end of the scope.


> [!WARNING]
> - The **size** must be **known** at compile time.
> - **Large** arrays may cause **stack overflow**.

### Dynamic Allocation

Dynamic allocation lets you **request memory** for arrays at **runtime**, using the `allocate` statement. For example:
```Fortran
integer, allocatable :: v(:)
allocate(v(10))
...
! do something
...
deallocate(v)
```

> [!NOTE]
> - You can **allocate** memory based on **input** values or **runtime** conditions.
> - **Larger arrays** are allowed (not limited by stack size).

> [!Warning]
> Deallocating memory in Fortran is a crucial practice in managing **dynamic memory** usage effectively. When an allocatable array is created using the `allocate` statement, memory is reserved on the **heap** to store its elements. If this memory is not released using the `deallocate` statement when the array is no longer needed, it can lead to **memory leaks**. Memory leaks occur when allocated memory is not properly freed, resulting in wasted resources and potentially causing a program to consume more memory than necessary. Over time, this can degrade system performance, lead to unexpected behavior, and, in extreme cases, exhaust available memory, leading to program crashes. Therefore, always ensure that any dynamically allocated memory is properly deallocated to maintain efficient memory management and optimal program performance.

> [!TIP]
> **Automatic Deallocation on Exit:** If the allocatable array goes out of scope when the subroutine exits, the memory will be automatically deallocated, but this behavior **only** applies if the array is declared as allocatable within that subroutine. If you use an allocatable array declared in a module or main program, it will remain allocated until explicitly deallocated.

> [!TIP]
> If you attempt to allocate an **already allocated** array in Fortran, it will result in a runtime error. Specifically, the program will throw an error indicating that the array is already allocated. To avoid this issue, **you should check** whether the array is allocated before allocating it again:
>
> ```Fortran
> if(.not.allocated(array))allocate(array)
> ```


## Example: Three Ways to Work with Arrays in Procedures

The example below demonstrates three common patterns:
- `f1`: allocates memory inside the function and returns the result.
- `f2`: takes an allocatable array and allocates inside the procedure.
- `f3`: modifies an existing statically allocated array.

```Fortran
program test_memory
  implicit none
  integer, allocatable :: v1(:), v2(:)
  integer :: w(2)
  integer :: s

  s = 2
  v1 = f1(s)
  call f2(v2, s)
  call f3(w, s)

  print *, v1(1), v2(1), w(1)

  call free(v1)
  call free(v2)

contains

  function f1(s) result(v)
    integer, intent(in) :: s
    integer, allocatable :: v(:)
    allocate(v(s))
    v(1) = 100
  end function f1

  subroutine f2(v, s)
    integer, intent(in) :: s
    integer, allocatable, intent(out) :: v(:)
    allocate(v(s))
    v(1) = 100
  end subroutine f2

  subroutine f3(w, s)
    integer, intent(in) :: s
    integer, intent(inout) :: w(s)
    w(1) = 100
  end subroutine f3

  subroutine free(arr)
    integer, allocatable, intent(inout) :: arr(:)
    if (allocated(arr)) deallocate(arr)
  end subroutine free

end program test_memory
```

> [!TIP]
> - Use **static** arrays when the **size is fixed and small.**
> - Use **dynamic** arrays when the size depends on user **input** or must be **very large**.
> - Use `allocate`/`deallocate` responsibly to avoid **memory leaks.**

## Array Size Management in Fortran: Using PARAMETER and INTEGER Variables

In Fortran, array sizes are typically specified using named constants or integer variables. The most common and robust method is to use the `PARAMETER` attribute:
```Fortran
program example
    implicit none
    integer, parameter :: N = 100
    real :: a(N)

    ! Do something with array a...
end program example
```
The `PARAMETER` keyword defines a constant at compile time, similar to C’s `#define` macro. It’s type-safe, scoped, and strongly recommended for fixed-size arrays.

You can also use integer variables to set array sizes, especially when dealing with allocatable arrays:
```Fortran
program dynamic_array_example
    implicit none
    integer :: N
    real, allocatable :: b(:)

    N = 100
    allocate(b(N))

    ! Use array b...

    deallocate(b)
end program dynamic_array_example
```

## Array initialisation

Initializing an array means assigning initial values to its elements at the time of declaration. Proper initialization is crucial to avoid unpredictable behavior caused by garbage values.

When you declare an array, you can initialize it in several ways:

1. Full Initialization: you provide explicit values for all elements:\
   ```integer :: arr(3) = (/1, 2, 3/)  ! Array of 3 integers, initialized to {1, 2, 3}```
1. Zero Initialization: You can explicitly initialize all elements to zero (or to any other value): \
   ```integer :: arr(4) = 0  ! Array of 4 integers, initialized to {0, 0, 0, 0}```

> [!WARNING]
> In FORTRAN, arrays are not automatically initialized when declared. If you declare an array without explicitly initializing it, the array elements will contain garbage values (random values left in memory). This can lead to unpredictable behavior in your program.
> Remember to always initialise them!

Here an example of arrays declaration and initialisation:

```Fortran
program arrays               
    implicit none

    real, dimension(3)    :: a                      ! Declare a real array 'a' of size 3 (uninitialized)
    real                  :: b(3)                   ! Declare a real array 'b' of size 3 (uninitialized)
    integer, dimension(5) :: c, d                   ! Declare two integer arrays 'c' and 'd', both of size 5 (uninitialized)
    integer               :: e(5), f(6)             ! Declare two integer arrays 'e' of size 5 and 'f' of size 6 (uninitialized)
    real, dimension(3)    :: g = (/1.2, 2., 3.7/)   ! Declare and initialize a real array 'g' with values 1.2, 2.0, and 3.7
    integer, dimension(7) :: h = 1                  ! Declare an integer array 'h' of size 7. All elements are initialized with 1
    
end program                
```

## Assumed-Shape vs Assumed-Size Arrays in Fortran

When passing arrays to procedures in Fortran, two common styles exist: **assumed-size** and **assumed-shape** arrays. 
While they may look similar at a glance, they behave quite differently.

### Assumed-Size Arrays: `arr(*)`

An assumed-size array **does not carry any size information** into the subroutine. It is declared using an asterisk `(*)`:
```Fortran 
subroutine process(arr)
    integer :: arr(*)  ! Assumed-size array
end subroutine
```
Characteristics:
- The **size** of the array is **not known** inside the subroutine.
- Functions like `SIZE(arr)` or `SHAPE(arr)` will **not work.**
- The **subroutine relies on the programmer** to know or manage the array size manually.
- This style originates from older Fortran (FORTRAN 77) and is still supported for legacy code.

### Assumed-Shape Arrays: `arr(:)`

An assumed-shape array **retains full size and shape information** when passed to the procedure. It is declared with a colon `(:)`.
```Fortran
subroutine process(arr)
    integer, intent(in) :: arr(:)  ! Assumed-shape array
end subroutine
```
Characteristics:
- You can query `SIZE(arr)`, `SHAPE(arr)`, and use bounds checking.
- Requires that the procedure has an explicit interface, either through:
    - A `MODULE`,
    - A `CONTAINS` block (internal procedures),
    - An `INTERFACE` block in the caller.

Assumed-shape arrays are the modern, safe, and preferred way to pass arrays. Ideal for robust and maintainable code.

> [!TIP]
> Unless you are interfacing with legacy code or performance is absolutely critical, **always prefer assumed-shape arrays**. They are safer, easier to work with, and fully compatible with modern Fortran features.


## Common Pitfalls When Using Arrays in Fortran

Fortran arrays are powerful and flexible, especially with features like automatic bounds checking and multidimensional support. However, there are still some pitfalls to be aware of.

### 1. Out-of-Bounds Access

By default, accessing elements outside the declared bounds of an array leads to undefined behavior. Some compilers may catch this (especially with bounds checking enabled), but others may silently produce wrong results or crash.
```Fortran
program out_of_bounds
    implicit none
    integer :: a(5)

    a = [1, 2, 3, 4, 5]
    print *, "Valid: ", a(5)     ! OK
    print *, "Invalid: ", a(6)   ! May cause crash or print garbage
end program out_of_bounds
```
With bounds checking enabled (e.g., `-fcheck=bounds` in `gfortran`), this will throw a runtime error. Without it, you may get garbage values or segmentation faults.

### 2. Uninitialized Arrays

Declaring an array without setting its values leads to uninitialized memory content — essentially garbage values.
```Fortran
program uninitialized
    implicit none
    integer :: b(3)
    integer :: i

    do i = 1, 3
        print *, "b(", i, ") = ", b(i)
    end do
end program uninitialized
```
Output might look like:
```
 b( 1 ) =  32767
 b( 2 ) = -1938123
 b( 3 ) =  0
```
The values are unpredictable and platform/compiler-dependent. Always initialize your arrays!

### 3. Misusing `SIZE` or `SHAPE`

If you pass an assumed-size array to a subroutine, you lose the ability to query its bounds with intrinsic functions like `SIZE` or `SHAPE`.
```Fortran
subroutine bad_size(arr)
    integer :: arr(*)   ! assumed-size: size is unknown
    print *, "Size = ", size(arr)  ! Compilation error
end subroutine bad_size
```
Correct approach: use assumed-shape arrays and declare an explicit interface (e.g., via a module or interface block):
```Fortran
subroutine good_size(arr)
    integer, intent(in) :: arr(:)  ! assumed-shape
    print *, "Size = ", size(arr)
end subroutine good_size
```
Here a code you can run:
```Fortran
program array_size_demo
    implicit none
    integer :: data(5) = [10, 20, 30, 40, 50]

    print *, "--- Calling bad_size ---"
    call bad_size(data)

    print *, "--- Calling good_size ---"
    call good_size(data)

contains

    ! This subroutine uses an assumed-size array.
    ! You cannot use SIZE() on it.
    subroutine bad_size(arr)
        implicit none
        integer :: arr(*)  ! Assumed-size array (no size metadata)
        ! print *, "Size = ", size(arr)  ! This would cause a compile-time error
        print *, "First element = ", arr(1)
        print *, "Second element = ", arr(2)
        ! Programmer must know the array size in advance
    end subroutine bad_size

    ! This subroutine uses an assumed-shape array.
    ! SIZE() works here.
    subroutine good_size(arr)
        implicit none
        integer, intent(in) :: arr(:)  ! Assumed-shape array
        print *, "Size = ", size(arr)  ! This works
        print *, "Last element = ", arr(size(arr))
    end subroutine good_size

end program array_size_demo
```

> [!NOTE]
> Assumed-shape arrays require an explicit interface when used across program units.
