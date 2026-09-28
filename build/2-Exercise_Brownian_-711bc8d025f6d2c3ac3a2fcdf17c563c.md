# Brownian Motion and the Diffusion Coefficient

Brownian motion is one of the most fundamental **stochastic processes** in physics.
It refers to the **erratic motion** of microscopic particles **suspended in a fluid**, first observed by the botanist **Robert Brown (1827)** and later explained by **Albert Einstein (1905)** as a consequence of molecular agitation. 
Each suspended particle is constantly bombarded by countless fluid molecules, and these collisions produce small random kicks that cause the particle to wander unpredictably through space.

Although the **instantaneous motion** is **irregular and random**, its **statistical** behavior is remarkably **simple**. Averaging over many particles (or over time, for a single long trajectory), the **particle’s mean square displacement (MSD)** increases **linearly** with time: 
$$\langle r^2(t) \rangle = 2dDt\ ,$$
where:
- $$\langle r^2(t) \rangle = \langle |\mathbf{r}(t) - \mathbf{r}(0)|^2 \rangle$$ is the mean square displacement (MSD);
- $d$ is the spatial dimension (here $d=2$);
- $D$ is the **diffusion coefficient**, a macroscopic quantity that characterizes how rapidly particles spread.

The linear relation between $\langle r^2\rangle$ and $t$ defines a diffusive regime.

## The Discrete Random Walk Model

To **simulate** Brownian motion on a computer, we can replace the continuous stochastic process by a **discrete random walk.**
In two dimensions, we consider a particle that moves on a square lattice with spacing $a$.
At each time step of duration $\Delta t$, the particle jumps by one step of length $a$ in one of the four cardinal directions $(\pm x, \pm y)$ each chosen with equal probability $1/4$.

After $n$ steps (corresponding to time $t = n\Delta t$), the average squared distance from the origin is:

$$\langle r^2(t) \rangle = na^2 = 4Dt\ .$$

From this, we obtain the theoretical diffusion coefficient for a 2D random walk:

$$D_{\text{theory}} = \frac{a^2}{4\Delta t}\ .$$

This model captures the essence of diffusion: even though **each step is random**, the **ensemble average** obeys a **deterministic** law. 
Increasing the number of trajectories reduces **statistical noise** and makes the numerical estimate of **D converge toward the theoretical value.**

## Numerical Implementation

In the numerical implementation, we simulate many independent trajectories to obtain a good statistical average. Each trajectory starts from the origin $(x,y)=(0,0)$.
At every iteration:
1.	We draw a uniform random number $r \in [0,1)$;
2.	We assign a direction according to r:
   - $r \in [0,0.25) \Longrightarrow$ move right ($+x$),
   - $r \in [0.25,0.5)\Longrightarrow$ move left ($−x$),
   - $r \in [0.5,0.75)\Longrightarrow$ move up ($+y$),
   - $r \in [0.75,1.0)\Longrightarrow$ move down ($−y$);
3.	We update the position accordingly by a distance $a$;
4.	We compute the squared distance from the origin $r^2 = x^2 + y^2$.

After all trajectories are computed, we average $\langle r^2(t_i)\rangle$ across all of them to obtain the ensemble mean at each time step $t_i$.

Finally, we perform a linear regression of $\langle r^2\rangle$ vs. $t$, forcing the fit to pass through the origin (since by definition $\langle r^2(0)\rangle = 0$).
In two dimensions, the slope of this fit equals $4D$, so that:

$$D = \frac{1}{4} \frac{d}{dt} \langle r^2\rangle\ .$$

This approach not only reproduces the diffusion law numerically but also demonstrates how a macroscopic transport coefficient can be inferred from microscopic stochastic dynamics.

## Metacode implementation

