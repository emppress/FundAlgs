#include "functions.h"

int main()
{
    pthread_t philosophers[NUM_PHILOS];
    philos_arg philosophers_args[NUM_PHILOS];
    int i, j;
    int fork_sem_id, waiter_sem_id;

    fork_sem_id = semget(FORK_SEM_KEY, NUM_FORK, IPC_CREAT | 0666);
    if (fork_sem_id == -1)
    {
        printf("Semget error.\n");
        return IPC_ERROR;
    }

    waiter_sem_id = semget(WAITER_SEM_KEY, 1, IPC_CREAT | 0666);
    if (waiter_sem_id == -1)
    {
        printf("Semget error.\n");
        semctl(fork_sem_id, 0, IPC_RMID);
        return IPC_ERROR;
    }

    if (sem_init(fork_sem_id, NUM_FORK, 1) == IPC_ERROR || sem_init(waiter_sem_id, 1, NUM_PHILOS - 1))
    {
        printf("Semctl error\n");
        semctl(fork_sem_id, 0, IPC_RMID);
        semctl(waiter_sem_id, 0, IPC_RMID);
        return IPC_ERROR;
    }

    for (i = 0; i < NUM_PHILOS; ++i)
    {
        philosophers_args[i].philosopher_id = i;
        philosophers_args[i].fork_sem_id = fork_sem_id;
        philosophers_args[i].waiter_sem_id = waiter_sem_id;

        if (pthread_create(&philosophers[i], NULL, philosopher, &philosophers_args[i]))
        {
            printf("Pthread_create error\n");
            for (j = 0; j < i; j++)
            {
                pthread_cancel(philosophers[j]);
            }
            if (semctl(fork_sem_id, 0, IPC_RMID) == -1)
            {
                printf("semctl error\n");
            }
            if (semctl(waiter_sem_id, 0, IPC_RMID) == -1)
            {
                printf("semctl error\n");
            }
            return PTHREAD_ERROR;
        }
    }

    sleep(60);

    printf("The end...\n");

    for (int i = 0; i < NUM_PHILOS; i++)
    {
        pthread_cancel(philosophers[i]);
    }

    if (semctl(fork_sem_id, 0, IPC_RMID) == -1)
    {
        semctl(waiter_sem_id, 0, IPC_RMID);
        printf("semctl error\n");
        return IPC_ERROR;
    }
    if (semctl(waiter_sem_id, 0, IPC_RMID) == -1)
    {
        printf("semctl error\n");
        return IPC_ERROR;
    }
    return SUCCESS;
}