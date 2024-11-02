#include "functions.h"

void print_menu()
{
    puts("____________________________");
    puts("Select an action:");
    puts("1) The largest number of routes;\n2) The smallest number of routes;\n3) The longest way;\n4) The shortest way;\n5) The longest rout;\n6) The shortest rout;\n7) The longest stop;\n8) The shortest stop;\n9) The longest all stops;\n0) End dialog;\n");
    puts("____________________________\n");
    printf(">>> ");
}

void free_buf()
{
    while (getchar() != '\n')
        ;
}

status validate_date(const Time *date)
{
    int month, day, year;
    month = date->month, day = date->day, year = date->year;

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return INPUT_ERROR;

    if (month == 2 && day > 28)
        return INPUT_ERROR;

    if (day > 31)
        return INPUT_ERROR;

    if (month > 12 || day == 0 || year < 1990 || year > 2025)
        return INPUT_ERROR;

    if (date->minute > 60 || date->hour > 23 || date->second > 60)
        return INPUT_ERROR;

    return SUCCESS;
}

int time_cmp(const Time *time_1, const Time *time_2)
{
    if (!time_1 || !time_2)
        return 0;
    if (time_1->year != time_2->year)
        return time_1->year - time_2->year;
    if (time_1->month != time_2->month)
        return time_1->month - time_2->month;
    if (time_1->day != time_2->day)
        return time_1->day - time_2->day;
    if (time_1->hour != time_2->hour)
        return time_1->hour - time_2->hour;
    if (time_1->minute != time_2->minute)
        return time_1->minute - time_2->minute;

    return time_1->second - time_2->second;
}

int date_cmp(const Time *time_1, const Time *time_2)
{
    if (!time_1 || !time_2)
        return 0;
    if (time_1->year != time_2->year)
        return time_1->year - time_2->year;
    if (time_1->month != time_2->month)
        return time_1->month - time_2->month;
    return time_1->day - time_2->day;
}

status init_rout_list(Rout_List *list)
{
    if (!list)
        return MEMORY_ERROR;
    list->head = NULL;
    list->size = 0;
    return SUCCESS;
}

status init_external_list(External_List *list)
{
    if (!list)
        return MEMORY_ERROR;

    list->head = NULL;
    list->size = 0;
    return SUCCESS;
}

status list_rout_insert(Rout_List *list, Rout_Node *new_node)
{
    Rout_Node *temp = NULL;
    size_t i;
    if (!list || !new_node)
        return MEMORY_ERROR;

    if (list->size == 0 || time_cmp(&new_node->data.stop_time, &list->head->data.stop_time) < 0)
    {
        new_node->next = list->head;
        list->head = new_node;
        list->size++;
        return SUCCESS;
    }

    temp = list->head;
    for (i = 0; i < list->size; ++i)
    {
        if (!temp->next || time_cmp(&new_node->data.stop_time, &temp->next->data.stop_time) < 0)
        {
            new_node->next = temp->next;
            temp->next = new_node;
            list->size++;
            break;
        }
        temp = temp->next;
    }
    return SUCCESS;
}

status add_new_stop_point(External_List *ex_list, Rout_Node *node)
{
    size_t i;
    External_Node *temp;
    if (!ex_list || !node)
        return MEMORY_ERROR;

    temp = ex_list->head;
    for (i = 0; i < ex_list->size; ++i)
    {
        if (!strcmp(temp->rout.head->data.vehicle_number, node->data.vehicle_number) &&
            temp->rout.head->data.stop_time.year == node->data.stop_time.year &&
            temp->rout.head->data.stop_time.month == node->data.stop_time.month &&
            temp->rout.head->data.stop_time.day == node->data.stop_time.day)
        {
            list_rout_insert(&temp->rout, node);
            return SUCCESS;
        }
        temp = temp->next;
    }

    temp = ex_list->head;
    ex_list->head = (External_Node *)malloc(sizeof(External_Node));
    if (!ex_list->head)
    {
        ex_list->head = temp->next;
        return MEMORY_ERROR;
    }
    init_rout_list(&ex_list->head->rout);
    list_rout_insert(&ex_list->head->rout, node);
    ex_list->head->next = temp;
    ex_list->size++;
    return SUCCESS;
}

