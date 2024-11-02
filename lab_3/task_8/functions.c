#include "functions.h"

status string_init(String *string)
{
    if (!string)
        return MEMORY_ERROR;
    string->capacity = 1;
    string->len = 0;
    if (!(string->arr = (char *)malloc(sizeof(char))))
        return MEMORY_ERROR;
    string->arr[0] = '\0';
    return SUCCESS;
}

status delete_string_content(String *string)
{
    if (!string)
        return MEMORY_ERROR;
    free(string->arr);
    string->arr = NULL;
    string->len = 0;
    string->capacity = 0;
    return SUCCESS;
}

int string_compare(const String *str_1, const String *str_2)
{
    if (str_1->len != str_2->len)
        return str_1->len - str_2->len;
    return strcmp(str_1->arr, str_2->arr);
}

int string_equivalence_relation(String *str_1, String *str_2)
{
    return strcmp(str_1->arr, str_2->arr) == 0;
}

status string_dup(String *dest, String *sourse)
{
    if (!dest || !sourse)
        return MEMORY_ERROR;

    dest->capacity = sourse->capacity;
    dest->len = sourse->len;
    if (!sourse->arr)
    {
        dest->arr = NULL;
        return SUCCESS;
    }
    dest->arr = (char *)malloc(sourse->capacity * sizeof(char));
    if (!dest->arr)
        return MEMORY_ERROR;
    strcpy(dest->arr, sourse->arr);
    return SUCCESS;
}

String *string_destroy(String *str)
{
    if (!str)
        return NULL;
    free(str->arr);
    free(str);
    return NULL;
}

status add_char_to_string(String *str, char ch)
{
    if (!str)
        return MEMORY_ERROR;
    if (str->capacity == 0)
    {
        if (string_init(str))
            return MEMORY_ERROR;
    }
    if (str->capacity == str->len + 1)
    {
        char *for_realloc;
        str->capacity *= 2;
        for_realloc = (char *)realloc(str->arr, str->capacity * sizeof(char));
        if (!for_realloc)
            return MEMORY_ERROR;
        str->arr = for_realloc;
    }
    str->arr[str->len++] = ch;
    str->arr[str->len] = '\0';
    return SUCCESS;
}

void free_buf()
{
    while (getchar() != '\n')
        ;
}

status init_polynom(Polynom *polynom)
{
    if (!polynom)
        return MEMORY_ERROR;

    polynom->head = NULL;
    polynom->size = 0;
    return SUCCESS;
}

void destroy_polynom(Polynom *polynom)
{
    Term *cur, *next;
    if (!polynom)
        return;

    cur = polynom->head;
    while (cur)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
    polynom->head = NULL;
    polynom->size = 0;
}

status insert_term(Polynom *polynom, int degree, int coef)
{
    if (!coef)
        return SUCCESS;
    Term *temp;
    if (!polynom)
        return MEMORY_ERROR;

    if (polynom->size == 0)
    {
        polynom->head = (Term *)malloc(sizeof(Term));
        if (!polynom->head)
        {
            return MEMORY_ERROR;
        }
        polynom->size++;
        polynom->head->coef = coef;
        polynom->head->degree = degree;
        polynom->head->next = NULL;
        return SUCCESS;
    }
    if (polynom->head->degree < degree)
    {
        temp = (Term *)malloc(sizeof(Term));
        if (!temp)
            return MEMORY_ERROR;

        temp->coef = coef;
        temp->degree = degree;
        temp->next = polynom->head;
        polynom->head = temp;
        polynom->size++;
        return SUCCESS;
    }
    if (polynom->head->degree == degree)
    {
        if (coef > 0 && INT_MAX - coef < polynom->head->coef)
            return MEMORY_ERROR;
        else if (coef < 0 && INT_MIN - coef > polynom->head->coef)
            return MEMORY_ERROR;
        polynom->head->coef += coef;
        if (polynom->head->coef == 0)
        {
            temp = polynom->head;
            polynom->head = temp->next;
            free(temp);
            polynom->size--;
        }
        return SUCCESS;
    }
    temp = polynom->head;
    while (temp != NULL)
    {
        if (temp->next == NULL || temp->next->degree < degree)
        {
            Term *new = (Term *)malloc(sizeof(Term));
            if (!new)
                return MEMORY_ERROR;

            new->coef = coef;
            new->degree = degree;
            new->next = temp->next;
            temp->next = new;
            polynom->size++;
            return SUCCESS;
        }
        if (temp->next->degree == degree)
        {
            if (coef > 0 && INT_MAX - coef < temp->next->coef)
                return MEMORY_ERROR;
            else if (coef < 0 && INT_MIN - coef > temp->next->coef)
                return MEMORY_ERROR;
            temp->next->coef += coef;
            if (temp->next->coef == 0)
            {
                Term *to_delete = temp->next;
                temp->next = to_delete->next;
                free(to_delete);
                polynom->size--;
            }
            return SUCCESS;
        }
        temp = temp->next;
    }
}

