
# Passing arguments in Fortran
We propose another variation of the [example code](Fortran_BASIC.md). 
Indeed, we now read the number of the trapezoids from the command line, and we ***dynamically*** allocate the memory to store `x_values` and `f_values` (if you didn't read the previous examples, don't worry: you will easliy understand what these varaibles are):

```Fortran
program trapezoidal_integration
    implicit none
    real(8) :: a = 0.0d0     ! Lower limit of integration
    real(8) :: b = 1.0d0     ! Upper limit of integration
    real(8), allocatable :: x_values(:), f_values(:)
    integer :: n, i
    real(8) :: result

    ! Prompt the user for the number of trapezoids
    print *, "Enter the number of trapezoids (n):"
    read(*,*) n

    ! Check if n is valid
    if (n <= 0) then
        print *, "Error: The number of trapezoids must be a positive integer."
        stop
    end if

    ! Allocate memory for x_values and f_values arrays
    allocate(x_values(n), f_values(n))

    ! Call the trapezoidal rule to perform the integration
    result = trapezoidal_rule(a, b, n, x_values, f_values)

    ! Print the result of the integration
    print *, "The integral of f(x) = x^3 from ", a, " to ", b, " is approximately: ", result

    ! Optionally, print out the x values and their corresponding f(x) values
    print *, "x values and f(x) evaluations:"
    do i = 1, n - 1
        print '(I3, F10.5, F15.5)', i, x_values(i), f_values(i)
    end do

    ! Free dynamically allocated memory
    deallocate(x_values, f_values)

contains

    ! Function to define f(x) = x^3
    real(8) function f(x)
        real(8), intent(in) :: x
        f = x**3
    end function f

    ! Function to perform the trapezoidal rule
    real(8) function trapezoidal_rule(a, b, n, x_values, f_values)
        real(8), intent(in) :: a, b
        integer, intent(in) :: n
        real(8), intent(out) :: x_values(n), f_values(n)
        real(8) :: p, sum
        integer :: i

        p = (b - a) / real(n)            ! Width of each trapezoid
        sum = 0.5d0 * (f(a) + f(b))      ! End points contribution

        ! Calculate x values and function evaluations
        do i = 1, n - 1
            x_values(i) = a + real(i) * p
            f_values(i) = f(x_values(i))  ! Store function evaluation
            sum = sum + f_values(i)
        end do

        trapezoidal_rule = sum * p        ! Return the integral result
    end function trapezoidal_rule

end program trapezoidal_integration
```

## Read command line arguments: `read()` 

In Fortran, the `read()` statement is used to read input from a source, typically the keyboard (standard input) or a file. It can be used in several forms to capture different types of input.
In our example:

```Fortran
read(*,*) n
```

the `read(*,*)` statement takes input for the integer `n` from the user. The `*` format specifier means free-form input, so the user can enter the values without needing to worry about specific formatting.

The general syntax is:

```Fortran
read(unit, format) variables
```
where
- `unit`: Specifies the input source. The most common value is `*`, which represents standard input (usually the keyboard).
- `format`: Defines how the input is interpreted. `*` can be used for free-form input, which allows the compiler to automatically interpret the format.
- `variables`: The variables where the input values are stored.
