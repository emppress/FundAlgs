#include "functions.h"

status sem_init(int id, int nsems, int val)
{
    union semun arg;
    int i;

    arg.val = val;

    for (int i = 0; i < nsems; i++)
    {
        if (semctl(id, i, SETVAL, arg) == -1)
            return IPC_ERROR;
    }
    return SUCCESS;
}

status get_permission(int waiter_sem_id)
{
    struct sembuf sb;
    sb.sem_flg = 0, sb.sem_num = 0, sb.sem_op = -1;

    if (semop(waiter_sem_id, &sb, 1) == -1)
        return IPC_ERROR;
    return SUCCESS;
}

status release_permission(int waiter_sem_id)
{
    struct sembuf sb;
    sb.sem_flg = 0, sb.sem_num = 0, sb.sem_op = 1;

    if (semop(waiter_sem_id, &sb, 1) == -1)
        return IPC_ERROR;
    return SUCCESS;
}

status get_fork(int fork_sem_id, int fork_num)
{
    struct sembuf sb;
    sb.sem_flg = 0, sb.sem_num = fork_num, sb.sem_op = -1;
    if (semop(fork_sem_id, &sb, 1) == -1)
        return IPC_ERROR;
    return SUCCESS;
}

status release_fork(int fork_sem_id, int fork_num)
{
    struct sembuf sb;
    sb.sem_flg = 0, sb.sem_num = fork_num, sb.sem_op = 1;
    if (semop(fork_sem_id, &sb, 1) == -1)
        return IPC_ERROR;
    return SUCCESS;
}

void *philosopher(void *args)
{
    philos_arg *p_arg = (philos_arg *)args;
    int id = p_arg->philosopher_id;
    int fork_sem_id = p_arg->fork_sem_id;
    int waiter_sem_id = p_arg->waiter_sem_id;
    int left_fork = id, right_fork = (id + 1) % NUM_PHILOS;

    while (1)
    {
        printf("Философ %d: сидит за столом, думает, чё происходит?!\n", id);
        sleep(3);
        printf("Философ %d: хоч куш!\n", id);

        printf("Философ %d: официант, можно куш??\n", id);
        if (get_permission(waiter_sem_id))
        {
            printf("Thread [%d] IPC_ERROR\n", id);
            pthread_exit(NULL);
        }
        printf("Официант философу %d: кушай!\n", id);

        printf("Философ %d: нужно взять левую вилку!\n", id);
        if (get_fork(fork_sem_id, left_fork))
        {
            printf("Thread [%d] IPC_ERROR\n", id);
            release_permission(waiter_sem_id);
            pthread_exit(NULL);
        }
        printf("Философ %d: схватил левую вилку!\n", id);

        sleep(5); // ловим deadlock

        printf("Философ %d: нужно отжать вилку у соседа справа!\n", id);
        if (get_fork(fork_sem_id, right_fork))
        {
            printf("Thread [%d] IPC_ERROR\n", id);
            release_fork(fork_sem_id, left_fork);
            release_permission(waiter_sem_id);
            pthread_exit(NULL);
        }
        printf("Философ %d: отжал вилку соседа справа!\n", id);

        printf("Философ %d: кушат кушат кушат, кусно кусно кууууууууууууууууууууууууууууууууууууууууууусно!\n", id);
        sleep(rand() % 5);

        printf("Философ %d: покуш, кладу левую вилку!\n", id);
        if (release_fork(fork_sem_id, left_fork))
        {
            printf("Thread [%d] IPC_ERROR\n", id);
        }
        printf("Философ %d: положил левую вилку!\n", id);

        printf("Философ %d: кладу правую вилку!\n", id);
        if (release_fork(fork_sem_id, right_fork))
        {
            printf("Thread [%d] IPC_ERROR\n", id);
        }
        printf("Философ %d: положил правую вилку!\n", id);

        if (release_permission(waiter_sem_id))
        {
            printf("Thread [%d] IPC_ERROR\n", id);
        }
        printf("Философ %d: официант, я покуш, корми остальных!\n", id);
    }
    pthread_exit(NULL);
}