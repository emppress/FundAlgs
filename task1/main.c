#include "functions.c"

int main(int argc, char *argv[])
{
    if (argc != 3 || !(argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0')
    {
        printf("Ошибка ввода");
        return INPUT_ERROR;
    }

    long num = 0;
    errno = 0;
    if (string_to_int(argv[2], &num) != OK)
    {
        return INPUT_ERROR;
    }

    switch (argv[1][1])
    {
    case 'h':
        int *res_h = NULL;
        int size = 0;
        status status_h = natural_nums(num, &res_h, &size);
        if (status_h == MEMORY_ERROR)
        {
            printf("Ошибка памяти\n");
            if (res_h != NULL)
                free(res_h);
            return MEMORY_ERROR;
        }

        if (status_h == INPUT_ERROR)
        {
            printf("Ошибка деления на ноль\n");
            if (res_h != NULL)
                free(res_h);
            return INPUT_ERROR;
        }

        if (size == 0)
        {
            printf("Таких чисел не существует\n");
        }

        else
        {
            for (int i = 0; i < size; i++)
            {
                printf("%d ", res_h[i]);
            }
            free(res_h);
        }
        break;

    case 'p':
        if (num < 0)
        {
            printf("Ошибка ввода\n");
            return INPUT_ERROR;
        }
        int res_p = is_prime(num);
        if (res_p == 0)
        {
            printf("Число не является ни простым, ни составным\n");
        }
        else if (res_p == 1)
        {
            printf("Число является составным\n");
        }
        else
        {
            printf("Число является простым\n");
        }

        break;

    case 'a':
        long long res_a = 0;
        if (sum(num, &res_a) == MEMORY_ERROR)
        {
            printf("Ошибка памяти\n");
            return MEMORY_ERROR;
        }
        printf("%lld\n", res_a);

        break;

    case 'f':
        long long res_f = 1;
        status st = fact(num, &res_f);
        if (st == INPUT_ERROR)
        {
            printf("Oшибка ввода: отрицательное значение\n");
            return INPUT_ERROR;
        }
        if (st == MEMORY_ERROR)
        {
            printf("Ошибка памяти\n");
            return MEMORY_ERROR;
        }
        printf("%lld\n", res_f);

        break;

    case 'e':
        if (degree_table(num) == INPUT_ERROR)
        {
            printf("Oшибка ввода:\n");
            return INPUT_ERROR;
        }

    case 's':
        int size_s;
        char *res_s = NULL;
        if (split_16(num, &res_s, &size_s) == MEMORY_ERROR)
        {
            printf("Ошибка памяти\n");
            if (res_s != NULL)
                free(res_s);
            return MEMORY_ERROR;
        }
        for (int i = 0; i < size_s; i++)
        {
            printf("%c ", res_s[i]);
        }
        putchar('\n');
        free(res_s);

    default:
        printf("Введённого параметра не существует\n");
        return INPUT_ERROR;
    }
    return 0;
}