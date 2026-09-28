# Fast Fourier Transform (FFT)

## The Continuous Fourier Transform (CFT)

The Fourier transform is a powerful mathematical tool used to analyze the frequency content of signals and functions. In essence, it allows us to represent a function $f(x)$, which might be a time-dependent signal or a spatial distribution, in terms of its constituent sinusoidal components. 

For a continuous function $f(x)$, the Fourier transform provides a way to **decompose it into a spectrum of frequencies**, which can reveal information that is not immediately apparent in the original function.

The **continuous Fourier transform (CFT)** of a function $f(x)$ is defined by the integral:

$$F(k) = \int_{-\infty}^{\infty} f(x) e^{2 \pi i k x} \ dx \ ,$$

where:

- $f(x)$ is the **original function** defined in the spatial or time domain
- $F(k)$ is the **Fourier transform** of $f(x)$, representing the amplitude and phase of the sinusoidal components at frequency $k$
- $e^{2 \pi i k x}$ is a complex exponential that oscillates at the frequency $k$

Clearly, the Fourier transformation is a **linear opeartion**: 

  - The transform of the sum of two functions is equal to the sum of the transforms.
  - The transform of a constant times a function is that same constant times the transform of the function. 

The **inverse Fourier transform** allows us to reconstruct the original function $f(x)$ from its Fourier transform $F(k)$:

$$f(x) = \int_{-\infty}^{\infty} F(k) e^{-2 \pi i k x} \ dk \ .$$

This transform pair establishes a relationship between the time (or spatial) domain and the frequency (or wavelength) domain, providing a dual view of the information content in $f(x)$.

The continuous Fourier transform is particularly useful in fields such as physics, engineering, and signal processing, where it helps analyze waveforms, signals, and spatial distributions. 

However, working with **continuous transforms is not always practical**, as it assumes an infinite and continuous range of data points. In practical applications, we often need a **discrete representation**, which leads us to the discrete Fourier transform (DFT). The DFT operates on sampled data and lays the foundation for computational methods, like the fast Fourier transform (FFT), which efficiently computes the Fourier transform for large datasets.

### Some properties

In the following table we report some relations between a function $f(x)$ and its Fourier transform $F(k)$:

| If . . .                             | then . . .                              |
|--------------------------------------|-----------------------------------------|
| $f(x)$ is real                       | $F(-k) = [F(k)]^*$                      |
| $f(x)$ is imaginary                  | $F(-k) = -[F(k)]^*$                     |
| $f(x)$ is even                       | $F(-k) = F(k)$  &nbsp; (i.e., $F(k)$ is even) |
| $f(x)$ is odd                        | $F(-k) = -F(k)$ &nbsp; (i.e., $F(k)$ is odd)  |
| $f(x)$ is real and even              | $F(k)$ is real and even                 |
| $f(x)$ is real and odd               | $F(k)$ is imaginary and odd             |
| $f(x)$ is imaginary and even         | $F(k)$ is imaginary and even            |
| $f(x)$ is imaginary and odd          | $F(k)$ is real and odd                  |

Other relations: 

| Transformation                     | Corresponding Change in Fourier Transform   | Description             |
|------------------------------------|---------------------------------------------|-------------------------|
| $f(ax)$                            | $\frac{1}{\vert a\vert}F\left(\frac{k}{a}\right)$ | "time scaling"          |
| $\frac{1}{\vert b\vert} f\left( \frac{x}{b} \right)$ | $F(bk)$                        | "frequency scaling"     |
| $f(x - x_0)$                       | $F(k) e^{2 \pi i k x_0}$                   | "time shifting"         |
| $f(x) e^{-2 \pi i k_0 x}$          | $F(k - k_0)$                               | "frequency shifting"    |


## The Discrete Fourier Transform (DFT)
The **Discrete Fourier Transform (DFT)** is a powerful tool in signal processing and numerical analysis, allowing us to analyze the frequency components of discrete signals or data sequences. 
While the CFT applies to continuous functions, the DFT is defined for **discrete sequences**, making it particularly useful for applications in digital computing and signal processing.

Given a discrete sequence of $N$ complex numbers $x_0, x_1, \dots, x_{N-1}$, the DFT transforms this sequence into another sequence $X_0, X_1, \dots, X_{N-1}$, which represents the frequency spectrum of the original data. 
The transformation is defined as:

$$X_k = \sum_{n=0}^{N-1} x_n e^{2 \pi i k n / N}, \quad k = 0, 1, \dots, N-1$$

Here:
- $x_n$  are the elements of the original sequence (often representing sampled data in time),
- $X_k$  are the elements of the transformed sequence (frequency domain representation),
- $i$  is the imaginary unit, 
- $N$  is the number of points in the sequence.

The DFT maps $N$ complex numbers (the $x_n$) into $N$ complex numbers (the $X_k$).

The original sequence $x_n$ can be recovered from its DFT $X_k$ using the **Inverse Discrete Fourier Transform**, defined as:

$$x_n = \frac{1}{N} \sum_{k=0}^{N-1} X_k e^{-2 \pi i k n / N}, \quad n = 0, 1, \dots, N-1$$

This equation reconstructs the original sequence from its frequency components by reversing the transformation.

### Nyquist critical frequency

For any sampling interval $∆$, there is also a special frequency $f_c$, called the **Nyquist critical frequency**, that is half of the sampling rate, $1/\Delta$, of a discrete signal:

$$f_c = \frac{1}{2\Delta}$$

Let's see why it is important:

- **Sampling Theorem**: According to the Nyquist-Shannon sampling theorem, to accurately capture all the information in a continuous signal without **aliasing** (falsely translated), the signal must be sampled at least at **twice the highest frequency component** present in the signal. This means that if the signal contains a frequency component higher than $f_c$, sampling at a rate lower than $1/\Delta$ would lead to aliasing, where **higher frequencies are misrepresented as lower ones**. In other words: _If a system uniformly samples an analog signal at a rate that exceeds the signal’s highest frequency by at least a factor of two, the original analog signal can be perfectly recovered from the discrete values produced by sampling._

- **Frequency Representation in Discrete Signals**: When we sample a continuous signal at a rate of $1/\Delta$, the highest frequency that can be captured in the resulting discrete signal is $f_c$, or half of the sampling rate. If any frequency component of the signal exceeds this Nyquist frequency, it cannot be distinguished from lower frequencies after sampling.
  
- When performing a DFT on a discrete signal, the **Nyquist frequency corresponds to the highest unique frequency that can be resolved without ambiguity**. In a DFT of length $N$, this corresponds to the index $n = N/2$ (for even $N$). This frequency is the boundary between positive and negative frequencies in the DFT, as both positive and negative frequencies can have values from $0$ to $\pm f_c$.

If a continuous function is **sampled properly**, we can assume that its Fourier transform should be **zero outside $[-f_c,f_c]$**. This is based on the idea that, after proper sampling, **no significant frequency components should exist outside this range**. So, when we estimate the Fourier transform of a signal from discrete samples, we typically expect the signal's frequency content to be confined to the range $[-f_c,f_c]$. This assumption helps us check whether the sampling was done correctly.

To verify that the continuous function has been competently sampled (**with minimal aliasing**), we examine how the Fourier transform behaves near the boundaries of the frequency range. Specifically:
   - As the frequency approaches $f_c$ from below (positive frequencies), or $-f_c$ from above (negative frequencies), the Fourier transform should approach zero.
   - If the Fourier transform doesn't approach zero and instead tends to a nonzero value as it reaches $f_c$ or $-f_c$, this suggests **aliasing**.

To conclude, **aliasing** occurs when **higher frequency components (above the Nyquist frequency)** "fold back" into the sampled signal's frequency range. This happens when the **sampling rate is too low** to capture the signal's full frequency content, causing higher frequencies to appear incorrectly as lower frequencies in the sampled data. 
If the **Fourier transform doesn't approach zero as expected**, it indicates that such folding has occurred, and the sampling was not sufficient to capture the original signal accurately.
By checking the behavior of the Fourier transform at the edges of this range, we can detect whether aliasing has occurred.

### Properties of the Discrete Fourier Transform

The DFT has several useful properties that are similar to those of the continuous Fourier transform:

1.	**Linearity**: The DFT of a linear combination of two sequences is the same combination of their individual DFTs: if $y_n = a x_n + b z_n$ then $Y_k = a X_k + b Z_k$.
2.	**Symmetry**: If the original sequence $x_n$ is real, then the DFT satisfies certain symmetry properties. For instance, the DFT of a real sequence has Hermitian symmetry: $X_{N-k} = X_k^{\*}$, where  $X_k^{\*}$ is the complex conjugate of $X_k$
3.	**Periodicity**: Both the input and output sequences are periodic with period $N$, meaning $x_{n+N} = x_n$  and  $X_{k+N} = X_k$.
4.	**Parseval’s Theorem**: This theorem relates the total energy of the sequence in the time domain to the energy in the frequency domain:
	
$$\sum_{n=0}^{N-1} |x_n|^2 = \frac{1}{N} \sum_{k=0}^{N-1} |X_k|^2$$

### Frequency indices in a DFT

1. **Index Range in DFT**: Usually, in the DFT, the index $n$ runs from $-N/2$ to $N/2$ for $N$ data points. However, because of periodicity in the DFT, the result repeats every $N$ indices. This allows us to “wrap around” the values so that we only need indices from $0$ to $N-1$, effectively covering a full cycle.
2. **Symmetry in Frequency**:  The property $H_{-n} = H_{N - n}$ indicates that values are symmetric around $n = 0$, reflecting the periodic structure. So, by considering only indices from $0$ to $N-1$, we avoid redundancies and simplify the indexing.
3. **Frequency Mapping and Ranges**:  When this convention is applied, the frequencies associated with each index $n$ are interpreted as follows:  
   - **Zero Frequency**: Corresponds to $n = 0$.  
   - **Positive Frequencies**: For frequencies in the range $0 < f < f_c$, the corresponding $n$ values are from $1$ to $N/2 - 1$. These represent the positive part of the frequency spectrum.  
   - **Negative Frequencies**: For frequencies $-f_c < f < 0$, we use the indices from $N/2 + 1$ to $N - 1$. This range effectively represents negative frequencies in the DFT.
4. **Nyquist Frequency**:  
   The index $n = N/2$ is special because it corresponds to the **Nyquist frequency** $f = f_c$, which is the highest frequency that can be resolved without **aliasing**. In this convention, $f = f_c$ is equivalent to $f = -f_c$, representing the boundary between positive and negative frequencies.

By reinterpreting $n$ to range from $0$ to $N-1$, we make the mapping between the indices and frequencies clearer and allow for an easier, symmetric representation of the DFT spectrum.

### Applications of the DFT

The DFT is widely used in fields such as signal processing, image processing, and communications. Some common applications include:
- **Spectral Analysis**: Understanding the frequency content of signals.
- **Filtering**: Applying filters in the frequency domain to remove unwanted components or enhance certain frequencies.
- **Data Compression**: Transforming data to reduce redundancy (e.g., JPEG image compression).
- **Convolution Calculations**: Efficient computation of convolutions, which are essential in many algorithms.

### Computational Complexity and the FFT

Computing the DFT directly using its definition requires $O(N^2)$ operations, because each of the $N$ outputs $X_k$ requires $N$ computations,. For large $N$, this becomes computationally expensive. This is where the **Fast Fourier Transform (FFT)** comes into play, providing a highly efficient algorithm to compute the DFT with a complexity of $O(N \log N)$. The FFT revolutionized digital signal processing by making the DFT feasible for large data sets.

In the next section, we will explore the FFT in detail, discussing how it optimizes the DFT calculation and why it is a cornerstone of modern signal processing.

## Fast Fourier Transform (FFT)

To compute the DFT, we can define 

$$W^{kn}_N = e^{2\pi i kn/N}$$

and the we have:

$$X_k = \sum_{n=0}^{N-1} x_n e^{2 \pi i k n / N} = \sum_{n=0}^{N-1} W^{kn}_N x_n , \quad k = 0, 1, \dots, N-1$$

In other words, **calculating the DFT** can be represented as **multiplying a vector of input values $x_n$ by a matrix**, where each entry in the matrix has the form $W^{k \times n}$, with $W$ being a complex constant. This matrix multiplication yields a new vector, $X_k$, containing the transformed values. However, performing this operation directly requires $N^2$ complex multiplications, making the DFT an $O(N^2)$ process. 

At first glance, this might seem like the only way to compute the DFT, but there's a far more efficient approach. The **Fast Fourier Transform (FFT)** algorithm allows us to compute the **DFT in only $O(N \log N)$ operations**, a significant improvement. For a large input size, say $N = 10^6$, the difference in performance is drastic: rather than taking around two weeks (for perforiming $10^12$ operations), an FFT computation might only take seconds on the same computer (by performing $\approx 6\times 10^{6}$ operations).