status scan_data_from_files(char **files, int count_files, External_List *ex_list)
{
    int i;
    size_t j;
    Coordinates cur_coords;
    FILE *file = NULL;
    if (!ex_list || !files)
        return MEMORY_ERROR;

    for (i = 0; i < count_files; ++i)
    {
        file = fopen(files[i], "r");
        if (!file)
            continue;

        if (fscanf(file, "%lf %lf\n", &cur_coords.x, &cur_coords.y) != 2)
        {
            fclose(file);
            continue;
        }

        while (!feof(file))
        {
            int count_scan = 0;
            Rout_Node *new_node = (Rout_Node *)malloc(sizeof(Rout_Node));
            if (!new_node)
            {
                fclose(file);
                return MEMORY_ERROR;
            }
            count_scan += fscanf(file, "%255s", new_node->data.vehicle_number);
            count_scan += fscanf(file, "%u.%u.%u %u:%u:%u ",
                                 &new_node->data.stop_time.day, &new_node->data.stop_time.month, &new_node->data.stop_time.year,
                                 &new_node->data.stop_time.hour, &new_node->data.stop_time.minute, &new_node->data.stop_time.second);

            count_scan += fscanf(file, "%u.%u.%u %u:%u:%u %c\n",
                                 &new_node->data.departure_time.day, &new_node->data.departure_time.month, &new_node->data.departure_time.year,
                                 &new_node->data.departure_time.hour, &new_node->data.departure_time.minute, &new_node->data.departure_time.second,
                                 &new_node->data.marker);

            new_node->data.coords = cur_coords;

            if (count_scan != 14 || validate_date(&new_node->data.stop_time) || validate_date(&new_node->data.departure_time) ||
                (new_node->data.marker != 'i' && new_node->data.marker != 's' && new_node->data.marker != 'f') ||
                date_cmp(&new_node->data.stop_time, &new_node->data.departure_time) > 0)
            {
                free(new_node);
                continue;
            }
            if (add_new_stop_point(ex_list, new_node) == MEMORY_ERROR)
            {
                fclose(file);
                free(new_node);
                return MEMORY_ERROR;
            }
        }
        fclose(file);
    }
    return SUCCESS;
}

status destroy_rout_list(Rout_List *list)
{
    Rout_Node *cur, *next;
    if (!list)
        return MEMORY_ERROR;

    cur = list->head;
    while (cur)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
    return SUCCESS;
}

status destroy_external_list(External_List *list)
{
    External_Node *cur, *next;
    if (!list)
        return MEMORY_ERROR;

    cur = list->head;
    while (cur)
    {
        destroy_rout_list(&cur->rout);
        next = cur->next;
        free(cur);
        cur = next;
    }
    return SUCCESS;
}

double length_vector(Coordinates point_1, Coordinates point_2)
{
    return 6371 * acos(sin(point_1.x / 57.3) * sin(point_2.x / 57.3) + cos(point_1.x / 57.3) * cos(point_2.x / 57.3) * cos(abs(point_1.y - point_2.y) / 57.35));
}

status length_rout(Rout_List *list, double *length)
{
    Rout_Node *temp = NULL;
    if (!list || !length)
        return MEMORY_ERROR;

    *length = 0;
    temp = list->head;
    while (temp->next)
    {
        *length += length_vector(temp->data.coords, temp->next->data.coords);
        temp = temp->next;
    }
    return SUCCESS;
}

// самый длинный маршрут
status max_length_rout(External_List *ex_list, double *max_len, char *v_number)
{
    External_Node *temp;
    if (!ex_list || !max_len || !v_number)
        return MEMORY_ERROR;

    *max_len = -1;
    temp = ex_list->head;
    while (temp)
    {
        double len;
        length_rout(&temp->rout, &len);
        if (len - *max_len > EPS)
        {
            *max_len = len;
            strcpy(v_number, temp->rout.head->data.vehicle_number);
        }
        temp = temp->next;
    }
    return SUCCESS;
}