```Plain
Goal: simulate 2D Brownian motion using a discrete random walk
      and estimate the diffusion coefficient D from ⟨r²(t)⟩ = 4 D t

---------------------------------------------------------------
1. Define simulation parameters
   - nsteps  → number of time steps per trajectory
   - ntraj   → number of independent trajectories
   - a       → step length
   - dt      → time step
   - D_theory = a² / (4 dt)

2. Allocate arrays
   - t[i]        : time vector, from 0 to (nsteps−1)*dt
   - MSD_avg[i]  : ensemble-averaged mean square displacement

3. For each trajectory j = 1 … ntraj
      Initialize position: x = 0, y = 0
      For each time step i = 2 … nsteps
          Draw a random number r ∈ [0, 1)
          Determine direction:
              if 0 ≤ r < 0.25  → move right (+x)
              if 0.25 ≤ r < 0.5 → move left (−x)
              if 0.5 ≤ r < 0.75 → move up (+y)
              if 0.75 ≤ r < 1.0 → move down (−y)
          Update position by ±a
          Compute squared displacement: r² = x² + y²
          Store r² at index i
      Add this trajectory’s r² values to the ensemble sum

4. After all trajectories are done:
      MSD_avg[i] = MSD_sum[i] / ntraj      → ensemble average

5. Fit ⟨r²(t)⟩ vs. t with a straight line through the origin:
      slope = Σ t_i * ⟨r²_i⟩ / Σ t_i²
      D_est = slope / 4                    → (valid in 2D)

6. Print results:
      Estimated D vs. theoretical D
---------------------------------------------------------------
```

> # Generating Random Numbers in Fortran and C
> ## In Fortran
> Fortran provides a built-in intrinsic subroutine called `random_number()` that generates pseudo-random numbers uniformly distributed in the interval [0, 1).
> These numbers are deterministic sequences produced by an internal algorithm, but they appear random for most simulation purposes (e.g., Monte Carlo or random walk problems).
> ```Fortran
> real :: r
> call random_number(r)
> ```
> After this call, the variable r contains a random real number such that $0 \le r < 1$.
> You can also generate arrays of random numbers:
> ```Fortran
> real, allocatable :: a(:)
> allocate(a(100))
> call random_number(a)
> ``` 
> Here, `a(i)` will hold 100 independent random values in [0, 1).
> ### Setting the random seed
> By default, Fortran uses an internal seed, which can vary between compilers.
> To ensure reproducibility (the same random sequence every time), you can explicitly set the seed using the subroutine `random_seed`:
> ```Fortran
> integer :: n, seed(1)
> call random_seed(size=n)         ! Query how many seed integers are needed
> allocate(seed(n))
> seed = 123456                    ! Choose your own seed value(s)
> call random_seed(put=seed)       ! Set the seed
> ```
> If you omit this step, the program will still work — but every run may produce a different sequence (useful when you want independent simulations).
> ### Generating random numbers in a specific range
> To obtain a random value in a custom range [a, b):
> ```Fortran
> real :: r_uniform
> call random_number(r_uniform)
> r_uniform = a + (b - a) * r_uniform
> ```
> ## In C
> In C, random numbers are produced using the C Standard Library functions `rand()` and `srand()`, defined in `<stdlib.h>`.
> The function `rand()` returns an integer between 0 and `RAND_MAX`.
> To convert it into a floating-point number in [0, 1):
> ```C
> #include <stdio.h>
> #include <stdlib.h>
>
> int main() {
>     double r;
>     r = (double) rand() / RAND_MAX;   // random number in [0, 1)
>     printf("%f\n", r);
>     return 0;
> }
> ```
> ### Setting the random seed
> Use `srand()` to initialize the pseudo-random number generator.
> If you use a fixed seed, the sequence will be reproducible;
> if you use a time-dependent seed, each run will produce a different sequence:
> ```C
> #include <stdlib.h>
> #include <time.h>
> 
> srand(12345);           // fixed seed (reproducible)
> // or
> srand(time(NULL));      // variable seed (different each run)
> ```
> ### Generating random numbers in a custom range
> To get a random number in [a, b):
> ```C
> double r_uniform = a + (b - a) * ((double) rand() / RAND_MAX);
> ```

