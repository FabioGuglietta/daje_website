
# Structures

In the previous lectures, we introduced some types in C (e.g., `int`, `float`, `double`, `char`, etc.). 
However, there is the possibility to extend such types, by using the so-called ***structures***.

In C, a ***structure*** (or `struct`) is a user-defined data type that allows you to **group different types of variables** together. 
This is particularly useful when you want to represent more complex data types that involve multiple variables of different types. 
Structures help organize data logically and make the code more readable and maintainable.

Structures are ideal for **bundling different data types** together **under one name**, especially when representing real-world entities such as points, students, or complex numbers. For instance, instead of using separate variables for each property of a student (e.g., `name`, `age`, `grade`), you can group them into **a single structure**.

Look how the following code can be simplified by using structures:

```C
int main(){

	char   student_firstName[20];
	char   student_lastName[20];
	int    student_age;
	char   student_gender;
	double student_height;
	
	char   professor_firstName[20];
	char   professor_lastName[20];
	int    professor_age;
	char   professor_gender;
	double professor_height;
	
	student_firstName = "Ludwig";
	student_lastName  = "Boltzmann";
	student_age       = 26;
	student_gender    = 'M';
	...
	
	professor_firstName = "Josef";
	professor_lastName  = "Stefan";
	professor_gender    = 'M';
	...
	
	return 0;
}
```

We immediately see that the information `firstName`, `lastName`, `age`, etc., are common to both `student` and `professor`. We could therefore define a `struct` called, let's say, `info`
```C
struct info{
	char   firstName[20];
	char   lastName[20];
	int    age;
	char   gender;
	double height;
};   // note the semicolon here!
```

and use it like this:

```C
#include <stdio.h>
int main(void){
	struct info student = {"Ludwik", "Boltzmann", 26, 'm', 1.72}; // yes, it's a typo ;)
    	struct info professor = {"Josef", "Stefan", 35, 'm', 1.68};

    	// Structure elements can be assigned using the . operator
        student.age       = 28;
        // Since `firstName` and `lastName` are arrays, they cannot be reassigned, but you need to use `sprintf()`
	sprintf(student.firstName, "%s", "Ludwig"); // let's fix the typo
return 0;
}
```

> [!WARNING]
> arrays of `char` and pointers to `char` are not the same thing. In fact, we could have done the following to assign element-by-element both strings and numbers:
> ```C
> struct info{
>	char   *firstName, *lastName;
>	int    age;
>	char   gender;
>	double height;
> };
> int main(void) { 
>	struct student;
> 	student.firstName = "Ludwik"; // our usual typo, now stored in read-only memory
> 	student.lastName  = "Boltzmann";
> 	student.firstName = "Ludwig"; // this will have another address
> }
> ```
> This is valid for the assignment only, and it's not possible to change the content of `firstName` and `secondName` without
> occupying another bit of read-only memory. See the [section on strings](../Lecture_the_C_programming_language_basics/C_strings.md)

## Example

Let's look at this code:

```C
#include <stdio.h>
#include <stdlib.h>

// Define the structure for a 3D point
struct Point3D {
    int id;         // Point identifier
    double x;       // x-coordinate
    double y;       // y-coordinate
    double z;       // z-coordinate
};

// Function to initialize a point
void init_point(struct Point3D *p, int id, double x, double y, double z) {
    p->id = id;
    p->x = x;
    p->y = y;
    p->z = z;
}

// Function to print a point
void print_point(struct Point3D p) {
    printf("Point %d: (%.2f, %.2f, %.2f)\n", p.id, p.x, p.y, p.z);
}

int main() {
    int n = 3;
    struct Point3D *points;

    // Allocate memory for n points
    points = (struct Point3D *)malloc(n * sizeof(struct Point3D));
    if (points == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }

    // Initialize each point
    init_point(&points[0], 0, 1.0, 2.0, 3.0);
    init_point(&points[1], 1, 4.5, 0.0, -1.2);
    init_point(&points[2], 2, -3.3, 2.2, 1.1);

    // Print all points
    for (int i = 0; i < n; i++) {
        print_point(points[i]);
    }

    // Free the allocated memory
    free(points);
    return 0;
}
```

Let’s break it down:

1. **`struct Point3D *points`:**
   - `struct Point3D` is a structure type that represents a 3D point, with fields `id`, `x`, `y`, and `z`:
     ```C
	 struct Point3D {
	   int id;         // Point identifier
	   double x;       // x-coordinate
	   double y;       // y-coordinate
	   double z;       // z-coordinate
	 };
	 ```
   - In the `manin()`, `struct Point3D *points` declares a pointer to a `Point3D` structure. This means `points` will hold the address of a **dynamically allocated** block of memory where `Point3D` structures will be stored.

