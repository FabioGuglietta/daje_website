# LU decomposition for a 3x3 matrix

To perform LU decomposition of a 3x3 matrix step by step, we decompose matrix $A$  into a lower triangular matrix $L$  and an upper triangular matrix  $U$  such that:

$$
A = L \cdot U
$$

Where:
- $L$  is a lower triangular matrix, meaning it has non-zero values only on and below the diagonal, and all elements above the diagonal are zero.
- $U$  is an upper triangular matrix, meaning it has non-zero values only on and above the diagonal, and all elements below the diagonal are zero.

Given:

$$
A = \begin{pmatrix}
a_{11} & a_{12} & a_{13} \\
a_{21} & a_{22} & a_{23} \\
a_{31} & a_{32} & a_{33}
\end{pmatrix}
$$

We need to find:

$$
L = \begin{pmatrix}
1 & 0 & 0 \\
l_{21} & 1 & 0 \\
l_{31} & l_{32} & 1
\end{pmatrix}
\quad \text{and} \quad
U = \begin{pmatrix}
u_{11} & u_{12} & u_{13} \\
0 & u_{22} & u_{23} \\
0 & 0 & u_{33}
\end{pmatrix}
$$

> ### General procedure:
> For each $j = 1, 2, \dots N$, we follow these two steps:
>    
> (a) Use the following equation to solve for each $u_{ij}$ where $i = 1, 2, \dots, j$ (in the below equation, the summation term is zero when $i=1$):
> 
> $$u_{ij} = a_{ij} - \sum_{k=1}^{i-1} l_{ik} u_{kj}$$
> 
> (b) Use the following equation to solve for $l_{ij}$ where $i =j+1, j+2, \dots, N$:
> 
> $$ l_{ij} = \frac{1}{u_{jj}} \left( a_{ij} - \sum_{k=1}^{j-1} l_{ik} u_{kj} \right) $$

## $j=1$

### (a) Solve for $u_{i1}$ where $i=1$

$$u_{11} = a_{11} - \sum_{k=1}^{0} l_{ik} u_{kj} = a_{11}$$

### (b) Solve for $l_{i1}$ where $i=1,2,3$ ($l_{11}=1$, as already known):

$$\begin{cases}
l_{21} = \frac{1}{u_{11}} (a_{21}-0) = \frac{a_{21}}{u_{11}}\ , \\
l_{31} = \frac{1}{u_{11}} (a_{31}-0) = \frac{a_{31}}{u_{11}}\ ,
\end{cases}
$$

## $j=2$

### (a) Solve for $u_{i2}$ where $i=1,2$

$$\begin{cases}
u_{12} = a_{12} - \sum_{k=1}^{0} l_{1k} u_{k2} = a_{12}\\
u_{22} = a_{22} - \sum_{k=1}^{1} l_{2k} u_{k2} = a_{22}-l_{21}u_{12}\\
\end{cases}$$

### (b) Solve for $l_{i2}$ where $i=2,3$ ($l_{22}=1$, as already known):

$$
l_{32} = \frac{1}{u_{22}} (a_{32}-\sum_{k=1}^{1} l_{3k} u_{k2}) = \frac{a_{32}}{u_{22}} - \frac{l_{31}u_{12}}{u_{22}}
$$

## $j=3$

### (a) Solve for $u_{i3}$ where $i=1,2,3$

$$\begin{cases}
u_{13} = a_{13} - \sum_{k=1}^{0} l_{1k} u_{k3} = a_{13}\ ,\\
u_{23} = a_{23} - \sum_{k=1}^{1} l_{2k} u_{k3} = a_{23}-l_{21}u_{13}\ ,\\
u_{33} = a_{33} - \sum_{k=1}^{2} l_{3k} u_{k3} = a_{33}-(l_{31}u_{13}+l_{32}u_{23})
\end{cases}$$

### (b) Solve for $l_{i3}$ where $i=3$ ($l_{33}=1$, as already known)

# Numerical example

Let's consider:

$$
A = \begin{pmatrix}
4 & 3 & 2 \\
3 & 7 & 1 \\
2 & 1 & 5
\end{pmatrix}
$$

## $j=1$

### (a) Solve for $u_{i1}$ where $i=1$

$$u_{11} = a_{11} = 4$$

### (b) Solve for $l_{i1}$ where $i=1,2,3$ ($l_{11}=1$, as already known):

$$\begin{cases}
l_{21} = \frac{a_{21}}{u_{11}} = \frac{3}{4} = 0.75\ , \\
l_{31} = \frac{a_{31}}{u_{11}} = \frac{2}{4} = 0.5\ ,
\end{cases}
$$

## $j=2$

### (a) Solve for $u_{i2}$ where $i=1,2$

$$\begin{cases}
u_{12} = a_{12} = 3\ ,\\
u_{22} = a_{22}-l_{21}u_{12} = 7 - \frac{9}{4} = \frac{19}{4} = 4.75 \\
\end{cases}$$

### (b) Solve for $l_{i2}$ where $i=2,3$ ($l_{22}=1$, as already known):

$$
l_{32} = \frac{a_{32}}{u_{22}} - \frac{l_{31}u_{12}}{u_{22}} = -\frac{2}{19} = -0.105\dots
$$

## $j=3$

### (a) Solve for $u_{i3}$ where $i=1,2,3$

$$\begin{cases}
u_{13} = a_{13} = 2\ ,\\
u_{23} = a_{23}-l_{21}u_{13} = 1 - \frac{6}{4} = -0.5\ ,\\
u_{33} = a_{33}-(l_{31}u_{13}+l_{32}u_{23}) = 5 - 0.5*2 - \frac{5}{38} = \frac{75}{19} = 3.947...
\end{cases}$$

Thus, the matrices $L$ and $U$ are:

$$
L = \begin{pmatrix}
1 & 0 & 0 \\
0.75 & 1 & 0 \\
0.5 & -0.105 & 1
\end{pmatrix}
\quad \text{and} \quad
U = \begin{pmatrix}
4 & 3 & 2 \\
0 & 4.75 & -0.5 \\
0 & 0 & 3.947
\end{pmatrix}
$$
