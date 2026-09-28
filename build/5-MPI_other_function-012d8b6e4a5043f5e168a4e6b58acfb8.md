# Essential MPI Functions Beyond the Basics

In addition to the fundamental MPI functions you already know, such as `MPI_Init`, `MPI_Finalize`, `MPI_Comm_rank`, `MPI_Comm_size`, `MPI_Send`, `MPI_Recv`, and `MPI_Reduce`, there are several other important MPI functions that provide greater flexibility and efficiency when designing parallel programs. These functions are especially useful for handling complex communication patterns, collective operations, and custom data handling.

This section is meant to just provide an overview of other existing MPI functions. 

## 1. **Non-blocking Communication**
Non-blocking communication functions allow processes to initiate a communication operation and then proceed with other work while the operation completes in the background. This can improve performance by overlapping communication and computation.

- **`MPI_Isend`**: Initiates a non-blocking send operation.
  ```c
  int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
  ```
- **`MPI_Irecv`**: Initiates a non-blocking receive operation.
  ```c
  int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
  ```
- **`MPI_Wait`**: Waits for a specific non-blocking operation to complete.
  ```c
  int MPI_Wait(MPI_Request *request, MPI_Status *status);
  ```
- **`MPI_Waitall`**: Waits for all specified non-blocking operations to complete.
  ```c
  int MPI_Waitall(int count, MPI_Request array_of_requests[], MPI_Status array_of_statuses[]);
  ```

## 2. **Broadcast and Scatter/Gather Operations**

These collective operations enable efficient communication of data among processes.

- **`MPI_Bcast`**: Broadcasts data from one process to all others.
  ```c
  int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
  ```
  Example: A root process sends an array to all processes in the communicator.

- **`MPI_Scatter`**: Distributes parts of an array from the root process to all processes.
  ```c
  int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
  ```

- **`MPI_Gather`**: Gathers parts of an array from all processes to the root process.
  ```c
  int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
  ```

- **`MPI_Allgather`**: Gathers data from all processes and distributes the results to all processes.
  ```c
  int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);
  ```
![plot](sketch_comm.png)

## 3. **Custom Datatypes**
MPI allows you to define custom data layouts to handle complex data structures or non-contiguous memory.

- **`MPI_Type_create_struct`**: Creates a custom datatype from multiple arrays of displacements, types, and block lengths.
  ```c
  int MPI_Type_create_struct(int count, const int array_of_blocklengths[], const MPI_Aint array_of_displacements[], const MPI_Datatype array_of_types[], MPI_Datatype *newtype);
  ```
- **`MPI_Type_commit`**: Commits the custom datatype for use in communication.
  ```c
  int MPI_Type_commit(MPI_Datatype *datatype);
  ```
- **`MPI_Type_free`**: Frees a previously defined custom datatype.
  ```c
  int MPI_Type_free(MPI_Datatype *datatype);
  ```

## 4. **Communicator Management**
Custom communicators allow you to divide processes into groups and define new communication contexts.

- **`MPI_Comm_split`**: Splits a communicator into disjoint sub-communicators based on color and key.
  ```c
  int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
  ```
  Example: Divide processes into groups based on their rank for separate communication.

- **`MPI_Comm_dup`**: Duplicates an existing communicator, creating a new one with the same group of processes.
  ```c
  int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm);
  ```

## 5. **Synchronization**
MPI provides functions for explicit synchronization among processes.

- **`MPI_Barrier`**: Blocks all processes in a communicator until all have reached this point.
  ```c
  int MPI_Barrier(MPI_Comm comm);
  ```

## 6. **Querying and Debugging**
You can query the environment or request information for debugging purposes.

- **`MPI_Wtime`**: Returns the current wall-clock time for timing measurements.
  ```c
  double MPI_Wtime(void);
  ```
  
### Summary Table

| **Function**        | **Purpose**                              |
|----------------------|------------------------------------------|
| `MPI_Isend`, `MPI_Irecv` | Non-blocking send and receive.         |
| `MPI_Bcast`, `MPI_Scatter`, `MPI_Gather` | Collective data distribution.       |
| `MPI_Type_create_struct` | Define custom data types.             |
| `MPI_Comm_split`, `MPI_Comm_dup` | Manage communicators.             |
| `MPI_Barrier`       | Synchronize processes.                   |
| `MPI_Wtime` | Timing and performance measurement.    |

These functions allow you to handle more complex parallelism scenarios, optimize performance, and manage data effectively across processes. Mastering them will give you a broader toolbox for tackling real-world computational problems.
