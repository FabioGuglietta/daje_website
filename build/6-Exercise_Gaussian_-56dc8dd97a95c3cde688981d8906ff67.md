# Gaussian quadrature

Gaussian quadrature is a powerful numerical integration technique designed to achieve high accuracy using a carefully chosen set of sample points and weights. Unlike basic methods of numerical integration, such as the trapezoidal or Simpson’s rule, which use equally spaced points, Gaussian quadrature selects both the locations of the points and their associated weights in an optimized way. This flexibility allows Gaussian quadrature to approximate integrals with fewer evaluations, often with remarkably high precision.

The key idea behind Gaussian quadrature is to approximate the integral of a function  $f(x)$  over an interval by summing the function’s values at specific points  $x_j$, each multiplied by a weight $w_j$. For an integral of the form: $\int_a^b f(x) \, dx$, Gaussian quadrature approximates it as:

$$\int_a^b f(x)\ dx \approx \sum_{j=1}^N w_j f(x_j)\ ,$$

where $N$ is the number of points used in the approximation.

Unlike basic methods, Gaussian quadrature does not restrict these points to be evenly spaced. This freedom in choosing both weights and points effectively doubles the degrees of freedom, allowing Gaussian quadrature to attain a higher-order accuracy than traditional methods like Newton-Cotes for the same number of evaluations.

Another key advantage of Gaussian quadrature is that it can be customized for specific classes of functions. By selecting appropriate weights and abscissas, we can design quadrature formulas that are tailored to integrands of kind “polynomials times some known function W (x)” rather than for the usual class of integrands “polynomials”.
Given such a weight function $W(x)$  and a fixed number of points $N$, we can determine weights $w_j$ and points $x_j$ that make the approximation

$$
\int_a^b W(x) f(x) \, dx \approx \sum_{j=1}^N w_j f(x_j)
$$

exact when $f(x)$ is a polynomial.

Here some examples of weight functions:

**Gauss-Legendre:**

$$
W(x) = 1 \quad -1 < x < 1
$$

**Gauss-Chebyshev:**

$$
W(x) = (1 - x^2)^{-1/2} \quad -1 < x < 1
$$

**Gauss-Laguerre:**

$$
W(x) = x^{\alpha} e^{-x} \quad 0 < x < \infty
$$

**Gauss-Hermite:**

$$
W(x) = e^{-x^2} \quad -\infty < x < \infty
$$

The computation of abscissas and weights can be easy or difficult depending on how much you already know about your weight function and its associated polynomials. In the case of classical, well-studied, orthogonal polynomials, practically everything is known, including good approximations for their zeros. These can be used as starting guesses, enabling Newton’s method to converge very rapidly. 

## Chebyshev

Chebyshev polynomials have analytic abscissas and weights:

$$
x_j = \cos \left( \frac{\pi \left( j - \frac{1}{2} \right)}{N} \right)
$$

$$
w_j = \frac{\pi}{N}
$$

# Gaussian quadrature with Chebyshev polynomials

We are going to write a code to perform integration using Chebyshev polynomials for [Gaussian quadrature](main.md). 
The idea is to approximate the integral of a function $f(x)$  over the interval $[-1, 1]$ using Chebyshev nodes and weights.

The Chebyshev quadrature rule gives the nodes $x_j$ and weights $w_j$ for integrating with a weight function 

$$W(x) = \frac{1}{\sqrt{1 - x^2}}\ .$$

For an $N$-point Chebyshev quadrature, the nodes $x_j$ and weights $w_j$ are given by:

$$x_j = \cos\left(\frac{\pi (j - 0.5)}{N}\right)$$

$$w_j = \frac{\pi}{N}$$


Here a _to do_ list that can help:
1.	Prompt the user to choose a function to integrate from the following list:
   
$$\mbox{[1]}\quad f(x) = e^{-\cos(x)^2}$$

$$\mbox{[2]}\quad f(x) = x^2 $$

$$\mbox{[3]}\quad f(x) = 3x^6 + 2x^3 - 3x^2 + 2 $$

$$\mbox{[4]}\quad f(x) = 3x^{10} + 2x^3 - 3x^2 + 2 $$

$$\mbox{[5]}\quad f(x) = 3x^{100} - 3x^{50} + 2x^3 - 3x^2 + 2 $$

$$\mbox{[6]}\quad f(x) = e^{-x^2} $$

2.	Allow the user to specify the maximum number of Chebyshev nodes, $N_{max}$, for the quadrature.
3.	Generate Chebyshev nodes and weights for the integration.
4.	Calculate and print the integral for different values of $N\in[2,N_{max}]$.



