# Object-Oriented Programming

Modern scientific and technical problems often require the simulation, modeling, and manipulation of complex systems: particles with position and velocity, meshes with multiple components, or patients with physiological data. 
Managing such systems using only primitive types and scattered functions quickly becomes unmanageable.

**Object-Oriented Programming (OOP)** offers a powerful paradigm for organizing and structuring code in a way that closely reflects the real-world systems we aim to describe. 
Instead of focusing solely on procedures and functions (as in _procedural programming_), OOP centers around **objects—self-contained units** that **combine data** (state) and **behavior** (methods) into a single logical entity.

At its core, Object-Oriented Programming is based on the concept of objects, which are instances of classes. A class is like a blueprint—it defines what an object knows (its attributes, or fields) and what it can do (its methods, or functions associated with that class).

For example, consider modeling a Particle:
- It has **data**: position, velocity, mass
- It has **behavior**: move, accelerate, collide

Instead of managing separate arrays for all particle positions and velocities, OOP allows you to define a **Particle class** and create **multiple objects**, each representing an independent particle with its own state and behavior.

OOP is especially useful in large, collaborative, or complex projects. Here’s why:
- **Encapsulation**: Combines data and methods into single units, preventing unintended interactions and simplifying debugging.
- **Abstraction**: Hides implementation details and exposes only what is necessary to the outside world.
- **Modularity**: Each class can be developed, tested, and maintained independently.
- **Reusability**: Classes can be reused across different projects or within the same project, reducing code duplication.
- **Extensibility**: New functionality can be added without modifying existing code, through inheritance and polymorphism.

These advantages make OOP not only a tool for clean and maintainable code, but also a **mental model** to decompose real-world problems into structured software designs.

Before diving into code examples, let’s briefly outline the fundamental concepts that make up object-oriented programming:
- **Class**: A blueprint that **defines** the **structure** and **behavior** of objects.
- **Object**: An **instance of a class**. Each object has its own state and shares the behavior defined by the class.
- **Encapsulation**: Restricting direct access to some components of an object, enforcing access through well-defined interfaces.
- **Inheritance**: Mechanism by which **one class** can **inherit** **fields** and **methods** from another, **promoting code reuse.**
- **Polymorphism**: Ability to **use a common interface** for different underlying types.

## Example in metacode

Let's see a metacode example that employes the following class:
```Plain
CLASS Particle
    PROPERTIES:
        position  // vector [x, y, z]
        velocity  // vector [vx, vy, vz]
        mass      // scalar

    METHODS:
        move(this, dt):
            this.position = this.position + this.velocity * dt

        apply_force(this, force, dt):
            acceleration = force / this.mass
            this.velocity = this.velocity + acceleration * dt
END CLASS
```
and the main code is:
```Plain
// Create two particles
p1 = class(Particle)
p1.position = [0, 0, 0]
p1.velocity = [1, 0, 0]
p1.mass = 1.0

p2 = class(Particle)
p2.position = [0, 1, 0]
p2.velocity = [0, -1, 0]
p2.mass = 2.0

// Apply a force to particle 1
force = [0, 10, 0]
p1.apply_force(force, dt = 0.1)

// Move both particles
p1.move(dt = 0.1)
p2.move(dt = 0.1)
```

## Inheritance and Polymorphism: Extending the Particle Class

Imagine we want to define different kinds of particles, for example, `ChargedParticles` that carry an electric charge in addition to mass, position, and velocity.

### Inheritance

**Inheritance** allows us to create a **new class** **based on an existing one**, reusing its properties and methods. The new class (called a subclass) can also **introduce new attributes** or **override methods**.

We’ll define ChargedParticle as a subclass of Particle:
```Plain
CLASS Particle
    PROPERTIES:
        position    // [x, y, z]
        velocity    // [vx, vy, vz]
        mass        // scalar

    METHODS:
        move(this, dt):
            this.position = this.position + this.velocity * dt

        apply_force(this, force, dt):
            acceleration = force / this.mass
            this.velocity = this.velocity + acceleration * dt
END CLASS
```
Now we extend it:
```Plain
CLASS ChargedParticle EXTENDS Particle
    PROPERTIES:
        charge    // scalar

    METHODS:
        apply_electric_field(this, E_field, dt):
            // F = qE, Newton’s second law: a = F/m = qE/m
            acceleration = this.charge * E_field / this.mass
            this.velocity = this.velocity + acceleration * dt
END CLASS
```

### Polymorphism

Polymorphism allows us to **write code** that **works with the base class**, but automatically **invokes the correct behavior** for a **derived class**.

Suppose we write a routine:
```Plain
SUBROUTINE apply_motion(particle, dt)
    particle.move(dt)
END SUBROUTINE
```
Thanks to polymorphism, we can now call `apply_motion` with either a `Particle` or a `ChargedParticle`, and it will behave correctly — because a `ChargedParticle` is a `Particle`.


