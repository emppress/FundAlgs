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
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    FILE_OPEN_ERROR
} status;

typedef struct Employee
{
    unsigned int id;
    char name[64];
    char surname[64];
    double salary;
} Employee;

status validate_input(int argc, char **argv);
status scan_employee_from_file(Employee **out_data, FILE *in, int *size_data);
int compare_a(const void *a, const void *b);
int compare_d(const void *a, const void *b);

#endif