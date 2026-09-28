# Strong scaling and weak scaling  in Parallel Computing

When working with parallel computing, it is important to evaluate the performance of algorithms as the number of processors or nodes is increased. Two common metrics used to assess performance in parallel computing are **strong scaling** and **weak scaling**. These metrics help understand how well a parallel system can handle increasing computational demands as resources are added.

## Strong Scaling

**Strong scaling** refers to the ability of a parallel system to solve a **fixed-size problem** faster as **more computational resources** (such as CPUs or nodes) are added. The key idea here is that the **problem size remains constant while the number of processors increases**. Strong scaling measures how efficiently the parallel system can reduce the execution time by dividing the work across more processors.

- **Goal:** Minimize the time to complete a fixed-size problem as the number of processors increases.
- **Ideal Scaling:** In an ideal scenario, adding more processors would reduce the execution time proportionally, meaning that doubling the number of processors would halve the execution time.
  
**Example of Strong Scaling:**
Suppose we have a computational problem that takes 1000 seconds to solve on 1 processor. If we add more processors, strong scaling aims to reduce the solution time. For example, if the problem takes 500 seconds with 2 processors, 250 seconds with 4 processors, and 125 seconds with 8 processors, this demonstrates **ideal strong scaling**.

However, strong scaling is limited by factors such as:
- **Overhead of Parallelization:** Increased communication between processors can introduce overhead that reduces the expected performance gain.
- **Load Balancing:** Not all problems can be evenly divided, leading to idle processors or unbalanced workloads, which can hinder scaling efficiency.
- **Amdahl's Law:** This law shows that the maximum performance improvement from parallelization is limited by the serial portion of the code. Even with an infinite number of processors, the speedup is constrained by the time spent in the non-parallelizable part of the code.

## Weak Scaling

**Weak scaling**, on the other hand, evaluates how the performance of a parallel system changes as **both the problem size** and the **number of processors** are increased proportionally. In weak scaling, the goal is to **maintain constant computational workload per processor** as you increase the number of processors. The **problem size grows with the number of processors**, so each processor is assigned an equal portion of the problem.

- **Goal:** Maintain the same workload per processor as more processors are added, while keeping the total computation proportional to the number of processors.
- **Ideal Scaling:** In an ideal weak scaling scenario, as you increase the number of processors, the time to solve the problem should remain constant because each processor handles an equal share of the increasing workload.

**Example of Weak Scaling:**
Consider a scenario where you are solving a problem that takes 1000 seconds on 1 processor. If you double the number of processors to 2, the problem size also doubles, and the workload per processor remains the same. In an ideal weak scaling scenario, if the problem size increases proportionally, the execution time should also remain close to 1000 seconds, as long as the problem is balanced and the communication overhead does not grow too large.

Weak scaling is often more relevant in situations where large problems need to be solved and where the number of processors must be increased to handle the growing size of the problem. 
Weak scaling helps determine how well a parallel system can handle increasing problem sizes.

## Comparing Strong and Weak Scaling

- **Strong Scaling:**
  - Keeps the problem size fixed.
  - Measures how quickly the problem can be solved with more processors.
  - Efficiency depends on how well the problem can be parallelized and how much overhead there is in communication and synchronization.

- **Weak Scaling:**
  - Increases both the problem size and the number of processors.
  - Measures how well the system handles larger problems as more processors are added, with each processor doing a proportional amount of work.
  - Efficiency depends on how well the system scales with increased problem size and how efficiently the work is distributed.

#### Visualizing Strong and Weak Scaling

- **Strong Scaling:** You would expect a decreasing curve in the time-to-solution as the number of processors increases. The curve flattens out once the system reaches a point where further increases in the number of processors do not significantly reduce the execution time due to overheads.
  
- **Weak Scaling:** The execution time should remain roughly constant as the number of processors increases, provided that the problem size increases in proportion. Any significant deviation from this ideal curve indicates poor scaling.

#### Practical Considerations

- **Strong scaling** is most useful for problems where the total amount of work is fixed, such as simulations or numerical calculations with a fixed dataset or model.
  
- **Weak scaling** is more applicable to problems where the size of the problem grows with the number of resources, such as large-scale simulations of physical systems (e.g., climate modeling or molecular dynamics simulations).

Both strong and weak scaling provide valuable insights into how well a parallel system can handle increased computational demands and can guide optimization efforts in algorithm design and system configuration.

# Speedup in Parallel Computing

In parallel computing, **speedup** is a measure of how much faster a parallel algorithm performs compared to a serial (non-parallel) algorithm. It is one of the key metrics for evaluating the effectiveness of parallelization and helps determine whether the effort to parallelize a program is worthwhile.

## Definition of Speedup

The **speedup** $S$ is defined as the ratio of the time taken to execute the serial version of a program to the time taken to execute the parallel version of the program. Mathematically, it is expressed as:

$$
S = \frac{T_{\text{serial}}}{T_{\text{parallel}}}
$$

Where:
- $T_{\text{serial}}$ is the execution time of the program when run on a single processor (the serial time).
- $T_{\text{parallel}}$ is the execution time of the program when run on multiple processors (the parallel time).

## Ideal Speedup

The ideal speedup is obtained when the parallel algorithm achieves perfect scalability, meaning that the execution time decreases in direct proportion to the number of processors used. For example, if a program takes 100 seconds to run on 1 processor, the ideal speedup would be:

- With 2 processors, the execution time should be $T_{\text{parallel}} = 50$ seconds, yielding a speedup of $S = \frac{100}{50} = 2$.
- With 4 processors, the execution time should be $T_{\text{parallel}} = 25$ seconds, yielding a speedup of $S = \frac{100}{25} = 4$.

