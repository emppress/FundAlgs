#include "functions.h"

void init_tree(Node *root)
{
    root->brother = NULL;
    root->prev = NULL;
    root->son = NULL;
}

status build_tree(Node *root, char *str)
{
    char c;
    Node *temp;
    temp = root;
    for (int i = 0; i < strlen(str); ++i)
    {
        if (str[i] == '(')
        {
            temp->son = (Node *)malloc(sizeof(Node));
            temp->son->prev = temp;
            temp = temp->son;
            temp->brother = NULL;
            temp->son = NULL;
        }
        else if (str[i] == ')')
        {
            temp = temp->prev;
        }
        else if (str[i] == ',')
        {
            temp->brother = (Node *)malloc(sizeof(Node));
            temp->brother->prev = temp->prev;
            temp = temp->brother;
            temp->son = NULL;
            temp->brother = NULL;
        }
        else if (isalpha(str[i]))
        {
            temp->data = str[i];
        }
    }
}

void print_tree(Node *root, int level)
{
    if (!root)
        return;
    for (int i = 0; i < level; i++)
    {
        printf("\t|");
    }
    printf("%c\n", root->data);
    if (root->son)
        print_tree(root->son, level + 1);
    if (root->brother)
        print_tree(root->brother, level);
}