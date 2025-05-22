#ifndef _IPC_FUNC_
#define _IPC_FUNC_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define KEY_SHM_SIZE 2000
#define KEY_SHM_DATA 3000
#define KEY_SEM 4321

typedef struct SH_SIZE
{
    size_t b_size, count_files;
} sh_size;

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    DIR_ERROR,
    NULL_PTR_ERROR,
    IPC_ERROR,
    MEMORY_ERROR
} status;

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
                           /* Linux specific part: */
    struct seminfo *__buf; /* buffer for IPC_INFO */
};
#endif

int is_regular_file(const char *path);

status semaphore_operation(int sem_id, int sem_num, int sem_op);

status delete_semaphore(int sem_id);

status my_ls(const char *directory);

#endif