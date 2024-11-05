#include "functions.h"

int main(int argc, char **argv)
{
    Hash_table hash_table;
    FILE *input, *output;
    char *outname;
    if (argc < 2)
    {
        printf("Input error. Enter <program_name> <input_file>\n");
        return INPUT_ERROR;
    }
    outname = (char *)malloc(sizeof(char) * 30);
    if (!outname)
    {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }
    create_out_name(argv[1], outname);

    input = fopen(argv[1], "r");
    if (!input)
    {
        free(outname);
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }

    output = fopen(outname, "w");
    free(outname);
    if (!output)
    {
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }
    if (hash_table_init(&hash_table))
    {
        printf("Memory error\n");
        fclose(input);
        fclose(output);
        return MEMORY_ERROR;
    }

    String buf;
    char last_char;
    status state;
    if (state = read_define(&hash_table, input, &buf, &last_char))
    {

        fclose(input);
        fclose(output);
        hash_table_delete(&hash_table);
        delete_string_content(&buf);
        if (state == MEMORY_ERROR)
        {
            printf("Memory error\n");
            return MEMORY_ERROR;
        }
        printf("Define input error\n");
        return INPUT_ERROR;
    }

    if (last_char != EOF)
    {
        if (replace_text(&hash_table, input, output, &buf, last_char) == MEMORY_ERROR)
        {
            printf("Memory error\n");
            fclose(input);
            fclose(output);
            hash_table_delete(&hash_table);
            delete_string_content(&buf);
            return MEMORY_ERROR;
        }
    }
    hash_table_delete(&hash_table);
    fclose(input);
    fclose(output);
    return 0;
}