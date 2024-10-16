#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#define EPS 1e-10

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    BASE_ERROR,
    MEMORY_ERROR
} status;

status has_a_finite_representation(int base, double *res, int *len_ans, int count_nums, ...);

#endif