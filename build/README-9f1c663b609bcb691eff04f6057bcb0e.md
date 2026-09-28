# DAJE

To build the book, install [jupyter book](https://jupyterbook.org), then launch
```bash
> jupyter book start
```

## TODOS

###  Marcello
1. FFT / spectra
2. System programming
3. Lecture SIMD

### Fabio

```{tableofcontents}


## synopsis


1. [Introduction](src/Introduction.md)
   1.  Scope of this resource
1. [Fundamentals]
   1. [Introduction to UNIX](src/Fundamentals/Lecture_Introduction_to_UNIX/main.md)
        1. [Unix and Linux operating systems](src/Fundamentals/Lecture_Introduction_to_UNIX/Unix_and_Linux_operating_systems.md)
        1. TODO  unix subsystem in Windows 10/11 + other options (VM, installing linux,...) 
        1. [The Shell and Basic Unix Commands](src/Fundamentals/Lecture_Introduction_to_UNIX/Shell_and_basic_Unix_commands.md)
        1. [Bash Job Control System aka Multitasking](src/Fundamentals/Lecture_Introduction_to_UNIX/Bash_Job_Control_System_aka_Multitasking.md)
        1. [x] Bash, commands (programs vs scripts)
        1. [x] ssh, scp  (keys)
        1. [x] ls, moving around filsystem
        1. [ ]  the unix filesystem structure: owner, group, permission, root user, links (hard/soft), ownership, concept of character devices
        1. [x] PATH, other environment variables (syntax ./, relative vs absolute paths)
        1. [x] Text manipulation: sed, awk, etc. (basic)
        1. [x] sed (advanced)
        1. [x] awk (advanced)
        1. [x] regex (advanced)
        1. [The Kernel](src/Fundamentals/Lecture_Introduction_to_UNIX/kernel_and_system_calls.md)
        1. [A Short Introduction to VIM](src/Fundamentals/Lecture_Practical_UNIX/vi_advanced.md)
        1. [The language of the CPU](src/Fundamentals/Lecture_Practical_UNIX/compilers_BASIC.md)
   1. [building code](src/Fundamentals/Lecture_building_code/main.md)
        1. [Compiling C Code on Linux/Unix Systems: A Practical Introduction](src/Fundamentals/Lecture_building_code/Introduction.md)
        1. compiling, linking, ideas of libraries (system and not)
        1. difference between executables and object code
        1. Hello world
        1. disassembling object code (part I)
        1. how does the execution of code works in a modern CPU/OS
   1. [the C programming language basics](src/Fundamentals/Lecture_the_C_programming_language_basics/main.md)
        1. [Pointers](src/Fundamentals/Lecture_the_C_programming_language_basics/C_pointers.md)
        1. [Arrays](src/Fundamentals/Lecture_the_C_programming_language_basics/C_arrays.md)
        1. [C and strings, a good excuse to discus the memory layout](src/Fundamentals/Lecture_the_C_programming_language_basics/C_strings_and_memory_layout.md)
        1. [summary](src/Fundamentals/Lecture_the_C_programming_language_basics/summary.md)
        1. [C BASIC](src/Fundamentals/Lecture_the_C_programming_language_basics/C_BASIC.md)
        1. [The C programming language: basics](src/Fundamentals/Lecture_the_C_programming_language_basics/Introduction.md)
        1. [C passing arguments](src/Fundamentals/Lecture_the_C_programming_language_basics/C_passing_arguments.md)
        1. Heap, stack, data. Static & co, scope.
        1. [Arrays and Pointers](src/Fundamentals/Lecture_the_C_programming_language_basics/C_pointers_and_arrays.md)
        1. performances of dynamic allocation, when to deallocate, when not
        1. Other reserved words in C, C standards
   1. [best practice](src/Fundamentals/Lecture_best_practice/main.md)
        1. [No programmer is an island](src/Fundamentals/Lecture_best_practice/Introduction.md)
        1. commenting code
        1. naming conventions (files, funtions, variables)
        1. indentation convention (using linter)
        1. versioning: git (basic), gitlab, github
        1. best and worst practices
   1. [the Fortran programming language basics](src/Fundamentals/Lecture_the_Fortran_programming_language_basics/main.md)
        1. [Fortran allocatable BASIC](src/Fundamentals/Lecture_the_Fortran_programming_language_basics/Fortran_allocatable_BASIC.md)
        1. [Fortran BASIC](src/Fundamentals/Lecture_the_Fortran_programming_language_basics/Fortran_BASIC.md)
        1. [Multidimensional arrays](src/Fundamentals/Lecture_the_Fortran_programming_language_basics/Fortran_arrays_BASIC.md)
   1. [Exercises in C and Fortran](src/Fundamentals/Lecture_Exercises_in_C_and_Fortran/main.md)
        1. [Exercises related to C_BASIC](src/Fundamentals/Lecture_Exercises_in_C_and_Fortran/Exercises_BASIC.md)
        1. [Gaussian quadrature](src/Fundamentals/Lecture_Exercises_in_C_and_Fortran/Exercise_Gaussian_quadrature.md)
        1. [Exercises related to arrays and pointers](src/Fundamentals/Lecture_Exercises_in_C_and_Fortran/Exercises_arrays_pointers.md)
   1. [the C programming language lib struc io](src/Fundamentals/Lecture_the_C_programming_language_lib_struc_io/main.md)
        1. Libraries (different functions for numerical integration) & headers
        1. Excursus: important system libraries and headers [`math.h`, `stdio.h`, `stdlib.h`] and associated function calls
        1. [FILE](src/Fundamentals/Lecture_the_C_programming_language_lib_struc_io/example.md)
        1. [Structures](src/Fundamentals/Lecture_the_C_programming_language_lib_struc_io/types.md)
        1. user-defined types: struct for complex numbers, struct mixed types (int, float, char) with a meaningful example
        1. FILE structure [system headers]
        1. Open a file for reading & writing, fopen, fscanf, fprint
        1. Binary I/O, Example with ASCII, example full binary
        1. Hexdump, binary, hex, oct representation
        1. Example: loading data from file
   1. [the Fortran programming language types io](src/Fundamentals/Lecture_the_Fortran_programming_language_types_io/main.md)
        1. [FILE](src/Fundamentals/Lecture_the_Fortran_programming_language_types_io/example.md)
   1. [Number Representation](src/Fundamentals/Lecture_Number_Representation/main.md)
        1. [How Numbers Are Represented in a Modern CPU (srcand Why You Should Care)](Fundamentals/Lecture_Number_Representation/Introduction.md)
   1. [Linear Algebra](src/Fundamentals/Lecture_Linear_Algebra/main.md)
        1. [Readme](src/Fundamentals/Lecture_Linear_Algebra/Readme.md)
   1. [LU decomposition](src/Fundamentals/Lecture_LU_decomposition/main.md)
        1. [Forward substitution: an example](src/Fundamentals/Lecture_LU_decomposition/forward_example.md)
        1. [LU decomposition for a 3x3 matrix](src/Fundamentals/Lecture_LU_decomposition/example_LU.md)
        1. [Back Substitution: an example](src/Fundamentals/Lecture_LU_decomposition/backward_example.md)
        1. [LU Decomposition](src/Fundamentals/Lecture_LU_decomposition/Introduction.md)
        1. [Extraction of spectral densities from lattice correlators](src/Fundamentals/Lecture_LU_decomposition/spectral_density.md)     
   1. BLAS
        1. basic blas usage
        2. openblas, mkl
   1. [Lapack](src/Fundamentals/Lecture_Lapack/main.md)
        1. [LAPACK](src/Fundamentals/Lecture_Lapack/Introduction.md)
   1. [roundoff](src/Fundamentals/Lecture_roundoff/main.md)
        1. [Handling Roundoff Errors in Scientific Computing](src/Fundamentals/Lecture_roundoff/Introduction.md)
   1. [FFT](src/Fundamentals/Lecture_FFT/main.md)
        1. [example](src/Fundamentals/Lecture_FFT/example.md)
        1. [Fast Fourier Transform (srcFFT)](Fundamentals/Lecture_FFT/Introduction.md)


1. [Unix System Programming]
    1.  Brief history of computers  TODO
    1.  Today's dominant architectures (x86-64, CUDA, ARM, TPU) TODO
    1.  What is an operating system and what does it do 
    1.  Different levels of programming/representation/file formats: binary (endians), hex, ascii, html/xml, fortran/c (compiled), python (interpreted)
    1.  Shells: power shell, bash, ...
    1.  client/server applications, LAN, internet, www.
    1.  Finite representations in OSs
    1.  UNIX syscalls
    1.  FS basics w/ historical example ext2 (inodes, ...) 
    1.  read()/write()
    1.  fork()
    1.  IPC: pipelines, semaphores
    1.  Example: a simple shell
    1.  Process scheduling (Linux's O(1) scheduler)
    1.  memory management
    1.  anatomy of an ELF executable
    1.  stripping down an executable to the bare minimum


1. [HPC](src/HPC/main.md)
    1. [Compiler Optimizations](src/HPC/Compiler_Optimizations/main.md)
    1. [SIMD](src/HPC/Lecture_SIMD/main.md)
    1. [MPI OpenMP](src/HPC/Lecture_MPI_OpenMP/main.md)
        1. [Message Passing Interface (srcMPI)](HPC/Lecture_MPI_OpenMP/MPI.md)
        1. [Introduction to Multi-CPU Parallelization](src/HPC/Lecture_MPI_OpenMP/Introduction.md)
        1. [Cartesian Topologies in MPI](src/HPC/Lecture_MPI_OpenMP/MPI_Cartesian.md)
        1. [Strong scaling and weak scaling  in Parallel Computing](src/HPC/Lecture_MPI_OpenMP/performances_strong_weak_scaling.md)
        1. [Essential MPI Functions Beyond the Basics](src/HPC/Lecture_MPI_OpenMP/MPI_other_functions.md)
        1. [Point-to-point communications](src/HPC/Lecture_MPI_OpenMP/MPI_SndRcv.md)
        1. [OpenMP](src/HPC/Lecture_MPI_OpenMP/OpenMP.md)
    1. [CUDA](src/HPC/Lecture_CUDA/main.md)
        1. [CUDA](src/HPC/Lecture_CUDA/CUDA.md)
        1. [query GPU prop](src/HPC/Lecture_CUDA/query_GPU_prop.md)
        1. [GPU Computing](src/HPC/Lecture_CUDA/Introduction.md)
        1. [Define the values of N to iterate over](src/HPC/Lecture_CUDA/scaling.md)
    1. [Anatomy of a CPU](src/HPC/Lecture_Anatomy_of_a_CPU/main.md)
        1. [x86 history](src/HPC/Lecture_Anatomy_of_a_CPU/x86_history.md)
        1. [Basics of x86-64 assembly language](src/HPC/Lecture_Anatomy_of_a_CPU/x86_assembly.md)
    1. Memory issues and best practices
        1. Different caches, speed
        1. Don't waste memory, think carefully (with examples)
            1. cache misses, cache profiling
        1. Memory leaks
        1. Valgrind/cachegrind
    1. [Performance in basic linear algebra](src/HPC/Lecture_Performance_in_basic_linear_algebra/main.md)
        1. time.h and clock()
        1. column major vs row major
        1. data layout (!) ref to cache misses, pipelines etc
        1. [Effect of optimisation](src/HPC/Lecture_Performance_in_basic_linear_algebra/effect_of_optimisation.md)
           1. disassembling (part III)
           2. pipelines (part II)
           3. cache: size, misses
           4. SIMD
        1. profiling / gprof
        2. compute-bound, memory-bound code, rooftop model
           1. LIKWID
   1. HPC clusters, major schedulers (with examples on choosing optimal number of nodes/cores - gromacs), chain jobs
   1. modern compilers and automatic parallelism
   1. OPP?  Fortran-2008, C++, python
   1. Examples of codes: bad/good/optimized
       1. linear algebra
       1. PDEs
       1. MD
       1. LB
1. [Tools](src/Tools/main.md)
    1. [Building Code](src/Tools/Lecture_Building_Code/main.md)
        1. [make](src/Tools/Lecture_Building_Code/make.md)
        1. [CMake](src/Tools/Lecture_Building_Code/CMake.md)
    1. [Debuggers and typical problems that require them](src/Tools/Lecture_Debuggers_and_typical_problems_that_require_them/main.md)
        1. [GDB](src/Tools/Lecture_Debuggers_and_typical_problems_that_require_them/gdb.md)
        1. examples of Heisenbugs, stack smashing
        1. examples of issues with branch prediction / introdction to pipelines
        1. the NULL pointer
    1. Cache profilers (valgrind), LIKWID: here or in the sections above ?
    1. [Git for version control](src/Tools/Lecture_Git_for_version_control/main.md)
        1. [basics](src/Tools/Lecture_Git_for_version_control/basics.md)
        1. [Branching](src/Tools/Lecture_Git_for_version_control/branching.md)
        1. [Git 1. Version Control System](src/Tools/Lecture_Git_for_version_control/Introduction.md)
        1. [Collaboration and testing](src/Tools/Lecture_Git_for_version_control/remote.md)
   
1. Bucketlist
       1. urban legends
       2. output redirections
       1. roundoff errors
       1. adding up numbers
       1. working with integers / exact representation
       1. 64/32/16/8 precision
       1. IEEE precision
       1. precision of math libs
       1. which precision do we need?
       1. simple cases where $N^2$ becomes $N\times M$ (neighbor lists)
       1. divide et impera: sorting, clustering
       1. qsort is not stable!
       1. linked lists, hash tables, and when to use them
       1. Bloom filters
       2. chmod
       3. Makefile (also Cmake?)
       4. GIT
           
