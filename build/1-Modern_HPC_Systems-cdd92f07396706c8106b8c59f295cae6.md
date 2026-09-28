# Modern HPC systems and the programming landscape

A **High-Performance Computing (HPC)** machine is usually a **cluster**: many separate computers (“nodes”) connected by a fast network. Each node is itself a parallel system (many CPU cores, often plus GPUs). To run fast, real HPC software typically exploits **multiple layers of parallelism** at once:

1. **Across nodes (distributed memory):** *MPI*
2. **Within a node (shared memory):** *OpenMP* (often combined with MPI)
3. **Within each CPU core:** *SIMD* (vector instructions)
4. **On accelerators (GPUs):** massively parallel GPU kernels (e.g., CUDA/HIP) + careful data layout

This structure is why your chapter naturally splits into **(i) MPI/OpenMP**, **(ii) CUDA**, **(iii) SIMD optimisation**.

---

## Two memory models: shared memory vs message passing

### Shared memory: OpenMP

**OpenMP** stands for **Open Multi-Processing**. It is an API for *shared-memory* parallel programming: a single process spawns multiple **threads** that share the same address space (same heap, same arrays). OpenMP is expressed mainly via compiler directives (`#pragma`) plus a runtime library. ([OpenMP][1])

Practical picture:

* you have one array `a[]` in memory
* you split loop iterations across threads
* threads must avoid writing the same location unless synchronized

```c
#pragma omp parallel for
for (int i = 0; i < N; i++) {
    out[i] = f(a[i]);
}
```

### Message passing: MPI

**MPI** stands for **Message Passing Interface**. It is a *process-based* model: you run multiple independent **processes** (called “ranks”), each with its own memory. Ranks exchange data explicitly using send/receive and collective operations (broadcast, reductions, etc.). ([MPI Forum][2])

Practical picture:

* each MPI rank owns a subdomain (a chunk of the data)
* ranks exchange boundary (“halo”) data or do global reductions (`MPI_Allreduce`, etc.)
* MPI scales to many nodes because it matches distributed memory

> [!NOTE]
> MPI is not “only for inter-node”. MPI ranks are often used **within a node** as well (e.g., one rank per socket or NUMA region). Many MPI libraries use shared-memory transports internally when ranks are on the same node, and for some codes this is simpler and faster than a pure shared-memory approach. ([Wikipedia][3])

### The common real-world pattern: hybrid MPI + OpenMP

A very typical HPC layout is:

* **MPI between nodes** (and often between sockets/NUMA domains)
* **OpenMP threads inside each MPI rank** (within a socket or NUMA region)

This hybrid approach is common because it balances:

* fewer MPI ranks (less communication overhead / fewer halos)
* many threads per rank (good core utilisation)

---

## Why SIMD matters even if you use OpenMP

Threading spreads work across cores, but each core still executes instructions. To approach peak performance you also need **vectorisation** (SIMD: Single Instruction, Multiple Data). OpenMP even has a directive to encourage this:

```c
#pragma omp simd
for (int i = 0; i < N; i++) out[i] = a[i] + b[i];
```

That directive exists specifically to express “multiple loop iterations can be executed concurrently using SIMD instructions.”

---

## What a GPU is, in practical terms

A modern HPC **GPU (Graphics Processing Unit)** is built to run **very large numbers of lightweight threads** concurrently. Compared to CPUs, GPUs are optimized for:

* **throughput** (many operations in flight)
* **high bandwidth** to GPU memory (HBM on many accelerators)
* hiding memory latency by switching between thousands of threads

### Offloading: CPU (“host”) vs GPU (“device”)

In GPU programming, the CPU typically:

* allocates and prepares data,
* **launches** a GPU kernel,
* optionally transfers results back.

This is the core “offload” idea: compute-intensive kernels run on the GPU device, while the CPU coordinates. (CUDA is the canonical example of this host/device model.)

### Why data layout often needs to change

To exploit thousands of GPU threads, you want adjacent threads to read/write adjacent memory locations. A classic example:

**Array of Structures (AoS)** (often awkward for coalesced access):

```c
struct P { double x,y,z; };
P p[N];
```

**Structure of Arrays (SoA)** (often better for GPUs *and* CPU SIMD):

```c
double x[N], y[N], z[N];
```

With SoA, thread `i` reads `x[i]` and neighbouring threads read `x[i+1]`, `x[i+2]`, … which matches what GPU memory systems are optimized for.

---

## GPU programming options you will meet

