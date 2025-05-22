#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

#define MAX_LOGIN_LENGTH 6
#define MAX_COMMAND_LENGTH 50
#define INITIAL_CAPACITY 2 // Начальный размер массива пользователей
#define USERS_FILE "users.dat"

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR,
    LOGIN_ERROR,
    PIN_ERROR,
    NOT_FOUND,
    FILE_RW_ERROR
} status;

typedef struct
{
    char login[MAX_LOGIN_LENGTH + 1]; // +1 для завершающего нуля
    int pin;
    int sanctions;         // Лимит команд (0 означает отсутствие ограничений)
    int commands_executed; // Счетчик выполненных команд в текущем сеансе
} User;

status register_user(size_t *num_registered_users, size_t users_capacity, User **users);
status login_user(User *users, size_t num_registered_users, User **found);
void show_commands();
void time_command();
void date_command();
bool is_valid_login(const char *login);
status parse_date(const char *date_str, struct tm *tm);
bool user_exists(const char *login, size_t num_registered_users, User const *users);
User *find_user(const char *login, User *users, size_t num_registered_users);
status sanctions_command(char *username, int number, User *users, size_t num_registered_users);
status read_n_chars(char *buf, size_t max_size);
status string_to_int(const char *str, int *result);
status load_users_from_file(size_t *num_registered_users, size_t *users_capacity, User **users);
status save_users_to_file(size_t num_registered_users, User *users);
status howmuch_command(char *time_str, char *flag);

#endif