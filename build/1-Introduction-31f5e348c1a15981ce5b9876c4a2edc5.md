# Introduction to Multi-CPU Parallelization

## Computer Cluster

A cluster is a **network of interconnected computers**, called **nodes**, that work together to perform parallel computations efficiently. Each node can have multiple processors or cores, allowing **tasks to be distributed** both _across nodes_ and _within them_. This layout is ideal for utilizing **MPI** (Message Passing Interface) for **inter-node communication** and **OpenMP** (Open Multi-Processing) for **multi-threading within nodes**.

- **Nodes:** Each node in a cluster functions as an individual computing unit. Nodes typically contain several CPU cores, memory, and storage. Clusters can contain anywhere from a few to thousands of nodes, depending on their purpose and scale.
- **Interconnect:** The nodes are linked through a high-speed network interconnect, which allows for rapid data transfer across nodes. This interconnect is critical to a cluster’s performance, as it determines how quickly information can be shared between nodes. Common interconnects in high-performance clusters include **Infiniband**, **Ethernet**, and more specialized networking technologies.

In scientific computing and high-performance applications, leveraging multiple CPUs can significantly reduce computation time. Two widely used frameworks for parallelization on multi-CPU systems are **MPI (Message Passing Interface)** and **OpenMP (Open Multi-Processing)**. 

Although both facilitate parallel computation, they rely on different paradigms and are suited for distinct types of applications. Understanding these paradigms allows developers to choose the most efficient approach for their specific computational tasks.

## OpenMP: Shared-Memory Parallelization

OpenMP (Open Multiprocessing) is designed for **shared-memory** parallelization, where **multiple threads** work on different parts of a task within a **single, shared memory space**. This approach allows threads to **directly access and modify the same variables in memory**, making OpenMP well-suited for systems with a single, large memory pool, like **multi-core CPUs**.

OpenMP uses a **directive-based approach** in languages like C and Fortran, allowing you to **mark loops or regions of code** to be executed in parallel. For instance, a simple loop that calculates values for an array can be parallelized with OpenMP by adding a single pragma directive:

```C
#include <omp.h>
#include <stdio.h>

#define N 1000  // total number of elements

int main() {
    int i;
    int array[N];

    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        array[i] = i * i; // each thread computes part of the array
    }

    // (Optional) Print result
	for (i = 0; i < N; i++) {
		printf("array[%d] = %d\n", i, array[i]);
	}

    return 0;
}
```

In this example, OpenMP automatically **splits the loop** among available **threads**, each of which independently performs its calculations on a portion of the array.

## MPI: Distributed-Memory Parallelization

MPI is suited for **distributed-memory systems**, where each CPU has **its own local memory**. Rather than sharing memory, processes communicate by **sending messages to each other**. This model is particularly useful in **large-scale computing clusters**, where multiple nodes with independent memory need to work together on complex computations.

MPI is implemented through **library functions** that handle the **sending and receiving** of data between processes. Here’s a simple example in C illustrating how MPI can be used to distribute computations across multiple processes:

```C
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000  // total number of elements

int main(int argc, char *argv[]) {
    int rank, size;
    int *global_array = NULL;
    int *local_array = NULL;
    int i, local_n;

    MPI_Init(&argc, &argv);                 // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get total number of processes

    // Determine the number of elements per process
    local_n = N / size;

    // Allocate local array
    local_array = malloc(local_n * sizeof(int));

    // Each process computes its portion of the array
    int start = rank * local_n;
    for (i = 0; i < local_n; i++) {
        local_array[i] = (start + i) * (start + i);
    }

    // Allocate global array on root process
    if (rank == 0) global_array = malloc(N * sizeof(int));

    // Gather all local arrays into the global array on rank 0
    MPI_Gather(local_array, local_n, MPI_INT, global_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // (Optional) On rank 0, print the result
    if (rank == 0) {
        printf("Results:\n");
        for (i = 0; i < N; i++) { printf("array[%d] = %d\n", i, global_array[i]);}
        free(global_array);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
```

This example shows how each process works independently, and coordination between processes requires **explicit communication** through MPI functions.

## Key Differences between MPI and OpenMP

