<!--- - [Pseudo-code algorithm](#pseudo-code-algorithm)
- [FORTRAN implementation](#Fortran-implementation)
- [Types](#Types)
- [The `print` function](#the-print-function)
- [The `write` function](#The-write-function)
- [The `if` statement](#the-if-statement)
- [Functions](#functions)
- [The `do` loop](#the-do-loop)
- [`program`](#program) --->

We now want to write a numerical code to compute
$$\int_0^1 f(x)\ dx$$
with $f(x)=x^3$. Obviously, we already know the solution:
$$\int_0^1 x^3\ dx = \frac{1}{4}$$

> [!TIP]
> Knowing exactly what a numerical code (and, in particular, a small part of a huge code) is expected to do is a key aspect in numerical sicence (sometimes, it can be regarded as a privilege!). In this way, we are sure that (a portion of) the code we implemented works.

We will implement the numerical integration by using the trapezoidal rule.

> ### Trapezoidal Rule [^wiki1]
> <div style="display: flex;">
>   <div style="flex: 1;">
>     In calculus, the trapezoidal rule is a technique for numerical integration, i.e., approximating the definite integral:
>
> $$ \int_a^b f(x) dx \approx \sum_{i=1}^N \frac{f(x_i)+f(x_{i+1})}{2}\Delta x_i $$
>
>   </div>
>   <div style="flex-shrink: 0;">
>     <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/d/d1/Integration_num_trapezes_notation.svg/1920px-Integration_num_trapezes_notation.svg.png" width="250" style="float:right; margin-left: 20px;">
>   </div>
> </div> 
> 

## Pseudo-code algorithm
In pseudo-code, we should do something like:

```fortran
function f(x)  
	return x^3 
end function

function trapezoidal_rule(a, b, N)  
	p = (b-a) / N
	sum = 0.
	do i = 0 to N-1
		x1 = a + i * p
		x2 = a + (i+1) * p
		sum = sum + (f(x1)+f(x2)) * p * 0.5
	return sum
end function

program
	a = 0  
	b = 1  
	N = 1000

	if N>0: print "The number of trapezoid used is N"
	if N<0: print "The number of trapezoid used is negative" stop
	if N=0: print "The number of trapezoid used is zero" stop
	
	result = trapezoidal_rule(a, b, N)  
	print "The integral of f(x) = x^3 from a to b is approximately: result"
	
end program
```

> [!TIP]
> Before implementing the code in Fortran, we notice that there is a more clever way of coding the function `trapezoidal_rule` calling the  function only once per iteration, as the `f(x2)` term at iteration `i` is the same as the term `f(x1)` at iteration `i+1`, and the only cases when this is not true are the  end points, which we can handle separately:  
> ```fortran
> function trapezoidal_rule(a, b, N)  
>	p = (b-a) / N  
>	sum = 0.5 * (f(a) + f(b)) # End points contribution  
>	do i = 1 to N-1	  
>		x = a + i * p     
>		sum = sum + f(x)     
>	return sum * p
> end function
> ```
> This way we already improved the numerical efficiency of our code by almost 100%!
>

> [!IMPORTANT]
> Often directly translating a mathematical formula into an algorithm does not yield
> the best performances, as in the case of the trapezoidal integration.
> When coding, always think if there is a clever way of implementing your algorithms, 
> that minimises the number of calculations and reduces reads and write to memory.


## FORTRAN implementation

We are now ready to look at the FORTRAN implementation:

```Fortran
program trapezoidal_integration
  implicit none
  real(8) :: a, b, result
  integer :: n

  ! Initialize variables
  a = 0.0d0
  b = 1.0d0
  n = 1000

  print*, "This program performs numerical integration of f(x) = x^3 from a = ", a, " to b = ", b, " using ", n, " trapezoids."

  ! Check if n is valid
  if (n > 0) then
    print*, "The number of trapezoids is positive."
  elseif (n < 0) then
    print*, "Error: The number of trapezoids is negative."
    stop
  else
    print*, "Error: The number of trapezoids is zero."
    stop
  endif

  ! Perform numerical integration
  result = trapezoidal_rule(a, b, n)

  print*, "The integral of f(x) = x^3 from ", a, " to ", b, " is approximately: ", result

contains

  ! Function to evaluate f(x) = x^3
  real(8) function f(x)
    real(8), intent(in) :: x
    f = x**3
  end function f

  ! Trapezoidal rule function
  real(8) function trapezoidal_rule(a, b, n)
    real(8), intent(in) :: a, b
    integer, intent(in) :: n
    real(8) :: p, sum, x
    integer :: i

    p = (b - a) / n
    sum = 0.5d0 * (f(a) + f(b))

    do i = 1, n-1
      x = a + i * p
      sum = sum + f(x)
    end do

    trapezoidal_rule = sum * p
  end function trapezoidal_rule

end program trapezoidal_integration
```

We now try to comment every code block:

## Types

FORTRAN supports different data types, but real(8) specifies a double-precision floating-point type, similar to C’s double. This ensures that our calculations maintain high precision.

> [!NOTE]
> Some of the basic data types are:
> 
> - `integer` (4 Bytes): Represents whole numbers, both positive and negative (e.g., `integer :: age = 25;`).
> - `real(4)` (4 Bytes): Represents single-precision floating-point numbers (e.g., `real(4) :: temperature = 98.6;`).
> - `real(8)` (8 Bytes): Represents double-precision floating-point numbers (e.g., `real(8) :: pi = 3.14159;`).
> - `character` (1 Byte): Represents a single character (e.g., `char letter = 'A'`;).

FORTRAN provides the `sizeof()` operator to determine the size of any type or variable in bytes. For example:

```Fortran
integer :: a
print*, "Size of int:", sizeof(a), " bytes"  ! Typically prints 4
```
In FORTRAN, types can also be automatically or explicitly converted:

- **Implicit** type conversion: The compiler automatically converts one data type to another when necessary. In expressions that involve different types (e.g., INTEGER and REAL), the smaller type is automatically promoted to the larger type.
- **Explicit** type conversion (also called "**casting**") is when you manually convert a value from one type to another using intrinsic functions like INT() or REAL().

The following example shows both kind of conversion:

```Fortran
program type_conversion_example
  implicit none
  integer :: a
  real(8) :: b, pi, result
  integer :: truncated_pi

  ! Variable initialization
  a = 5
  b = 2.5d0
  pi = 3.14159d0

  ! Implicit conversion of 'a' (integer) to 'real'
  result = a + b
  print*, "Result (a + b): ", result  ! Output: 7.500000000000000

  ! Explicit conversion of 'pi' (real) to 'integer'
  truncated_pi = int(pi)
  print*, "Pi (truncated): ", truncated_pi  ! Output: 3

end program type_conversion_example
```

> [!WARNING]
> Implicit conversion can lead to precision loss: when converting from a larger or more precise type (like REAL or DOUBLE PRECISION) to a smaller or less precise type (like INTEGER), the conversion can result in a loss of precision or data.
>
> ```Fortran
> real(8) :: pi = 3.14159
> integer :: value = pi ! Implicit conversion truncates decimal part
> ```

## The `print` function

In FORTRAN, the `print*` statement is used to display output to the console. It is a simple way to print values without needing to specify a format explicitly. The asterisk (`*`) indicates that the output format is chosen automatically by the compiler, making it convenient for quick printing of variables. In the specific case of our example, we have:

`print*, "This program performs numerical integration of f(x) = x^3 from a = ", a, " to b = ", b, " using ", n, " trapezoids."`

In this example, `print*` automatically formats and prints the values of `a`, `b` and `n`. However, if more control over the output format is required, using `write` with a specific format is a better option.

Here some examples of ***format specifiers***:

- `A`: Used for character strings.
	- Example: `A10` means a character field of width 10.
- `I`: Used for integers.
	- Example: `I5` means an integer field with a width of 5 characters.
- `F`: Used for floating-point numbers in fixed-point notation.
	- Example: `F8.2` means a floating-point number with a total width of 8 characters, including 2 digits after the decimal point.
- `E`: Used for floating-point numbers in scientific notation.
	- Example: `E10.3` means a floating-point number with a total width of 10 characters and 3 digits after the decimal.
- `X`: Inserts spaces between fields.
	- Example: `X5` inserts 5 spaces.
- `/`: Starts a new line.

> [!WARNING]
> Ensure the format specifier corresponds to the type of the variable. Using the wrong specifier can lead to unexpected output or program crashes.



> ## The `write` function 
> In FORTRAN, the `write` statement is used for more flexible and controlled output compared to `print`. It allows you to specify the format of the output and target different I/O units (like files or the console). The write statement is often paired with a format specifier to control how the output is displayed.
> The syntax is:
> ```fortran 
> write(unit, format) var1, var2, ...
> ```
> where: 
> - `unit` specifies where the output goes (`unit=6` or `*` typically refers to the console); 
> - `format` controls the layout of the output: it can be a label pointing to a format statement or an asterisk (`*`) for automatic formatting.
> Here an example:
> ```fortran
> integer :: a = 10
> real    :: b = 3.14159
> write(*, '(A, I2)')   'Value of a: ', a  ! Output with format
> write(*, '(A, F6.2)') 'Value of b: ', b  ! Output with fixed decimal format
> ! Or using the format statement:
> write(*, 10)   'Value of a: ', a  ! Output with format
> 10 format('(A, I2)')
> ! We can also specify a number for the unit:
> ! In the above example, it will create a file called fort.123 
> write(123, '(A, F6.2)') 'Value of b: ', b 
> ```
> More details on the use of `write` related to the possibility of opening and closing files are provided in the next lecture. 


## The `if` statement

The following code block shows an example of an `if` statement:

```Fortran
if (n > 0) then
    print*, 'The number of trapezoids is positive.'
else if (n < 0) then
    print*, 'Error: The number of trapezoids is negative.'
	stop
else
    print*, 'Error: The number of trapezoids is zero.'
	stop
end if
```

The `if` statement in FORTRAN allows you to control the flow of your program based on conditions. It checks if a certain condition is true and executes a block of code accordingly. If the condition is false, it moves to the next condition or the `else` block.

1.	`if (n > 0)`:
	- This condition checks if the value of `n` is greater than 0.
	- If true, the program executes the code inside this block: it prints “The number of trapezoids is positive.”.
	- If false, it moves to the next condition.
2.	`else if (n < 0)`:
	- This checks if `n` is less than 0.
	- If true, the program prints an error message: “Error: The number of trapezoids is negative.”
	- If false, the program moves to the final else block.
3.	`else`:
	- This block is executed if none of the previous conditions were true. Since the only other possibility is that `n` equals 0, the program prints: “Error: The number of trapezoids is zero.”

You can combine multiple conditions using logical `.AND.` (AND) and `.OR.` (OR) to make your checks more compact.

```Fortran
if (n > 0 .AND. n < 100) then
    print *, 'n is positive and less than 100.'
end if
```

> [!TIP]
> In some cases, you can use a simpler `if` construct for one-liners without else or else if blocks.
> ```Fortran
> if(n > 0) print*, "n is grater than 0."
> ```

> [!WARNING]
> Be cautious with comparisons of floating-point numbers! Due to precision issues, comparing floating-point numbers using `==` can be unreliable. It is better to use a small tolerance instead of direct equality checks.
> ```Fortran
> if (abs(a - b) < 0.00001) then  ! a and b are "close enough"
>    ! Handle case where a is approximately equal to b
> end if
> ```


## Functions

Let's look at the first function called `f`:

```Fortran
real(8) function f(x)
	real(8), intent(in) :: x
	f = x**3
end function f}
```

- The first line `function f(x)` defines a function named `f` that takes a single argument `x`. In FORTRAN, the data types of both the function and the argument are declared separately. In this case, `x` and `f` are both declared as `real(8)` (i.e., a floating-point number with double precision).
- The logic of the function is contained between the lines `function f(x)` and `end function f`. - The line `f = x**3` computes `x` raised to the power of 3 using the `**` operator (in FORTRAN, `**` is used for exponentiation). The result is assigned to `f`, which is the name of the **function** and also serves as the **variable** that holds the return value.

> [!NOTE]
> The general syntax used for defining functions is:
> 
> ```fortran
> kind function name_function(arg1,arg2,...)
> kind :: arg1
> kind :: arg2
> ...
> !do something
> end function
> ```
>
>Another way to define them is the following:
> 
> ```fortran
> function name_function(arg1,arg2,...)
> kind :: name_function
> kind :: arg1
> kind :: arg2
> ...
> !do something
> end function
> ```

In Fortran, the `intent` attribute is used in procedure arguments to specify how the arguments are intended to be used within the subroutine or function. This feature enhances code clarity, facilitates debugging, and enables better optimization by the compiler. The three main types of intent are:

1. `intent(in)`: This indicates that the argument is used for **input** only. The procedure can read the value of the argument, but it should not modify it. This helps ensure that the original value remains unchanged.
2. `intent(out)`: This specifies that the argument will be used for **output** only. The procedure is expected to assign a value to this argument before the procedure ends. The initial value of the argument is considered undefined.
3. `intent(inout)`: This indicates that the argument will be used for both **input and output**. The procedure can read the value of the argument and also modify it. The original value may be used as input, and a new value is assigned before the procedure finishes.

> [!TIP]
> By specifying the intended use of variables, you reduce the risk of unintended side effects and make your code safer. Moreover, the compiler can better optimize the code when it knows how variables are being used, potentially improving performance. Finally, it can serves as documentation within the code, making it clearer to users and maintainers what each variable’s role is.



## The `do` loop

In the second function defined in the example code, we can see an example of ***for loop***:

```Fortran
do i = 1, n-1
	x = a + i * p
	sum = sum + f(x)
end do
```

The first line `do i = 1, n-1` sets up a do loop that will iterate from `i = 1` to `i = n-1`. The loop performs the following:
- `i = 1` initializes the loop counter `i` to 1.
- `n-1` specifies the final value of `i`. The loop will continue until `i` reaches `n-1`.
- Inside the loop, `x = a + i * p` and `sum = sum + func(x)` are executed for each iteration.

The `end do` marks the end of the loop.

> In FORTRAN, the `do` loop can be used both with and without a loop counter. When used without a counter, the `do` loop acts more like a general loop that can run until a specific condition is met, such as through an `exit` or `cycle` statement. The 'do' loop without a counter is useful when the number of iterations is not known in advance and depends on some runtime condition.
> Here is an example of a do loop without a counter:
> ```fortran
> program doWithoutCounter
>     implicit none
>     integer :: i = 1
>     do
>         print *, 'Iteration: ', i
>         i = i + 1
>         if (i > 5) exit   ! Exit loop when i exceeds 5
>     end do
> end program doWithoutCounter
> ```
> The loop runs indefinitely until the `exit` condition is met, which is used to break out of the loop when a condition is satisfied. Alternatively, `cycle` can be used to skip to the next iteration without completing the remaining statements in the current iteration. 

## `program`

In FORTRAN, the `program` block is used to define the main part of a program. Every FORTRAN program must begin with the `program` keyword, followed by a name (optional but recommended), and ends with the `end program` statement.


```Fortran
program myProgram
    implicit none
    ! Variable declarations
    integer :: a, b, result
    
    ! Initialize variables
    a = 5
    b = 10

    ! Perform a simple calculation
    result = a + b
    print *, 'The sum is: ', result

end program myProgram
```

> [!TIP]
> `implicit none` is recommended to avoid automatic type assignment, forcing explicit declarations of all variables. You should write `implicit none` in each program as well as in functions and subroutines. 


## References
[^wiki1]: https://en.wikipedia.org/wiki/Trapezoidal_rule
