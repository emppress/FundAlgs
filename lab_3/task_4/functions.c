#include "functions.h"

void print_menu()
{
    puts("____________________________");
    puts("Select an action:");
    puts("1) Add mail;\n2) Delete mail;\n3) Search mail;\n4) Search for delivered mail;\n5) Search for overdue mail;\n6) Print all mails;\n0) End dialog;\nd)(admins only)set delivered;\n");
    puts("____________________________\n");
    printf(">>> ");
}

status string_to_uint(const char *str, unsigned long *result)
{
    char *endinp;
    *result = strtoul(str, &endinp, 10);
    if (*result == ULONG_MAX)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

status string_to_double(const char *str, double *result)
{
    char *endptr;
    *result = strtod(str, &endptr);
    if (*result == HUGE_VAL || *result == -HUGE_VAL || *endptr != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

status post_init(Post *post)
{
    post->address_ptr = (Address *)malloc(sizeof(Address));
    if (!post->address_ptr)
        return MEMORY_ERROR;
    post->mail_capacity = 2;
    post->mail_arr = (Mail *)malloc(sizeof(Mail) * post->mail_capacity);
    if (!post->mail_arr)
    {
        free(post->address_ptr);
        return MEMORY_ERROR;
    }

    return SUCCESS;
}

void clear_mail(Mail *mail)
{
    clear_address(&mail->address);
    delete_string_content(&mail->create_time);
    delete_string_content(&mail->postal_index);
    delete_string_content(&mail->getting_time);
}

void get_string_with_space(char *str, int max_size)
{
    char c;
    int size = 0;
    while (isspace(c = getchar()))
        ;
    str[size++] = c;
    while ((c = getchar()) != '\n')
    {
        str[size++] = c;
        if (size == max_size - 1)
            break;
    }
    str[size] = '\0';
}

status get_address(Address *address)
{
    if (!address)
        MEMORY_ERROR;

    int i;
    unsigned long _apartment_number, _house_number;
    char house_number[12], city[128], street[128], building[128], apartment_number[128], index[8];
    printf("Enter city: ");
    get_string_with_space(city, 128);
    printf("Enter street: ");
    get_string_with_space(street, 128);
    printf("Enter house number: ");
    scanf("%11s", house_number);
    printf("Enter building: ");
    scanf("%127s", building);
    printf("Enter apartment_number: ");
    scanf("%127s", apartment_number);
    printf("Enter index(digit format: xxxxxx): ");
    scanf("%7s", index);

    for (i = 0; i < 6; ++i)
    {
        if (!isdigit(index[i]))
            return INPUT_ERROR;
    }
    if (index[6] != '\0')
    {
        while (getchar() != '\n')
            ;
        return INPUT_ERROR;
    }

    if (string_to_uint(house_number, &_house_number))
        return INPUT_ERROR;
    if (string_to_uint(apartment_number, &_apartment_number))
        return INPUT_ERROR;
    address->apartment_number = _apartment_number;
    address->house_number = (unsigned int)_house_number;
    address->street = string_create(street);
    address->building = string_create(building);
    address->city = string_create(city);
    address->index = string_create(index);
    if (!address->street.arr ||
        !address->building.arr ||
        !address->city.arr ||
        !address->index.arr)
        return MEMORY_ERROR;

    return SUCCESS;
}

status get_weight(double *weight)
{
    char _weight[128];
    if (!weight)
        MEMORY_ERROR;

    printf("Enter mail weight: ");
    scanf("%127s", _weight);
    return string_to_double(_weight, weight);
}

status get_postal_index(String *str)
{
    char _postal_index[16];
    int i;
    if (!str)
        MEMORY_ERROR;
    str->arr = NULL;
    str->size = 0;
    printf("Enter postal index(14 digits): ");
    scanf("%15s", _postal_index);
    if (_postal_index[14] != '\0')
        return INPUT_ERROR;
    for (i = 0; i < 14; ++i)
    {
        if (!isdigit(_postal_index[i]))
            return INPUT_ERROR;
    }
    *str = string_create(_postal_index);
    if (str->size == -1)
        return MEMORY_ERROR;
    return SUCCESS;
}

status validate_date(const char *date)
{
    if (date[10] != '\0' || date[2] != ':' || date[5] != ':' || !isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[3]) || !isdigit(date[4]) || !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8]) || !isdigit(date[9]))
        return INPUT_ERROR;

    int day, month, year;
    day = atoi(date);
    month = atoi(date + 3);
    year = atoi(date + 6);
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return INPUT_ERROR;

    if (month == 2 && day > 28)
        return INPUT_ERROR;

    if (day > 31)
        return INPUT_ERROR;

    if (month > 12 || day == 0 || year < 1990 || year > 2025)
        return INPUT_ERROR;

    return SUCCESS;
}