<details>
  <summary>C code</summary>
  
  ```C
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  
  double f(double x, int lab) {
      switch (lab) {
          case 1: return exp(-cos(x) * cos(x));
          case 2: return x * x;
          case 3: return 3 * pow(x, 6) + 2 * pow(x, 3) - 3 * x * x + 2;
          case 4: return 3 * pow(x, 10) + 2 * pow(x, 3) - 3 * x * x + 2;
          case 5: return 3 * pow(x, 100) - 3 * pow(x, 50) + 2 * pow(x, 3) - 3 * x * x + 2;
          case 6: return exp(-x * x);
          default: return 0.0;
      }
  }
  
  void generate_weights_abscissae(double *w, double *x, int N) {
      const double pi = 3.141592653589793;
      *w = pi / N; // Chebyshev weight
      for (int j = 0; j < N; j++) {
          x[j] = cos(pi * (j + 0.5) / N); // Chebyshev node
      }
  }
  
  double gauss_chebyshev(double w, double *x, int N, int lab) {
      double integral = 0.0;
      for (int j = 0; j < N; j++) {
          integral += w * f(x[j], lab)*sqrt(1.-x[j]*x[j]);
      }
      return integral;
  }
  
  int main() {
      int Nmax, lab;
  
      printf("Choose the function to be integrated from -1 to 1 (just type the number):\n");
      printf("(1) exp(-cos(x)*cos(x))\n");
      printf("(2) x*x\n");
      printf("(3) 3*x^6 + 2*x^3 - 3*x^2 + 2\n");
      printf("(4) 3*x^10 + 2*x^3 - 3*x^2 + 2\n");
      printf("(5) 3*x^100 - 3*x^50 + 2*x^3 - 3*x^2 + 2\n");
      printf("(6) exp(-x*x)\n");
      scanf("%d", &lab);
  
      if (lab < 1 || lab > 6) {
          fprintf(stderr, "ERROR: you must insert an integer number between 1 and 6.\n");
          return 1;
      }
  
      printf("Enter the maximum number of Chebyshev points (Nmax): ");
      scanf("%d", &Nmax);
  
      if (Nmax < 2) {
          fprintf(stderr, "ERROR: Nmax must be at least 2.\n");
          return 1;
      }
  
      // Open the output file
      FILE *file = fopen("results.txt", "w");
      if (file == NULL) {
          fprintf(stderr, "ERROR: Could not open file for writing.\n");
          return 1;
      }
  
      // Write header to the output file
      fprintf(file, "#N       Integral\n");
  
      for (int N = 2; N <= Nmax; N++) {
          // Allocate memory for the Chebyshev nodes
          double *x = (double *)malloc(N * sizeof(double));
          if (x == NULL) {
              fprintf(stderr, "ERROR: Memory allocation failed.\n");
              fclose(file);
              return 1;
          }
  
          double w;
          generate_weights_abscissae(&w, x, N);
  
          double integral = gauss_chebyshev(w, x, N, lab);
  
          // Write the result to the file
          fprintf(file, "%-3d     %.6f\n", N, integral);
  
          // Free allocated memory
          free(x);
      }
  
      // Close the output file
      fclose(file);
  
      printf("Results written to results.txt\n");
  
      return 0;
  }
  ```
</details>


<details>
  <summary>FORTRAN code</summary>
  
  ```FORTRAN
  program chebyshev_integration
    implicit none
    integer :: N,Nmax ! Number of Chebyshev points
    real(8) :: a, b, integral, W
    integer :: j, lab
    real(8), allocatable, dimension(:) :: x  ! Arrays to store nodes and weights
    
    ! Define the limits of integration
    a = -1.0d0
    b = 1.0d0
  
    ! Ask for input
    print *, "Choose the function to be integrated from -1 to 1 (just type the number):"
    print *, "(1) exp(-cos(x)*cos(x))"
    print *, "(2) x*x"
    print *, "(3) 3*x**6+2*x**3-3*x**2+2"
    print *, "(4) 3*x**10+2*x**3-3*x**2+2"
    print *, "(5) 3*x**100-3*x**50+2*x**3-3*x**2+2"
    print *, "(6) exp(-x*x)"
  
    !Read input
    read(*, *) lab
  
    !Check input
    if(lab.lt.0.or.lab.gt.6)then
      print*, "ERROR: you must insert an integer number between 1 and 6."
      stop
    endif
    
    print *, "Choose the function to be integrated from -1 to 1 (just type the number):"
  
    !Read input
    read(*, *) Nmax
  
    !Check input
    if(Nmax.lt.0)then
      print*, "ERROR: you must insert a positive integer number."
      stop
    endif
    
    !Header for the output
    write(666,'(A3, 7x, A3)') "#N", "Integral"
  
    do N = 2, Nmax
  
      !Allocate the array for the abscissae
      if(allocated(x))deallocate(x)
      allocate(x(N))
  
      ! Generate Chebyshev nodes and weights
      call generate_weights_abscissae(w,x)
  
      ! Perform the integration
      integral = gauss_chebyshev(w,x,lab)
  
      ! Print the result
      write(666,'(I3,5x,F10.6)') N, integral
    enddo
  contains
  
    ! Define the function to integrate here
    real(8) function f(x,lab)
      real(8), intent(in) :: x
      integer,intent(in)  :: lab
      select case(lab)
      case(1)
        f = exp(-cos(x)*cos(x))                 ! Example: Integrate exp(-x^2) over [-1, 1]
      case(2)
        f = x*x
      case(3)
        f = 3*x**6+2*x**3-3*x**2+2
      case(4)
        f = 3*x**10+2*x**3-3*x**2+2
      case(5)
        f = 3*x**100-3*x**50+2*x**3-3*x**2+2
      case(6)
        f = exp(-x*x)
      end select
    end function
  
    subroutine generate_weights_abscissae(w,x)
      implicit none
      real(8), intent(inout)               :: w
      real(8), dimension(:), intent(inout) :: x
      real(8), parameter                   :: pi = 3.141592653589793d0
      integer :: N
      N = size(x)
      w = pi / real(N) ! Chebyshev weight
      do j = 1, N
        x(j) = cos(pi * (real(j) - 0.5d0) / real(N))  ! Chebyshev node
      end do
    end subroutine
    
    real(8) function gauss_chebyshev(w,x,lab)
      real(8), intent(in)               :: w
      real(8), dimension(:), intent(in) :: x
      integer, intent(in)               :: lab
      integer                           :: j, N
      N = size(x)
      gauss_chebyshev = 0.0d0
      do j = 1, N
        gauss_chebyshev = gauss_chebyshev + W * f(x(j),lab)*sqrt(1-x(j)*x(j))
      end do
    end function
  
  
  end program chebyshev_integration
  ```

</details>
