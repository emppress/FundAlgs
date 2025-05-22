#include "ipc_func.h"

int main(int argc, char *argv[])
{
    int shm_size_id, sem_id, shm_data_id, i;
    char *shared_data, *shared_data_tmp;
    size_t required_size = 0;
    sh_size *shared_sizes;

    if (argc < 2)
    {
        printf("Usage: %s <file_path1> <file_path2> ...\n", argv[0]);
        return INPUT_ERROR;
    }

    shm_size_id = shmget(KEY_SHM_SIZE, sizeof(sh_size), 0666);
    if (shm_size_id == -1)
    {
        printf("shmget\n");
        return IPC_ERROR;
    }

    shared_sizes = (sh_size *)shmat(shm_size_id, NULL, 0);
    if (shared_sizes == (void *)-1)
    {
        printf("shmat\n");
        return IPC_ERROR;
    }

    sem_id = semget(KEY_SEM, 1, 0666);
    if (sem_id == -1)
    {
        printf("semget");
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");

        return IPC_ERROR;
    }

    // Обработка строк

    for (i = 1; i < argc; ++i)
    {
        required_size += strlen(argv[i]) + 1;
    }

    if (semaphore_operation(sem_id, 0, -1) == -1)
    {
        if (shmdt(shared_sizes) == -1)
        {
            printf("shmdt\n");
        }
        printf("semop\n");
        return IPC_ERROR;
    }

    shared_sizes->b_size = required_size;
    shared_sizes->count_files = (size_t)(argc - 1);

    if (semaphore_operation(sem_id, 1, 1) == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");

        printf("semop\n");
        return IPC_ERROR;
    }

    if (semaphore_operation(sem_id, 2, -1) == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");

        printf("semop\n");
        return IPC_ERROR;
    }

    shm_data_id = shmget(KEY_SHM_DATA, required_size, 0666);
    if (shm_size_id == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");

        printf("shmget\n");
        return -1;
    }

    shared_data = (char *)shmat(shm_data_id, NULL, 0);
    if (shared_sizes == (void *)-1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");
        printf("shmat\n");
        return -1;
    }

    shared_data_tmp = shared_data;
    for (i = 1; i < argc; ++i)
    {
        char *ptr = argv[i];
        while (*ptr != '\0')
        {
            *shared_data_tmp++ = *ptr++;
        }
        *shared_data_tmp++ = '\0';
    }

    if (semaphore_operation(sem_id, 3, 1) == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");
        if (shmdt(shared_data) == -1)
            printf("shmdt\n");
        printf("semop\n");
        return IPC_ERROR;
    }

    if (semaphore_operation(sem_id, 2, -1) == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");
        if (shmdt(shared_data) == -1)
            printf("shmdt\n");
        printf("semop\n");
        return IPC_ERROR;
    }

    printf("Server answer: %s\n", shared_data);

    if (semaphore_operation(sem_id, 3, 1) == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");
        if (shmdt(shared_data) == -1)
            printf("shmdt\n");
        printf("semop\n");
        return IPC_ERROR;
    }

    if (shmdt(shared_sizes) == -1)
    {
        shmdt(shared_data);
        printf("shmdt\n");
        return IPC_ERROR;
    }

    if (shmdt(shared_data) == -1)
    {
        printf("shmdt\n");
        return IPC_ERROR;
    }

    return SUCCESS;
}