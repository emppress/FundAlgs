#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

status string_to_int(const char *str, long *result);
status string_to_double(const char *str, double *result);
void swap(double *a, double *b);
int no_dublicate(int *size, double *coefs, double ***result, double eps);
status gen_permutations(int *size, double *coefs, double eps, double ***result, int l, int r);
int is_overflow(double a, double b, double c, double eps);
status is_triangle(double side1, double side2, double side3, int *res, double eps);

#endif