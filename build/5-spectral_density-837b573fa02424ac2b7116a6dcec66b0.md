# Extraction of spectral densities from lattice correlators

The correlator is defined as

$$c(t) = \int_0^{\infty} e^{-tE}\rho(E)dE$$

This quantity is usually known for some discrete values. We therefore discretise the correlator ($t\mapsto at$ and $E\mapsto \sigma n$, with $0\le t< N_T/a$ and $0\le n< N_E$):

$$\tilde{c}(at) = \sigma \sum_{n=0}^{N_E-1}e^{-at\sigma n} \rho(\sigma n)$$

We introduce the veilbein matrix:

$$\varepsilon_{tn}=\sigma e^{-at\sigma n}$$

which, in principle, is a rectangular matrix. 

Thus we can write: 

$$\tilde{c_t} = \varepsilon_{tn} \rho_n$$

If we multiply $\varepsilon$ for its transpose ($\varepsilon^T$), we obtain:

$$\varepsilon^T_{nt}\tilde{c_t} = \varepsilon^T_{nt}\varepsilon_{tm} \rho_m$$

We introduce: 

$$G_{nm} = \[\varepsilon^T \varepsilon\]\_{nm} = \sigma^2 \sum_{t=0}^{N_T-1} e^{-at\sigma(n+m)} $$

Therefore

$$\varepsilon^T_{nt}\tilde{c}\_t = G_{nm} \rho_m$$

and finally the density $\rho$ is given by:

$$ G_{nm}^{-1} \varepsilon^T_{nt}\tilde{c}_t =\rho_m$$

It is useful to define:

$$ g_t(\sigma n) = \frac{1}{\sigma} \sum_{m=0}^{N_E-1} G_{nm}^{-1} {\varepsilon_{tm}} $$

such that

$$\rho_n = \sum_{t=0}^{N_T-1}g_t(\sigma n) \tilde{c}\_t$$
