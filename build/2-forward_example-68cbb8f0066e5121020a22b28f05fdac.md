# Forward substitution: an example

To provide an example of **forward substitution** for solving a lower triangular system like $$L \cdot y = b$$, let’s apply the forward substitution method to a specific 4x4 system.

We start with the lower triangular matrix $L$, the vector $b$, and the unknown vector $y$.
In this case, $$L \cdot y = b$$ becomes:

$$
\begin{pmatrix}
\alpha_{11} & 0          & 0         & 0 \\
\alpha_{21} & \alpha_{22} & 0         & 0 \\
\alpha_{31} & \alpha_{32} & \alpha_{33} & 0 \\
\alpha_{41} & \alpha_{42} & \alpha_{43} & \alpha_{44}
\end{pmatrix}
\cdot
\begin{pmatrix}
y_1 \\
y_2 \\
y_3 \\
y_4
\end{pmatrix}=
\begin{pmatrix}
b_1 \\
b_2 \\
b_3 \\
b_4
\end{pmatrix}
$$

We will now apply the **forward substitution steps** to each row (i.e., each equation):

### 1. First Equation $(i = 1)$:
The first equation is simple because only $y_1$ appears:

$$
\alpha_{11} y_1 = b_1
$$

$$
y_1 = \frac{b_1}{\alpha_{11}}
$$

### 2. Second Equation $(i = 2)$:
The second equation involves $y_1$ and $y_2$:

$$
\alpha_{21} y_1 + \alpha_{22} y_2 = b_2
$$

$$
y_2 = \frac{b_2 - \alpha_{21} y_1}{\alpha_{22} }
$$

And we can substitute $y_1$ from the first equation.

### Third Equation $(i = 3)$:

The third equation involves $y_1$, $y_2$, and $y_3$:

$$
\alpha_{31} y_1 + \alpha_{32} y_2 + \alpha_{33} y_3 = b_3
$$

$$
y_3 = \frac{b_3 - \alpha_{31} y_1 - \alpha_{32} y_2}{\alpha_{33}}
$$

And we can substitute $y_1$ and $y_2$ from previous equations.

### 4. Fourth Equation $(i = 4)$:

The fourth equation involves $y_1$, $y_2$, $y_3$, and $y_4$:

$$
\alpha_{41} y_1 + \alpha_{42} y_2 + \alpha_{43} y_3 + \alpha_{44} y_4 = b_4
$$


$$
y_4 = \frac{b_4 - \alpha_{41} y_1 - \alpha_{42} y_2 - \alpha_{43} y_3}{\alpha_{44}}
$$

And we can substitute $y_1$, $y_2$, and $y_3$ from the previous equations.

### Summary of the Solution:

We now have the solution for the vector $y$:

$$
y_1 = \frac{b_1}{\alpha_{11}}
$$

$$
y_2 = \frac{b_2 - \alpha_{21} y_1}{\alpha_{22}}
$$

$$
y_3 = \frac{b_3 - \alpha_{31} y_1 - \alpha_{32} y_2}{\alpha_{33}}
$$

$$
y_4 = \frac{b_4 - \alpha_{41} y_1 - \alpha_{42} y_2 - \alpha_{43} y_3}{\alpha_{44}}
$$

that can be generalised as:

$$
y_i = \frac{1}{\alpha_{ii}} \left( b_i - \sum_{j=1}^{i-1} \alpha_{ij} y_j \right) \quad i = 2, 3, \dots, N
$$
