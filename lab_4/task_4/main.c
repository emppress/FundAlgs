#include "functions.h"

int main(int argc, char **argv)
{
    FILE *input, *trace = NULL;
    String buf;
    Vector *vectors_arr;
    char check, name, base_2[BUFSIZ];

    if (argc != 2 && argc != 4)
    {
        printf("Input error.\n");
        return INPUT_ERROR;
    }
    if (argc == 4)
    {
        if (strcmp("/trace", argv[2]) || validate_file(argv))
        {
            printf("Input error.\n");
            return INPUT_ERROR;
        }
        trace = fopen(argv[3], "w");
        if (!trace)
        {
            printf("File open error\n");
            return FILE_OPEN_ERROR;
        }
    }

    input = fopen(argv[1], "r");
    if (!input)
    {
        printf("File open error\n");
        if (trace)
            fclose(trace);
        return FILE_OPEN_ERROR;
    }

    if (argc)

        vectors_arr = (Vector *)malloc(sizeof(Vector) * COUNT_VECTORS);
    if (!vectors_arr)
    {
        printf("Memory error\n");
        fclose(input);
        if (trace)
            fclose(trace);
        return MEMORY_ERROR;
    }

    init_vectors_arr(vectors_arr);

    while (!feof(input))
    {
        if (read_string(input, &buf))
        {
            write_message("Read string: memory error\n", trace);
            fclose(input);
            delete_string_content(&buf);
            free(vectors_arr);
            if (trace)
                fclose(trace);
            return MEMORY_ERROR;
        }

        if (buf.len == 0)
            break;

        if (strstr(buf.arr, "READ"))
        {
            char base[10];
            String _value;
            long _base;
            _value.arr = NULL;
            if (sscanf(buf.arr, "READ(%c, %9[^)])%c", &name, base, &check) != 3 || !isalpha(name) || check != ';' || string_to_long(base, &_base, 10) || _base < 2 || _base > 36)
            {
                write_message("Read: input error\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                delete_string_content(&_value);
                return INPUT_ERROR;
            }
            printf("Enter value (%c): ", name);
            if (read_word(&_value))
            {
                write_message("Read: memory error\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                delete_string_content(&_value);
                return MEMORY_ERROR;
            }
            if (vectors_arr[name - 'A'].init)
            {
                convert_to_bitstr(vectors_arr[name - 'A'].value, base_2);
                write_message("OPERATION READ (%c):\nbefore: %s\n", trace, name, base_2);
            }
            else
                write_message("OPETATION READ (%c):\nbefore: not init\n", trace, name);
            if (read_vector(&vectors_arr[name - 'A'], _value.arr, _base) == INPUT_ERROR)
            {
                write_message("Read: input error\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                delete_string_content(&_value);
                return INPUT_ERROR;
            }
            convert_to_bitstr(vectors_arr[name - 'A'].value, base_2);
            write_message("after: %s\n\n", trace, base_2);
            delete_string_content(&_value);
        }

        else if (strstr(buf.arr, "WRITE"))
        {
            char base[10];
            char value[128];
            long _base;
            if (sscanf(buf.arr, "WRITE(%c, %9[^)])%c", &name, base, &check) != 3 || !isalpha(name) || check != ';')
            {
                write_message("Write: input error\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                return INPUT_ERROR;
            }

            if (string_to_long(base, &_base, 10) || _base < 2 || _base > 36)
            {
                write_message("Write: input error\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                return INPUT_ERROR;
            }
            if (vectors_arr[name - 'A'].init == 0)
            {
                write_message("Write: vector missing\n\n", trace);
            }
            else
            {
                convert_to_xbase(vectors_arr[name - 'A'].value, _base, value);
                write_message("OPERATION WRITE(%c, %ld) = %s\n\n", trace, name, _base, value);
                printf("%c = %s\n", name, value);
            }
        }

        else if (buf.len == 6 && buf.arr[3] == '\\')
        {
            char name_res;
            if (sscanf(buf.arr, "%c:=\\%c%c", &name_res, &name, &check) != 3 || !isalpha(name) || check != ';' || !isalpha(name_res))
            {
                write_message("Negation: input error\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                return INPUT_ERROR;
            }
            if (vectors_arr[name - 'A'].init == 0)
            {
                write_message("Negation: vector not init\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                return MISSING;
            }
            if (vectors_arr[name_res - 'A'].init)
            {
                convert_to_bitstr(vectors_arr[name_res - 'A'].value, base_2);
                write_message("OPERATION (%c) := \\(%c):\nbefore: %s\n", trace, name_res, name, base_2);
            }
            else
                write_message("OPETATION (%c) := \\(%c):\nbefore: not init\n", trace, name_res, name);
            vectors_arr[name_res - 'A'].init = 1;
            vectors_arr[name_res - 'A'].value = ~vectors_arr[name - 'A'].value;
            convert_to_bitstr(vectors_arr[name_res - 'A'].value, base_2);
            write_message("after: %s\n\n", trace, base_2);
        }
        else
        {
            char name_1, name_2, name_res, op[4];
            if (sscanf(buf.arr, "%c:=%c%2[^A-Z]%c%c", &name_res, &name_1, op, &name_2, &check) != 5 ||
                !isalpha(name_res) || !isalpha(name_1) || !isalpha(name_2) || check != ';' ||
                (strcmp(op, "+") && strcmp(op, "&") && strcmp(op, "->") && strcmp(op, "<-") && strcmp(op, "~") && strcmp(op, "<>") && strcmp(op, "+>") && strcmp(op, "!") && strcmp(op, "?")))
            {
                write_message("Input error: command incorrect\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                return INPUT_ERROR;
            }
            if (vectors_arr[name_1 - 'A'].init == 0 || vectors_arr[name_2 - 'A'].init == 0)
            {
                write_message("Vector in expression not init\n", trace);
                fclose(input);
                delete_string_content(&buf);
                free(vectors_arr);
                if (trace)
                    fclose(trace);
                return INPUT_ERROR;
            }
            if (vectors_arr[name_res - 'A'].init)
            {
                convert_to_bitstr(vectors_arr[name_res - 'A'].value, base_2);
                write_message("OPERATION (%c) := (%c) %s (%c):\nbefore: %s\n", trace, name_res, name_1, op, name_2, base_2);
            }
            else
                write_message("OPERATION (%c) := (%c) %s (%c):\nbefore: not init\n", trace, name_res, name_1, op, name_2, name_res);

            calculate_operation(&vectors_arr[name_1 - 'A'], &vectors_arr[name_2 - 'A'], op, &vectors_arr[name_res - 'A']);
            convert_to_bitstr(vectors_arr[name_res - 'A'].value, base_2);
            write_message("after: %s\n\n", trace, base_2);
        }

        delete_string_content(&buf);
    }
    delete_string_content(&buf);
    fclose(input);
    free(vectors_arr);
    if (trace)
        fclose(trace);
    return SUCCESS;
}
