# No programmer is an island

Scientific computing is an essential tool for modern researchers, but it often suffers from inefficiencies and errors due to a lack of formal training in software development. By embracing a set of best practices, scientists can significantly improve the reliability, maintainability, and productivity of their code. This chapter outlines key principles to guide researchers in writing better scientific software:

1. Prioritize Readability
2. Leverage Automation
3. Evolve Code Incrementally
4. Minimize Redundancy
5. Plan for Errors
6. Optimize Judiciously
7. Document for Understanding
8. Collaborate Effectively

## 1. Prioritize Readability

Code is read far more often than it is written. To ensure that software remains comprehensible:
- **Simplify and modularize**: Break down programs into small, clear functions, each performing a single task.
- **Use meaningful names**: Choose variable and function names that convey purpose, avoiding ambiguous terms.
- **Maintain consistency**: Adopt uniform formatting and naming conventions to streamline collaboration and reduce errors.

Readable code is not just easier to debug—it also facilitates reuse and adaptation for future projects.

> [!TIP]
> As a rule of thumb, a well-written function must fit entirely on a laptop screen -- at a comfortable font size! 😉

### Naming Conventions

The **names** you choose for **variables**, **functions**, and other elements in your code play a **critical role** in its readability and maintainability. **Descriptive and consistent naming** conventions help others (and **your future self!!!**) understand the code’s purpose at a glance. Poor naming, on the other hand, leads to confusion and errors.

#### 1.	Be Descriptive
Names should clearly indicate the role or purpose of a variable or function. Avoid generic names like temp or data unless they truly convey the meaning.
Example:
- ✅ `particle_velocity`
- ❌ `v`
#### 2.	Use Consistent Formatting
Stick to a single naming style throughout your project, such as **snake_case** or **camelCase**. Inconsistent styles can be distracting and make your code harder to follow.
Example:
-	✅ `compute_average` (snake_case)
-	✅ `computeAverage` (camelCase)
-	❌ `computeAverage` and `compute_Variance` (mixing styles)
#### 3.	Avoid Abbreviations
Shortcuts and abbreviations are often unclear, especially for someone unfamiliar with the context.
Example:
- ✅ `total_energy`
- ❌ `totE`
#### 4.	Distinguish Similar Names
Avoid using names that are too similar, as they can easily be confused.
Example:
- ✅ `initial_velocity, final_velocity`
- ❌ `velocity1, velocity2`
#### 5.	Avoid Reserved Words and Context-Specific Jargon
Steer clear of language-specific reserved words (e.g., `int`, `class`) and domain-specific acronyms unless they’re universally understood in your field.

#### Examples of Good and Bad Practice

❌ Bad Practice:
```C
double x, y;  // x and y are not descriptive.
double calc(double a, double b);  // Unclear what calc does.
```
✅ Improved Code:
```C
double particle_mass, particle_velocity;  // Names describe their purpose.
double compute_kinetic_energy(double mass, double velocity);  // Clearly describes its task.
```
❌ Bad Practice:
```C
double v1 = 10.0;
double v2 = 20.0;  // Unclear distinction between v1 and v2.
```
✅ Improved Code:
```C
double initial_velocity = 10.0;
double final_velocity = 20.0;  // Explicit distinction between the two variables.
```
❌ Bad Practice:
```C
int compute;  // Uses a reserved word, could lead to confusion or errors.
```
✅ Improved Code:
```C
int num_computations;  // Avoids reserved words and adds clarity.
```

## 2. Leverage Automation

Scientists often perform **repetitive computational tasks**. Automating these workflows saves time and reduces the risk of human error.
- **Automate repetitive actions**: Write scripts to handle frequently used operations.
- **Use build tools**: Employ tools like **Make** or workflow managers to handle complex dependencies and data pipelines.

Automation frees researchers to focus on analysis rather than redundant tasks.

## 3. Evolve Code Incrementally

Good software development is an **iterative** process. Start small and refine incrementally:
- **Develop in manageable steps**: Make small changes and test frequently to ensure the code behaves as expected.
- **Track progress with version control**: Tools like **Git** help manage changes, revert to earlier states, and collaborate effectively.

This approach accommodates the evolving nature of research projects, where requirements often shift based on results.

## 4. Minimize Redundancy

**Duplication in code** or data increases the risk of inconsistencies and **errors**. To avoid this:
- **Centralize critical data**: Use single sources of truth for constants and datasets.
- **Modularize functionality**: Reuse functions or libraries instead of duplicating logic across files.

Adhering to the "**Don’t Repeat Yourself**" principle makes code easier to maintain and less prone to bugs.

### Principles of Reducing Redundancy

