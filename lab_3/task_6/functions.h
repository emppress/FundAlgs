#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#define BUFER_SIZE 256
#define TIME_SIZE 20
#define EPS 1e-14

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

typedef struct Time
{
    unsigned int day, year, month, hour, minute, second;
} Time;

typedef struct Coordinates
{
    double x, y;
} Coordinates;

typedef struct Data
{
    char vehicle_number[BUFER_SIZE];
    Time stop_time, departure_time;
    char marker;
    Coordinates coords;
} Data;

typedef struct Rout_Node
{
    Data data;
    struct Rout_Node *next;
} Rout_Node;

typedef struct Rout_List
{
    size_t size;
    Rout_Node *head;
} Rout_List;

typedef struct External_Node
{
    Rout_List rout;
    struct External_Node *next;
} External_Node;

typedef struct External_List
{
    size_t size;
    External_Node *head;
} External_List;

union Value
{
    size_t svalue;
    double dvalue;
};

typedef struct Pair
{
    char *key;
    union Value value;
} Pair;

status scan_data_from_files(char **files, int count_files, External_List *ex_list);
status init_external_list(External_List *list);
status max_time_stop_all(External_List *ex_list, unsigned int *stop_time, char *vehicle_number);
status destroy_external_list(External_List *list);
status max_length_rout(External_List *ex_list, double *max_len, char *v_number);
status min_length_rout(External_List *ex_list, double *min_len, char *v_number);
status max_len_way(External_List *ex_list, double *len_way, char *vehicle_number);
status min_len_way(External_List *ex_list, double *len_way, char *vehicle_number);
status max_count_routs(External_List *ex_list, size_t *count_routs, char *vehicle_number);
status min_count_routs(External_List *ex_list, size_t *count_routs, char *vehicle_number);
status max_time_stop(External_List *ex_list, unsigned int *stop_time, char *vehicle_number);
status min_time_stop(External_List *ex_list, unsigned int *stop_time, char *vehicle_number);
void print_menu();
void free_buf();
#endif