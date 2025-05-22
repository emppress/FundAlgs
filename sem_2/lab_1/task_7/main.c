
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    DIR_ERROR,
    NULL_PTR_ERROR
} status;

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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Enter one or more directory names\n");
        return INPUT_ERROR;
    }

    for (size_t i = 1; i < argc; ++i)
    {
        switch (my_ls(argv[i]))
        {
        case INPUT_ERROR:
            printf("ls: cannot access '%s'\n", argv[i]);
            break;

        case DIR_ERROR:
            printf("ls: closedir error '%s'\n", argv[i]);
            break;

        case NULL_PTR_ERROR:
            printf("ls: null ptr string\n");
            break;
        }
    }

    return SUCCESS;
}