# LU Decomposition
## Introduction
A generic linear system looks like this:

$$
\begin{align*}
a_{11}x_1 + a_{12}x_2 + a_{13}x_3 + \cdots + a_{1N}x_N &= b_1 \\
a_{21}x_1 + a_{22}x_2 + a_{23}x_3 + \cdots + a_{2N}x_N &= b_2 \\
a_{31}x_1 + a_{32}x_2 + a_{33}x_3 + \cdots + a_{3N}x_N &= b_3 \\
\vdots \hspace{3cm} &\vdots \\
a_{M1}x_1 + a_{M2}x_2 + a_{M3}x_3 + \cdots + a_{MN}x_N &= b_M
\end{align*}
$$

whose short (matrix) form is

$$ A_{ij}x_j = b_i $$

where $A$ is a 2D matrix with $M$ rows and $N$ columns whose values are known, $x$ are the $N$ unknowns and the r.h.s. is a $M$-dimensional vector. 

## LU Decomposition

When solving systems of linear equations, one powerful method is to decompose the matrix **A** into a product of two matrices: a lower triangular matrix **L** and an upper triangular matrix **U**:

$$
L \cdot U = A \tag{1}
$$

where:
- **L** is the lower triangular matrix, meaning it has non-zero elements only on the diagonal and below.
- **U** is the upper triangular matrix, with non-zero elements only on the diagonal and above.

This decomposition allows us to break the problem into simpler parts.

> For example, for a 4x4 matrix **A**, the decomposition in Eq. (1) would look like this:
> 
> $$
> \begin{bmatrix}
> \alpha_{11} & 0 & 0 & 0 \\
> \alpha_{21} & \alpha_{22} & 0 & 0 \\
> \alpha_{31} & \alpha_{32} & \alpha_{33} & 0 \\
> \alpha_{41} & \alpha_{42} & \alpha_{43} & \alpha_{44}
> \end{bmatrix}
> \cdot
> \begin{bmatrix}
> \beta_{11} & \beta_{12} & \beta_{13} & \beta_{14} \\
> 0 & \beta_{22} & \beta_{23} & \beta_{24} \\
> 0 & 0 & \beta_{33} & \beta_{34} \\
> 0 & 0 & 0 & \beta_{44}
> \end{bmatrix} =
> \begin{bmatrix}
> a_{11} & a_{12} & a_{13} & a_{14} \\
> a_{21} & a_{22} & a_{23} & a_{24} \\
> a_{31} & a_{32} & a_{33} & a_{34} \\
> a_{41} & a_{42} & a_{43} & a_{44}
> \end{bmatrix} 
> $$

With this decomposition, we can solve the linear system:

$$
A \cdot x = (L \cdot U) \cdot x = L \cdot (U \cdot x) = b
$$

First, we solve for **y** using forward substitution:

$$
L \cdot y = b
$$

Then, we solve for **x** by backsubstitution:

$$
U \cdot x = y
$$

> [!NOTE]
> ### Why Decompose?
> The advantage of this approach is that triangular systems (like **L** and **U**) are easier to solve.
> 
> In a triangular system:
>   - **L** (lower triangular) only **has non-zero elements** on the diagonal and below. This makes it possible to solve for the unknowns sequentially using a method called forward substitution, starting from the first row and moving downwards.
>   - **U** (upper triangular) **has non-zero elements** on the diagonal and above. For U, you use a method called backsubstitution, which solves the system from the last row upwards.
>
  >These two methods —- **forward substitution** and **back substitution** -— are computationally efficient. Instead of having to deal with the full matrix at once, you solve the system in steps, reducing the overall complexity. This stepwise approach results in fewer calculations compared to methods like Gaussian elimination, making it faster and less prone to numerical errors for large systems.

### Forward Substitution
The forward substitution for solving **L** is:

$$
y_1 = \frac{b_1}{\alpha_{11}}
$$

For the remaining elements of **y**:

$$
y_i = \frac{1}{\alpha_{ii}} \left( b_i - \sum_{j=1}^{i-1} \alpha_{ij} y_j \right) \quad i = 2, 3, \dots, N
$$

[Click here to see an example for a 4x4 case](forward_example.md)

### Back Substitution
Once **y** is found, we can use backsubstitution to solve **U** for **x**:

