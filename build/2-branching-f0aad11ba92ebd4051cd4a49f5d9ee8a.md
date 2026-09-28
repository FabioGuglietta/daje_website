# Branching
Branching means you diverge from the main line of development and continue to do work without messing with that main line. With *folder-based version control* this is a somewhat expensive process as it probably requires you to create a whole new copy of your source code directory.

Instead, with Git it becomes incredibly lightweight, making branching operations nearly instantaneous, for example switching back and forth between branches. Git encourages workflows that branch and merge often, even multiple times in a day. Mastering this feature might change entirely the way that you develop.

## Laplace solver

To start this lecture, let's create a repository with a very basic code for solving the Laplace equation in two dimensions. We have a square domain $\Omega=[0,1]\times[0,1]$, with Dirichlet boundary conditions on the square's borders. Additionally, we put two plates (lines in 2d) inside the square where we enforce dirichlet boundary conditions $u=\pm1$:

$$\nabla^2 u = 0$$
$$u|_{\partial\Omega}=0$$
$$u(0.5-L/2<x<0.5+L/2,0.5-D/2)=-1$$
$$u(0.5-L/2<x<0.5+L/2,0.5+D/2)=+1$$

where $L$ is the lenght of the two plates and $D$ is the distance between the two.

We can discretize the problem using second order central differences for the laplacian. Let $u_{ij}=u(x_i,y_j)$, with $x_i = i/dx$ and $y_i=j/dy$, $dx=dy$. This will end up with

$$u_{i,j}=\frac{u_{i+1,j}+u_{i-1,j}+u_{i,j+1}+u_{i,j-1}}{4}$$

which we can solve with, for example the iterative Jacobi method:

$$u^{k+1}_{i,j}=\frac{u^{k}_{i+1,j}+u^{k}_{i-1,j}+u^{k}_{i,j+1}+u^{k}_{i,j-1}}{4}$$.

The idea is that performing several iterations, the method will converge to the discretized solution of the original laplace equation. This approach is generally faster and requires a lot less memory for sparse algebraic systems than direct methods.

