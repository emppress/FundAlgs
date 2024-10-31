#include "functions.h"

#define max(a, b) (a > b) ? (a) : (b)

int string_comparator(const char *str_1, const char *str_2)
{
    int len_1, len_2;
    len_1 = strlen(str_1);
    len_2 = strlen(str_2);
    if (len_1 != len_2)
    {
        return len_1 - len_2;
    }
    return strcmp(str_1, str_2);
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
    tree->comparator = string_comparator;
    tree->root = NULL;
    return SUCCESS;
}

void delete_tree_kernel(Node *root)
{
    if (!root)
        return;
    delete_tree_kernel(root->left);
    delete_tree_kernel(root->right);
    free(root->data);
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

status add_node(Tree *tree, char *word, size_t size_word)
{
    Node **temp = NULL;
    int res_cmp;
    if (!tree || !word)
        return MEMORY_ERROR;

    temp = &tree->root;
    while (*temp != NULL)
    {
        if ((res_cmp = tree->comparator(word, (*temp)->data)) < 0)
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
        (*temp)->data = (char *)malloc((size_word + 1) * sizeof(char));
        if (!(*temp)->data)
            return MEMORY_ERROR;
        strcpy((*temp)->data, word);
        return SUCCESS;
    }
}

status build_tree_from_file(Tree *tree, const char *separators, FILE *input)
{
    char c, word[BUFSIZ];
    size_t size_word = 0;
    if (!tree || !separators || !input)
        return MEMORY_ERROR;

    while (c = getc(input))
    {
        if (c == EOF || strchr(separators, c))
        {
            if (size_word)
            {
                word[size_word] = '\0';
                if (add_node(tree, word, size_word) == MEMORY_ERROR)
                    return MEMORY_ERROR;
            }
            size_word = 0;
            if (c == EOF)
                break;
        }
        else if (!isspace(c))
        {
            word[size_word++] = c;
        }
    }
    return SUCCESS;
}

status find_string(const char *str, Tree *tree, Node **found)
{
    Node *temp;
    int res_cmp;
    if (!str || !tree || !found)
        return MEMORY_ERROR;

    temp = tree->root;
    while (temp != NULL)
    {
        if ((res_cmp = tree->comparator(str, temp->data)) == 0)
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

status find_longest_word(Tree *tree, char *str)
{
    Node *temp = NULL;
    if (!tree || !str)
        return MEMORY_ERROR;

    temp = tree->root;
    if (!temp)
        return MISSING;
    while (temp->right != NULL)
        temp = temp->right;

    strcpy(str, temp->data);
    return SUCCESS;
}

status find_shortest_word(Tree *tree, char *str)
{
    Node *temp = NULL;
    if (!tree || !str)
        return MEMORY_ERROR;

    temp = tree->root;
    if (!temp)
        return MISSING;
    while (temp->left != NULL)
        temp = temp->left;

    strcpy(str, temp->data);
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
        printf("%zu) %s, count: %zu;\n", i + 1, temp->data->data, temp->data->count_repeats);
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
        fprintf(file, "%s%c", root->data, separator);
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
