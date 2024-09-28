#include "functions.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }
    if (!((argv[1][0] == '-' || argv[1][0] == '/') && (argv[1][2] == '\0' || argv[1][3] == '\0')) || strlen(argv[1]) < 2 || strlen(argv[1]) > 3)
    {
        printf("Ошибка ввода\n");
        return INPUT_ERROR;
    }

    FILE *input = fopen(argv[2], "r");
    if (input == NULL)
    {
        printf("Ошибка открытия файла\n");
        return FILE_OPEN_ERROR;
    }

    FILE *output = NULL;
    char *outfile_path = NULL;
    int len_flag = strlen(argv[1]);

    if (len_flag == 3)
    {
        if (argc != 4)
        {
            printf("Ошибка ввода\n");
            fclose(input);
            return INPUT_ERROR;
        }
        else if (argv[1][1] == 'n')
        {
            outfile_path = (char *)malloc((strlen(argv[3])) * sizeof(char));
            if (outfile_path == NULL)
            {
                printf("Ошибка выделения памяти\n");
                fclose(input);
                return MEMORY_ERROR;
            }
            strcpy(outfile_path, argv[3]);
            output = fopen(argv[3], "w");
            if (output == NULL)
            {
                printf("Ошибка открытия файла\n");
                fclose(input);
                return FILE_OPEN_ERROR;
            }
        }
        else
        {
            printf("Ошибка ввода\n");
            fclose(input);
            return INPUT_ERROR;
        }
    }
    else
    {
        if (argc != 3)
        {
            printf("Ошибка ввода\n");
            fclose(input);
            return INPUT_ERROR;
        }
        outfile_path = (char *)malloc((strlen(argv[2]) + 6) * sizeof(char));
        if (outfile_path == NULL)
        {
            printf("Ошибка выделения памяти\n");
            fclose(input);
            return MEMORY_ERROR;
        }
        char *end_slesh = strrchr(argv[2], '/');
        if (end_slesh == NULL)
        {
            strcpy(outfile_path, "out_");
            strcat(outfile_path, argv[2]);
        }
        else
        {
            int len_end = strlen(end_slesh);
            strncpy(outfile_path, argv[2], strlen(argv[2]) - len_end);
            outfile_path[strlen(argv[2]) - len_end] = '\000';
            strcat(outfile_path, "/out_");
            strcat(outfile_path, end_slesh + 1);
        }

        output = fopen(outfile_path, "w");
        if (output == NULL)
        {
            printf("Ошибка открытия файла\n");
            fclose(input);
            return FILE_OPEN_ERROR;
        }
    }

    switch (argv[1][len_flag - 1])
    {
    case 'd':
        exclude_nums(input, output);
        printf("Арабские цифры удалены,\nадрес файла с результатом: %s", outfile_path);
        break;

    case 'i':
        count_letters(input, output);
        printf("Количество латинских букв в строке подсчитано,\nадрес файла с результатом: %s", outfile_path);
        break;
    case 's':
        count_symbols(input, output);
        printf("Количество символов подсчитано,\nадрес файла с результатом: %s", outfile_path);
        break;
    case 'a':
        replace_symbols(input, output);
        printf("Символы заменены ASCII кодом в 16СС,\nадрес файла с результатом: %s", outfile_path);
        break;
    default:
        printf("Ошибка: флага %s не существует\n", argv[1]);
        fclose(input);
        fclose(output);
        return INPUT_ERROR;
    }

    fclose(input);
    fclose(output);
    free(outfile_path);
    return 0;
}