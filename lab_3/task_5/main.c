#include "functions.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Input error: enter (program_name) (input_file)\n");
        return INPUT_ERROR;
    }
    Student *arr = NULL, *found, **found_arr;
    char user_ans;
    double average;
    size_t size, found_size;
    FILE *input = fopen(argv[1], "r");
    if (!input)
        return MEMORY_ERROR;

    if (list_file_scan(&arr, input, &size, &average) == MEMORY_ERROR)
    {
        fclose(input);
        return MEMORY_ERROR;
    }
    fclose(input);

    printf("The file was read successfully\n");

    print_menu();
    while ((user_ans = getchar()) != '0')
    {
        if (user_ans == '\n')
            continue;
        switch (user_ans)
        {
        case '1':
        {
            Student *found;
            char _id[BUFF_SIZE];
            ull id;
            printf("Enter ID: ");
            if (scanf("%255s", _id) != 1 || string_to_ull(_id, &id))
            {
                printf("Input error! Try again\n");
                break;
            }

            if (id_search(arr, size, id, &found) == MISSING)
            {
                printf("Student not found\n");
                break;
            }
            student_print(found);
            break;
        }

        case '2':
        {
            Student **found_arr = NULL;
            char surname[BUFF_SIZE];
            size_t found_size, i;
            char c = 0;
            printf("Enter mod:\ns - surname, n - name, g - group\n>>> ");
            c = getchar();
            while (isspace(c))
                c = getchar();
            if (c != 's' && c != 'n' && c != 'g')
            {
                printf("Input error. Try again\n");
                break;
            }
            printf("Enter search key: ");
            scanf("%255s", surname);
            if (surname_or_name_search(arr, size, surname, &found_arr, &found_size, c) == MEMORY_ERROR)
            {
                printf("Memory error\n");
                break;
            }
            if (found_size == 0)
            {
                printf("Student not found\n");
                free(found_arr);
                break;
            }
            for (i = 0; i < found_size; ++i)
            {
                student_print(found_arr[i]);
            }
            free(found_arr);
            break;
        }
        case '3':
        {
            qsort(arr, size, sizeof(Student), id_cmp);
            break;
        }
        case '4':
        {
            qsort(arr, size, sizeof(Student), surname_cmp);
            break;
        }
        case '5':
        {
            qsort(arr, size, sizeof(Student), name_cmp);
            break;
        }
        case '6':
        {
            qsort(arr, size, sizeof(Student), group_cmp);
            break;
        }
        case '7':
        {
            char filename[BUFF_SIZE];
            FILE *output;
            printf("Enter file: ");
            scanf("%255s", filename);
            output = fopen(filename, "w");
            if (!output)
            {
                printf("File open eror\n");
                break;
            }
            avg_search_print_to_trace_file(arr, size, average, output);
            fclose(output);
            break;
        }
        case '8':
        {
            Student *found;
            char filename[BUFF_SIZE];
            FILE *output;
            char _id[BUFF_SIZE];
            ull id;

            printf("Enter file: ");
            scanf("%255s", filename);
            output = fopen(filename, "w");
            if (!output)
            {
                printf("File open eror\n");
                break;
            }
            printf("Enter ID: ");
            if (scanf("%255s", _id) != 1 || string_to_ull(_id, &id))
            {
                printf("Input error! Try again\n");
                fclose(output);
                break;
            }

            if (id_search(arr, size, id, &found) == MISSING)
            {
                printf("Student not found\n");
                fclose(output);
                break;
            }
            student_file_print(found, output);
            fclose(output);
            break;
        }
        }
        free_buf();
        //    print_menu();
    }
    delete_student_arr(arr, size);
}
