#include "ipc_func.h"

int main()
{
    int shm_size_id, sem_id, shm_data_id, last_slash_idx = -1;
    size_t required_size, count_files, idx;
    union semun arg;
    sh_size *shared_sizes;
    char *shared_data;
    char **dirs;
    unsigned short sem_array[4] = {1, 0, 0, 0};

    shm_size_id = shmget(KEY_SHM_SIZE, sizeof(sh_size), IPC_CREAT | 0666);
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

    sem_id = semget(KEY_SEM, 4, IPC_CREAT | 0666); // size[W,R] data[W,R]
    if (sem_id == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");
        if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
            printf("shmctl(IPC_RMID)\n");
        printf("semget\n");

        return IPC_ERROR;
    }

    arg.array = sem_array;

    if (semctl(sem_id, 0, SETALL, arg) == -1)
    {
        if (shmdt(shared_sizes) == -1)
            printf("shmdt\n");
        if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
            printf("shmctl(IPC_RMID)\n");
        if (delete_semaphore(sem_id))
            printf("semctl\n");

        printf("semctl\n");
        return IPC_ERROR;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    printf("Server started. Waiting for data...\n");
    int flag = 3;
    while (flag--)
    {

        if (semaphore_operation(sem_id, 1, -1))
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            printf("semaphore_operation\n");
            return IPC_ERROR;
        }

        required_size = shared_sizes->b_size > 16 ? shared_sizes->b_size : 16;
        count_files = shared_sizes->count_files;

        dirs = (char **)malloc(sizeof(char *) * count_files);
        if (!dirs)
        {
            printf("Memory error\n");
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");
            return MEMORY_ERROR;
        }

        shm_data_id = shmget(KEY_SHM_DATA, required_size, IPC_CREAT | 0666);
        if (shm_data_id == -1)
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            free(dirs);
            printf("shmget\n");
            return IPC_ERROR;
        }

        shared_data = (char *)shmat(shm_data_id, NULL, 0);
        if (shared_data == (void *)-1)
        {
            printf("shmat\n");
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (shmctl(shm_data_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            free(dirs);
            return IPC_ERROR;
        }

        // разрешили писать в канал и запрос на чтение
        if (semaphore_operation(sem_id, 2, 1) || semaphore_operation(sem_id, 3, -1))
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmdt(shared_data) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (shmctl(shm_data_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");
            free(dirs);

            printf("semaphore_operation\n");
            return IPC_ERROR;
        }

        idx = 0;
        if (required_size > 0)
        {
            dirs[idx++] = shared_data;
        }

        for (size_t i = 1; i < required_size - 1; ++i)
        {
            if (shared_data[i] == '\0')
            {
                if (required_size != i + 1)
                    dirs[idx++] = shared_data + i + 1;
            }
        }

        for (int i = 0; i < count_files; ++i)
        {
            struct stat buffer;
            if (stat(dirs[i], &buffer) != 0)
            {
                printf("Error: File does not exist: %s\n", dirs[i]);
                continue;
            }

            // Проверка, является ли это файлом
            if (!is_regular_file(dirs[i]))
            {
                printf("Error: Not a regular file: %s\n", dirs[i]);
                continue;
            }

            char *last_slash = strrchr(dirs[i], '/');
            if (last_slash == NULL)
            {
                printf("Error: Invalid file path: %s\n", dirs[i]);
                continue;
            }
            *last_slash = '\0';
            switch (my_ls(dirs[i]))
            {
            case INPUT_ERROR:
                printf("cannot access '%s'\n", dirs[i]);
                break;

            case DIR_ERROR:
                printf("closedir error '%s'\n", dirs[i]);
                break;

            case NULL_PTR_ERROR:
                printf("ls: null ptr string\n");
                break;
            }
        }

        free(dirs);
        dirs = NULL;

        // отправка ответа, об успешном завершении
        strcpy(shared_data, "SUCCESS");

        if (semaphore_operation(sem_id, 2, 1))
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmdt(shared_data) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (shmctl(shm_data_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            printf("semaphore_operation\n");
            return IPC_ERROR;
        }

        if (semaphore_operation(sem_id, 3, -1))
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmdt(shared_data) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (shmctl(shm_data_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            printf("semaphore_operation\n");
            return IPC_ERROR;
        }

        if (shmdt(shared_data) == -1)
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmdt(shared_data) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (shmctl(shm_data_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            printf("shmdt\n");
            return IPC_ERROR;
        }
        if (shmctl(shm_data_id, IPC_RMID, NULL) == -1)
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (shmctl(shm_data_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            printf("shmctl(IPC_RMID)\n");
            return IPC_ERROR;
        }

        if (semaphore_operation(sem_id, 0, 1))
        {
            if (shmdt(shared_sizes) == -1)
                printf("shmdt\n");
            if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
                printf("shmctl(IPC_RMID)\n");
            if (delete_semaphore(sem_id))
                printf("semctl\n");

            printf("semaphore_operation\n");
            return IPC_ERROR;
        }
    }

    printf("Server stopped.\n");

    if (shmdt(shared_sizes) == -1)
    {
        if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
            printf("shmctl(IPC_RMID)\n");
        if (delete_semaphore(sem_id))
            printf("semctl\n");
        printf("shmdt\n");
        return IPC_ERROR;
    }
    if (shmctl(shm_size_id, IPC_RMID, NULL) == -1)
    {
        if (delete_semaphore(sem_id))
            printf("semctl\n");
        printf("shmctl(IPC_RMID)\n");
        return IPC_ERROR;
    }
    if (delete_semaphore(sem_id) == -1)
    {
        printf("delete_semaphore\n");
        return IPC_ERROR;
    }

    return SUCCESS;
}