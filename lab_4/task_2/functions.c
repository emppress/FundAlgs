#include "functions.h"

status string_to_int(const char *str, long *result)
{
    char *endinp;
    *result = strtol(str, &endinp, 10);
    if (*result == LONG_MAX || *result == LONG_MIN)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

status string_to_size_t(const char *str, size_t *result)
{
    char *endinp;
    *result = (size_t)strtoul(str, &endinp, 10);
    if (*result == ULONG_MAX || *endinp != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

void init_array(Array *arr, char name)
{
    if (!arr)
        return;

    arr->name = name;
    arr->size = arr->capacity = 0;
    arr->data = NULL;
}

void delete_array(Array *arr)
{
    if (!arr)
        return;

    free(arr->data);
    arr->data = NULL;
    arr->capacity = 0;
    arr->size = 0;
}

status load_arr(Array *arr, FILE *input)
{
    int *new_data;
    size_t capacity = 2, size = 0;
    char buf[BUFSIZ];
    long num;
    if (!arr || !input)
        return MEMORY_ERROR;

    new_data = (int *)malloc(sizeof(int) * capacity);
    if (!new_data)
        return MEMORY_ERROR;

    while ((fscanf(input, "%s", buf)) == 1)
    {
        if (string_to_int(buf, &num))
        {

            free(new_data);
            return MEMORY_ERROR;
        }
        if (size == capacity)
        {
            int *realloc_data;
            capacity *= 2;
            realloc_data = (int *)realloc(new_data, sizeof(int) * capacity);
            if (!realloc_data)
            {
                free(new_data);
                return MEMORY_ERROR;
            }
            new_data = realloc_data;
        }
        new_data[size++] = (int)num;
    }

    if (!feof(input))
    {
        free(new_data);
        return INPUT_ERROR;
    }
    free(arr->data);
    arr->data = new_data;
    arr->size = size;
    arr->capacity = capacity;
    return SUCCESS;
}

status save_arr(Array *arr, FILE *output)
{
    size_t i;
    if (!arr || !output)
        return MEMORY_ERROR;

    for (i = 0; i < arr->size; ++i)
    {
        fprintf(output, "%d ", arr->data[i]);
    }
    return SUCCESS;
}

status rand_arr(Array *arr, int count, int lb, int rb)
{
    int *new_data;
    size_t i, capacity = count, size = count;
    if (!arr)
        return MEMORY_ERROR;

    if (lb > rb || count <= 0)
        return INPUT_ERROR;

    new_data = (int *)malloc(sizeof(int) * capacity);
    if (!new_data)
        return MEMORY_ERROR;

    for (i = 0; i < size; ++i)
    {
        new_data[i] = lb + rand() % (rb - lb + 1);
    }

    free(arr->data);
    arr->data = new_data;
    arr->size = size;
    arr->capacity = capacity;
    return SUCCESS;
}

status concat_arr(Array *arr_1, Array *arr_2)
{
    size_t len, i;
    int *new_data;
    if (!arr_1 || !arr_2)
        return MEMORY_ERROR;

    len = arr_1->size + arr_2->size;

    new_data = (int *)malloc(sizeof(int) * len);
    if (!new_data)
        return MEMORY_ERROR;

    for (i = 0; i < arr_1->size; ++i)
    {
        new_data[i] = arr_1->data[i];
    }
    for (i = 0; i < arr_2->size; ++i)
    {
        new_data[i + arr_1->size] = arr_2->data[i];
    }

    free(arr_1->data);
    arr_1->data = new_data;
    arr_1->capacity = len;
    arr_1->size = len;
    return SUCCESS;
}

status remove_arr(Array *arr, size_t lb, size_t count)
{
    size_t i;
    if (!arr || lb < 0 || count < 0)
        return MEMORY_ERROR;

    if (!count)
        return SUCCESS;
    for (i = lb; i < arr->size; ++i)
    {
        if (i + count >= arr->size)
        {
            int *for_realloc;
            arr->size = i;
            arr->capacity = i;
            if (!(for_realloc = (int *)realloc(arr->data, sizeof(int) * i)))
                return MEMORY_ERROR;
            arr->data = for_realloc;
            return SUCCESS;
        }
        arr->data[i] = arr->data[i + count];
    }
    return SUCCESS;
}

status copy_arr(Array *dest, Array *sourse, size_t lb, size_t rb)
{
    size_t i, size, capacity = rb - lb + 1;
    int *new_data;
    if (!dest || !sourse)
        return MEMORY_ERROR;

    new_data = (int *)malloc(sizeof(int) * capacity);
    if (!new_data)
        return MEMORY_ERROR;

    size = 0;
    for (i = lb; i < sourse->size; ++i)
    {
        if (i > rb)
            break;
        new_data[size++] = sourse->data[i];
    }
    free(dest->data);
    dest->data = new_data;
    dest->capacity = capacity;
    dest->size = size;
    return SUCCESS;
}

int compare_a(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int compare_d(const void *a, const void *b)
{
    return (*(int *)b - *(int *)a);
}

int compare_s(const void *a, const void *b)
{
    return rand() % 3 - 1;
}

status print_one_element(Array *arr, size_t idx)
{
    if (!arr)
        return MEMORY_ERROR;

    if (idx >= arr->size)
        return INPUT_ERROR;
    printf("%d\n", arr->data[idx]);
    return SUCCESS;
}

status print_all(Array *arr)
{
    size_t i;
    if (!arr)
        return MEMORY_ERROR;

    if (arr->size == 0)
    {
        printf("Print: Array is empty\n");
        return SUCCESS;
    }
    for (i = 0; i < arr->size; ++i)
        printf("%d ", arr->data[i]);
    printf("\n");

    return SUCCESS;
}

status print_section(Array *arr, size_t lb, size_t rb)
{
    size_t i;
    if (!arr)
        return MEMORY_ERROR;

    if (arr->size == 0)
    {
        printf("Print: Array is empty\n");
        return SUCCESS;
    }

    rb = (rb < arr->size) ? (rb) : (arr->size - 1);
    for (i = lb; i <= rb; ++i)
        printf("%d ", arr->data[i]);
    printf("\n");
    return SUCCESS;
}

status print_stats(Array *arr)
{
    int *copy_data, count = 1, most_freq_num, max_count = -1;
    double avg = 0, deviation;
    size_t i;
    if (!arr)
        return MEMORY_ERROR;

    if (arr->size == 0)
    {
        printf("Stats: Arr is empty\n");
        return SUCCESS;
    }

    copy_data = (int *)malloc(sizeof(int) * arr->capacity);
    memcpy(copy_data, arr->data, sizeof(int) * arr->size);
    qsort(copy_data, arr->size, sizeof(int), compare_d);

    for (i = 0; i < arr->size - 1; ++i)
    {
        if (copy_data[i] == copy_data[i + 1])
            count += 1;
        else
        {
            if (count > max_count)
            {
                max_count = count;
                most_freq_num = copy_data[i];
            }
            count = 1;
        }
        avg += ((double)copy_data[i] / arr->size);
    }
    avg += ((double)copy_data[i] / arr->size);
    deviation = (fabs(copy_data[0] - avg) > fabs(copy_data[arr->size - 1] - avg)) ? fabs(copy_data[0] - avg) : fabs(copy_data[arr->size - 1] - avg);
    printf("----------------------------------------\n");
    printf("Stats %c:\n", arr->name);
    printf("Array size: %zu;\n", arr->size);
    printf("Max: %d; Min: %d;\n", copy_data[0], copy_data[arr->size - 1]);
    printf("The most frequent: %d, time(s): %d\n", most_freq_num, max_count);
    printf("Avg: %.3lf; Max deviation: %.3lf\n", avg, deviation);
    printf("----------------------------------------\n");
    free(copy_data);
    return SUCCESS;
}

status read_command(char **command, FILE *input)
{
    char *str, ch;
    size_t size = 0, capacity = 2;
    if (!command || !input)
        return MEMORY_ERROR;

    *command = NULL;
    ch = getc(input);
    while (ch != EOF && isspace(ch))
        ch = getc(input);

    if (ch == EOF)
        return SUCCESS;

    str = (char *)malloc(sizeof(char) * capacity);
    if (!str)
        return MEMORY_ERROR;

    while (ch != ';')
    {
        if (ch == EOF)
        {
            free(str);
            return INPUT_ERROR;
        }
        if (!isspace(ch))
        {
            if (size + 2 == capacity)
            {
                char *for_real;
                capacity *= 2;
                for_real = (char *)realloc(str, capacity * sizeof(char));
                if (!for_real)
                {
                    free(str);
                    return MEMORY_ERROR;
                }
                str = for_real;
            }
            str[size++] = ch;
        }
        ch = getc(input);
    }
    str[size++] = ';';
    str[size] = '\0';
    *command = str;
    return SUCCESS;
}

void delete_all(Array *arr)
{
    size_t i;
    if (!arr)
        return;

    for (i = 0; i < 26; ++i)
    {
        delete_array(&arr[i]);
    }
}