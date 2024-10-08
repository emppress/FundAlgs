#include "functions.h"

status string_to_int(const char *str, long *result)
{
    char *endinp;
    for (int i = 0; i < strlen(str); i++)
    {
        if ((str[i] < '0' || str[i] > '9') && !(str[i] == '-' && i == 0))
            return INPUT_ERROR;
    }
    *result = strtol(str, &endinp, 10);
    if (*result == LONG_MAX || *result == LONG_MIN)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    return OK;
}

status natural_nums(const long x, int **result, int *size)
{
    if (x == 0)
    {
        return INPUT_ERROR;
    }

    *result = (int *)malloc(sizeof(int) * 100);
    if (*result == NULL)
        return MEMORY_ERROR;

    for (int i = 1; i <= 100; i++)
    {
        if (i % x == 0)
        {
            (*result)[(*size)++] = i;
        }
    }
    return OK;
}

// enum
int is_prime(const long x)
{
    if (x <= 1)
        return 0;
    if (x == 2)
        return 2;
    if (x % 2 == 0)
        return 1;

    for (int i = 2; i * i <= x; i++)
    {
        if (x % i == 0)
        {
            return 1;
        }
    }
    return 2;
}

status sum(const long x, long long *result)
{

    for (long i = 1; i <= x; i++)
    {
        if (LLONG_MAX - i <= *result)
            return MEMORY_ERROR;
        (*result) += i;
    }

    return OK;
}

status fact(const long x, long long *result)
{
    if (x < 0)

        return INPUT_ERROR;

    *result = 1;

    for (long i = 2; i <= x; i++)
    {
        if (LLONG_MAX / i < *result)
            return MEMORY_ERROR;

        (*result) *= i;
    }
    return OK;
}

status degree_table(const long x)
{
    if (x < 1 || x > 10)
        return INPUT_ERROR;
    long long num;
    for (int i = 1; i <= 10; i++)
    {
        num = 1;
        printf("| %2d |", i);
        for (int j = 1; j <= x; j++)
        {
            num *= i;
            printf("%10lld ", num);
        }
        printf("\n");
    }
    return OK;
}

status split_16(long x, char **result, int *size)
{
    int flag = 0;
    int capacity = 4;
    (*size) = 0;
    char *resize = NULL, temp;
    *result = malloc(capacity * sizeof **result);
    if (*result == NULL)
        return MEMORY_ERROR;

    if (x < 0)
    {
        flag = 1;
        x *= -1;
    }

    while (x)
    {
        (*result)[(*size)++] = ((x % 16 < 10) ? (x % 16 + '0') : (x % 16 + 'A' - 10));
        x /= 16;
        if (*size == capacity)
        {
            capacity *= 2;
            resize = (char *)realloc(*result, capacity * sizeof(char));
            if (resize == NULL)
                return MEMORY_ERROR;
            *result = resize;
        }
    }
    if (flag)
        (*result)[(*size)++] = '-';

    for (int i = 0; i < (*size) / 2; i++)
    {
        temp = (*result)[i];
        (*result)[i] = (*result)[*size - i - 1];
        (*result)[*size - i - 1] = temp;
    }

    return OK;
}