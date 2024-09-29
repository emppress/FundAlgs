#include "functions.h"

double find_component(point a, point b, point c)
{
    double res = a.x * b.y + a.y * c.x + b.x * c.y - c.x * b.y - b.x * a.y - a.x * c.y;
    return res;
}

int is_convex(int count_corners, ...)
{
    if (count_corners < 0)
        return 0;
    if (count_corners <= 3)
        return 1;

    point first, second, s1, s2, s3;
    int component;
    va_list args;
    va_start(args, count_corners);
    first = va_arg(args, point);
    second = va_arg(args, point);
    s3 = va_arg(args, point);
    component = (find_component(first, second, s3) >= 0.0) ? 1 : 0;
    s1 = first;
    s2 = second;
    for (int i = 0; i < count_corners - 3; i++)
    {
        s1 = s2;
        s2 = s3;
        s3 = va_arg(args, point);
        if (((find_component(s1, s2, s3) >= 0.0) ? 1 : 0) != component)
        {
            va_end(args);
            return 0;
        }
    }
    if (((find_component(s2, s3, first) >= 0.0) ? 1 : 0) != component)
    {
        return 0;
        va_end(args);
    }
    if (((find_component(s3, first, second) >= 0.0) ? 1 : 0) != component)
    {
        return 0;
        va_end(args);
    }
    return 1;
}

status find_polynom(double *res, double x, int n, ...)
{
    *res = 0;
    if (n < 0)
        return INPUT_ERROR;

    va_list coef;
    va_start(coef, n);
    for (int i = n; i >= 0; --i)
    {
        *res += pow(x, i) * va_arg(coef, double);
        if (isinf(*res) || isnan(*res))
        {
            va_end(coef);
            return OVERFLOW_ERROR;
        }
    }
    va_end(coef);
    return SUCCESS;
}

status string_to_uint(const char *str, unsigned long *result, int base)
{
    if (str[0] == '-')
        return INPUT_ERROR;
    char *endinp;
    *result = strtoul(str, &endinp, base);
    if (*result == ULONG_MAX)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

void convert_to_xbase(unsigned long num, int base, char *result)
{
    int i, flag_minus = 0;
    char temp;
    int len_result = 0;
    if (num < 0)
    {
        num = llabs(num);
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
}

status pow_base(char *number, int base)
{
    long int num_10;
    if (string_to_uint(number, &num_10, base) != SUCCESS || ULONG_MAX / num_10 < num_10)
        return INPUT_ERROR;
    num_10 *= num_10;
    convert_to_xbase(num_10, base, number);
    return SUCCESS;
}

status sum_base(char *number_1, char *number_2, int base, unsigned long *res)
{
    long int a_10, b_10;
    if (string_to_uint(number_1, &a_10, base) != SUCCESS)
        return INPUT_ERROR;
    if (string_to_uint(number_2, &b_10, base) != SUCCESS)
        return INPUT_ERROR;
    if (a_10 == 0 || b_10 == 0)
    {
        *res = 0;
        return SUCCESS;
    }
    if (ULONG_MAX - b_10 < a_10)
        return INPUT_ERROR;
    *res = a_10 + b_10;
    return SUCCESS;
}

status is_kaprekar(int count_numbers, int base, ...)
{
    va_list args;
    va_start(args, base);
    char number[CAPACITY], left[CAPACITY];
    int len, flag = 0;
    unsigned long x, sum;

    for (int i = 0; i < count_numbers; i++)
    {
        strcpy(number, va_arg(args, char *));
        if (string_to_uint(number, &x, base) != SUCCESS || pow_base(number, base) != SUCCESS || ULONG_MAX / x < x)
        {
            va_end(args);
            return INPUT_ERROR;
        }
        len = strlen(number);
        flag = 0;
        if (len == 1 && number[0] == '1')
            flag = 1;
        for (int j = 1; j < len; j++)
        {
            strncpy(left, number, j);
            left[j] = '\0';
            if (sum_base(left, &(number[j]), base, &sum) != SUCCESS)
            {
                va_end(args);
                return INPUT_ERROR;
            }
            if (sum == x)
            {
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            convert_to_xbase(x, base, number);
            printf("%s - NO\n", number);
        }
        else
        {
            convert_to_xbase(x, base, number);
            printf("%s - YES\n", number);
        }
    }
    va_end(args);
    return SUCCESS;
}