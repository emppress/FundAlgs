#include "functions.h"
#include "unidirectional_list.h"

int main(int argc, char **argv)
{
    UList list, list_new_data, list_old_data;
    Stack stack_modifications;
    FILE *input;
    status state;
    size_t count_modifications = 0;

    char user_ans;
    if (argc != 2)
    {
        puts("Input error. Enter (program_name) (input_file)");
        return INPUT_ERROR;
    }

    list_init(&list);
    list_init(&list_new_data);
    list_init(&list_old_data);
    stack_init(&stack_modifications);

    input = fopen("in.txt", "r");
    if (!input)
        return FILE_OPEN_ERROR;
    state = list_file_scan(&list, input);
    fclose(input);
    if (state == INPUT_ERROR)
    {
        puts("Incorrect data in the file");
        return INPUT_ERROR;
    }
    else if (state == MEMORY_ERROR)
    {
        puts("Memory error");
        return MEMORY_ERROR;
    }
    else
        puts("The file was successfully read");

    print_menu();
    while ((user_ans = getchar()) != '0')
    {
        if (user_ans == '\n')
            continue;
        switch (user_ans)
        {
        case '1':
        {
            Liver *found, search;
            if (get_liver(&search) == INPUT_ERROR)
            {
                printf("Get liver: input error! Try again\n");
                break;
            }
            if ((state = list_find(&list, &search, &found)) == MEMORY_ERROR)
            {
                printf("List find: memory error\n");
                break;
            }
            else if (state == MISSING)
            {
                printf("\nLiver is missing\n");
            }
            else
            {
                printf("\nLiver found\n");
            }
            break;
        }
        case '2':
        {
            Liver to_change, new_data;
            puts("Enter liver to change:");
            if (get_liver(&to_change) == INPUT_ERROR)
            {
                printf("Get liver: input error! Try again\n");
                break;
            }
            puts("Enter new data:");
            if (get_liver(&new_data) == INPUT_ERROR)
            {
                printf("Get liver: input error! Try again\n");
                break;
            }
            if ((state = liver_change(&list, &to_change, &new_data)) == MISSING)
            {
                printf("\nLiver is missing\n");
                break;
            }
            if (state == MEMORY_ERROR)
            {
                printf("Liver change: memory error\n");
                break;
            }
            if (state == SUCCESS)
            {
                printf("\nSuccess! Liver update\n");
                if (list_push(&list_old_data, &to_change) == MEMORY_ERROR ||
                    list_push(&list_new_data, &new_data) == MEMORY_ERROR ||
                    stack_push(&stack_modifications, '2') == MEMORY_ERROR)
                {
                    list_destroy(&list_old_data);
                    list_destroy(&list_new_data);
                    list_destroy(&list);
                    stack_destroy(&stack_modifications);
                    printf("Memory error\n");
                    return MEMORY_ERROR;
                }
                count_modifications++;
            }
            break;
        }
        case '3':
        {
            Liver new_liver;
            puts("Enter liver info:");
            if (get_liver(&new_liver) == INPUT_ERROR)
            {
                printf("Get liver: input error! Try again\n");
                break;
            }
            if (list_insert(&list, &new_liver) == MEMORY_ERROR)
            {
                printf("List_inser: Memory error\n");
                break;
            }
            printf("Success! New liver added\n");
            if (stack_push(&stack_modifications, '3') == MEMORY_ERROR ||
                list_push(&list_new_data, &new_liver) == MEMORY_ERROR)
            {
                list_destroy(&list_old_data);
                list_destroy(&list_new_data);
                list_destroy(&list);
                stack_destroy(&stack_modifications);
                printf("Memory error\n");
                return MEMORY_ERROR;
            }
            count_modifications++;
            break;
        }
        case '4':
        {
            Liver to_delete;
            puts("Enter liver info:");
            if (get_liver(&to_delete) == INPUT_ERROR)
            {
                printf("Get liver: input error! Try again\n");
                break;
            }
            if (list_delete_node(&list, &to_delete) == MISSING)
            {
                printf("Liver missing\n");
                break;
            }
            printf("Success! Liver deleted\n");
            if (list_push(&list_old_data, &to_delete) ||
                stack_push(&stack_modifications, '4'))
            {
                list_destroy(&list_old_data);
                list_destroy(&list_new_data);
                list_destroy(&list);
                stack_destroy(&stack_modifications);
                printf("Memory error\n");
                return MEMORY_ERROR;
            }
            count_modifications++;
            break;
        }
        case '5':
        {
            FILE *output;
            char out_path[BUFSIZ];
            printf("Enter out file: ");
            scanf("%s", out_path);
            output = fopen(out_path, "w");
            if (!output)
            {
                printf("File open error\n");
                break;
            }
            list_file_print(&list, output);
            fclose(output);
            printf("Success! Data uploaded\n");
            break;
        }
        case '6':
        {
            int i, count_undo;
            char c;
            Liver to_change, new_data;
            printf("Enter count undo: ");
            if (!scanf("%d", &count_undo))
            {
                printf("Input error! Try again\n");
                break;
            }
            if (count_undo < 0 || count_undo > (count_modifications / 2))
            {
                printf("You can undo %ld changes\n", count_modifications / 2);
                break;
            }

            for (i = 0; i < count_undo; ++i)
            {
                stack_pop(&stack_modifications, &c);
                if (c == '2')
                {
                    list_pop(&list_new_data, &to_change);
                    list_pop(&list_old_data, &new_data);
                    liver_change(&list, &to_change, &new_data);
                }
                else if (c == '3')
                {
                    list_pop(&list_new_data, &new_data);
                    list_delete_node(&list, &new_data);
                }
                else if (c == '4')
                {
                    list_pop(&list_old_data, &new_data);
                    if (list_insert(&list, &new_data) == MEMORY_ERROR)
                    {
                        printf("List_inser: Memory error\n");
                        stack_destroy(&stack_modifications);
                        list_destroy(&list);
                        list_destroy(&list_old_data);
                        list_destroy(&list_new_data);
                        return MEMORY_ERROR;
                    }
                }
            }
            printf("Undo success. Cancelled %ld modifications\n", count_modifications / 2);
            count_modifications = 0;
            list_destroy(&list_old_data);
            list_destroy(&list_new_data);
            stack_destroy(&stack_modifications);
            break;
        }
        }
        free_buf();
        print_menu();
    }
    stack_destroy(&stack_modifications);
    list_destroy(&list);
    list_destroy(&list_old_data);
    list_destroy(&list_new_data);
    return 0;
}
