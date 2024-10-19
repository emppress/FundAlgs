#include "functions.h"
#include "my_string.h"

int main(int argc, char **argv)
{
    {
        puts("_______________________________Test string library_______________________________\n");
        String str_1, *str_2, str_3;
        str_1 = string_create("string check");
        if (str_1.size == -1)
        {
            printf("str_1: Memory error\n");
            return MEMORY_ERROR;
        }

        str_2 = string_dup_new_dynamic(&str_1);
        if (!str_2)
        {
            printf("str_2: Memory error\n");
            delete_string_content(&str_1);
            return MEMORY_ERROR;
        }

        if (string_dup(&str_3, str_2))
        {
            printf("str_3: Memory error\n");
            delete_string_content(&str_1);
            str_2 = string_destroy(str_2);
            return MEMORY_ERROR;
        }
        printf("str_1: %d %s\nstr_2: %d %s\nstr_3: %d %s\n", str_1.size, str_1.arr, str_2->size, str_2->arr, str_3.size, str_3.arr);

        if (strings_concatenation(str_2, &str_3) != SUCCESS)
        {
            printf("concatenation: Memory error\n");
            delete_string_content(&str_1);
            str_2 = string_destroy(str_2);
            delete_string_content(&str_3);
            return MEMORY_ERROR;
        }
        printf("concatenation str_2 & str_3: %d %s\n", str_2->size, str_2->arr);

        printf("compare str_1 & str_2: %d\n", string_compare(&str_1, str_2));                            // str_1 < str_2 (ans -12)
        printf("equivalence_relation str_1 & str_3: %d\n", string_equivalence_relation(&str_1, &str_3)); // ans: 1

        string_destroy(str_2);
        delete_string_content(&str_1);
        delete_string_content(&str_3);
        puts("_____________________________________end test_____________________________________\n\n");
    }

    puts("__________________________________Mail assistant__________________________________\n");

    char user_ans;
    Post post;
    status state;
    int count_mails = 0;
    if (post_init(&post))
    {
        printf("Address: Memory error\n");
        ;
        return MEMORY_ERROR;
    }
    printf("Enter the address of the current post office:\n");
    while (state = get_address(post.address_ptr))
    {
        if (state == MEMORY_ERROR)
        {
            printf("Address: Memory error\n");
            ;
            return MEMORY_ERROR;
        }
        printf("\nThe data is incorrect!\n");
        printf("Enter the address of the current post office:\n");
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
            if ((state = add_mail(&post, &count_mails)) == MEMORY_ERROR)
            {
                printf("Add_mail: memory error");
            }
            else if (state == INPUT_ERROR)
                printf("\nThe data is incorrect! Try again\n");
            else
            {
                printf("success\n");
            }
            qsort(post.mail_arr, count_mails, sizeof(Mail), compare_mails);
            break;
        }

        case '2':
        {
            if ((state = delete_mail(&post, &count_mails)) == MEMORY_ERROR)
            {
                printf("Add_mail: memory error");
            }
            else if (state == INPUT_ERROR)
                printf("\nThe data is incorrect! Try again\n");
            else
            {
                printf("success\n");
            }
            break;
        }
        case '3':
        {
            if ((state = search_mail(&post, &count_mails)) == MEMORY_ERROR)
            {
                printf("Search_mail: memory error");
            }
            else if (state == INPUT_ERROR)
                printf("\nThe data is incorrect! Try again\n");
            else
            {
                printf("success\n");
            }
            break;
        }
        case 'd':
        {
            if ((state = set_delivered(&post, &count_mails)) == MEMORY_ERROR)
            {
                printf("Set_delivered: memory error");
            }
            else if (state == INPUT_ERROR)
                printf("\nThe data is incorrect! Try again\n");
            else
            {
                printf("success\n");
            }
            break;
        }
        case '4':
        {
            int count_delivered;
            Mail *arr_delivered = NULL;
            if (search_delivered(&post, count_mails, &count_delivered, &arr_delivered))
            {
                printf("Search_delivered error\n");
            }
            for (int i = 0; i < count_delivered; ++i)
            {
                print_mail_info(&arr_delivered[i]);
                printf("\n");
            }
            free(arr_delivered);
            if (!count_delivered)
                puts("No delivered");
            break;
        }

        case '5':
        {
            int count_overdue;
            Mail *arr_overdue = NULL;
            if (search_overdue(&post, count_mails, &count_overdue, &arr_overdue))
            {
                printf("Search_delivered error\n");
            }
            for (int i = 0; i < count_overdue; ++i)
            {
                print_mail_info(&arr_overdue[i]);
                printf("\n");
            }
            free(arr_overdue);
            if (!count_overdue)
                puts("No overdue");
            break;
        }
        case '6':
        {
            if (!compare_mails)
                puts("No mails");
            for (int i = 0; i < count_mails; ++i)
            {
                printf("____________Mail_%s____________\n", post.mail_arr[i].postal_index.arr);
                print_mail_info(&post.mail_arr[i]);
                putchar('\n');
            }
            break;
        }
        }
        print_menu();
    }
    clear_post(&post, count_mails);
    return 0;
}