2. **`points = (struct Point3D *)malloc(n * sizeof(struct Point3D))`:**
   - `malloc()` is a standard library function used to allocate a block of memory. It returns a pointer to the beginning of the allocated memory. `n * sizeof(struct Point3D)` calculates the total amount of memory to allocate (`n` is the number of `Point3D` structures, `sizeof(struct Point3D)` gives the size (in bytes) of a single `Point3D` structure).
   - `(struct Point3D *)`: since the result of `malloc()` is a `void *` (which is a generic pointer type), you need to cast it to the appropriate type (`struct Point3D *`).  `(struct Point3D *)` is a type cast that converts the `void *` returned by `malloc()` into a `struct Point3D *`.

3. **`init_point()` Function**
   - The function `init_point()` is used to initialize a structure through a pointer. It takes a pointer to a `struct Point3D` and assigns values to its individual fields (`id`, `x`, `y`, and `z`). This approach is common in C because it allows the function to modify the original structure in memory, rather than a local copy. Passing a pointer is also more efficient, especially when dealing with large or dynamically allocated data.
   - Inside the function, the arrow operator `->` is used to access structure members through a pointer. The expression `p->x` is equivalent to `(*p).x`: the pointer `p` is first dereferenced (using `*`), and then the member `x` is accessed. The **arrow operator** simply **combines** these two operations into a single, **more readable** form.
   - To **call** this function, you must pass the **address** of a `Point3D` variable (or one element of a structure array):
     ```C
     struct Point3D p;
     init_point(&p, 1, 1.0, 2.0, 3.0);  // pass the address of p
	 ```
     Here, `&p` passes the **address** of the structure, and inside the function, `p->x` (or `p->id`, etc.) accesses and modifies its members directly. The same logic applies when initializing elements of a **dynamically allocated array**, e.g. `init_point(&points[i], ...)`.

## Difference between dot `.` and arrow `->` operators in C
In C, both the dot `.` and arrow `->` operators are used to access members of a structure. However, they are used in different contexts depending on whether you’re working with a **structure variable** or a **pointer to a structure**. 

The **dot operator** is used when you are working with structure variables directly. If you have a structure variable (i.e., the actual instance of the structure), you can access its members using the dot operator: `struct_name.member_name`.

```C
struct Point3D {
    double x, y, z;
};

struct Point3D point;

point.x = 1.0;
point.y = 2.0;
point.z = 3.0;

printf("x: %f, y: %f, z: %f\n", point.x, point.y, point.z);
```
In this example, point is a structure variable, so you use `point.x`, `point.y`, and `point.z` to access the members of the `Point3D` structure.

The **arrow operator** is used when you are working with a **pointer to a structure**. Since a pointer points to the memory address of the structure, you first need to dereference it to access the structure’s members. The arrow operator combines both dereferencing the pointer and accessing the member in one step: `pointer_to_struct->member_name`.

```C
struct Point3D {
    double x, y, z;
};

struct Point3D point;
struct Point3D *point_ptr = &point; // point_ptr is a pointer to the structure

point_ptr->x = 1.0;
point_ptr->y = 2.0;
point_ptr->z = 3.0;

printf("x: %f, y: %f, z: %f\n", point_ptr->x, point_ptr->y, point_ptr->z);
```
Here, point_ptr is a pointer to the structure point. You use `point_ptr->x`, `point_ptr->y`, and `point_ptr->z` to access the members of the structure via the pointer.

To summarise:
- Use the **dot** `.` operator for direct access to structure elements in an array.
- Use the **arrow** `->` operator when accessing members via a pointer.

```C
struct Point3D point;
struct Point3D *point_ptr = &point;
point.x = 1.0;       // Access structure members directly
point_ptr->x = 1.0;  // Access structure members through a pointer
```

> [!WARNING]
> In the function `Point3D calculate_centroid(struct Point3D *points, int n)`, `points` is a pointer to an array of Point3D structures, and `points[i]` gives the actual structure at index `i` (it is not a pointer). Since `points[i]` is a structure, you access its members using the dot operator `.`, like `points[i].x`.
> So, when you have an **array of structures** (like `points`), you access elements of the structure like `points[i].x`. When you have a **pointer to a structure**, then you use `ptr->x` to access the `x` member of the structure.
```
