# GPU Computing

As computational problems grow in complexity and scale, the demand for **high-performance computing** solutions becomes increasingly critical. While traditional CPU-based parallelization techniques like MPI and OpenMP offer substantial gains by distributing tasks across multiple cores and processors, certain applications benefit significantly from a different architectural approach: **Graphics Processing Units (GPUs).**

Originally designed to handle the massive parallelism required for rendering images and videos, GPUs have evolved into powerful tools for general-purpose computing. Unlike **CPUs**, which **excel at executing a few threads at high speed**, **GPUs** are built to handle **thousands of threads simultaneously.** This architectural distinction makes GPUs particularly well-suited for **tasks with a high degree of parallelism,** such as **matrix operations**, **particle simulations**, and **machine learning.**

In this chapter, we will delve into the fundamentals of GPU computing, focusing on the **CUDA** (Compute Unified Device Architecture) programming model. Developed by **NVIDIA**, CUDA enables developers to harness the computational power of GPUs for general-purpose applications. Before exploring the specifics of CUDA, we will examine the key **differences between CPUs and GPUs**, the types of problems **GPUs excel at solving**, and the typical **workflow of a GPU-accelerated application**.

**GPU computing does not replace CPU-based parallelization strategies**; rather, it complements them. **Hybrid solutions** that integrate CPUs, GPUs, and multi-node communication (e.g., using MPI) are increasingly common in modern scientific computing, providing the tools to tackle even the most demanding computational challenges.

## Key Differences Between CPUs and GPUs

Understanding the differences between **Central Processing Units (CPUs)** and **Graphics Processing Units (GPUs)** is crucial to appreciate how these components complement each other in high-performance computing. While both are essential for modern computing systems, they are architected to serve different purposes.

### 1. Architecture: General-purpose vs. Specialized Parallelism

- CPU (Central Processing Unit):
CPUs are designed as **general-purpose processors** capable of executing a wide variety of tasks. They typically consist of a **few powerful cores** optimized for **sequential performance**. Each core is equipped with **substantial cache memory** and **complex control logic**, allowing CPUs to efficiently handle tasks requiring rapid decision-making and minimal parallelism.
- GPU (Graphics Processing Unit):
GPUs, on the other hand, are **specialized for parallel tasks**. A GPU consists of **thousands of smaller, simpler cores** that can **execute the same or similar operations simultaneously** on multiple data elements. This makes GPUs ideal for workloads with high data parallelism, such as image processing, matrix calculations, and simulations.

### 2. Performance Focus: Latency vs. Throughput

- CPU (Latency-Oriented):
CPUs are designed to **minimize latency** for individual tasks, that is, to minimize the time it takes for a single operation or task to complete. CPUs are optimized for low-latency operations like branch-heavy control flows, system management, and single-threaded computations.
- GPU (Throughput-Oriented):
GPUs focus on **maximizing throughput** by processing many operations concurrently. They are **less concerned with the speed of individual operations** and **more with the overall volume of operations** executed over time.

### 3. Memory Hierarchy and Bandwidth

- CPU Memory System:
CPUs rely on a **deep cache hierarchy** (L1, L2, and often L3) to **minimize memory access latency**. The **memory bandwidth is lower than that of GPUs** but optimized for frequent, small, and random memory accesses typical in general-purpose tasks.
- GPU Memory System:
GPUs have access to high-bandwidth memory designed for streaming large amounts of data in parallel. While the latency for accessing GPU memory (global memory) is higher than CPU cache, the architecture compensates with a large number of threads, ensuring many operations can proceed while others wait for memory access.

### 4. Control Logic vs. Compute Density

- CPU:
CPUs dedicate a significant portion of their silicon area to control logic and sophisticated branch prediction. This enables efficient handling of diverse and dynamic workloads but limits the number of cores available.
- GPU:
**GPUs prioritize compute density**, allocating most of their silicon to **arithmetic** and **logic units** (ALUs). This allows them to maximize the number of cores available for parallel computations, at the expense of complex control mechanisms.

### 5. Programming Models

- CPU Programming:
Programming for CPUs often involves traditional languages like **C, C++, or Fortran,** combined with parallelization libraries such as **OpenMP** and **MPI**. These frameworks are designed for task parallelism or domain decomposition.
- GPU Programming:
GPU programming requires specific frameworks like **CUDA** or **OpenCL**, which expose the parallel nature of the GPU to developers. These frameworks require **explicitly managing data transfer between CPU and GPU** and structuring algorithms to leverage the massive parallelism of the GPU.

### 6. Energy Efficiency

- CPU:
CPUs are versatile but consume more power per operation due to their focus on low-latency, complex control logic.
- GPU:
**GPUs are more energy-efficient** for highly parallel workloads, achieving greater computational output per watt for suitable tasks.

### 7. Use Cases

- CPU:
CPUs are better suited for tasks involving decision-making, low-latency requirements, and sequential processes, such as running operating systems, handling interrupts, and processing small-scale simulations.
- GPU:
GPUs excel at **tasks that can be broken into many parallel subtasks**, such as scientific simulations, image rendering, deep learning, and large-scale numerical computations.