$$
x_N = \frac{y_N}{\beta_{NN}} \tag{2.3.7}
$$

For the remaining elements of **x**:

$$
x_i = \frac{1}{\beta_{ii}} \left( y_i - \sum_{j=i+1}^{N} \beta_{ij} x_j \right) \quad i = N - 1, N - 2, \dots, 1
$$

[Click here to see an example for a 4x4 case](backward_example.md)

By decomposing the matrix **A** into **L** and **U**, we can solve the system of equations step-by-step, first with forward substitution and then with backsubstitution, making the process more efficient.

## Performing the LU decomposition
In the previous section, we saw that _once_ we have the lower and upper triangular matrices ($L$ and $U$, respectively) such that $A = L\cdot U$, we can easily solve a linear system like $A\cdot x = b$. Now, we see how to compute $L$ and $U$, i.e., how to perform the LU decomposition.

Let's start writing:

$$
A = L\cdot U = 
\begin{bmatrix}
\alpha_{11} & 0 & 0 & 0 \\
\alpha_{21} & \alpha_{22} & 0 & 0 \\
\alpha_{31} & \alpha_{32} & \alpha_{33} & 0 \\
\alpha_{41} & \alpha_{42} & \alpha_{43} & \alpha_{44}
\end{bmatrix}
\cdot
\begin{bmatrix}
\beta_{11} & \beta_{12} & \beta_{13} & \beta_{14} \\
0 & \beta_{22} & \beta_{23} & \beta_{24} \\
0 & 0 & \beta_{33} & \beta_{34} \\
0 & 0 & 0 & \beta_{44}
\end{bmatrix} =
\begin{pmatrix}
\alpha_{11} \beta_{11} & \alpha_{11} \beta_{12} & \alpha_{11} \beta_{13} & \alpha_{11} \beta_{14} \\
\alpha_{21} \beta_{11} & \alpha_{21} \beta_{12} + \alpha_{22} \beta_{22} & \alpha_{21} \beta_{13} + \alpha_{22} \beta_{23} & \alpha_{21} \beta_{14} + \alpha_{22} \beta_{24} \\
\alpha_{31} \beta_{11} & \alpha_{31} \beta_{12} + \alpha_{32} \beta_{22} & \alpha_{31} \beta_{13} + \alpha_{32} \beta_{23} + \alpha_{33} \beta_{33} & \alpha_{31} \beta_{14} + \alpha_{32} \beta_{24} + \alpha_{33} \beta_{34} \\
\alpha_{41} \beta_{11} & \alpha_{41} \beta_{12} + \alpha_{42} \beta_{22} & \alpha_{41} \beta_{13} + \alpha_{42} \beta_{23} + \alpha_{43} \beta_{33} & \alpha_{41} \beta_{14} + \alpha_{42} \beta_{24} + \alpha_{43} \beta_{34} + \alpha_{44} \beta_{44}
\end{pmatrix}
$$

and the associated system of equations is:

$$
\begin{cases}
a_{11} = \alpha_{11} \beta_{11} \\
a_{12} = \alpha_{11} \beta_{12} \\
a_{13} = \alpha_{11} \beta_{13} \\
a_{14} = \alpha_{11} \beta_{14} \\
a_{21} = \alpha_{21} \beta_{11} \\
a_{22} = \alpha_{21} \beta_{12} + \alpha_{22} \beta_{22} \\
a_{23} = \alpha_{21} \beta_{13} + \alpha_{22} \beta_{23} \\
a_{24} = \alpha_{21} \beta_{14} + \alpha_{22} \beta_{24} \\
a_{31} = \alpha_{31} \beta_{11} \\
a_{32} = \alpha_{31} \beta_{12} + \alpha_{32} \beta_{22} \\
a_{33} = \alpha_{31} \beta_{13} + \alpha_{32} \beta_{23} + \alpha_{33} \beta_{33} \\
a_{34} = \alpha_{31} \beta_{14} + \alpha_{32} \beta_{24} + \alpha_{33} \beta_{34} \\
a_{41} = \alpha_{41} \beta_{11} \\
a_{42} = \alpha_{41} \beta_{12} + \alpha_{42} \beta_{22} \\
a_{43} = \alpha_{41} \beta_{13} + \alpha_{42} \beta_{23} + \alpha_{43} \beta_{33} \\
a_{44} = \alpha_{41} \beta_{14} + \alpha_{42} \beta_{24} + \alpha_{43} \beta_{34} + \alpha_{44} \beta_{44}
\end{cases}
$$

