```plaintext
Function four1(data, nn, isign)
    Define constants:
        PI = 3.141592653589793
    Define variables:
        n = nn * 2             // Total length of the data array (real + imaginary parts)
        j = 1                   // Index for bit-reversed ordering
        tempr, tempi            // Temporary variables for swapping and storing complex values

    // Step 1: Bit-Reversal Permutation
    For i from 1 to n-1 by 2 do
        If j > i then
            Swap data[j] and data[i]
            Swap data[j+1] and data[i+1]
        End If
        
        m = n / 2
        While m >= 2 and j > m do
            j = j - m
            m = m / 2
        End While
        j = j + m
    End For

    // Step 2: Danielson-Lanczos Lemma Application
    mmax = 2                    // Initial size of the transform (2-point)
    While n > mmax do
        istep = mmax * 2
        theta = isign * (2 * PI / mmax)   // Angle for the complex exponential
        wtemp = sin(0.5 * theta)
        wpr = -2.0 * wtemp * wtemp
        wpi = sin(theta)
        wr = 1.0
        wi = 0.0

        // Step 2.1: Loop over different sub-transforms at the current stage
        For m from 1 to mmax-1 by 2 do
            // Step 2.2: Apply the transform to each element within the sub-transform
            For i from m to n by istep do
                j = i + mmax

                // Compute temporary real and imaginary values
                tempr = wr * data[j] - wi * data[j+1]
                tempi = wr * data[j+1] + wi * data[j]

                // Update data values for the current pair
                data[j] = data[i] - tempr
                data[j+1] = data[i+1] - tempi
                data[i] = data[i] + tempr
                data[i+1] = data[i+1] + tempi
            End For

            // Update wr and wi using recurrence relations
            temp_wr = wr
            wr = temp_wr * wpr - wi * wpi + wr
            wi = wi * wpr + temp_wr * wpi + wi
        End For

        // Double the size of the transform for the next stage
        mmax = istep
    End While
End Function
```

<details>
 <summary> Explanation first for loop </summary>
  
 The first loop in the FFT code handles **bit-reversal reordering** of the input data, which is crucial for the FFT algorithm to compute the transformation in an efficient, in-place manner. 
  For an FFT of size $N = 8$, bit-reversal ordering means rearranging indices based on their binary representations, **reversing the bits** to determine their new order. So, the index 1 (binary 001) would move to position 4 (binary 100), and so forth. This rearrangement helps the algorithm to structure data for efficient recursive application of smaller FFTs.
  
  ### Example Walkthrough: \( N = 8 \)
  
  Suppose we have an array `data` of length \( N = 8 \). In our case, the input data will be `f[0], f[1], ..., f[7]`. Since FFT operates on complex numbers, we consider `data` with `2*N` real values, where each pair represents the real and imaginary parts for a point.
  
  ### Initial Setup
  
  Given:
  - `n = 2 * nn = 2 * 8 = 16` (total array length in terms of real + imaginary parts)
  - `j = 1` (index for tracking bit-reversed positions)
    
  Each step will swap elements if `j > i` to achieve bit-reversal ordering.
  
  ### Loop Explanation: Step by Step
  
  #### Initial Array Index Positions
  Before bit-reversal, let's assume we have the following values at each position in the array `data`:
  ```plaintext
  Index:    0  1  2  3  4  5  6  7
  Data:    f0 f1 f2 f3 f4 f5 f6 f7
  ```
  
  #### Bit-Reversal Process
  
  1. **Loop Iteration 1 (`i = 1`)**:
     - `j > i` is **false** (`j = 1`, `i = 1`), so **no swap**.
     - Update `j`: 
       - Calculate `m = n / 2 = 8`.
       - Since `j < m`, add `m` to `j`: `j = j + m = 1 + 8 = 9`.
  
  2. **Loop Iteration 2 (`i = 3`)**:
     - `j > i` is **true** (`j = 9`, `i = 3`), so **swap** `data[3]` with `data[9]` and `data[4]` with `data[10]`.
     - Update `j`:
       - Calculate `m = 4` (half of previous `m = 8`).
       - `j > m`, so subtract `m`: `j = j - m = 9 - 4 = 5`.
     
  3. **Loop Iteration 3 (`i = 5`)**:
     - `j > i` is **true** (`j = 5`, `i = 5`), so **swap** `data[5]` with `data[11]` and `data[6]` with `data[12]`.
     - Update `j`:
       - Calculate `m = 2`.
       - `j < m`, so add `m`: `j = j + m = 5 + 2 = 7`.
     
  4. **Loop Iteration 4 (`i = 7`)**:
     - `j > i` is **true** (`j = 7`, `i = 7`), so **swap** `data[7]` with `data[15]` and `data[8]` with `data[16]`.
     - Update `j`: 
       - Calculate `m = 1`.
       - `j < m`, so add `m`: `j = j + m = 7 + 1 = 8`.
  
  #### Final Reordered Array:
  After the loop, the indices have been swapped into bit-reversed order.
  In this context, the index `m` represents a **pivot or halfway point** used to help calculate the bit-reversed position `j` as the algorithm iterates over the indices `i`.

  ### Purpose of `m` in Bit-Reversal Reordering
  
  The purpose of `m` is to control how `j` is adjusted to follow the bit-reversed order as `i` increments. Essentially, it allows the algorithm to step backwards through the binary representation of `j` to find the next bit-reversed index.
  
  - **If `j < m`**: This condition indicates that the lower bits of `j` still need to increment, so `j` is increased by `m` to reach the next position in bit-reversed order.
  - **If `j >= m`**: This condition means `j` has reached the current bit-reversal limit for its lower bits, so `j` needs to be adjusted down (i.e., we subtract `m`) to set up the next higher bit in the bit-reversed order.
  
  ### Why `m` is Halved
  
  The value of `m` starts at `n / 2` and halves each time `j` needs a larger adjustment (moving to higher bits in the binary representation). This allows the loop to work through each bit level one at a time until `j` aligns correctly with the bit-reversed order for each `i`. By halving `m`, the algorithm efficiently steps through the necessary adjustments to reverse the bits in `j` as `i` progresses.

</details>