status create_polynom(Polynom *polynom, const char *expression)
{
    int coef, sign = 1, degree;
    const char *symbol = expression;
    if (!polynom || !expression)
        return MEMORY_ERROR;

    init_polynom(polynom);
    while (*symbol != '\0')
    {
        if (*symbol == '-' || *symbol == '+')
        {
            sign = (*symbol == '+') ? 1 : -1;
            coef = 0;
            symbol++;
            if (!isdigit(*symbol) && *symbol != 'x')
                return INPUT_ERROR;
            if (*symbol == 'x')
            {
                coef = 1;
                continue;
            }
            while (*symbol != 0 && isdigit(*symbol))
            {
                if (INT_MAX / 10 < coef)
                    return MEMORY_ERROR;
                coef *= 10;
                if (INT_MAX - (*symbol - '0') < coef)
                    return MEMORY_ERROR;
                coef += *symbol - '0';
                symbol++;
            }
            if (*symbol != 'x')
            {
                if (insert_term(polynom, 0, coef * sign) == MEMORY_ERROR)
                    return MEMORY_ERROR;
                coef = 0;
            }
        }
        else if (*symbol == 'x')
        {
            if (symbol == expression)
                coef = 1;
            degree = 0;
            symbol++;
            if (*symbol != '\0' && *symbol == '^')
            {
                if (!isdigit(*(++symbol)))
                    return INPUT_ERROR;

                while (*symbol != 0 && isdigit(*symbol))
                {
                    if (INT_MAX / 10 < degree)
                        return MEMORY_ERROR;
                    degree *= 10;
                    if (INT_MAX - (*symbol - '0') < degree)
                        return MEMORY_ERROR;
                    degree += *symbol - '0';
                    symbol++;
                }
                if (*symbol == 'x')
                    return INPUT_ERROR;
            }
            else
            {
                degree = 1;
            }
            if (insert_term(polynom, degree, coef * sign) == MEMORY_ERROR)
                return MEMORY_ERROR;
        }
        else if (isdigit(*symbol))
        {
            coef = 0;
            sign = 1;
            while (*symbol != 0 && isdigit(*symbol))
            {
                if (INT_MAX / 10 < coef)
                    return MEMORY_ERROR;
                coef *= 10;
                if (INT_MAX - (*symbol - '0') < coef)
                    return MEMORY_ERROR;
                coef += *symbol - '0';
                symbol++;
            }
            if (*symbol != 'x')
            {
                if (insert_term(polynom, 0, coef) == MEMORY_ERROR)
                    return MEMORY_ERROR;
            }
        }
        else
            return INPUT_ERROR;
    }
    return SUCCESS;
}

// Добавляет в первый полином
status add(Polynom *polynom_1, Polynom *polynom_2)
{
    Term *temp;
    if (!polynom_1 || !polynom_2)
        return MEMORY_ERROR;

    temp = polynom_2->head;
    while (temp)
    {
        if (insert_term(polynom_1, temp->degree, temp->coef) == MEMORY_ERROR)
            return MEMORY_ERROR;
        temp = temp->next;
    }
    return SUCCESS;
}

