# Exercises related to arrays and pointers

Here some exercises with the solutions in C and FORTRAN. Feel free to start from them and expand them as you wish. For example, for some of them can be usefull to print the output! 

## Exercise 1: Vector-vector sum

Write a program that sum the following vectors:
```
arr1 = [0,1,2,3,4,5,6,7,8,9]
arr2 = [0,10,20,30,40,50,60,70,80,90]
```
Initialise them with a for loop and write a function that perform the sum. 

### Solution

<details>
  <summary>C code</summary>
  
  ```C
  #include <stdio.h>
  #include <stdlib.h>
  
  void sum_arrays(int *arr1, int *arr2, int *arr3, int N){
    for(int i = 0;i<N; i++){
      arr3[i] = arr1[i] + arr2[i];
    }
    return;
  }
  
  
  int main(){
    int arr1[10], arr2[10], arr3[10];
    int N = sizeof(arr1)/sizeof(arr1[0]); 
  
    for(int i=0;i<N;i++){
      arr1[i] = i;
      arr2[i] = i*10;
    }
  
    sum_arrays(arr1, arr2, arr3, N);
  
    for(int i=0;i<N;i++){
      printf("%d + %d = %d\n", arr1[i],arr2[i],arr3[i]);
    }
  
    return 0;
  }
  ```

</details>

<details>
  <summary>FORTRAN code</summary>

  ```Fortran
  program sum_arrays_example
      implicit none
      integer, parameter :: N = 10
      integer :: arr1(N), arr2(N), arr3(N)
      integer :: i
  
      ! Initialize the arrays
      do i = 1, N
          arr1(i) = i - 1          ! Fill arr1 with 0, 1, ..., 9
          arr2(i) = (i - 1) * 10   ! Fill arr2 with 0, 10, ..., 90
      end do
  
      ! Call the subroutine to sum the arrays
      call sum_arrays(arr1, arr2, arr3, N)
  
      ! Print the results
      do i = 1, N
          print *, arr1(i), ' + ', arr2(i), ' = ', arr3(i)
      end do
  
  contains
  
      ! Subroutine to sum the arrays
      subroutine sum_arrays(arr1, arr2, arr3, N)
          integer, intent(in) :: arr1(N), arr2(N)
          integer, intent(out) :: arr3(N)
          integer :: i
  
          do i = 1, N
              arr3(i) = arr1(i) + arr2(i)
          end do
      end subroutine sum_arrays
  
  end program sum_arrays_example
  ```

</details>

## Exercise 2: Bubble sorting

Bubble sort is a simple sorting algorithm that repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. This process is repeated until no more swaps are needed, meaning the list is fully sorted. After each full pass through the list, the largest unsorted element “bubbles” up to its correct position, reducing the portion of the list that needs sorting. The algorithm continues until the entire list is sorted.

Write a code that generates $N$ random numbers in the range 0, 999 (where $N$ is passed from command line) and sorts them in increasing order using the bubble sorting algorithm. 

To help the implementation of the bubble sorting, we also provide a pseudo code.

<details>
  <summary>Pseudo code</summary>

  ```plain
  function bubble_sort(arr, N):
      pass = 0
      flag_iterate = true
  
      while flag_iterate:
          flag_iterate = false
          pass = pass + 1
  
          for i = 0 to N - 2:
              if arr[i+1] < arr[i]:
                  swap arr[i] and arr[i+1]
                  flag_iterate = true
  
      return arr
  ```
</details>

### Solution
<details>
  <summary>C code</summary>
  
  ```C
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <time.h>
  // Bubble sort implementation
  void bubble_sorting(int N, int *arr){
      int hold;
      bool flag_iterate = true;
      int pass = 0;
  
      while (flag_iterate) {
          pass += 1;
          flag_iterate = false;
  
          // Bubble sorting pass
          for (int i = 0; i < N - 1; i++) {
              if (arr[i + 1] < arr[i]) {
                  flag_iterate = true;
                  hold = arr[i];
                  arr[i] = arr[i + 1];
                  arr[i + 1] = hold;
              }
          }
  
          // Output the array after each pass
          printf("OUTPUT: ");
          for (int i = 0; i < N; i++) {
              printf("%d ", arr[i]);
          }
          printf("\n");
      }
  
      printf("Total number of steps: %d\n", pass);
  } 
  
  int main() {
      int N;
      printf("Enter the number of elements: ");
      scanf("%d", &N);
  
      // Check if N is valid
      if (N <= 0) {
          printf("Error: Number of elements should be greater than 0.\n");
          return 1;
      }
  
      // Allocate memory for the array
      int *arr = malloc(N * sizeof(int));
      if (arr == NULL) {
          printf("Error: Memory allocation failed.\n");
          return 1;
      }
  
      // Seed the random number generator
      srand(time(0));
  
      // Generate random numbers and fill the array
      printf("INPUT:  ");
      for (int i = 0; i < N; i++) {
          arr[i] = rand() % 1000;  // Random numbers between 0 and 999
          printf("%d ", arr[i]);
      }
      printf("\n");
  
      // Perform bubble sort
      bubble_sorting(N, arr);
  
      // Free the allocated memory
      free(arr);
  
      return 0;
  }
  ```
