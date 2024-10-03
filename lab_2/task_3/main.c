#include "functions.h"

int main()
{
    found *res, *cur;
    status err = search_str_in_files("ggg", 2, &res, "text.txt", "b.txt");

    if (err == INPUT_ERROR)
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }

    cur = res;
    while (cur)
    {
        found *next = cur->next;
        printf("File: %s line: %d, char: %d;\n", cur->file_name, cur->n_line, cur->n_char);
        free(cur->file_name);
        free(cur);
        cur = next;
    }

    if (err == FILE_OPEN_ERROR)
    {
        printf("Some files could not be opened\n");
        return FILE_OPEN_ERROR;
    }
    if (err == MEMORY_ERROR)
    {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }
    return SUCCESS;
}
