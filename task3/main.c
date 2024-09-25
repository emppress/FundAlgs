#include "functions.h"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }
    if (!((argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0'))
    {
        printf("Ошибка ввода");
        return INPUT_ERROR;
    }

    switch (argv[1][1])
    {
    case 'q':
        if (argc != 6)
        {
            printf("Ошибка ввода");
            return INPUT_ERROR;
        }

        double eps_q, a, b, c;

        if (string_to_double(argv[2], &eps_q) ||
            string_to_double(argv[3], &a) ||
            string_to_double(argv[4], &b) ||
            string_to_double(argv[5], &c))
        {
            printf("Ошибка ввода");
            return INPUT_ERROR;
        }

        double **permutations = (double **)malloc(6 * sizeof(double *));
        double *coefs = (double *)malloc(3 * sizeof(double));
        if (permutations == NULL || coefs == NULL)
        {
            if (!permutations)
            {
                for (int i = 0; i < 6; i++)
                    free(permutations[i]);
                free(permutations);
            }
            if (!coefs)
                free(coefs);
            printf("Ошибка памяти\n");
            return MEMORY_ERROR;
        }
        coefs[0] = a;
        coefs[1] = b;
        coefs[2] = c;
        int size = 0;

        if (gen_permutations(&size, coefs, eps_q, &permutations, 0, 2) != OK)
        {

            for (int i = 0; i < size; i++)
            {
                free(permutations[i]);
            }
            free(permutations);
            printf("Ошибка памяти\n");
            return MEMORY_ERROR;
        }

        for (int i = 0; i < size; i++)
        {
            printf("уравнение %d) %.3lfx^2 + %.3lfx + %.3lf = 0\n", i + 1, permutations[i][0], permutations[i][1], permutations[i][2]);
            double a = permutations[i][0];
            double b = permutations[i][1];
            double c = permutations[i][2];
            double discriminant = b * b - 4.0 * a * c;
            if (discriminant < 0.0)
            {
                printf("действительных корней нет\n");
            }
            else
            {
                discriminant = sqrt(discriminant);
                b *= -1.0;
                double x1 = (b - discriminant) / (2.0 * a);
                double x2 = (b + discriminant) / (2.0 * a);
                printf("x1 = %lf\n", x1);
                printf("x2 = %lf\n", x2);
            }
            printf("\n");
        }
        for (int i = 0; i < size; i++)
        {
            free(permutations[i]);
        }
        free(permutations);
        break;

    case 'm':
        if (argc != 4)
        {
            printf("Ошибка ввода\n");
            return INPUT_ERROR;
        }
        long int num1, num2;
        if (string_to_int(argv[2], &num1) || string_to_int(argv[3], &num2) || num1 == 0 || num2 == 0)
        {
            printf("Ошибка ввода\n");
            return INPUT_ERROR;
        }
        if (num1 % num2 == 0)
        {
            printf("Число %ld кратно %ld\n", num1, num2);
        }
        else
        {
            printf("Число %ld НЕ кратно %ld\n", num1, num2);
        }
        break;

    case 't':
        if (argc != 6)
        {
            printf("Ошибка ввода\n");
            return INPUT_ERROR;
        }

        double eps_t, side1, side2, side3;

        if (string_to_double(argv[2], &eps_t) ||
            string_to_double(argv[3], &side1) ||
            string_to_double(argv[4], &side2) ||
            string_to_double(argv[5], &side3))
        {
            printf("Ошибка ввода\n");
            return INPUT_ERROR;
        }

        int res_t;
        status st = is_triangle(side1, side2, side3, &res_t, eps_t);
        if (st == OVERFLOW_ERROR)
        {
            printf("Ошибка переполнения\n");
            return OVERFLOW_ERROR;
        }
        else if (st == INPUT_ERROR)
        {
            printf("Ошибка ввода\n");
            return INPUT_ERROR;
        }

        if (res_t == 1)
        {
            printf("Введённые стороны могут образовать прямоугольный треугольник\n");
        }
        else
            printf("Введённые стороны не могут образовать прямоугольный треугольник\n");
        break;

    default:
        printf("Введённого параметра не существует\n");
        return INPUT_ERROR;
    }
    return OK;
}