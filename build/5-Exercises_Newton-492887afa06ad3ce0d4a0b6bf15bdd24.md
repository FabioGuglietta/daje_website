# Build a Stable Planetary System

Simulate N gravitating bodies in two dimensions by integrating:

$$
\frac{d\mathbf{r}_i}{dt}=\mathbf{v}_i,
\qquad
\frac{d\mathbf{v}_i}{dt}=\mathbf{a}_i
=G\sum_{j\ne i}m_j
\frac{\mathbf{r}_j-\mathbf{r}_i}{|\mathbf{r}_j-\mathbf{r}_i|^3}.
$$

Here $\mathbf{r}_i=(x_i,y_i)$, $\mathbf{v}_i=(v_{x,i},v_{y,i})$, and
$m_i$ are position, velocity, and mass. Use reduced units with G = 1.
All bodies move, including the star.

Write the simulation in C using structures, a dynamically allocated array,
and functions that access and update the array through pointers. Save the
trajectories and visualize the orbits with the supplied Python script.

## Integrate with velocity Verlet

Compute the initial accelerations. For each time step, perform these stages:

1. Half-step velocity update:
   $$
   \mathbf{v}_i^*=\mathbf{v}_i+\frac{\Delta t}{2}\mathbf{a}_i.
   $$
2. Position update:
   $$
   \mathbf{r}_i^{new}=\mathbf{r}_i+\Delta t\,\mathbf{v}_i^*.
   $$
3. Recompute the accelerations from the new positions.
4. Complete the velocity update:
   $$
   \mathbf{v}_i^{new}=\mathbf{v}_i^*+\frac{\Delta t}{2}\mathbf{a}_i^{new}.
   $$

Complete each stage for **all bodies** before moving to the next. Reset
accelerations to zero before summing contributions and exclude self-interactions.
Stop if a separation is smaller than `1e-6`; close encounters require a smaller
time step and are outside this exercise.

## Represent and read the system

Use this structure:

```c
struct Body {
    int id;
    double mass;
    double position[2];
    double velocity[2];
    double acceleration[2];
};
```

Read the number of bodies and their initial states from a text file:

```text
N
id mass x y vx vy
...
```

Allocate an array of N bodies after reading N. Suggested interfaces:

```c
struct Body *read_system(const char *filename, int *n);
int compute_accelerations(struct Body *bodies, int n);
int step_verlet(struct Body *bodies, int n, double dt);
```

The reader returns the allocated array and sets `*n`; return `NULL` on failure.
The other functions return 1 on success and 0 on failure. Use `const` pointers
for functions that inspect the system without modifying it.

Download the initial configurations:

- [two_bodies.dat](planetary_motion/two_bodies.dat): a star of mass 1 and a planet
  of mass 0.001, separated by one unit, in a circular orbit.
- [three_bodies.dat](planetary_motion/three_bodies.dat): a star and two interacting
  planets, initially on approximately circular orbits.

- [solar_system.dat](planetary_motion/solar_system.dat): the Sun and eight planets;
  see the simplified Solar System configuration below.

All three files have the center of mass initially at rest at the origin.

Compile and run:

```bash
gcc planetary.c -o planetary -lm
./planetary two_bodies.dat trajectory.dat 0.01 10000
```

The arguments are input file, trajectory file, time step, and step count. This example ends at t = 100.

Check argument count, file openings, input records, positive masses and body
count, positive time step and step count, and allocations. Close files and free
allocated memory.

## Write trajectories

Save t = 0 and every complete step. The trajectory file has one row per body
at each time:

```text
# time id x y vx vy
```

Keep identifiers unchanged and use `%.15g` for floating-point output.

For the supplied two-body case, the orbital period is approximately 6.28.

## Visualize

Download [plot_orbits.py](planetary_motion/plot_orbits.py). It requires NumPy
and Matplotlib.

```bash
python plot_orbits.py trajectory.dat
python plot_orbits.py trajectory.dat --animate
```

To save the orbit plot, add `--save orbits.png` to the first
command.

## A simplified Solar System

The file `solar_system.dat` uses solar masses and a length unit of approximately
1 AU. With G = 1, one time unit is about 58 days; Earth's orbital period is
approximately `2 * pi` time units.

| ID | Body |
| --- | --- |
| 0 | Sun |
| 1 | Mercury |
| 2 | Venus |
| 3 | Earth |
| 4 | Mars |
| 5 | Jupiter |
| 6 | Saturn |
| 7 | Uranus |
| 8 | Neptune |

Masses and orbital radii are based on the [NASA Planetary Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/)
and [Sun Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/sunfact.html).
The radii use the planets' semimajor axes. Initial orbits are circular and
coplanar, with successive planets placed 45 degrees apart and tangential speeds
`sqrt((1 + mass) / radius)`. Positions and velocities are shifted to the
center-of-mass frame. These are teaching initial conditions, not an ephemeris
for a real date; eccentricities, inclinations, and moons are omitted.

```bash
./planetary solar_system.dat solar_trajectory.dat 0.01 10000
python plot_orbits.py solar_trajectory.dat --animate
```

