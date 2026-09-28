# Dedicated Lecture: regex - Practical Examples

## Practical example: selecting useful lines from a log

Given `simulation.log`, print only simulation step lines:

```bash
grep -E '^step' simulation.log
```

Print only failed or warning steps:

```bash
grep -E 'status (WARNING|FAILED)$' simulation.log
```

Print lines with decimal numbers:

```bash
grep -E '[0-9]+\.[0-9]+' simulation.log
```

Print assignment lines:

```bash
grep -E '^[[:space:]]*[A-Za-z_][A-Za-z0-9_]*[[:space:]]*=' simulation.log
```

Print non-comment lines:

```bash
grep -Ev '^[[:space:]]*#' simulation.log
```

---

## Practical example: modifying an input file with regex

Create an input file:

```bash
cat > inputfile
# Simulation parameters
dt = 0.01
nsteps = 100
restart = false
output_file = results.dat
path = /home/m.sega/data
^D
```

Change `dt`, regardless of its previous value:

```bash
sed -E 's/^dt[[:space:]]*=.*/dt = 0.005/' inputfile
```

Change `nsteps`:

```bash
sed -E 's/^nsteps[[:space:]]*=.*/nsteps = 1000/' inputfile
```

Change only uncommented `restart` lines:

```bash
sed -E '/^[[:space:]]*#/!s/^restart[[:space:]]*=.*/restart = true/' inputfile
```

Replace a path:

```bash
sed -E 's|/home/m\.sega/data|/scratch/m.sega/data|' inputfile
```

Apply all edits:

```bash
sed -E \
    -e 's/^dt[[:space:]]*=.*/dt = 0.005/' \
    -e 's/^nsteps[[:space:]]*=.*/nsteps = 1000/' \
    -e '/^[[:space:]]*#/!s/^restart[[:space:]]*=.*/restart = true/' \
    -e 's|/home/m\.sega/data|/scratch/m.sega/data|' \
    inputfile > inputfile_new
```

Inspect:

```bash
cat inputfile_new
```
