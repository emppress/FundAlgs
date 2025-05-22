#include "functions.h"

int main(int argc, char *argv[])
{
    int n, i;
    size_t count = 0, n_copy;
    uint64_t result_xor;
    const char *command = argv[argc - 1];

    if (argc < 3)
    {
        printf("Input error. Enter <file1> <file2> ... <flag> [arg]\n");
        return INPUT_ERROR;
    }

    if (!strcmp(argv[argc - 2], "mask"))
    {
        for (i = 1; i < argc - 2; ++i)
        {
            switch (mask(argv[i], argv[argc - 1], &count))
            {
            case INPUT_ERROR:
            {
                printf("Mask [%s]: input error\n", argv[i]);
                return INPUT_ERROR;
            }
            case FILE_OPEN_ERROR:
            {
                printf("Mask [%s]: file open error\n", argv[i]);
                break;
            }
            case SUCCESS:
            {
                printf("Mask [%s] result: %zu\n", argv[i], count);
                break;
            }
            default:
            {
                printf("Mask: memory error\n");
            }
            }
        }
    }

    else if (!strcmp(argv[argc - 2], "find"))
    {
        char *find_in, flag;
        find_in = (char *)malloc(sizeof(char) * argc - 3);
        if (!find_in)
        {
            printf("Memory error\n");
            return MEMORY_ERROR;
        }

        switch (find_string((const char **)argv + 1, argc - 3, argv[argc - 1], find_in, &flag))
        {
        case MEMORY_ERROR:
        {
            printf("Find: memory error\n");
            free(find_in);
            return MEMORY_ERROR;
        }
        case FILE_OPEN_ERROR:
        {
            printf("Find: file open error\n");
            free(find_in);
            return FILE_OPEN_ERROR;
        }
        case FORK_ERROR:
        {
            printf("Find: fork error\n");
            free(find_in);
            return FORK_ERROR;
        }
        case SUCCESS:
        {
            for (i = 0; i < argc - 3; ++i)
            {
                printf("%s - %s\n", argv[i + 1], (flag && find_in[i]) ? "FOUND" : "NOT FOUND");
            }
        }
        }
        free(find_in);
    }

    else if (!strncmp(command, "xor", 3))
    {
        n = command[3] - '0';
        if (n < 2 || n > 6)
        {
            printf("Input error. N: 2-6\n");
            return INPUT_ERROR;
        }

        for (i = 1; i < argc - 1; ++i)
        {
            switch (xor_blocks(argv[i], n, &result_xor))
            {
            case INPUT_ERROR:
            {
                printf("Xor [%s]: input error\n", argv[i]);
                return INPUT_ERROR;
            }
            case FILE_OPEN_ERROR:
            {
                printf("Xor [%s]: file open error\n", argv[i]);
                break;
            }
            case SUCCESS:
            {
                printf("Xor [%s] result: %lu\n", argv[i], result_xor);
                break;
            }
            default:
            {
                printf("Xor: memory error\n");
            }
            }
        }
    }

    else if (!strncmp(command, "copy", 4))
    {

        if (command[4] == '\0' || string_to_size_t(command + 4, &n_copy) || n_copy == 0)
        {
            printf("Copy: input N error\n");
            return INPUT_ERROR;
        }

        for (i = 1; i < argc - 1; ++i)
        {
            switch (copy_file(argv[i], n_copy))
            {
            case INPUT_ERROR:
            {
                printf("Copy [%s]: input error\n", argv[i]);
                return INPUT_ERROR;
            }
            case FILE_OPEN_ERROR:
            {
                printf("Copy [%s]: file open error\n", argv[i]);
                break;
            }
            case SUCCESS:
            {
                printf("Copy [%s]: SUCCESS\n", argv[i]);
                break;
            }
            case FORK_ERROR:
            {
                printf("Copy [%s]: fork error\n", argv[i]);
                break;
            }

            case RW_ERROR:
            {
                printf("Copy [%s]: read/write error\n", argv[i]);
                break;
            }
            default:
            {
                printf("Copy: memory error\n");
            }
            }
        }
    }

    else
    {
        printf("Input error. Enter <file1> <file2> ... <flag> [arg]\n");
        return INPUT_ERROR;
    }

    return 0;
}