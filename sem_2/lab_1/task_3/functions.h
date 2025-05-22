#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NUM_PHILOS 5
#define NUM_FORK NUM_PHILOS
#define FORK_SEM_KEY 1234
#define WAITER_SEM_KEY 2345

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR,
    NOT_FOUND,
    FORK_ERROR,
    IPC_ERROR,
    RW_ERROR,
    PTHREAD_ERROR
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

typedef struct PhilosArg
{

    int philosopher_id;
    int fork_sem_id;
    int waiter_sem_id;
} philos_arg;

status sem_init(int id, int nsems, int val);
void *philosopher(void *args);

#endif