status validate_time(const char *time)
{
    if (time[8] != '\0' || time[2] != ':' || time[5] != ':' || !isdigit(time[0]) || !isdigit(time[1]) || !isdigit(time[3]) || !isdigit(time[4]) || !isdigit(time[6]) || !isdigit(time[7]))
        return INPUT_ERROR;

    int h, m, s;
    h = atoi(time);
    m = atoi(time + 3);
    s = atoi(time + 6);
    if (h > 23 || m > 59 || s > 59)
    {
        return INPUT_ERROR;
    }
    return SUCCESS;
}

status get_time(String *str)
{
    char date[24], time[10];
    status state;
    if (!str)
        return MEMORY_ERROR;

    scanf("%s", date);
    scanf("%s", time);
    if (validate_time(time) || validate_date(date))
        return INPUT_ERROR;
    strcat(date, " ");
    strcat(date, time);
    *str = string_create(date);
    if (str->size == -1)
        return MEMORY_ERROR;
    return SUCCESS;
}

void clear_address(Address *address)
{
    delete_string_content(&address->building);
    delete_string_content(&address->city);
    delete_string_content(&address->index);
    delete_string_content(&address->street);
}

status add_mail(Post *post, int *count_mails)
{
    Mail *mail_realloc;
    status state;
    if (*count_mails == post->mail_capacity)
    {
        post->mail_capacity *= 2;
        mail_realloc = (Mail *)realloc(post->mail_arr, post->mail_capacity * sizeof(Mail));
        if (!mail_realloc)
            return MEMORY_ERROR;
        post->mail_arr = mail_realloc;
    }
    if ((state = get_address(&post->mail_arr[*count_mails].address)) != SUCCESS)
        return state;
    if ((state = get_weight(&post->mail_arr[*count_mails].weight)) != SUCCESS)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        return state;
    }
    if ((state = get_postal_index(&post->mail_arr[*count_mails].postal_index)) != SUCCESS)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        return state;
    }
    printf("Enter create time(format: dd:MM:yyyy hh:mm:ss): ");
    if ((state = get_time(&post->mail_arr[*count_mails].create_time)) != SUCCESS)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        delete_string_content(&post->mail_arr[*count_mails].postal_index);
        return state;
    }
    printf("Enter getting time(format: dd:MM:yyyy hh:mm:ss): ");
    if ((state = get_time(&post->mail_arr[*count_mails].getting_time)) != SUCCESS)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        delete_string_content(&post->mail_arr[*count_mails].postal_index);
        delete_string_content(&post->mail_arr[*count_mails].create_time);
        return state;
    }
    if (compare_dates(post->mail_arr[*count_mails].getting_time.arr, post->mail_arr[*count_mails].create_time.arr) <= 0)
    {
        clear_address(&post->mail_arr[*count_mails].address);
        delete_string_content(&post->mail_arr[*count_mails].postal_index);
        delete_string_content(&post->mail_arr[*count_mails].create_time);
        delete_string_content(&post->mail_arr[*count_mails].getting_time);
        return INPUT_ERROR;
    }
    post->mail_arr[*count_mails].delivered = 'n';

    (*count_mails)++;
    return SUCCESS;
}

status delete_mail(Post *post, int *count_mails)
{
    if (!post || !post->mail_arr)
        return MEMORY_ERROR;

    status state;

    String _postal_index;
    if ((state = get_postal_index(&_postal_index)) != SUCCESS)
        return state;

    for (int i = 0; i < *count_mails; ++i)
    {
        if (string_equivalence_relation(&post->mail_arr[i].postal_index, &_postal_index))
        {
            clear_mail(&post->mail_arr[i]);
            for (int j = i; j < (*count_mails - 1); ++j)
            {
                post->mail_arr[i] = post->mail_arr[i + 1];
            }
            (*count_mails)--;
            delete_string_content(&_postal_index);
            return SUCCESS;
        }
    }
    delete_string_content(&_postal_index);
    return INPUT_ERROR;
}

void print_address(Address *address)
{
    printf("City: %s\n", address->city.arr);
    printf("Street: %s\n", address->street.arr);
    printf("House number: %u\n", address->house_number);
    printf("Building: %s\n", address->building.arr);
    printf("Apartment number: %u\n", address->apartment_number);
    printf("Index: %s\n", address->index.arr);
}

void print_mail_info(Mail *mail)
{
    print_address(&mail->address);
    printf("Create time: %s; getting_time: %s\n", mail->create_time.arr, mail->getting_time.arr);
    printf("Postal index: %s\n", mail->postal_index.arr);
    printf("Weight: %lf\n", mail->weight);
}

status search_mail(Post *post, int *count_mails)
{
    if (!post || !post->mail_arr)
        return MEMORY_ERROR;

    status state;

    String _postal_index;
    if ((state = get_postal_index(&_postal_index)) != SUCCESS)
        return state;

    for (int i = 0; i < *count_mails; ++i)
    {
        if (string_equivalence_relation(&post->mail_arr[i].postal_index, &_postal_index))
        {
            print_mail_info(&post->mail_arr[i]);
            delete_string_content(&_postal_index);
            return SUCCESS;
        }
    }
    delete_string_content(&_postal_index);
    return INPUT_ERROR;
}

