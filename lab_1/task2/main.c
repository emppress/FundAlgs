#include "functions.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    double eps;

    if (string_to_double(argv[1], &eps) != OK || eps <= 0.0)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    double answers[15];
    char types[5][10] = {"e", "pi", "ln2", "sqrt(2)", "y"};
    e_limit(eps, &answers[0]);
    pi_limit(eps, &answers[1]);
    ln2_limit(eps, &answers[2]);
    sqrt_limit(eps, &answers[3]);
    y_limit(eps, &answers[4]);

    e_row(eps, &answers[5]);
    pi_row(eps, &answers[6]);
    ln2_row(eps, &answers[7]);
    sqrt_row(eps, &answers[8]);
    y_row(eps, &answers[9]);

    e_equation(eps, &answers[10]);
    pi_equation(eps, &answers[11]);
    ln2_equation(eps, &answers[12]);
    sqrt_equation(eps, &answers[13]);
    y_equation(eps, &answers[14]);
    printf("ПРЕДЕЛ:\n");
    for (int i = 0; i < 5; i++)
    {
        if (answers[i] != INFINITY)
        {
            printf("%s: %lf\n", types[i], answers[i]);
        }
    }
    printf("РЯД:\n");
    for (int i = 5; i < 10; i++)
    {
        if (answers[i] != INFINITY)
        {
            printf("%s: %lf\n", types[i % 5], answers[i]);
        }
    }
    printf("УРАВНЕНИЕ:\n");
    for (int i = 10; i < 15; i++)
    {
        if (answers[i] != INFINITY)
        {
            printf("%s: %lf\n", types[i % 5], answers[i]);
        }
    }
}