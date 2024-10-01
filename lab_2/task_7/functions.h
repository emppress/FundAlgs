#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum Status
{
    SUCCESS,
    NO_ROOTS,
    INPUT_ERROR
} status;

double func1(double x);
double func2(double x);
double func3(double x);
double func4(double x);
double func5(double x);
status dichotomy_method(double *res, double left, double right, double eps, double (*f)(double));

#endif