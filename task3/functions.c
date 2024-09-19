#include "functions.h"

status string_to_int(const char *str, long *result)
{
    char *endinp;
    *result = strtol(str, &endinp, 10);
    if (errno == ERANGE && (*result == LONG_MAX || *result == LONG_MIN))
    {
        return INPUT_ERROR;
    }
    if (*endinp != '\0')
    {
        return INPUT_ERROR;
    }
    if (errno != 0 && *result == 0)
    {
        return INPUT_ERROR;
    }
    return OK;
}

status string_to_double(const char *str, double *result)
{
    char *endinp;
    *result = strtold(str, &endinp);
    if (errno == ERANGE && (*result == HUGE_VALL || *result == (-HUGE_VAL)))
    {
        printf("Ошибка ввода");
        return INPUT_ERROR;
    }
    if (*endinp != '\0')
    {
        printf("Ошибка ввода");
        return INPUT_ERROR;
    }
    if (errno != 0 && *result == 0)
    {
        printf("Ошибка ввода");
        return INPUT_ERROR;
    }
    return OK;
}

void swap(double *a, double *b)
{
    long double temp = *a;
    *a = *b;
    *b = temp;
}

int no_dublicate(int *size, double *coefs, double ***result, double eps)
{
    int count = 0;
    for (int i = 0; i < *size; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (fabsl((*result)[i][j] - coefs[j]) < eps)
                count++;
        }
        if (count == 3)
            return 0;
        count = 0;
    }
    return 1;
}

status gen_permutations(int *size, double *coefs, double eps, double ***result, int l, int r)
{
    status state = OK;
    if (no_dublicate(size, coefs, result, eps))
    {
        (*result)[*size] = (double *)malloc(sizeof(long double) * 3);
        if ((*result)[*size] == NULL)
        {
            return MEMORY_ERROR;
        }
        for (int i = 0; i < 3; i++)
        {
            (*result)[*size][i] = coefs[i];
        }
        (*size)++;
    }
    for (int i = l; i <= r; i++)
    {
        swap(coefs + i, coefs + l);
        if (gen_permutations(size, coefs, eps, result, l + 1, r) == MEMORY_ERROR)
            return MEMORY_ERROR;
        swap(coefs + i, coefs + l);
    }
    return state;
}

int is_overflow(double a, double b, double c, double eps)
{
    double sqt = sqrtl(DBL_MAX);
    if (a - sqt >= eps || b - sqt >= eps || c - sqt >= eps)
        return 1;
    return 0;
}

status is_triangle(double side1, double side2, double side3, int *res, double eps)
{
    if (side1 <= eps || side2 <= eps || side3 <= eps)
    {
        return INPUT_ERROR;
    }
    if (is_overflow(side1, side2, side3, eps))
    {
        return OVERFLOW_ERROR;
    }

    if (fabsl(side1 * side1 + side2 * side2 - side3 * side3) <= eps ||
        fabsl(side1 * side1 + side3 * side3 - side2 * side2) <= eps ||
        fabsl(side2 * side2 + side3 * side3 - side1 * side1) <= eps)
        *res = 1;
    else
        *res = 0;

    return OK;
}