This covers roughly 16 years. For a full orbit of Neptune, increase the step
count to about `110000`. Zoom into the plot to inspect the inner planets.

## Experiments

Run the following experiments separately. Keep the original input files and
write each trajectory to a different output file.

### 1. Recover a circular orbit

Use `two_bodies.dat` without modifying it:

```bash
./planetary two_bodies.dat circular.dat 0.01 10000
python plot_orbits.py circular.dat --animate
```

The planet should move on a nearly circular orbit and the star should make a
much smaller orbit around the center of mass. Estimate the period from the
time between successive revolutions, using the time displayed in the animation
or recorded in the trajectory file. Compare it with the expected value, about
6.28 time units.

### 2. Change the initial speed

Make three copies of `two_bodies.dat`:

```bash
cp two_bodies.dat two_bodies_slow.dat
cp two_bodies.dat two_bodies_fast.dat
cp two_bodies.dat two_bodies_escape.dat
```

Each body record has the columns `id mass x y vx vy`. In this particular file,
`vx = 0` for both bodies. Change **only the last column, `vy`**, in each copy:

| Input file | Factor applied to original velocities | Star: `vy`, ID 0 | Planet: `vy`, ID 1 |
| --- | --- | --- | --- |
| `two_bodies_slow.dat` | 0.8 | -0.000799600299750218 | 0.799600299750218 |
| `two_bodies_fast.dat` | 1.2 | -0.00119940044962533 | 1.19940044962533 |
| `two_bodies_escape.dat` | 1.5 | -0.00149925056203166 | 1.49925056203166 |

These factors all refer to the **original** velocities. Do not obtain the
1.2 case by multiplying the already reduced velocities by 1.2.

Run the three configurations:

```bash
./planetary two_bodies_slow.dat slow.dat 0.01 10000
./planetary two_bodies_fast.dat fast.dat 0.01 10000
./planetary two_bodies_escape.dat escape.dat 0.01 10000
```

Visualize each output, for example:

```bash
python plot_orbits.py slow.dat --animate
```

Repeat with `fast.dat` and `escape.dat`. The slow and fast cases should produce
ellipses: the initial separation is respectively the maximum and minimum
separation of the orbit. The escape case should not close.

For these circular initial conditions, the escape speed is `sqrt(2)` times
the original relative speed. The factor 1.5 exceeds that threshold.
Scaling both bodies' velocities preserves the zero total momentum.

### 3. Change the time step

Use the same elliptical configuration, `two_bodies_slow.dat`, in all three
runs. Change the step count so the final time stays at `t = 100`:

| Time step | Number of steps | Final time |
| --- | --- | --- |
| 0.02 | 5000 | 100 |
| 0.01 | 10000 | 100 |
| 0.005 | 20000 | 100 |

```bash
./planetary two_bodies_slow.dat slow_dt020.dat 0.02 5000
./planetary two_bodies_slow.dat slow_dt010.dat 0.01 10000
./planetary two_bodies_slow.dat slow_dt005.dat 0.005 20000
```

Plot each trajectory and compare the ellipse, estimated period, and final
positions at the same time. Results should approach one another as the time
step decreases. If the curves look identical at first, zoom in; small position
differences can accumulate over many revolutions.

### 4. Increase a planet's mass

First run the supplied three-body configuration:

```bash
./planetary three_bodies.dat three_original.dat 0.01 10000
python plot_orbits.py three_original.dat --animate
```

Copy `three_bodies.dat` to `three_bodies_heavy.dat`. In the record with
**ID 2**, change the mass (second column) from `0.0003` to `0.003`. Leave
all positions and velocities unchanged, then run:

```bash
./planetary three_bodies_heavy.dat three_heavy.dat 0.01 10000
python plot_orbits.py three_heavy.dat --animate
```

Compare the motion of the star and both planets with the original case.
The more massive planet produces stronger accelerations on the other bodies.

Changing the mass also changes the center-of-mass position and velocity.
A translation of the whole system is therefore not, by itself, evidence of
an unstable orbit. For a comparison without that drift, recenter the modified
initial state before integrating:

$$
\mathbf{R}_{CM}=\frac{\sum_i m_i\mathbf{r}_i}{\sum_i m_i},
\qquad
\mathbf{V}_{CM}=\frac{\sum_i m_i\mathbf{v}_i}{\sum_i m_i},
$$

then subtract $\mathbf{R}_{CM}$ from every initial position and
$\mathbf{V}_{CM}$ from every initial velocity. This preserves all relative
positions and velocities. Stability over a finite run does not imply permanent
stability.

## Submission

Submit the C source, modified input files, orbit plots, and a short comparison
of the four experiments. State which input parameters changed in each case
and what changed in the resulting motion.

## Solution

<details>
<summary>Solution in C</summary>

Save the code as `planetary.c`, or [download it](planetary_motion/planetary.c).
Use distinct paths for the input and output files.
As in the introductory I/O examples, numeric inputs are assumed to be within the representable ranges of the C types.

