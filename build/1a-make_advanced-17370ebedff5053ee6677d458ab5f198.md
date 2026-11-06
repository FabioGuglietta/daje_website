# Makefiles: Beyond the Basics

This Makefile builds the same `energy` program as in
[Makefiles: The Basics](1-make.md), with sources in `src`, headers in `include`,
and generated object files in `build`:

```makefile
CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -O2 -Wall -Wextra
LDFLAGS =
LDLIBS =

TARGET = energy
SOURCES = src/main.c src/physics.c src/mathutils.c src/output.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

build/%.o: src/%.c | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)

-include $(DEPS)
```

Recipe lines still begin with a tab. This example uses GNU Make and a
compiler supporting `-MMD` and `-MP`, such as GCC or Clang.

## Arrange the project files

Use the same four source files and three headers as in the project outlined
in the basic lesson. Place them as follows before running `make`:

```text
project/
├── Makefile
├── include/
│   ├── physics.h
│   ├── mathutils.h
│   └── output.h
└── src/
    ├── main.c
    ├── physics.c
    ├── mathutils.c
    └── output.c
```

The Makefile creates `build` when necessary. The option `-Iinclude` tells
the compiler to look in `include` for headers, so `#include "physics.h"`
continues to work.

## Use variables for repeated settings

A definition such as `CC = gcc` assigns text to a Make variable.
`$(CC)` inserts its value into a command. These are Make variables, not
variables in the C program.

| Variable | Role in this Makefile |
| --- | --- |
| `CC` | C compiler |
| `CPPFLAGS` | Preprocessor options, such as `-Iinclude` and macro definitions |
| `CFLAGS` | C compilation options, here optimization and warnings |
| `LDFLAGS` | Linker options, such as `-L/path/to/libraries` |
| `LDLIBS` | Libraries to link, such as `-lm` |
| `TARGET` | Executable name |
| `SOURCES` | Source files belonging to this program |
| `OBJECTS` | Corresponding object files |
| `DEPS` | Generated dependency files |

`LDFLAGS` and `LDLIBS` are empty because this example needs no extra
libraries. For a program that uses the mathematical library, you could set
`LDLIBS = -lm`. Libraries appear after the object files in the link command.

The substitution

```makefile
OBJECTS = $(SOURCES:src/%.c=build/%.o)
```

turns `src/main.c src/physics.c src/mathutils.c src/output.c` into
`build/main.o build/physics.o build/mathutils.o build/output.o`.
The next substitution replaces `.o` with `.d` to obtain the dependency file names.

Keep the source list explicit: a directory may also contain test programs
with their own `main`, which should not be linked into this executable.

## Replace repeated compilation rules with a pattern

```makefile
build/%.o: src/%.c | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@
```

`%` matches the shared part of a file name. To build `build/physics.o`,
this rule uses `src/physics.c`.

**Automatic variables** provide names for the current rule:

| Variable | Meaning | For this compilation |
| --- | --- | --- |
| `$@` | Target being built | `build/physics.o` |
| `$<` | First normal prerequisite | `src/physics.c` |

In the link recipe, `$@` instead means `energy`. Another useful automatic
variable is `$^`, the list of normal prerequisites with duplicates removed.
For the executable rule, it could replace `$(OBJECTS)`.

See the [GNU Make automatic variables](https://www.gnu.org/s/make/manual/html_node/Automatic-Variables.html)
for these definitions.

## Create the build directory before compiling

The `| build` part is an **order-only prerequisite**: the directory must
exist before compilation, but changes to the directory's modification time
must not trigger recompilation. Adding an object file changes that timestamp
without changing any C source.

The separate `build` rule creates the directory if it is missing. Its command,
`mkdir -p build`, also succeeds if the directory already exists.

The `all` action depends on the executable and appears before the other
ordinary rules. It is therefore the default goal: `make` and `make all`
build the same program.

## Let the compiler track header dependencies

During compilation, `-MMD` writes a `.d` file alongside each object file,
listing its source and included project headers. For example,
`build/main.d` contains a rule equivalent to:

```makefile
build/main.o: src/main.c include/physics.h include/output.h
```

`-MP` adds empty rules for the listed headers. This avoids a stale dependency
file causing a Make error solely because a header has been removed or renamed.
It does not allow C code to include a header that no longer exists.

The final line reads these generated rules:

```makefile
-include $(DEPS)
```

The leading `-` tolerates missing `.d` files, as on the first build.
After that build, editing `include/physics.h` automatically recompiles
`main.c` and `physics.c` before relinking. This also tracks project headers
included indirectly through other headers.

Editing `include/mathutils.h` similarly recompiles `physics.c` and
`mathutils.c`. The compiler records this dependency because `physics.c`
includes that header, even though the call to `square` is resolved only
when linking with `build/mathutils.o`.

The [GCC preprocessor options](https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html)
document `-MMD` and `-MP`.

## Change build options deliberately

Command-line assignments override the values in this Makefile. For example,
to rebuild for debugging:

```bash
make clean
make CFLAGS="-g -O0 -Wall -Wextra"
```

To rebuild with Clang instead of GCC, if it is installed:

```bash
make clean
make CC=clang
```

Make does not normally detect changes to compiler options or recipe text.
Without cleaning, it may reuse object files compiled with the old settings.
For this small project, a clean rebuild avoids mixing configurations.

`make clean` removes the entire generated `build` directory and the
executable. Keep source files, inputs, and simulation results outside `build`.

## Inspect commands and compile independent modules in parallel

```bash
make -n
```

This prints the commands needed for the requested build without executing
the recipes in this example. If everything is up to date, there is nothing
to print.

```bash
make -j4
```

This allows up to four build jobs at once. The four modules can compile
independently, but linking still waits for all object files. It speeds up
the build where useful; it does not parallelize the simulation itself.

Run cleaning and building as separate commands, as above. Requesting
`clean` and `all` together with parallel jobs can make deletion race with
compilation.

## Add another module

Place its `.c` file in `src`, its headers in `include`, and add the source
to `SOURCES`. The pattern rule builds its object file; the compiler records
its header dependencies; the object list includes it when linking.

For further details, see the [GNU Make manual](https://www.gnu.org/s/make/manual/make.html).
