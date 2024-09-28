#include "functions.h"

int main(int argc, char *argv[])
{
    if (!((argv[1][0] == '-' || argv[1][0] == '/') && argv[1][2] == '\0'))
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    char *word = NULL;
    int len_word = 0;

    switch (argv[1][1])
    {
    case 'r':
        FILE *file1 = fopen(argv[2], "r");
        FILE *file2 = fopen(argv[3], "r");
        FILE *file_result = fopen(argv[argc - 1], "w");
        if (file1 == NULL || file2 == NULL || file_result == NULL)
        {
            printf("Ошибка открытия файла\n");
            if (file1 != NULL)
                fclose(file1);
            if (file2 != NULL)
                fclose(file2);
            if (file_result != NULL)
                fclose(file_result);
            return FILE_OPEN_ERROR;
        }

        while (!feof(file1) && !feof(file2))
        {
            if (read_word_from_file(file1, &word, &len_word) == MEMORY_ERROR)
            {
                printf("Ошибка памяти");
                if (word != NULL)
                    free(word);
                fclose(file1);
                fclose(file2);
                fclose(file_result);
                return MEMORY_ERROR;
            }

            if (len_word > 0)
            {
                fprintf(file_result, "%s ", word);
                free(word);
                word = NULL;
            }

            if (read_word_from_file(file2, &word, &len_word) == MEMORY_ERROR)
            {
                printf("Ошибка памяти");
                if (word != NULL)
                    free(word);
                fclose(file1);
                fclose(file2);
                fclose(file_result);
                return MEMORY_ERROR;
            }

            if (len_word > 0)
            {
                fprintf(file_result, "%s ", word);
                free(word);
                word = NULL;
            }
        }

        while (!feof(file1))
        {
            if (read_word_from_file(file1, &word, &len_word) == MEMORY_ERROR)
            {
                printf("Ошибка памяти");
                if (word != NULL)
                    free(word);
                fclose(file1);
                fclose(file2);
                fclose(file_result);
                return MEMORY_ERROR;
            }

            if (len_word > 0)
            {
                fprintf(file_result, "%s ", word);
                free(word);
                word = NULL;
            }
        }

        while (!feof(file2))
        {
            if (read_word_from_file(file2, &word, &len_word) == MEMORY_ERROR)
            {
                printf("Ошибка памяти");
                if (word != NULL)
                    free(word);
                fclose(file1);
                fclose(file2);
                fclose(file_result);
                return MEMORY_ERROR;
            }

            if (len_word > 0)
            {
                fprintf(file_result, "%s ", word);
                free(word);
                word = NULL;
            }
        }

        fclose(file1);
        fclose(file2);
        fclose(file_result);
        printf("Лексемы записаны в файл: %s\n", argv[argc - 1]);
        break;

    case 'a':
        FILE *input = fopen(argv[2], "r");
        FILE *output = fopen(argv[argc - 1], "w");

        if (input == NULL || output == NULL)
        {
            printf("Ошибка открытия файла\n");
            if (input != NULL)
                fclose(input);
            if (output != NULL)
                fclose(output);
            return FILE_OPEN_ERROR;
        }

        char *word_a = NULL;
        int *result_a = NULL;
        int count_world = 1;
        while (!feof(input))
        {
            if (read_word_from_file(input, &word_a, &len_word) == MEMORY_ERROR)
            {
                printf("Ошибка памяти");
                if (word != NULL)
                    free(word);
                fclose(input);
                fclose(output);
                return MEMORY_ERROR;
            }

            if (len_word > 0)
            {
                if (count_world % 10 == 0)
                {
                    if (result_a != NULL)
                        result_a = realloc(result_a, (len_word + 1) * sizeof(int));
                    else
                        result_a = (int *)malloc((len_word + 1) * sizeof(int));

                    if (result_a == NULL)
                    {
                        printf("Ошибка памяти");
                        if (word != NULL)
                            free(word);
                        fclose(input);
                        fclose(output);
                        return MEMORY_ERROR;
                    }

                    word_to_4(&word_a, len_word, &result_a);
                    for (int i = 0; i < len_word; ++i)
                    {
                        fprintf(output, "'%d'", result_a[i]);
                    }
                    fprintf(output, " ");
                }

                if (count_world % 10 != 0 && count_world % 2 == 0)
                {
                    if (result_a != NULL)
                        result_a = realloc(result_a, (len_word + 1) * sizeof(int));
                    else
                        result_a = (int *)malloc((len_word + 1) * sizeof(int));

                    if (result_a == NULL)
                    {
                        printf("Ошибка памяти");
                        if (word != NULL)
                            free(word);
                        fclose(input);
                        fclose(output);
                        return MEMORY_ERROR;
                    }

                    to_lowercase(&word_a, len_word);
                    fprintf(output, "%s ", word_a);
                }

                else if (count_world % 10 != 0 && count_world % 5 == 0)
                {
                    if (result_a != NULL)
                        result_a = realloc(result_a, (len_word + 1) * sizeof(int));
                    else
                        result_a = (int *)malloc((len_word + 1) * sizeof(int));

                    if (result_a == NULL)
                    {
                        printf("Ошибка памяти");
                        if (word != NULL)
                            free(word);
                        fclose(input);
                        fclose(output);
                        return MEMORY_ERROR;
                    }

                    word_to_8(&word_a, len_word, &result_a);
                    for (int i = 0; i < len_word; ++i)
                    {
                        fprintf(output, "'%d'", result_a[i]);
                    }
                    fprintf(output, " ");
                }

                else
                {
                    fprintf(output, "%s ", word_a);
                }

                free(word_a);
                word_a = NULL;
            }
            count_world++;
        }

        fclose(input);
        fclose(output);
        printf("Файл %s преобразован в файл %s\n", argv[2], argv[argc - 1]);
        break;

    default:
        printf("Ошибка: флага %s не существует\n", argv[1]);
        return INPUT_ERROR;
    }
}