> [!NOTE]
>
> To have some physical intuition, you are basically solving the electrostatic potential problem in a 2D domain by numerically integrating the Laplace equation. Two parallel plates with fixed potentials simulate a capacitor, generating an electric field in the surrounding region.
>
> <figure>
>   <img src="poisson.png" alt="Laplace domain"  width="500px">
>   <figcaption>Figure 1. Domain sketch. </figcaption>
> </figure>
>
> [!SOURCES]
> <details>
> 
>   <summary>poisson.f90</summary>
> 
>   ```fortran
>   program poisson
>     use precision
>     implicit none
>     real(dp), dimension(:,:), allocatable :: U, Uold
>     real(dp), dimension(:,:), allocatable :: rho
>     real(dp) :: tolerance, a, err, maxerr, w
>     integer :: i,j,k, N, error
>     character(1) :: M, BC
>     character(20) :: arg
>     real(dp), parameter :: Pi=3.141593_dp
>     real(dp), parameter :: e2=14.4_dp ! eV*Ang
>     real(dp) :: L, dx, D
>     integer :: istart, iend, j1, j2
>     
>     ! command line arguments help
>     if (iargc()<5) then
>        write(*,*) 'poisson Method N tolerance w BC'
>        write(*,*) '  - Method: J|G (Jacobi or Gauss-Siedel)'
>        write(*,*) '  - N: <int> (number of points in x and y)'
>        write(*,*) '  - tolerance: <real> (for convergence)'
>        write(*,*) '  - w: <real> (relaxation only for G)'
>        write(*,*) '  - BC: D|N (Dirichlet or Neumann)'
>        stop
>     endif
>     
>     ! parsing of command line arguments
>     call getarg(1,arg)
>     read(arg,*) M
>   
>     call getarg(2,arg)
>     read(arg,*) N
>   
>     call getarg(3,arg)
>     read(arg,*) tolerance
>   
>     call getarg(4,arg)
>     read(arg,*) w
>   
>     call getarg(5,arg)
>     read(arg,*) BC
>   
>     allocate(U(N,N), stat=error)
>     allocate(Uold(N,N), stat=error)
>     if (error /= 0) then
>        write(*,*) 'allocation error'
>        stop
>     endif
>   
>     ! grid spacing
>     dx = 1.0_dp / N
>   
>     ! initial condition for first iteration
>     U=0.0_dp
>   
>     ! dirichlet boundary conditions (box)
>     U(1:N,1) = 0.0_dp ! bottom edge
>     U(1:N,N) = 0.0_dp ! top edge
>     U(1,1:N) = 0.0_dp ! left edge
>     U(N,1:N) = 0.0_dp ! right edge
>     ! dirichlet boundary condition on plates
>     L = 0.3_dp ! plate length
>     D = 0.5_dp ! plate separation
>     istart = int(N/2 - L/(2*dx))     ! Plate x-start
>     iend   = int(N/2 + L/(2*dx))     ! Plate x-end
>     j1     = int(N/2 - D/(2*dx))     ! Bottom plate y-index
>     j2     = int(N/2 + D/(2*dx))     ! Top plate y-index
>     U(istart:iend, j1) = -1.0_dp
>     U(istart:iend, j2) = +1.0_dp
>   
>     select case (M)
>     case("J")
>       write(*,*) "Jacobi iteration"
>     case("G")
>       write(*,*) "Gauss-Siedel iteration"
>     end select
>     
>     ! -----------------------------
>     ! Iterative solver main loop
>     ! -----------------------------
>     err = 2.0_dp * tolerance
>     k = 1
>     do while (err > tolerance)
>       Uold = U          ! Store previous solution
>       maxerr = 0.0_dp   ! Reset max error
>    
>       ! ---------------
>       ! loop in space
>       ! ---------------
>       do j = 2, N-1
>         do i = 2, N-1
>   
>           ! Skip capacitor plates
>           if (i >= istart .and. i<=iend .and. (j==j1 .or. j==j2)) cycle  
>           
>           ! solution domain: perform iteration update
>           select case (M)
>           case("J")
>             U(i,j) = (Uold(i-1,j) + Uold(i+1,j) + Uold(i,j-1) + Uold(i,j+1))/4.0_dp
>           case("G")
>             write(*,*)'Gauss-Siedel not implemented. Stopping'
>             call exit(-1)
>           end select
>           
>           ! check covergence
>           if (abs(Uold(i,j)-U(i,j)) > maxerr ) then
>              maxerr = abs(Uold(i,j) - U(i,j))
>           end if
>         
>           ! relaxation factor w
>           U(i,j) = (1-w)*Uold(i,j) + w*U(i,j)
>   
>           ! optional Neumann o(a^2) along x==1 and x==n
>           if (BC.eq."N") then
>             write(*,*)'Neumann B.C. not implemented. Stopping'
>             call exit(-1)
>           endif
>      
>         end do
>   
>         ! optional Neumann o(a^2) along y==1 and y==n
>         if (BC.eq."N") then
>             write(*,*)'Neumann B.C. not implemented. Stopping'
>             call exit(-1)
>         endif
>       end do
>    
>       ! Output iteration progress
>       write(*,*) 'iter: ',k, maxerr
>       err = maxerr
>       k = k + 1
>     end do   
>    
>   
>   
>   
>   
>   
>     ! write to file in fortran order
>     open(101, file='sol.dat')
>     do j = 1, N
>        do i = 1, N
>           write(101, *) U(i,j)
>        end do
>        write(101,*)
>     end do
>     close(101)
>    
>   end program poisson
>   ```
> </details>
> 
> <details>
> 
>   <summary>precision.f90</summary>
> 
>   ```fortran
>   module precision
>     integer, parameter, public :: dp = 8
>   end module precision
>   
>   ```
> </details>
>
> <details>
> 
>   <summary>plot.py</summary>
> 
>   ```python
>   import numpy as np
>   import matplotlib.pyplot as plt
>   import sys
>   
>   N = int(sys.argv[1])
>   nx, ny = N, N
>   D = 0.5
>   L = 0.3
>   boxC = 0.5
>   # load flattened data with fortran order (y1x1 y1x2 y1x3 ... y2x1 y2x2 y2x3 ...)
>   # (x is contiguous in memory)
>   data = np.loadtxt('sol.dat')
>   # reshape and transpose to convert to "standard" c order
>   # (y is contiguous in memory)
>   data = data.reshape((ny,nx)).T
>   x = np.linspace(0, 1, nx)
>   y = np.linspace(0, 1, ny)
>   X, Y = np.meshgrid(x, y, indexing='ij')
>   Ey, Ex = np.gradient(-data, y, x)
>   
>   # create figure and axis
>   fig, ax = plt.subplots()
>   ax.set_xlabel('y')
>   ax.set_ylabel('x')
>   
>   # plot 2d solution with contour lines
>   im = ax.imshow(data, extent=[x[0],x[-1],y[0],y[-1]], origin='lower', interpolation='nearest')
>   fig.colorbar(im, ax=ax)
>   
>   # plot streamlines of the gradient field (electric field)
>   Em = np.sqrt(Ey**2. + Ex**2.)
>   lw = 8. * Em / Em.max() # linewidth depending on magnitude
>   ax.streamplot(x, y, Ex, Ey, color='white', linewidth=lw, arrowsize=0.7, density=1.2)
>   
>   cntr = ax.contour(data, [-0.7, -0.25, -0.05, 0.05, 0.25, 0.7], colors='red', extent=[0,1,0,1])
>   ax.clabel(cntr, cntr.levels, fontsize=10, colors='red') # plot contour values
>   
>   # plot bars inside domain
>   ax.vlines(x=boxC-D/2., ymin=boxC-L/2., ymax=boxC+L/2., linewidth=2, color='b')
>   ax.vlines(x=boxC+D/2., ymin=boxC-L/2., ymax=boxC+L/2., linewidth=2, color='b')
>   
>   plt.show()
>   ```
>
> </details>
>
> <details>
> 
>   <summary>Makefile</summary>
>
>   ```makefile
>   # Simple Makefile for a Fortran program
>   # Program: poisson.f90
>   # Module: precision.f90 (used by poisson.f90)
>   
>   # Compiler and flags
>   # Fortran compiler
>   FC = gfortran
>   # Optimization level 3
>   FFLAGS = -O3
>   
>   # Define source files and the final executable name
>   MODULE = precision.f90
>   MAIN = poisson.f90
>   EXEC = poisson
>   
>   # Define object files: these are compiled versions of the source files
>   OBJS = precision.o poisson.o
>   
>   # Default target: builds the executable
>   # This rule says: to build `poisson`, first make sure all object files are compiled
>   $(EXEC): $(OBJS)
>   	$(FC) $(FFLAGS) -o $(EXEC) $(OBJS)
>   
>   # Rule to compile the module
>   # (modules must be compiled before the files that use them)
>   precision.o: precision.f90
>   	$(FC) $(FFLAGS) -c precision.f90
>   
>   # Rule to compile the main program
>   # Depends on both poisson.f90 and precision.o
>   poisson.o: poisson.f90 precision.o
>   	$(FC) $(FFLAGS) -c poisson.f90
>   
>   # Utility target: clean up compilation artifacts
>   # Run `make clean` to remove object files, module files, and the executable
>   clean:
>   	rm -f *.o *.mod $(EXEC)
>   ```
>
> ![WARNING]
>
> HTML cannot render hard-tabs, which are required in makefile language. To fix this, you have to replace soft tabs in front of `$(FC) ...` with hard tabs. You can use:
> ```bash
> # linux
> sed -i 's/^\(.*\$(FC)\)/\t\$(FC)/' Makefile
> #mac os x
> sed -i .bak 's/^\(.*\$(FC)\)/\t\$(FC)/' Makefile
> ```
>
> </details>