// самый короткий маршрут
status min_length_rout(External_List *ex_list, double *min_len, char *v_number)
{
    External_Node *temp;
    if (!ex_list || !min_len || !v_number)
        return MEMORY_ERROR;

    *min_len = INFINITY;
    temp = ex_list->head;
    while (temp)
    {
        double len;
        length_rout(&temp->rout, &len);
        if (*min_len - len > EPS)
        {
            *min_len = len;
            strcpy(v_number, temp->rout.head->data.vehicle_number);
        }
        temp = temp->next;
    }
    return SUCCESS;
}

status time_substract(const Time *time_1, const Time *time_2, unsigned int *seconds_res)
{
    unsigned int seconds_1 = 0, seconds_2 = 0;
    if (!time_1 || !time_2 || !seconds_res)
        return MEMORY_ERROR;

    seconds_1 += time_1->hour * 3600 + time_1->minute * 60 + time_1->second;
    seconds_2 += time_2->hour * 3600 + time_2->minute * 60 + time_2->second;
    *seconds_res = seconds_1 - seconds_2;
    return SUCCESS;
}

// самая долгая остановка
status max_time_stop(External_List *ex_list, unsigned int *stop_time, char *vehicle_number)
{
    External_Node *ex_temp;
    Rout_Node *rout_temp;
    unsigned int temp_time;
    if (!ex_list || !stop_time || !vehicle_number)
        return MEMORY_ERROR;

    *stop_time = 0;

    ex_temp = ex_list->head;
    while (ex_temp)
    {
        rout_temp = ex_temp->rout.head;
        while (rout_temp)
        {
            time_substract(&rout_temp->data.departure_time, &rout_temp->data.stop_time, &temp_time);
            if (temp_time >= *stop_time)
            {
                *stop_time = temp_time;
                strcpy(vehicle_number, rout_temp->data.vehicle_number);
            }
            rout_temp = rout_temp->next;
        }
        ex_temp = ex_temp->next;
    }
    return SUCCESS;
}

// самая короткая остановка
status min_time_stop(External_List *ex_list, unsigned int *stop_time, char *vehicle_number)
{
    External_Node *ex_temp;
    Rout_Node *rout_temp;
    unsigned int temp_time;
    if (!ex_list || !stop_time || !vehicle_number)
        return MEMORY_ERROR;

    *stop_time = UINT_MAX;

    ex_temp = ex_list->head;
    while (ex_temp)
    {
        rout_temp = ex_temp->rout.head;
        while (rout_temp)
        {
            time_substract(&rout_temp->data.departure_time, &rout_temp->data.stop_time, &temp_time);
            if (temp_time < *stop_time)
            {
                *stop_time = temp_time;
                strcpy(vehicle_number, rout_temp->data.vehicle_number);
            }
            rout_temp = rout_temp->next;
        }
        ex_temp = ex_temp->next;
    }
    return SUCCESS;
}

