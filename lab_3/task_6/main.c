#include "functions.h"

int main(int argc, char **argv)
{
    External_List ex_list;
    char **files = argv + 1, user_ans;
    char v_number[BUFER_SIZE];
    if (argc < 2)
    {
        printf("Input error. Enter (program_name) (file_names_array)\n");
        return INPUT_ERROR;
    }

    init_external_list(&ex_list);
    if (scan_data_from_files(files, argc - 1, &ex_list) == MEMORY_ERROR)
    {
        printf("Memory error\n");
        return MEMORY_ERROR;
    }
    if (ex_list.size == 0)
    {
        printf("Error: void data\n");
        return INPUT_ERROR;
    }

    print_menu();
    while ((user_ans = getchar()) != '0')
    {
        if (user_ans == '\n')
            continue;
        switch (user_ans)
        {
        case '1':
        {
            size_t count_routs = 0;
            if (max_count_routs(&ex_list, &count_routs, v_number) == MEMORY_ERROR)
                printf("Memory error\n");
            else
                printf("Vehicle number: %s; count routs - %zu\n", v_number, count_routs);
            break;
        }
        case '2':
        {
            size_t count_routs = 0;
            if (min_count_routs(&ex_list, &count_routs, v_number) == MEMORY_ERROR)
                printf("Memory error\n");
            else
                printf("Vehicle number: %s; count routs - %zu\n", v_number, count_routs);
            break;
        }
        case '3':
        {
            double len_way;
            if (max_len_way(&ex_list, &len_way, v_number) == MEMORY_ERROR)
                printf("Memory error\n");
            else
                printf("Vehicle number: %s; way length - %.3lfkm\n", v_number, len_way);
            break;
        }
        case '4':
        {
            double len_way;
            if (min_len_way(&ex_list, &len_way, v_number) == MEMORY_ERROR)
                printf("Memory error\n");
            else
                printf("Vehicle number: %s; way length - %.3lfkm\n", v_number, len_way);
            break;
        }
        case '5':
        {
            double len_rout;
            if (max_length_rout(&ex_list, &len_rout, v_number) == MEMORY_ERROR)
                printf("Memory error\n");
            else
                printf("Vehicle number: %s; rout length - %.3lfkm\n", v_number, len_rout);
            break;
        }
        case '6':
        {
            double len_rout;
            if (min_length_rout(&ex_list, &len_rout, v_number) == MEMORY_ERROR)
                printf("Memory error\n");
            else
                printf("Vehicle number: %s; rout length - %.3lfkm\n", v_number, len_rout);
            break;
        }
        case '7':
        {
            unsigned int stop_time;
            max_time_stop(&ex_list, &stop_time, v_number);
            printf("Vehicle number: %s; stop time - %us\n", v_number, stop_time);
            break;
        }
        case '8':
        {
            unsigned int stop_time;
            min_time_stop(&ex_list, &stop_time, v_number);
            printf("Vehicle number: %s; stop time - %us\n", v_number, stop_time);
            break;
        }
        case '9':
        {
            unsigned int stop_time;
            if (max_time_stop_all(&ex_list, &stop_time, v_number) == MEMORY_ERROR)
                printf("Memory error\n");
            else
                printf("Vehicle number: %s; stop time - %us\n", v_number, stop_time);
            break;
        }
        }
        free_buf();
        print_menu();
    }

    destroy_external_list(&ex_list);
}