```c
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Body {
    int id;
    double mass;
    double position[2];
    double velocity[2];
    double acceleration[2];
};

struct Body *read_system(const char *filename, int *n) {
    FILE *input = fopen(filename, "r");
    if (input == NULL) {
        fprintf(stderr, "Cannot open %s\n", filename);
        return NULL;
    }

    int count;
    if (fscanf(input, "%d", &count) != 1 || count <= 0) {
        fprintf(stderr, "Invalid body count.\n");
        fclose(input);
        return NULL;
    }

    struct Body *bodies = calloc(count, sizeof *bodies);
    if (bodies == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(input);
        return NULL;
    }

    int valid = 1;
    for (int i = 0; i < count && valid; ++i) {
        struct Body *b = &bodies[i];
        if (fscanf(input, "%d %lf %lf %lf %lf %lf",
                   &b->id, &b->mass,
                   &b->position[0], &b->position[1],
                   &b->velocity[0], &b->velocity[1]) != 6) {
            valid = 0;
            break;
        }
        if (!isfinite(b->mass) || b->mass <= 0.0) valid = 0;
        for (int k = 0; k < 2; ++k) {
            if (!isfinite(b->position[k]) || !isfinite(b->velocity[k]))
                valid = 0;
        }
        for (int j = 0; j < i; ++j) {
            if (bodies[j].id == b->id) valid = 0;
        }
    }

    fclose(input);
    if (!valid) {
        fprintf(stderr, "Invalid body record, mass, or duplicate identifier.\n");
        free(bodies);
        return NULL;
    }

    *n = count;
    return bodies;
}

int compute_accelerations(struct Body *bodies, int n) {
    /* Reduced units: G = 1. */
    const double minimum_distance = 1e-6;

    for (int i = 0; i < n; ++i) {
        bodies[i].acceleration[0] = 0.0;
        bodies[i].acceleration[1] = 0.0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = bodies[j].position[0] - bodies[i].position[0];
            double dy = bodies[j].position[1] - bodies[i].position[1];
            double r2 = dx * dx + dy * dy;
            if (!isfinite(r2) || r2 < minimum_distance * minimum_distance) {
                fprintf(stderr, "Invalid separation between bodies %d and %d.\n",
                        bodies[i].id, bodies[j].id);
                return 0;
            }

            double inverse_r3 = 1.0 / (r2 * sqrt(r2));
            bodies[i].acceleration[0] += bodies[j].mass * dx * inverse_r3;
            bodies[i].acceleration[1] += bodies[j].mass * dy * inverse_r3;
            bodies[j].acceleration[0] -= bodies[i].mass * dx * inverse_r3;
            bodies[j].acceleration[1] -= bodies[i].mass * dy * inverse_r3;
        }
    }
    return 1;
}

int step_verlet(struct Body *bodies, int n, double dt) {
    /* Half kick for all bodies. */
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < 2; ++k)
            bodies[i].velocity[k] += 0.5 * dt * bodies[i].acceleration[k];

    /* Drift for all bodies. */
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < 2; ++k)
            bodies[i].position[k] += dt * bodies[i].velocity[k];

    if (!compute_accelerations(bodies, n)) return 0;

    /* Second half kick for all bodies. */
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < 2; ++k)
            bodies[i].velocity[k] += 0.5 * dt * bodies[i].acceleration[k];

    return 1;
}

int write_snapshot(FILE *output, const struct Body *bodies, int n, double time) {
    for (int i = 0; i < n; ++i) {
        if (fprintf(output, "%.15g %d %.15g %.15g %.15g %.15g\n",
                    time, bodies[i].id,
                    bodies[i].position[0], bodies[i].position[1],
                    bodies[i].velocity[0], bodies[i].velocity[1]) < 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s input trajectory dt steps\n", argv[0]);
        return EXIT_FAILURE;
    }

    double dt;
    int steps;
    if (sscanf(argv[3], "%lf", &dt) != 1 || !isfinite(dt) || dt <= 0.0 ||
        sscanf(argv[4], "%d", &steps) != 1 || steps <= 0) {
        fprintf(stderr, "Time step and step count must be positive.\n");
        return EXIT_FAILURE;
    }

    int n;
    struct Body *bodies = read_system(argv[1], &n);
    if (bodies == NULL) return EXIT_FAILURE;
    if (!compute_accelerations(bodies, n)) {
        free(bodies);
        return EXIT_FAILURE;
    }

    FILE *trajectory = fopen(argv[2], "w");
    if (trajectory == NULL) {
        fprintf(stderr, "Cannot open %s\n", argv[2]);
        free(bodies);
        return EXIT_FAILURE;
    }

    int ok = fprintf(trajectory, "# time id x y vx vy\n") >= 0;

    /* Save the initial state, before the first integration step. */
    if (ok) ok = write_snapshot(trajectory, bodies, n, 0.0);

    for (int step = 0; step < steps && ok; ++step) {
        if (!step_verlet(bodies, n, dt)) {
            ok = 0;
            break;
        }
        double time = (step + 1) * dt;
        ok = write_snapshot(trajectory, bodies, n, time);
    }

    if (fclose(trajectory) == EOF) ok = 0;
    free(bodies);

    if (!ok) {
        fprintf(stderr, "Simulation stopped: integration or output error.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
```

</details>
