#include "functions.h"

int main(int argc, char *argv[])
{
    pthread_t people[COUNT_PEOPLE];
    BathArg bath_args;
    size_t i;

    if (argc != 2)
    {
        printf("Enter the maximum number of people in the bathroom\n");
        return INPUT_ERROR;
    }

    if (string_to_size_t(argv[1], &bath_args.max_count) || bath_args.max_count == 0)
    {
        printf("Input max error\n");
        return INPUT_ERROR;
    }

    bath_args.current_count = 0;
    bath_args.state = EMPTY;

    bath_args.sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (bath_args.sem_id == -1)
    {
        printf("Semget error\n");
        return IPC_ERROR;
    }

    if (sem_init(bath_args.sem_id, 1) == IPC_ERROR)
    {
        printf("Semctl error\n");
        semctl(bath_args.sem_id, 0, IPC_RMID);
        return IPC_ERROR;
    }

    for (i = 0; i < COUNT_PEOPLE; ++i)
    {
        if (pthread_create(&people[i], NULL, bathroom_block, &bath_args) != 0)
        {
            printf("Thread create error\n");
        }
    }

    for (i = 0; i < COUNT_PEOPLE; ++i)
    {
        pthread_join(people[i], NULL);
    }

    if (semctl(bath_args.sem_id, 0, IPC_RMID) == -1)
    {
        printf("Semctl error\n");
        return IPC_ERROR;
    }
    return SUCCESS;
}