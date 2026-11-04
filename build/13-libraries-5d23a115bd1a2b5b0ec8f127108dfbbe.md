# Static and Shared Libraries in C

Suppose you collect numerical routines to reuse in future simulation programs.
We will package two functions, an arithmetic mean and a dot product, into a
library and call them from `main`. The same source code will produce a
**static library** and a **shared library**.

The main commands below are for **Linux with GCC**. The static example also
works on macOS; alternative commands for its shared library are given below.

## The complete source files

Place these three files in the same directory. They are also available to
download: [numutils.h](libraries_example/numutils.h),
[numutils.c](libraries_example/numutils.c), and [main.c](libraries_example/main.c).

### numutils.h

```c
#ifndef NUMUTILS_H
#define NUMUTILS_H

double num_mean(const double values[], int n);
double num_dot(const double a[], const double b[], int n);

#endif
```

### numutils.c

```c
#include "numutils.h"

double num_mean(const double values[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += values[i];
    }
    return sum / n;
}

double num_dot(const double a[], const double b[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}
```

### main.c

```c
#include <stdio.h>
#include "numutils.h"

int main(void) {
    double a[] = {1.0, 2.0, 3.0, 4.0};
    double b[] = {2.0, 0.0, 1.0, 3.0};
    int n = 4;

    printf("Mean of a: %.2f\n", num_mean(a, n));
    printf("Dot product: %.2f\n", num_dot(a, b, n));
    return 0;
}
```

Both arrays passed to `num_dot` must contain at least n elements; `num_mean`
also requires n > 0. The example uses n = 4.

The header declares the functions. Their definitions belong to `numutils.c`.
Including the header lets the compiler check calls; it does not supply the
compiled function code to the executable. The linker must connect those calls
to their implementations.

## Create and use a static library

First compile the library source and the main program into object files:

```bash
gcc -c numutils.c -o numutils_static.o
gcc -c main.c -o main.o
```

The option `-c` compiles without creating an executable. Package the library
object into a static archive:

```bash
ar rcs libnumutils.a numutils_static.o
```

`ar` manages archives: `r` adds or replaces object files, `c` creates the archive
without a creation warning, and `s` writes the symbol index used for linking.
The conventional name is `lib` followed by the library name and `.a`.

Link the main program with that archive, then run:

```bash
gcc main.o ./libnumutils.a -o demo_static
./demo_static
```

Output:

```text
Mean of a: 2.50
Dot product: 17.00
```

The linker extracts the archive members needed to resolve the calls from
`main.o` and incorporates their code into `demo_static`. The executable no
longer needs `libnumutils.a` to run.

> [!NOTE]
> Linking our library statically does not make the entire program statically
> linked. The executable can still depend on shared system libraries.

## Create and use a shared library

A shared library keeps its compiled code in a separate file. The executable
records a dependency that the runtime loader must satisfy when the program starts.

On Linux, compile a separate object with position-independent code, then
create the shared object:

```bash
gcc -fPIC -c numutils.c -o numutils_shared.o
gcc -shared numutils_shared.o -o libnumutils.so
```

`-fPIC` generates code suitable for a shared library whose load address can
vary. `-shared` creates the shared object rather than a normal executable.

Reuse the same `main.o` and link:

```bash
gcc main.o -L. -lnumutils -o demo_shared
```

- `-L.` adds the current directory to the linker's search paths.
- `-lnumutils` searches for a library named `numutils`, using the `lib` prefix
  and the platform's library suffix.

On Linux, when both `libnumutils.so` and `libnumutils.a` are available in the
same search directory, this command normally chooses the shared version.
That is why the static example specifies `./libnumutils.a` explicitly.

Run the shared executable from the directory containing the library:

```bash
LD_LIBRARY_PATH=. ./demo_shared
```

It prints the same two results as `demo_static`.

> [!NOTE]
> **Finding a library when linking and when running are separate operations.**
>
> `-L.` helps build the executable; it does not make the current directory
> a runtime search path. Here `LD_LIBRARY_PATH=.` tells the Linux loader where
> to find our shared library for this invocation. Installed libraries and
> embedded runtime search paths are other possible arrangements.

<details>
<summary>Shared-library commands on macOS</summary>

macOS uses `.dylib` and different linker options. After compiling
`numutils_shared.o` and `main.o` as above, use:

```bash
gcc -dynamiclib numutils_shared.o -Wl,-install_name,@rpath/libnumutils.dylib -o libnumutils.dylib
gcc main.o ./libnumutils.dylib -Wl,-rpath,@executable_path -o demo_shared
./demo_shared
```

The install name uses `@rpath`, and the executable supplies its own directory
as a runtime search path. Keep the executable and `libnumutils.dylib` together.
`-Wl,` passes the following comma-separated options to the linker.

</details>

## See the difference

On Linux, temporarily rename the shared library:

```bash
mv libnumutils.so libnumutils.so.saved
./demo_static
LD_LIBRARY_PATH=. ./demo_shared
mv libnumutils.so.saved libnumutils.so
```

The static executable still runs. The shared one cannot start because its
library is missing. The final command restores the library.

On macOS, make the same comparison by temporarily renaming
`libnumutils.dylib` and running the executables without `LD_LIBRARY_PATH`.

## Reuse and update the library

A new program can include `numutils.h` and link the existing library without
recompiling `numutils.c`. If the header and library are in another directory,
provide their locations, for example:

```bash
gcc -I/path/to/numutils another_main.c /path/to/numutils/libnumutils.a -o another_program
```

`-I` supplies a header search path. Replace the example paths with the directory
containing your files.

If you change a routine's implementation, rebuild the library:

- **Static:** rebuild `libnumutils.a`, then relink each executable that should
  receive the change. Its previously incorporated code is otherwise unchanged.
- **Shared:** rebuild `libnumutils.so`. New runs of compatible executables
  use the replacement without relinking. If the binary interface changes,
  dependent programs may need rebuilding too.

Changing only an implementation does not require recompiling `main.c` when
the interface is unchanged. Changes to declarations or data layouts require
checking and rebuilding the callers as appropriate.

## Which version should you use?

| | Static library | Shared library |
| --- | --- | --- |
| Executable | Incorporates the required library code | Keeps a runtime dependency |
| Distribution | No need to distribute our archive with the executable | Library must be available and discoverable |
| Updating a routine | Rebuild the archive and relink callers | Replace the library, preserving binary compatibility |
| Several programs | Each executable incorporates its own library code | Programs can use one installed library; code pages can be shared in memory |
| Main practical advantage | Simple handling of a project's own dependency | Central installation and updates |
| Main practical cost | Updates require relinking callers | Runtime paths and compatible versions must be managed |

For a personal collection of numerical routines, a static library is a simple
starting point. A shared library is useful when a common installation serves
several programs. Neither choice guarantees faster numerical calculations:
performance depends on the code, compiler options, and workload.

Further reading: [GCC linking options](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html),
[Linux runtime loader](https://man7.org/linux/man-pages/man8/ld.so.8.html),
and [macOS run-path-dependent libraries](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/DynamicLibraries/100-Articles/RunpathDependentLibraries.html).
