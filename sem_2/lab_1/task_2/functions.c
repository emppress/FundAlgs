#include "functions.h"

status string_to_uint32(const char *str, u_int32_t *result)
{
    char *endinp;
    unsigned long res;
    if (!str || !result)
        return MEMORY_ERROR;
    res = strtoul(str, &endinp, 16);
    if (res > UINT32_MAX)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    *result = (u_int32_t)res;
    return SUCCESS;
}

status string_to_size_t(const char *str, size_t *result)
{
    char *endinp;
    unsigned long long res;
    if (!str || !result)
        return MEMORY_ERROR;
    res = strtoull(str, &endinp, 10);
    if (res >= SIZE_MAX)
        return INPUT_ERROR;
    if (*endinp != '\0')
        return INPUT_ERROR;
    *result = (size_t)res;
    return SUCCESS;
}

status mask(const char *file_name, const char *_mask, size_t *_count)
{
    u_int32_t mask, num32;
    size_t count = 0;
    int tst;
    FILE *file;
    if (!file_name || !_mask)
        return MEMORY_ERROR;

    if (strlen(_mask) > 4 || string_to_uint32(_mask, &mask))
        return INPUT_ERROR;

    file = fopen(file_name, "rb");
    if (!file)
        return FILE_OPEN_ERROR;

    while ((tst = fread(&num32, sizeof(uint32_t), 1, file)) == 1)
    {
        if (mask & num32 == mask)
            count++;
    }
    fclose(file);

    *_count = count;
    return SUCCESS;
}

status find_string(const char **file_paths, size_t file_count, const char *pattern, char *found_in, char *flag_found)
{
    char *shared;
    char ch;
    size_t i, idx = 0, pattern_size = strlen(pattern);
    pid_t pid;
    int shm_id;

    if (!file_paths || !pattern || !found_in || !flag_found)
        return MEMORY_ERROR;

    shm_id = shmget(IPC_PRIVATE, (file_count + 1) * sizeof(char), IPC_CREAT | 0666);

    if (shm_id == -1)
        return MEMORY_ERROR;

    // Присоединяем разделяемую память
    shared = (char *)shmat(shm_id, NULL, 0);
    if (shared == (void *)-1)
    {
        shmctl(shm_id, IPC_RMID, NULL);
        return MEMORY_ERROR;
    }

    // Инициализация разделяемой памяти
    memset(shared, 0, file_count + 1);

    // Создаём дочерние процессы для поиска в каждом файле
    for (i = 0; i < file_count; ++i)
    {
        pid = fork();
        if (pid == 0)
        { // Дочерний процесс
            FILE *file = fopen(file_paths[i], "r");
            if (!file)
                exit(FILE_OPEN_ERROR);

            while ((ch = fgetc(file)) != EOF)
            {
                if (pattern[idx] == ch)
                {
                    if (idx == pattern_size - 1)
                    {
                        shared[file_count] = 1;
                        shared[i] = 1;
                        break;
                    }
                    idx++;
                }
                else
                {
                    fseek(file, -idx, SEEK_CUR);
                    idx = 0;
                }
            }

            fclose(file);

            exit(SUCCESS);
        }
        else if (pid < 0)
        {
            shmdt(shared);
            shmctl(shm_id, IPC_RMID, NULL);
            return FORK_ERROR;
        }
    }

    // Ожидание завершения всех дочерних процессов
    for (int i = 0; i < file_count; i++)
    {
        wait(NULL);
    }

    // Копируем результаты из разделяемой памяти
    *flag_found = shared[file_count];

    memcpy(found_in, shared, file_count * sizeof(char));

    // Освобождаем ресурсы
    shmdt(shared);
    shmctl(shm_id, IPC_RMID, NULL);

    return SUCCESS;
}

status xor_blocks(const char *filename, int N, uint64_t *result)
{
    FILE *fp;
    uint64_t block_size_bits = 1ULL << N;
    uint64_t xor_sum = 0, current_block_value = 0;
    int i, bit_count = 0, current_bit;
    uint8_t byte;

    if (!filename || !result)
        return MEMORY_ERROR;

    if (N < 2 || N > 6)
        return INPUT_ERROR;

    fp = fopen(filename, "rb");
    if (!fp)
        return FILE_OPEN_ERROR;

    while (fread(&byte, 1, 1, fp) == 1)
    {
        for (i = 0; i < 8; ++i)
        {
            current_bit = (byte >> i) & 1; // Получаем текущий бит

            current_block_value |= (current_bit << bit_count); // Добавляем бит в блок

            bit_count++;

            if (bit_count == block_size_bits)
            {
                xor_sum ^= current_block_value;
                current_block_value = 0;
                bit_count = 0;
            }
        }
    }

    // Обрабатываем последний неполный блок
    if (bit_count > 0)
    {
        xor_sum ^= current_block_value;
    }

    fclose(fp);
    *result = xor_sum;
    return SUCCESS;
}

status _copy_file(const char *source_filename, const char *dest_filename)
{
    int source_fd, dest_fd;
    char buffer[4096];
    ssize_t bytes_read, bytes_written;

    if (!source_filename || !dest_filename)
        return MEMORY_ERROR;

    source_fd = open(source_filename, O_RDONLY);
    if (source_fd == -1)
    {
        return FILE_OPEN_ERROR;
    }

    dest_fd = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dest_fd == -1)
    {
        close(source_fd);
        return FILE_OPEN_ERROR;
    }

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0)
    {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            close(source_fd);
            close(dest_fd);
            return RW_ERROR;
        }
    }

    close(source_fd);
    close(dest_fd);

    if (bytes_read == -1)
    {
        close(source_fd);
        close(dest_fd);
        return RW_ERROR;
    }

    return SUCCESS;
}

status copy_file(const char *file_path, size_t n)
{
    pid_t pid;
    size_t i, base_len;
    status st, st_ans = SUCCESS;
    int state;
    char *dest_filename, *dot;

    if (!file_path)
        return MEMORY_ERROR;

    dest_filename = (char *)malloc((strlen(file_path) + 36) * sizeof(char));
    if (!dest_filename)
        return MEMORY_ERROR;

    dot = strrchr(file_path, '.');

    for (i = 0; i < n; ++i)
    {
        pid = fork();
        if (pid == -1)
        {
            free(dest_filename);
            return FORK_ERROR;
        }

        else if (pid == 0)
        {
            if (!dot)
            {
                // Расширение отсутствует
                sprintf(dest_filename, "%s.%zu", file_path, i);
            }
            else
            {
                // Расширение присутствует
                base_len = dot - file_path;
                sprintf(dest_filename, "%.*s%zu%s", (int)base_len, file_path, i, dot);
            }

            st = _copy_file(file_path, dest_filename);
            free(dest_filename);
            exit(st);
        }
    }

    for (i = 0; i < n; i++)
    {
        wait(&state);
        if (state != SUCCESS)
        {
            st_ans = state;
        }
    }
    free(dest_filename);
    return st_ans;
}