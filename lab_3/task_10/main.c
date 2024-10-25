#include "functions.h"

int main(int argc, char **argv)
{
    FILE *input, *output;
    if (validate_input(argc, argv))
    {
        puts("Input error! Enter (program_name) (file_in) (file_out)");
        return INPUT_ERROR;
    }

    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if (build_tree(input, output) == MEMORY_ERROR)
    {
        puts("Build_tree memory error");
        return MEMORY_ERROR;
    }
    fclose(input);
    fclose(output);

    return 0;
}