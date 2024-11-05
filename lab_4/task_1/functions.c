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

int string_equivalence_relation(const String *str_1, const String *str_2)
{
    return strcmp(str_1->arr, str_2->arr) == 0;
}

status string_dup(String *dest, const String *sourse)
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

status read_word(FILE *input, String *str, char *last_ch, char flag_prespace)
{
    char ch;
    if (!input || !str || !last_ch)
        return MEMORY_ERROR;

    if (string_init(str) == MEMORY_ERROR)
        return MEMORY_ERROR;

    ch = getc(input);
    if (flag_prespace)
    {
        while (ch != EOF && isspace(ch))
        {
            ch = getc(input);
        }
    }
    while (ch != EOF && !isspace(ch))
    {
        if (add_char_to_string(str, ch))
            return MEMORY_ERROR;
        ch = getc(input);
    }
    *last_ch = ch;
    return SUCCESS;
}

status hash_table_init(Hash_table *table)
{
    size_t i;
    if (!table)
        return MEMORY_ERROR;

    table->arr = (Node **)calloc(START_HASH_SIZE, sizeof(Node *));
    if (!table->arr)
        return MEMORY_ERROR;
    table->size = START_HASH_SIZE;
    return SUCCESS;
}

status calculate_hash(const String *key, size_t *hash)
{
    size_t i;
    if (!key || !hash)
        return MEMORY_ERROR;

    *hash = 0;
    for (i = 0; i < key->len; ++i)
    {
        *hash = (*hash * 62) % __SIZE_MAX__;
        if (key->arr[i] >= 'A' && key->arr[i] <= 'Z')
            *hash += (key->arr[i] - 'A' + 10);
        else if (key->arr[i] >= 'a' && key->arr[i] <= 'z')
            *hash += (key->arr[i] - 'a' + 36);
        else if (isdigit(key->arr[i]))
            *hash += (key->arr[i] - '0');
        else
            return INPUT_ERROR;

        *hash %= __SIZE_MAX__;
    }
    return SUCCESS;
}

status find(const String *key, const Hash_table *hash_table, Node **found)
{
    size_t hash = 0;
    Node *temp;
    if (!key || !hash_table || !found)
        return MEMORY_ERROR;

    *found = NULL;
    if (calculate_hash(key, &hash))
        return MISSING;
    temp = hash_table->arr[hash % hash_table->size];
    while (temp)
    {
        if (string_equivalence_relation(key, &temp->key))
        {
            *found = temp;
            return SUCCESS;
        }
        temp = temp->next;
    }
    return MISSING;
}

status remove_elem(const String *key, Hash_table *hash_table)
{
    size_t hash = 0;
    Node *temp;
    if (!key || !hash_table)
        return MEMORY_ERROR;

    if (calculate_hash(key, &hash))
        return MISSING;
    hash %= hash_table->size;
    temp = hash_table->arr[hash];
    if (!temp)
        return MISSING;
    if (string_equivalence_relation(key, &temp->key))
    {
        hash_table->arr[hash] = temp->next;
        delete_string_content(&temp->key);
        delete_string_content(&temp->value);
        free(temp);
        return SUCCESS;
    }
    while (temp->next)
    {
        if (string_equivalence_relation(key, &temp->next->key))
        {
            Node *to_del = temp->next;
            temp->next = to_del->next;
            free(to_del);
            return SUCCESS;
        }
        temp = temp->next;
    }
    return MISSING;
}

status add(const String *key, const String *value, Hash_table *hash_table)
{
    size_t full_hash;
    Node *new_node;
    if (!key || !value || !hash_table)
        return MEMORY_ERROR;

    full_hash = 0;
    new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
        return MEMORY_ERROR;

    if (calculate_hash(key, &full_hash))
    {
        free(new_node);
        return INPUT_ERROR;
    }

    if (string_dup(&new_node->key, key) == MEMORY_ERROR)
    {
        free(new_node);
        return MEMORY_ERROR;
    }

    if (string_dup(&new_node->value, value) == MEMORY_ERROR)
    {
        delete_string_content(&new_node->key);
        free(new_node);
        return MEMORY_ERROR;
    }
    new_node->full_hash = full_hash, new_node->next = NULL;
    full_hash %= hash_table->size;

    if (hash_table->arr[full_hash] == NULL)
    {
        hash_table->arr[full_hash] = new_node;
    }
    else if (string_equivalence_relation(&hash_table->arr[full_hash]->key, key))
    {
        delete_string_content(&new_node->key);
        delete_string_content(&new_node->value);
        free(new_node);
        return INPUT_ERROR;
    }
    else
    {
        new_node->next = hash_table->arr[full_hash];
        hash_table->arr[full_hash] = new_node;
        if (resize(hash_table) == MEMORY_ERROR)
            return MEMORY_ERROR;
    }
    return SUCCESS;
}

