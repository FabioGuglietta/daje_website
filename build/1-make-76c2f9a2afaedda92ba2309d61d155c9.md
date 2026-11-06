# Makefiles: The Basics

Consider a small C project organized as follows:

```text
project/
├── Makefile
├── main.c
├── physics.c
├── physics.h
├── mathutils.c
├── mathutils.h
├── output.c
└── output.h
```

`physics.c` contains the physical calculations, `mathutils.c` provides
mathematical helper functions, and `output.c` handles output. For example,
the calls can form this chain:

```text
main (main.c)
├── kinetic_energy (physics.c)
│   └── square (mathutils.c)
└── print_energy (output.c)
```

Here `kinetic_energy` calls `square` to compute the speed squared. Each
module includes its own header. In addition, `main.c` includes `physics.h`
and `output.h`, while `physics.c` includes `mathutils.h` to obtain the
declaration of `square`. The public header `physics.h` does not need to
include `mathutils.h`.

## Compile the project by hand

Without a Makefile, we can compile each source file and then link the object
files into an executable called `energy`:

```bash
gcc -c main.c -o main.o
gcc -c physics.c -o physics.o
gcc -c mathutils.c -o mathutils.o
gcc -c output.c -o output.o
gcc main.o physics.o mathutils.o output.o -o energy
```

The first four commands compile the modules independently. The last command
links them together. We can then run the program with `./energy`.

After editing `physics.c`, we must recompile it and link again:

```bash
gcc -c physics.c -o physics.o
gcc main.o physics.o mathutils.o output.o -o energy
```

If we edit only `mathutils.c`, leaving `mathutils.h` unchanged, the commands
are instead:

```bash
gcc -c mathutils.c -o mathutils.o
gcc main.o physics.o mathutils.o output.o -o energy
```

Even though `physics.c` calls `square`, we do not need to recompile it:
the function's declaration has not changed. The existing `physics.o` still
contains a reference to `square`, and linking connects it to the new
implementation in `mathutils.o`.

If we also change `mathutils.h`, we must recompile both files that include
it, then link again:

```bash
gcc -c mathutils.c -o mathutils.o
gcc -c physics.c -o physics.o
gcc main.o physics.o mathutils.o output.o -o energy
```

As the project grows, keeping track of these steps by hand becomes easy to
get wrong:

- If we forget to recompile a changed source file, linking may succeed using
  an old object file. The executable will not contain our latest changes.
- If we change `physics.h`, both `main.c` and `physics.c` must be recompiled
  because they include it. Recompiling only `physics.c` can leave modules
  built against different versions of the same declarations.
- If we recompile an object file but forget to link again, `./energy` still
  runs the previous executable.

We could instead compile every source file on every build:

```bash
gcc main.c physics.c mathutils.c output.c -o energy
```

This avoids reusing old object files, provided compilation and linking
succeed, but repeats all compilation work even when only one module changed.

## Automate the build with a Makefile

The following `Makefile` records the commands and their dependencies, so
Make can rebuild the parts affected by a change:

```makefile
energy: main.o physics.o mathutils.o output.o
	gcc main.o physics.o mathutils.o output.o -o energy

main.o: main.c physics.h output.h
	gcc -c main.c -o main.o

physics.o: physics.c physics.h mathutils.h
	gcc -c physics.c -o physics.o

mathutils.o: mathutils.c mathutils.h
	gcc -c mathutils.c -o mathutils.o

output.o: output.c output.h
	gcc -c output.c -o output.o

.PHONY: clean
clean:
	rm -f energy main.o physics.o mathutils.o output.o
```

> [!IMPORTANT]
> Each command line must begin with a **tab character**, not spaces.
> If your editor inserts spaces when you press Tab, change that setting for
> the Makefile. A `missing separator` error often means the tab is missing.

From the project directory, compile and run with:

```bash
make
./energy
```

`make` reads the Makefile and runs the commands needed to build the program.
It does not replace the compiler: here, it invokes `gcc` for us.

## Read a rule: target, prerequisites, recipe

Each rule follows this structure:

```text
target: prerequisites
	command
```

The **target** is what the rule builds. The **prerequisites** are the files
it depends on. The indented commands form its **recipe**.

For example:

```makefile
physics.o: physics.c physics.h mathutils.h
	gcc -c physics.c -o physics.o
```

This says that `physics.o` depends on `physics.c`, `physics.h`, and
`mathutils.h`. The recipe compiles `physics.c` into the object file `physics.o`:
`-c` requests compilation without linking, and `-o` names the output file.

Header files are prerequisites because changing a declaration can affect
code that includes it. They are not compiled into separate object files.

