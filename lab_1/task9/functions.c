#include "functions.h"

status string_to_int(const char *str, long *result)
{
    char *endinp;
    for (int i = 0; i < strlen(str); i++)
    {
        if ((str[i] < '0' || str[i] > '9') && !(str[i] == '-' && i == 0))
            return INPUT_ERROR;
    }
    *result = strtol(str, &endinp, 10);
    if (*result == LONG_MAX || *result == LONG_MIN)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    return OK;
}

void generate_static_array(long array[], long a, long b)
{
    srand(time(NULL));
    for (int i = 0; i < CAPACITY; i++)
    {
        array[i] = rand() % (b - a + 1) + a;
    }
}

void swap_min_max(long array[])
{
    int i_max = 0, i_min = 0;
    long temp;
    for (int i = 0; i < CAPACITY; i++)
    {
        if (array[i] > array[i_max])
            i_max = i;
        if (array[i] < array[i_min])
            i_min = i;
    }

    temp = array[i_max];
    array[i_max] = array[i_min];
    array[i_min] = temp;
}

status generate_dynamic_array(int **array, int size_array)
{
    srand(time(NULL));
    *array = malloc(size_array * sizeof **array);
    if (*array == NULL)
        return MEMORY_ERROR;

    for (int i = 0; i < size_array; i++)
    {
        (*array)[i] = rand() % 2001 - 1000;
    }
    return OK;
}

void bubble_sort(int *array, int size_array)
{
    long temp;
    for (int i = 0; i < size_array; i++)
    {
        for (int j = 0; j < size_array - i - 1; j++)
        {
            if (array[j + 1] < array[j])
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int find_nearest_num(int x, int *array, int size_array)
{
    int l = 0, r = size_array - 1, mid;
    while (l <= r)
    {
        mid = (l + r) / 2;
        if (array[mid] > x)
            r = mid - 1;
        else
            l = mid + 1;
    }

    if (r < 0)
        return array[l];
    if (l >= size_array)
        return array[r];
    return ((abs(array[l] - x) < abs(array[r] - x)) ? array[l] : array[r]);
}

void generate_sum_array(int *A, int *B, int *C, int size_array)
{
    bubble_sort(B, size_array);

    for (int i = 0; i < size_array; i++)
    {
        C[i] = A[i] + find_nearest_num(A[i], B, size_array);
    }
}
