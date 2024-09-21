#include "functions.h"

status string_to_ll(const char *str, long long *result, int base)
{
    char *endptr;
    int flag = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (base < 10)
        {
            if (str[i] == '+' || str[i] == '-')
            {
                if (flag || i != 0)
                    return INPUT_ERROR;
                flag = 1;
            }
            else if ((str[i] < '0' || str[i] > base + '0'))
                return INPUT_ERROR;
        }
        else
        {
            if (str[i] == '+' || str[i] == '-')
            {
                if (flag || i != 0)
                    return INPUT_ERROR;
                flag = 1;
            }
            else if ((str[i] < '0' || str[i] > '9') && (str[i] < 'A' || str[i] > base - 10 + 'A'))
            {
                return INPUT_ERROR;
            }
        }
    }
    *result = strtoll(str, &endptr, base);
    if (*result == LLONG_MAX || *result == LLONG_MIN)
        return INPUT_ERROR;
    if (*endptr != '\0')
        return INPUT_ERROR;
    return OK;
}

void convert_to_xbase(long long num, int base, char *result)
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