// Самое большое время простоя
status max_time_stop_all(External_List *ex_list, unsigned int *stop_time, char *vehicle_number)
{
    External_Node *ex_temp;
    Rout_Node *rout_temp;
    Pair *pair_arr;
    char flag = 0;
    unsigned int temp_time;
    size_t capacity = 2, size_arr = 0, i;
    if (!ex_list || !stop_time || !vehicle_number)
        return MEMORY_ERROR;

    *stop_time = 0;
    pair_arr = (Pair *)malloc(capacity * sizeof(Pair));
    if (!pair_arr)
        return MEMORY_ERROR;

    ex_temp = ex_list->head;
    while (ex_temp)
    {
        temp_time = 0;
        rout_temp = ex_temp->rout.head;
        while (rout_temp)
        {
            unsigned int scan_time;
            time_substract(&rout_temp->data.departure_time, &rout_temp->data.stop_time, &scan_time);
            temp_time += scan_time;
            rout_temp = rout_temp->next;
        }
        for (i = 0; i < size_arr; ++i)
        {
            if (!strcmp(ex_temp->rout.head->data.vehicle_number, pair_arr[i].key))
            {
                pair_arr[i].value.svalue += temp_time;
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            if (size_arr == capacity)
            {
                capacity *= 2;
                Pair *for_realloc = (Pair *)realloc(pair_arr, capacity * sizeof(Pair));
                if (!for_realloc)
                {
                    free(pair_arr);
                    return MEMORY_ERROR;
                }
                pair_arr = for_realloc;
            }
            pair_arr[size_arr].key = ex_temp->rout.head->data.vehicle_number;
            pair_arr[size_arr++].value.svalue = (size_t)temp_time;
        }
        ex_temp = ex_temp->next;
    }

    for (i = 0; i < size_arr; ++i)
    {
        if (pair_arr[i].value.svalue >= *stop_time)
        {
            capacity = i;
            *stop_time = pair_arr[i].value.svalue;
        }
    }
    *stop_time = pair_arr[capacity].value.svalue;
    strcpy(vehicle_number, pair_arr[capacity].key);
    free(pair_arr);
    return SUCCESS;
}

// Самый длинный путь
status max_len_way(External_List *ex_list, double *len_way, char *vehicle_number)
{
    External_Node *ex_temp;
    Rout_Node *rout_temp;
    Pair *pair_arr;
    char flag = 0;
    double temp_len;
    size_t capacity = 2, size_arr = 0, i;
    if (!ex_list || !len_way || !vehicle_number)
        return MEMORY_ERROR;

    *len_way = 0;
    pair_arr = (Pair *)malloc(capacity * sizeof(Pair));
    if (!pair_arr)
        return MEMORY_ERROR;

    ex_temp = ex_list->head;
    while (ex_temp)
    {
        length_rout(&ex_temp->rout, &temp_len);

        for (i = 0; i < size_arr; ++i)
        {
            if (!strcmp(ex_temp->rout.head->data.vehicle_number, pair_arr[i].key))
            {
                pair_arr[i].value.dvalue += temp_len;
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            if (size_arr == capacity)
            {
                capacity *= 2;
                Pair *for_realloc = (Pair *)realloc(pair_arr, capacity * sizeof(Pair));
                if (!for_realloc)
                {
                    free(pair_arr);
                    return MEMORY_ERROR;
                }
                pair_arr = for_realloc;
            }
            pair_arr[size_arr].key = ex_temp->rout.head->data.vehicle_number;
            pair_arr[size_arr++].value.dvalue = temp_len;
        }
        ex_temp = ex_temp->next;
    }

    for (i = 0; i < size_arr; ++i)
    {
        if (pair_arr[i].value.dvalue - *len_way >= EPS)
        {
            capacity = i;
            *len_way = pair_arr[i].value.dvalue;
        }
    }
    strcpy(vehicle_number, pair_arr[capacity].key);
    free(pair_arr);
    return SUCCESS;
}

// Самый короткий путь
status min_len_way(External_List *ex_list, double *len_way, char *vehicle_number)
{
    External_Node *ex_temp;
    Rout_Node *rout_temp;
    Pair *pair_arr;
    char flag = 0;
    double temp_len;
    size_t capacity = 2, size_arr = 0, i;
    if (!ex_list || !len_way || !vehicle_number)
        return MEMORY_ERROR;

    *len_way = INFINITY;
    pair_arr = (Pair *)malloc(capacity * sizeof(Pair));
    if (!pair_arr)
        return MEMORY_ERROR;

    ex_temp = ex_list->head;
    while (ex_temp)
    {
        length_rout(&ex_temp->rout, &temp_len);

        for (i = 0; i < size_arr; ++i)
        {
            if (!strcmp(ex_temp->rout.head->data.vehicle_number, pair_arr[i].key))
            {
                pair_arr[i].value.dvalue += temp_len;
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            if (size_arr == capacity)
            {
                capacity *= 2;
                Pair *for_realloc = (Pair *)realloc(pair_arr, capacity * sizeof(Pair));
                if (!for_realloc)
                {
                    free(pair_arr);
                    return MEMORY_ERROR;
                }
                pair_arr = for_realloc;
            }
            pair_arr[size_arr].key = ex_temp->rout.head->data.vehicle_number;
            pair_arr[size_arr++].value.dvalue = temp_len;
        }
        ex_temp = ex_temp->next;
    }

    for (i = 0; i < size_arr; ++i)
    {
        if (*len_way - pair_arr[i].value.dvalue >= EPS)
        {
            capacity = i;
            *len_way = pair_arr[i].value.dvalue;
        }
    }
    strcpy(vehicle_number, pair_arr[capacity].key);
    free(pair_arr);
    return SUCCESS;
}

// Больше всего путей
status max_count_routs(External_List *ex_list, size_t *count_routs, char *vehicle_number)
{
    External_Node *ex_temp;
    Rout_Node *rout_temp;
    Pair *pair_arr;
    char flag = 0;
    size_t capacity = 2, size_arr = 0, i;
    if (!ex_list || !count_routs || !vehicle_number)
        return MEMORY_ERROR;

    *count_routs = 0;
    pair_arr = (Pair *)malloc(capacity * sizeof(Pair));
    if (!pair_arr)
        return MEMORY_ERROR;

    ex_temp = ex_list->head;
    while (ex_temp)
    {

        for (i = 0; i < size_arr; ++i)
        {
            if (!strcmp(ex_temp->rout.head->data.vehicle_number, pair_arr[i].key))
            {
                pair_arr[i].value.svalue += 1;
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            if (size_arr == capacity)
            {
                capacity *= 2;
                Pair *for_realloc = (Pair *)realloc(pair_arr, capacity * sizeof(Pair));
                if (!for_realloc)
                {
                    free(pair_arr);
                    return MEMORY_ERROR;
                }
                pair_arr = for_realloc;
            }
            pair_arr[size_arr].key = ex_temp->rout.head->data.vehicle_number;
            pair_arr[size_arr++].value.svalue = 1;
        }
        ex_temp = ex_temp->next;
    }

    for (i = 0; i < size_arr; ++i)
    {
        if (pair_arr[i].value.svalue > *count_routs)
        {
            capacity = i;
            *count_routs = pair_arr[i].value.svalue;
        }
    }
    strcpy(vehicle_number, pair_arr[capacity].key);
    free(pair_arr);
    return SUCCESS;
}

// Меньше всего путей
status min_count_routs(External_List *ex_list, size_t *count_routs, char *vehicle_number)
{
    External_Node *ex_temp;
    Rout_Node *rout_temp;
    Pair *pair_arr;
    char flag = 0;
    size_t capacity = 2, size_arr = 0, i;
    if (!ex_list || !count_routs || !vehicle_number)
        return MEMORY_ERROR;

    *count_routs = __SIZE_MAX__;
    pair_arr = (Pair *)malloc(capacity * sizeof(Pair));
    if (!pair_arr)
        return MEMORY_ERROR;

    ex_temp = ex_list->head;
    while (ex_temp)
    {

        for (i = 0; i < size_arr; ++i)
        {
            if (!strcmp(ex_temp->rout.head->data.vehicle_number, pair_arr[i].key))
            {
                pair_arr[i].value.svalue += 1;
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            if (size_arr == capacity)
            {
                capacity *= 2;
                Pair *for_realloc = (Pair *)realloc(pair_arr, capacity * sizeof(Pair));
                if (!for_realloc)
                {
                    free(pair_arr);
                    return MEMORY_ERROR;
                }
                pair_arr = for_realloc;
            }
            pair_arr[size_arr].key = ex_temp->rout.head->data.vehicle_number;
            pair_arr[size_arr++].value.svalue = 1;
        }
        ex_temp = ex_temp->next;
    }

    for (i = 0; i < size_arr; ++i)
    {
        if (pair_arr[i].value.svalue < *count_routs)
        {
            capacity = i;
            *count_routs = pair_arr[i].value.svalue;
        }
    }
    strcpy(vehicle_number, pair_arr[capacity].key);
    free(pair_arr);
    return SUCCESS;
}