### Laplace repository
Enough with equations. Create the directory, create the files (copying the content from the sources given in the previous note) and use `git init`, `git add` and `git commit` commands to setup the repository. You can run the program by doing:
```bash
> make
> # run the solver with 100x100 grid, 1e-5 tolerance
> ./poisson J 100 1e-5 1.0 D
> # plot results (100x100 grid)
> python3 plot.py 100
```
##

Now, how do we proceed if we want to add some features to the code? Let's implement Gauss-Siedel method instead of Jacobi. To do that, you should change the update rule with the following:

```fortran
! U(i,j) = (Uold(i-1,j) + Uold(i+1,j) + Uold(i,j-1) + Uold(i,j+1))/4.0
U(i,j) = (U(i-1,j) + Uold(i+1,j) + U(i,j-1) + Uold(i,j+1))/4.0_dp
```

If you want, you can delete the whole `poisson.f90` file and create it from scratch (git will have a backup in any case) with the following updated source:

> [!SOURCES]
>
> <details>
> 
>   <summary>poisson.f90</summary>
> 
> ```fortran
> program poisson
>   use precision
>   implicit none
>   real(dp), dimension(:,:), allocatable :: U, Uold
>   real(dp), dimension(:,:), allocatable :: rho
>   real(dp) :: tolerance, a, err, maxerr, w
>   integer :: i,j,k, N, error
>   character(1) :: M, BC
>   character(20) :: arg
>   real(dp), parameter :: Pi=3.141593_dp
>   real(dp), parameter :: e2=14.4_dp ! eV*Ang
>   real(dp) :: L, dx, D
>   integer :: istart, iend, j1, j2
>   
>   ! command line arguments help
>   if (iargc()<5) then
>      write(*,*) 'poisson Method N tolerance w BC'
>      write(*,*) '  - Method: J|G (Jacobi or Gauss-Siedel)'
>      write(*,*) '  - N: <int> (number of points in x and y)'
>      write(*,*) '  - tolerance: <real> (for convergence)'
>      write(*,*) '  - w: <real> (relaxation only for G)'
>      write(*,*) '  - BC: D|N (Dirichlet or Neumann)'
>      stop
>   endif
>   
>   ! parsing of command line arguments
>   call getarg(1,arg)
>   read(arg,*) M
> 
>   call getarg(2,arg)
>   read(arg,*) N
> 
>   call getarg(3,arg)
>   read(arg,*) tolerance
> 
>   call getarg(4,arg)
>   read(arg,*) w
> 
>   call getarg(5,arg)
>   read(arg,*) BC
> 
>   allocate(U(N,N), stat=error)
>   allocate(Uold(N,N), stat=error)
>   if (error /= 0) then
>      write(*,*) 'allocation error'
>      stop
>   endif
> 
>   ! grid spacing
>   dx = 1.0_dp / N
> 
>   ! initial condition for first iteration
>   U=0.0_dp
> 
>   ! dirichlet boundary conditions (box)
>   U(1:N,1) = 0.0_dp ! bottom edge
>   U(1:N,N) = 0.0_dp ! top edge
>   U(1,1:N) = 0.0_dp ! left edge
>   U(N,1:N) = 0.0_dp ! right edge
>   ! dirichlet boundary condition on plates
>   L = 0.3_dp ! plate length
>   D = 0.5_dp ! plate separation
>   istart = int(N/2 - L/(2*dx))     ! Plate x-start
>   iend   = int(N/2 + L/(2*dx))     ! Plate x-end
>   j1     = int(N/2 - D/(2*dx))     ! Bottom plate y-index
>   j2     = int(N/2 + D/(2*dx))     ! Top plate y-index
>   U(istart:iend, j1) = -1.0_dp
>   U(istart:iend, j2) = +1.0_dp
> 
>   select case (M)
>   case("J")
>     write(*,*) "Jacobi iteration"
>   case("G")
>     write(*,*) "Gauss-Siedel iteration"
>   end select
>   
>   ! -----------------------------
>   ! Iterative solver main loop
>   ! -----------------------------
>   err = 2.0_dp * tolerance
>   k = 1
>   do while (err > tolerance)
>     Uold = U          ! Store previous solution
>     maxerr = 0.0_dp   ! Reset max error
>  
>     ! ---------------
>     ! loop in space
>     ! ---------------
>     do j = 2, N-1
>       do i = 2, N-1
> 
>         ! Skip capacitor plates
>         if (i >= istart .and. i<=iend .and. (j==j1 .or. j==j2)) cycle  
>         
>         ! solution domain: perform iteration update
>         select case (M)
>         case("J")
>           U(i,j) = (Uold(i-1,j) + Uold(i+1,j) + Uold(i,j-1) + Uold(i,j+1))/4.0_dp
>         case("G")
>           U(i,j) = (U(i-1,j) + Uold(i+1,j) + U(i,j-1) + Uold(i,j+1))/4.0_dp
>         end select
>         
>         ! check covergence
>         if (abs(Uold(i,j)-U(i,j)) > maxerr ) then
>            maxerr = abs(Uold(i,j) - U(i,j))
>         end if
>       
>         ! relaxation factor w
>         U(i,j) = (1-w)*Uold(i,j) + w*U(i,j)
> 
>         ! optional Neumann o(a^2) along x==1 and x==n
>         if (BC.eq."N") then
>           write(*,*)'Neumann B.C. not implemented. Stopping'
>           call exit(-1)
>         endif
>    
>       end do
> 
>       ! optional Neumann o(a^2) along y==1 and y==n
>       if (BC.eq."N") then
>           write(*,*)'Neumann B.C. not implemented. Stopping'
>           call exit(-1)
>       endif
>     end do
>  
>     ! Output iteration progress
>     write(*,*) 'iter: ',k, maxerr
>     err = maxerr
>     k = k + 1
>   end do   
>  
> 
> 
> 
> 
> 
>   ! write to file in fortran order
>   open(101, file='sol.dat')
>   do j = 1, N
>      do i = 1, N
>         write(101, *) U(i,j)
>      end do
>      write(101,*)
>   end do
>   close(101)
>  
> end program poisson
> ```
> </details>

