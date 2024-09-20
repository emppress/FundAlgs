#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define PI 3.14159265358979323846

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

status string_to_double(const char *str, double *result);
status e_limit(double eps, double *result);
status pi_limit(double eps, double *result);
status ln2_limit(double eps, double *result);
status sqrt_limit(double eps, double *result);
status e_row(double eps, double *result);
status pi_row(double eps, double *result);
status ln2_row(double eps, double *result);
status sqrt_row(double eps, double *result);
status e_equation(double eps, double *result);
status pi_equation(double eps, double *result);
status ln2_equation(double eps, double *result);
status sqrt_equation(double eps, double *result);
status y_limit(double eps, double *result);
status y_row(double eps, double *result);
status y_equation(double eps, double *result);

#endif