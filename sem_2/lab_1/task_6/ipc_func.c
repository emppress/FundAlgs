#include "ipc_func.h"

int is_regular_file(const char *path)
{
    struct stat path_stat;
    if (!path)
        return 0;

    if (stat(path, &path_stat) == -1)
    {
        return 0;
    }
    return S_ISREG(path_stat.st_mode);
}

status semaphore_operation(int sem_id, int sem_num, int sem_op)
{
    struct sembuf sem_buf;
    sem_buf.sem_num = sem_num;
    sem_buf.sem_op = sem_op;
    sem_buf.sem_flg = 0;

    if (semop(sem_id, &sem_buf, 1) == -1)
        return IPC_ERROR;

    return SUCCESS;
}

status delete_semaphore(int sem_id)
{
    if (semctl(sem_id, 0, IPC_RMID, NULL) == -1)
        return IPC_ERROR;

    return SUCCESS;
}

status my_ls(const char *directory)
{
    DIR *dir;
    struct dirent *entry;

    if (!directory)
        return NULL_PTR_ERROR;

    dir = opendir(directory);
    if (!dir)
        return INPUT_ERROR;

    printf("%s:\n", directory);
    while ((entry = readdir(dir)))
    {
        // Пропускаем текущий и родительский каталоги
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        switch (entry->d_type)
        {
        case DT_REG:
            printf("Обычный файл: ");
            break;

        case DT_DIR:
            printf("Директория: ");
            break;

        case DT_LNK:
            printf("Символическая ссылка: ");
            break;

        case DT_CHR:
            printf("Символьное устройство: ");
            break;

        case DT_BLK:
            printf("Блочное устройство: ");
            break;

        case DT_FIFO:
            printf("FIFO (именованный канал): ");
            break;

        case DT_SOCK:
            printf("Сокет: ");
            break;
        default:
            printf("Неизвестный тип файла (d_type): ");
        }
        printf("%s %lu\n", entry->d_name, entry->d_ino);
    }
    printf("\n");

    if (closedir(dir) == -1)
        return DIR_ERROR;

    return SUCCESS;
}