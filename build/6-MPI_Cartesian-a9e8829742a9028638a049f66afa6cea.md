# Cartesian Topologies in MPI

MPI provides a way to arrange processes in a Cartesian grid, which is especially useful for applications with structured grids, such as finite difference or finite volume solvers.

#### **Key Features**
- Define a Cartesian topology for processes in a communicator.
- Simplify neighbor communication by using relative shifts in grid dimensions.
- Enable wrap-around (periodic) boundaries for dimensions.

## **Main Functions**

### 1. **`MPI_Cart_create`: Create a Cartesian Topology**
This function creates a new communicator with a Cartesian process topology.

**Function Signature:**
```c
int MPI_Cart_create(MPI_Comm old_comm, int ndims, const int dims[], const int periods[], int reorder, MPI_Comm *new_comm);
```

**Parameters:**
- **`old_comm`**: Existing communicator (usually `MPI_COMM_WORLD`).
- **`ndims`**: Number of dimensions in the Cartesian grid.
- **`dims`**: Array specifying the number of processes in each dimension.
- **`periods`**: Array specifying whether each dimension is periodic (`1`) or not (`0`).
- **`reorder`**: Allows process ranks to be reordered (`1`) or not (`0`).
- **`new_comm`**: New communicator with the Cartesian topology.

**Example:**
```c
int dims[2] = {2, 2};          // 2x2 grid
int periods[2] = {1, 0};       // Periodic in one dimension
MPI_Comm cart_comm;
MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);
```

### 2. **`MPI_Cart_coords`: Get Coordinates of a Process**
This function retrieves the Cartesian coordinates of a process in the grid.

**Function Signature:**
```c
int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int coords[]);
```

**Parameters:**
- **`comm`**: Cartesian communicator.
- **`rank`**: Rank of the process in the communicator.
- **`maxdims`**: Maximum number of dimensions.
- **`coords`**: Array to store the coordinates.

**Example:**
```c
int coords[2];
MPI_Cart_coords(cart_comm, rank, 2, coords);
printf("Rank %d has coordinates (%d, %d)\n", rank, coords[0], coords[1]);
```

### 3. **`MPI_Cart_rank`: Get Rank from Coordinates**
This function computes the rank of a process given its Cartesian coordinates.

**Function Signature:**
```c
int MPI_Cart_rank(MPI_Comm comm, const int coords[], int *rank);
```

**Parameters:**
- **`comm`**: Cartesian communicator.
- **`coords`**: Cartesian coordinates of the process.
- **`rank`**: Rank of the process in the communicator.

**Example:**
```c
int rank;
int coords[2] = {1, 0};
MPI_Cart_rank(cart_comm, coords, &rank);
printf("Coordinates (1,0) correspond to rank %d\n", rank);
```

### 4. **`MPI_Cart_shift`: Determine Neighbors**
This function finds the ranks of neighboring processes in a specific dimension.

**Function Signature:**
```c
int MPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *source, int *dest);
```

**Parameters:**
- **`comm`**: Cartesian communicator.
- **`direction`**: Dimension in which to find neighbors.
- **`disp`**: Displacement (e.g., `1` for next neighbor).
- **`source`**: Rank of the source neighbor.
- **`dest`**: Rank of the destination neighbor.

**Example:**
```c
int source, dest;
MPI_Cart_shift(cart_comm, 0, 1, &source, &dest);
printf("Rank %d: source in dim 0 = %d, dest in dim 0 = %d\n", rank, source, dest);
```

## **Full Example: 2D Cartesian Grid**

```c
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[2] = {3, 2};
    MPI_Dims_create(size, 2, dims);  // Automatically calculate grid dimensions

    int periods[2] = {1, 0};  // Periodic in the first dimension
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);

    int coords[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);
    printf("Rank %d -> Coordinates (%d, %d)\n", rank, coords[0], coords[1]);

    int source, dest;
    MPI_Cart_shift(cart_comm, 0, 1, &source, &dest);
    printf("Rank %d -> Source = %d, Dest = %d\n", rank, source, dest);

    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}
```

---

### **Summary Table**

| **Function**         | **Purpose**                                      |
|-----------------------|--------------------------------------------------|
| `MPI_Cart_create`     | Creates a Cartesian topology communicator.       |
| `MPI_Cart_coords`     | Retrieves Cartesian coordinates of a rank.       |
| `MPI_Cart_rank`       | Converts coordinates to a rank.                 |
| `MPI_Cart_shift`      | Finds neighbors in a specific direction.         |