</details>

<details>
  <summary>FORTRAN code</summary>
  
  ```Fortran
  program bubble_sort
      implicit none
      integer :: N, i, temp, pass
      logical :: flag_iterate
      real    :: tmp
      integer, allocatable :: arr(:)
  
      ! Ask for number of elements
      print *, "Enter the number of elements: "
      read(*, *) N
  
      ! Allocate the array
      allocate(arr(N))
  
      ! Generate random values for the array
      call random_seed()  ! Set random seed
      write(*, '(A8)', advance='no') "INPUT: "
      do i = 1, N
          call random_number(tmp)  ! Random number between 0 and 1
          arr(i) = int(tmp * 1000) ! Scale to get values between 0 and 1000
          write(*, '(I5)', advance='no') arr(i)
      end do
      print *, ""
  
      ! Bubble Sort algorithm
      pass = 0
      flag_iterate = .true.
      
      do while (flag_iterate)
          pass = pass + 1
          flag_iterate = .false.
          do i = 1, N - 1
              if (arr(i+1) < arr(i)) then
                  temp = arr(i)
                  arr(i) = arr(i+1)
                  arr(i+1) = temp
                  flag_iterate = .true.
              end if
          end do
  
          ! Print intermediate output after each pass
          write(*, '(A8)', advance='no') "OUTPUT: "
          do i = 1, N
              write(*, '(I5)', advance='no') arr(i)
          end do
          print *, ""
      end do
  
      ! Output the total number of passes
      print *, "Total number of steps: ", pass
  
      ! Deallocate the array
      deallocate(arr)
  end program bubble_sort
  ```
</details>

### Variation1: read from input and write to output
In this variation, the numbers to be ordered are read from input and the result is directed to output. The name of the input and output files are provided by the user: `./exe input_file output_file`. 
Note that your code must be able to count how many elements are given in the input file. 

<details>
  <summary>C code</summary>
  
  ```C
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  
  // Bubble sort implementation
  void bubble_sorting(int N, int *arr, FILE *output_file) {
    int hold;
    bool flag_iterate = true;
    int pass = 0;
  
    while (flag_iterate) {
      pass += 1;
      flag_iterate = false;
  
      // Bubble sorting pass
      for (int i = 0; i < N - 1; i++) {
        if (arr[i + 1] < arr[i]) {
          flag_iterate = true;
          hold = arr[i];
          arr[i] = arr[i + 1];
          arr[i + 1] = hold;
        }
      }
  
      // Output the array after each pass
      fprintf(output_file, "OUTPUT after pass %d: ", pass);
      for (int i = 0; i < N; i++) {
        fprintf(output_file, "%d ", arr[i]);
      }
      fprintf(output_file, "\n");
    }
  
    fprintf(output_file, "Total number of steps: %d\n", pass);
  }
  
  int count_numbers_in_file(FILE *input_file) {
    int count = 0;
    int temp;
  
    // Count the number of integers in the file
    while (fscanf(input_file, "%d", &temp) != EOF) {
      count++;
    }
  
    // Reset the file pointer to the beginning for the next read
    rewind(input_file);
  
    return count;
  }
  
  int main(int argc, char *argv[]) {
    int N;
    char input_filename[100], output_filename[100];
  
    // Check if less (or more) then 3 arguments are given in command line
    if (argc != 3) {
      fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
      return 1;
    }
  
    // Open input file for reading
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) { // Check open
      printf("Error opening input file");
      return 1;
    }
  
    // Open output file for writing
    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL) { // Check open
      printf("Error opening output file");
      fclose(input_file); // Close input_file before exit
      return 1;
    }
  
  
    // Count the number of integers in the input file
    N = count_numbers_in_file(input_file);
  
    // Check if N is valid
    if (N <= 0) {
      fprintf(output_file, "Error: No valid elements found in the file.\n");
      fclose(input_file);
      fclose(output_file);
      return 1;
    }
  
    // Allocate memory for the array
    int *arr = malloc(N * sizeof(int));
    if (arr == NULL) {
      fprintf(output_file, "Error: Memory allocation failed.\n");
      fclose(input_file);
      fclose(output_file);
      return 1;
    }
  
    // Read the elements from the input file
    for (int i = 0; i < N; i++) {
      fscanf(input_file, "%d", &arr[i]);
    }
  
    // Perform bubble sort and write to output file
    bubble_sorting(N, arr, output_file);
  
    // Free the allocated memory
    free(arr);
  
    // Close the files
    fclose(input_file);
    fclose(output_file);
  
    printf("Sorting complete. Results written to %s\n", argv[2]);
  
    return 0;
  } 
  ```