The nice thing is that we can let Git check the differences to be sure that nothing else changed:

```bash
> git diff
diff --git a/poisson.f90 b/poisson.f90
index f93d61f..6552a6c 100644
--- a/poisson.f90
+++ b/poisson.f90
@@ -97,8 +97,7 @@ program poisson
         case("J")
           U(i,j) = (Uold(i-1,j) + Uold(i+1,j) + Uold(i,j-1) + Uold(i,j+1))/4.0_dp
         case("G")
-          write(*,*)'Gauss-Siedel not implemented. Stopping'
-          call exit(-1)
+          U(i,j) = (U(i-1,j) + Uold(i+1,j) + U(i,j-1) + Uold(i,j+1))/4.0_dp
         end select

         ! check covergence
```

By committing this "new file", Git will automatically update the previous snapshot by adding only the new feature. This is very important, as it allows us to understand clearly what changed from the previous directory snapshot (commit).

You can run the code again with the Gauss-Siedel method using the `G` option instead of `J`. As it often happens, it converges faster:

```bash
> make
> ./poisson J 100 1e-5 1.0 D     # converges in 2005 iterations
> ./poisson G 100 1e-5 1.0 D     # converges in 1131 iterations
```

> ![EXERCISE]
>
> Did you notice there is a small problem in the code? The convergence check is before the relaxation step. Fix it and do a specifi commit.

