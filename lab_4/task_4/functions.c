#include "functions.h"

status string_to_long(const char *str, long *result, int base)
{
    char *endinp;
    if (!result || !str)
        return MEMORY_ERROR;
    *result = strtol(str, &endinp, base);
    if (*result == LONG_MAX || *result == LONG_MIN)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

status validate_file(char **argv)
{
    if (!argv)
        return INPUT_ERROR;
    char full_path_1[BUFSIZ], full_path_2[BUFSIZ];
    full_path_2[0] = '\0';
    if (!realpath(argv[1], full_path_1))
        return INPUT_ERROR;
    realpath(argv[3], full_path_2);
    if (!strcmp(full_path_1, full_path_2))
        return INPUT_ERROR;
    return SUCCESS;
}

status convert_to_xbase(long num, long base, char *result)
{
    if (!result)
        return MEMORY_ERROR;
    int i, flag_minus = 0;
    char temp;
    int len_result = 0;
    if (num == 0)
    {
        result[0] = '0';
        result[1] = '\0';
        return SUCCESS;
    }
    if (num < 0)
    {
        num = labs(num);
        flag_minus = 1;
    }
    while (num)
    {
        result[len_result++] = ((num % base > 9) ? (num % base - 10 + 'A') : (num % base + '0'));
        num /= base;
    }
    if (flag_minus)
        result[len_result++] = '-';
    for (i = 0; i < len_result / 2; i++)
    {
        temp = result[i];
        result[i] = result[len_result - 1 - i];
        result[len_result - 1 - i] = temp;
    }
    result[len_result] = '\000';
    return SUCCESS;
}

status convert_to_bitstr(long num, char *result)
{
    size_t i;
    if (!result)
        return MEMORY_ERROR;

    for (i = 0; i < sizeof(long) * 8; ++i)
    {
        result[i] = ((1ul << i) & num) ? '1' : '0';
    }
    result[i] = '\0';
    return SUCCESS;
}

status string_init(String *string)
{
    if (!string)
        return MEMORY_ERROR;
    string->capacity = 1;
    string->len = 0;
    if (!(string->arr = (char *)malloc(sizeof(char))))
        return MEMORY_ERROR;
    string->arr[0] = '\0';
    return SUCCESS;
}

status delete_string_content(String *string)
{
    if (!string)
        return MEMORY_ERROR;
    free(string->arr);
    string->arr = NULL;
    string->len = 0;
    string->capacity = 0;
    return SUCCESS;
}

int string_compare(const String *str_1, const String *str_2)
{
    if (str_1->len != str_2->len)
        return str_1->len - str_2->len;
    return strcmp(str_1->arr, str_2->arr);
}

int string_equivalence_relation(const String *str_1, const String *str_2)
{
    return strcmp(str_1->arr, str_2->arr) == 0;
}

status string_dup(String *dest, const String *sourse)
{
    if (!dest || !sourse)
        return MEMORY_ERROR;

    dest->capacity = sourse->capacity;
    dest->len = sourse->len;
    if (!sourse->arr)
    {
        dest->arr = NULL;
        return SUCCESS;
    }
    dest->arr = (char *)malloc(sourse->capacity * sizeof(char));
    if (!dest->arr)
        return MEMORY_ERROR;
    strcpy(dest->arr, sourse->arr);
    return SUCCESS;
}

String *string_destroy(String *str)
{
    if (!str)
        return NULL;
    free(str->arr);
    free(str);
    return NULL;
}

status add_char_to_string(String *str, char ch)
{
    if (!str)
        return MEMORY_ERROR;
    if (str->capacity == 0)
    {
        if (string_init(str))
            return MEMORY_ERROR;
    }
    if (str->capacity == str->len + 1)
    {
        char *for_realloc;
        str->capacity *= 2;
        for_realloc = (char *)realloc(str->arr, str->capacity * sizeof(char));
        if (!for_realloc)
            return MEMORY_ERROR;
        str->arr = for_realloc;
    }
    str->arr[str->len++] = ch;
    str->arr[str->len] = '\0';
    return SUCCESS;
}

status read_word(String *str)
{
    char ch;
    if (!str)
        return MEMORY_ERROR;

    if (string_init(str) == MEMORY_ERROR)
        return MEMORY_ERROR;

    ch = getchar();
    while (ch != EOF && !isspace(ch))
    {
        if (add_char_to_string(str, ch))
            return MEMORY_ERROR;
        ch = getchar();
    }
    return SUCCESS;
}

status read_string(FILE *input, String *str)
{
    char ch;
    if (!input || !str)
        return MEMORY_ERROR;

    if (string_init(str) == MEMORY_ERROR)
        return MEMORY_ERROR;

    ch = getc(input);

    while (ch != EOF && ch != ';')
    {
        if (ch == '%')
        {
            while (ch != EOF && ch != '\n')
            {
                ch = getc(input);
            }
        }
        else if (ch == '{')
        {
            while (ch != EOF && ch != '}')
            {
                ch = getc(input);
            }
            if (ch == EOF)
                return INPUT_ERROR;
        }

        else if (ch != EOF && !isspace(ch))
        {
            ch = toupper(ch);
            if (add_char_to_string(str, ch))
                return MEMORY_ERROR;
        }
        ch = getc(input);
    }
    if (ch != EOF)
    {
        if (add_char_to_string(str, ch))
            return MEMORY_ERROR;
    }
    return SUCCESS;
}

status read_vector(Vector *vector, const char *value, long base)
{
    if (!vector)
        return MEMORY_ERROR;

    vector->init = 1;
    if (string_to_long(value, &vector->value, (int)base))
    {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

status calculate_operation(Vector *a, Vector *b, const char *op, Vector *result)
{
    if (!a || !b || !result)
        return MEMORY_ERROR;

    if (!a->init || !b->init)
        return MISSING;

    result->init = 1;
    if (!strcmp(op, "+"))
        result->value = a->value | b->value;

    else if (!strcmp(op, "&"))
        result->value = a->value & b->value;

    else if (!strcmp(op, "->"))
        result->value = (!a->value) | b->value;

    else if (!strcmp(op, "<-"))
        result->value = a->value | (!b->value);

    else if (!strcmp(op, "~"))
        result->value = ((!a->value) | b->value) & (a->value | (!b->value));

    else if (!strcmp(op, "<>"))
        result->value = a->value ^ b->value;

    else if (!strcmp(op, "+>"))
        result->value = a->value & (!b->value);

    else if (!strcmp(op, "?"))
        result->value = (!a->value) | (!b->value);

    else if (!strcmp(op, "!"))
        result->value = (!a->value) & (!b->value);

    return SUCCESS;
}

status init_vectors_arr(Vector *v_arr)
{
    int i;
    if (!v_arr)
        return MEMORY_ERROR;

    for (i = 0; i < COUNT_VECTORS; ++i)
    {
        v_arr[i].init = 0;
        v_arr[i].name = 'A' + i;
        v_arr[i].value = 0;
    }
    return SUCCESS;
}

void write_message(const char *msg, FILE *out, ...)
{
    va_list args;
    if (!msg)
        return;

    va_start(args, out);
    if (out)
    {
        vfprintf(out, msg, args);
    }
    else
    {
        vprintf(msg, args);
    }
    va_end(args);
}