</details>

## Exercise 3: 2D Matrix Addition Using Dynamic Memory Allocation

In this exercise, you are required to write a program that dynamically allocates memory for two square matrices, $A$ and $B$, of size $N \times N$ , and computes their sum in a third matrix $C$. 

Here a _to do_ list that can help:

1.	Ask the user to input the size of the matrix  $N$  (which must be greater than 0).
2.	Dynamically allocate memory for three matrices $A$, $B$, and $C$, each of size  $N \times N$.
3.	Populate the matrices $A$ and $B$ with some values (e.g., fill matrix $A$ with values  $i \times N + j$  and matrix B with values  $i \times N + j + 100$. where $i$ and $j$ are the row and column indices, respectively).
4.	Create a function `sum_matrices` that takes the two input matrices `A` and `B` and computes their sum into matrix `C`.
5.	Print out the resulting matrix $C$ after computing the sum.
6.	Ensure that you free the allocated memory before exiting the program to avoid memory leaks.

<details>
  <summary>C code</summary>
  
  ```C
  #include <stdio.h>
  #include <stdlib.h>
  
  // Function to sum two NxN matrices A and B, storing the result in C
  void sum_matrices(float **A, float **B, float **C, int N){
    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        *(*(C+i)+j) = *(*(A+i)+j) + *(*(B+i)+j);
      }
    }
  }
  
  int main(int argc, char* argv[]){
  
    int N;
    float **A, **B, **C;
  
    // Prompt user for input
    printf("A and B are NxN matrices. Insert the value of N:\n");
    scanf("%d", &N);
    if(N <= 0){
      printf("N must be greater than 0.\n");
      return 1;
    }
  
    // Allocate memory for matrices A, B, and C
    A = (float **)malloc(N * sizeof(float *));
    B = (float **)malloc(N * sizeof(float *));
    C = (float **)malloc(N * sizeof(float *));
    if (A == NULL || B == NULL || C == NULL) {
      printf("Memory allocation failed.\n");
      return 1;
    }
  
    for(int i=0; i<N; i++){
      A[i] = (float *)malloc(N * sizeof(float));
      B[i] = (float *)malloc(N * sizeof(float));
      C[i] = (float *)malloc(N * sizeof(float));
      if (A[i] == NULL || B[i] == NULL || C[i] == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
      }
    }
  
    // Populate matrices A and B
    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        A[i][j] = i * N + j;
        B[i][j] = i * N + j + 100;
      }
    }
  
    // Compute sum: C = A + B
    sum_matrices(A, B, C, N);
  
    // Print matrix C
    printf("Matrix C = A + B:\n");
    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        printf("C[%d][%d] = %f\n", i, j, C[i][j]);
      }
    }
  
    // Free allocated memory
    for(int i=0; i<N; i++){
      free(A[i]);
      free(B[i]);
      free(C[i]);
    }
    free(A);
    free(B);
    free(C);
  
    return 0;
  }
```

</details>

