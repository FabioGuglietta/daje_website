
# Derived Types

In previous lectures, we introduced some basic Fortran data types such as `INTEGER`, `REAL`, `CHARACTER`, etc. 
However, Fortran also allows you to define more complex **types** that **group multiple variables** — possibly of different types — under a **single name**. 
These are called **derived types**, and they are Fortran’s equivalent of C’s structures.

Derived types are extremely useful when you want to represent real-world entities that are best modeled as a collection of attributes. 
For example, a student might be described by a name, age, gender, and height — rather than managing each property separately, you can **bundle them together** into a **single entity** using a derived type.

This not only improves code **readability** and **structure**, but also supports **modularity** and better **data organization** — especially in scientific computing, where one often works with arrays of particles, cells, points, etc., each having multiple properties.

You define a derived type using the `TYPE` keyword. Here’s how you could represent a person:
```Fortran
TYPE :: Person
    CHARACTER(len=20) :: firstName
    CHARACTER(len=20) :: lastName
    INTEGER           :: age
    CHARACTER         :: gender
    REAL              :: height
END TYPE Person
```
You can now create variables of this new type:
```Fortran
PROGRAM test_struct
    IMPLICIT NONE

    TYPE(Person) :: student, professor

    ! Initialize using assignments
    student%firstName = 'Ludwik'
    student%lastName  = 'Boltzmann'
    student%age       = 26
    student%gender    = 'M'
    student%height    = 1.72

    professor%firstName = 'Josef'
    professor%lastName  = 'Stefan'
    professor%age       = 35
    professor%gender    = 'M'
    professor%height    = 1.68

    ! You can print or modify fields using the % operator
    student%age = 28  ! Correct the student's age
    student%firstName = 'Ludwig'  ! Fix the typo

    PRINT *, 'Student:', student%firstName, student%lastName, student%age
    PRINT *, 'Professor:', professor%firstName, professor%lastName, professor%age
END PROGRAM test_struct
```
> [!NOTE]
> The `%` symbol is used in Fortran to **access fields** inside a derived type.

You can declare arrays of derived types. For example:
```Fortran 
TYPE(Person), ALLOCATABLE :: people(:)
INTEGER :: n, i

n = 2
ALLOCATE(people(n))

people(1)%firstName = 'Ada'
people(1)%lastName  = 'Lovelace'
people(1)%age       = 36
people(1)%gender    = 'F'
people(1)%height    = 1.65

people(2)%firstName = 'Alan'
people(2)%lastName  = 'Turing'
people(2)%age       = 41
people(2)%gender    = 'M'
people(2)%height    = 1.75

DO i = 1, n
    PRINT *, TRIM(people(i)%firstName), TRIM(people(i)%lastName), people(i)%age
END DO

DEALLOCATE(people)
```

> [!NOTE]
> In Fortran, the `TRIM` function is used to **remove trailing spaces** from a `CHARACTER` string.
> Unlike strings in C, Fortran `CHARACTER` variables have fixed length, and any unused characters are automatically padded with spaces. This can lead to unwanted spaces when printing or comparing strings.
> The syntax is `result = TRIM(string)`, where `string` is a `CHARACTER` expression, and `result` is a new `CHARACTER` value with the same content but with **trailing blanks removed**.
> ```Fortran
> CHARACTER(len=10) :: name
> name = 'Ada'  ! stored as 'Ada       '
> PRINT *, 'Hello, [' // name // ']'
> PRINT *, 'Hello, [' // TRIM(name) // ']'
> ```
> Output:
> ```Plain
> Hello, [Ada       ]
> Hello, [Ada]
> ```
> Without TRIM, the name variable includes extra spaces, because it has length 10. This affects formatting and string comparisons.

> [!TIP]
> Always use `TRIM()` when printing or concatenating Fortran strings — especially when their length is larger than the actual content.

## Example

Let’s look at this code:
```Fortran
program point3d_example
    implicit none

    ! Define a derived type for a 3D point
    type :: Point3D
        integer :: id      ! Point identifier
        real(8) :: x, y, z ! Coordinates
    end type Point3D

    ! Declare variables
    type(Point3D), allocatable :: points(:)
    integer :: n, i

    n = 3
    allocate(points(n))  ! Allocate memory for n Point3D elements

    ! Initialize each point using a procedure
    call init_point(points(1), 0, 1.0d0, 2.0d0, 3.0d0)
    call init_point(points(2), 1, 4.5d0, 0.0d0, -1.2d0)
    call init_point(points(3), 2, -3.3d0, 2.2d0, 1.1d0)

    ! Print all points
    do i = 1, n
        call print_point(points(i))
    end do

    deallocate(points)  ! Free the allocated memory

contains

    ! Subroutine to initialize a point
    subroutine init_point(p, id, x, y, z)
        type(Point3D), intent(inout) :: p
        integer, intent(in)          :: id
        real(8), intent(in)          :: x, y, z

        p%id = id
        p%x = x
        p%y = y
        p%z = z
    end subroutine init_point

    ! Subroutine to print a point
    subroutine print_point(p)
        type(Point3D), intent(in) :: p
        print '(A, I0, A, F5.2, A, F5.2, A, F5.2, A)', &
              "Point ", p%id, ": (", p%x, ", ", p%y, ", ", p%z, ")"
    end subroutine print_point

end program point3d_example
```
Let’s break it down:
1. **Derived Type Definition:**
   - We define a custom data type using `type :: Point3D`, which groups an integer `id` and three double-precision floating-point coordinates.
2. **Dynamic Allocation:**
	- In Fortran, `allocate(points(n))` **dynamically allocates** memory for an array of derived type `Point3D`.
3. **Initialization:**
	- The subroutine `init_point()` receives a `Point3D` variable by **reference** and assigns the values.
4. **Accessing Members:**
	- Fortran uses the `%` operator to access components of a derived type, such as `p%id`.
5. **Deallocation:**
	- Memory allocated with allocate must be explicitly released using `deallocate`.
