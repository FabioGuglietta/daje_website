# Scaling

Consider the following code parallelised with CUDA C.

```C
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <time.h>

//#define N 1024  // Matrix size

// Kernel for matrix multiplication
__global__ void matrix_multiply_kernel(double *A, double *B, double *C, int size) {
    int row = blockIdx.y * blockDim.y + threadIdx.y; // Row index
    int col = blockIdx.x * blockDim.x + threadIdx.x; // Column index

    if (row < size && col < size) {
        double sum = 0.0;
        for (int k = 0; k < size; k++) {
            sum += A[row * size + k] * B[k * size + col];
        }
        C[row * size + col] = sum;
    }
}

int main(int argc, char* argv[]) {
    double *A, *B, *C;           // Host pointers
    double *d_A, *d_B, *d_C;     // Device pointers
    clock_t start, end;
    double cpu_time_used;

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    if (argc < 4) {
      printf("Usage: %s <N> <blockDim.x> <blockDim.y>:\n", argv[0]);
      printf("<N> in the number of rows and columns (NxN matrix)\n");
      printf("<blockDim.x> is the number of threads in the block (x direction)\n");
      printf("<blockDim.y> is the number of threads in the block (y direction)\n");
      return 1;
    }

    int N = atoi(argv[1]);
    int block_dim_x = atoi(argv[2]);
    int block_dim_y = atoi(argv[3]);

    if(block_dim_x*block_dim_y>prop.maxThreadsPerBlock){
      printf("Max threads per block: %d\n", prop.maxThreadsPerBlock);
      printf("You chose %d x %d = %d threads\n", block_dim_x, block_dim_y, block_dim_x*block_dim_y);
      return 1;
    }

    size_t matrix_size = N * N * sizeof(double);

    // Allocate memory on the host
    A = (double*) malloc(matrix_size);
    B = (double*) malloc(matrix_size);
    C = (double*) malloc(matrix_size);

    // Initialize matrices (example initialization)
    for (int i = 0; i < N * N; i++) {
        A[i] = 0.01*i;
        B[i] = 0.025*i;
    }

    // Allocate memory on the device
    cudaMalloc((void**)&d_A, matrix_size);
    cudaMalloc((void**)&d_B, matrix_size);
    cudaMalloc((void**)&d_C, matrix_size);

    // Copy matrices A and B from host to device
    cudaMemcpy(d_A, A, matrix_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, matrix_size, cudaMemcpyHostToDevice);

    // Define grid and block dimensions
    dim3 blockDim(block_dim_x, block_dim_y);
    dim3 gridDim((N + blockDim.x - 1) / blockDim.x, (N + blockDim.y - 1) / blockDim.y);

    // Start timing
    start = clock();

    // Launch kernel
    matrix_multiply_kernel<<<gridDim, blockDim>>>(d_A, d_B, d_C, N);

    // Wait for the GPU to finish
    cudaError_t err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
      printf("Kernel execution failed: %s\n", cudaGetErrorString(err));
    }

    // Stop timing
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Copy result matrix C from device to host
    cudaMemcpy(C, d_C, matrix_size, cudaMemcpyDeviceToHost);

    //printf("Matrix multiplication completed in %.2f ms\n", cpu_time_used * 1000);
    printf("%d %d %d %.6f\n", N, block_dim_x, block_dim_y, cpu_time_used);

    // Free memory on device
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Free memory on host
    free(A);
    free(B);
    free(C);

    return 0;
}
```

In this code, we vary the size `N` and the number of threads per block, both along x and y (`block_dim_x` and `block_dim_y`). Note that the times are taken right before the call to the kernel `matrix_multiply_kernel` and after the call to `cudaDeviceSynchronize()`. 
This last call is crucial, because **kernel launches are asynchronous**, meaning that the host (CPU) does not wait for the kernel execution on the device (GPU) to finish before continuing to the next line of code. If you measure time immediately after launching a kernel without waiting for its completion, the measured time will not accurately reflect the kernel’s execution duration. 

