# Pointers and Arrays 

## 1D arrays and pointers

We now consider the following code:

```C
#include <stdio.h>

int main(){
  int arr1D[5]    = {1,2,3,4,5};
  int  *p1     =  arr1D;

  printf("-------------------------------\n");
  printf("1D array and pointer\n");
  printf("-------------------------------\n");
  printf("Number of element of arr1D = %d \n",  sizeof(arr1D)/sizeof(int));
  printf("Sizeof(arr1D) = %d Bytes\n\n",  sizeof(arr1D));

  printf("Addresses:\n");
  printf("arr1D       = %d\n", arr1D);       //Points to element 0
  printf("&arr1D[0]   = %d\n", &arr1D[0]);   //Points to element 0
  printf("&arr1D      = %d\n", &arr1D);      //Points to the whole array
  printf("p1          = %d\n\n", p1);        //Points to the element 0

  printf("&arr1D[1]   = %d\n\n", &arr1D[1]); //Points to element 1
                                             
  printf("Summing +1:\n");
  printf("arr1D    +1 = %d\n", arr1D+1);     
  printf("&arr1D[0]+1 = %d\n", &arr1D[0]+1); 
  printf("&arr1D   +1 = %d\n\n", &arr1D+1);    
  printf("p1       +1 = %d\n\n", p1+1);
}
```
whose output is
```plaintext
-------------------------------
1D array and pointer
-------------------------------
Number of element of arr1D = 5 
Sizeof(arr1D) = 20 Bytes

Addresses:
arr1D       = 1832891120
&arr1D[0]   = 1832891120
&arr1D      = 1832891120
p1          = 1832891120

&arr1D[1]   = 1832891124

Summing +1:
arr1D    +1 = 1832891124
&arr1D[0]+1 = 1832891124
&arr1D   +1 = 1832891140

p1       +1 = 1832891124

```
The array `arr1D` has 5 elements: since each of them is an integer (4 Bytes), the total size of `arr1D` is $5\times 4$ Bytes = 20 Bytes. 

Notice that `arr1D` and `&arr1D[0]` both point to the 0th element of the array `arr1D`. Thus, **the name of an array is itself a pointer** to the 0th element of the array. Here, both point to the first element, which has a size of 4 Bytes. When you add 1 to them, they now point to the element with index 1 of the array (i.e., `&arr1D[1]`), this resulting in an address increment of 4 Bytes.

On the other hand, `&prime` is a pointer to an array of 5 integers. It holds the base address of the array `arr1D[5]`, which is the same as the address of the first element. Therefore, increasing by 1 results in an address increment of 5 x 4 = 20 Bytes.

> [!NOTE]
> The name of an array is itself a pointer to the 0th element of the array, and if we increase it by one, we move to the address of the next element. On the other hand, `&arr1D` is a pointer to the whole array, and increasing it by one results in an address increment of `sizeof(arr1D)`.
>
> In short, `arr` and `&arr[0]` point to the 0th element, while `&arr` points to the entire array.

We can access the elements of the array using indexed variables like this:

```C
int arr = {5,10,15,20,25};
for (int i = 0; i < 5; i++){
  printf("index = %d, address = %d, value = %d\n", i, &arr[i], arr[i]);
}
```

or we can do the same thing using pointers, which are generally faster than using indexing:

```C
int arr = {5,10,15,20,25};
for (int i = 0; i < 5; i++){
  printf("index = %d, address = %d, value = %d\n", i, arr+i, *(arr+i));
}
```

In both cases, the output is:
```
index = 0, address = 845682720, value = 1
index = 1, address = 845682724, value = 2
index = 2, address = 845682728, value = 3
index = 3, address = 845682732, value = 4
index = 4, address = 845682736, value = 5
```

> [!TIP]
> Accessing the elements of the array using pointers is faster. If you don't believe... try!

## 2D arrays and pointers

In this section, we consider 2D arrays (but the discussion can be easily extended to multidimensional arrays). 