status sub(Polynom *polynom_1, Polynom *polynom_2)
{
    Term *temp;
    if (!polynom_1 || !polynom_2)
        return MEMORY_ERROR;

    temp = polynom_2->head;
    while (temp)
    {
        if (insert_term(polynom_1, temp->degree, -temp->coef) == MEMORY_ERROR)
            return MEMORY_ERROR;
        temp = temp->next;
    }
    return SUCCESS;
}

status mult(Polynom *polynom_1, Polynom *polynom_2, Polynom *res)
{
    Term *temp_1, *temp_2;
    if (!polynom_1 || !polynom_2 || !res)
        return MEMORY_ERROR;

    init_polynom(res);
    temp_1 = polynom_1->head;
    temp_2 = polynom_2->head;
    while (temp_1)
    {
        temp_2 = polynom_2->head;
        while (temp_2)
        {
            int check_overflov = temp_1->coef * temp_2->coef;
            if (INT_MAX - temp_1->degree < temp_2->degree || (temp_1->coef && check_overflov / temp_1->coef != temp_2->coef))
                return MEMORY_ERROR;
            if (insert_term(res, temp_1->degree + temp_2->degree, temp_1->coef * temp_2->coef) == MEMORY_ERROR)
                return MEMORY_ERROR;
            temp_2 = temp_2->next;
        }
        temp_1 = temp_1->next;
    }
    return SUCCESS;
}

status eval(Polynom *polynom, double x, double *result)
{
    Term *temp;
    int i, last_degree = 0;
    if (!polynom || !result)
        return MEMORY_ERROR;

    temp = polynom->head;
    *result = 0;
    while (temp)
    {
        for (i = 0; i < last_degree - temp->degree; ++i)
        {
            *result *= x;
        }
        *result += temp->coef;
        last_degree = temp->degree;
        temp = temp->next;
    }
    for (i = 0; i < last_degree; ++i)
    {
        *result *= x;
    }
    return SUCCESS;
}

status diff(Polynom *polynom, Polynom *res)
{
    Term *temp, *temp_res;
    if (!polynom || !res)
        return MEMORY_ERROR;

    init_polynom(res);
    temp = polynom->head;
    while (temp && temp->degree > 0)
    {
        if (res->size == 0)
        {
            res->head = (Term *)malloc(sizeof(Term));
            if (!res->head)
                return MEMORY_ERROR;
            res->head->next == NULL;
            res->head->coef = temp->coef * temp->degree;
            res->head->degree = temp->degree - 1;
            temp_res = res->head;
            res->size++;
        }
        else
        {
            temp_res->next = (Term *)malloc(sizeof(Term));
            if (!temp_res->next)
                return MEMORY_ERROR;

            temp_res = temp_res->next;
            temp_res->next = NULL;
            temp_res->coef = temp->coef * temp->degree;
            temp_res->degree = temp->degree - 1;
            res->size++;
        }
        temp = temp->next;
    }
    return SUCCESS;
}

status cmps(Polynom *polynom_1, Polynom *polynom_2, Polynom *result)
{
    Term *temp;
    Polynom res;
    int i, last_degree = 0;
    if (!polynom_1 || !polynom_2 || !result)
        return MEMORY_ERROR;

    init_polynom(result);
    temp = polynom_1->head;
    while (temp)
    {
        for (i = 0; i < last_degree - temp->degree; ++i)
        {
            mult(result, polynom_2, &res);
            destroy_polynom(result);
            *result = res;
            init_polynom(&res);
        }
        insert_term(result, 0, temp->coef);
        last_degree = temp->degree;
        temp = temp->next;
    }
    for (i = 0; i < last_degree; ++i)
    {
        mult(result, polynom_2, &res);
        destroy_polynom(result);
        *result = res;
        init_polynom(&res);
    }
    return SUCCESS;
}

