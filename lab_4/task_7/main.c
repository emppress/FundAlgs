#include "functions.h"

int main(int argc, char **argv)
{
    Variables vars;
    FILE *input;
    String line;
    status err;
    size_t number_of_string = 0;

    input = fopen("in.txt", "r");
    if (!input)
    {
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }

    if (init_variables(&vars) == MEMORY_ERROR)
    {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }

    while (!feof(input))
    {
        number_of_string++;
        if (err = read_string(input, &line))
        {
            break;
        }

        if (line.len == 0)
            break;

        if (!strcmp(line.arr, "print"))
        {
            printf("print all: ");
            print_all(&vars);
            putchar('\n');
        }
        else if (strstr(line.arr, "print"))
        {
            size_t i;
            String var_name;
            if (err = string_init(&var_name))
                break;
            for (i = 5; i < line.len; ++i)
            {
                if (err = add_char_to_string(&var_name, line.arr[i]))
                {
                    delete_string_content(&var_name);
                    break;
                }
            }

            if (err)
            {
                delete_string_content(&var_name);
                break;
            }
            if (err = print_var(var_name.arr, &vars))
            {
                delete_string_content(&var_name);
                break;
            }
            delete_string_content(&var_name);
        }
        else
        {
            if (err = execute_expression(&line, &vars))
                break;
        }
        delete_string_content(&line);
    }

    delete_string_content(&line);
    delete_variables(&vars);
    fclose(input);
    if (err == MEMORY_ERROR)
    {
        printf("*** Memory error. Line: %zu ***\n", number_of_string);
        return MEMORY_ERROR;
    }
    if (err == INPUT_ERROR)
    {
        printf("*** Input error. Line: %zu ***\n", number_of_string);
        return MEMORY_ERROR;
    }
    if (err == OVERFLOW_ERROR)
    {
        printf("*** Overflow error. Line: %zu ***\n", number_of_string);
        return MEMORY_ERROR;
    }
    if (err == DIVISION_BY_ZERO_ERROR)
    {
        printf("*** Division by zero. Line: %zu ***\n", number_of_string);
        return MEMORY_ERROR;
    }
    return SUCCESS;
}
