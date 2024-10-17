#include "functions.h"

int main(int argc, char **argv)
{
    int size_data;
    if (validate_input(argc, argv))
    {
        printf("You need to enter 3 arguments (file_in)(flag(-a /a -d /d))(file_out)\n");
        return INPUT_ERROR;
    }
    Employee *data = NULL;

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[3], "w");
    if (!input || !output)
    {
        if (!input)
            fclose(input);
        if (!output)
            fclose(output);
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }
    scan_employee_from_file(&data, input, &size_data);
    qsort(data, size_data, sizeof(Employee), compare_a);
    for (int i = 0; i < size_data; i++)
    {
        fprintf(output, "%u %s %s %lf\n", data[i].id, data[i].name, data[i].surname, data[i].salary);
    }
    free(data);
    fclose(input);
    fclose(output);
}