> [!NOTE]
> ### Kernel execution and `cudaDeviceSynchronize()`
> When you invoke a CUDA kernel (like `matrix_multiply_kernel<<<gridDim, blockDim>>>()`), the kernel is queued for execution but the **host does not block or wait for it to complete**. Without `cudaDeviceSynchronize`, the `clock()` call to stop timing may occur before the kernel has finished executing, leading to incorrect timing. `cudaDeviceSynchronize()` forces the host to wait until all preceding GPU work (including the kernel) is completed. `cudaDeviceSynchronize()` also provides an opportunity to check for errors during kernel execution: if there is an issue (e.g., out-of-bounds memory access), the error can be detected and reported immediately after synchronization.

Here a bash script that can be used to run multiple times the above code varying the input parameters:

```bash
#!/bin/bash

# Define the values of N to iterate over
Ns=(1024 2048 4096)

# Define the thread counts for nx and ny
thread_values=(1 2 4 8 16 32 64 128 256 512 1024)

# Loop over each N
for N in "${Ns[@]}"; do

    # Vary nx with ny fixed to 1
    for nx in "${thread_values[@]}"; do
        ny=1
        if (( nx * ny < 1024 )); then
            ./scaling.exe $N $nx $ny 
        fi  
    done

    # Vary ny with nx fixed to 1
    for ny in "${thread_values[@]}"; do
        nx=1
        if (( nx * ny < 1024 )); then
            ./scaling.exe $N $nx $ny 
        fi  
    done

    # Vary nx and ny equally
    for nx in "${thread_values[@]}"; do
        ny=$nx
        if (( nx * ny < 1024 )); then
            ./scaling.exe $N $nx $ny 
        fi  
    done
done
```

This is the scaling plot:

![plot](cuda_scaling.png)

The plateau observed in execution time as we increase the number of threads along the x-direction in the matrix-matrix multiplication kernel is due to the **saturation of computational resources** and the characteristics of GPU hardware. Here’s why this happens:

### 1. Maximum Occupancy of GPU Resources

- GPUs have a limited number of streaming multiprocessors (SMs), each capable of running a specific number of threads simultaneously.
- Each SM has a finite pool of resources, such as registers, shared memory, and execution units.
- When the number of threads reaches a point where the **GPU is fully utilizing its SMs**, adding more threads does not improve performance. The GPU has reached its maximum occupancy.

For instance, if 256 threads per block fully occupy all the available SMs, increasing to 512 or 1024 threads per block will not yield any additional performance gains because the GPU cannot process more threads simultaneously.

### 2. Memory Bandwidth Saturation

- Matrix-matrix multiplication is **memory-intensive**, involving **frequent memory reads and writes**.
- As you increase the number of threads, the **memory system becomes a bottleneck** because the **global memory bandwidth is finite**.
- Once **memory bandwidth is saturated**, increasing the number of threads **does not reduce execution time** because threads must wait for memory transactions to complete.

### 3. Latency Hiding and Diminishing Returns

- One of the key goals of increasing the number of threads is to **hide memory latency by overlapping memory operations with computations**. However:
    - Once **latency is fully hidden** (e.g., with 32-64 threads per block in our case), adding more threads does not improve performance further.
    - The GPU scheduler cannot overlap more memory transactions than its hardware supports.

### 4. Optimal Thread Configuration

- Your results suggest that the **optimal number of threads per block** for this kernel is between **64 and 256** threads along the x-direction. This configuration:
    - **Maximizes the use of computational resources**.
    - **Balances memory bandwidth** and **arithmetic throughput**.
    - Beyond this range, the **additional threads either remain idle** or compete for the same resources without reducing execution time.

### 5. Explanation of Results

| Threads (x) | Time (s)       | Explanation                                                                 |
|-------------|----------------|-----------------------------------------------------------------------------|
| 1-16        | Decreases      | More threads reduce execution time by parallelizing the workload and hiding latency. |
| 32-64       | Near-optimal   | The GPU achieves maximum performance, balancing computational and memory resources. |
| 128-1024    | Plateau        | The GPU is fully utilized, and additional threads do not improve performance because resources are saturated. |

Here the speedup:

![plot](cuda_speedup.png)