### Resetting to a previous commit

Now, what if you messed up something, a problem came up, and you want to revert to the last "working" commit? First, let's look at the history:

```bash
> git log
commit c528bea83fdaaf6115dbd41c72a90ff191f744a0 (main)
Author: scarpma <scarpma@gmail.com>
Date:   Sun Jun 8 21:55:06 2025 +0200

    inverted convergence check and relaxation step

commit 75f7a90f0628375cdb3bc1ddcefa23e747f8fe76
Author: scarpma <scarpma@gmail.com>
Date:   Sun Jun 8 18:01:35 2025 +0200

    implemented Gauss-Siedel method

commit f5d62843f8bff218434a93c72319b27d05000128
Author: Alessandro Pecchia <alessandro.pecchia@ismn.cnr.it>
Date:   Tue Dec 1 11:29:52 2020 +0100

    First commit
```

If we want to go back to a previous commit we can use the `git reset`. In Git language, "resetting" means to change the snapshot the current branch and `HEAD` point to. You can do this by also "changing" the working tree (**hard resetting**) or by leaving it as it is now (**soft resetting**). Let's go back to the last commit (`75f7a`):

```bash
> git reset 75f7a
> git log
commit 75f7a90f0628375cdb3bc1ddcefa23e747f8fe76 (HEAD -> main)
Author: scarpma <scarpma@gmail.com>
Date:   Sun Jun 8 18:01:35 2025 +0200

    implemented Gauss-Siedel method

commit f5d62843f8bff218434a93c72319b27d05000128
Author: Alessandro Pecchia <alessandro.pecchia@ismn.cnr.it>
Date:   Tue Dec 1 11:29:52 2020 +0100

    First commit
>
>On branch main
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   poisson.f90

no changes added to commit (use "git add" and/or "git commit -a") git status
```

As you can see from `git log`, our "last commit" now is `75f7a`, which is the one we just resetted onto. One commit disappeared. However, its changes are still on the working tree (as can be seen with `git status`).