As you can see from this example, we have $N^2$ equations and $N^2+N$ unknowns. Since the number of unknowns is greater than the number of equations, we are invited to specify $N$ of the unknowns arbitrarily and then try to solve for the others. We therefore choose:

$$\alpha_{ii} = 1\qquad\qquad i=1,\dots,N$$

We now apply the following procedure: 


Crout’s algorithm provides a very efficient way to solve the system of equations generated by the matrix decomposition $L \cdot U = A$. The method tackles the set of $N^2 + N$ equations, which include both the elements of $L$ and $U$, by cleverly rearranging them so that fewer unknowns are solved at each step. This reordering allows us to sequentially solve for each $\alpha_{ij}$ and $\beta_{ij}$, as outlined below:

1.	First, we set the diagonal elements of $L$ to unity:

$$ \alpha_{ii} = 1 \quad \text{for} \ i = 1, 2, …, N $$
	
2. Then, for each $j = 1, 2, \dots N$, we follow these two steps:
   
  - Use the following equation to solve for each $\beta_{ij}$ where $i = 1, 2, \dots, j$ (in the below equation, the summation term is zero when $i=1$):

$$\beta_{ij} = a_{ij} - \sum_{k=1}^{i-1} \alpha_{ik} \beta_{kj}$$

  - Use the following equation to solve for $\alpha_{ij}$ where $i = j+1, j+2, \dots N$:

$$ \alpha_{ij} = \frac{1}{\beta_{jj}} \left( a_{ij} - \sum_{k=1}^{j-1} \alpha_{ik} \beta_{kj} \right) $$

As you proceed through these iterations, you will notice that each $\alpha_{ij}$ and $\beta_{ij}$ is determined by previously computed values, allowing for an “in-place” decomposition. This means that no extra storage is required for these values, as they overwrite the corresponding locations in the matrix.

The algorithm essentially fills in the matrix by columns from left to right and within each column, from top to bottom.

Thus, after completing the process, the resulting matrix looks like this:

$$
\begin{pmatrix}
\beta_{11} & \beta_{12} & \beta_{13} & \beta_{14} \\
\alpha_{21} & \beta_{22} & \beta_{23} & \beta_{24} \\
\alpha_{31} & \alpha_{32} & \beta_{33} & \beta_{34} \\
\alpha_{41} & \alpha_{42} & \alpha_{43} & \beta_{44}
\end{pmatrix}
$$

This combined matrix stores both the $\alpha$’s and $\beta$’s, effectively performing $LU$ decomposition in a highly efficient manner.

[Here an example of the LU decomposition](example_LU.md)

## Pivoting 

Pivoting is a technique used to improve the numerical stability of the factorization process: without it, LU decomposition can suffer from numerical instability, especially when small or zero elements are encountered on the diagonal.

Pivoting refers to **reordering the rows** (or sometimes columns) of a matrix during the decomposition to avoid dividing by small or zero pivot elements, which can lead to **large numerical errors**.

In the context of LU decomposition, **partial pivoting** (the most common form) means **swapping rows** to ensure that the _largest absolute value in each column becomes the pivot element_ (diagonal element of the matrix). This helps reduce round-off errors and increases the numerical stability of the factorization.

Without pivoting, if a small pivot element is encountered during the factorization process, division by that small value can amplify rounding errors, making the solution inaccurate. Pivoting minimizes this risk by ensuring that the pivot element is large enough to avoid such issues.

### Pivoting in the Crout Algorithm

In the Crout method, you decompose the matrix ￼ into:

$$ 
A = L \cdot U
$$

where $L$ has ones on the diagonal (in some variations), and $U$ is an upper triangular matrix.

#### Standard Crout Algorithm (Without Pivoting):

1. Initialize $L$ as a lower triangular matrix with arbitrary entries and $U$ as an upper triangular matrix.
2. For each column of the matrix $A$, compute the elements of $L$ and $U$ by solving systems of equations based on the known entries of $A$, $L$, and $U$.

