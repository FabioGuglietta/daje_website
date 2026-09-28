# Pass by reference and by value

Functions can be invoked in two ways: **Call by Value** or **Call by Reference**. The difference lies in how the parameters are passed to the function.

The arguments provided to the function are known as **actual parameters**, while the variables used inside the function are called **formal parameters**.

## Call by Value
In the **call by value** method, the values of the actual parameters are copied into the function's formal parameters. This means **two separate copies** of the parameters are stored in memory: one for the original values and one for the function's use. Since changes are made only to the function’s local copy, any modifications inside the function do not affect the actual parameters in the calling code.

For example:

```C
#include <stdio.h>

float add_by_value(float x, float y){ //Formal arguments
  x = 100; // Changing the values of the formal arguments does not 
  y = 200; // change the values of the actual arguments
  return x+y;
}

int main(){

  float a = 1.3;
  float b = 2.5;
  float res = 0.;

  //By value
  printf("Pass by value:\n");
  printf("Expected: a = %.1f, b = %.1f, a + b = %.1f\n", a,b,a+b);
  res = add_by_value(a,b); // Actual arguments
  printf("Result  : a = %.1f, b = %.1f, a + b = %.1f\n\n", a,b,res);

  return 0;
}
```

The output is

```plaintext
Pass by value:
Expected: a = 1.3, b = 2.5, a + b = 3.8
Result  : a = 1.3, b = 2.5, a + b = 300.0
```

In the above example, we invoke the function `add_by_value` passing `float a = 1.3` and `float b = 2.5` (actual arguments). The function takes its formal arguments (`x` and `y`), change their _local_ value and return their sum.
If we print the values of `a` and `b` after the call to the function, we can see that their value is not changed. Indeed, the function `add_by_value` changed the values of the (local) formal arguments. 

## Call by Reference

In the **call by reference** method, the **address** of the actual parameters is passed to the function as **formal parameters**. In C, this is done using **pointers**.

Since both the **actual** and **formal** parameters point to the **same memory location**, any changes made inside the function **directly affect the original values** in the calling code. 
This allows the function to modify the actual parameters.

Let's now look at this example:
```C
#include <stdio.h>

float add_by_reference(float *x, float *y){ //Formal arguments
  *x = 100;
  *y = 200;
  return *x+*y;
}

int main(){

  float a = 1.3;
  float b = 2.5;
  float res = 0.;

  //By reference
  printf("Pass by reference:\n");
  printf("Expected: a = %.1f, b = %.1f, a + b = %.1f\n", a,b,a+b);
  res = add_by_reference(&a,&b);
  printf("Result  : a = %.1f, b = %.1f, a + b = %.1f\n", a,b,res);
  return 0;
}
```
The output is
```plaintext
Pass by reference:
Expected: a = 1.3, b = 2.5, a + b = 3.8
Result  : a = 100.0, b = 200.0, a + b = 300.0
```

On the contrary of the previous example, the values of `a` and `b` result changed after invoking the function `add_by_reference`. Note also that this function takes as an input the **address** of the varaible, NOT its value. Indeed, the function is defined as `float add_by_reference(float *x, float *y)`, that means `x` and `y` are pointers to `float`. Note also that if we want to change the values of `x` and `y` (which are pointers), we must dereference them. 

# Summarise
In C, functions can handle parameters in two ways: 

- **Call by Value**: A **copy** of the actual parameter’s value is passed to the function. The **function works with this copy**, so any changes made inside the function **do not affect** the original variable.
-	**Call by Reference**: Instead of passing the value, the **address** of the actual parameter is passed, using **pointers**. This allows the function to **modify the original variable**.

> [!TIP]
> - Use **call by value** when you want to protect the original values from being modified.
> - Use **call by reference** when you want to modify the caller’s variable or work with large data structures efficiently.

> [!WARNING]
> - **Call by value** can use more memory for large data structures since the values are copied.
> - **Call by reference**: be careful when using pointers; incorrect handling may lead to memory corruption or unexpected behavior. Always ensure pointers are properly initialized and handle memory carefully to avoid issues like segmentation faults.
