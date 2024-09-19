#include "functions.h"

status string_to_double(const char *str, double *result)
{
    char *endinp;
    *result = strtold(str, &endinp);
    if (errno == ERANGE && (*result == HUGE_VALL || *result == (-HUGE_VAL)))
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    if (errno != 0 && *result == 0)
        return INPUT_ERROR;
    return OK;
}

status e_limit(double eps, double *result)
{
    int n = 1;
    double cur = 0, prev = 0;
    do
    {
        prev = cur;
        cur = pow((1.0 + 1.0 / n), n);
        n *= 2;
    } while (fabs(prev - cur) >= eps);
    *result = cur;
    return OK;
}

status pi_limit(double eps, double *result)
{
    int n = 2;
    double cur = 4, prev = 0;
    do
    {
        prev = cur;
        cur *= (4.0 * n * (n - 1.0)) / pow(2.0 * n - 1.0, 2);
        n++;
    } while (fabs(prev - cur) >= eps);
    *result = cur;
    return OK;
}

status ln2_limit(double eps, double *result)
{
    int n = 1;
    double cur = 0, prev = 0;
    do
    {
        prev = cur;
        cur = n * (pow(2, 1.0 / (double)n) - 1);
        n *= 2;
    } while (fabs(prev - cur) >= eps);
    *result = cur;
    return OK;
}

status sqrt_limit(double eps, double *result)
{
    int n = 1;
    double cur = -0.5, prev = -0.5;
    do
    {
        prev = cur;
        cur = prev - prev * prev / 2 + 1;
        n++;
    } while (fabs(prev - cur) >= eps);
    *result = cur;
    return OK;
}

status e_row(double eps, double *result)
{
    double sum = 1, prev = 1;
    int n = 1;
    do
    {
        prev /= (double)n;
        sum += prev;
        n++;
    } while (prev >= eps);
    *result = sum;
    return OK;
}

status pi_row(double eps, double *result)
{
    double sum = 0, qur = 0, sign = 1;
    int n = 1;
    do
    {
        qur = sign / (2.0 * n - 1.0);
        sum += qur;
        n++;
        sign *= -1;
    } while (fabs(4 * qur) >= eps);
    *result = 4.0 * sum;
    return OK;
}

status ln2_row(double eps, double *result)
{
    double sum = 0, qur = 0, sign = 1;
    int n = 1;
    do
    {
        qur = sign / n;
        sum += qur;
        n++;
        sign *= -1;
    } while (fabs(qur) >= eps);
    *result = sum;
    return OK;
}

status sqrt_row(double eps, double *result)
{
    double sum, cur = pow(2.0, 0.25), prev_sum;
    sum = cur;
    int n = 3;
    do
    {
        prev_sum = sum;
        cur = pow(cur, 0.5);
        sum *= cur;
        n++;
    } while (fabs(prev_sum - sum) >= eps);
    *result = sum;
    return OK;
}

status e_equation(double eps, double *result)
{
    double x = 3, f = 1, df, last_x = 0;
    while (fabs(last_x - x) >= eps)
    {
        f = log(x) - 1;
        df = 1.0 / x;
        last_x = x;
        x = x - f / df;
    }
    *result = x;
    return OK;
}

status pi_equation(double eps, double *result)
{
    double x = 3, f = 1, df, last_x = 0;
    while (fabs(last_x - x) >= eps)
    {
        f = cos(x) + 1;
        df = -sin(x);
        last_x = x;
        x = x - f / df;
    }
    *result = x;
    return OK;
}

status ln2_equation(double eps, double *result)
{
    double x = 1, f = 1, df, last_x = 0;
    while (fabs(last_x - x) >= eps)
    {
        f = exp(x) - 2;
        df = exp(x);
        last_x = x;
        x = x - f / df;
    }
    *result = x;
    return OK;
}

status sqrt_equation(double eps, double *result)
{
    double x = 1, f = 1, df, last_x = 0;
    while (fabs(last_x - x) >= eps)
    {
        f = x * x - 2;
        df = 2 * x;
        last_x = x;
        x = x - f / df;
    }
    *result = x;
    return OK;
}

double sum(int n)
{
    double s = 0;
    for (int k = 1; k <= n; k++)
    {
        s += (1.0 / k);
    }

    return s;
}

status y_limit(double eps, double *result)
{
    int n = 1;
    double cur = 1, prev = 1;
    do
    {
        prev = cur;
        n *= 2;
        cur = sum(n) - log(n);
    } while (fabs(prev - cur) >= eps);
    *result = cur;
    return OK;
}

status y_row(double eps, double *result)
{
    double cur = 0.5, sq = 0, prev = 0;
    int k = 2;
    do
    {
        prev = cur;
        k++;
        sq = sqrt(k);
        if (fmod(sq, 1.0) == 0)
        {
            k++;
            sq = (int)pow(k, 0.5);
        }
        cur += 1.0 / pow((int)sq, 2.0) - 1.0 / k;
    } while (fabs(prev - cur) >= eps);
    *result = (cur - (pow(PI, PI) / 6));
    return OK;
}

int is_prime(int number)
{

    if (number == 0 || number == 1 || number == 2)
        return 0;

    if (number % 2 == 0)
        return 1;

    for (int i = 3; i * i <= number; i++)
    {
        if (number % i == 0)
            return 1;
    }
    return 0;
}

status y_equation(double eps, double *result)
{
    int p = 2;
    double cur = log(2) * 0.5, prev = 0, prod = 0.5;
    do
    {
        prev = cur;
        do
        {
            p++;
        } while (is_prime(p));
        prod *= (p - 1.0) / p;
        cur = log(p) * prod;

    } while (fabs(prev - cur) >= eps);
    *result = (-log(cur));
    return OK;
}
