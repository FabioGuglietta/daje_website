# File Input/Output: Reading and Writing Files
Consider the following task: we want to read from an external file some $x$ and $y$ coordinates. The file is organised as follow:

We have some particles made of discrete points. Each particle has $N_p$ points with $x,y,z$ coordinates. Such coordinates are included in a `txt` file like this:

```plaintext
number_of_coordinates name_dataset
point_id coord_x coord_y coord_z
point_id coord_x coord_y coord_z
point_id coord_x coord_y coord_z
...
```

For example

```plaintext
4 Particle_A
1 1.0 2.0 3.0
2 4.0 5.0 6.0
3 7.0 8.0 9.0
4 10.0 11.0 12.0
```

is a particle made of 4 points with $(x,y,z)$-coordinates. The name of the particle is `Particle_A`.

We want to write a program that can be executed like this

```
./test.exe input_file output_file
```

that is, we want to specify from command line the name of the input and output files. 

The code will read the coordinates of the input file and will compute the centroid of the particle.

Here the pseudo code:

```plaintext
FUNCTION calculate_centroid(points_x, points_y, points_z, n) 
	centroid_x=0; centroid_y=0; centroid_z=0;
    FOR i = 0, n-1
        centroid_x += points_x[i];
        centroid_y += points_y[i];
        centroid_z += points_z[i];
    centroid_x /= n; centroid_y /= n; centroid_z /= n;
	 RETURN centroid_x, centroid_y, centroid_z

MAIN PROGRAM

	 READ(input_file,points_x,points_y,points_z) //Read the input_file and store the n coordinates in points_x, points_y, points_z
	
	centroid_x,centroid_y,centroid_z = calculate_centroid(points_x, points_y, points_z, n) 
	
	WRITE(output_file,"Set name", set_name)
	WRITE(output_file,"Number of points", n)
	WRITE(output_file,"The centroid of the points is at", centroid_x,centroid_y,centroid_z)
	WRITE(output_file,"Coordinates of the points:")
	FOR i = 0, n-1
		WRITE("Point:", points_id[i], points_x[i], points_y[i], points_z[i])
	
	END PROGRAM
```

Here is the C code:

```C
#include <stdio.h>
#include <stdlib.h>

// Define a structure to store 3D points with additional fields (id and name)
struct Point3D {
  int id;         // Point identifier
  double x;       // x-coordinate
  double y;       // y-coordinate
  double z;       // z-coordinate
};

// Function to calculate the centroid of an array of 3D points
struct Point3D calculate_centroid(struct Point3D *points, int n) {
  struct Point3D centroid;
  centroid.x = 0.0;
  centroid.y = 0.0;
  centroid.z = 0.0;

  for (int i = 0; i < n; i++) {
    centroid.x += points[i].x;
    centroid.y += points[i].y;
    centroid.z += points[i].z;
  }

  centroid.x /= n;
  centroid.y /= n;
  centroid.z /= n;

  return centroid;
}

int main(int argc, char *argv[]) {
  // Check if less (or more) then 3 arguments are given in command line
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
    return 1;
  }

  // Open input file for reading
  FILE *input_file = fopen(argv[1], "r");
  if (input_file == NULL) { // Check open
    printf("Error opening input file");
    return 1;
  }

  // Open output file for writing
  FILE *output_file = fopen(argv[2], "w");
  if (output_file == NULL) { // Check open
    printf("Error opening output file");
    fclose(input_file); // Close input_file before exit
    return 1;
  }

  // Read the number of points and name of the set from the input file
  int n;
  char set_name[100]; // Name of the dataset
  fscanf(input_file, "%d %s", &n, set_name);

  // Allocate memory for an array of 3D points
  struct Point3D *points = (struct Point3D *)malloc(n * sizeof(struct Point3D));

  // Read points data (ID, x, y, z) from the input file
  for (int i = 0; i < n; i++) {
    fscanf(input_file, "%d %lf %lf %lf", &points[i].id, &points[i].x, &points[i].y, &points[i].z);
  }

  // Calculate the centroid of the points
  struct Point3D centroid = calculate_centroid(points, n);

  // Write the results to the output file
  fprintf(output_file, "Set Name: %s\n", set_name);
  fprintf(output_file, "Number of points: %d\n", n);
  fprintf(output_file, "The centroid of the points is at (%.2f, %.2f, %.2f)\n", centroid.x, centroid.y, centroid.z);
  fprintf(output_file, "Data of the points:\n");

  for (int i = 0; i < n; i++) {
    fprintf(output_file, "Point %d: (%.2f, %.2f, %.2f)\n", points[i].id, points[i].x, points[i].y, points[i].z);
  }

  // Clean up
  free(points);
  fclose(input_file);
  fclose(output_file);

  printf("Centroid calculation complete. Results written to %s\n", argv[2]);

  return 0;
}
```

Let's look closer, starting from `main`

# FILE

## Open a file
This line

```C
FILE *input_file = fopen(argv[1], "r");
```
involves opening a file for reading.	`FILE` is a special `type` in C (a so-called "structure", more on this later) that is used to handle files. The `*` indicates that `input_file` is a pointer to a `FILE` structure. This pointer will be used to interact with the file (e.g., reading or writing).

