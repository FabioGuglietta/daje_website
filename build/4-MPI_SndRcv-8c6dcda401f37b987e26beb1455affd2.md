# Point-to-point communications

The equation governing heat diffusion (or thermal conduction) is known as the heat equation:

$$\frac{\partial T(x,y,z,t)}{\partial t} =\alpha \nabla^2 T(x,y,z,t)$$

where $\alpha$ is a positive coefficient called the thermal diffusivity, which depends on the material properties.

In one spatial dimension, the heat equation is given by:

$$\frac{\partial T(x, t)}{\partial t} = \alpha \frac{\partial^2 T(x, t)}{\partial x^2}$$


## Discretizing the Heat Equation

For numerical simulations, the heat equation is often discretized using **finite differences**. 
For example, using the explicit finite difference method, the time derivative can be approximated as:


$$\frac{\partial T}{\partial t} \approx \frac{T(x, t + \Delta t) - T(x, t)}{\Delta t}$$

and the second spatial derivative as:

$$\frac{\partial^2 T}{\partial x^2} \approx \frac{T(x + \Delta x, t) - 2T(x, t) + T(x - \Delta x, t)}{(\Delta x)^2}$$

Substituting these approximations into the heat equation gives a scheme to calculate  $T(x, t + \Delta t)$  from  $T(x, t)$:

$$T(x, t + \Delta t) = T(x, t) + \alpha \frac{\Delta t}{(\Delta x)^2} \left( T(x + \Delta x, t) - 2T(x, t) + T(x - \Delta x, t) \right)$$

## How to make it parallel with MPI

In parallelizing the heat diffusion equation, the computational domain is divided into **subdomains**, each handled by a separate process. This division allows each process to handle a smaller subset of the domain, **reducing the computational load per process** and enabling larger simulations to run more efficiently.

Let’s assume a one-dimensional domain with $N$ points divided into $n_p$ subdomains, where $n_p$ is the number of processes. Each process is responsible for updating the temperature values for a certain range of  $x$-values within the domain. For example:
- Process $0$ will handle points from  $x_0$  to  $x_{N/n_p-1}$.
- Process $1$ will handle points from  $x_{N/n_p}$  to  $x_{2N/n_p-1}$.
- And so on, until the last process handles  $x_{(n_p-1)N/np}$  to  $x_N$.

For example, if we have $N=64$ points and $n_p=4$ processes, each process will handle $n=N/n_p=16$ points. We will divide the domain as follow:
- Process $0$ will handle points in range $[0,n-1]=[0,N/n_p-1]=[0,15]$.
- Process $1$ will handle points in range $[n,2n-1]=[n,2N/n_p-1]=[16,31]$.
- Process $2$ will handle points in range $[2n,3n-1]=[2n,3N/n_p-1]=[32,47]$.
- Process $3$ will handle points in range $[3n,4n-1]=[3n,4N/n_p-1]=[48,63]$.

### Boundary Communication

In each time step, updating the temperature at a given point **requires information from neighboring points**. Specifically, points near the **boundaries of each process’s subdomain** need the latest temperature values **from the adjacent processes**. This is where boundary communication becomes necessary:
1.	**Exchange of Boundary Values**: Each process needs to **send its boundary data** (the temperature at its boundary points) to its neighboring processes. For instance:
  - Process  $i$  must send the temperature at its rightmost point to Process $i+1$  and receive the leftmost boundary value from Process $i-1$.
  - Similarly, it sends its leftmost boundary to Process  i-1  and receives the rightmost boundary value from Process $i+1$.
2.	**Using `MPI_Send` and `MPI_Recv`**: Communication can be implemented with `MPI_Send` and `MPI_Recv`:
  - _Non-blocking communication_ (e.g., using `MPI_Isend` and `MPI_Irecv`) can allow computation to overlap with communication, potentially improving performance.
  - Boundary update ensures that each process has the necessary information before updating the temperature values at boundary points.

## C code

Here the MPI C code:

