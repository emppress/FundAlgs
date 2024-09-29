#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#define CAPACITY 100

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

typedef struct Point
{
    double x;
    double y;
} point;

int is_convex(int count_corners, ...);
status find_polynom(double *res, double x, int n, ...);
status is_kaprekar(int *result, int count_numbers, int base, ...);

#endif