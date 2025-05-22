#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <sys/types.h>
#include <limits.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR,
    NOT_FOUND,
    FORK_ERROR,
    RW_ERROR
} status;

status mask(const char *file_name, const char *_mask, size_t *_count);
status find_string(char const **file_paths, size_t file_count, const char *pattern, char *found_in, char *flag_found);
status xor_blocks(const char *filename, int N, uint64_t *result);
status copy_file(const char *file_path, size_t n);
status string_to_size_t(const char *str, size_t *result);

#endif