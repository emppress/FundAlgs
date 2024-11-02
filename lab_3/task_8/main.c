#include "functions.h"

int main(int argc, char **argv)
{
    FILE *input;
    char command[5];
    char ch;
    int count;
    Polynom adder, polynom_1, polynom_2, res;
    status state;
    if (argc != 2)
    {
        printf("Input error. Enter (program_file) (input_file)\n");
        return INPUT_ERROR;
    }

    init_polynom(&adder);
    input = fopen(argv[1], "r");
    if (!input)
    {
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }

    while (!feof(input))
    {
        command[0] = '\0';
        ch = getc(input);
        if (isspace(ch))
            continue;
        if (ch == '%')
        {
            while (ch != EOF && ch != '\n')
                ch = getc(input);
            continue;
        }
        if (ch == '[')
        {
            while (ch != EOF && ch != ']')
                ch = getc(input);
            if (ch == EOF)
            {
                fclose(input);
                destroy_polynom(&adder);
                printf("Input error\n");
                return INPUT_ERROR;
            }
            continue;
        }
        if (ch == 'A' || ch == 'S' || ch == 'M' || ch == 'D' || ch == 'E' || ch == 'C')
        {
            command[0] = ch;
            count = 0;
            fscanf(input, "%[^(]5s", command + 1);
            if (strcmp(command, "Add") && strcmp(command, "Sub") &&
                strcmp(command, "Mult") && strcmp(command, "Div") &&
                strcmp(command, "Mod") && strcmp(command, "Eval") &&
                strcmp(command, "Diff") && strcmp(command, "Cmps"))
            {
                printf("Input error\n");
                destroy_polynom(&adder);
                fclose(input);
                return INPUT_ERROR;
            }
            if (strcmp(command, "Eval") && strcmp(command, "Diff"))
            {
                init_polynom(&polynom_1);
                init_polynom(&polynom_2);
                init_polynom(&res);
                state = get_expression(&polynom_1, &polynom_2, input, &count);
                if (state)
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&adder);
                    fclose(input);
                    if (state == MEMORY_ERROR)
                    {
                        printf("Memory error\n");
                        return MEMORY_ERROR;
                    }
                    printf("Input error\n");
                    return INPUT_ERROR;
                }
                if (getc(input) != ';')
                {
                    destroy_polynom(&adder);
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    fclose(input);
                    printf("Input error\n");
                    return INPUT_ERROR;
                }
            }
        }
        if (!strcmp(command, "Add"))
        {
            if (count == 1)
            {
                if (add(&adder, &polynom_1))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                destroy_polynom(&polynom_1);
            }
            if (count == 2)
            {
                if (state = add(&polynom_1, &polynom_2))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                destroy_polynom(&adder);
                adder = polynom_1;
            }
            destroy_polynom(&polynom_2);
            printf("Add result: ");
            print_polynom(&adder);
        }
        else if (!strcmp(command, "Sub"))
        {

            if (count == 1)
            {
                if (sub(&adder, &polynom_1))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                destroy_polynom(&polynom_1);
            }
            if (count == 2)
            {
                if (state = sub(&polynom_1, &polynom_2))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                destroy_polynom(&adder);
                adder = polynom_1;
            }
            destroy_polynom(&polynom_2);
            printf("Sub result: ");
            print_polynom(&adder);
        }
        else if (!strcmp(command, "Mult"))
        {
            if (count == 1)
            {
                if (mult(&adder, &polynom_1, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
            }
            if (count == 2)
            {
                if (state = mult(&polynom_1, &polynom_2, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
            }
            destroy_polynom(&polynom_1);
            destroy_polynom(&polynom_2);
            destroy_polynom(&adder);
            adder = res;
            printf("Mult result: ");
            print_polynom(&adder);
        }
        else if (!strcmp(command, "Div"))
        {
            if (count == 1)
            {
                if (div_mod(&adder, &polynom_1, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
            }
            if (count == 2)
            {
                if (state = div_mod(&polynom_1, &polynom_2, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
            }
            destroy_polynom(&polynom_1);
            destroy_polynom(&polynom_2);
            destroy_polynom(&adder);
            adder = res;
            printf("Div result: ");
            print_polynom(&adder);
        }
        else if (!strcmp(command, "Mod"))
        {
            if (count == 1)
            {
                if (div_mod(&adder, &polynom_1, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                destroy_polynom(&polynom_1);
            }
            if (count == 2)
            {
                if (state = div_mod(&polynom_1, &polynom_2, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                destroy_polynom(&adder);
                adder = polynom_1;
            }
            destroy_polynom(&polynom_2);
            destroy_polynom(&res);
            printf("Mod result: ");
            print_polynom(&adder);
        }
        else if (!strcmp(command, "Cmps"))
        {
            if (count == 1)
            {
                if (cmps(&adder, &polynom_1, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
            }
            if (count == 2)
            {
                if (state = cmps(&polynom_1, &polynom_2, &res))
                {
                    destroy_polynom(&polynom_1);
                    destroy_polynom(&polynom_2);
                    destroy_polynom(&res);
                    destroy_polynom(&adder);
                    fclose(input);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
            }
            destroy_polynom(&polynom_1);
            destroy_polynom(&polynom_2);
            destroy_polynom(&adder);
            adder = res;
            printf("Cmps result: ");
            print_polynom(&adder);
        }
        else if (!strcmp(command, "Eval"))
        {
            double x, res_double;
            state = SUCCESS;
            if (getc(input) != '(')
                state = INPUT_ERROR;
            if (fscanf(input, "%lf", &x) != 1 || getc(input) != ')' || getc(input) != ';' || state || eval(&adder, x, &res_double))
            {
                destroy_polynom(&adder);
                fclose(input);
                printf("Input error\n");
                return INPUT_ERROR;
            }
            printf("Eval result: %lf\n", res_double);
        }
        else if (!strcmp(command, "Diff"))
        {
            state = get_expression(&polynom_1, &polynom_2, input, &count);
            if (state == INPUT_ERROR || state == MEMORY_ERROR)
            {
                destroy_polynom(&polynom_1);
                destroy_polynom(&adder);
                fclose(input);
                if (state == MEMORY_ERROR)
                {
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                printf("Input error\n");
                return INPUT_ERROR;
            }
            if (state == ZERO_ARGS)
                state = diff(&adder, &res);
            else
                state = diff(&polynom_1, &res);
            if (state)
            {
                destroy_polynom(&polynom_1);
                destroy_polynom(&adder);
                destroy_polynom(&res);
                fclose(input);
                printf("Memory error\n");
                return MEMORY_ERROR;
            }
            destroy_polynom(&adder);
            destroy_polynom(&polynom_1);
            destroy_polynom(&polynom_2);
            adder = res;
            printf("Diff result: ");
            print_polynom(&adder);
        }
    }
    destroy_polynom(&adder);
    destroy_polynom(&polynom_1);
    destroy_polynom(&polynom_2);
    fclose(input);
}