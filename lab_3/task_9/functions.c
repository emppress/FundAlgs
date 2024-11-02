#include "functions.h"

#define max(a, b) (a > b) ? (a) : (b)

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
    return SUCCESS;
}

status string_scan(String *str)
{
    char ch;
    if (!str)
        return MEMORY_ERROR;

    if (string_init(str))
        return MEMORY_ERROR;
    while ((ch = getchar()) != EOF && !isspace(ch))
    {
        if (add_char_to_string(str, ch))
            return MEMORY_ERROR;
    }
    if (add_char_to_string(str, '\0'))
        return MEMORY_ERROR;
    ungetc(ch, stdin);
    return SUCCESS;
}

void print_menu()
{
    puts("____________________________");
    puts("Select an action:");
    puts("1) Number of repetitions of a word;\n2) Find most frequent words;\n3) Find longest word;\n4) Find shortest word;\n5) Find depth;\n6) Upload to file;\n7) Load from file;\n0) End dialog;\n");
    puts("____________________________\n");
    printf(">>> ");
}

void free_buf()
{
    while (getchar() != '\n')
        ;
}

status init_tree(Tree *tree)
{
    if (!tree)
        return MEMORY_ERROR;
    tree->comparator = string_compare;
    tree->root = NULL;
    return SUCCESS;
}

void delete_tree_kernel(Node *root)
{
    if (!root)
        return;
    delete_tree_kernel(root->left);
    delete_tree_kernel(root->right);
    delete_string_content(&root->data);
    free(root);
}

status delete_tree(Tree *tree)
{
    if (!tree)
        return MEMORY_ERROR;
    delete_tree_kernel(tree->root);
    tree->comparator = NULL;
    tree->root = NULL;
    return SUCCESS;
}

status add_node(Tree *tree, String *word)
{
    Node **temp = NULL;
    int res_cmp;
    if (!tree || !word)
        return MEMORY_ERROR;

    temp = &tree->root;
    while (*temp != NULL)
    {
        if ((res_cmp = tree->comparator(word, &(*temp)->data)) < 0)
        {
            temp = &(*temp)->left;
        }
        else if (res_cmp == 0)
            break;
        else if (res_cmp > 0)
        {
            temp = &(*temp)->right;
        }
    }
    if (*temp != NULL)
    {
        (*temp)->count_repeats++;
    }
    else
    {
        *temp = (Node *)malloc(sizeof(Node));
        if (!temp)
            return MEMORY_ERROR;
        (*temp)->left = (*temp)->right = NULL;
        (*temp)->count_repeats = 1;
        if (string_dup(&(*temp)->data, word) == MEMORY_ERROR)
        {
            free(*temp);
            *temp = NULL;
            return MEMORY_ERROR;
        }
        return SUCCESS;
    }
}

status build_tree_from_file(Tree *tree, const char *separators, FILE *input)
{
    char c;
    String word;
    if (!tree || !separators || !input)
        return MEMORY_ERROR;

    if (string_init(&word))
        return MEMORY_ERROR;
    while (c = getc(input))
    {
        if (c == EOF || strchr(separators, c))
        {
            if (word.len)
            {
                if (add_char_to_string(&word, '\0'))
                {
                    delete_string_content(&word);
                    return MEMORY_ERROR;
                }
                if (add_node(tree, &word) == MEMORY_ERROR)
                    return MEMORY_ERROR;
            }
            delete_string_content(&word);
            if (c == EOF)
                break;
        }
        else if (!isspace(c))
        {
            if (add_char_to_string(&word, c))
            {
                delete_string_content(&word);
                return MEMORY_ERROR;
            }
        }
    }
    return SUCCESS;
}

status find_string(const String *str, Tree *tree, Node **found)
{
    Node *temp;
    int res_cmp;
    if (!str || !tree || !found)
        return MEMORY_ERROR;

    temp = tree->root;
    while (temp != NULL)
    {
        if ((res_cmp = tree->comparator(str, &temp->data)) == 0)
        {
            *found = temp;
            return SUCCESS;
        }
        if (res_cmp < 0)
            temp = temp->left;
        else
            temp = temp->right;
    }
    return MISSING;
}