Suppose we want to allocate an array to represent the 3 components of the velocity for each of the $N$ particles. 
To allocate it dynamically, we must use pointers:
```C
arr = (float **)malloc(N*sizeof(float *));
for (int i = 0; i < N; i++) {
  arr[i] = (float *)malloc(3 * sizeof(float));
}
```
Here, `arr` is declared as a pointer to a pointer to a float. This type of pointer is used to represent a 2D array because, in C, a 2D array is essentially an **array of arrays**. Each pointer in the first dimension (`arr[i]`) will point to a one-dimensional array of floats (the columns).
To allocate the first dimension (rows), we have used:
```C
arr = (float **)malloc(N * sizeof(float *));
```
This line allocates memory for `N` pointers to floats, where `N` represents the number of rows. The `malloc` function dynamically allocates memory, and `sizeof(float *)` ensures that the correct amount of memory is reserved for each pointer. Since `arr` is a pointer to a pointer, it needs memory for storing `N` pointers that will later point to the actual rows of floats.

> [!NOTE]
> The `(float **)` in front of the `malloc()` call is a **type cast**. Indeed, `malloc()` returns a pointer of type `void *`. A `void *` pointer is a generic pointer that can point to any type of data, but it doesn’t carry type information itself. Therefore, when you assign the result of `malloc()` to a variable of a specific pointer type (like `float **` in your case), you need to cast the `void *` pointer to the appropriate type so that the compiler knows how to interpret it. So, the `(float **)` cast is used to explicitly convert the `void *` returned by `malloc()` into a `float **`, which is necessary for the variable `arr`.

To allocate the second dimension (columns), we have used:
```C
for (int i = 0; i < N; i++) {
    arr[i] = (float *)malloc(3 * sizeof(float));
}
```
This loop allocates memory for each row. For each `i` from `0` to `N-1`, `arr[i]` is allocated memory to hold 3 float values. This creates a row with 3 columns for each row in the 2D array. The `sizeof(float)` ensures that the correct amount of memory is allocated for each float. The number 3 here specifies the number of columns in each row.

To summarise, we created a pointer (`arr`) that points to an **array of pointers**. Each of these pointers will, in turn, point to an **array of floats**. The first call to `malloc` allocates enough memory to hold `N` pointers, one for each row. This is why arr is of type `float **`. For each row, the second `malloc` allocates memory for 3 floats. These floats represent the columns in each row, creating the second dimension of the array.

> [!WARNING]
> **Memory Management:** Since you’re **dynamically allocating memory**, it’s essential to **free** the allocated memory later to avoid **memory leaks**. This would be done using `free()` for each row and finally for the array of pointers:
> ```C
> for (int i = 0; i < N; i++) {
>    free(arr[i]);  // Free each row
> }
> free(arr);  // Free the array of pointers
> ```


The follwing example shows also how to pass this 2D array to a function:

```C
#include <stdio.h>
#include <stdlib.h>

void func(float **arr, int dim1, int dim2){

  for(int i = 0; i<dim1; i++){
    for(int j = 0; j<dim2; j++){
      *(*(arr+i)+j) = i*dim2+j;
    }
  }
}

int main(){

  int const N = 10;
  float **arr;

  arr = (float **)malloc(N*sizeof(float *));
  for (int i = 0; i < N; i++) {
    arr[i] = (float *)malloc(3 * sizeof(float));
  }

  func(arr, N, 3);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < 3; j++) {
    printf("%d: %f\n",i, *(*(arr+i)+j));
    }
  }

  for (int i = 0; i < N; i++) {
    free(arr[i]);  // Free each row
  }
  free(arr);  // Free the array of pointers

  return 0;
}
```

Note that in the function `func` we are using **pointers arithmetics**: the expression `*(*(arr + i) + j)` is used to access the elements of a 2D array:

- `arr` is a pointer to a pointer. Essentially, `arr` is a pointer to the first element of an array of pointers, where each pointer points to the first element of a row in the 2D array.
- `arr + i` advances the pointer `arr` by `i` positions. Since `arr` is a pointer to pointers `(float **arr)`, `arr + i` moves the pointer `i` steps ahead, pointing to the i-th row of the array.
- `*(arr + i)` dereferences the pointer to the i-th row, i.e., it accesses the pointer that points to the first element of the i-th row. In simpler terms, `*(arr + i)` gives you the address of the first element in the i-th row.
- `*(arr + i) + j`: from the address of the i-th row (which is itself a pointer), you move `j` positions ahead to reach the j-th element of the i-th row. `*(arr + i) + j` gives you a pointer to the element at position `arr[i][j]`.
- *(*(arr + i) + j): finally, the outer `*` dereferences the pointer obtained from the previous step, giving you the actual value of the element at position `arr[i][j]`.

![plot](fig/arr_ptr.png)
