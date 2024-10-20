#include "functions.h"

void print_menu()
{
    puts("____________________________");
    puts("Select an action:");
    puts("1) Find liver;\n2) Change liver;\n3) Add liver;\n4) Delete liver;\n5) Upload to file;\n6) Undo;\n0) End dialog;\n");
    puts("____________________________\n");
    printf(">>> ");
}

status get_liver(Liver *liver)
{
    if (!liver)
        return MEMORY_ERROR;

    printf("Enter surname: ");
    scanf("%s", liver->surname);
    printf("Enter name: ");
    scanf("%s", liver->name);
    printf("Enter patronymic(if no enter '-'): ");
    scanf("%s", liver->patronymic);
    if (liver->patronymic[0] == '-')
        liver->patronymic[0] = '\0';
    printf("Enter birth date(dd.mm.yyyy): ");
    scanf("%d.%d.%d", &liver->birth_date.day, &liver->birth_date.month, &liver->birth_date.year);
    getchar();
    printf("Enter sex(M/W): ");
    scanf("%c", &liver->sex);
    printf("Enter income: ");
    scanf("%lf", &liver->income);

    if (liver_validate(liver))
        return INPUT_ERROR;
    return SUCCESS;
}

status stack_init(Stack *stack)
{
    if (!stack)
        return MEMORY_ERROR;
    stack->head = NULL;
    stack->size = 0;
    return SUCCESS;
}

status stack_push(Stack *stack, char c)
{
    SNode *new_node;
    if (!stack)
        return MEMORY_ERROR;

    new_node = (SNode *)malloc(sizeof(SNode));
    if (!new_node)
        return MEMORY_ERROR;

    new_node->c = c;
    new_node->next = stack->head;
    stack->head = new_node;
    stack->size++;
    return SUCCESS;
}

status stack_pop(Stack *stack, char *top)
{
    SNode *temp;
    if (!stack || !top)
        return MEMORY_ERROR;

    if (!stack->size)
        return MEMORY_ERROR;

    *top = stack->head->c;
    temp = stack->head;
    stack->head = temp->next;
    free(temp);
    stack->size--;
    return SUCCESS;
}

status stack_destroy(Stack *stack)
{
    SNode *temp, *next;
    if (!stack)
        return MEMORY_ERROR;

    temp = stack->head;
    while (temp)
    {
        next = temp->next;
        free(temp);
        temp = next;
    }
    stack->head = NULL;
    stack->size = 0;
    return SUCCESS;
}