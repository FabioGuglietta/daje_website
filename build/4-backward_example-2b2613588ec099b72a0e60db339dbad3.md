# Back Substitution: an example

We are given an upper triangular matrix $U$ from the $LU$ decomposition and the vector $y$ from the (forward substitution)[forward_example.md] step:

$$
U\cdot x = y
$$

that, in our 4x4 example, becomes:

$$
\begin{bmatrix}
\beta_{11} & \beta_{12} & \beta_{13} & \beta_{14} \\
0 & \beta_{22} & \beta_{23} & \beta_{24} \\
0 & 0 & \beta_{33} & \beta_{34} \\
0 & 0 & 0 & \beta_{44}
\end{bmatrix}
\cdot
\begin{bmatrix}
x_1 \\
x_2 \\
x_3 \\
x_4 
\end{bmatrix} =
\begin{bmatrix}
y_1 \\
y_2 \\
y_3 \\
y_4 
\end{bmatrix} 
$$




Now we solve for the vector $x$ using back substitution. 

System to solve:

$$
\begin{aligned}
\beta_{11} x_1 + \beta_{12} x_2 + \beta_{13} x_3 + \beta_{14} x_4 &= y_1 \\
\beta_{22} x_2 + \beta_{23} x_3 + \beta_{24} x_4 &= y_2 \\
\beta_{33} x_3 + \beta_{34} x_4 &= y_3 \\
\beta_{44} x_4 &= y_4
\end{aligned}
$$

## Back Substitution Steps:

We will start from the last equation (the bottom row) and work our way upwards, solving for each $x_i$ one by one.

### 1. Solve for $x_4$ from the last equation:

$$
x_4 = \frac{y_4}{\beta_{44}}
$$

### 2. Solve for $x_3$ from the third equation:

$$
x_3 = \frac{1}{\beta_{33}} \left( y_3 - \beta_{34} x_4 \right)
$$

### 3. Solve for $x_2$ from the second equation:

$$
x_2 = \frac{1}{\beta_{22}} \left( y_2 - \beta_{23} x_3 - \beta_{24} x_4 \right)
$$

### 4.	Solve for $x_1$ from the first equation:

$$
x_1 = \frac{1}{\beta_{11}} \left( y_1 - \beta_{12} x_2 - \beta_{13} x_3 - \beta_{14} x_4 \right)
$$