In this case, the speedup is proportional to the number of processors, which is the ideal scenario.

## Amdahl’s Law and Its Impact on Speedup

**Amdahl’s Law** provides an upper bound on the speedup that can be achieved by parallelizing a program. It accounts for the fact that not all parts of a program can be parallelized, and some portion must always remain serial.

Amdahl’s Law states that the maximum speedup $S_{\text{max}}$ that can be achieved with $P$ processors is given by:

$$
S_{\text{max}} = \frac{1}{(1 - f) + \frac{f}{P}}
$$

Where:
- $f$ is the fraction of the program that can be parallelized (i.e., the parallelizable portion).
- $P$ is the number of processors.

- If $f = 1$ (i.e., the entire program can be parallelized), then the speedup increases linearly with the number of processors.
- If $f$ is less than 1 (which is the usual case), the speedup starts to plateau as the number of processors increases.

For example, if 90% of a program can be parallelized ($f = 0.9$), even with an infinite number of processors, the maximum speedup will be:

$$
S_{\text{max}} = \frac{1}{(1 - 0.9)} = 10
$$

This demonstrates that the serial portion of the program limits the overall speedup, and beyond a certain point, adding more processors will result in diminishing returns.

## Practical Speedup and Diminishing Returns

In practice, the speedup achieved in parallel computing often deviates from the ideal, due to factors such as:

1. **Communication Overhead:** In parallel systems, processors need to exchange data. The time spent on communication (especially in distributed-memory systems like clusters) can reduce the effectiveness of parallelization.
   
2. **Load Imbalance:** If the workload is not evenly distributed among the processors, some processors may be idle while others are overburdened, reducing overall efficiency.

3. **Synchronization Costs:** In parallel systems, synchronizing the execution of different processors (e.g., waiting for all processors to reach a certain point) can add overhead that reduces speedup.

4. **Amdahl’s Law:** The serial portion of the program limits how much speedup can be achieved. Even if most of the program is parallelized, the small serial portion can significantly reduce the total speedup.

## Superlinear Speedup

In some cases, **superlinear speedup** can occur, where the parallel version of a program performs faster than the serial version, even though more processors are used. This can happen due to:

- **Cache effects:** In some cases, parallelization can lead to better cache utilization, reducing memory access times and improving performance.
- **Problem decomposition:** If the problem is divided in a way that allows for more efficient data access patterns or algorithmic improvements, the parallel version can outperform the serial version.
- **Hardware acceleration:** If parallelization leverages specific hardware features (such as GPUs or specialized processors), the speedup may exceed linear scaling.

However, superlinear speedup is usually a result of specific optimizations and should not be expected in all parallel programs.

> [!NOTE]
> ### Visualizing Speedup
> A typical graph for speedup looks like this:
> - **X-axis:** Number of processors (or nodes).
> - **Y-axis:** Speedup $S$.
> In the ideal case, the graph shows a straight line where speedup increases linearly with the number of processors. However, in most practical scenarios, the curve starts to level off due to diminishing returns caused by overheads (communication, synchronization, etc.).

# Example of strong scaling

Consider the following code parallelised with MPI.

```C
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 1024  // Matrix size (can adjust for strong/weak scaling)

void matrix_multiply(double *A, double *B, double *C, int size, int rank, int num_procs) {
    int i, j, k;
    int rows_per_proc = size / num_procs;

    for (i = 0; i < rows_per_proc; i++) {
        for (j = 0; j < size; j++) {
            double sum = 0.0;
            for (k = 0; k < size; k++) {
                sum += A[i * size + k] * B[k * size + j];
            }
            C[i * size + j] = sum;
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, num_procs;
    int i;
    double *A, *B, *C, *local_A, *local_C;
    clock_t start, end;
    double cpu_time_used;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int rows_per_proc = N / num_procs;
    int matrix_size = N * N;

    // Memory allocation
    A = (double*) malloc(matrix_size * sizeof(double));
    B = (double*) malloc(matrix_size * sizeof(double));
    C = (double*) malloc(matrix_size * sizeof(double));

    // Initialize matrices
    for (i = 0; i < matrix_size; i++) {
      A[i] = 1.0;
      B[i] = 1.0;
    }

    local_A = (double*) malloc(rows_per_proc * N * sizeof(double));
    local_C = (double*) malloc(rows_per_proc * N * sizeof(double));

    start = clock();
    // Perform matrix multiplication on each process
    matrix_multiply(local_A, B, local_C, N, rank, num_procs);

    // Gather the results from all processes
    MPI_Gather(local_C, rows_per_proc * N, MPI_DOUBLE, C, rows_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    if(rank==0) printf("%d %f\n", num_procs, cpu_time_used*1000);
    
    // Finalize and free memory
    free(A);
    free(B);
    free(C);
    free(local_A);
    free(local_C);

    MPI_Finalize();
    return 0;
}
```

In this code, the size is fixed ($N=1024$) and we are going to vary the number of processors. 
Note that the times are taken right before the call to `matrix_multiply` and after `MPI_Gather`. In this way, we are only measuring the time it takes to perform the calculations (in parallel) and to communicate. Note also that before calling `end = clock();`, we invoke `MPI_Barrier`: this is usefull when the MPI calls are not asynchronus. 

Here a bash script that can be used to run multiple time the above code varying the number of processors:

```bash
echo "#np  #t[ms]"
for np in 1 2 4 8 16 32;
do
  mpirun -np $np ./exe
done
```

The following plot shows the strong scaling with 16 processors. 

![plot](scaling.png)
