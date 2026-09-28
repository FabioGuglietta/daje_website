
# Multidimensional arrays

In Fortran, multidimensional arrays are easily declared by specifying the dimensions in parentheses. These are useful for representing data in matrices, grids, or higher-dimensional structures.

```Fortran
type :: array_name(size1, size2, ...)
```

An example of a 2D array:

```Fortran
integer :: matrix(3, 4)  ! Array of 3 rows and 4 columns
```

It can be initialised either with a *full initialisation*:

```Fortran
integer :: matrix(2, 3) = reshape([1, 2, 3, 4, 5, 6], shape(matrix))
```

Example:

```Fortran
program multidimensional_array
    implicit none
    integer :: i, j
    integer :: matrix(2, 3)

    ! Initialize the 2D array
    matrix = reshape([1, 2, 3, 4, 5, 6], shape(matrix))

    ! Print the 2D array
    print *, "Matrix:"
    do i = 1, 2  ! Loop through rows
        do j = 1, 3  ! Loop through columns
            print *, matrix(i, j),
        end do
        print *, ''  ! Newline after each row
    end do
end program multidimensional_array
```

> [!TIP]
> In Fortran, the `size()` function is used to determine the number of elements in an array. It can be applied to arrays of any dimension.
> The syntax is:
>
> ```Fortran
> n = SIZE(array [, dim])
> ```
>where `array` is the array whose size you want to determine and `dim` is optional and representes the dimension along which the size is returned (if omitted, `size()` returns the total number of elements in the array).
>
> ```Fortran
> ! 1D array
> real :: arr1(5)
> print *, SIZE(arr1)  ! Outputs: 5
> 
> ! 2D array
> real :: arr2(3,4)
> print *, SIZE(arr2)  ! Outputs: 12 (total number of elements)
> print *, SIZE(arr2, 1)  ! Outputs: 3 (size along the first dimension)
> print *, SIZE(arr2, 2)  ! Outputs: 4 (size along the second dimension)
> ```

> [!WARNING]
> In Fortran, multidimensional arrays are stored in **column-major order**, meaning that elements of a column are stored **contiguously** in memory before moving to the next column. This contrasts with languages like C, which use row-major order, where elements of a row are stored contiguously.
> 
> When looping over multidimensional arrays in Fortran, it is **more efficient** to iterate over the leftmost index (first dimension) in the inner loop and the rightmost index (last dimension) in the outer loop. This is because Fortran stores data in column-major order, so accessing elements in this sequence ensures better memory locality and performance.
> 
> ```Fortran
> program example
>     implicit none
>     real, dimension(128,128,128) :: u          ! Velocity field
>     integer                      :: ix, iy, iz ! Indices used in the do loop
> 
>     do iz = 1, size(u,3)
>         do iy = 1, size(u,2)
>             do ix = 1, size(u,3)
>             ...
>             end do
>         end do
>     end do
> 
> end program
> ```
> 
> This approach allows Fortran to **access memory contiguously**, improving cache performance. If you switch the order of loops (i.e., iterate over the rows in the outer loop and columns in the inner loop), the program will still work but may be less efficient due to non-contiguous memory access.

## Dynamic Allocation of Multidimensional Arrays
In Fortran, multidimensional arrays can be allocated dynamically using the `allocate` statement. 
You can specify multiple dimensions when allocating, allowing you to create complex data structures such as matrices or tensors.
Dynamic allocation is useful when the array size is not known at compile time and must be set at runtime.

### Example: Dynamic 2D Array
```Fortran 
program dynamic_2d_array
    implicit none
    integer :: nrows, ncols
    integer, allocatable :: A(:,:)

    ! Set dimensions at runtime
    nrows = 3
    ncols = 4

    ! Allocate memory for a 2D array
    allocate(A(nrows, ncols))

    !do something...

    ! Deallocate memory when done
    deallocate(A)
end program
```
> [!TODO]
> Discuss memory deallocation, scope, and memory leaks

This approach can be extended to higher dimensions simply by adding more indices in the allocate statement. 
Since Fortran stores arrays in column-major order, it’s efficient to initialize them by columns.

## Passing Multidimensional Arrays to Procedures

When passing multidimensional arrays to `subroutines` or `functions` in Fortran, you generally have two options: **assumed-shape** and **assumed-size** arrays.

### Assumed-shape arrays
With an **assumed-shape** array, you declare the dummy argument using colons to indicate that the dimensions will be inherited from the caller:
```Fortran
subroutine print_matrix(A)
    integer, intent(in) :: A(:, :)
    print *, "Shape: ", size(A,1), size(A,2)
end subroutine
```
This method is **strongly recommended** in modern Fortran: it allows the procedure **to know the shape of the array** (e.g., number of rows and columns) and enables bounds checking. 
However, it requires an explicit interface — either by placing the subroutine inside a `MODULE` or a `CONTAINS` block.

### Assumed-size arrays
In contrast, an **assumed-size** array is declared using an asterisk in the last dimension:
```Fortran
subroutine legacy_print(A, nrow)
    integer :: A(nrow, *)  ! Assumed-size: only first dimension known
end subroutine
```
This older style **does not carry size information** for the last dimension, making it **unsafe** for most modern applications. It should be avoided unless working with legacy code or interfacing with external libraries.

> [!NOTE]
> - **Use assumed-shape** arrays (`(:, :))` for **clean**, **safe**, and modern code.
> - **Avoid assumed-size** arrays (`(n, *)`) unless necessary for compatibility.
