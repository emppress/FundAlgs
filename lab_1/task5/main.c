#include "functions.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Input ERROR\n");
        return INPUT_ERROR;
    }

    double eps, x, result;
    int flag = 0;
    if (string_to_double(argv[1], &eps) != OK || eps <= 0)
    {
        printf("Input ERROR\n");
        return INPUT_ERROR;
    }
    if (string_to_double(argv[2], &x) != OK)
    {
        printf("Input ERROR\n");
        return INPUT_ERROR;
    }

    if (sum_a(x, eps, &result) != OK)
    {
        printf("The value (a) could have been calculated incorrectly\n");
        flag = 1;
    }
    printf("a) %lf\n", result);

    if (sum_b(x, eps, &result) != OK)
    {
        printf("The value (b) could have been calculated incorrectly\n");
        flag = 1;
    }
    printf("b) %lf\n", result);

    if (sum_c(x, eps, &result) != OK)
    {
        printf("The value (c) could have been calculated incorrectly\n");
        flag = 1;
    }
    printf("c) %lf\n", result);

    if (sum_d(x, eps, &result) != OK)
    {
        printf("The value (d) could have been calculated incorrectly\n");
        flag = 1;
    }
    printf("d) %lf\n", result);

    if (flag)
    {
        return INPUT_ERROR;
    }
    return 0;
}