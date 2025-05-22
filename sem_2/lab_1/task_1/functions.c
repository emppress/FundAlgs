#include "functions.h"

status string_to_int(const char *str, int *result)
{
    char *endinp;
    long res;
    if (!str || !result)
        return MEMORY_ERROR;

    res = strtol(str, &endinp, 10);
    if (res >= INT_MAX || res <= INT_MIN)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    *result = (int)res;
    return SUCCESS;
}

status read_n_chars(char *buf, size_t max_size)
{
    size_t i;
    char ch;
    if (!buf)
        return MEMORY_ERROR;

    while (isspace(ch = getchar()))
        ;

    for (i = 0; i < max_size; ++i)
    {
        if (isspace(ch))
        {
            buf[i] = '\0';
            return SUCCESS;
        }
        buf[i] = ch;
        ch = getchar();
    }
    if (!isspace(ch))
        return INPUT_ERROR;
    return SUCCESS;
}

status register_user(size_t *num_registered_users, size_t users_capacity, User **users)
{
    char login[MAX_LOGIN_LENGTH + 1], _pin[7];
    int pin;
    User *tmp_arr = NULL;

    if (!users || !num_registered_users)
        return MEMORY_ERROR;

    printf("Enter new login (max %d characters, alphanumeric): ", MAX_LOGIN_LENGTH);
    if (read_n_chars(login, MAX_LOGIN_LENGTH))
    {
        while (getchar() != '\n')
            ;
        return LOGIN_ERROR;
    }

    if (!is_valid_login(login))
        return LOGIN_ERROR;

    if (user_exists(login, *num_registered_users, *users))
        return LOGIN_ERROR;

    printf("Enter PIN (0-100000): ");
    if (scanf("%6s", _pin) != 1 || string_to_int(_pin, &pin) || pin < 0 || pin > 100000)
    {
        while (getchar() != '\n')
            ;
        return PIN_ERROR;
    }

    if (*num_registered_users == users_capacity)
    {
        tmp_arr = (User *)realloc(*users, ((users_capacity == 0) ? 2 : users_capacity * 2) * sizeof(User));
        if (!tmp_arr)
            return MEMORY_ERROR;

        *users = tmp_arr;
    }

    strcpy((*users)[*num_registered_users].login, login);
    (*users)[*num_registered_users].pin = pin;
    (*users)[*num_registered_users].sanctions = 0;
    (*users)[(*num_registered_users)++].commands_executed = 0;

    return SUCCESS;
}

status login_user(User *users, size_t num_registered_users, User **found)
{
    char login[MAX_LOGIN_LENGTH + 1], _pin[7];
    int pin;
    User *user = NULL;

    if (!found)
        return MEMORY_ERROR;

    printf("Enter login: ");
    if (read_n_chars(login, MAX_LOGIN_LENGTH))
    {
        while (getchar() != '\n')
            ;
        return INPUT_ERROR;
    }

    printf("Enter PIN: ");
    if (scanf("%6s", _pin) != 1 || string_to_int(_pin, &pin) || pin < 0 || pin > 100000)
    {
        while (getchar() != '\n')
            ;
        return INPUT_ERROR;
    }

    user = find_user(login, users, num_registered_users);
    if (user != NULL && user->pin == pin)
    {
        *found = user;
        return SUCCESS;
    }

    return INPUT_ERROR;
}

void show_commands()
{
    printf("\nAvailable commands:\n");
    printf("  Time\n");
    printf("  Date\n");
    printf("  Howmuch <time> flag\n");
    printf("  Logout\n");
    printf("  Sanctions username <number>\n");
    printf("\n");
}

void time_command()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void date_command()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d:%02d:%04d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int days_in_month(int month, int year)
{
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year))
    {
        return 29;
    }
    return days[month];
}

