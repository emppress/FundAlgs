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

double f_a(double x)
{
    return log(1 + x) / x;
}

double f_b(double x)
{
    return exp(-x * x / 2);
}

double f_c(double x)
{
    return log(1 / (1 - x));
}

double f_d(double x)
{
    return pow(x, x);
}

void right_rect(int n, double *result, double (*function)(double))
{
    double h = 1.0 / n, sum = 0;
    for (int i = 1; i <= n; i++)
    {
        sum += function((double)i * h);
    }
    *result = sum * h;
}

void left_rect(int n, double *result, double (*function)(double))
{
    double h = 1.0 / n, sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += function((double)i * h);
    }
    *result = sum * h;
}

status integral(double eps, double *result, double (*function)(double), void (*method)(int, double *, double (*)(double)))
{
    double prev = 0.0, cur = 0.0, n = 1;
    do
    {
        if (INT_MAX / 2 < n)
            return INPUT_ERROR;
        n *= 2;
        prev = cur;
        method(n, &cur, function);

    } while (fabs(prev - cur) >= eps);
    *result = cur;
    return OK;
}