status __mult(Polynom *polynom_1, int degree, int coef, Polynom *res)
{
    Term *temp_1;
    if (!polynom_1 || !res)
        return MEMORY_ERROR;

    init_polynom(res);
    temp_1 = polynom_1->head;
    while (temp_1)
    {
        int check_overflov = temp_1->coef * coef;
        if (INT_MAX - temp_1->degree < degree || (temp_1->coef && check_overflov / temp_1->coef != coef))
            return MEMORY_ERROR;
        if (insert_term(res, temp_1->degree + degree, temp_1->coef * coef) == MEMORY_ERROR)
            return MEMORY_ERROR;
        temp_1 = temp_1->next;
    }
    return SUCCESS;
}

status div_mod(Polynom *dividend, Polynom *divider, Polynom *res)
{
    Term *temp;
    if (!dividend || !divider || !res || !divider->size)
        return MEMORY_ERROR;

    init_polynom(res);
    while (dividend->size && (dividend->head->degree >= divider->head->degree))
    {
        Polynom new_polynom;
        int coef = dividend->head->coef / divider->head->coef,
            degree = dividend->head->degree - divider->head->degree;
        if (dividend->head->coef % divider->head->coef)
        {
            return INPUT_ERROR;
        }
        if (insert_term(res, degree, coef))
            return MEMORY_ERROR;
        init_polynom(&new_polynom);
        if (__mult(divider, degree, coef, &new_polynom))
            return MEMORY_ERROR;
        sub(dividend, &new_polynom);
        destroy_polynom(&new_polynom);
    }

    return SUCCESS;
}

status get_expression(Polynom *polynom_1, Polynom *polynom_2, FILE *input, int *count)
{
    char ch, created = 0;
    String expr;
    status state;
    if (!polynom_1 || !polynom_2 || !input || !count)
        return MEMORY_ERROR;

    *count = 0;
    ch = getc(input);
    if (ch != '(')
        return INPUT_ERROR;
    if (string_init(&expr))
        return MEMORY_ERROR;
    while (!feof(input))
    {
        ch = getc(input);
        if (ch == ',')
        {
            if (expr.len == 0 || created)
            {
                delete_string_content(&expr);
                return INPUT_ERROR;
            }
            if ((state = create_polynom(polynom_1, expr.arr)) == MEMORY_ERROR)
            {
                delete_string_content(&expr);
                return MEMORY_ERROR;
            }
            delete_string_content(&expr);
            if (state == INPUT_ERROR)
                return INPUT_ERROR;
            created = 1;
            *count = 1;
        }
        else if (isdigit(ch) || ch == '+' || ch == '-' || ch == '^' || ch == 'x')
        {
            if (add_char_to_string(&expr, ch))
            {
                delete_string_content(&expr);
                return MEMORY_ERROR;
            }
        }
        else if (ch == ')')
        {
            if (expr.len == 0 && *count == 0)
            {
                delete_string_content(&expr);
                return ZERO_ARGS;
            }
            if (expr.len == 0)
            {
                delete_string_content(&expr);
                return INPUT_ERROR;
            }
            state = create_polynom((created ? polynom_2 : polynom_1), expr.arr);
            delete_string_content(&expr);
            if (state)
                return state;
            *count += 1;
            break;
        }
        else
        {
            delete_string_content(&expr);
            return INPUT_ERROR;
        }
    }
    delete_string_content(&expr);
    return SUCCESS;
}

status print_polynom(Polynom *polynom)
{
    Term *temp;
    if (!polynom)
        return MEMORY_ERROR;

    if (!polynom->size)
        printf("0");

    temp = polynom->head;
    while (temp)
    {
        if (temp != polynom->head)
        {
            if (temp->coef > 0)
                printf(" + ");
            else
                printf(" - ");
        }
        else if (temp->coef < 0)
            printf("-");
        if (!temp->degree)
            printf("%d", abs(temp->coef));
        else
        {
            if (abs(temp->coef) != 1)
                printf("%d", abs(temp->coef));
            printf("x");
            if (temp->degree > 1)
                printf("^%d", temp->degree);
        }
        temp = temp->next;
    }
    putchar('\n');
    return SUCCESS;
}