Although `physics.c` calls a function defined in `mathutils.c`, we do **not**
make `physics.o` depend on `mathutils.o` or `mathutils.c`. Compiling
`physics.c` needs the declaration in `mathutils.h`, not the compiled
implementation of `square`.

The call to `square` remains an unresolved reference in `physics.o` until
linking. The executable depends on both objects so that the linker can
connect that call to the definition in `mathutils.o`.

## Compile the modules, then link them

The first rule describes the final executable:

```makefile
energy: main.o physics.o mathutils.o output.o
	gcc main.o physics.o mathutils.o output.o -o energy
```

Before running this recipe, `make` ensures that the four object files are
up to date, using their rules. It then links them into `energy`.

Without an explicit target, `make` chooses the first ordinary target in
this Makefile: `energy`. You can also request it by name:

```bash
make energy
```

## Does the order matter?

### The first rule chooses the default target

Keep `energy` as the first ordinary target so that `make` builds the whole
program. If you put the `physics.o` rule first, plain `make` would only build
that object file. `make energy` would still build the complete executable,
because it explicitly names the target.

Apart from choosing this default, the explicit rules in our example can
appear in any order. The rule for `energy` can refer to `main.o` before the
rule describing how to build `main.o` appears. Make reads the Makefile before
starting the build; it does not execute its rules from top to bottom like
a shell script.

### Prerequisites describe dependencies, not a sequence of commands

These two prerequisite lists describe the same requirements:

```makefile
energy: main.o physics.o mathutils.o output.o
```

```makefile
energy: output.o mathutils.o main.o physics.o
```

All four object files must be ready before linking. None of these modules
needs another object file to be compiled first. Their independent compilations
may also run in parallel, as explained in the advanced lesson.

Likewise, `physics.o: physics.c physics.h mathutils.h` and
`physics.o: mathutils.h physics.h physics.c` describe the same dependencies
in our basic Makefile. We conventionally list the source first and then its headers.
The recipe explicitly names `physics.c`, so changing the prerequisite order
does not change which file it compiles. In the advanced lesson, the shortcut
`$<` means the **first prerequisite**, so that order will matter there.

### Commands inside a recipe run in order

If one recipe contains several command lines, they run in the order written.
Make normally stops that recipe if a command fails. This is different from
the order in which rules appear in the file.

### The compiler's argument order is a separate issue

The prerequisite list tells Make which files are needed; the recipe tells
`gcc` what command to execute. The four ordinary object files in our link
command can be reordered, but library placement can matter. For example,
when a program needs the mathematical library, write:

```bash
gcc main.o physics.o mathutils.o output.o -o energy -lm
```

Keep libraries after the object files that use them. Reordering prerequisites
alone does not reorder a command that lists those files explicitly.

## Rebuild only what changed

For a file target, `make` checks modification times. It runs the recipe if
the target is missing or a prerequisite is newer. It does not inspect the
C code to discover which headers are used: in this example, we list them.

| Change | What the next `make` rebuilds |
| --- | --- |
| First build | All four object files, then `energy` |
| Edit `physics.c` | `physics.o`, then `energy` |
| Edit `mathutils.c` | `mathutils.o`, then `energy` |
| Edit `mathutils.h` | `physics.o` and `mathutils.o`, then `energy` |
| Edit `physics.h` | `main.o` and `physics.o`, then `energy` |
| Edit `output.h` | `main.o` and `output.o`, then `energy` |
| No changes | Nothing |


Notice the difference between editing `mathutils.c` and `mathutils.h`.
Changing only the implementation of `square` rebuilds `mathutils.o` and
relinks the executable; it does not require recompiling `physics.c`.
Changing the header also rebuilds the code that includes it.

This is why a Makefile is more useful than a script that always recompiles
every source file.

## Remove generated files with `make clean`

```bash
make clean
```

The `clean` recipe removes only the executable and object files listed in
it. It leaves the source and header files in place. `rm -f` also works when
some of those generated files are already absent.

`.PHONY: clean` tells `make` that `clean` names an action, rather than a file
to check. The action will run even if a file called `clean` exists.

To rebuild everything from scratch, run:

```bash
make clean
make
```

## Adapt the Makefile to your program

Choose your executable name and list all its object files in the first rule.
For each `.c` file, add a compilation rule listing that source and the local
headers it uses, including headers included indirectly. When you add a module,
update the link rule and the list of generated files removed by `clean`.

The next lesson, [Makefiles: Beyond the Basics](1a-make_advanced.md), removes
repeated commands and lets the compiler track header dependencies.
The [GNU Make rule documentation](https://www.gnu.org/s/make/manual/html_node/Rules.html)
and [phony targets](https://www.gnu.org/s/make/manual/html_node/Phony-Targets.html)
describe these mechanisms in more detail.