> [!NOTE]
> # Linear Regression in the MSD Analysis
> In the final part of the program, the diffusion coefficient is estimated using a simple linear regression between the mean square displacement (MSD) and time.
> From theory, the MSD in two dimensions follows a linear relation:
>
> $$\langle r^2(t) \rangle = 4 D t\  ,$$
> 
> where D is the diffusion coefficient.
> The numerical data consist of discrete pairs $(t_i, \langle r^2_i \rangle)$, computed from the ensemble of random walks.
> To extract D, we perform a least-squares fit of a straight line through the origin:
>
> $$\text{slope} = \frac{\sum_i t_i\,\langle r^2_i \rangle}{\sum_i t_i^2}\ ,\qquad D = \frac{\text{slope}}{4}\ .$$
>
> This formula minimizes the squared difference between the simulated data and the best-fitting straight line constrained to pass through zero (since $\langle r^2(0)\rangle = 0$).
> The computed slope represents the rate of increase of the MSD with time, and dividing by 4 gives the effective diffusion coefficient in two dimensions.
> This simple regression is a direct numerical confirmation of Einstein’s diffusion law, connecting microscopic random motion to macroscopic transport behavior.

### Fortran Program: Estimating D from the MSD
<details>
  <summary>C code</summary>
   
   ```C
   #include <stdio.h>
   #include <stdlib.h>
   #include <math.h>
   #include <time.h>
   
   /* ==========================================================
      Simulation of 2D Brownian motion
      Estimate diffusion coefficient D from the mean square displacement (MSD)
      ========================================================== */
   
   #define NSTEPS 2000     /* time steps per trajectory */
   #define NTRAJ  2000     /* number of independent trajectories */
   #define A      1.0      /* step length */
   #define DT     1.0      /* time step */
   
   /* ---------- Function declarations ---------- */
   void simulate_random_walks(double *msd_avg, int nsteps, int ntraj, double a);
   double estimate_diffusion(double *t, double *msd_avg, int nsteps);
   
   int main(void)
   {
       int i;
       double *t = NULL, *msd_avg = NULL;
       double Dhat, Dtheory;
   
       /* ---------- Initialize random seed ---------- */
       srand((unsigned int) time(NULL));
   
       /* ---------- Allocate arrays ---------- */
       t = (double *) malloc(NSTEPS * sizeof(double));
       msd_avg = (double *) calloc(NSTEPS, sizeof(double)); /* initialized to 0 */
   
       if (t == NULL || msd_avg == NULL) {
           fprintf(stderr, "Memory allocation failed.\n");
           return 1;
       }
   
       /* ---------- Initialize time vector ---------- */
       for (i = 0; i < NSTEPS; i++)
           t[i] = i * DT;
   
       /* ---------- Simulate random walks ---------- */
       simulate_random_walks(msd_avg, NSTEPS, NTRAJ, A);
   
       /* ---------- Compute diffusion coefficient ---------- */
       Dhat = estimate_diffusion(t, msd_avg, NSTEPS);
       Dtheory = (A * A) / (4.0 * DT);
   
       /* ---------- Print results ---------- */
       printf("Estimated D (from MSD fit) = %.4f\n", Dhat);
       printf("Theoretical D              = %.4f\n", Dtheory);
   
       free(t);
       free(msd_avg);
       return 0;
   }
   
   /* ==========================================================
      Simulate multiple random walks and compute ensemble-averaged MSD
      ========================================================== */
   void simulate_random_walks(double *msd_avg, int nsteps, int ntraj, double a)
   {
       int i, j, dir;
       double x, y, r;
       double *msd = NULL;
   
       msd = (double *) malloc(nsteps * sizeof(double));
       if (msd == NULL) {
           fprintf(stderr, "Memory allocation failed in simulate_random_walks.\n");
           exit(1);
       }
   
       for (i = 0; i < nsteps; i++)
           msd_avg[i] = 0.0;
   
       for (j = 0; j < ntraj; j++) {
           x = 0.0;
           y = 0.0;
           msd[0] = 0.0;
   
           for (i = 1; i < nsteps; i++) {
               /* random direction: 1 to 4 */
               r = (double) rand() / RAND_MAX;
               dir = (int)(4 * r) + 1;
   
               switch (dir) {
                   case 1: x += a; break; /* right */
                   case 2: x -= a; break; /* left  */
                   case 3: y += a; break; /* up    */
                   case 4: y -= a; break; /* down  */
               }
   
               msd[i] = x*x + y*y;
           }
   
           /* accumulate MSD for ensemble average */
           for (i = 0; i < nsteps; i++)
               msd_avg[i] += msd[i];
       }
   
       /* compute ensemble average */
       for (i = 0; i < nsteps; i++)
           msd_avg[i] /= (double) ntraj;
   
       free(msd);
   }
   
   /* ==========================================================
      Estimate diffusion coefficient from MSD(t)
      Fit ⟨r²⟩ = slope * t  →  D = slope / 4  (in 2D)
      ========================================================== */
   double estimate_diffusion(double *t, double *msd_avg, int nsteps)
   {
       int i;
       double num = 0.0, den = 0.0, slope;
   
       for (i = 1; i < nsteps; i++) { /* skip t=0 */
           num += t[i] * msd_avg[i];
           den += t[i] * t[i];
       }
   
       slope = num / den;
       return slope / 4.0;
   }
   ```
   </details>
   
   <details>
     <summary>FORTRAN code</summary>
   
   ```Fortran
   program estimate_D_from_MSD
     implicit none
     ! ==========================================================
     ! Simulation of 2D Brownian motion
     ! Estimate diffusion coefficient D from the mean square displacement (MSD)
     ! ==========================================================
   
     integer, parameter :: nsteps = 2000      ! time steps per trajectory
     integer, parameter :: ntraj  = 2000      ! number of independent trajectories
     real,    parameter :: a  = 1.0           ! step length
     real,    parameter :: dt = 1.0           ! time step
   
     real, allocatable :: t(:), msd_avg(:)
     real              :: Dhat, Dtheory
     integer           :: i
   
     ! ---------------- Allocate and initialize time ------------------
     allocate(t(nsteps))
     do i = 1, size(t)
       t(i) = (i - 1) * dt
     end do
   
     ! ---------------- Simulate random walks ------------------------
     allocate(msd_avg(nsteps))
     call simulate_random_walks(msd_avg, nsteps, ntraj, a)
   
     ! ---------------- Compute diffusion coefficient ----------------
     Dhat = estimate_diffusion(t, msd_avg, nsteps)
     Dtheory = (a*a) / (4.0*dt)
   
     ! ---------------- Print results ----------------
     print '(A,F8.4)', 'Estimated D (from MSD fit) = ', Dhat
     print '(A,F8.4)', 'Theoretical D              = ', Dtheory
   
     deallocate(t, msd_avg)
   
   contains
   
     subroutine simulate_random_walks(msd_avg, nsteps, ntraj, a)
       implicit none
       real, intent(out)   :: msd_avg(:)
       integer, intent(in) :: nsteps, ntraj
       real, intent(in)    :: a
   
       real, allocatable :: msd(:)
       real              :: x, y, r
       integer           :: i, j, dir
   
       allocate(msd(nsteps))
       msd_avg = 0.0
   
       do j = 1, ntraj
         x = 0.0
         y = 0.0
         msd(1) = 0.0
   
         do i = 2, nsteps
           call random_number(r)
           dir = int(4*r) + 1
   
           select case (dir)
           case (1)
             x = x + a       ! right
           case (2)
             x = x - a       ! left
           case (3)
             y = y + a       ! up
           case (4)
             y = y - a       ! down
           end select
   
           msd(i) = x*x + y*y
         end do
   
         ! Accumulate MSD for ensemble average
         msd_avg = msd_avg + msd
       end do
   
       ! Ensemble average
       msd_avg = msd_avg / real(ntraj)
   
       deallocate(msd)
     end subroutine simulate_random_walks
   
     real function estimate_diffusion(t, msd_avg, nsteps)
       implicit none
       real, intent(in) :: t(:), msd_avg(:)
       integer, intent(in) :: nsteps
       real :: num, den, slope
       integer :: i
   
       num = 0.0
       den = 0.0
   
       ! Fit MSD(t) = slope * t  →  D = slope / 4 (in 2D)
       do i = 2, nsteps
         num = num + t(i) * msd_avg(i)
         den = den + t(i) * t(i)
       end do
       slope = num / den
       estimate_diffusion = slope / 4.0
     end function estimate_diffusion
   end program estimate_D_from_MSD
   ```
   </details>