```C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


// Initialize temperature 
void initialize(double *temperature, int local_n, int rank, int size) {
    // Initialize the temperature array (e.g., set an initial heat distribution)
    for (int i = 0; i < local_n; i++) {
        temperature[i] = 0.;
    }
    // Boundary conditions: high temperature at left edge
    if (rank == 0) temperature[0] = 1.0;
}

// Update temperature using the heat equation
void update_temperature(double *temp, double *new_temp, int local_n, double alpha, double dx, double dt, int rank, int size) {
    for (int i = 1; i < local_n-1; i++) {
      new_temp[i] = temp[i] + alpha*(dt / (dx * dx)) * (temp[i + 1] - 2 * temp[i] + temp[i - 1]);
    }
}

// Write data to a file for Gnuplot visualization
void write_results(int rank, int size, int local_n, double *temp, int step, int NX) {
    char filename[256];
    sprintf(filename, "heat_output_step_%d.dat", step);
    FILE *fp = fopen(filename, rank == 0 ? "w" : "a");
    //Write header
    if (rank == 0) fprintf(fp, "# Step %d\n", step);

    //Buffers used to send and recv data during REDUCE (used to print data)
    double *sendbuff = (double *)malloc(NX * sizeof(double));
    double *recvbuff = (double *)malloc(NX * sizeof(double));
    for (int i = 0; i < NX; i++){
      sendbuff[i]=0;
      recvbuff[i]=0;
    }

    for (int i = 1; i < local_n-1; i++) sendbuff[(local_n-2)*rank+(i-1)] = temp[i]; //We remove from local_n the boundaries (that are 2: left and right)

    //All processors reduce on root their local values
    MPI_Reduce(sendbuff, recvbuff, NX, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    //Only root prints
    if(rank==0){
      for (int i = 0; i < NX; i++) fprintf(fp, "%f\n", recvbuff[i]);
    }

    fclose(fp);
    free(sendbuff);
    free(recvbuff);
}

int main(int argc, char *argv[]) {
    double dx, dt = 0.001, alpha = 0.01;
    int NX = 100;         //Number of grid points
    int NT = 1e5;         //Number of time steps
    int time_print = 1e3; //Print frequency
    int ln, rn;           //Left and Right Neighbours ranks
    int rank, size;
    int local_n;
    double *temp, *new_temp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ln = rank-1;
    rn = rank+1;
    if(rank==0)      ln = rank;
    if(rank==size-1) rn = rank;

    dx = 1./((double)NX);
    local_n = NX / size + 2; //We add 2 beacuse, for each process, we add the "boundaries" at 0 and Nx/size+1
    temp = (double *)malloc(local_n * sizeof(double));
    new_temp = (double *)malloc(local_n * sizeof(double));

    // Initialize the temperature field
    initialize(temp, local_n, rank, size);

    // Time-stepping loop
    for (int t = 0; t < NT; t++) {
        // Exchange boundary points
        if(rank!=0)      MPI_Send(&temp[1],           1, MPI_DOUBLE, ln, 0, MPI_COMM_WORLD);
        if(rank!=size-1) MPI_Send(&temp[local_n - 2], 1, MPI_DOUBLE, rn, 1, MPI_COMM_WORLD);
        if(rank!=size-1) MPI_Recv(&temp[local_n - 1], 1, MPI_DOUBLE, rn, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(rank!=0)      MPI_Recv(&temp[0],           1, MPI_DOUBLE, ln, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Update temperature field
        update_temperature(temp, new_temp, local_n, alpha, dx, dt, rank, size);

        // Swap pointers
        double *swap = temp;
        temp = new_temp;
        new_temp = swap;

        // Write results for Gnuplot
        //if(t%time_print==0) write_results(rank, size, local_n, temp, t, NX);
    }

    free(temp);
    free(new_temp);
    MPI_Finalize();

    return 0;
}
```

## `MPI_Send`

The `MPI_Send` function is a core part of MPI that sends a message **from one process to another** within an MPI communicator. It performs a standard-mode **blocking** send (below there is a section explaining blocking vs. non-blocking operations), meaning it only returns once the send buffer is safe to modify, which may occur when the message is delivered. Here’s a breakdown of its syntax in both C and Fortran.

> The **signature in C** is:
> 
> ```C
> int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
> ```
> 
> - `buf`: Pointer to the data being sent.
> - `count`: Number of elements in the send buffer.
> - `datatype`: Data type of each element (e.g., `MPI_INT`, `MPI_DOUBLE`).
> - `dest`: Rank of the destination process within the communicator.
> - `tag`: Message tag to identify the message type, used in pairing sends and receives.
> - `comm`: Communicator (usually `MPI_COMM_WORLD`).


> The **signature in Fortran** is:
> 
> ```Fortran
> MPI_Send(buf, count, datatype, dest, tag, comm, ierror)
> <type>, dimension(*) :: buf
> integer :: count, datatype, dest, tag, comm, ierror
> ```
> - `buf`: Array containing data to be sent.
> - `count`: Number of elements in `buf`.
> - `datatype`: MPI data type of each element (e.g., `MPI_INTEGER`, `MPI_DOUBLE_PRECISION`).
> - `dest`: Destination process rank.
> - `tag`: Message tag identifier.
> - `comm`: Communicator.
> - `ierror`: Error code.


## `MPI_Recv`

The `MPI_Recv` function is the counterpart to `MPI_Send`, and it allows a process to receive a message from another process in a **blocking** fashion. This means that the receiving process will wait (block) until the requested message is received, and it can then proceed with execution. The message is stored in a buffer that is provided by the receiving process.

The matching between the send and receive operations is done using the message's `tag` and destination `rank`. The `tag` allows the program to distinguish between different types of messages.

