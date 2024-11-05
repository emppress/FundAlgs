#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#define START_HASH_SIZE 128

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    MISSING,
    FILE_OPEN_ERROR
} status;

typedef struct String
{
    char *arr;
    size_t capacity;
    size_t len;
} String;

typedef struct Node
{
    String key;
    String value;
    size_t full_hash;
    struct Node *next;
} Node;

typedef struct Hash_table
{
    size_t size;
    Node **arr;
} Hash_table;

status add(const String *key, const String *value, Hash_table *hash_table);
status remove_elem(const String *key, Hash_table *hash_table);
status find(const String *key, const Hash_table *hash_table, Node **found);
status hash_table_init(Hash_table *table);
status resize(Hash_table *hash_table);
void hash_table_delete(Hash_table *table);

status read_define(Hash_table *hash_table, FILE *input, String *buf, char *last_char);
status replace_text(Hash_table *hash_table, FILE *input, FILE *output, String *buf, char ch);
status add_char_to_string(String *str, char ch);
String *string_destroy(String *str);
status string_dup(String *dest, const String *sourse);
status delete_string_content(String *string);
status string_init(String *string);
status create_out_name(const char *filename, char *new_file_name);

#endif