#### Usage Example
```Plain
// Create a regular particle
p1 = class(Particle)
p1.position = [0, 0, 0]
p1.velocity = [1, 0, 0]
p1.mass = 1.0

// Create a charged particle
p2 = class(ChargedParticle)
p2.position = [0, 0, 0]
p2.velocity = [0, 1, 0]
p2.mass = 2.0
p2.charge = 1.6e-19

// Apply an electric field
E = [0, 0, 10^5]
p2.apply_electric_field(E, dt = 0.01)

// Move both particles
p1.move(dt = 0.01)
p2.move(dt = 0.01)

// Apply a generic motion routine
apply_motion(p1, dt = 0.01)
apply_motion(p2, dt = 0.01)
```

> [!NOTE]
> - **Inheritance** lets you define new classes by extending existing ones.
> - **Polymorphism** lets you write general-purpose code that works with multiple types of related objects.
> - In our example:
>     - ChargedParticle inherited properties and methods from Particle.
>     - It added a new method apply_electric_field.
>     - It could be used wherever a Particle was expected.

## Classes in C
C does not support classes — at least, not in the way that object-oriented languages like C++, Java, or Python do. C is a procedural programming language, not an object-oriented one.
But wait. That doesn’t mean you can’t mimic class-like behavior. You can:
1.	Use `struct` to group data (like object properties).
2.	Write functions that operate on those structs (like methods).
3.	Use **function pointers** inside structs to get very close to methods and polymorphism.

Let's see an example code:

```C
#include <stdio.h>
#include <stdlib.h>

// Define a struct that simulates a "class"
typedef struct Particle {
    double position[3];   // Holds x, y, z positions
    double velocity[3];   // Holds vx, vy, vz velocities
    double mass;          // Mass of the particle

    // Function pointers to simulate "methods"
    void (*move)(struct Particle*, double);
    void (*apply_force)(struct Particle*, double[3], double);
} Particle;

// ========================
// Method: move
// Updates position based on velocity and time step
void move(Particle* p, double dt) {
    for (int i = 0; i < 3; i++) {
        p->position[i] += p->velocity[i] * dt;
    }
}

// ========================
// Method: apply_force
// Updates velocity based on external force and time step
void apply_force(Particle* p, double force[3], double dt) {
    for (int i = 0; i < 3; i++) {
        double acceleration = force[i] / p->mass;
        p->velocity[i] += acceleration * dt;
    }
}

// ========================
// "Constructor" for a Particle
// Allocates memory, initializes values, and sets function pointers
Particle* new_particle(double pos[3], double vel[3], double mass) {
    Particle* p = malloc(sizeof(Particle));
    for (int i = 0; i < 3; i++) {
        p->position[i] = pos[i];
        p->velocity[i] = vel[i];
    }
    p->mass = mass;
    p->move = move;               // Assign method implementations
    p->apply_force = apply_force;
    return p;
}

// ========================
// Main: create and manipulate particles
int main() {
    double pos[3] = {0.0, 0.0, 0.0};
    double vel[3] = {1.0, 0.0, 0.0};
    Particle* p = new_particle(pos, vel, 1.0);

    double force[3] = {0.0, 10.0, 0.0};
    p->apply_force(p, force, 0.1); // Call method via function pointer
    p->move(p, 0.1);               // Call method via function pointer

    printf("p position: [%.2f, %.2f, %.2f]\n", p->position[0], p->position[1], p->position[2]);

    free(p); // Free allocated memory
    return 0;
}
```

> [!NOTE]
> - **No method binding**: In C, `struct` is just a data container. It doesn’t support defining behavior (i.e., functions) inside it. There’s no concept of binding functions to data by default.
> - **No `this` keyword**: C lacks the concept of `this` (or `self`), which refers to the current instance of a class. This is crucial in OOP for accessing instance attributes inside methods.
> - **No support for inheritance or virtual dispatch**: You can’t override or dynamically resolve method behavior based on type in C, unless you simulate it manually with function pointers.

C structs only hold data, not behavior. But with function pointers, you can emulate OOP patterns — at the cost of clarity, safety, and compile-time checks. This workaround gives you flexibility, but also puts all responsibility on the programmer.
If you need proper classes and methods, use C++.

## Classes in Fortran
In Modern Fortran (90 and beyond), the language **does support object-oriented programming** natively. You can define `derived types` with `procedures`, use encapsulation, type-bound procedures, and even inheritance with extends.

Let’s now compare the two approaches by providing the equivalent of the “Particle class” in Fortran, followed by a breakdown of differences.