## Discussion and Interpretation

When running this code with a large number of trajectories (e.g., 2000 or more), the estimated diffusion coefficient $D_\text{num}$ will fluctuate slightly around the theoretical value $D_\text{theory} = a^2/(4\Delta t)$.
The fluctuations decrease as the number of trajectories increases, following the law of large numbers.

This simulation highlights how macroscopic diffusion emerges from microscopic randomness.
Although the motion of a single particle appears completely irregular, the ensemble-averaged dynamics obey a simple, deterministic law.
Such stochastic simulations serve as a bridge between microscopic models (molecular collisions, random walks) and macroscopic transport equations, such as the diffusion equation:

$$\frac{\partial c}{\partial t} = D \nabla^2 c,$$

where $c(\mathbf{r},t)$ is the probability (or concentration) of finding the particle at position $\mathbf{r}$ at time $t$.

This connection between random motion and diffusion is one of the most beautiful demonstrations of statistical physics — showing how order and predictability can emerge from pure randomness.

# Separating Simulation and Analysis

We now **modularize** the workflow by separating the simulation phase from the data analysis phase. Starting from a single monolithic program that simulates 2D Brownian motion and estimates the diffusion coefficient from the mean squared displacement (MSD), the task is to divide the workflow into two distinct programs:
	1.	A **simulation program** that generates particle trajectories and writes them to a file.
	2.	An **analysis program** that reads the trajectory file, computes the MSD, and estimates the diffusion coefficient.
	
