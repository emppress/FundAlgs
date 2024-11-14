#include "functions.h"

int main(int argc, char **argv)
{
    char *command;
    int line = 0;
    size_t i;
    status state;
    Array arrays[26];
    FILE *input;

    if (argc != 2)
    {
        printf("Input error. Enter <program_name><your_program_file>\n");
        return INPUT_ERROR;
    }
    input = fopen(argv[1], "r");
    if (!input)
    {
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }

    for (i = 0; i < 26; ++i)
    {
        init_array(&arrays[i], 'A' + i);
    }

    srand(time(NULL));

    while (!feof(input))
    {
        state = read_command(&command, input);
        line++;
        if (state == INPUT_ERROR)
        {
            printf("Input error, line: %d\n", line);
            FREE_STR(command);
            delete_all(arrays);
            fclose(input);
            return INPUT_ERROR;
        }
        if (state == MEMORY_ERROR)
        {
            printf("Memory error\n");
            FREE_STR(command);
            fclose(input);
            return MEMORY_ERROR;
        }
        if (command == NULL)
            break;

        if (strstr(command, "Load"))
        {
            char arr_name, check;
            char file_path[1024];
            FILE *read_file;
            if (sscanf(command, "Load%c,%1023[^;]%c", &arr_name, file_path, &check) != 3 || !isalpha(arr_name) || check != ';')
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (!(read_file = fopen(file_path, "r")))
            {
                printf("File open error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return FILE_OPEN_ERROR;
            }
            arr_name = toupper(arr_name);
            state = load_arr(&arrays[arr_name - 'A'], read_file);
            fclose(read_file);
            if (state == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return MEMORY_ERROR;
            }
            if (state == INPUT_ERROR)
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }

        else if (strstr(command, "Save"))
        {
            char arr_name, check;
            char file_path[1024];
            FILE *save_file;
            if (sscanf(command, "Save%c,%1023[^;]%c", &arr_name, file_path, &check) != 3 || !isalpha(arr_name) || check != ';')
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (!(save_file = fopen(file_path, "w")))
            {
                printf("File open error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return FILE_OPEN_ERROR;
            }
            arr_name = toupper(arr_name);
            state = save_arr(&arrays[arr_name - 'A'], save_file);
            fclose(save_file);
        }

        else if (strstr(command, "Concat"))
        {
            char name_1, name_2, check;

            if (sscanf(command, "Concat%c,%c%c", &name_1, &name_2, &check) != 3 || !isalpha(name_1) || !isalpha(name_2) || check != ';')
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);
            name_2 = toupper(name_2);

            if (concat_arr(&arrays[name_1 - 'A'], &arrays[name_2 - 'A']))
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                fclose(input);
                FREE_STR(command);
                return MEMORY_ERROR;
            }
        }

        else if (strstr(command, "Free"))
        {
            char name_1, check;

            if (sscanf(command, "Free(%c)%c", &name_1, &check) != 2 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                fclose(input);
                FREE_STR(command);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            delete_array(&arrays[name_1 - 'A']);
        }
        else if (strstr(command, "Sort"))
        {
            char name_1, op, check;

            if (sscanf(command, "Sort%c%c%c", &name_1, &op, &check) != 3 || check != ';' || !isalpha(name_1) || (op != '+' && op != '-'))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            qsort(arrays[name_1 - 'A'].data, arrays[name_1 - 'A'].size, sizeof(int), (op == '+') ? compare_a : compare_d);
        }
        else if (strstr(command, "Shuffle"))
        {
            char name_1, check;

            if (sscanf(command, "Shuffle%c%c", &name_1, &check) != 2 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            qsort(arrays[name_1 - 'A'].data, arrays[name_1 - 'A'].size, sizeof(int), compare_s);
        }
        else if (strstr(command, "Stats"))
        {
            char name_1, check;

            if (sscanf(command, "Stats%c%c", &name_1, &check) != 2 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            if (print_stats(&arrays[name_1 - 'A']) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Rand"))
        {
            char name_1, check;
            char _lb[256], _rb[256], _count[256];
            long lb, rb, count;

            if (sscanf(command, "Rand%c,%255[0-9],%255[0-9-],%255[0-9-]%c", &name_1, _count, _lb, _rb, &check) != 5 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);

                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            if (string_to_int(_lb, &lb) || string_to_int(_rb, &rb) || string_to_int(_count, &count))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (rand_arr(&arrays[name_1 - 'A'], (int)count, (int)lb, (int)rb) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Remove"))
        {
            char name_1, check;
            char _lb[256], _count[256];
            size_t lb, count;

            if (sscanf(command, "Remove%c,%255[0-9],%255[0-9]%c", &name_1, _count, _lb, &check) != 4 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            if (string_to_size_t(_lb, &lb) || string_to_size_t(_count, &count))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (remove_arr(&arrays[name_1 - 'A'], lb, count) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Copy"))
        {
            char name_1, check, name_2;
            char _lb[256], _rb[256];
            size_t lb, rb;

            if (sscanf(command, "Copy%c,%255[0-9],%255[0-9],%c%c", &name_1, _lb, _rb, &name_2, &check) != 5 || check != ';' || !isalpha(name_1) || !isalpha(name_2))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);
            name_2 = toupper(name_2);

            if (string_to_size_t(_lb, &lb) || string_to_size_t(_rb, &rb) || lb > rb)
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (copy_arr(&arrays[name_2 - 'A'], &arrays[name_1 - 'A'], lb, rb) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Print"))
        {
            char name_1, check;
            char _lb[256], _rb[256], _pos[256];
            size_t lb, rb, pos;

            if (sscanf(command, "Print%c,%255[0-9]%c", &name_1, _pos, &check) == 3 && check == ';' && isalpha(name_1))
            {
                name_1 = toupper(name_1);
                if (string_to_size_t(_pos, &pos) || print_one_element(&arrays[name_1 - 'A'], pos) == INPUT_ERROR)
                {
                    printf("Input error, line: %d\n", line);
                    delete_all(arrays);
                    FREE_STR(command);
                    fclose(input);
                    return INPUT_ERROR;
                }
            }
            else if (sscanf(command, "Print%c,%255[0-9],%255[0-9]%c", &name_1, _lb, _rb, &check) == 4 && check == ';' && isalpha(name_1))
            {
                name_1 = toupper(name_1);
                if (string_to_size_t(_lb, &lb) || string_to_size_t(_rb, &rb) || rb < lb)
                {
                    printf("Input error, line: %d\n", line);
                    delete_all(arrays);
                    FREE_STR(command);
                    fclose(input);
                    return INPUT_ERROR;
                }
                print_section(&arrays[name_1 - 'A'], lb, rb);
            }
            else if (sscanf(command, "Print%c,all%c", &name_1, &check) == 2 && check == ';' && isalpha(name_1))
            {
                name_1 = toupper(name_1);
                print_all(&arrays[name_1 - 'A']);
            }
            else
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else
        {
            printf("Input error, line: %d\n", line);
            delete_all(arrays);
            fclose(input);
            FREE_STR(command);
            return INPUT_ERROR;
        }
        FREE_STR(command);
    }
    FREE_STR(command);
    delete_all(arrays);
    fclose(input);
}