These differences make CPUs and GPUs complementary rather than competitive. While CPUs remain central to the orchestration and execution of diverse tasks, GPUs offer unparalleled acceleration for data-parallel computations, enabling hybrid systems that harness the strengths of both.

<figure>
  <img src="ArchCPUGPUcores.png" alt="plot">
  <figcaption>Figure 1: Architecture comparison of CPU and GPU cores. Source: https://cvw.cac.cornell.edu/gpu-architecture/gpu-characteristics/design </figcaption>
</figure>

## GPU Hardware Architecture

<figure>
  <img src="GPU_arch.jpg" alt="plot">
  <figcaption>Figure 2: GPU architecture. Source: 10.1371/journal.pone.0061892 </figcaption>
</figure>

### 1. Streaming Multiprocessors (SMs)

At the core of the GPU are **Streaming Multiprocessors (SMs)**, which are the building blocks of computational power. Each SM contains **multiple CUDA cores** (NVIDIA’s terminology for GPU cores) and additional specialized units. These elements collectively perform the computations required for parallel processing.
- **CUDA Cores**: Lightweight processing units within each SM. They execute arithmetic and logical operations in parallel across thousands of threads.
- Specialized Units:
	- **Tensor Cores**: Accelerate matrix operations, crucial for deep learning and scientific computations.
	- **Registers**: High-speed memory local to the SM for thread-level data storage.
	- **Texture and Load/Store Units**: Optimize memory access for specific tasks.
   
### 2. Memory Hierarchy

Efficient memory access is critical for GPU performance. The memory hierarchy in GPUs is designed to support high bandwidth and minimize latency:
- **Global Memory**: The largest and slowest memory accessible by all threads. It is used to store data shared across the GPU. Due to its high latency, programmers strive to minimize direct access to global memory.
- **Shared Memory**: A small, fast memory shared among threads within the same block on an SM. It allows threads to collaborate efficiently and reduces the need to access global memory.
- **Registers**: Each thread has private registers for storing frequently used variables. These are the fastest memory available on the GPU.
- **High-Bandwidth Memory (HBM)**: Many modern GPUs use HBM for ultra-fast data transfers required by parallel workloads.
- **Texture and Constant Memory**: Specialized memory spaces for specific read-only access patterns, such as spatially coherent data.

### 3. Threads, Blocks, and Grids Hierarchy

GPUs organize computations into a hierarchical model, allowing developers to write scalable parallel programs:
- **Threads**: The smallest unit of execution on a GPU. Each thread executes a single instance of the kernel function (a GPU program) and operates independently.
- **Thread Block**: Threads are grouped into blocks. A block contains up to a maximum number of threads (typically 512 or 1024, depending on the GPU). Threads within a block can:
	- Share data via shared memory.
	- Synchronize using barriers (`__syncthreads()` in CUDA).
Each block runs on a single SM, and its threads are scheduled for execution in warps (groups of 32 threads).
- **Grid**: Blocks are organized into a grid. The grid represents the entire set of computations that the GPU will perform. A grid can be 1D, 2D, or 3D, allowing for flexible mapping of data to computation.

### 4. Warp Execution

Threads within a block are **executed in groups of 32**, called **warps**. All threads in a warp execute the same instruction at the same time (SIMD: Single Instruction, Multiple Data). If threads in a warp take different execution paths (e.g., due to if conditions), the warp must serialize those paths, leading to warp divergence and reduced performance. Minimizing warp divergence is crucial when optimizing GPU programs.

### 5. Memory Bandwidth and Latency Hiding

GPUs achieve high performance by compensating for memory latency with massive parallelism:
- Latency Hiding: While some threads are waiting for memory operations to complete, others continue executing, ensuring the SMs remain fully utilized.
- High Memory Bandwidth: GPUs are equipped with memory systems designed to support the large data transfers required by parallel workloads.

### 6. Interconnects

Modern GPUs use high-speed interconnects to communicate with the CPU and other GPUs. Examples include:
- PCIe (Peripheral Component Interconnect Express): Transfers data between the CPU and GPU. It has significantly lower bandwidth compared to GPU memory, making efficient data transfer strategies essential.
- NVLink: A high-speed GPU-to-GPU interconnect for faster communication in multi-GPU systems.

### 7. Thread Hierarchy Example

Consider a real-world example: processing a 2D image.
- Grid: Represents the entire image. Each block corresponds to a subregion of the image.
- Block: Handles a smaller section of the image (e.g., a 32x32 pixel tile).
- Thread: Processes a single pixel in the tile. Thousands of threads work simultaneously across the grid.

This hierarchical structure ensures scalability: small problems can fit into a single block, while larger problems scale by increasing the number of blocks in the grid.

##  Installing CUDA

Installing CUDA involves downloading and configuring the necessary software to develop GPU-accelerated applications on NVIDIA GPUs. Below is a general guide to install CUDA on Linux and macOS (no Windows-specific instructions per your preference).

### Step 1: Check System Requirements