The FFT algorithm was popularized in the mid-1960s through the work of J.W. Cooley and J.W. Tukey, but this discovery was not entirely new. Efficient methods for computing the DFT date back much further; mathematicians, including Carl Friedrich Gauss, independently discovered similar techniques as early as 1805.

A key rediscovery of the FFT, made by **Danielson and Lanczos in 1942**, provides an elegant and straightforward derivation of the algorithm. They showed that a DFT of length $N$ can be **broken down into two smaller DFTs**, each of length $N/2$. Specifically, this involves **separating the original sequence into two parts**: one DFT that processes only the **even-numbered** points and another that processes the **odd-numbered** points. 

By expressing the full DFT in terms of these two smaller DFTs, they revealed a structure that significantly reduces the computational load:

$$ F_k = \sum_{j=0}^{N-1} e^{2 \pi i j k / N} f_j = \sum_{j=0}^{N/2 - 1} e^{2 \pi i k (2j) / N} f_{2j} + \sum_{j=0}^{N/2 - 1} e^{2 \pi i k (2j + 1) / N} f_{2j + 1}= \sum_{j=0}^{N/2 - 1} e^{2 \pi i k j / (N/2)} f_{2j} + W^k_N \sum_{j=0}^{N/2 - 1} e^{2 \pi i k j / (N/2)} f_{2j + 1} = F_k^e + W^k_N F_k^o$$

The term $F_k^e$ represents the $k$-th component of the Fourier transform of length $N/2$, calculated from the **even-indexed elements** of the original sequence $f_j$. Similarly, $F_k^o$ is the Fourier transform of length $N/2$ obtained from the **odd-indexed** elements. Note that in the last equality, $k$ ranges from 0 to $N$, rather than stopping at $N/2$. 

The beauty of the Danielson-Lanczos Lemma is that it can be **applied repeatedly**. By breaking down $F_k$ into the smaller transforms $F_k^e$ and $F_k^o$, we can further split each of these into transforms of length $N/4$, composed of even and odd indices within each half. This approach continues recursively, dividing the data into smaller and smaller sections, labeled as $F_k^{ee}$ (**even-even terms**), $F_k^{eo}$ (**even-odd terms**), and so on, with each subdivision based on even and odd indices.

The most straightforward application of this approach is when **$N$ is a power of two**. It is highly recommended to use the FFT only when $N$ is a power of two, as this ensures the recursive breakdown works optimally. If your dataset length is not a power of two, you should **pad it with zeros** until it reaches the next power of two. This recursive splitting continues until each sub-transform is of **length 1**. At this point, the Fourier transform of a single element is trivial—it simply returns that element, effectively **copying the input to the output directly**.

In other words, for every pattern of $\log_2 N$ e’s and o’s, there is a one-point transform that is just one of the input numbers $f_n$

$$F_k^{eooeooeeeeoo\dots oee} = f_n$$

The next step is to determine **which value of $n$ corresponds to each sequence of e's and o's**. Here’s the trick: **reverse** the sequence of e's and o's, then **replace** each "e" with 0 and each "o" with 1. The result is the **binary representation of $n$**.

Why does this method work? It’s because each successive division into even and odd parts effectively tests the lower bits of $n$. Each step down in the recursive process separates the data based on whether an index is even or odd at that bit level, starting from the least significant bit. By following this process, the bit pattern builds up from the least significant bit to the most significant bit as we proceed through the recursion.
	
		           f0   f1   f2   f3   f4   f5   f6   f7
		                   /                   \
		                  e                     o
		           f0 f2 f4 f6              f1 f3 f5 f7
		           /        \               /        \
		          e          o             e          o
		        f0 f4      f2 f6         f1 f5        f3 f7
		       /     \     /   \         /   \        /   \
		      e      o    e     o       e     o      e     o
		     f0     f4    f2    f6     f1     f5     f3    f7
		     eee    eeo   eoe   eoo    oee    oeo    ooe   ooo
	Reverse:     eee    oee   eoe   ooe    eeo    oeo    eoo   ooo
	(e=0, o=1):  000    100   010   110    001    101    011   111

