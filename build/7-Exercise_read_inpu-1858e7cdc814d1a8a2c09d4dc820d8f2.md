# Exercise — Analysis of a velocity time series

A numerical simulation produces a file containing measurements of the velocity \(v(t)\) at different times.

The input file contains exactly **100 data points**. Each line contains two values:

```text
time    velocity
```

For example:

```text
0.0   1.24
0.1   1.31
0.2   1.18
...
```

The first column contains the time \(t\), while the second column contains the velocity \(v(t)\).

The input file is provided as `input.dat`.

## Program execution

Write a C program that is executed from the command line as

```bash
./analyze input.dat output.dat
```

where:

- `argv[0]` is the program name;
- `argv[1]` is the input file name;
- `argv[2]` is the output file name.

The program must check that the correct number of command-line arguments has been provided.

## Tasks

1. Open the input file specified by `argv[1]`.

2. Check that the file was opened successfully.

3. Read the 100 pairs $(t_i, v_i)$ and store them in two arrays:

```c
double time[100];
double velocity[100];
```

4. Compute the mean velocity $\bar{v} = \frac{1}{N} \sum_{i=0}^{N-1} v_i$, with $N=100$.

5. Compute the standard deviation
$\sigma_v =
\sqrt{
\frac{1}{N}
\sum_{i=0}^{N-1}
(v_i-\bar{v})^2
}$.

6. Find the minimum and maximum velocity.

7. Count how many velocity values satisfy $v_i > \bar{v}$.

8. Open the output file specified by `argv[2]`.

9. Write the following information to the output file:

```text
Number of measurements: 100

Mean velocity:     ...
Std. deviation:    ...
Minimum velocity:  ...
Maximum velocity:  ...
Values above mean: ...
```

10. After the summary, write all data points in the following format:

```text
# time        velocity        deviation_from_mean
0.000000      ...             ...
0.100000      ...             ...
...
```

where $\delta v_i = v_i - \bar{v}$.

11. Close both files using `fclose()`.

## Useful functions

You may use:

```c
fopen()
fclose()
fscanf()
fprintf()
sqrt()
```

