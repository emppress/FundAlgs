#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <errno.h>
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
status integral(double eps, double *result, double (*function)(double), void (*method)(int, double *, double (*)(double)));
double f_a(double x);
double f_b(double x);
double f_c(double x);
double f_d(double x);
void left_rect(int n, double *result, double (*function)(double));
void right_rect(int n, double *result, double (*function)(double));

#endif