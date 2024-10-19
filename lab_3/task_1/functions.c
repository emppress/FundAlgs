#include "functions.h"

int add(int x, int y)
{
    int keep = (x & y) << 1;
    int res = x ^ y;
    if (keep == 0)
        return res;

    return add(keep, res);
}

int negative(int x)
{
    return add(~x, 1);
}

status base10_to_basex(int num, int r, char *ans, int *size)
{
    int mask, digit;
    char base[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV", flag = 0;
    *size = 0;
    if (!ans || r < 1 || r > 5)
    {
        return INPUT_ERROR;
    }
    if (num < 0)
    {
        num = negative(num);
        flag = '1';
    }
    do
    {
        mask = add(1 << r, -1);
        digit = num & mask;
        num >>= r;
        ans[*size] = base[digit];
        *size = add(*size, 1);
    } while (num);
    if (flag)
    {
        ans[*size] = '-';
        *size = add(*size, 1);
    }
    return SUCCESS;
}

void print_result(const char *res, int size)
{
    int i;
    printf("Result: ");
    for (i = add(size, -1); i >= 0; i = add(i, -1))
    {
        putchar(res[i]);
    }
    putchar('\n');
}