Remember to include:

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
```

Compile the program with:

```bash
gcc analyze.c -o analyze -lm
```


## Dataset
<details>
	<summary> input.dat </summary>

	0.0 1.320000
	0.1 1.345334
	0.2 1.365332
	0.3 1.380159
	0.4 1.390194
	0.5 1.396005
	0.6 1.398321
	0.7 1.397990
	0.8 1.395940
	0.9 1.393127
	1.0 1.390493
	1.1 1.388915
	1.2 1.389163
	1.3 1.391863
	1.4 1.397465
	1.5 1.406218
	1.6 1.418156
	1.7 1.433097
	1.8 1.450645
	1.9 1.470207
	2.0 1.491019
	2.1 1.512179
	2.2 1.532683
	2.3 1.551475
	2.4 1.567490
	2.5 1.579704
	2.6 1.587183
	2.7 1.589123
	2.8 1.584888
	2.9 1.574049
	3.0 1.556395
	3.1 1.531958
	3.2 1.501007
	3.3 1.464049
	3.4 1.421807
	3.5 1.375196
	3.6 1.325289
	3.7 1.273275
	3.8 1.220414
	3.9 1.167989
	4.0 1.117254
	4.1 1.069388
	4.2 1.025448
	4.3 0.986331
	4.4 0.952738
	4.5 0.925156
	4.6 0.903835
	4.7 0.888790
	4.8 0.879801
	4.9 0.876432
	5.0 0.878055
	5.1 0.883879
	5.2 0.892994
	5.3 0.904413
	5.4 0.917119
	5.5 0.930114
	5.6 0.942463
	5.7 0.953342
	5.8 0.962071
	5.9 0.968149
	6.0 0.971275
	6.1 0.971361
	6.2 0.968538
	6.3 0.963147
	6.4 0.955721
	6.5 0.946966
	6.6 0.937720
	6.7 0.928919
	6.8 0.921550
	6.9 0.916604
	7.0 0.915027
	7.1 0.917676
	7.2 0.925277
	7.3 0.938383
	7.4 0.957350
	7.5 0.982311
	7.6 1.013165
	7.7 1.049577
	7.8 1.090983
	7.9 1.136613
	8.0 1.185511
	8.1 1.236577
	8.2 1.288605
	8.3 1.340332
	8.4 1.390482
	8.5 1.437821
	8.6 1.481203
	8.7 1.519614
	8.8 1.552212
	8.9 1.578355
	9.0 1.597631
	9.1 1.609864
	9.2 1.615122
	9.3 1.613709
	9.4 1.606149
	9.5 1.593156
	9.6 1.575607
	9.7 1.554495
	9.8 1.530889
	9.9 1.505883
</details>

## Solution
<details>
  <summary>Solution in C</summary>
   
   ```c
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	
	#define N 100
	
	int main(int argc, char **argv)
	{
	    FILE *fin;
	    FILE *fout;
	
	    double time[N];
	    double velocity[N];
	
	    /* Check command-line arguments */
	    if (argc != 3) {
	        printf("Usage: %s input_file output_file\n", argv[0]);
	        return 1;
	    }
	
	    /* Open input file */
	    fin = fopen(argv[1], "r");
	
	    if (fin == NULL) {
	        printf("Error: cannot open input file %s\n", argv[1]);
	        return 1;
	    }
	
	    /* Read the 100 data points */
	    for (int i = 0; i < N; i++) {
	        fscanf(fin, "%lf %lf", &time[i], &velocity[i]);
	    }
	
	    fclose(fin);
	
	    /* Compute mean velocity */
	    double sum = 0.0;
	
	    for (int i = 0; i < N; i++) {
	        sum += velocity[i];
	    }
	
	    double mean = sum / N;
	
	    /* Compute standard deviation */
	    double sum2 = 0.0;
	
	    for (int i = 0; i < N; i++) {
	        double diff = velocity[i] - mean;
	        sum2 += diff * diff;
	    }
	
	    double std_dev = sqrt(sum2 / N);
	
	    /* Find minimum and maximum */
	    double min = velocity[0];
	    double max = velocity[0];
	
	    for (int i = 1; i < N; i++) {
	
	        if (velocity[i] < min) {
	            min = velocity[i];
	        }
	
	        if (velocity[i] > max) {
	            max = velocity[i];
	        }
	    }
	
	    /* Count values above the mean */
	    int above_mean = 0;
	
	    for (int i = 0; i < N; i++) {
	
	        if (velocity[i] > mean) {
	            above_mean++;
	        }
	    }
	
	    /* Open output file */
	    fout = fopen(argv[2], "w");
	
	    if (fout == NULL) {
	        printf("Error: cannot open output file %s\n", argv[2]);
	        return 1;
	    }
	
	    /* Write summary */
	    fprintf(fout, "Number of measurements: %d\n\n", N);
	
	    fprintf(fout, "Mean velocity:     %f\n", mean);
	    fprintf(fout, "Std. deviation:    %f\n", std_dev);
	    fprintf(fout, "Minimum velocity:  %f\n", min);
	    fprintf(fout, "Maximum velocity:  %f\n", max);
	    fprintf(fout, "Values above mean: %d\n", above_mean);
	
	    /* Write data and deviation from the mean */
	    fprintf(fout, "\n");
	    fprintf(fout, "# time        velocity        deviation_from_mean\n");
	
	    for (int i = 0; i < N; i++) {
	
	        fprintf(fout, "%f    %f    %f\n",
	                time[i],
	                velocity[i],
	                velocity[i] - mean);
	    }
	
	    fclose(fout);
	
	    return 0;
	}
```
</details>
