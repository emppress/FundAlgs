#include "functions.h"

status sem_init(int id, int val)
{
    union semun arg;
    arg.val = val;

    if (semctl(id, 0, SETVAL, arg) == -1)
        return IPC_ERROR;

    return SUCCESS;
}

status string_to_size_t(const char *str, size_t *result)
{
    char *endinp;
    unsigned long long res;
    if (!str || !result)
        return MEMORY_ERROR;
    if (str[0] == '-')
        return INPUT_ERROR;
    res = strtoull(str, &endinp, 10);
    if (res >= SIZE_MAX)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    *result = (size_t)res;
    return SUCCESS;
}

int take_semaphore(int sem_id, int sem_num)
{
    struct sembuf sb;
    sb.sem_flg = 0;
    sb.sem_num = sem_num;
    sb.sem_op = -1;

    if (semop(sem_id, &sb, 1) == -1)
        return IPC_ERROR;

    return SUCCESS;
}

status release_semaphore(int sem_id, int sem_num)
{
    struct sembuf sb;
    sb.sem_flg = 0;
    sb.sem_num = sem_num;
    sb.sem_op = 1;

    if (semop(sem_id, &sb, 1) == -1)
        return IPC_ERROR;

    return SUCCESS;
}

status woman_wants_to_enter(BathArg *bath_arg)
{
    if (!bath_arg)
        return MEMORY_ERROR;

    if (take_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;

    while (bath_arg->current_count == bath_arg->max_count || bath_arg->state == MAN)
    {
        if (release_semaphore(bath_arg->sem_id, 0))
            return IPC_ERROR;

        if (take_semaphore(bath_arg->sem_id, 0))
            return IPC_ERROR;
    }

    bath_arg->state = WOMAN;
    bath_arg->current_count++;
    printf("Woman enter. State - %s, current count - %zu\n", STATE_TO_STRING(bath_arg->state), bath_arg->current_count);

    if (release_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;

    return SUCCESS;
}

status woman_leaves(BathArg *bath_arg)
{
    if (!bath_arg)
        return MEMORY_ERROR;

    if (take_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;

    bath_arg->current_count--;
    if (bath_arg->current_count == 0)
    {
        bath_arg->state = EMPTY;
    }

    printf("Woman leaves. State - %s, current count - %zu\n", STATE_TO_STRING(bath_arg->state), bath_arg->current_count);
    if (release_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;
    return SUCCESS;
}

status man_wants_to_enter(BathArg *bath_arg)
{
    if (!bath_arg)
        return MEMORY_ERROR;

    if (take_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;

    while (bath_arg->current_count == bath_arg->max_count || bath_arg->state == WOMAN)
    {
        if (release_semaphore(bath_arg->sem_id, 0))
            return IPC_ERROR;

        if (take_semaphore(bath_arg->sem_id, 0))
            return IPC_ERROR;
    }

    bath_arg->state = MAN;
    bath_arg->current_count++;
    printf("Man enter. State - %s, current count - %zu\n", STATE_TO_STRING(bath_arg->state), bath_arg->current_count);

    if (release_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;
    return SUCCESS;
}

status man_leaves(BathArg *bath_arg)
{
    if (!bath_arg)
        return MEMORY_ERROR;

    if (take_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;

    bath_arg->current_count--;
    if (bath_arg->current_count == 0)
    {
        bath_arg->state = EMPTY;
    }

    printf("Man leaves. State - %s, current count - %zu\n", STATE_TO_STRING(bath_arg->state), bath_arg->current_count);
    if (release_semaphore(bath_arg->sem_id, 0))
        return IPC_ERROR;
    return SUCCESS;
}

void *bathroom_block(void *args)
{
    BathArg *bath_arg = (BathArg *)args;

    if (rand() % 2)
    {
        woman_wants_to_enter(bath_arg);
        sleep(rand() % 2);
        woman_leaves(bath_arg);
    }
    else
    {
        man_wants_to_enter(bath_arg);

        sleep(rand() % 2);
        man_leaves(bath_arg);
    }
    pthread_exit(NULL);
}