<details>
  <summary>C code (variation n. 1)</summary>
  In this variation, two functions are coded to allocate and deallocate the pointers. Moreover, the function `clock()` is invoked to measure the time the code takes to sum the matrices.

  ```C
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>
  
  // Function prototypes [to declare the functions before their definitions, so they can be used in the main()]
  void sum_matrices(float **, float **, float **, int);
  float **my_malloc(int);
  void my_free(float **, int);
  
  int main(int argc, char* argv[]){
    int N;
  
    // Prompt user for input
    printf("A and B are NxN matrices. Insert the value of N:\n");
    scanf("%d", &N);
    if(N <= 0){
      printf("N must be greater than 0.\n");
      return 1;
    }
  
    // Allocate memory for matrices A, B, and C
    float **A = my_malloc(N);
    float **B = my_malloc(N);
    float **C = my_malloc(N);
  
    // Populate matrices A and B
    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        A[i][j] = i * N + j;
        B[i][j] = i * N + j + 100;
      }
    }
  
    // Compute sum: C = A + B
    float ts = clock();
    sum_matrices(A, B, C, N);
    float te = clock();
  
    printf("Time = %f ms\n", (te-ts)/CLOCKS_PER_SEC*1000);
  
    // Print matrix C
    printf("Matrix C = A + B:\n");
    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        printf("C[%d][%d] = %f\n", i, j, C[i][j]);
      }
    }
  
    // Free memory
    my_free(A,N);
    my_free(B,N);
    my_free(C,N);
  
    return 0;
  }
  
  // Function to sum two NxN matrices A and B, storing the result in C
  void sum_matrices(float **A, float **B, float **C, int N){
    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        *(*(C+i)+j) = *(*(A+i)+j) + *(*(B+i)+j);
      }
    }
  }
  
  // Implementation of the allocation function
  float **my_malloc(int N){
    float **array = (float **)malloc(N * sizeof(float *));
    if (array == NULL) {
      printf("Memory allocation failed.\n");
    }
    for(int i=0; i<N; i++){
      array[i] = (float *)malloc(N * sizeof(float));
      if (array[i] == NULL) {
        printf("Memory allocation failed.\n");
      }
    }
    return array;
  }
  
  // Implementation of the free function
  void my_free(float **array, int N) {
      for (int i = 0; i < N; i++) {
          free(array[i]);  // Free each row
      }
      free(array);  // Free the array of row pointers
  }
  ```

</details>

<details>
  <summary>FORTRAN code</summary>
  
  ```Fortran
  program main
    implicit none
  
    integer :: N, i, j
    real, dimension(:,:), allocatable :: A, B, C

    ! Prompt user for input
    print*, "A and B are NxN matrices. Insert the value of N:"
    read(*,*) N
    if(N<=0)then
      print*, "N must be grater than 0."
      stop
    endif

    ! Allocate memory for matrices A, B, and C
    allocate(A(N,N),B(N,N),C(N,N))

    ! Populate matrices A, B, and C
    do j = 1, N
      do i = 1, N
        A(i,j) = (i-1)*N+(j-1)
        B(i,j) = (i-1)*N+(j-1)+100
      enddo
    enddo
  
    ! Compute sum C = A + B
    call sum_matrices(A,B,C)
  
    ! Print matrix C
    do j = 1, N
      do i = 1, N
        print*,"C[",i,"][",j,"] = ", C(i,j)
      enddo
    enddo
  
    deallocate(A,B,C)
  
  contains

    ! Function to sum two NxN matrices A and B, storing the result in C
    subroutine sum_matrices(A, B, C)
      implicit none
      real, dimension(:,:) :: A, B, C
      C = A + B
    end subroutine
  
  end program
  ```
</details>


## Exercise 4: numerical derivative
Implement a program to compute the numerical derivative of a function at a given point using _function pointers_. 
This exercise will help you become familiar with passing functions as arguments and comparing numerical derivatives with their theoretical counterparts.

Problem Statement:

You are tasked with writing a program that:

1.	Prompts the user to select a mathematical function from the following options:
  - `sqrt(x)`
  - `sin(x)`
  - `cos(x)`
2.	Computes the numerical derivative of the chosen function at a given point (`x0 = 1.0`), using the formula:

$$f{\prime}(x) \approx \frac{f(x + \epsilon) - f(x)}{\epsilon}$$

where $\epsilon$ is a small number that starts from 1 and decreases by a factor of 10 in each iteration.

3.	Compares the computed numerical derivative to the theoretical derivative of the function.
4.	Outputs both the numerical and theoretical derivatives for comparison and displays the error percentage between them.
5.	Stops the calculation when epsilon becomes too small (around 1 \times 10^{-16}).

<details>
  <summary>C code</summary>
  
