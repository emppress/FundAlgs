#include "functions.h"

int main()
{
    point p1, p2, p3, p4, p5;
    p1 = (point){-4., 1.};
    p2 = (point){-2., 4.};
    p3 = (point){2., 5.};
    p4 = (point){5., 2.};
    p5 = (point){0., 1.44444};
    printf("%s\n", is_convex(5, p5, p4, p3, p2, p1) ? "YES" : "NO");

    double result;
    switch (find_polynom(&result, 2., 2, 2., 3., 1.))
    {
    case INPUT_ERROR:
        printf("Input error\n");
        return INPUT_ERROR;
        break;
    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("%lf\n", result);
        break;
    }

    if (is_kaprekar(1, 16, "1") != SUCCESS)
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }
}
