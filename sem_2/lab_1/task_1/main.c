#include "functions.h"

int main()
{
    User *users = NULL;
    size_t num_registered_users = 0;
    size_t users_capacity = 0;
    User *current_user = NULL; // Указатель на текущего пользователя (NULL, если не залогинен)
    char choice;

    char command[MAX_COMMAND_LENGTH + 1];

    switch (load_users_from_file(&num_registered_users, &users_capacity, &users))
    {
    case MEMORY_ERROR:
        printf("Memory error.\n");
        return MEMORY_ERROR;

    case FILE_RW_ERROR:
        printf("Data file read error.\n");
        return FILE_RW_ERROR;

    case NOT_FOUND:
        printf("Data file not found. (If this is the first run of the program, everything is fine)\n");
        break;

    case SUCCESS:
        printf("Data load success.\n");
        break;
    }

    printf("Welcome to the command line shell!\n");

    while (true)
    {
        if (current_user == NULL)
        {
            printf("\nAuthentication required:\n");
            printf("1. Register\n");
            printf("2. Login\n");
            printf("3. Exit\n");

            printf("Enter your choice: ");
            if (scanf("%c", &choice) != 1)
            {
                printf("Invalid input.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            switch (choice)
            {
            case '1':
                switch (register_user(&num_registered_users, users_capacity, &users))
                {
                case LOGIN_ERROR:
                    printf("Login error!\n");
                    break;
                case PIN_ERROR:
                    printf("Pin error!\n");
                    break;
                case MEMORY_ERROR:
                    printf("Memory error!\n");
                    break;

                case SUCCESS:
                    printf("Registration is successful!\n");
                    break;
                }
                break;
            case '2':
                if (login_user(users, num_registered_users, &current_user))
                {
                    printf("Incorrect username or PIN\n");
                }
                else
                {
                    current_user->commands_executed = 0; // Сброс счетчика при входе
                    printf("Login successful. Welcome, %s!\n", current_user->login);
                    show_commands();
                }
                break;
            case '3':
                printf("Exiting.\n");
                switch (save_users_to_file(num_registered_users, users))
                {
                case FILE_OPEN_ERROR:
                    printf("Data file open error.\n");
                    break;

                case FILE_RW_ERROR:
                    printf("Data file write error.\n");
                    break;

                case MEMORY_ERROR:
                    printf("Memory error.\n");
                    return MEMORY_ERROR;

                case SUCCESS:
                    printf("Data save success.\n");
                    break;
                }
                free(users);
                return 0;
            default:
                printf("Invalid choice.\n");
            }
        }
        else
        {
            printf("%s> ", current_user->login);
            if (read_n_chars(command, MAX_COMMAND_LENGTH))
            {
                while (getchar() != '\n')
                    ;
            }

            if (current_user->sanctions > 0 && current_user->commands_executed >= current_user->sanctions)
            {
                printf("Sanction limit reached. Logging out.\n");
                if (current_user != NULL)
                {
                    printf("Logging out, %s.\n", current_user->login);
                    current_user = NULL;
                }
                continue;
            }

            if (strcmp(command, "Time") == 0)
            {
                time_command();
                current_user->commands_executed++;
            }
            else if (strcmp(command, "Date") == 0)
            {
                date_command();
                current_user->commands_executed++;
            }
            else if (strcmp(command, "Howmuch") == 0)
            {
                char time_str[12], flag[3];
                if (read_n_chars(time_str, 10) || read_n_chars(flag, 2) || flag[0] != '-' ||
                    (flag[1] != 's' && flag[1] != 'm' && flag[1] != 'h' && flag[1] != 'y'))
                {
                    while (getchar() != '\n')
                        ;
                    printf("Input error. Usage: Howmuch <time> flag (-s -m -h -y).\n");
                    continue;
                }
                if (howmuch_command(time_str, flag))
                {
                    printf("Input error. Usage: Howmuch <time> flag (-s -m -h -y).\n");
                    continue;
                }
                current_user->commands_executed++;
            }
            else if (strcmp(command, "Logout") == 0)
            {
                if (current_user != NULL)
                {
                    printf("Logging out, %s.\n", current_user->login);
                    current_user = NULL;
                }
                else
                {
                    printf("Not logged in.\n");
                }
            }
            else if (strcmp(command, "Sanctions") == 0)
            {
                char username[MAX_LOGIN_LENGTH + 1], number_str[64];
                int number;

                if (read_n_chars(username, MAX_LOGIN_LENGTH) || read_n_chars(number_str, 63) || string_to_int(number_str, &number) || number < 0)
                {
                    while (getchar() != '\n')
                        ;
                    printf("Input error. Usage: Sanctions username <number>\n");
                    continue;
                }

                if (current_user == NULL)
                {
                    printf("You must be logged in to use this command.\n");
                }
                else
                {
                    switch (sanctions_command(username, number, users, num_registered_users))
                    {
                    case NOT_FOUND:
                        printf("User not found.\n");
                        break;

                    case INPUT_ERROR:
                        printf("Incorrect code.\n");
                        break;

                    case SUCCESS:
                        printf("Success. The sanction has been established.\n");
                        break;
                    }
                }
            }
            else
            {
                printf("Unknown command. Type 'help' to see available commands.\n");
            }
        }
    }

    return 0;
}