This exercise is designed to encourage good computational practices that are essential in both academic research and industrial software development:
- **Modularity**: By isolating the simulation from the analysis, each component can be developed, tested, and reused independently.
- **Reusability**: Once trajectories are stored in a file, they can be reused for alternative analyses or post-processing steps without repeating the simulation.
- **Reproducibility**: Saving simulation output allows results to be archived, versioned, and reproduced at a later time, which is critical for scientific reliability.
- **Efficiency**: In realistic applications, simulations may be computationally expensive. Decoupling simulation and analysis avoids the need to re-run simulations unnecessarily.

## Code structures
We start from the provided C/Fortran code — which currently generates Brownian trajectories, computes the MSD, and estimates the diffusion coefficient in a single program — and split it into two:
1.	**Simulation Program** 
	- **Generates** NTRAJ independent 2D Brownian trajectories, each with NSTEPS time steps.
 	- **Saves** the full trajectory data to a plain text file (e.g., trajectories.dat), where each line contains:
	```Plain
	traj_id  step  time  x  y
	```
 	- Ensures the file is formatted in a way that is easy to parse and analyze.

2.	Analysis Program (analyze.c)
	- **Reads** the trajectory file and reconstructs the particle displacements.
	- **Computes** the ensemble-averaged mean squared displacement (MSD) as a function of time.
 	- **Estimates the diffusion coefficient** by performing a linear fit of MSD versus time, according to the theoretical relation.
  	- Compares the estimated diffusion coefficient to the theoretical one, and prints both values.