- **Memory Model**: _OpenMP_ relies on _shared memory_, making it more straightforward but limited to systems with shared memory architectures (typically single-node systems). On the other hand, _MPI_ uses _distributed memory_, which is ideal for multi-node clusters.
- **Scalability**: _MPI_ is _more scalable_ across nodes, as each process has its own memory. OpenMP’s shared memory model generally scales well only within a single node.
- **Communication Overhead**: _OpenMP_ has _lower overhead_ for data access since memory is shared. _MPI_, however, has _higher overhead_ due to message passing, which involves _data transfer between separate memory spaces_.
- **Ease of Use**: _OpenMP is typically easier_ to implement in code, as it requires fewer modifications to existing code (mostly through compiler directives). _MPI requires explicit communication_, which can make it more complex but offers greater control over data distribution and process management.

> ### Choosing Between MPI and OpenMP
> The choice between MPI and OpenMP depends on the problem size and the hardware architecture. For multi-core CPUs or systems with shared memory, OpenMP is often more convenient and efficient. However, for computations that need to span multiple nodes or require significant data movement, MPI is usually the better choice. For hybrid systems with both shared and distributed memory (e.g., a cluster of multi-core nodes), a combination of MPI and OpenMP can maximize performance, using MPI for inter-node communication and OpenMP for intra-node parallelism.

## Example Setup: MPI and OpenMP

Consider 4 nodes on a cluster. Each node has:
- **CPUs**: 2 CPUs per node
- **Cores**: 32 cores per CPU
- **Total Cores per Node**: 64 cores

In such a configuration, you can either use **MPI** on $4\times 64$ cores, or you can use the **hybrid MPI/OpenMP**: in the latter case, you run **MPI** on $4\times 2$ CPUs with each MPI process using 32 **OpenMP** threads.

### Practical Example: Solving a Large Problem with MPI + OpenMP

Imagine you need to perform a large-scale numerical integration that would take too long on a single core:
	
 1.	Using **MPI**, you can divide the integration domain into smaller subdomains, assigning one to each node. Each node then focuses on a portion of the problem, making inter-node communication essential to manage the shared workload.
2.	Within each node, **OpenMP** can parallelize calculations across the 64 cores, speeding up the integration of the assigned subdomain.
3.	Finally, MPI gathers the partial results from each node, combining them to produce the final solution.

This combination of MPI and OpenMP might be powerful. It allows clusters to solve problems at a scale and speed that would be impossible on a single machine.

# How to install

## OpenMP

OpenMP is a specification supported by many compilers, such as GCC, Clang, and Intel compilers. To use OpenMP, you need a compatible compiler and enable OpenMP support during compilation. 

## MPI

To install MPI, you typically install an implementation such as **OpenMPI** or **MPICH**. 

Here's a step-by-step guide depending on your operating system:

<details>
  <summary> Linux </summary>
Most Linux distributions have precompiled versions of MPI in their package managers.

#### **Option 1: Using Package Manager**
1. **OpenMPI**
   ```bash
   sudo apt update
   sudo apt install openmpi-bin openmpi-common libopenmpi-dev
   ```
   For Fedora/RHEL:
   ```bash
   sudo dnf install openmpi openmpi-devel
   ```

2. **MPICH**
   ```bash
   sudo apt install mpich
   ```
   For Fedora/RHEL:
   ```bash
   sudo dnf install mpich mpich-devel
   ```

#### **Option 2: From Source**
1. **Download the Source**
   - [OpenMPI Downloads](https://www.open-mpi.org/software/ompi/)
   - [MPICH Downloads](https://www.mpich.org/downloads/)

2. **Build and Install**
   ```bash
   tar -xzf openmpi-x.x.x.tar.gz   # Replace x.x.x with the version number
   cd openmpi-x.x.x
   ./configure --prefix=/path/to/install
   make -j$(nproc)
   sudo make install
   ```

   Add MPI to your path:
   ```bash
   export PATH=/path/to/install/bin:$PATH
   export LD_LIBRARY_PATH=/path/to/install/lib:$LD_LIBRARY_PATH
   ```
</details>


<details>
  <summary> MacOS </summary>
	
1. **Using Homebrew**
   - **OpenMPI**
     ```bash
     brew install open-mpi
     ```
   - **MPICH**
     ```bash
     brew install mpich
     ```
</details>

### **Verify Installation**
After installation, verify using:
```bash
mpicc --version   # Check the C compiler
mpiexec --version # Check the execution environment
```