This bit-reversal technique is an essential part of making FFT algorithms efficient. The idea is to **rearrange the original data** array in a special way: instead of ordering data elements sequentially by their index $j$, we order them according to the "bit-reversed" version of $j$. This means that if we represent the index $j$ in binary, we reverse its bits to get a new index, and reorder the data based on these reversed indices.

	| 000 |   -->	| 000 |
	| 001 | 	| 100 |
	| 010 |   -->	| 010 |
	| 011 |     	| 110 |
	| 100 |		| 001 |
	| 101 |   -->  	| 101 |
	| 110 |      	| 011 |
	| 111 |	  -->	| 111 |

Using this bit-reversed order **simplifies the recursive application** of the Danielson-Lanczos Lemma. In this configuration, we start with single-point "transforms" (just the original data points), then combine adjacent pairs to get two-point transforms, then combine adjacent pairs of those to get four-point transforms, and continue doubling in this manner. This process progresses until we combine the first and second halves of the entire dataset into the final transform.

The FFT algorithm achieves an $O(N \log_2 N)$ runtime because each level of combination involves $N$ operations and there are $\log_2 N$ levels (or "stages") of combination. Reordering the data into bit-reversed order itself also takes $O(N \log_2 N)$ time, so it doesn’t increase the algorithm's overall complexity.

The FFT algorithm, then, consists of two main parts:
1. **Bit-reversal reordering**: This rearranges the data in-place, without requiring additional storage, by swapping pairs of elements according to bit-reversed indices.
2. **Transform computation**: This part has an outer loop that runs $\log_2 N$ times, each time calculating sub-transforms of length $2, 4, 8, \ldots, N$. Each stage applies the Danielson-Lanczos Lemma through nested inner loops, combining previously computed sub-transforms. To optimize efficiency, sines and cosines of angles are calculated only $\log_2 N$ times in the outer loop and used with recurrence relations in the inner loops to avoid repeated calls for trigonometric functions. 

This structure allows FFT to efficiently compute discrete Fourier transforms, dramatically reducing computation time compared to direct DFT methods.

### Example

The Danielson-Lanczos Lemma states that:

$$F(n) = \sum_{k=0}^{N-1}x(k)\ e^{-\frac{j2\pi kn}{N}} = \sum_{k=0}^{N/2-1}x(2k)\ e^{-\frac{j2\pi kn}{N/2}} + W_N^n \sum_{k=0}^{N/2-1}x(2k+1)\ e^{-\frac{j2\pi kn}{N/2}} = E + O$$

where the first summation is the "even terms" ($E$) and the second is the "odd terms" ($O$); $W_N^n=e^{-\frac{j2\pi n}{N}}$ is the Twiddle factor. 

For the case where $N = 4$, we need to expand the expression further to separate it into four terms. 
Starting from the values $E$ and $O$:

$$E = \sum_{k=0}^{N/2-1}x(2k)\ e^{-\frac{j2\pi kn}{N/2}} $$
$$O = W_N^n \sum_{k=0}^{N/2-1}x(2k+1)\ e^{-\frac{j2\pi kn}{N/2}} $$

we continue by substituting $k = 2k$ for the even terms and $k = 2k + 1$ for the odd terms, thereby halving the range of the summation. 
This results in the following expressions:

$$E = EE + EO$$
$$O = OE + OO$$

with:

$$EE = \sum_{k=0}^{N/4-1} x(2(2k)) e^{-\frac{j 2\pi (2k) n}{N/2}} = \sum_{k=0}^{N/4-1} x(4k) e^{-\frac{j 2\pi k n}{N/4}} $$

$$EO = \sum_{k=0}^{N/4-1} x(2(2k+1)) e^{-\frac{j 2\pi (2k+1) n}{N/2}} = \sum_{k=0}^{N/4-1} x(4k+2) e^{-\frac{j 2\pi k n}{N/4}}\left[e^{-\frac{j 2\pi n}{N/2}}\right] = W_{N/2}^n \sum_{k=0}^{N/4-1} x(4k+2) e^{-\frac{j 2\pi k n}{N/4}}$$

$$OE = W_N^n \sum_{k=0}^{N/4-1} x(2(2k)+1) e^{-\frac{j 2\pi (2k) n}{N/2}}=W_N^n \sum_{k=0}^{N/4-1} x(4k+1) e^{-\frac{j 2\pi k n}{N/4}}$$