> [!TIP]
> One can validate the analysis using visual inspection of the MSD curve (e.g., by plotting it in Python or gnuplot), and to discuss the statistical convergence as a function of the number of trajectories.

<details>
  <summary>C code</summary>
   This is the code that performs the simulation:
   
   ```C
   #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* ==========================================================
   Simulation of 2D Brownian motion
   Writes all trajectories to a file for later analysis
   ========================================================== */

#define NSTEPS 2000     /* time steps per trajectory */
#define NTRAJ  2000     /* number of independent trajectories */
#define A      1.0      /* step length */
#define DT     1.0      /* time step */

/* ---------- Function declaration ---------- */
void simulate_and_write(const char *filename, int nsteps, int ntraj, double a, double dt);

int main(void)
{
    srand((unsigned int) time(NULL));

    printf("Simulating %d trajectories of %d steps each...\n", NTRAJ, NSTEPS);
    simulate_and_write("trajectories.dat", NSTEPS, NTRAJ, A, DT);
    printf("Trajectories written to file 'trajectories.dat'\n");

    return 0;
}

/* ==========================================================
   Simulate Brownian motion and write trajectories to file
   ========================================================== */
void simulate_and_write(const char *filename, int nsteps, int ntraj, double a, double dt)
{
    FILE *fp;
    int i, j, dir;
    double x, y, r;

    fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file %s for writing.\n", filename);
        exit(1);
    }

    /* Write header */
    fprintf(fp, "# traj_id  step  time  x  y\n");

    for (j = 0; j < ntraj; j++) {
        x = 0.0;
        y = 0.0;
        for (i = 0; i < nsteps; i++) {

            /* Write current position */
            fprintf(fp, "%d %d %.3f %.6f %.6f\n", j, i, i * dt, x, y);

            /* Random step */
            r = (double) rand() / RAND_MAX;
            dir = (int)(4 * r) + 1;
            switch (dir) {
                case 1: x += a; break; /* right */
                case 2: x -= a; break; /* left  */
                case 3: y += a; break; /* up    */
                case 4: y -= a; break; /* down  */
            }
        }
    }

    fclose(fp);
}
   ```
And this is the code that performs the analysis:

```C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ==========================================================
   Read trajectory file and compute MSD and diffusion coefficient
   ========================================================== */

#define NSTEPS 2000
#define NTRAJ  2000
#define DT     1.0

double estimate_diffusion(double *t, double *msd_avg, int nsteps);

int main(void)
{
    FILE *fp;
    int traj, step;
    double time, x, y;
    double *t = NULL, *msd_avg = NULL;
    double Dhat, Dtheory;

    /* Allocate arrays */
    t = (double *) malloc(NSTEPS * sizeof(double));
    msd_avg = (double *) calloc(NSTEPS, sizeof(double));
    if (t == NULL || msd_avg == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    /* Initialize time */
    for (step = 0; step < NSTEPS; step++)
        t[step] = step * DT;

    /* Open file */
    fp = fopen("trajectories.dat", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file trajectories.dat\n");
        return 1;
    }

    /* Skip header line */
    char line[256];
    fgets(line, sizeof(line), fp);

    /* Temporary arrays for current trajectory */
    double *r2 = (double *) malloc(NSTEPS * sizeof(double));
    if (r2 == NULL) {
        fprintf(stderr, "Memory allocation failed for r2.\n");
        return 1;
    }

    int current_traj = -1;
    while (fscanf(fp, "%d %d %lf %lf %lf", &traj, &step, &time, &x, &y) == 5) {

        /* New trajectory detected */
        if (traj != current_traj) {
            if (current_traj >= 0) {
                /* Accumulate into MSD average */
                for (int i = 0; i < NSTEPS; i++)
                    msd_avg[i] += r2[i];
            }
            current_traj = traj;
        }

        /* Compute r² for this step */
        r2[step] = x * x + y * y;
    }

    /* Add last trajectory */
    for (int i = 0; i < NSTEPS; i++)
        msd_avg[i] += r2[i];

    fclose(fp);
    free(r2);

    /* Compute ensemble average */
    for (step = 0; step < NSTEPS; step++)
        msd_avg[step] /= (double) NTRAJ;

    /* Estimate diffusion coefficient */
    Dhat = estimate_diffusion(t, msd_avg, NSTEPS);
    Dtheory = 1.0 / 4.0; /* since A = DT = 1.0 */

    printf("Estimated D (from MSD fit) = %.4f\n", Dhat);
    printf("Theoretical D              = %.4f\n", Dtheory);

    free(t);
    free(msd_avg);
    return 0;
}

/* ==========================================================
   Estimate diffusion coefficient from MSD(t)
   Fit ⟨r²⟩ = slope * t → D = slope / 4  (in 2D)
   ========================================================== */
double estimate_diffusion(double *t, double *msd_avg, int nsteps)
{
    int i;
    double num = 0.0, den = 0.0, slope;

    for (i = 1; i < nsteps; i++) { /* skip t=0 */
        num += t[i] * msd_avg[i];
        den += t[i] * t[i];
    }

    slope = num / den;
    return slope / 4.0;
}
```
</details>
   