```Fortran
module particle_module
  implicit none
  private
  public :: Particle

  type :: Particle
    real :: position(3)
    real :: velocity(3)
    real :: mass
  contains
    procedure :: move
    procedure :: apply_force
  end type Particle

contains

  subroutine move(this, dt)
    class(Particle), intent(inout) :: this
    real, intent(in) :: dt
    this%position = this%position + this%velocity * dt
  end subroutine move

  subroutine apply_force(this, force, dt)
    class(Particle), intent(inout) :: this
    real, intent(in) :: force(3), dt
    real :: acceleration(3)
    acceleration = force / this%mass
    this%velocity = this%velocity + acceleration * dt
  end subroutine apply_force

end module particle_module
```
and the main code is:
```Fortran
program main
  use particle_module
  implicit none

  type(Particle) :: p1, p2
  real :: dt
  real :: force(3)

  dt = 0.1
  p1%position = [0.0, 0.0, 0.0]
  p1%velocity = [1.0, 0.0, 0.0]
  p1%mass     = 1.0

  p2%position = [0.0, 1.0, 0.0]
  p2%velocity = [0.0, -1.0, 0.0]
  p2%mass     = 2.0

  force = [0.0, 10.0, 0.0]

  call p1%apply_force(force, dt)
  call p1%move(dt)
  call p2%move(dt)

  print *, "p1 position:", p1%position
  print *, "p2 position:", p2%position
end program main
```
While you can emulate OOP in C, it’s manual, verbose, and lacks many features (like inheritance, encapsulation, polymorphism).
In Modern Fortran, OOP is first-class and integrated directly into the language. For scientific computing, Fortran’s OOP features are often sufficient and readable without losing performance.

### Inheritance and Polymorphism in Fortran
Here’s the equivalent Fortran implementation of the `Particle` and `ChargedParticle` classes using type extension (inheritance) and polymorphism.

This is the module that contains the class `Particle':
```Fortran
! particle_module.f90
module particle_module
    implicit none

    ! Base class
    type :: Particle
        real :: position(3)
        real :: velocity(3)
        real :: mass
    contains
        procedure :: move
        procedure :: apply_force
    end type Particle

contains

    subroutine move(this, dt)
        class(Particle), intent(inout) :: this
        real, intent(in) :: dt
        this%position = this%position + this%velocity * dt
    end subroutine move

    subroutine apply_force(this, force, dt)
        class(Particle), intent(inout) :: this
        real, intent(in) :: force(3)
        real, intent(in) :: dt
        real :: a(3)
        a = force / this%mass
        this%velocity = this%velocity + a * dt
    end subroutine apply_force

end module particle_module
```
And this is the module that contains the class `ChargedParticle':
```Fortran
! charged_particle_module.f90
module charged_particle_module
    use particle_module
    implicit none

    ! Derived class
    type, extends(Particle) :: ChargedParticle
        real :: charge
    contains
        procedure :: apply_electric_field
    end type ChargedParticle

contains

    subroutine apply_electric_field(this, E_field, dt)
        class(ChargedParticle), intent(inout) :: this
        real, intent(in) :: E_field(3)
        real, intent(in) :: dt
        real :: a(3)
        a = this%charge * E_field / this%mass
        this%velocity = this%velocity + a * dt
    end subroutine apply_electric_field

end module charged_particle_module
```
The main code is:
```Fortran
! main.f90
program main
    use particle_module
    use charged_particle_module
    implicit none

    class(Particle), allocatable :: p1
    class(Particle), allocatable :: p2  ! Polymorphic pointer to ChargedParticle

    real :: E(3) = [0.0, 0.0, 1.0e5]
    real :: dt = 0.01

    ! Allocate and initialize a Particle
    allocate(Particle :: p1)
    p1%position = [0.0, 0.0, 0.0]
    p1%velocity = [1.0, 0.0, 0.0]
    p1%mass = 1.0

    ! Allocate and initialize a ChargedParticle
    allocate(ChargedParticle :: p2)
    select type (cp => p2)
    type is (ChargedParticle)
        cp%position = [0.0, 0.0, 0.0]
        cp%velocity = [0.0, 1.0, 0.0]
        cp%mass = 2.0
        cp%charge = 1.6e-19

        call cp%apply_electric_field(E, dt)
    end select

    ! Move both particles (polymorphic call to `move`)
    call p1%move(dt)
    call p2%move(dt)

    print *, 'p1 position =', p1%position
    print *, 'p2 position =', p2%position
end program main
```

Let's break down the code:
- `type, extends(Particle) :: ChargedParticle` creates a new class that **inherits** from `Particle`.
- `class(Particle), allocatable :: p2` allows `p2` to point to any type extending `Particle`, enabling polymorphism.
- `select type(...)` allows us to **downcast** and access `ChargedParticle`-specific components.
- Methods like `move` and `apply_force` are **invoked dynamically** depending on the **actual type** of the object at runtime — this is runtime polymorphism in action.

> [!NOTE]
> Modern Fortran (from 2003 onward) supports all the major object-oriented features: encapsulation, inheritance, polymorphism, and even type-bound procedures with dynamic dispatch.
> Fortran 90 is not enough if you want to do full object-oriented programming with inheritance and polymorphism. With F90 you get **encapsulation** of data (via types) and **modular design** using `MODULE`, but there is no inheritance, no polymorphism and no type-bound procedures (you can’t do `p%move()`). Fortran 2003 introduced true object-oriented programming, including all of them.
>
> Most modern compilers (like `gfortran`, `ifort`, `nvfortran`, etc.) automatically support Fortran 2003 features by default — you don’t need a special flag just to enable the language version.