```C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double func1(double x) {
  return sqrt(x);
}
double deriv_func1(double x) {
  return 0.5*pow(x,-0.5);
}

double func2(double x) {
  return sin(x);
}
double func3(double x) {
  return cos(x);
}

// Generic numerical derivative function
double derivative(double (*f)(double), double x, double epsilon){
  return (f(x+epsilon) - f(x)) / epsilon;
}

int main(int argc, char *argv[]){

  double epsilon = 1; 
  double x0 = 1.;
  double (*func)(double);
  double deriv_th, deriv_num;

  if (argc < 2) {
    printf("Usage: %s <function>:\n", argv[0]);
    printf("<1>: sqrt(x)\n");
    printf("<2>: sin(x)\n");
    printf("<3>: cos(x)\n");
    return 1;
  }

  // Read the number of trapezoids from the command line
  int function = atoi(argv[1]);

  // Select the appropriate function and theoretical derivative
  switch (function) {
    case 1:
      func = func1; 
      deriv_th = deriv_func1(x0); // Exact derivative of sqrt(x)
      break;
    case 2:
      func = func2; 
      deriv_th = cos(x0);       // Exact derivative of sin(x)
      break;
    case 3:
      func = func3;
      deriv_th = -sin(x0);      // Exact derivative of cos(x)
      break;
    default:
      printf("Invalid function choice! Please choose 1, 2, or 3.\n");
      return 1;  // Exit if invalid function
  }

  printf("---------------------------------------\n");
  printf("The derivative in %f is %f\n", x0, deriv_th);
  printf("---------------------------------------\n");

  printf("#epsilon #num #err\n"); 
  while (epsilon > 1.e-18) {
    deriv_num = derivative(func, x0, epsilon);
    printf("%e %f %f %% \n", epsilon, deriv_num, fabs((deriv_num-deriv_th)/deriv_th)*100.);
    epsilon /= 10.;
  }
  return 0;
}
```

</details>

<details>
  <summary>FORTRAN code</summary>
  
```FORTRAN
program derivative_calculator
  implicit none
  real(8) :: epsilon, x0, deriv_th, deriv_num
  integer :: function_choice

  ! Input handling
  print *, 'Choose a function to differentiate:'
  print *, '1: sqrt(x)'
  print *, '2: sin(x)'
  print *, '3: cos(x)'
  read(*, *) function_choice

  ! Set the value of x0 and epsilon
  x0 = 1.0d0
  epsilon = 1.0d0

  ! Call the appropriate function based on the user's choice
  select case (function_choice)
    case (1)
      deriv_th = deriv_func1(x0)
      print *, '---------------------------------------'
      print *, 'The derivative of sqrt(x) at ', x0, ' is ', deriv_th
      print *, '---------------------------------------'
      call compute_derivative(func1, x0, epsilon, deriv_th)

    case (2)
      deriv_th = cos(x0)
      print *, '---------------------------------------'
      print *, 'The derivative of sin(x) at ', x0, ' is ', deriv_th
      print *, '---------------------------------------'
      call compute_derivative(func2, x0, epsilon, deriv_th)

    case (3)
      deriv_th = -sin(x0)
      print *, '---------------------------------------'
      print *, 'The derivative of cos(x) at ', x0, ' is ', deriv_th
      print *, '---------------------------------------'
      call compute_derivative(func3, x0, epsilon, deriv_th)

    case default
      print *, 'Invalid function choice! Please choose 1, 2, or 3.'
      stop
  end select
contains 

  ! Function for computing numerical derivative
  subroutine compute_derivative(f, x, epsilon, deriv_th)
    implicit none
    real(8), external   :: f
    real(8), intent(in) :: x, epsilon, deriv_th
    real(8)             :: deriv_num, epsilon_tmp

    epsilon_tmp = epsilon
    print *, '#epsilon          #num              #err(%)'

    do while (epsilon_tmp > 1.0d-18)
      deriv_num = (f(x + epsilon_tmp) - f(x)) / epsilon_tmp
      print *, epsilon_tmp, deriv_num, abs((deriv_num - deriv_th) / deriv_th) * 100.0d0
      epsilon_tmp = epsilon_tmp / 10.0d0
    end do
  end subroutine

  ! Define the mathematical functions
  real(8) function func1(x)
    real(8), intent(in) :: x
    func1 = sqrt(x)
  end function

  real(8) function deriv_func1(x)
    real(8), intent(in) :: x
    deriv_func1 = 0.5d0 * (x**(-0.5d0))
  end function

  real(8) function func2(x)
    real(8), intent(in) :: x
    func2 = sin(x)
  end function

  real(8) function func3(x)
    real(8), intent(in) :: x
    func3 = cos(x)
  end function
end program
```

</details>