#### 1.	Encapsulate Repeated Logic in Functions
Whenever you find yourself copying and pasting code, stop and create a function instead. This reduces duplication and makes the logic easier to debug and reuse.
Example:
- ✅ Using a function:
```C
double calculate_area(double length, double width) {
    return length * width;
}
```
Calling the function:
```C
double area1 = calculate_area(5.0, 3.0);
double area2 = calculate_area(7.0, 2.0);
```

- ❌ Repeating the same logic:
```C
double area1 = 5.0 * 3.0;
double area2 = 7.0 * 2.0;
```

#### 2. Avoid Hardcoding
Place repeated values or logic into functions that take parameters, allowing flexibility and reducing potential for errors.
Example:
- ✅ Flexible function:
```C
double compute_cylinder_volume(double radius, double height) {
    const double pi = 3.141592653589793;
    return pi * radius * radius * height;
}
```
Calling the function with different values:
```C
double volume1 = compute_cylinder_volume(2.0, 5.0);
double volume2 = compute_cylinder_volume(3.0, 7.0);
```

- ❌ Hardcoding values:
```C
double volume1 = 3.141592653589793 * 2.0 * 2.0 * 5.0;
double volume2 = 3.141592653589793 * 3.0 * 3.0 * 7.0;
```

#### 3.	Reuse Existing Functions
If a library or another part of your program already provides the functionality you need, use it instead of rewriting the code.
Example:
- ✅ Reusing a function for finding the maximum value:
```C
#include <math.h> // Library function
double find_maximum(double a, double b) {
    return fmax(a, b);
}
```
- ❌ Implementing a redundant maximum function:
```C
double find_maximum(double a, double b) {
    return (a > b) ? a : b;
}
```

### Examples of Good and Bad Practice

❌ Bad Practice:

Repeating code to convert temperatures from Celsius to Fahrenheit multiple times:
```C
double temp1_f = (temp1_c * 9.0 / 5.0) + 32.0;
double temp2_f = (temp2_c * 9.0 / 5.0) + 32.0;
double temp3_f = (temp3_c * 9.0 / 5.0) + 32.0;
```
✅ Improved Code:

Using a function to handle the conversion:
```C
double celsius_to_fahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

double temp1_f = celsius_to_fahrenheit(temp1_c);
double temp2_f = celsius_to_fahrenheit(temp2_c);
double temp3_f = celsius_to_fahrenheit(temp3_c);
```

❌ Bad Practice:

Calculating distances repeatedly with inline code:
```C
double distance1 = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
double distance2 = sqrt(pow(x4 - x3, 2) + pow(y4 - y3, 2));
```
✅ Improved Code:

Creating a reusable function for distance calculation:
```C
#include <math.h>
double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double distance1 = calculate_distance(x1, y1, x2, y2);
double distance2 = calculate_distance(x3, y3, x4, y4);
```
By consolidating repeated tasks into functions, you not only reduce redundancy but also make your code more modular and easier to debug or enhance. This approach aligns with the “Don’t Repeat Yourself” (DRY) principle, a cornerstone of efficient programming practices.

## 5. Plan for Errors

Mistakes are inevitable, but their impact can be mitigated with careful preparation:
- **Validate assumptions**: Add **checks** (assertions) in the code to catch **invalid inputs** or **unexpected behavior**.
- **Write tests**: Use **automated tests** to verify that individual components work as intended and continue to do so after changes.
-	**Debug effectively**: Employ interactive debugging tools to identify and fix issues systematically.

Proactively addressing errors improves software reliability and reduces the time spent troubleshooting.

## 6. Optimize Judiciously

Performance is important, but **premature optimization can lead to complexity** and wasted effort. Start by making the code correct:
- **Profile before optimizing**: Use profiling tools to identify bottlenecks rather than guessing.
- **Prototype in high-level languages**: Develop early versions in user-friendly languages, then translate critical sections to low-level languages like C or Fortran only if needed.

This approach balances productivity with performance, ensuring that effort is focused where it matters most.

## 7. Document for Understanding

**Clear documentation** bridges the gap between the code and its users, ensuring that others (and **your future self!!!**) can understand its purpose and usage:
- **Focus on intent**: Describe what the code does and why, rather than how it works.
- **Embed documentation**: Include comments directly in the code or use tools to generate user-friendly references.

> [!WARNING]
> Documentation is not an **afterthought!** It is a key part of making software usable and reproducible.

## 8. Collaborate Effectively

Scientific computing often involves teamwork. Good collaboration practices improve both the quality of the code and the transfer of knowledge:
- **Review before merging**: Have team members examine code for readability and correctness before integrating it.
- **Use task tracking**: Manage project milestones and bug fixes with an issue tracker.
- **Pair programming when needed**: Tackle challenging problems or onboard new team members through close collaboration.

By embracing collaboration, teams can build better software and share expertise more effectively.
