#include "functions.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    long a, b;

    if (string_to_int(argv[1], &a) != OK ||
        string_to_int(argv[2], &b) != OK || a > b)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    srand(time(NULL));

    long array[CAPACITY];
    generate_static_array(array, a, b);
    printf("Первое задание:\n");
    printf("Массив до замены: [");
    for (int i = 0; i < CAPACITY; i++)
    {
        printf("%ld", array[i]);
        if (i != CAPACITY - 1)
            printf(", ");
    }
    printf("]\n");

    swap_min_max(array);
    printf("Массив после замены: [");
    for (int i = 0; i < CAPACITY; i++)
    {
        printf("%ld", array[i]);
        if (i != CAPACITY - 1)
            printf(", ");
    }
    printf("]\n");

    printf("Второе задание:\n");

    int *A = NULL, *B = NULL, *C = NULL;
    int size_array = rand() % (10000 - 10 + 1) + 10;
    C = malloc(size_array * sizeof *C);
    if (generate_dynamic_array(&A, size_array) != OK ||
        generate_dynamic_array(&B, size_array) != OK || C == NULL)
    {
        printf("Ошибка памяти\n");
        if (A)
            free(A);
        if (B)
            free(B);
        if (C)
            free(C);
        return MEMORY_ERROR;
    }

    int res;
    printf("Массив A: [");
    for (int i = 0; i < size_array; i++)
    {
        printf("%d", A[i]);
        if (i != size_array - 1)
            printf(", ");
    }
    printf("]\n");
    printf("Массив B: [");
    for (int i = 0; i < size_array; i++)
    {
        printf("%d", B[i]);
        if (i != size_array - 1)
            printf(", ");
    }
    printf("]\n");

    generate_sum_array(A, B, C, size_array);

    printf("Массив C: [");
    for (int i = 0; i < size_array; i++)
    {
        printf("%d", C[i]);
        if (i != size_array - 1)
            printf(", ");
    }
    printf("]\n");

    free(A);
    free(B);
    free(C);
}