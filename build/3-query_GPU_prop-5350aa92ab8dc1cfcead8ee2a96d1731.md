# Query GPU properties 
Here is a CUDA C program to query the properties of the available GPUs on your system using the `cudaGetDeviceCount` and `cudaGetDeviceProperties` functions:

```C
#include <stdio.h>
#include <cuda_runtime.h>

int main() {
    int deviceCount = 0;

    // Get the number of available GPUs
    cudaError_t err = cudaGetDeviceCount(&deviceCount);

    if (err != cudaSuccess) {
        printf("Error querying the number of GPUs: %s\n", cudaGetErrorString(err));
        return -1;
    }

    printf("Number of CUDA-capable GPUs: %d\n\n", deviceCount);

    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, i);

        printf("GPU #%d: %s\n", i, deviceProp.name);
        printf("  Compute Capability: %d.%d\n", deviceProp.major, deviceProp.minor);
        printf("  Total Global Memory: %.2f GB\n", deviceProp.totalGlobalMem / (1024.0 * 1024.0 * 1024.0));
        printf("  Shared Memory Per Block: %.2f KB\n", deviceProp.sharedMemPerBlock / 1024.0);
        printf("  Registers Per Block: %d\n", deviceProp.regsPerBlock);
        printf("  Warp Size: %d\n", deviceProp.warpSize);
        printf("  Maximum Threads Per Block: %d\n", deviceProp.maxThreadsPerBlock);
        printf("  Maximum Block Dimensions: (%d, %d, %d)\n",
               deviceProp.maxThreadsDim[0],
               deviceProp.maxThreadsDim[1],
               deviceProp.maxThreadsDim[2]);
        printf("  Maximum Grid Dimensions: (%d, %d, %d)\n",
               deviceProp.maxGridSize[0],
               deviceProp.maxGridSize[1],
               deviceProp.maxGridSize[2]);
        printf("  Clock Rate: %.2f MHz\n", deviceProp.clockRate / 1000.0);
        printf("  Memory Bus Width: %d bits\n", deviceProp.memoryBusWidth);
        printf("  Memory Bandwidth: %.2f GB/s\n\n",
               2.0 * deviceProp.memoryClockRate * (deviceProp.memoryBusWidth / 8) / 1.0e6);
    }

    return 0;
}
```

The output looks like:

```bash
GPU #0: Tesla P100-PCIE-16GB
  Compute Capability: 6.0
  Total Global Memory: 15.89 GB
  Shared Memory Per Block: 48.00 KB
  Registers Per Block: 65536
  Warp Size: 32
  Maximum Threads Per Block: 1024
  Maximum Block Dimensions: (1024, 1024, 64)
  Maximum Grid Dimensions: (2147483647, 65535, 65535)
  Clock Rate: 1328.50 MHz
  Memory Bus Width: 4096 bits
  Memory Bandwidth: 732.16 GB/s
```