However, this approach is prone to numerical issues if a small or zero pivot element appears during the decomposition.

#### Crout Algorithm with Partial Pivoting:

In Crout’s method with partial pivoting, the algorithm includes an additional step where the rows of the matrix are swapped to ensure that the pivot element (the diagonal element of $U$) is the largest element in its column. Here’s how the process works:

1. Initialize the LU Decomposition: Start with matrix $A$, and begin the process of decomposing it into $L\cdot U$.
2. At each step (for each column):
	- Before computing the current column of $L$ and $U$, search for the largest absolute value in the current column (from the diagonal element down) and swap rows so that the largest value becomes the pivot element.
3 Row Swapping:
	- When a row swap is performed, you must update both $L$ and $U$ to reflect the new row ordering. Typically, these row swaps are recorded in a permutation matrix $P$, and the decomposition becomes:

$$
P\cdot A = L\cdot U
$$
￼
where $P$ is the permutation matrix that tracks row swaps.
4. Compute the LU Factors:
	- After pivoting, compute the entries of the $L$ and $U$ matrices as usual. Ensure that the pivot element (diagonal element of $U$) is now large enough to avoid division by small numbers.

## Example of LU Decomposition with Pivoting:

Consider a matrix $A$:

$$
A = \begin{pmatrix}
0 & 2 & 3 \\
4 & 5 & 6 \\
7 & 8 & 9
\end{pmatrix}
$$

Without pivoting, you would start by dividing by 0, which is not possible. With partial pivoting:

1. Find the largest element in the first column (7 in this case).
2. Swap rows 1 and 3:

$$
A = \begin{pmatrix}
7 & 8 & 9 \\
4 & 5 & 6 \\
0 & 2 & 3
\end{pmatrix}
$$

4. Continue the decomposition as normal, ensuring that each diagonal element is large enough to avoid numerical issues.

# Matrix Inverse with LU Decomposition

LU decomposition is useful for solving a series of $Ax = b$ problems with the same $A$ matrix and different $b$ matrices. This is advantageous for computing the inverse of $A$, as only one decomposition is required.

The inverse of a matrix $A^{-1}$ is defined such that:

$$
AA^{-1} = I
$$

where \(I\) is the identity matrix. 

For a general 3x3 matrix, this looks like:

$$
\begin{bmatrix}
A_{11} & A_{12} & A_{13} \\
A_{21} & A_{22} & A_{23} \\
A_{31} & A_{32} & A_{33}
\end{bmatrix}
\cdot
\begin{bmatrix}
a\prime_{11} & a\prime_{12} & a\prime_{13} \\
a\prime_{21} & a\prime_{22} & a\prime_{23} \\
a\prime_{31} & a\prime_{32} & a\prime_{33}
\end{bmatrix} =
\begin{bmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1
\end{bmatrix}
$$

This can be set up as ($n$) $Ax = b$-problems with different $b$ matrices, where the $x$ matrix becomes the $n$-th column in the inverse matrix. 

For example, the first problem is:

$$
\begin{bmatrix}
A_{11} & A_{12} & A_{13} \\
A_{21} & A_{22} & A_{23} \\
A_{31} & A_{32} & A_{33}
\end{bmatrix}
\cdot
\begin{bmatrix}
a\prime_{11} \\
a\prime_{21} \\
a\prime_{31}
\end{bmatrix} =
\begin{bmatrix}
1 \\
0 \\
0
\end{bmatrix}
$$

The second column of the inverse can be computed by changing $b$ to $[0, 1, 0]^T$, and the third column with $[0, 0, 1]^T$, and so on.

This method is efficient because only back- and forward-substitution is required after the initial LU decomposition.

> [!WARNING]
> To compute the inverse of the matrix, you need to solve ($n$) $Ax = b$-problems, one for each column of the inverse. If you're trying to compute the inverse just to solve an $Ax = b$ problem, you introduce more numerical error and slow down your computation time. **In short, avoid computing the matrix inverse unless absolutely necessary, and instead solve the system of equations directly.**

## Applications

Here a list of examples involving the matrix inversion:

1. [Extraction of spectral densities from lattice correlators](spectral_density.md)