1.	GPU Compatibility:
- Ensure your NVIDIA GPU is CUDA-compatible. Check the CUDA GPUs list.
2.	Operating System:
- Supported Linux distributions (e.g., Ubuntu, RHEL, Fedora, etc.) or macOS (for cross-compilation).
3.	Supported Compiler:
- Verify the version of GCC or Clang on your system is compatible with the CUDA version. The CUDA release notes specify supported versions.
4.	Driver Version:
- Ensure your NVIDIA driver is updated to a version that supports the desired CUDA version.

### Step 2: Download CUDA Toolkit

1.	Visit the CUDA Toolkit Downloads page.
2.	Select your operating system, architecture, distribution, and version.
3.	Download the installer for the desired CUDA version.

### Step 3: Install CUDA Toolkit

#### For Linux:

1. Update System Packages:
```
sudo apt update
sudo apt upgrade
```

2.	Install NVIDIA Driver:

Use the package manager to install the NVIDIA driver (or verify it’s already installed).

```
sudo apt install nvidia-driver-<version>
```

3.	Install CUDA Toolkit:

Using the downloaded installer, follow the instructions:
```
sudo sh cuda_<version>_linux.run
```

You can also use a package manager for easier updates:
```
sudo apt install cuda
```

4.	Add CUDA to Path:

Add the following to your shell configuration file (e.g., ~/.bashrc or ~/.zshrc):
```
export PATH=/usr/local/cuda/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```

Then apply the changes:
```
source ~/.bashrc
```

___
#### For macOS (for cross-compilation):

1.	Install the CUDA toolkit using the .dmg file downloaded from the NVIDIA website.
2.	Follow the installer steps, then add CUDA to your environment paths manually.

### Step 4: Verify Installation

1.	Check CUDA Version:

```
nvcc --version
```

This should display the installed CUDA version.

2.	Test CUDA Samples:

The CUDA toolkit includes sample programs. Compile and run them to verify the installation:

```
cd /usr/local/cuda/samples
make
cd bin/x86_64/linux/release
./deviceQuery
```

The deviceQuery program should list the properties of your GPU.

## Types of NVIDIA GPUs and Compute Capability

NVIDIA provides a **wide range of GPUs** tailored for different use cases, including **gaming, scientific computing, AI, and deep learning**. CUDA developers should understand the types of GPUs available and the concept of compute capability, which defines the features and hardware capabilities of a GPU for CUDA programming.

### Types of NVIDIA GPUs

1.	**Gaming** GPUs (GeForce Series):
- Designed primarily for gaming and general-purpose computing.
- Examples: GeForce RTX 40 Series (Ada Lovelace architecture) and earlier models.
- Supports CUDA programming but may have limited double-precision floating-point performance compared to professional GPUs.
2.	**Professional** GPUs (Quadro and RTX A Series):
- Built for 3D rendering, CAD, and engineering simulations.
- Examples: NVIDIA RTX A6000.
- Provides higher memory capacity and precision than gaming GPUs, ideal for professional workloads.
3.	**Data Center** GPUs (Tesla, A100, H100):
- Designed for high-performance computing (HPC), AI training, and deep learning.
- Examples: NVIDIA A100 (Ampere architecture), H100 (Hopper architecture).
- Features include high memory bandwidth, tensor cores for AI computations, and optimized power consumption.

### Compute Capability?

Compute Capability (CC) is a **version number** assigned to each NVIDIA GPU that specifies its hardware features and CUDA capabilities. It informs developers about:
- The architectural generation of the GPU.
- Supported CUDA features and functions.
- Hardware specifications, such as the number of registers, shared memory, and support for advanced instructions.

Key Components of Compute Capability:
- **Major version**: Indicates the architectural generation (e.g., 7 for Volta, 8 for Ampere).
- **Minor version**: Indicates incremental improvements within the same architecture.

> Common Compute Capabilities and feature.:
> - 3.x: **Kepler** -- Basic CUDA features, fewer cores.
> - 5.x: **Maxwell** -- Improved energy efficiency.
> - 6.x: **Pascal** -- Unified memory, better shared memory.
> - 7.x: **Volta** -- Tensor cores for AI acceleration.
> - 8.x: **Ampere** -- Improved tensor cores, multi-instance GPU.
> - 9.x: **Hopper** -- Dynamic programming instructions, large-scale AI.

### How Compute Capability Affects Development

1.	Determining Hardware Features:
- GPUs with higher compute capability support more advanced features, such as tensor cores for deep learning, warp matrix operations, and larger shared memory.
2.	Kernel Optimization:
- The number of threads, registers, and shared memory available per block can vary depending on the compute capability.
3.	Backward Compatibility:
- CUDA applications compiled for a lower compute capability GPU can run on higher compute capability GPUs, but the reverse is not true.
4. Checking Your GPU’s Compute Capability:
- Use the CUDA deviceQuery sample program:
```
cd /usr/local/cuda/samples/1_Utilities/deviceQuery
make
./deviceQuery
```
This will display the compute capability of your GPU.