int compare_mails(const void *a, const void *b)
{
    const Mail *mail_1 = (const Mail *)a;
    const Mail *mail_2 = (const Mail *)b;
    int state;
    if ((state = string_compare(&mail_1->address.index, &mail_2->address.index)) != 0)
    {
        return state;
    }

    return string_compare(&mail_1->postal_index, &mail_2->postal_index);
}

status set_delivered(Post *post, int *count_mails)
{
    if (!post || !post->mail_arr)
        return MEMORY_ERROR;

    status state;

    String _postal_index;
    if ((state = get_postal_index(&_postal_index)) != SUCCESS)
    {
        delete_string_content(&_postal_index);
        return state;
    }

    for (int i = 0; i < *count_mails; ++i)
    {
        if (string_equivalence_relation(&post->mail_arr[i].postal_index, &_postal_index))
        {
            post->mail_arr[i].delivered = 'y';
            delete_string_content(&_postal_index);
            return SUCCESS;
        }
    }
    delete_string_content(&_postal_index);
    return INPUT_ERROR;
}

int time_compare_mails(const void *a, const void *b)
{
    const Mail *mail_1 = (const Mail *)a;
    const Mail *mail_2 = (const Mail *)b;

    String time_1, time_2;
    int param_1, param_2;
    time_1 = mail_1->create_time;
    time_2 = mail_2->create_time;

    param_1 = atoi(time_1.arr + 6);
    param_2 = atoi(time_2.arr + 6);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1.arr + 3);
    param_2 = atoi(time_2.arr + 3);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1.arr);
    param_2 = atoi(time_2.arr);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }
    return strcmp(time_1.arr + 11, time_2.arr + 11);
}

int compare_dates(const char *time_1, const char *time_2)
{
    int param_1, param_2;
    param_1 = atoi(time_1 + 6);
    param_2 = atoi(time_2 + 6);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1 + 3);
    param_2 = atoi(time_2 + 3);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }

    param_1 = atoi(time_1);
    param_2 = atoi(time_2);
    if (param_1 != param_2)
    {
        return param_1 - param_2;
    }
    return strcmp(time_1 + 11, time_2 + 11);
}

status search_delivered(Post *post, int count_mails, int *count_delivered, Mail **arr_delivered)
{
    if (!post || !post->mail_arr || !arr_delivered)
        return MEMORY_ERROR;

    int capacity = 2;
    *count_delivered = 0;
    *arr_delivered = NULL;
    Mail *delivered = (Mail *)malloc(sizeof(Mail) * capacity), *temp_realloc;
    if (!delivered)
        return MEMORY_ERROR;

    for (int i = 0; i < count_mails; ++i)
    {
        if (post->mail_arr[i].delivered == 'y')
        {
            if (*count_delivered == capacity)
            {
                capacity *= 2;
                temp_realloc = (Mail *)realloc(delivered, sizeof(Mail) * capacity);
                if (!temp_realloc)
                {
                    free(delivered);
                    return MEMORY_ERROR;
                }
                delivered = temp_realloc;
            }

            delivered[(*count_delivered)++] = post->mail_arr[i];
        }
    }
    qsort(delivered, *count_delivered, sizeof(Mail), time_compare_mails);
    *arr_delivered = delivered;
    return SUCCESS;
}

status search_overdue(Post *post, int count_mails, int *count_overdue, Mail **arr_overdue)
{
    if (!post || !post->mail_arr || !arr_overdue)
        return MEMORY_ERROR;

    int capacity = 2;
    *count_overdue = 0;
    *arr_overdue = NULL;
    Mail *overdue = (Mail *)malloc(sizeof(Mail) * capacity), *temp_realloc;
    if (!overdue)
        return MEMORY_ERROR;

    time_t _cur_time = time(NULL);
    struct tm *now = localtime(&_cur_time);
    char cur_time[24];
    sprintf(cur_time, "%02d:%02d:%d %02d:%02d:%02d", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec);
    for (int i = 0; i < count_mails; ++i)
    {
        if (post->mail_arr[i].delivered == 'n' && compare_dates(post->mail_arr[i].getting_time.arr, cur_time) < 0)
        {
            if (*count_overdue == capacity)
            {
                capacity *= 2;
                temp_realloc = (Mail *)realloc(overdue, sizeof(Mail) * capacity);
                if (!temp_realloc)
                {
                    free(overdue);
                    return MEMORY_ERROR;
                }
                overdue = temp_realloc;
            }

            overdue[(*count_overdue)++] = post->mail_arr[i];
        }
    }
    qsort(overdue, *count_overdue, sizeof(Mail), time_compare_mails);
    *arr_overdue = overdue;
    return SUCCESS;
}

void clear_post(Post *post, int count_mails)
{
    clear_address(post->address_ptr);
    free(post->address_ptr);
    for (int i = 0; i < count_mails; ++i)
    {
        clear_mail(&post->mail_arr[i]);
    }
    free(post->mail_arr);
}