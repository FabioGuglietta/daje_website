# File Input/Output: Reading and Writing Files
Consider the following task: we want to read from an external file some $x$ and $y$ coordinates. The file is organised as follow:

We have some particles made of discrete points. Each particle has $N_p$ points with $x,y,z$ coordinates. Such coordinates are included in a `txt` file like this:

```plaintext
number_of_coordinates name_dataset
point_id coord_x coord_y coord_z
point_id coord_x coord_y coord_z
point_id coord_x coord_y coord_z
...
```

For example

```plaintext
4 Particle_A
1 1.0 2.0 3.0
2 4.0 5.0 6.0
3 7.0 8.0 9.0
4 10.0 11.0 12.0
```

is a particle made of 4 points with $(x,y,z)$-coordinates. The name of the particle is `Particle_A`.

We want to write a program that can be executed like this

```
./test.exe input_file output_file
```

that is, we want to specify from command line the name of the input and output files. 

The code will read the coordinates of the input file and will compute the centroid of the particle.

Here the pseudo code:

```plaintext
FUNCTION calculate_centroid(points_x, points_y, points_z, n) 
	centroid_x=0; centroid_y=0; centroid_z=0;
    FOR i = 0, n-1
        centroid_x += points_x[i];
        centroid_y += points_y[i];
        centroid_z += points_z[i];
    centroid_x /= n; centroid_y /= n; centroid_z /= n;
	 RETURN centroid_x, centroid_y, centroid_z

MAIN PROGRAM

	 READ(input_file,points_x,points_y,points_z) //Read the input_file and store the n coordinates in points_x, points_y, points_z
	
	centroid_x,centroid_y,centroid_z = calculate_centroid(points_x, points_y, points_z, n) 
	
	WRITE(output_file,"Set name", set_name)
	WRITE(output_file,"Number of points", n)
	WRITE(output_file,"The centroid of the points is at", centroid_x,centroid_y,centroid_z)
	WRITE(output_file,"Coordinates of the points:")
	FOR i = 0, n-1
		WRITE("Point:", points_id[i], points_x[i], points_y[i], points_z[i])
	
	END PROGRAM
```

Here is the Fortran code:

```Fortran
program centroid_calculation
  implicit none
  integer            :: n, i
  real(8)            :: centroid_x, centroid_y, centroid_z
  character(len=100) :: set_name

  type Point3D
     integer :: id
     real(8) :: x, y, z
  end type 

  type(Point3D), allocatable :: points(:)
  
  character(len=255) :: input_filename, output_filename
  integer            :: input_unit, output_unit, ios

  ! Check if the correct number of command-line arguments are given
  call get_command_argument(1, input_filename)
  call get_command_argument(2, output_filename)
  if (len_trim(input_filename) == 0 .or. len_trim(output_filename) == 0) then
     print *, "Usage: <program> <input_file> <output_file>"
     stop 1
  end if

  ! Open input and output files
  open(unit=input_unit, file=trim(input_filename), status='old', action='read', iostat=ios)
  if (ios /= 0) then
     print *, "Error opening input file"
     stop 1
  end if

  open(unit=output_unit, file=trim(output_filename), status='replace', action='write', iostat=ios)
  if (ios /= 0) then
     print *, "Error opening output file"
     close(input_unit)
     stop 1
  end if

  ! Read number of points and set name from input file
  read(input_unit, *) n, set_name

  ! Allocate memory for the array of points
  allocate(points(n))

  ! Read points data (ID, x, y, z) from the input file
  do i = 1, n
     read(input_unit, *) points(i)%id, points(i)%x, points(i)%y, points(i)%z
  end do

  ! Calculate the centroid of the points
  call calculate_centroid(points, n, centroid_x, centroid_y, centroid_z)

  ! Write the results to the output file
  write(output_unit, '(A)') "Set Name: " // trim(set_name)
  write(output_unit, '(A, I0)') "Number of points: ", n
  write(output_unit, '(A, F7.2, A, F7.2, A, F7.2, A)') "The centroid of the points is at (", centroid_x, ", ", centroid_y, ", ", centroid_z, ")"
  write(output_unit, '(A)') "Data of the points:"

  do i = 1, n
     write(output_unit, '(A, I0, A, F7.2, A, F7.2, A, F7.2, A)') "Point ", points(i)%id, ": (", points(i)%x, ", ", points(i)%y, ", ", points(i)%z, ")"
  end do

  ! Clean up
  deallocate(points)
  close(input_unit)
  close(output_unit)

  print *, "Centroid calculation complete. Results written to ", trim(output_filename)

contains

  ! Function to calculate the centroid of an array of 3D points
  subroutine calculate_centroid(points, n, centroid_x, centroid_y, centroid_z)
    type(Point3D), intent(in) :: points(:)
    integer, intent(in) :: n
    real(8), intent(out) :: centroid_x, centroid_y, centroid_z
    integer :: i

    centroid_x = 0.0
    centroid_y = 0.0
    centroid_z = 0.0

    do i = 1, n
       centroid_x = centroid_x + points(i)%x
       centroid_y = centroid_y + points(i)%y
       centroid_z = centroid_z + points(i)%z
    end do

    centroid_x = centroid_x / real(n)
    centroid_y = centroid_y / real(n)
    centroid_z = centroid_z / real(n)
  end subroutine

end program
```

