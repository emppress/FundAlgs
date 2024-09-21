#include "functions.h"

status read_num_from_file(FILE *file, char *word, int *len_word, int *base)
{
    *len_word = 0;
    *base = 2;
    char symbol = fgetc(file);
    while (symbol == ' ' || symbol == '\t' || symbol == '\n')
    {
        if (symbol == EOF)
            return OK;
        symbol = fgetc(file);
    }
    int flag = 0;
    while (symbol != ' ' && symbol != '\t' && symbol != '\n' && symbol != EOF)
    {
        if (symbol == '-' || symbol == '+')
        {
            if (*len_word != 0)
                return INPUT_ERROR;
        }

        else if (!isalnum(symbol))
            return INPUT_ERROR;

        if (isalpha(symbol))
        {
            symbol = toupper(symbol);
            flag = 1;
            if (symbol - 'A' + 10 >= *base)
            {
                *base = symbol - 'A' + 10 + 1;
                if (*base > 32)
                    return INPUT_ERROR;
            }
        }
        else if (isdigit(symbol))
        {
            if (symbol - '0' >= *base)
                *base = symbol - '0' + 1;
            if (symbol != '0')
                flag = 1;
        }
        if (symbol != '0' || flag == 1)
            word[(*len_word)++] = symbol;
        symbol = fgetc(file);
        if (*len_word == CAPACITY)
            return INPUT_ERROR;
    }
    word[*len_word] = '\000';
    return OK;
}

status convert_xbase_to_ll(const char *const word, int base, long long *result)
{

    char *endptr;
    *result = strtoll(word, &endptr, base);
    if (*result == LLONG_MAX || *result == LLONG_MIN)
        return INPUT_ERROR;
    else if (*endptr != '\000')
        return INPUT_ERROR;

    return OK;
}
