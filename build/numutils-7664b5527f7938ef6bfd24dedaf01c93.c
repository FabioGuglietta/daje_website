#include "numutils.h"

double num_mean(const double values[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += values[i];
    }
    return sum / n;
}

double num_dot(const double a[], const double b[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}
