#include "functions.h"

int main(int argc, char *argv[])
{
    int base;
    if (!scanf("%d", &base) || base < 2 || base > 32)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    char *num_array = malloc(70 * sizeof *num_array);
    if (num_array == NULL)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    long long num_base10, max_num = LLONG_MIN;

    while (scanf("%69s", num_array))
    {
        if (!strcmp(num_array, "Stop"))
        {
            if (max_num == LLONG_MIN)
            {
                printf("Числа не были введены\n");
                free(num_array);
                return INPUT_ERROR;
            }
            printf("%lld\n", max_num);
            return OK;
        }

        else
        {
            if (string_to_ll(num_array, &num_base10, base) == INPUT_ERROR)
            {
                printf("Ошибка ввода\n");
                free(num_array);
                return INPUT_ERROR;
            }
            if (llabs(num_base10) > max_num)
                max_num = llabs(num_base10);
        }
    }
    free(num_array);
}