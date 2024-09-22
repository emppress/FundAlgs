#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <string.h>

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

status string_to_double(const char *str, double *result);
status sum_a(double x, double eps, double *result);
status sum_b(double x, double eps, double *result);
status sum_c(double x, double eps, double *result);
status sum_d(double x, double eps, double *result);

#endif