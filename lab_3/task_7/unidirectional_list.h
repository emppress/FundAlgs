#ifndef _UNIDIRECTIONAL_LIST_H_
#define _UNIDIRECTIONAL_LIST_H_

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "status.h"

#define BUF_SIZE 256
#define EPS 1e-10

typedef struct Date
{
    int day, month, year;
} Date;

typedef struct Liver
{
    char surname[BUF_SIZE];
    char name[BUF_SIZE];
    char patronymic[BUF_SIZE];
    Date birth_date;
    char sex;
    double income;
} Liver;

typedef struct Node
{
    Liver data;
    struct Node *next;
} Node;

typedef struct UList
{
    Node *head;
    size_t size;
} UList;

status list_init(UList *list);
size_t list_size(UList *list);
int is_empty(UList *list);
int date_cmp(const Date *date_1, const Date *date_2);
status list_insert(UList *list, Liver *liver);
status list_delete_node(UList *list, const Liver *liver);
status list_find(UList *list, const Liver *liver, Liver **found);
status liver_change(UList *list, const Liver *to_change, const Liver *new_data);
status list_file_print(UList *list, FILE *file);
status list_file_scan(UList *list, FILE *file);
status liver_validate(const Liver *liver);
status list_push(UList *list, const Liver *liver);
status list_pop(UList *list, Liver *top_liver);
status list_destroy(UList *list);

#endif