> The **signature in C** is:
> 
> ```C
> int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
> ```
> 
> - `buf`: Pointer to the buffer where the received data will be stored.
> - `count`: The maximum number of elements to receive (this should match the number of elements sent by the sender).
> - `datatype`: Data type of each element (e.g., `MPI_INT`, `MPI_DOUBLE`).
> - `source`: The rank of the sending process, or `MPI_ANY_SOURCE` if the message can come from any process.
> - `tag`: The message tag used to identify the message, or `MPI_ANY_TAG` to accept messages with any tag.
> - `comm`: Communicator (usually `MPI_COMM_WORLD`).
> - `status`: A pointer to an `MPI_Status` structure, which is used to store information about the received message, such as the actual `source` and `tag` of the message.

> The **signature in Fortran** is:
> 
> ```Fortran
> MPI_Recv(buf, count, datatype, source, tag, comm, status, ierror)
> <type>, dimension(*) :: buf
> integer :: count, datatype, source, tag, comm, ierror
> integer, dimension(MPI_STATUS_SIZE) :: status
> ```
> - `buf`: The array where the received data will be stored.
> - `count`: The number of elements to be received.
> - `datatype`: MPI data type of each element (e.g., `MPI_INTEGER`, `MPI_DOUBLE_PRECISION`).
> - `source`: The rank of the sending process, or `MPI_ANY_SOURCE` if the message can come from any process.
> - `tag`: The message tag used to identify the message, or `MPI_ANY_TAG` to accept messages with any tag.
> - `comm`: Communicator (usually `MPI_COMM_WORLD`).
> - `status`: An array to hold the message status information (e.g., `source`, `tag`).
> - `ierror`: Error code.

## Blocking vs. Non-Blocking calls

In MPI, a **blocking** call refers to a function that causes the calling process to **wait until the operation completes** before it continues execution. This means the process will block, or be paused, until it receives a response or the data it is waiting for has been transmitted or received.

For example, when a process calls `MPI_Send` to send a message, it will block until the message is sent (or at least successfully enqueued for transmission, depending on the specific call).
Similarly, `MPI_Recv` is blocking: the receiver will block until the message is received, meaning it waits for the message to arrive.

Blocking calls can simplify program flow because the program doesn't need to handle **asynchronous** communication. However, blocking calls can also lead to inefficiencies, especially in parallel applications where multiple processes might be waiting on each other.
In contrast, **non-blocking** calls like `MPI_Isend` and `MPI_Irecv` allow processes to continue execution while the communication is being handled in the background, and completion must be explicitly checked (using `MPI_Wait`, for example).

Blocking calls are generally easier to program but can lead to performance bottlenecks, especially when processes have to wait for each other to send or receive messages.

### Example

Here is an example in which the **code gets blocked** due to **incorrect** use of `MPI_Send` and `MPI_Recv`. This happens when the sender and receiver are not properly matched, or if there is a mismatch between the expected number of elements or tags.

Suppose we have two processes. One process is sending data, and the other process is supposed to receive it. The issue occurs when the receiver is not correctly set up to receive the message, such as by using the wrong tag or not providing enough space in the receive buffer.

```C
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int send_data = 123;  // Data to send
    int recv_data;        // Buffer to receive data

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This example requires at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Process 0 sends data with tag 100
        printf("Process %d sending data: %d\n", rank, send_data);
        MPI_Send(&send_data, 1, MPI_INT, 1, 100, MPI_COMM_WORLD);
    } 
    else if (rank == 1) {
        // Process 1 expects data with tag 200 (incorrect tag)
        printf("Process %d waiting for data...\n", rank);
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Incorrect tag here
        printf("Process %d received data: %d\n", rank, recv_data);
    }

    MPI_Finalize();
    return 0;
}
```

To fix it, just change the tag to match the send and receive message.

## Visualisation

You might use the following Python script to create an animation showing the time evolution of the temperature field:

```Python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from IPython.display import HTML

# Parameters
NX = 100
NT = 99900
every = 1000

# Grid
x = np.linspace(0, NX - 1, NX)
y = np.ones(NX)

import matplotlib.colors as mcolors
# Figure
fig, ax = plt.subplots(figsize=(10, 2))

Tmin, Tmax = 0.0, 1.0  # scegli tu
norm = mcolors.Normalize(vmin=Tmin, vmax=Tmax)

scat = ax.scatter(x, y, c=np.zeros(NX), cmap="coolwarm", norm=norm, marker='s', s=100)
plt.colorbar(scat, ax=ax, orientation='horizontal', label='Temperature')

plt.xlim([0, NX])
plt.ylim([0.995, 1.005])
ax.set_title("1D Heat Diffusion")

# Update function
def update(frame):
    file = f'heat_output_step_{frame}.dat'

    try:
        T = np.loadtxt(file, skiprows=1)
        scat.set_array(T)  # just update colors
        ax.set_title(f"1D Heat Diffusion - Step {frame}")
    except Exception as e:
        print(f"Error loading file {file}: {e}")

    return scat,

# Animation
frames = range(0, NT, every)
ani = FuncAnimation(fig, update, frames=frames, blit=True)
plt.close(fig)

HTML(ani.to_jshtml())
```
