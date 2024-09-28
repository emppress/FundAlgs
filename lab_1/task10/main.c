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

            char *num_new_base = malloc(22 * sizeof *num_new_base);
            if (num_new_base == NULL)
            {
                printf("Ошибка ввода\n");
                return INPUT_ERROR;
            }
            convert_to_xbase(max_num, base, num_new_base);
            printf("In %d: %s\n", base, num_new_base);
            convert_to_xbase(max_num, 9, num_new_base);
            printf("In 9: %s\n", num_new_base);
            convert_to_xbase(max_num, 18, num_new_base);
            printf("In 18: %s\n", num_new_base);
            convert_to_xbase(max_num, 27, num_new_base);
            printf("In 27: %s\n", num_new_base);
            convert_to_xbase(max_num, 36, num_new_base);
            printf("In 36: %s\n", num_new_base);
            free(num_new_base);
            break;
        }

        else
        {
            if (string_to_ll(num_array, &num_base10, base) == INPUT_ERROR)
            {
                printf("Ошибка ввода\n");
                free(num_array);
                return INPUT_ERROR;
            }
            if (llabs(num_base10) > llabs(max_num))
            {
                max_num = num_base10;
            }
        }
    }
    free(num_array);
    return OK;
}