If we want to "hard reset" we can do:

```bash
> git reset --hard 75f7a
> git status
On branch main
Your branch is behind 'origin/main' by 1 commit, and can be fast-forwarded.
  (use "git pull" to update your local branch)

nothing to commit, working tree clean
```

Now it's really as if the deleted commit never happened. At least from the history we see. What if we want to recover that commit? We went back in the past by selecting a specific commit using `git log`, but it cannot see the future. So is the deleted commit lost? In Git almost nothing is ever lost, especially if it was committed somewhere. We can, for example, scroll back in the terminal and see the hash of the deleted commit (`c528b`). Once we have it, we can do a hard reset again:

```bash
> git reset --hard c528b
> git log
commit c528bea83fdaaf6115dbd41c72a90ff191f744a0 (HEAD -> main)
Author: scarpma <scarpma@gmail.com>
Date:   Sun Jun 8 21:55:06 2025 +0200

    inverted convergence check and relaxation step

commit 75f7a90f0628375cdb3bc1ddcefa23e747f8fe76
Author: scarpma <scarpma@gmail.com>
Date:   Sun Jun 8 18:01:35 2025 +0200

    implemented Gauss-Siedel method

commit f5d62843f8bff218434a93c72319b27d05000128
Author: Alessandro Pecchia <alessandro.pecchia@ismn.cnr.it>
Date:   Tue Dec 1 11:29:52 2020 +0100

    First commit
```

and here we are, as if nothing ever happened. This is not the only way it could be recovered (check `git reflog` for example), however there's a better way to do this kind of *commit tagging*: **branching**.

### Branching

Creating a branch is equivalent to putting a reference to a certain commit. As we saw in the first lecture, the output of `git log` shows, for each commit listed, if some branch points to it. In the `git log` output just above `(HEAD -> main)` is along the commit `c528b`, meaning that the **main branch** points to it. Additionally, it means the the current working directory (**HEAD**) points to main as well.

```
                               ┌───────┐  
                               │ HEAD  │  
                               └───┬───┘  
                                   │      
                               ┌───▼───┐  
                               │ main  │  
                               └───┬───┘  
                                   │      
┌─────────┐    ┌─────────┐    ┌────▼────┐ 
│         │    │         │    │         │ 
│  f5d62  ◄────┼  75f7a  ◄────┼  c528b  │ 
│         │    │         │    │         │ 
└─────────┘    └─────────┘    └─────────┘ 
```                                          
In other words, doing

```bash
> git reset --hard main
```
or 
```bash
> git reset --hard c528b
```
is exactly equivalent, because they point to the same repository snapshot.

