#include "functions.h"

int main(int argc, char **argv)
{
    Tree tree;
    char separators[128], user_ans;
    int i;
    if (argc < 3)
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }
    for (i = 2; i < argc; ++i)
    {
        if (strlen(argv[i]) != 1)
        {
            printf("Input separators error\n");
            return INPUT_ERROR;
        }
        separators[i - 2] = argv[i][0];
    }
    separators[i - 2] = '\0';

    FILE *input = fopen(argv[1], "r");
    if (!input)
        return FILE_OPEN_ERROR;

    init_tree(&tree);
    if (build_tree_from_file(&tree, separators, input))
    {
        delete_tree(&tree);
        fclose(input);
        printf("Memory error\n");
        return MEMORY_ERROR;
    }
    fclose(input);

    if (print_n_most_frequent_words(&tree, -1) == MEMORY_ERROR)
    {
        delete_tree(&tree);
        printf("Memory error\n");
        return MEMORY_ERROR;
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
            char word[BUFSIZ];
            Node *found;
            printf("Enter word: ");
            scanf("%s", word);
            if (find_string(word, &tree, &found) == MISSING)
            {
                printf("Word missing in the text\n");
                break;
            }
            printf("%s - %zu time(s)\n", word, found->count_repeats);
            break;
        }
        case '2':
        {
            int n;
            printf("Enter count: ");
            if (scanf("%d", &n) != 1)
            {
                printf("Input error. Try again\n");
                break;
            }
            if (print_n_most_frequent_words(&tree, n) == MEMORY_ERROR)
            {
                printf("Memory error detected. Try again later\n");
            }
            break;
        }
        case '3':
        {
            char word[BUFSIZ];
            if (find_longest_word(&tree, word) == MISSING)
            {
                printf("File is empty\n");
                break;
            }
            printf("Found - %s\n", word);
            break;
        }
        case '4':
        {
            char word[BUFSIZ];
            if (find_shortest_word(&tree, word) == MISSING)
            {
                printf("File is empty\n");
                break;
            }
            printf("Found - %s\n", word);
            break;
        }
        case '5':
        {
            int depth;
            if ((depth = max_depth(tree.root)) == -1)
            {
                printf("Tree is empty\n");
                break;
            }
            printf("Depth: %d\n", depth);
            break;
        }
        case '6':
        {
            char file_path[BUFSIZ];
            FILE *output;
            printf("Enter file name: ");
            scanf("%s", file_path);
            printf("%s\n", file_path);
            output = fopen(file_path, "w");
            if (!output)
            {
                printf("File open error. Try again\n");
                break;
            }
            upload_tree_to_file(&tree, output, separators[0]);
            printf("Success. WARNING: TREE DELETED!\n");
            fclose(output);
            delete_tree(&tree);
            init_tree(&tree);
            break;
        }
        case '7':
        {
            char file_path[BUFSIZ];
            printf("Enter file name: ");
            scanf("%s", file_path);
            input = fopen(file_path, "r");
            if (!input)
            {
                printf("File open error. Try again\n");
                break;
            }
            if (build_tree_from_file(&tree, separators, input) == MEMORY_ERROR)
            {
                fclose(input);
                delete_tree(&tree);
                printf("Memory error\n");
                return MEMORY_ERROR;
            }
            printf("Success\n");
            fclose(input);
            break;
        }
        }
        free_buf();
        print_menu();
    }
    delete_tree(&tree);
}