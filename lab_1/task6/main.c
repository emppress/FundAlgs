#include "functions.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Input ERROR\n");
        return INPUT_ERROR;
    }

    double eps;
    if (string_to_double(argv[1], &eps) != OK || eps <= 0.0)
    {
        printf("Input ERROR\n");
        return INPUT_ERROR;
    }

    double result;
    if (integral(eps, &result, f_a, right_rect))
    {
        printf("The epsilon is too high\n");
        return INPUT_ERROR;
    }

    printf("%lf\n", result);
    if (integral(eps, &result, f_b, right_rect))
    {
        printf("The epsilon is too high\n");
        return INPUT_ERROR;
    }
    printf("%lf\n", result);
    if (integral(eps, &result, f_c, left_rect))
    {
        printf("The epsilon is too high\n");
        return INPUT_ERROR;
    }
    printf("%lf\n", result);
    if (integral(eps, &result, f_d, right_rect))
    {
        printf("The epsilon is too high\n");
        return INPUT_ERROR;
    }
    printf("%lf\n", result);
}