To create a new branch:
```bash
> git branch debug
```
This creates a new "pointer" or "reference" called *debug* to the same commit you are currently on.
```
                               ┌───────┐ 
                               │ HEAD  │ 
                               └───┬───┘ 
                                   │     
                               ┌───▼───┐ 
                               │ main  │ 
                               └───┬───┘ 
                                   │     
┌─────────┐    ┌─────────┐    ┌────▼────┐
│         │    │         │    │         │
│  f5d62  ◄────┼  75f7a  ◄────┼  c528b  │
│         │    │         │    │         │
└─────────┘    └─────────┘    └────▲────┘
                                   │     
                               ┌───┼───┐ 
                               │ debug │ 
                               └───────┘ 

```
You can check this with `git log`. However, we have not **checked out** it yet (i.e. "switched to it" in Git terminology). Indeed, *HEAD* still directly points to *main*. To switch to an existing branch, you run the `git checkout` command, so
```bash
> git checkout debug
```
This moves `HEAD` to point to the `debug` branch.
```
                               ┌───────┐  
                               │ main  │  
                               └───┬───┘  
                                   │      
┌─────────┐    ┌─────────┐    ┌────▼────┐ 
│         │    │         │    │         │ 
│  f5d62  ◄────┼  75f7a  ◄────┼  c528b  │ 
│         │    │         │    │         │ 
└─────────┘    └─────────┘    └────▲────┘ 
                                   │      
                               ┌───┼───┐  
                               │ debug │  
                               └───▲───┘  
                                   │      
                               ┌───┼───┐  
                               │ HEAD  │  
                               └───────┘  
```
Now, doing the same operation
```bash
> git reset --hard 75f7a
```
will change the commit `debug` points to (and also `HEAD`)
```
                                ┌───────┐  
                                │ main  │  
                                └───┬───┘  
                                    │      
 ┌─────────┐    ┌─────────┐    ┌────▼────┐ 
 │         │    │         │    │         │ 
 │  f5d62  ◄────┼  75f7a  ◄────┼  c528b  │ 
 │         │    │         │    │         │ 
 └─────────┘    └────▲────┘    └─────────┘ 
                     │                     
                 ┌───┼───┐                 
                 │ debug │                 
                 └───▲───┘                 
                     │                     
                 ┌───┼───┐                 
                 │ HEAD  │                 
                 └───────┘
```
From Git, this is shown by the `git log` command. However, doing `git log` now will show only the history of the `debug` branch, so `c528b` will be hidden. Instead, we can provide the branch `main` as argument
```bash
> git log main
commit c528bea83fdaaf6115dbd41c72a90ff191f744a0 (origin/main, main)
Author: scarpma <scarpma@gmail.com>
Date:   Sun Jun 8 21:55:06 2025 +0200

    inverted convergence check and relaxation step

commit 75f7a90f0628375cdb3bc1ddcefa23e747f8fe76 (HEAD -> debug)
Author: scarpma <scarpma@gmail.com>
Date:   Sun Jun 8 18:01:35 2025 +0200

    implemented Gauss-Siedel method

commit f5d62843f8bff218434a93c72319b27d05000128
Author: Alessandro Pecchia <alessandro.pecchia@ismn.cnr.it>
Date:   Tue Dec 1 11:29:52 2020 +0100

    First commit
```
and we can see that the reference decorators describe the situation perfectly. Now, if you would like to go back to the "most update" version of the repository, you could simply do `git checkout main`, without having to remember the hash of the precise commit.

### Neumann boundary conditions (on the `debug` branch)

Imagine that now we are asked to implement Neumann boundary conditions, but we are not sure if the last commit added to `main` is correct or not. It might be better to continue working on the `debug` branch and figure out later how to fix the problem
```bash
> git checkout debug
```
So let's implement Neumann boundary conditions $$\partial u / \parital n = 0$$ on the `debug` branch.

You can insert
```fortran
! optional Neumann o(a^2) along x [y==1 and y==N] dUdy=0
if (BC.eq."N") then
  U(i,1) = 4.0_dp/3.0_dp * U(i,2) - 1.0_dp/3.0_dp * U(i,3)
  U(i,N) = 4.0_dp/3.0_dp * U(i,N-1) - 1.0_dp/3.0_dp * U(i,N-2)
endif
```
in the loop along `j` ($y$) and
```fortran
! optional Neumann o(a^2) along y [x==1 and x==N] dUdx=0
if (BC.eq."N") then
  U(1,j) = 4.0_dp/3.0_dp * U(2,j) - 1.0_dp/3.0_dp * U(3,j)
  U(N,j) = 4.0_dp/3.0_dp * U(N-1,j) - 1.0_dp/3.0_dp * U(N-2,j)
  ! box corners (dudx+dudy=0)
  U(1,1) = 0.5_dp * (&
    4.0_dp/3.0_dp * U(2,1) - 1.0_dp/3.0_dp * U(3,1) + &
    4.0_dp/3.0_dp * U(1,2) - 1.0_dp/3.0_dp * U(1,3))
  U(1,N) = 0.5_dp * (&
    4.0_dp/3.0_dp * U(2,N) - 1.0_dp/3.0_dp * U(3,N) + &
    4.0_dp/3.0_dp * U(1,N-1) - 1.0_dp/3.0_dp * U(1,N-2))
  U(N,N) = 0.5_dp * (&
    4.0_dp/3.0_dp * U(N-1,N) - 1.0_dp/3.0_dp * U(N-2,N) + &
    4.0_dp/3.0_dp * U(N,N-1) - 1.0_dp/3.0_dp * U(N,N-2))
  U(N,1) = 0.5_dp * (&
    4.0_dp/3.0_dp * U(N-1,1) - 1.0_dp/3.0_dp * U(N-2,1) + &
    4.0_dp/3.0_dp * U(N,2) - 1.0_dp/3.0_dp * U(N,3))
endif
```
in the loop along `i` ($x$). Now, we test the implementation
```bash
> make
> ./poisson G 100 1e-5 1.0 N; python3 plot.py 100
```
and we can see that the streamlines are tangent to the domain boundaries, meaning that $\partial u /\partial n = 0$. Let's commit
```bash
> git add poisson.f90
> git commit -m "implemented Neumann boundary conditions"
```