$$OO =  W_N^n \sum_{k=0}^{N/4-1} x(2(2k+1)+1) e^{-\frac{j 2\pi (2k+1) n}{N/2}} = W_N^n\sum_{k=0}^{N/4-1} x(4k+3) e^{-\frac{j 2\pi k n}{N/4}}\left[e^{-\frac{j 2\pi n}{N/2}}\right] = W_N^n W_{N/2}^n \sum_{k=0}^{N/4-1} x(4k+3) e^{-\frac{j 2\pi k n}{N/4}}$$

Each summation involves splitting and applying the Fourier components in a recursive pattern, halving the size of the summations at each level until we reach single-point transforms. This breakdown simplifies computation by leveraging symmetry and periodicity in the terms, a core idea in the Fast Fourier Transform (FFT) algorithm.

Finally, putting it all together:

$$F(n) = E + O = EE + EO + OE + OO = \sum_{k=0}^{N/4-1} x(4k)\ e^{-\frac{j 2\pi k n}{N/4}} +  W_{N/2}^n \sum_{k=0}^{N/4-1} x(4k+2)\ e^{-\frac{j 2\pi k n}{N/4}}+ W_N^n \sum_{k=0}^{N/4-1} x(4k+1)\ e^{-\frac{j 2\pi k n}{N/4}} + W_N^n W_{N/2}^n \sum_{k=0}^{N/4-1} x(4k+3)\ e^{-\frac{j 2\pi k n}{N/4}}$$

Since $N=4$, all the sums range from 0 to 0:

$$F(n)= x(0) +  W_{2}^n x(2)+ W_4^n x(1) + W_4^n W_{2}^n x(3)$$

## Windowing

In Fourier analysis, **windowing** is a technique used to improve the accuracy of the DFT when applied to real-world signals, especially those that are **not periodic** within the sampled interval. When we analyze signals in practice, we often encounter finite-length data segments that may not reflect the full behavior of the signal over time. This truncation can introduce errors, commonly known as **spectral leakage**, which makes interpreting the frequency components more challenging. Windowing mitigates these effects and enhances the quality of the frequency analysis.

The DFT assumes that the data being transformed is periodic, meaning that the signal repeats itself seamlessly. However, if the data ends abruptly or does not naturally repeat, the DFT sees a discontinuity at the edges of the interval. This discontinuity introduces artificial **high-frequency components** that distort the frequency spectrum of the signal. This phenomenon, called **spectral leakage**, causes energy from one frequency to "leak" into neighboring frequencies, resulting in a smeared or spread-out spectrum.

For example, if we analyze a single tone signal (e.g., a sine wave) with a finite, non-periodic segment, the frequency spectrum ideally should have a single sharp peak at the signal's frequency. However, without windowing, spectral leakage will cause **additional frequency components** to appear around the main peak, complicating the analysis and obscuring other important frequencies in the data.

### What is Windowing?

Windowing involves multiplying the signal by a **window function** before performing the DFT. A window function is typically a smooth, bell-shaped curve that **tapers to zero at the edges**. By applying a window, we force the signal to **smoothly approach zero at the edges**, reducing the abrupt transitions that cause spectral leakage.

Mathematically, for a signal $x[n]$ of length $N$, windowing modifies the signal by multiplying each sample by a window function $w[n]$:

$$x_{\text{windowed}}[n] = x[n] \cdot w[n] \quad \text{for } n = 0, 1, \ldots, N-1.$$

The windowed signal $x_{\text{windowed}}[n]$ is then used as the input to the DFT, resulting in a more accurate frequency spectrum.

There are **many window functions**, each designed to balance between minimizing spectral leakage and preserving signal information. Here are some commonly used window functions:

- **Hann (or Hanning) Window**: A cosine-based window that smoothly tapers to zero at the edges. It significantly reduces spectral leakage and is widely used for general-purpose frequency analysis.

- **Hamming Window**: Similar to the Hann window, but with a slightly different shape. It reduces side lobes (frequency leakage) even further, making it useful when high spectral resolution is needed.

- **Blackman Window**: A more aggressively tapered window, reducing side lobes even more than the Hann or Hamming windows. However, it also has a wider main lobe, meaning some frequency resolution is sacrificed.

The choice of window depends on the specific analysis goals—whether to prioritize frequency resolution (sharp peaks in the frequency domain) or suppression of leakage.