<details>
   <summary>FORTRAN code</summary>
   This is the code that performs the simulation:
   
   ```Fortran
   program simulate_brownian
     implicit none
     integer, parameter :: nsteps = 2000, ntraj = 2000
     real,    parameter :: a = 1.0, dt = 1.0
   
     real :: x, y, r
     integer :: i, j, dir
     character(len=*), parameter :: outfile = "trajectories.dat"
     open(unit=10, file=outfile, status="replace")
   
     call random_seed()
   
     do j = 1, ntraj
       x = 0.0
       y = 0.0
       write(10,*) j, 1, 0.0, x, y  ! initial position
   
       do i = 2, nsteps
         call random_number(r)
         dir = int(4*r) + 1
   
         select case (dir)
         case (1)
           x = x + a
         case (2)
           x = x - a
         case (3)
           y = y + a
         case (4)
           y = y - a
         end select
   
         write(10,*) j, i, (i - 1) * dt, x, y
       end do
     end do
   
     close(10)
     print *, "Simulation complete. Trajectories written to ", outfile
   end program simulate_brownian
   ```
  And this is the code that performs the analysis:
  ```Fortran
   program analyze_trajectories
     implicit none
     integer, parameter :: nsteps = 2000, ntraj = 2000
     real,    parameter :: dt = 1.0, a = 1.0
     real, allocatable  :: t(:), msd_avg(:)
     real               :: Dhat, Dtheory
     character(len=*), parameter :: infile = "trajectories.dat"
   
     allocate(t(nsteps), msd_avg(nsteps))
     call initialize_time(t, nsteps, dt)
     call read_and_compute_msd(msd_avg, nsteps, ntraj, infile)
     Dhat     = estimate_diffusion(t, msd_avg, nsteps)
     Dtheory  = (a*a) / (4.0 * dt)
   
     print '(A,F8.4)', 'Estimated D (from MSD fit) = ', Dhat
     print '(A,F8.4)', 'Theoretical D              = ', Dtheory
   
     deallocate(t, msd_avg)
   
   contains
   
     subroutine initialize_time(t, nsteps, dt)
       real, intent(out) :: t(:)
       integer, intent(in) :: nsteps
       real, intent(in) :: dt
       integer :: i
       do i = 1, nsteps
         t(i) = (i - 1) * dt
       end do
     end subroutine
   
     subroutine read_and_compute_msd(msd_avg, nsteps, ntraj, filename)
       real, intent(out) :: msd_avg(:)
       integer, intent(in) :: nsteps, ntraj
       character(len=*), intent(in) :: filename
   
       real, allocatable :: x(:), y(:)
       integer :: id, step, i, j, ios
       real :: tval, xpos, ypos
   
       allocate(x(nsteps), y(nsteps))
       msd_avg = 0.0
       x = 0.0
       y = 0.0
   
       open(unit=11, file=filename, status="old", iostat=ios)
       if (ios /= 0) then
         print *, "Error opening file ", filename
         stop
       end if
   
       do j = 1, ntraj
         do i = 1, nsteps
           read(11,*,iostat=ios) id, step, tval, xpos, ypos
           if (ios /= 0) exit
           x(i) = xpos
           y(i) = ypos
         end do
         do i = 1, nsteps
           msd_avg(i) = msd_avg(i) + x(i)**2 + y(i)**2
         end do
       end do
   
       msd_avg = msd_avg / real(ntraj)
       close(11)
       deallocate(x, y)
     end subroutine
   
     real function estimate_diffusion(t, msd_avg, nsteps)
       real, intent(in) :: t(:), msd_avg(:)
       integer, intent(in) :: nsteps
       real :: num, den, slope
       integer :: i
   
       num = 0.0
       den = 0.0
   
       do i = 2, nsteps
         num = num + t(i) * msd_avg(i)
         den = den + t(i) * t(i)
       end do
   
       slope = num / den
       estimate_diffusion = slope / 4.0
     end function
end program analyze_trajectories
```