### Rebasing

If you committed this on the `debug` branch, the Git tree is now
```
                                ┌───────┐  
                                │ main  │  
                                └───┬───┘  
                                    │      
 ┌─────────┐    ┌─────────┐    ┌────▼────┐ 
 │         │    │         │    │         │ 
 │  f5d62  ◄────┼  75f7a  ◄────┼  c528b  │ 
 │         │    │         │    │         │ 
 └─────────┘    └────▲────┘    └─────────┘ 
                     │                     
                ┌────┼────┐                
                │         │                
                │  7d210  │                
                │         │                
                └────▲────┘                
                     │                     
                 ┌───┼───┐                 
                 │ debug │                 
                 └───▲───┘                 
                     │                     
                 ┌───┼───┐                 
                 │ HEAD  │                 
                 └───────┘

```
Visually, it's clear why they are called branch: we branched from the **main history** of our project and created a different path. This is called a **divergent** history (the debug branch has diverged from the "main" history)

Now, if, after some testing, we understand that commit `c528b` did not introduce any problem, it may have been better to implement the new feature directly in the `main` branch. However, this is not a problem, as Git allows us to move and rebase commits at will. There are two ways to solve this situation now. Remember that commits can be seen as local edits to the repository. We might want to copy commit `c528b` on top of the `debug` branch (`git cherry-pick`), or we might move, i.e. **rebase**, the whole `debug` branch on top of the update `main` (`git rebase`).

Let's do a rebase. We want to achieve something like this
```
                                ┌───────┐   
                                │ main  │   
                                └───┬───┘   
                                    │       
 ┌─────────┐    ┌─────────┐    ┌────▼────┐  
 │         │    │         │    │         │  
 │  f5d62  ◄────┼  75f7a  ◄────┼  c528b  │  
 │         │    │         │    │         │  
 └─────────┘    └─────────┘    └────▲────┘  
                                    │       
                               ┌────┼────┐  
                               │         │  
                               │  d0710  │  
                               │         │  
                               └────▲────┘  
                                    │       
                                ┌───┼───┐   
                                │ debug │   
                                └───▲───┘   
                                    │       
                                ┌───┼───┐   
                                │ HEAD  │   
                                └───────┘   
```
Fortunately, this is very easy: `git rebase` command takes the commit you want to "**base**" your current branch onto, so
```bash
> git rebase main
```
will do the work. Since `c528b` is very similar to the previous "base" `75f7a`, everything should go smoothly and complete automatically. Note that the commit hash has changed to `d0710` after rebasing. This is normal because, as we already covered, the hash reflects the content of that snapshot. If the base of a commit changes, its hash changes as well.

We can check with `git log` that now our debug branch is based on `main`. Tipically, this is the best situation because, if everything is ok and we are convinced to keep this "version" of the repository, we can directly merge into main without any problems
```bash
> git checkout main
> git merge debug
```
and now the situation is clean again, with all updated and working features on the main branch:
```
                                                         
                                              ┌───────┐  
                                              │ HEAD  │  
                                              └───┬───┘  
                                                  │      
                                              ┌───▼───┐  
                                              │ main  │  
                                              └───┬───┘  
                                                  │      
┌─────────┐    ┌─────────┐    ┌─────────┐    ┌────▼────┐ 
│         │    │         │    │         │    │         │ 
│  f5d62  ◄────┼  75f7a  ◄────┼  c528b  ◄────┼  d0710  │ 
│         │    │         │    │         │    │         │ 
└─────────┘    └─────────┘    └─────────┘    └────▲────┘ 
                                                  │      
                                              ┌───┼───┐  
                                              │ debug │  
                                              └───────┘  
```
We can now delete the debug branch with `git branch -d debug`.

In summary we
- created a debug branch;
- reverted the repository to a previous state (`git reset --hard`);
- we added a feature (Neumann B.C.) on the old snapshot;
- we rebased the debug branch on the updated main branch (`git rebase`);
- we merged back to main (`git merge debug`);
- deleted the `debug` branch.

