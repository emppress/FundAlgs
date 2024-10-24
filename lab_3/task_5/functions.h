#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define EPS DBL_EPSILON
#define ull unsigned long long
#define BUFF_SIZE 256

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    FILE_OPEN_ERROR,
    MISSING
} status;

typedef struct Student
{
    ull id;
    char name[BUFF_SIZE];
    char surname[BUFF_SIZE];
    char group[BUFF_SIZE];
    unsigned char *marks;
} Student;

status list_file_scan(Student **student, FILE *file, size_t *size, double *average);
void delete_student_arr(Student *arr, size_t size);
status surname_or_name_search(Student *student_arr, size_t size, const char *key, Student ***found_arr, size_t *found_size, char mod);
status id_search(Student *student_arr, size_t size, ull id, Student **found);
status avg_search_print_to_trace_file(const Student *student_arr, size_t size, double avg, FILE *output);
int id_cmp(const void *a, const void *b);
int surname_cmp(const void *a, const void *b);
int name_cmp(const void *a, const void *b);
int group_cmp(const void *a, const void *b);
void free_buf();
void print_menu();
status string_to_ull(const char *str, unsigned long long *result);
void student_print(const Student *student);
void student_file_print(const Student *student, FILE *output);

#endif