status howmuch_command(char *time_str, char *flag)
{
    struct tm start_tm;
    time_t start_time, current_time;
    double diff;

    if (!time_str || !flag)
        return MEMORY_ERROR;

    if (parse_date(time_str, &start_tm))
        return INPUT_ERROR;

    int year = start_tm.tm_year + 1900;
    int month = start_tm.tm_mon + 1;
    int day = start_tm.tm_mday;

    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > days_in_month(month, year))
        return INPUT_ERROR;

    start_time = mktime(&start_tm);
    if (start_time == -1)
        return INPUT_ERROR;

    current_time = time(NULL);

    if (current_time == -1)
        return INPUT_ERROR;

    diff = difftime(current_time, start_time);

    if (strcmp(flag, "-s") == 0)
    {
        printf("%.0lf seconds\n", floor(diff));
    }
    else if (strcmp(flag, "-m") == 0)
    {
        printf("%.0lf minutes\n", floor(diff / 60.0));
    }
    else if (strcmp(flag, "-h") == 0)
    {
        printf("%.0lf hours\n", floor(diff / 3600.0));
    }
    else if (strcmp(flag, "-y") == 0)
    {
        printf("%.0lf years\n", floor(diff / (365.25 * 24 * 3600)));
    }
    else
    {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

status sanctions_command(char *username, int number, User *users, size_t num_registered_users)
{
    User *target_user;
    char confirmation[7];

    if (!username || !users)
        return NOT_FOUND;

    target_user = find_user(username, users, num_registered_users);
    if (target_user == NULL)
        return NOT_FOUND;

    printf("Enter confirmation code (12345): ");
    if (scanf("%6s", confirmation) != 1 || confirmation[5] != '\0' || strcmp(confirmation, "12345"))
    {
        while (getchar() != '\n')
            ;
        return INPUT_ERROR;
    }

    target_user->sanctions = number;
    return SUCCESS;
}

bool is_valid_login(const char *login)
{
    if (!login)
        return false;

    for (int i = 0; login[i] != '\0'; i++)
    {
        if (!isalnum(login[i]))
        {
            return false;
        }
    }
    return true;
}

bool user_exists(const char *login, size_t num_registered_users, User const *users)
{
    for (int i = 0; i < num_registered_users; i++)
    {
        if (strcmp(users[i].login, login) == 0)
        {
            return true;
        }
    }
    return false;
}

User *find_user(const char *login, User *users, size_t num_registered_users)
{
    if (!users || !login)
        return NULL;

    for (int i = 0; i < num_registered_users; i++)
    {
        if (strcmp(users[i].login, login) == 0)
        {
            return users + i;
        }
    }
    return NULL;
}

status parse_date(const char *date_str, struct tm *tm)
{
    int day, month, year;
    if (!date_str || !tm)
        return MEMORY_ERROR;

    if (sscanf(date_str, "%d:%d:%d", &day, &month, &year) != 3)
        return INPUT_ERROR;

    tm->tm_mday = day;
    tm->tm_mon = month - 1;
    tm->tm_year = year - 1900;
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1;

    return SUCCESS;
}

status load_users_from_file(size_t *num_registered_users, size_t *users_capacity, User **users)
{
    FILE *fp;
    if (!num_registered_users || !users_capacity || !users)
        return MEMORY_ERROR;

    fp = fopen(USERS_FILE, "rb");
    if (fp == NULL)
    {
        // Файл не существует, это нормально при первом запуске.
        return NOT_FOUND;
    }

    if (fread(num_registered_users, sizeof(size_t), 1, fp) != 1)
    {
        fclose(fp);
        return FILE_RW_ERROR;
    }

    *users_capacity = *num_registered_users;

    *users = (User *)malloc(*users_capacity * sizeof(User));
    if (*users == NULL)
    {
        fclose(fp);
        return MEMORY_ERROR;
    }

    if (fread(*users, sizeof(User), *num_registered_users, fp) != *num_registered_users)
    {
        fclose(fp);
        free(*users);
        *users = NULL;
        *num_registered_users = 0;
        *users_capacity = 0;
        return FILE_RW_ERROR;
    }

    fclose(fp);
    return SUCCESS;
}

status save_users_to_file(size_t num_registered_users, User *users)
{
    FILE *fp;
    if (!users)
        return MEMORY_ERROR;

    fp = fopen(USERS_FILE, "wb");
    if (!fp)
        return FILE_OPEN_ERROR;

    if (fwrite(&num_registered_users, sizeof(size_t), 1, fp) != 1)
    {
        fclose(fp);
        return FILE_RW_ERROR;
    }

    if (fwrite(users, sizeof(User), num_registered_users, fp) != num_registered_users)
    {
        fclose(fp);
        return FILE_RW_ERROR;
    }

    fclose(fp);
    return SUCCESS;
}