The `fopen()` function opens a file. It takes two arguments:

1. The first argument (`argv[1]`) is the name of the file to open. `argv[1]` is coming from the command-line arguments, meaning the program expects the user to provide the filename as the second argument when they run the program.
2. The second argument (`"r"`) tells fopen to open the file in read mode (`"r"` stands for read). This means the program can read the content of the file, but cannot write or modify it.

> [!NOTE]
> The second argument of `fopen` can be:
> - `r`: Open for reading. The file must exist.
> - `w`: Open for writing. Creates an empty file or truncates an existing file.
> - `a`: Open for appending. Writes data at the end of the file. Creates the file if it does not exist.
> - `r+`: Open for reading and writing. The file must exist.
> - `w+`: Open for reading and writing. Creates an empty file or truncates an existing file.
> - `a+`: Open for reading and appending. The file is created if it does not exist.

If the file exists and opens successfully, `fopen()` returns a pointer to the `FILE`, which is assigned to `input_file`. If the file can’t be opened (for example, if it doesn’t exist), `fopen()` returns `NULL`, and you’d typically check for that to handle errors:

```C
  if (input_file == NULL) { // Check open
    printf("Error opening input file");
    return 1;
  }
```

> [!WARNING]
> `fopen()` and `open()` are two different functions, and you should know when to use which:
> 1. Binary mode: With `fopen()` you can choose whether you want to read in text mode (by default) or in binary mode using the "b" flag. In contrast, `open()` treats all files as binary by default.
> 2. High-level vs. Low-level: `fopen()` is higher-level and provides more functionality than `open()` like formatted I/O (`fprintf`, `fscanf`). `open()`, instead, is a system call (i.e., it's part
>    of the kernel) and provides low-level, direct control over file access, but you will have to write your own routines to read/write specific formats.
> 4. Buffering: `fopen()` buffers I/O operations, meaning that it stores data in memory before sending it to or reading it from the actual file. This can improve performance for reading and writing files, particularly for larger files.
    
Example:



Similarly, this line

```C
FILE *output_file = fopen(argv[2], "w");
```

is used to open a file for writing. The first argument of `fopen()` is again the name of the file to open (`argv[1]` is the second argument passed by the command-line). The second argument is now the `"w"`, flag, which stands for write. 

## Close a file
This line: 

```C
fclose(input_file);
```

is used to close a file that was previously opened with `fopen()`. 
`fclose()` is a standard library function in C that closes a file stream. It takes one argument, which is a pointer to the `FILE` object that represents the open file (in our example, `input_file` is the pointer to the file that was opened for reading).

You should call `fclose()` when you are done working with the file. This includes after you have finished reading from or writing to the file and before the program exits or before opening another file. 

> [!TIP]
> Why Close a File?: 
> - **Resource Management:** Each open file uses system resources. Closing a file frees up these resources and allows the operating system to manage them efficiently.
> - **Data Integrity:** For files that are being written to, closing the file ensures that all data is properly written and saved. Even though input_file is opened in read mode in this case, it is still good practice to close files when done.
> - **Preventing Memory Leaks:** Failing to close files can lead to memory leaks and resource exhaustion, which can affect the stability of the program or the system.

> [!WARNING]
> Although it might sound counterintuitive, closing a file with `fclose()` does not necessarily flush the content to file. The operating system decides when it's time to do that.
> What is guaranteed is that by opening the file for writing/appending *after* calling `fclose()`, the order of write operations will be preserved. This is not guaranteed if you are writing
> on a file which has been opened and is being written to by two functions. The behaviour in this case is unpredictable. 

## Read from a file

The line:

```C
fscanf(input_file, "%d %s", &n, set_name);
```

is used to read formatted input from a file. 

`fscanf()` is a standard library function used to read data from a file. It works similarly to `scanf()`, but instead of reading from the standard input (keyboard), it reads from a file stream.

It takes these arguments: 

1. File pointer (`input_file`, in our example): This is the pointer to the FILE object that represents the open file from which data will be read. It was previously obtained using `fopen()`.
2. Format string (`"%d %s"`, in our example): This is the format string that specifies how the input data should be interpreted. `"%d"` tells `fscanf` to read an integer from the file, while `"%s"` tells `fscanf` to read a string (a sequence of characters) from the file. It will read until it encounters a whitespace character.
3. Addresses of variables where the read data will be stored (`&n, set_name`): &n is the address of the integer variable n. `fscanf` will read an integer value from the file and store it at this address. `set_name` is a character array (or string) where the string read from the file will be stored. The `fscanf` function will copy the characters into this array until it encounters a whitespace.

> [!WARNING]
> In C, when using functions like `fscanf()` that require **addresses** of variables to store input data, the use of the address-of operator (`&`) is necessary for some types and not for others. `&var` is used to get the memory address of `var`.
> In our example, `n` is an integer variable: when using `fscanf()` (or similar functions), you need to provide the **address** of `n` so that `fscanf()` can store the read value directly into `n`. 
> 
> In our example, `set_name` is a character array (or string). In C, the name of an array (like `set_name`) automatically represents the **address** of the first element of the array.
> When passing `set_name` to `fscanf()`, you are providing a **pointer** to the beginning of the array. `fscanf()` will use this pointer to write the string data directly into the array.