* **CUDA** = *Compute Unified Device Architecture* (NVIDIA-specific, dominant ecosystem)
* **HIP** = *Heterogeneous-Compute Interface for Portability* (AMD ROCm ecosystem; CUDA-like kernels) ([Calculator Hub][4])
* **OpenACC** = *Open Accelerators* (directive-based GPU offload)
* **OpenMP target offload** (OpenMP extended to accelerators)
* Portability layers you may see in HPC codes: **SYCL**, **Kokkos**, **RAJA** (one codebase, multiple backends; maturity depends on compiler/backend)

For beginners: directives (OpenMP/OpenACC) are often the easiest first exposure; CUDA/HIP gives the most control when you need to tune kernels.

---

## Typical scientific workloads (where these paradigms show up)

You’ll see MPI/OpenMP + SIMD + GPUs across many domains:

* **Molecular dynamics (MD):** short-range forces, neighbour lists, PME/FFTs
* **Computational fluid dynamics (CFD):** stencils, flux computations, Riemann solvers

  * including **Lattice Boltzmann (LB)** methods (regular memory access, good for SIMD/GPU)
* **PDE solvers:** sparse linear algebra, multigrid, Krylov methods (MPI collectives + bandwidth-bound kernels)
* **Artificial neural networks (ANNs):** GPU/accelerator-heavy (dense linear algebra / tensor cores), with distributed training using MPI-like collectives

This is why “HPC programming” is not one tool: the same application often uses several.

---

## The memory and network hierarchy: orders of magnitude

Performance is often limited by **moving data** (not by arithmetic). The key levels are:

### Inside one CPU core: registers and caches

Typical (representative) server-core numbers:

* **L1 data cache size:** often ~32 KiB per core (64-byte lines)
  L1 can sustain extremely high bandwidth; on modern Intel cores it can be close to “two 32-byte loads + one 32-byte store per cycle” under ideal conditions. ([Intel Community][5])
* **L2 cache size:** often a few hundred KiB per core; e.g. Skylake-class designs commonly report 256 KiB/core (client) and larger per core on server variants; L2↔L1 bandwidth is often quoted around **64 bytes/cycle** on Skylake-family designs. ([en.wikichip.org][6])
* **L3 cache:** shared across cores (size varies widely by CPU; often many MiB). It is slower than L2 but still far faster than DRAM.

> [!TIP]
> Treat these as *order-of-magnitude* calibration, not constants. Exact bandwidth depends on microarchitecture, access pattern (streaming vs random), and load/store mix. ([Intel Community][5])

### Between nodes: interconnect bandwidth (InfiniBand vs Ethernet)

Network bandwidth is orders of magnitude below on-chip movement:

* **Gigabit Ethernet:** 1 Gbit/s = **125 MB/s** (idealized line-rate conversion). ([Wikipedia][7])
* **InfiniBand HDR:** commonly **200 Gbit/s** class links in HPC. ([NVIDIA][8])
* **InfiniBand NDR:** **400 Gbit/s** class links (newer generation). ([NVIDIA][9])

This gap is the reason MPI communication patterns (halo exchange frequency, message sizes, collective choice, overlap) dominate scaling.


[1]: https://www.openmp.org/?utm_source=chatgpt.com "Home - OpenMP"
[2]: https://www.mpi-forum.org/docs/mpi-4.1/mpi41-report.pdf?utm_source=chatgpt.com "MPI: A Message-Passing Interface Standard"
[3]: https://en.wikipedia.org/wiki/Message_Passing_Interface?utm_source=chatgpt.com "Message Passing Interface"
[4]: https://calculatorhub.app/gigabit-to-megabyte-per-second/?utm_source=chatgpt.com "Gbps to MB/s Converter - Calculator Hub"
[5]: https://community.intel.com/t5/Software-Tuning-Performance/How-to-Interpret-Latencies-in-Intel-64-and-IA-32-Architectures/m-p/1154591?utm_source=chatgpt.com "The bandwidth values are - Intel Community"
[6]: https://en.wikichip.org/wiki/intel/microarchitectures/skylake_%28server%29?utm_source=chatgpt.com "Skylake (server) - Microarchitectures - Intel - WikiChip"
[7]: https://en.wikipedia.org/wiki/Data-rate_units?utm_source=chatgpt.com "Data-rate units - Wikipedia"
[8]: https://network.nvidia.com/files/doc-2020/wp-introducing-200g-hdr-infiniband-solutions.pdf?utm_source=chatgpt.com "Introducing 200G HDR InfiniBand Solutions - NVIDIA"
[9]: https://www.nvidia.com/content/dam/en-zz/Solutions/networking/ndr-technology/pdf/br-ndr-architecture-brochure.pdf?utm_source=chatgpt.com "NVIDIA MELLANOX INFINIBAND"