# FILE

## Open a file

In Fortran, the `open` statement is used to open a file for reading, writing, or both. 
In our example
```Fortran
open(unit=input_unit, file=trim(input_filename), status='old', action='read', iostat=ios)
```
The general syntax is: 
```Fortran
open(unit=<integer>, file=<filename>, [status=<status>], [action=<action>], [iostat=<ios_var>])
```
where:
- `unit`: This specifies the logical unit number (an integer) to be associated with the file. It is a reference for file operations like read and write.
- `file`: This specifies the name of the file as a string. The file must exist (or be created, depending on the `status` attribute) at the specified path.
- `status` (optional, default = `unknown`): This controls how the file should be handled:
	- `old` ensures the file must exist.
	- `new` creates a new file and raises an error if the file already exists.
	- `replace` creates a new file or overwrites an existing one.
	- `unknown` allows either reading from or creating the file, depending on its existence.
- `action` (optional, default = `readwrite`): This specifies whether the file is to be opened for `read`, `write`, or `readwrite` operations.
- `form` (optional, default = `formatted`): Specifies whether the file is opened for `formatted` or `unformatted` I/O. `formatted` is used for human-readable text files, while `unformatted` is for binary files. [default is usually 'formatted'
- `iostat` (optional): If provided, this integer variable will store the status of the operation. A value of 0 indicates success, while a nonzero value indicates an error (e.g., file not found).


## Close a file

The `close` statement is used to close an open file that has been previously accessed using the `open` statement. Properly closing files ensures that any pending I/O operations are completed, resources are released, and that no further operations can be performed on the file until it is reopened.

The general syntax for the CLOSE statement is:
```Fortran
close ([unit=]unit_number [, iostat=ios] [, status=sta] [, err=err_label])
```

## Read from a file

In out example, the line
```Fortran
read(input_unit, *) n, set_name
```
is used for reading `n` and `set_name` from the file whose `unit` is `input_unit`. The second argument of `read` is used to specify the format. If the `*` is used, Fortran will automatically match the input data to the variables `n` and `set_name` without requiring you to specify a format string.


### Format specifiers

Fortran provides a powerful and flexible formatting system for reading from and writing to files or standard I/O. The FORMAT statement (or its shorthand within READ, WRITE, and PRINT statements) allows you to control the appearance of the data input/output. This section explains various format specifiers available in Fortran and how to use them effectively.

The general syntax of a FORMAT statement is:
```fortran
FORMAT(format-specifier-list)
```
Alternatively, in the READ, WRITE, or PRINT statement, the format specifier can be placed directly in parentheses:
```fortran
WRITE(unit, format-specifier) var1, var2, ...
READ(unit, format-specifier) var1, var2, ...
```
for example:
```fortran
WRITE(*, '(I5, F10.2)') 123, 3.14159
```
This writes an integer in a field of width 5 and a floating-point number in a field of width 10 with 2 decimal places.

Fortran format specifiers define how different types of variables are formatted during input/output. Here are the most common format specifiers:
- **Integer** Format: `Iw`
 	- `I` stands for an integer.
  	- `w` specifies the width of the output field.
  	
  	For example: `WRITE(*, '(I5)') 123   ! Output: "  123"` where `I5` prints an `integer` in a field of 5 characters. If the `integer` has fewer than 5 digits, it is right-aligned, and spaces are added on the left.
 	
- **Floating-Point** Format: `Fw.d`
	- `F` is for real (floating-point) numbers.
	- `w` specifies the total width of the field.
	- `d` specifies the number of digits after the decimal point.

	For example: `WRITE(*, '(F10.2)') 3.14159  ! Output: "     3.14"` where `F10.2` writes a floating-point number in a field of width 10, with 2 digits after the decimal point. 

- **Exponential** Format: `Ew.d` or `ESw.d`
	- `E` is used for real numbers in scientific notation.
	- `w` specifies the total width of the field.
	- `d` specifies the number of digits after the decimal point.

	For example: `WRITE(*, '(E12.4)') 12345.6789  ! Output: "  0.1235E+05"` where `E12.4` prints a number in scientific notation, with a total width of 12 and 4 digits after the decimal point.

- **Character** Format: `Aw`
	- `A` is used for character strings.
	- `w` specifies the width of the output field.

	For example: `WRITE(*, '(A10)') 'Fortran'  ! Output: "Fortran   "` where `A10` prints a string in a field of width 10.

	`A` is a shorthand for printing a string of any length without specifying the width. For example `WRITE(*, '(A)') 'Fortran is great!'  ! Output: "Fortran is great!"`

A complete list of format specifiers in Fortran can be found at this [link](https://docs.oracle.com/cd/E19957-01/805-4939/z40007437a2e/index.html).