status resize_add(Node **arr, Node *node, size_t size)
{
    size_t hash = node->full_hash % size;
    if (!node || !arr)
    {
        return MEMORY_ERROR;
    }
    if (arr[hash] != NULL)
    {
        node->next = arr[hash];
        arr[hash] = node;
        return MISSING;
    }

    node->next = NULL;
    arr[hash] = node;
    return SUCCESS;
}

status resize(Hash_table *hash_table)
{
    Node **new_arr;
    char flag = 0;
    size_t new_size = hash_table->size * 2, i;
    if (!hash_table)
        return MEMORY_ERROR;

    new_arr = (Node **)calloc(new_size, sizeof(Node *));
    if (!new_arr)
        return MEMORY_ERROR;

    for (i = 0; i < hash_table->size; ++i)
    {
        Node *temp = hash_table->arr[i], *next;
        while (temp)
        {
            next = temp->next;
            if (resize_add(new_arr, temp, new_size) == MISSING)
                flag = 1;
            temp = next;
        }
    }
    free(hash_table->arr);
    hash_table->arr = new_arr;
    hash_table->size = new_size;
    if (flag)
        return resize(hash_table);
    return SUCCESS;
}

void hash_table_delete(Hash_table *table)
{
    size_t i;
    if (!table)
        return;

    for (i = 0; i < table->size; ++i)
    {
        Node *temp, *next;
        temp = table->arr[i];
        while (temp)
        {
            next = temp->next;
            delete_string_content(&temp->key);
            delete_string_content(&temp->value);
            free(temp);
            temp = next;
        }
    }
    table->size = 0;
    free(table->arr);
    table->arr = NULL;
}

status read_define(Hash_table *hash_table, FILE *input, String *buf, char *last_char)
{
    char ch;
    status state;
    String key, value;
    if (!hash_table || !input || !buf || !last_char)
        return MEMORY_ERROR;

    value.capacity = 0;
    while (!feof(input))
    {
        if (read_word(input, buf, &ch, 0))
        {
            delete_string_content(buf);
            return MEMORY_ERROR;
        }
        if (!strcmp(buf->arr, "#define"))
        {
            if (read_word(input, &key, &ch, 1) == MEMORY_ERROR)
            {
                delete_string_content(buf);
                delete_string_content(&key);
                return MEMORY_ERROR;
            }
            while ((ch = getc(input)) != EOF && ch != '\n')
            {
                if (add_char_to_string(&value, ch))
                {
                    delete_string_content(buf);
                    delete_string_content(&key);
                    delete_string_content(&value);
                    return MEMORY_ERROR;
                }
            }
            if ((state = add(&key, &value, hash_table)))
            {
                delete_string_content(buf);
                delete_string_content(&key);
                delete_string_content(&value);
                if (state == MEMORY_ERROR)
                    return MEMORY_ERROR;
                return INPUT_ERROR;
            }
            delete_string_content(buf);
            delete_string_content(&key);
            delete_string_content(&value);
        }
        else
        {
            break;
        }
    }
    *last_char = ch;
    return SUCCESS;
}

status replace_text(Hash_table *hash_table, FILE *input, FILE *output, String *buf, char ch)
{
    Node *found;
    if (!hash_table || !output || !buf)
        return MEMORY_ERROR;

    while (1)
    {
        if (find(buf, hash_table, &found) == MISSING)
        {
            fprintf(output, "%s", buf->arr);
        }
        else
        {
            fprintf(output, "%s", found->value.arr);
        }
        if (ch != EOF)
            fputc(ch, output);
        delete_string_content(buf);
        if (ch == EOF)
            break;

        if (read_word(input, buf, &ch, 0))
            return MEMORY_ERROR;
    }
    return SUCCESS;
}

status create_out_name(const char *filename, char *new_file_name)
{
    char *ch;
    if (!filename || !new_file_name)
        return MEMORY_ERROR;

    ch = strrchr(filename, '.');
    if (ch == NULL)
        strcpy(new_file_name, "out.txt");
    else
    {
        strcpy(new_file_name, filename);
        ch = strrchr(new_file_name, '.');
        *ch = '\0';
        strcat(new_file_name, "_out.txt");
    }
    return SUCCESS;
}