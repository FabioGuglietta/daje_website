# Estimate the Uncertainty of a Derived Quantity Using Bootstrap

In many experimental or computational settings, one often needs to propagate uncertainties from measured quantities to derived functions. 

In real-world experiments, it is common for different measurements to come from different sources, instruments, or time series—leading to datasets with different lengths. In this exercise, you are given two independent sets of measurements:

```Plain
A(1), A(2), ..., A(NA)
B(1), B(2), ..., B(NB)
```
and you know that there is a third quantity (C) that has a given mean value and a given error. 
These measurements are assumed to be independently drawn from some unknown underlying distributions.

Define a derived quantity from these variables as:

$$F_i = A(i) * \sin(B(i)) + C(i)$$

Estimate the uncertainty (standard error) on the mean of the derived observable $\langle F\rangle$ using the bootstrap method.


This exercise demonstrates how to:
	- Propagate measurement uncertainty through non-linear functions.
	- Estimate the distribution and confidence intervals of derived quantities.
	- Apply the bootstrap method as a powerful, assumption-free alternative to error propagation formulas.
	
## Hints
1.	Load (or generate) the three datasets A, B, and C (each of length N, e.g. `N = 1000`).
2.	Compute the original value of the derived observable:
	- Evaluate $F(i) = A(i)·sin(B(i)) + C(i)$ for $i = 1,…,N$.
	- Compute the mean value $\langle F\rangle$.
3.	Bootstrap procedure:
	- Generate M bootstrap resamples (e.g. M = 1000), each obtained by sampling `N` triplets `(A, B, C)` with replacement from the original dataset.
	- For each resample, compute the new set of `F*` values and the corresponding average $\langle F*\rangle$.
	- Store the `M` values of $\langle F*\rangle$.
4.	Estimate:
	- The bootstrap estimate of the standard error of $\langle F\rangle$ as the standard deviation of the `M` $\langle F*\rangle$ values.
