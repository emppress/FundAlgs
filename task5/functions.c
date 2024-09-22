#include "functions.h"

status string_to_double(const char *str, double *result)
{
    int flag = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
        {
            if (flag)
                return INPUT_ERROR;
            flag = 1;
        }

        else if ((str[i] < '0' || str[i] > '9') && !(str[i] == '-' && i == 0))
            return INPUT_ERROR;
    }
    if (sscanf(str, "%lf", result) != 1)
        return INPUT_ERROR;
    return OK;
}

status sum_a(double x, double eps, double *result)
{
    int n = 0;
    *result = 0;
    double cur = 1;
    while (fabs(cur) >= eps)
    {
        (*result) += cur;
        if (n == INT_MAX)
            return INPUT_ERROR;
        n++;
        cur *= x / n;
    }
    return OK;
}

status sum_b(double x, double eps, double *result)
{
    int n = 0;
    *result = 0;
    double cur = 1;
    while (fabs(cur) >= eps)
    {
        (*result) += cur;
        if (n == INT_MAX)
            return INPUT_ERROR;
        n++;
        cur *= (-1.0) * x * x / (2 * n * (2 * n - 1.0));
    }
    return OK;
}

status sum_c(double x, double eps, double *result)
{
    int n = 0;
    *result = 0;
    double cur = 1;
    while (fabs(cur) >= eps)
    {
        (*result) += cur;
        if (n == INT_MAX)
            return INPUT_ERROR;
        n++;
        cur *= (x * x * 9.0 * n * n) / (9 * n * (n - 1) + 2);
        if (cur + eps >= 1.0)
        {
            *result = INFINITY;
            return OK;
        }
    }
    return OK;
}

status sum_d(double x, double eps, double *result)
{
    int n = 1;
    *result = 0;
    double cur = -1.0 * x * x / 2.0;
    while (fabs(cur) >= eps)
    {
        (*result) += cur;
        if (n == INT_MAX)
            return INPUT_ERROR;
        n++;
        cur *= (-1.0 * x * x * (2.0 * n - 1.0)) / (2.0 * n);
    }
    return OK;
}