</details>


## Video visualisation

```Python
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# ---------- Parameters ----------
filename = "trajectories.txt"  # Your data file
n_particles_to_show = 10000      # Number of particles to include in animation
output_file = "trajectories_xy.mp4"
save_video = True              # Set to False to just display interactively

# ---------- Load and preprocess data ----------
data = np.loadtxt(filename, comments="#")
ids = data[:, 0].astype(int)
steps = data[:, 1].astype(int)
times = data[:, 2]
x = data[:, 3]
y = data[:, 4]

unique_ids = np.unique(ids)
n_steps = np.sum(ids == unique_ids[0])
time_vector = times[ids == unique_ids[0]]

# Select a subset of particles if needed
selected_ids = unique_ids[:min(n_particles_to_show, len(unique_ids))]
n_selected = len(selected_ids)

# Prepare data array: shape (n_steps, n_selected, 2)
trajectories = np.zeros((n_steps, n_selected, 2))
for i, pid in enumerate(selected_ids):
    mask = ids == pid
    trajectories[:, i, 0] = x[mask]
    trajectories[:, i, 1] = y[mask]

# ---------- Set up the animation ----------
fig, ax = plt.subplots(figsize=(6, 6))
scat = ax.scatter([], [], s=20, c='blue')
time_text = ax.text(0.02, 0.95, '', transform=ax.transAxes)

# Set axis limits
all_x = x[ids <= selected_ids[-1]]
all_y = y[ids <= selected_ids[-1]]
padding = 1.0
ax.set_xlim(np.min(all_x) - padding, np.max(all_x) + padding)
ax.set_ylim(np.min(all_y) - padding, np.max(all_y) + padding)
ax.set_title("Particle Trajectories on X–Y Lattice")
ax.set_xlabel("X")
ax.set_ylabel("Y")

# ---------- Animation functions ----------
def init():
    # must be a 2D empty array, not 1D
    scat.set_offsets(np.empty((0, 2)))
    time_text.set_text('')
    return scat, time_text

def update(frame):
    coords = trajectories[frame]  # shape (n_selected, 2)
    scat.set_offsets(coords)
    time_text.set_text(f'Time = {time_vector[frame]:.2f}')
    return scat, time_text

ani = animation.FuncAnimation(
    fig, update, frames=n_steps, init_func=init,
    blit=True, interval=30
)

# ---------- Export or display ----------
if save_video:
    ani.save(output_file, fps=30, dpi=150, extra_args=['-vcodec', 'libx264'])
    print(f"Video saved as {output_file}")
else:
    plt.show()
```
