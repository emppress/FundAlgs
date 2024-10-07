#include "functions.h"

int my_strlen(const char *str)
{
    const char *start = str;
    while (*str++)
        ;
    return str - start - 1;
}

status add_new_found(found **head, char *file_name, int len_filename, int n_line, int n_char)
{
    found *new_elem = (found *)malloc(sizeof(found));
    if (!new_elem)
        return MEMORY_ERROR;
    new_elem->file_name = (char *)malloc((len_filename + 1) * sizeof(char));
    if (!new_elem->file_name)
    {
        free(new_elem);
        return MEMORY_ERROR;
    }

    for (int i = 0; i <= len_filename; i++)
    {
        new_elem->file_name[i] = file_name[i];
    }

    new_elem->n_char = n_char;
    new_elem->n_line = n_line;
    new_elem->next = NULL;

    if (!(*head))
    {
        *head = new_elem;
    }
    else
    {
        found *cur = (*head);
        while (cur->next)
        {
            cur = cur->next;
        }
        cur->next = new_elem;
    }
    return SUCCESS;
}

status search_str_in_files(char *substring, int count_files, found **result, ...)
{
    int len_substr = my_strlen(substring);
    *result = NULL;
    if (count_files < 1 || !len_substr)
        return INPUT_ERROR;

    va_list args;
    va_start(args, result);

    status state = SUCCESS;
    char c, *name_file;
    FILE *file;
    int idx_substr, n_line, n_char, n_line_answ, n_char_answ, count_new_str;

    for (int i = 0; i < count_files; i++)
    {
        n_line = 1, n_char = 0;
        name_file = va_arg(args, char *);
        int len_filename = my_strlen(name_file);
        if (!(file = fopen(name_file, "r")))
        {
            va_end(args);
            state = FILE_OPEN_ERROR;
        }

        idx_substr = 0;
        while ((c = getc(file)) != EOF)
        {
            n_char++;
            if (c == '\n')
            {
                n_line++;
            }
            if (c == substring[idx_substr])
            {
                idx_substr++;
                if (idx_substr == 1)
                {
                    n_line_answ = n_line;
                    n_char_answ = n_char;
                }
                if (idx_substr == len_substr)
                {
                    if (add_new_found(result, name_file, len_filename, n_line_answ, n_char_answ) == MEMORY_ERROR)
                        return MEMORY_ERROR;
                    fseek(file, -idx_substr + 1, SEEK_CUR);
                    count_new_str = 0;
                    for (int j = 0; j < idx_substr; j++)
                    {
                        if (substring[j] == '\n')
                            count_new_str++;
                    }
                    n_char -= (idx_substr - 1);
                    n_line -= count_new_str;
                    idx_substr = 0;
                }
            }

            else if (idx_substr)
            {
                fseek(file, -idx_substr + 1, SEEK_CUR);
                count_new_str = 0;
                for (int j = 0; j < idx_substr; j++)
                {
                    if (substring[j] == '\n')
                        count_new_str++;
                }
                if (c == '\n')
                    count_new_str++;
                n_char -= (idx_substr - 1);
                idx_substr = 0;
                n_line -= count_new_str;
            }
            if (c == '\n')
            {
                n_char = 0;
            }
        }

        fclose(file);
    }
    va_end(args);
    return state;
}