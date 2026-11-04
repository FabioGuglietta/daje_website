#include <stdio.h>
#include "numutils.h"

int main(void) {
    double a[] = {1.0, 2.0, 3.0, 4.0};
    double b[] = {2.0, 0.0, 1.0, 3.0};
    int n = 4;

    printf("Mean of a: %.2f\n", num_mean(a, n));
    printf("Dot product: %.2f\n", num_dot(a, b, n));
    return 0;
}
