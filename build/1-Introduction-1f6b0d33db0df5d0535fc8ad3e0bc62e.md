# No programmer is an island

Scientific code will be read, modified, and reused by other people, including
your future self. Clear names, comments, and consistent interfaces make it
easier to understand a calculation and work on it together.

This lesson focuses on how to write and communicate code. The next lessons
cover [reliable numerical calculations](2-Numerical_best_practices.md) and
[organizing a scientific project](3-Organizing_scientific_code.md).

## Make the calculation readable

Use names that make sense in their context. In a short numerical loop,
`i`, `n`, `x`, and `v` can match the mathematical notation perfectly well.
Across a larger program, names such as `time_step`, `particle_mass`, and
`output_file` make their roles easier to recognize.

For example, this function hides the meaning of its arguments.

**Hard to interpret:**

```c
double calc(double a, const double b[]) {
    return 0.5 * a * (b[0] * b[0] + b[1] * b[1]);
}
```

**Clearer:**

```c
double kinetic_energy(double mass, const double velocity[2]) {
    return 0.5 * mass *
           (velocity[0] * velocity[0] + velocity[1] * velocity[1]);
}
```

The function name states what is calculated; the arguments identify the
required data. The expression can be checked directly against the formula.

Comments should supply information that the code alone does not give:

```c
/* Velocities are advanced by half a step in velocity Verlet. */
double half_step = 0.5 * time_step;
```

Document units, boundary conditions, and assumptions. For example, state
whether velocities are in SI or reduced units and whether positions obey
periodic boundaries.

For a calculation using SI units, make that convention explicit.

**Ambiguous:**

```c
double dt = 0.001;  // What is the time unit?
```

**Clearer:**

```c
/* Positions are in metres and velocities in metres per second. */
double time_step_seconds = 0.001;
```

Both declarations are valid C. The difference is whether a reader can identify
the physical meaning without guessing.

### Naming conventions

Choose one naming style and use it consistently. For the C examples in this
course, `snake_case` is a useful default:

| Element | Convention | Examples |
| --- | --- | --- |
| Variables | Name the quantity or role | `particle_mass`, `input_file`, `num_steps` |
| Functions | Describe the operation | `compute_force`, `read_configuration`, `write_snapshot` |
| Macros | Uppercase with underscores | `MAX_FILENAME_LENGTH` |
| Structure types | Use a consistent type-name style | `struct Body`, `struct SimulationParameters` |

Prefer names that distinguish the roles of related values, such as
`initial_velocity` and `final_velocity`. Numbered names such as `velocity1`
and `velocity2` are appropriate only when the numbering has a clear meaning.

Include units when that prevents ambiguity: `time_seconds` and
`temperature_kelvin` are useful at input/output boundaries. In a calculation
using reduced units, document the unit convention once and follow it
consistently.

Standard mathematical names and familiar abbreviations can be clearer than
long descriptions. Names such as `x`, `i`, `dt`, and `num_particles` are
reasonable when their meaning is evident. Avoid inventing abbreviations that
readers must decode.

Use the same name for the same concept throughout the project. For example,
choose `num_particles` or `particle_count` rather than alternating between
both for no reason.

> [!NOTE]
> A naming convention helps readers; it is not a rule imposed by C.
> `compute_force` and `computeForce` are both valid. Follow an existing
> project's conventions when contributing to it. Avoid language keywords,
> names beginning with an underscore, and names that conflict with standard
> library functions such as `printf` or `sqrt`.

## Keep repeated definitions consistent

Duplicated formulas can diverge when one copy is edited. Suppose
`velocity_before` and `velocity_after` each hold two velocity components.

**Problematic:**

```c
double energy_before = 0.5 * mass *
    (velocity_before[0] * velocity_before[0] +
     velocity_before[1] * velocity_before[1]);

double energy_after = mass *
    (velocity_after[0] * velocity_after[0] +
     velocity_after[1] * velocity_after[1]);  // Missing factor 0.5.
```

**Improved:**

```c
double energy_before = kinetic_energy(mass, velocity_before);
double energy_after = kinetic_energy(mass, velocity_after);
```

The shared function keeps the physical definition in one place. It still
needs to be checked against a known result.

## Agree on conventions when working together

Follow the project's naming and formatting style. Agree on units, array shapes,
and who is responsible for allocating and releasing data passed between functions.

When reviewing a change, check both what it does and how another person would
use it. Explain changes to physical assumptions or file formats in the review
and documentation. Keep unrelated edits separate so a collaborator can follow
the reasoning.

## Write for the next person

Include a short README with the purpose of the code, how to compile and run it,
the input and output formats, and one example with an expected result.

Before sharing a change, make sure another person can run that example from
the documented instructions. Ask them to inspect the assumptions, units,
indexing, and boundary conditions as well as the C syntax.

The next reader may be a collaborator, a student, or your future self.
Leave enough information for them to understand both the program and the
scientific calculation it implements.