status find_longest_word(Tree *tree, String *str)
{
    Node *temp = NULL;
    if (!tree || !str)
        return MEMORY_ERROR;

    temp = tree->root;
    if (!temp)
        return MISSING;
    while (temp->right != NULL)
        temp = temp->right;

    if (string_dup(str, &temp->data))
        return MEMORY_ERROR;
    return SUCCESS;
}

status find_shortest_word(Tree *tree, String *str)
{
    Node *temp = NULL;
    if (!tree || !str)
        return MEMORY_ERROR;

    temp = tree->root;
    if (!temp)
        return MISSING;
    while (temp->left != NULL)
        temp = temp->left;

    if (string_dup(str, &temp->data))
        return MEMORY_ERROR;
    return SUCCESS;
}

int max_depth(Node *root)
{
    int depth, l, r;
    if (root)
    {
        l = max_depth(root->left);
        r = max_depth(root->right);
        depth = max(l, r);
        return 1 + depth;
    }
    return -1;
}

status list_init(List *list)
{
    if (!list)
        return MEMORY_ERROR;
    list->head = NULL;
    list->size = 0;
    return SUCCESS;
}

status list_add(List *list, Node *node)
{
    Lnode *temp = NULL;
    if (!list || !node)
        return MEMORY_ERROR;

    Lnode *new_lnode = (Lnode *)malloc(sizeof(Lnode));
    if (!new_lnode)
        return MEMORY_ERROR;

    new_lnode->data = node;
    new_lnode->next = NULL;

    if (list->size == 0 || node->count_repeats >= list->head->data->count_repeats)
    {
        new_lnode->next = list->head;
        list->head = new_lnode;
        list->size++;
        return SUCCESS;
    }

    temp = list->head;
    while (temp->next && node->count_repeats < temp->next->data->count_repeats)
    {
        temp = temp->next;
    }
    new_lnode->next = temp->next;
    temp->next = new_lnode;
    list->size++;
    return SUCCESS;
}

status list_destroy(List *list)
{
    Lnode *temp, *next;
    if (!list)
        return MEMORY_ERROR;

    temp = list->head;
    while (temp)
    {
        next = temp->next;
        free(temp);
        temp = next;
    }
    list->head = NULL;
    list->size = 0;
    return SUCCESS;
}

status tree_to_list(Node *root, List *list)
{
    if (!list || !root)
        return MEMORY_ERROR;

    if (list_add(list, root) == MEMORY_ERROR)
        return MEMORY_ERROR;
    if (root->left)
    {
        if (tree_to_list(root->left, list) == MEMORY_ERROR)
            return MEMORY_ERROR;
    }
    if (root->right)
    {
        if (tree_to_list(root->right, list) == MEMORY_ERROR)
            return MEMORY_ERROR;
    }
    return SUCCESS;
}

status print_n_most_frequent_words(Tree *tree, int n)
{
    List list;
    Lnode *temp = NULL;
    size_t i;
    if (!tree)
        return MEMORY_ERROR;

    list_init(&list);
    if (tree_to_list(tree->root, &list) == MEMORY_ERROR)
    {
        list_destroy(&list);
        return MEMORY_ERROR;
    }

    if (list.size < n)
    {
        printf("There are only %zu different words in the text:\n", list.size);
        n = (int)list.size;
    }
    if (n == -1)
        n = (int)list.size;
    temp = list.head;
    for (i = 0; i < n; ++i)
    {
        printf("%zu) %s, count: %zu;\n", i + 1, temp->data->data.arr, temp->data->count_repeats);
        temp = temp->next;
    }
    list_destroy(&list);
    return SUCCESS;
}

status upload_tree_to_file_kernel(Node *root, FILE *file, char separator)
{
    size_t i;
    if (!root || !file)
        return MEMORY_ERROR;

    for (i = 0; i < root->count_repeats; ++i)
    {
        fprintf(file, "%s%c", root->data.arr, separator);
    }
    if (root->left)
        upload_tree_to_file_kernel(root->left, file, separator);
    if (root->right)
        upload_tree_to_file_kernel(root->right, file, separator);
    return SUCCESS;
}

status upload_tree_to_file(Tree *tree, FILE *file, char separator)
{
    if (!tree || !file)
        return MEMORY_ERROR;

    if (tree->root == NULL)
        return SUCCESS;
    if (upload_tree_to_file_kernel(tree->root, file, separator) == MEMORY_ERROR)
        return MEMORY_ERROR;

    return SUCCESS;
}
